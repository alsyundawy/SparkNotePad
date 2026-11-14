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

#include "FocusWatcher.h"
#include "QuickFindWidget.h"
#include "ScintillaNext.h"
#include "FadingIndicator.h"
#include "utils/ExtendedSearchParser.h"
#include "ui_QuickFindWidget.h"

#include <QKeyEvent>
#include <QLineEdit>
#include <QShortcut>
#include <QScrollBar>
#include <algorithm>

QuickFindWidget::QuickFindWidget(QWidget *parent) : QFrame(parent), ui(std::make_unique<Ui::QuickFindWidget>()) {
    ui->setupUi(this);

    // Move the focus to the line edit widget
    setFocusProxy(ui->lineEdit);

    ui->lineEdit->installEventFilter(this);

    const auto *fw = new FocusWatcher(ui->lineEdit);
    connect(fw, &FocusWatcher::focusOut, this, &QuickFindWidget::focusOut);
    connect(fw, &FocusWatcher::focusIn, this, &QuickFindWidget::focusIn);

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &QuickFindWidget::returnPressed);

    // Any changes need to trigger a new search
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &QuickFindWidget::performNewSearch);
    connect(ui->buttonMatchCase, &QToolButton::toggled, this, &QuickFindWidget::performNewSearch);
    connect(ui->buttonWholeWord, &QToolButton::toggled, this, &QuickFindWidget::performNewSearch);
    connect(ui->buttonRegexp, &QToolButton::toggled, this, &QuickFindWidget::performNewSearch);
    connect(ui->buttonFuzzy, &QToolButton::toggled, this, &QuickFindWidget::performNewSearch);

    setStyleSheet(
        QStringLiteral("QFrame#QuickFindWidget {"
                       "  background-color: rgba(32, 36, 42, 0.94);"
                       "  border: 1px solid rgba(255, 255, 255, 0.16);"
                       "  border-radius: 8px;"
                       "}"
                       "QLineEdit {"
                       "  border-radius: 4px;"
                       "  padding: 3px 6px;"
                       "}"
                       "QToolButton {"
                       "  border-radius: 4px;"
                       "  padding: 2px 5px;"
                       "}"));
}

QuickFindWidget::~QuickFindWidget() = default;

void QuickFindWidget::setEditor(ScintillaNext *newEditor) {
    if (this->editor != nullptr) {
        disconnect(this->editor, &ScintillaNext::resized, this, &QuickFindWidget::positionWidget);
    }

    if (newEditor != nullptr) {
        connect(newEditor, &ScintillaNext::resized, this, &QuickFindWidget::positionWidget);
    }

    this->editor = newEditor;

    if (finder == nullptr) {
        finder = std::make_unique<Finder>(newEditor);
        finder->options().wrapAround = true;  // Always wrap the search
    } else {
        finder->setEditor(newEditor);
    }

    if (editor != nullptr) {
        initializeEditorIndicator();
        positionWidget();
    }
}

bool QuickFindWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        const QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // Use escape key to close the quick find widget
        if (keyEvent->key() == Qt::Key_Escape) {
            clearHighlights();
            clearCachedMatches();
            hide();
            if (editor != nullptr) {
                editor->grabFocus();
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void QuickFindWidget::setSearchContextColorBad() {
    setSearchContextColor(QStringLiteral("red"));
}

void QuickFindWidget::setSearchContextColorGood() {
    setSearchContextColor(QStringLiteral("blue"));
}

void QuickFindWidget::performNewSearch() {
    clearHighlights();
    clearCachedMatches();
    ui->lblInfo->hide();

    // Early out
    if (searchText().isEmpty()) {
        setSearchContextColorGood();
        return;
    }

    prepareSearch();
    finder->forEachMatch([this](int start, int end) {
        matches.append(qMakePair(start, end));
        return qMax(start + 1, end);
    });

    if (matches.empty()) {
        setSearchContextColorBad();
    } else {
        setSearchContextColorGood();
    }

    highlightMatches();
    navigateToNextMatch(false);
}

void QuickFindWidget::highlightMatches() {
    qInfo(Q_FUNC_INFO);

    if (editor == nullptr) {
        return;
    }

    editor->setIndicatorCurrent(indicator);
    for (const auto &[start, end] : matches) {
        editor->indicatorFillRange(start, end - start);
    }
}

void QuickFindWidget::showWrapIndicator() {
    FadingIndicator::showPixmap(editor, QStringLiteral(":/icons/wrapindicator.svg"));
}

void QuickFindWidget::navigateToNextMatch(bool skipCurrent) {
    qInfo(Q_FUNC_INFO);

    // Early out if there are no matches
    if (matches.empty()) {
        ui->lblInfo->hide();
        return;
    }

    if (currentMatchIndex != -1) {
        currentMatchIndex++;
        if (currentMatchIndex >= matches.length()) {
            currentMatchIndex = 0;
        }
    } else {
        int startPos = INVALID_POSITION;
        if (skipCurrent) {
            startPos = static_cast<int>(editor->selectionEnd());
        } else {
            startPos = static_cast<int>(editor->selectionStart());
        }

        const auto it = std::ranges::lower_bound(matches, startPos, std::less<int>{},
                                                 [](const QPair<int, int> &pair) { return pair.first; });

        if (it != matches.end()) {
            currentMatchIndex = std::distance(matches.begin(), it);
        } else {
            // Wrap back around
            currentMatchIndex = 0;
        }
    }

    // Search wrapped around
    if (currentMatchIndex == 0) {
        showWrapIndicator();
    }

    goToCurrentMatch();
}

void QuickFindWidget::navigateToPrevMatch() {
    qInfo(Q_FUNC_INFO);

    // Early out if there are no matches
    if (matches.empty()) {
        ui->lblInfo->hide();
        return;
    }

    if (currentMatchIndex != -1) {
        currentMatchIndex--;
        if (currentMatchIndex < 0) {
            currentMatchIndex = matches.length() - 1;
        }
    } else {
        qWarning("navigateToPrevMatch() with no valid index yet");
        return;
    }

    // Search wrapped around
    if (currentMatchIndex == matches.length() - 1) {
        showWrapIndicator();
    }

    goToCurrentMatch();
}

void QuickFindWidget::goToCurrentMatch() {
    if (editor == nullptr || currentMatchIndex < 0 || currentMatchIndex >= matches.length()) {
        return;
    }

    editor->setSel(matches[currentMatchIndex].first, matches[currentMatchIndex].second);
    editor->verticalCentreCaret();

    ui->lblInfo->show();
    ui->lblInfo->setText(tr("%L1/%L2").arg(currentMatchIndex + 1).arg(matches.length()));
}

Scintilla::FindOption QuickFindWidget::computeSearchFlags() const {
    using enum Scintilla::FindOption;
    Scintilla::FindOption searchFlags = {};

    if (ui->buttonMatchCase->isChecked()) {
        searchFlags |= MatchCase;
    }

    if (ui->buttonWholeWord->isChecked()) {
        searchFlags |= WholeWord;
    }

    if (ui->buttonRegexp->isChecked()) {
        searchFlags |= RegExp;
    }

    return searchFlags;
}

void QuickFindWidget::setSearchContextColor(const QString &color) {
    ui->lineEdit->setStyleSheet(QStringLiteral("border: 1px solid %1; padding: 2px;").arg(color));
}

void QuickFindWidget::initializeEditorIndicator() {
    if (editor == nullptr) {
        return;
    }

    indicator = editor->allocateIndicator(QStringLiteral("quick_find"));

    editor->indicSetFore(indicator, 0xFF8000);
    editor->indicSetStyle(indicator, INDIC_FULLBOX);
    editor->indicSetOutlineAlpha(indicator, 150);
    editor->indicSetAlpha(indicator, 50);
    editor->indicSetUnder(indicator, true);
}

QString QuickFindWidget::searchText() const {
    const QString raw = ui->lineEdit->text();
    if (!ui->buttonRegexp->isChecked() && raw.contains(QLatin1Char('\\'))) {
        return ExtendedSearchParser::parse(raw);
    }
    return raw;
}

void QuickFindWidget::positionWidget() {
    if (editor == nullptr) {
        return;
    }

    int usableWidth = editor->width();

    // Account for the scrollbar as the quick find will be underneath if the scrollbar is visible
    if (editor->verticalScrollBar() && editor->verticalScrollBar()->isVisible()) {
        usableWidth -= editor->verticalScrollBar()->width();
    }

    const auto position = QPoint(usableWidth - width(), 0);

    move(editor->mapTo(parentWidget(), position));
}

void QuickFindWidget::prepareSearch() {
    finder->options().text = searchText();
    finder->options().flags = computeSearchFlags();
    finder->options().fuzzy = ui->buttonFuzzy->isChecked();
}

void QuickFindWidget::focusIn() {
    ui->lineEdit->selectAll();
    ui->lblInfo->hide();
    performNewSearch();
}

void QuickFindWidget::focusOut() {
    clearHighlights();
    clearCachedMatches();
    hide();
}

void QuickFindWidget::returnPressed() {
    if (QGuiApplication::keyboardModifiers() & Qt::ShiftModifier) {
        navigateToPrevMatch();
    } else {
        navigateToNextMatch(true);
    }
}

void QuickFindWidget::clearHighlights() {
    if (editor == nullptr) {
        return;
    }

    editor->setIndicatorCurrent(indicator);
    editor->indicatorClearRange(0, editor->length());
}

void QuickFindWidget::clearCachedMatches() {
    matches.clear();
    currentMatchIndex = -1;
}
