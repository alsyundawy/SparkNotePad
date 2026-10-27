/*
 *
 * This file is part of SparkNotepad.
 *
 * Copyright 2018-2026 Justin Dailey
 * Copyright 2025-2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotepad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Notepad Next is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Notepad Next.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "FindReplaceDialog.h"
#include "ApplicationSettings.h"
#include "ui_FindReplaceDialog.h"
#include "features/ThemeManager.h"

#include <QStatusBar>
#include <QLineEdit>
#include <QAbstractItemView>
#include <QListView>
#include <QShortcut>
#include <QClipboard>
#include <QGuiApplication>
#include <QScreen>

#include "ScintillaNext.h"
#include "MainWindow.h"
#include "BookMarkDecorator.h"
#include "utils/RegexSafetyEngine.h"

static void convertToExtended(QString &str) {
    str.replace(R"(\r)", "\r");
    str.replace(R"(\n)", "\n");
    str.replace(R"(\t)", "\t");
    str.replace(R"(\0)", "\0");
    str.replace(R"(\\)", R"(\)");
    // Extended search escapes for CR, LF, TAB, NUL, and backslash
}

FindReplaceDialog::FindReplaceDialog(ISearchResultsHandler *searchResults, MainWindow *window)
    : QDialog(window, Qt::Dialog),
      ui(std::make_unique<Ui::FindReplaceDialog>()),
      searchResultsHandler(searchResults),
      finder(std::make_unique<Finder>(window ? window->currentEditor() : nullptr)) {
    qInfo(Q_FUNC_INFO);

    // Turn off the help button on the dialog
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);

    // Get the current editor, and keep up the reference
    if (window) {
        setEditor(window->currentEditor());
        connect(window, &MainWindow::editorActivated, this, &FindReplaceDialog::setEditor);
    }

    tabBar = new QTabBar(this);
    tabBar->setDrawBase(false);
    tabBar->addTab(tr("Find"));
    tabBar->addTab(tr("Replace"));
    tabBar->addTab(tr("Mark"));
    tabBar->setExpanding(false);
    qobject_cast<QVBoxLayout *>(layout())->insertWidget(0, tabBar, 0, Qt::AlignLeft);
    connect(tabBar, &QTabBar::currentChanged, this, &FindReplaceDialog::changeTab);

    statusBar = new QStatusBar(this);
    statusBar->setSizeGripEnabled(false);  // the dialog has one already
    qobject_cast<QVBoxLayout *>(layout())->insertWidget(-1, statusBar);

    // Disable auto completion and cap dropdown items
    ui->comboFind->setCompleter(nullptr);
    ui->comboReplace->setCompleter(nullptr);
    ui->comboFind->setMaxVisibleItems(10);
    ui->comboReplace->setMaxVisibleItems(10);

    auto *findListView = new QListView(ui->comboFind);
    findListView->setUniformItemSizes(true);
    findListView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboFind->setView(findListView);

    auto *replaceListView = new QListView(ui->comboReplace);
    replaceListView->setUniformItemSizes(true);
    replaceListView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboReplace->setView(replaceListView);

    setStyleSheet(ThemeManager::instance()->getThemeStyleSheet());
    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this,
            [this]() { setStyleSheet(ThemeManager::instance()->getThemeStyleSheet()); });
    connect(ThemeManager::instance(), &ThemeManager::paletteChanged, this,
            [this]() { setStyleSheet(ThemeManager::instance()->getThemeStyleSheet()); });

    // If the selection changes highlight the text
    connect(ui->comboFind, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            ui->comboFind->lineEdit(), &QLineEdit::selectAll);
    connect(ui->comboReplace, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            ui->comboReplace->lineEdit(), &QLineEdit::selectAll);

    // Force focus on the find text box
    connect(this, &FindReplaceDialog::windowActivated, [this]() {
        ui->comboFind->setFocus();
        ui->comboFind->lineEdit()->selectAll();
    });

    connect(this, &QDialog::rejected, [this]() {
        statusBar->clearMessage();
        savePosition();
    });

    connect(ui->radioRegexSearch, &QRadioButton::toggled, this, [this](bool checked) {
        if (checked) {
            ui->checkBoxMatchWholeWord->setChecked(false);
        }

        ui->checkBoxMatchWholeWord->setDisabled(checked);
        ui->checkBoxRegexMatchesNewline->setEnabled(checked);
    });

    connect(ui->radioOnLosingFocus, &QRadioButton::toggled, this, &FindReplaceDialog::adjustOpacityWhenLosingFocus);
    connect(ui->radioAlways, &QRadioButton::toggled, this, &FindReplaceDialog::adjustOpacityAlways);
    connect(ui->transparency, &QGroupBox::toggled, this, &FindReplaceDialog::transparencyToggled);

    connect(ui->buttonFind, &QPushButton::clicked, this, &FindReplaceDialog::find);
    connect(ui->buttonCount, &QPushButton::clicked, this, &FindReplaceDialog::count);
    connect(ui->buttonFindAllInCurrent, &QPushButton::clicked, this, [this]() {
        prepareToPerformSearch();

        searchResultsHandler->newSearch(findString());

        findAllInCurrentDocument();

        searchResultsHandler->completeSearch();

        close();
    });
    connect(ui->buttonFindAllInDocuments, &QPushButton::clicked, this, [this]() {
        prepareToPerformSearch();

        searchResultsHandler->newSearch(findString());

        findAllInDocuments();

        searchResultsHandler->completeSearch();

        close();
    });
    connect(ui->buttonReplace, &QPushButton::clicked, this, &FindReplaceDialog::replace);
    connect(ui->buttonReplaceAll, &QPushButton::clicked, this, &FindReplaceDialog::replaceAll);
    connect(ui->buttonReplaceAllInDocuments, &QPushButton::clicked, this, [this]() {
        prepareToPerformSearch(true);

        QString replaceText = replaceString();

        if (ui->radioExtendedSearch->isChecked()) {
            convertToExtended(replaceText);
        }

        int totalReplaced = 0;
        ScintillaNext *current_editor = editor;

        if (const auto *mainWin = qobject_cast<MainWindow *>(parent())) {
            for (ScintillaNext *targetEd : mainWin->editors()) {
                setEditor(targetEd);
                totalReplaced += finder->replaceAll(replaceText);
            }
        }

        setEditor(current_editor);

        showMessage(tr("Replaced %Ln matches", "", totalReplaced), MessageType::Success);
    });
    connect(ui->buttonClose, &QPushButton::clicked, this, &FindReplaceDialog::close);
    connect(ui->buttonMarkAll, &QPushButton::clicked, this, &FindReplaceDialog::markAll);
    connect(ui->buttonClearAllMarks, &QPushButton::clicked, this, &FindReplaceDialog::clearAllMarks);
    connect(ui->buttonCopyMarkedText, &QPushButton::clicked, this, &FindReplaceDialog::copyMarkedText);

    const auto findPrevious = [this]() {
        const int curTab = tabBar->currentIndex();
        if (curTab == FIND_TAB || curTab == REPLACE_TAB) {
            performFind(SearchDirection::Backwards);
        }
    };
    new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_Return), this, this, findPrevious, Qt::WidgetWithChildrenShortcut);
    new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_Enter), this, this, findPrevious, Qt::WidgetWithChildrenShortcut);

    loadSettings();

    changeTab(tabBar->currentIndex());
}

FindReplaceDialog::~FindReplaceDialog() = default;

void FindReplaceDialog::setFindString(const QString &string) {
    ui->comboFind->setCurrentText(string);
    ui->comboFind->lineEdit()->selectAll();
}

void FindReplaceDialog::setTab(int tab) {
    tabBar->setCurrentIndex(tab);
    changeTab(tab);
}

bool FindReplaceDialog::event(QEvent *dialogEvent) {
    if (dialogEvent->type() == QEvent::WindowActivate) {
        emit windowActivated();
    } else if (dialogEvent->type() == QEvent::WindowDeactivate) {
        emit windowDeactivated();
    }

    return QDialog::event(dialogEvent);
}

void FindReplaceDialog::showEvent(QShowEvent *showEvt) {
    qInfo(Q_FUNC_INFO);

    if (!isFirstTime)
        restorePosition();

    isFirstTime = false;

    QDialog::showEvent(showEvt);
}

void FindReplaceDialog::closeEvent(QCloseEvent *closeEvt) {
    saveSettings();

    QDialog::closeEvent(closeEvt);
}

static void updateComboList(QComboBox *comboBox, const QString &text) {
    // Block the signals while it is manipulated
    const QSignalBlocker blocker(comboBox);

    // Remove it if it is in the list, add it to beginning, and select it
    comboBox->removeItem(comboBox->findText(text));
    comboBox->insertItem(0, text);
    comboBox->setCurrentIndex(0);
}

void FindReplaceDialog::updateFindList(const QString &text) {
    if (!text.isEmpty())
        updateComboList(ui->comboFind, text);
}

void FindReplaceDialog::updateReplaceList(const QString &text) {
    updateComboList(ui->comboReplace, text);
}

void FindReplaceDialog::find() {
    performFind(ui->checkBoxBackwardsDirection->isChecked() ? SearchDirection::Backwards : SearchDirection::Forwards);
}

void FindReplaceDialog::performFind(SearchDirection direction) {
    qInfo(Q_FUNC_INFO);

    prepareToPerformSearch();

    FindResult result = direction == SearchDirection::Forwards ? finder->findNext() : finder->findPrev();

    if (result) {
        if (result.wrapped) {
            showMessage(tr("The end of the document has been reached. Found 1st occurrence from the top."),
                        MessageType::Success);
        }

        // Defensive handling for zero-length match boundaries
        if (result.range.cpMin == result.range.cpMax) {
            qWarning() << "0 length match at" << result.range.cpMin;
        }

        editor->goToRange(result.range);
    } else {
        showMessage(tr("No matches found."), MessageType::Error);
    }
}

void FindReplaceDialog::findAllInCurrentDocument() {
    qInfo(Q_FUNC_INFO);

    bool firstMatch = true;

    QString text = findString();

    finder->options().text = std::move(text);
    finder->forEachMatch([&firstMatch, this](int start, int end) {
        // Only add the file entry if there was a valid search result
        if (firstMatch) {
            searchResultsHandler->newFileEntry(editor);
            firstMatch = false;
        }

        const auto line = static_cast<int>(editor->lineFromPosition(start));
        const auto lineStartPosition = static_cast<int>(editor->positionFromLine(line));
        const auto lineEndPosition = static_cast<int>(editor->lineEndPosition(line));
        const int startPositionFromBeginning = start - lineStartPosition;
        const int endPositionFromBeginning = end - lineStartPosition;
        QString lineText = editor->get_text_range(lineStartPosition, lineEndPosition);

        searchResultsHandler->newResultsEntry(lineText, line, startPositionFromBeginning, endPositionFromBeginning);

        return end;
    });
}

void FindReplaceDialog::findAllInDocuments() {
    qInfo(Q_FUNC_INFO);

    ScintillaNext *current_editor = editor;

    if (const auto *mainWin = qobject_cast<MainWindow *>(parent())) {
        for (ScintillaNext *ed : mainWin->editors()) {
            setEditor(ed);
            findAllInCurrentDocument();
        }
    }

    setEditor(current_editor);
}

void FindReplaceDialog::replace() {
    qInfo(Q_FUNC_INFO);

    prepareToPerformSearch();

    QString replaceText = replaceString();

    if (ui->radioExtendedSearch->isChecked()) {
        convertToExtended(replaceText);
    }

    if (finder->replaceSelectionIfMatch(replaceText)) {
        showMessage(tr("1 occurrence was replaced"), MessageType::Info);
    }

    FindResult result = finder->findNext();

    if (result) {
        editor->goToRange(result.range);
    } else {
        showMessage(tr("No more occurrences were found"), MessageType::Error);
        ui->comboFind->setFocus();
        ui->comboFind->lineEdit()->selectAll();
    }
}

void FindReplaceDialog::replaceAll() {
    qInfo(Q_FUNC_INFO);

    prepareToPerformSearch(true);

    QString replaceText = replaceString();

    if (ui->radioExtendedSearch->isChecked()) {
        convertToExtended(replaceText);
    }

    int replacedCount = finder->replaceAll(replaceText);
    showMessage(tr("Replaced %Ln matches", "", replacedCount), MessageType::Success);
}

void FindReplaceDialog::count() {
    qInfo(Q_FUNC_INFO);

    prepareToPerformSearch();

    int total = finder->count();

    showMessage(tr("Found %Ln matches", "", total), MessageType::Success);
}

void FindReplaceDialog::setEditor(ScintillaNext *edit) {
    this->editor = edit;

    finder->setEditor(edit);
}

void FindReplaceDialog::performNextSearch() {
    FindResult result = finder->findNext();

    if (result)
        editor->goToRange(result.range);
}

void FindReplaceDialog::performPrevSearch() {
    FindResult result = finder->findPrev();

    if (result)
        editor->goToRange(result.range);
}

void FindReplaceDialog::adjustOpacity(int value) {
    qInfo(Q_FUNC_INFO);

    setWindowOpacity(value * .01);
}

void FindReplaceDialog::transparencyToggled(bool on) {
    qInfo(Q_FUNC_INFO);

    if (on) {
        if (ui->radioOnLosingFocus->isChecked()) {
            adjustOpacityWhenLosingFocus(true);
            adjustOpacityAlways(false);
        } else {
            adjustOpacityWhenLosingFocus(false);
            adjustOpacityAlways(true);
        }
    } else {
        adjustOpacityWhenLosingFocus(false);
        adjustOpacityAlways(false);
        adjustOpacity(100);
    }
}

void FindReplaceDialog::adjustOpacityWhenLosingFocus(bool checked) {
    qInfo(Q_FUNC_INFO);

    if (checked) {
        connect(this, &FindReplaceDialog::windowActivated, [this]() { this->adjustOpacity(100); });
        connect(this, &FindReplaceDialog::windowDeactivated,
                [this]() { this->adjustOpacity(ui->horizontalSlider->value()); });
        adjustOpacity(100);
    } else {
        disconnect(this, &FindReplaceDialog::windowActivated, nullptr, nullptr);
        disconnect(this, &FindReplaceDialog::windowDeactivated, nullptr, nullptr);
    }
}

void FindReplaceDialog::adjustOpacityAlways(bool checked) {
    qInfo(Q_FUNC_INFO);

    if (checked) {
        connect(ui->horizontalSlider, &QSlider::valueChanged, this, &FindReplaceDialog::adjustOpacity);
        adjustOpacity(ui->horizontalSlider->value());
    } else {
        disconnect(ui->horizontalSlider, &QSlider::valueChanged, this, &FindReplaceDialog::adjustOpacity);
    }
}

void FindReplaceDialog::changeTab(int index) {
    if (index == FIND_TAB) {
        ui->labelReplaceWith->hide();
        ui->comboReplace->hide();
        ui->labelReplaceWith->setMaximumHeight(0);
        ui->comboReplace->setMaximumHeight(0);
        // The combo box is hidden and focus policy adjusted so it cannot be tabbed to
        ui->comboReplace->setFocusPolicy(Qt::NoFocus);

        ui->buttonFind->show();

        ui->buttonReplace->hide();
        ui->buttonReplaceAll->hide();
        ui->buttonReplaceAllInDocuments->hide();
        ui->buttonMarkAll->hide();
        ui->buttonClearAllMarks->hide();
        ui->buttonCopyMarkedText->hide();

        ui->buttonCount->show();
        ui->buttonFindAllInCurrent->show();
        ui->buttonFindAllInDocuments->show();

        ui->checkBoxBookmarkLine->hide();
        ui->checkBoxPurgeForEachSearch->hide();

        // Backwards regex supported
        ui->checkBoxWrapAround->setEnabled(true);
    } else if (index == REPLACE_TAB) {
        ui->labelReplaceWith->setMaximumHeight(QWIDGETSIZE_MAX);
        ui->comboReplace->setMaximumHeight(QWIDGETSIZE_MAX);
        ui->labelReplaceWith->show();
        ui->comboReplace->show();
        ui->comboReplace->setFocusPolicy(Qt::StrongFocus);  // Reset its focus policy

        ui->buttonFind->show();

        ui->buttonReplace->show();
        ui->buttonReplaceAll->show();
        ui->buttonReplaceAllInDocuments->show();
        ui->buttonMarkAll->hide();
        ui->buttonClearAllMarks->hide();
        ui->buttonCopyMarkedText->hide();

        ui->buttonCount->hide();
        ui->buttonFindAllInCurrent->hide();
        ui->buttonFindAllInDocuments->hide();

        ui->checkBoxBookmarkLine->hide();
        ui->checkBoxPurgeForEachSearch->hide();

        // Backwards regex supported
        ui->checkBoxWrapAround->setEnabled(true);
    } else if (index == MARK_TAB) {
        ui->labelReplaceWith->hide();
        ui->comboReplace->hide();
        ui->labelReplaceWith->setMaximumHeight(0);
        ui->comboReplace->setMaximumHeight(0);
        ui->comboReplace->setFocusPolicy(Qt::NoFocus);

        ui->buttonFind->hide();

        ui->buttonReplace->hide();
        ui->buttonReplaceAll->hide();
        ui->buttonReplaceAllInDocuments->hide();
        ui->buttonCount->hide();
        ui->buttonFindAllInCurrent->hide();
        ui->buttonFindAllInDocuments->hide();

        ui->buttonMarkAll->show();
        ui->buttonClearAllMarks->show();
        ui->buttonCopyMarkedText->show();

        ui->checkBoxBookmarkLine->show();
        ui->checkBoxPurgeForEachSearch->show();

        // Backwards regex supported
        ui->checkBoxWrapAround->setEnabled(true);
    }

    ui->comboFind->setFocus();
    ui->comboFind->lineEdit()->selectAll();
}

QString FindReplaceDialog::findString() const {
    return ui->comboFind->currentText();
}

QString FindReplaceDialog::replaceString() const {
    return ui->comboReplace->currentText();
}

void FindReplaceDialog::setSearchResultsHandler(ISearchResultsHandler *searchResults) {
    this->searchResultsHandler = searchResults;
}

void FindReplaceDialog::prepareToPerformSearch(bool isReplace) {
    qInfo(Q_FUNC_INFO);

    QString findText = findString();

    updateFindList(findText);
    if (isReplace) {
        QString replaceText = replaceString();
        updateReplaceList(replaceText);
    }

    statusBar->clearMessage();

    if (ui->radioRegexSearch->isChecked()) {
        QString redosWarning;
        if (!RegexSafetyEngine::isSafeRegex(findText, &redosWarning)) {
            statusBar->showMessage(redosWarning, 5000);
            qWarning("Regex ReDoS guard triggered: %s", qUtf8Printable(redosWarning));
            return;
        }
    }

    if (ui->radioExtendedSearch->isChecked()) {
        convertToExtended(findText);
    }

    finder->options() = findOptions();
}

void FindReplaceDialog::loadSettings() {
    qInfo(Q_FUNC_INFO);

    ApplicationSettings settings;

    settings.beginGroup("FindReplaceDialog");

    restoreGeometry(settings.value("geometry").toByteArray());

    // Defensively clamp the restored size in case a previously persisted geometry
    // is larger than the available screen (e.g. from an older version that let the
    // dialog grow to fit a very long recent search string). This lets users recover
    // automatically without having to edit or delete sparknotepad.ini.
    if (const QScreen *screen = QGuiApplication::primaryScreen()) {
        const QSize available = screen->availableGeometry().size();
        const QSize current = size();
        const QSize clamped = current.boundedTo(available);
        if (clamped != current)
            resize(clamped);
    }

    ui->comboFind->addItems(settings.value("RecentSearchList").toStringList());
    ui->comboReplace->addItems(settings.value("RecentReplaceList").toStringList());

    ui->checkBoxBackwardsDirection->setChecked(settings.value("Backwards").toBool());
    ui->checkBoxMatchWholeWord->setChecked(settings.value("WholeWord").toBool());
    ui->checkBoxMatchCase->setChecked(settings.value("MatchCase").toBool());
    ui->checkBoxWrapAround->setChecked(settings.value("WrapAround", true).toBool());

    if (settings.contains("SearchMode")) {
        const QString searchMode = settings.value("SearchMode").toString();
        if (searchMode == "normal")
            ui->radioNormalSearch->setChecked(true);
        else if (searchMode == "extended")
            ui->radioExtendedSearch->setChecked(true);
        else
            ui->radioRegexSearch->setChecked(true);
    }
    ui->checkBoxRegexMatchesNewline->setChecked(settings.value("DotMatchesNewline").toBool());

    ui->transparency->setChecked(settings.value("TransparencyUsed").toBool());
    if (ui->transparency->isChecked()) {
        ui->horizontalSlider->setValue(settings.value("Transparency", 70).toInt());

        if (settings.value("TransparencyMode").toString() == "focus") {
            ui->radioOnLosingFocus->setChecked(true);
        } else {
            ui->radioAlways->setChecked(true);
        }
    }

    settings.endGroup();
}

void FindReplaceDialog::saveSettings() const {
    qInfo(Q_FUNC_INFO);

    ApplicationSettings settings;

    settings.beginGroup("FindReplaceDialog");
    settings.remove("");  // clear out any previous keys

    settings.setValue("geometry", saveGeometry());

    QStringList recentSearches;
    for (int i = 0; i < ui->comboFind->count(); ++i) {
        recentSearches << ui->comboFind->itemText(i);
    }
    settings.setValue("RecentSearchList", recentSearches);

    recentSearches.clear();
    for (int i = 0; i < ui->comboReplace->count(); ++i) {
        recentSearches << ui->comboReplace->itemText(i);
    }
    settings.setValue("RecentReplaceList", recentSearches);

    settings.setValue("Backwards", ui->checkBoxBackwardsDirection->isChecked());
    settings.setValue("WholeWord", ui->checkBoxMatchWholeWord->isChecked());
    settings.setValue("MatchCase", ui->checkBoxMatchCase->isChecked());
    settings.setValue("WrapAround", ui->checkBoxWrapAround->isChecked());

    if (ui->radioNormalSearch->isChecked())
        settings.setValue("SearchMode", "normal");
    else if (ui->radioExtendedSearch->isChecked())
        settings.setValue("SearchMode", "extended");
    else if (ui->radioRegexSearch->isChecked())
        settings.setValue("SearchMode", "regex");
    settings.setValue("DotMatchesNewline", ui->checkBoxRegexMatchesNewline->isChecked());

    settings.setValue("TransparencyUsed", ui->transparency->isChecked());
    if (ui->transparency->isChecked()) {
        settings.setValue("Transparency", ui->horizontalSlider->value());
        settings.setValue("TransparencyMode", ui->radioOnLosingFocus->isChecked() ? "focus" : "always");
    }

    settings.endGroup();
}

void FindReplaceDialog::savePosition() {
    qInfo(Q_FUNC_INFO);

    lastClosedPosition = pos();
}

void FindReplaceDialog::restorePosition() {
    qInfo(Q_FUNC_INFO);

    ApplicationSettings settings;

    if (settings.centerSearchDialog()) {
        const QPoint centerPoint = parentWidget()->geometry().center();
        move(centerPoint - rect().center());
    } else {
        move(lastClosedPosition);
    }
}

FindOptions FindReplaceDialog::findOptions() const {
    using enum Scintilla::FindOption;
    FindOptions options;

    options.text = findString();

    if (ui->checkBoxMatchWholeWord->isChecked())
        options.flags |= WholeWord;

    if (ui->checkBoxMatchCase->isChecked())
        options.flags |= MatchCase;

    if (ui->radioRegexSearch->isChecked()) {
        options.flags |= RegExp;
        if (ui->checkBoxRegexMatchesNewline->isChecked()) {
            options.flags |= Cxx11RegEx;
        }
    }

    options.wrapAround = ui->checkBoxWrapAround->isChecked();

    return options;
}

int FindReplaceDialog::ensureMarkIndicator() {
    int markIndicator = editor->allocateIndicator(QStringLiteral("find_mark_highlight"));
    editor->indicSetFore(markIndicator, 0xFFCC00);
    editor->indicSetStyle(markIndicator, INDIC_FULLBOX);
    editor->indicSetOutlineAlpha(markIndicator, 200);
    editor->indicSetAlpha(markIndicator, 100);
    editor->indicSetUnder(markIndicator, true);

    return markIndicator;
}

BookMarkDecorator *FindReplaceDialog::bookMarkDecorator() const {
    if (auto *decorator = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);
        decorator && decorator->isEnabled()) {
        return decorator;
    }

    return nullptr;
}

void FindReplaceDialog::clearAllBookmarks() const {
    if (auto *decorator = bookMarkDecorator()) {
        decorator->clearAllBookmarks();
    }
}

void FindReplaceDialog::markAll() {
    qInfo(Q_FUNC_INFO);

    prepareToPerformSearch();
    int markIndicator = ensureMarkIndicator();

    editor->setIndicatorCurrent(markIndicator);

    if (ui->checkBoxPurgeForEachSearch->isChecked()) {
        editor->indicatorClearRange(0, editor->length());
        clearAllBookmarks();
    }

    BookMarkDecorator *bmDecorator = nullptr;
    if (ui->checkBoxBookmarkLine->isChecked()) {
        bmDecorator = this->bookMarkDecorator();
    }

    int matchCount = 0;
    finder->forEachMatch([this, &matchCount, bmDecorator](int start, int end) {
        editor->indicatorFillRange(start, end - start);
        matchCount++;

        if (bmDecorator) {
            const auto line = static_cast<int>(editor->lineFromPosition(start));
            if (!bmDecorator->isBookmarkSet(line)) {
                bmDecorator->addBookmark(line);
            }
        }

        return end;
    });

    showMessage(tr("Mark: %Ln match in entire file", "", matchCount), MessageType::Success);
}

void FindReplaceDialog::clearAllMarks() {
    qInfo(Q_FUNC_INFO);

    int markIndicator = ensureMarkIndicator();
    editor->setIndicatorCurrent(markIndicator);
    editor->indicatorClearRange(0, editor->length());
    clearAllBookmarks();
    showMessage(tr("All marks cleared"), MessageType::Success);
}

void FindReplaceDialog::copyMarkedText() {
    qInfo(Q_FUNC_INFO);

    int markIndicator = ensureMarkIndicator();

    QStringList markedTexts;
    int pos = 0;
    const auto len = static_cast<int>(editor->length());

    while (pos < len) {
        if (editor->indicatorValueAt(markIndicator, pos)) {
            const auto end = static_cast<int>(editor->indicatorEnd(markIndicator, pos));
            markedTexts << QString::fromUtf8(editor->get_text_range(pos, end));
            pos = end;
        } else {
            pos++;
        }
    }

    if (markedTexts.isEmpty()) {
        showMessage(tr("No marks to copy"), MessageType::Error);
        return;
    }

    QGuiApplication::clipboard()->setText(markedTexts.join("\n"));
    showMessage(tr("Copied %Ln marked text(s)", "", static_cast<int>(markedTexts.size())), MessageType::Success);
}

void FindReplaceDialog::showMessage(const QString &message, MessageType type) {
    const bool dark = ThemeManager::instance()->isDarkActive();
    QString resolvedColor;
    using enum MessageType;
    switch (type) {
        case Success:
            resolvedColor = dark ? QStringLiteral("#3fb950") : QStringLiteral("#1a7f37");
            break;
        case Error:
            resolvedColor = dark ? QStringLiteral("#f85149") : QStringLiteral("#cf222e");
            break;
        case Info:
            resolvedColor = dark ? QStringLiteral("#58a6ff") : QStringLiteral("#0969da");
            break;
    }
    statusBar->setStyleSheet(QStringLiteral("color: %1; font-weight: 500;").arg(resolvedColor));
    statusBar->showMessage(message);
}
