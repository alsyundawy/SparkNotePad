/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad3 status bar customization architecture.
 * Sources: rizonesoft/Notepad3 (src/Notepad3.ini, Statusbar Settings)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "EditorInfoStatusBar.h"
#include "EncodingDetector.h"
#include "CharacterInfo.h"
#include "MainWindow.h"
#include "StatusLabel.h"
#include "StatusBarCustomizerDialog.h"

#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QSettings>

EditorInfoStatusBar::EditorInfoStatusBar(QMainWindow *window) : QStatusBar(window) {
    // Set up the status bar fields
    docType = new StatusLabel();
    addWidget(docType, 1);
    docType->setToolTip(tr("Document Language / Syntax Scheme"));

    docSize = new StatusLabel(180);
    addPermanentWidget(docSize, 0);
    docSize->setToolTip(tr("Document Size & Line Count"));

    docPos = new StatusLabel(230);
    addPermanentWidget(docPos, 0);
    docPos->setToolTip(tr("Cursor Position & Selection"));

    unicodeChar = new StatusLabel(90);
    addPermanentWidget(unicodeChar, 0);
    unicodeChar->setToolTip(tr("Character Unicode Code Point at Cursor (Click for full details)"));
    connect(qobject_cast<StatusLabel *>(unicodeChar), &StatusLabel::clicked, this, [this]() {
        if (const auto *mw = qobject_cast<const MainWindow *>(this->window())) {
            if (auto *act = mw->findChild<QAction *>("actionShowCharacterInfo")) {
                act->trigger();
            }
        }
    });

    occurrences = new StatusLabel(80);
    addPermanentWidget(occurrences, 0);
    occurrences->setToolTip(tr("Marked Occurrences Count"));
    occurrences->setText(tr("Occ: -"));

    eolFormat = new StatusLabel(100);
    addPermanentWidget(eolFormat, 0);
    eolFormat->setToolTip(tr("End of Line Format (Right-click to convert)"));
    eolFormat->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(qobject_cast<StatusLabel *>(eolFormat), &StatusLabel::customContextMenuRequested, this,
            [this](const QPoint &pos) { emit customContextMenuRequestedForEOLLabel(eolFormat->mapToGlobal(pos)); });

    unicodeType = new StatusLabel(110);
    addPermanentWidget(unicodeType, 0);
    unicodeType->setToolTip(tr("Encoding / BOM (Click to view or convert)"));
    connect(qobject_cast<StatusLabel *>(unicodeType), &StatusLabel::clicked, this, [this]() {
        if (const auto *mw = qobject_cast<const MainWindow *>(this->window())) {
            if (auto *act = mw->findChild<QAction *>("actionShowEncodingInfo")) {
                act->trigger();
            }
        }
    });

    overType = new StatusLabel(30);
    addPermanentWidget(overType, 0);
    overType->setToolTip(tr("Typing Mode: Insert (INS) / Overtype (OVR)"));

    const auto *w = qobject_cast<const MainWindow *>(window);
    connect(w, &MainWindow::editorActivated, this, &EditorInfoStatusBar::connectToEditor);

    connect(qobject_cast<StatusLabel *>(overType), &StatusLabel::clicked, w, [this, w]() {
        if (auto *editor = w->currentEditor()) {
            editor->editToggleOvertype();
            updateOverType(editor);
        }
    });

    loadSettings();
}

QLabel *EditorInfoStatusBar::labelForField(Field field) const {
    switch (field) {
        case FieldLanguage:
            return docType;
        case FieldDocSize:
            return docSize;
        case FieldDocPos:
            return docPos;
        case FieldUnicodeChar:
            return unicodeChar;
        case FieldOccurrences:
            return occurrences;
        case FieldEol:
            return eolFormat;
        case FieldEncoding:
            return unicodeType;
        case FieldOverType:
            return overType;
        default:
            return nullptr;
    }
}

void EditorInfoStatusBar::loadSettings() {
    QSettings settings;
    settings.beginGroup(QStringLiteral("StatusBarFields"));
    m_fieldVisibility[FieldLanguage] = settings.value(QStringLiteral("Language"), true).toBool();
    m_fieldVisibility[FieldDocSize] = settings.value(QStringLiteral("DocSize"), true).toBool();
    m_fieldVisibility[FieldDocPos] = settings.value(QStringLiteral("DocPos"), true).toBool();
    m_fieldVisibility[FieldUnicodeChar] = settings.value(QStringLiteral("UnicodeChar"), true).toBool();
    m_fieldVisibility[FieldOccurrences] = settings.value(QStringLiteral("Occurrences"), true).toBool();
    m_fieldVisibility[FieldEol] = settings.value(QStringLiteral("Eol"), true).toBool();
    m_fieldVisibility[FieldEncoding] = settings.value(QStringLiteral("Encoding"), true).toBool();
    m_fieldVisibility[FieldOverType] = settings.value(QStringLiteral("OverType"), true).toBool();
    settings.endGroup();

    for (int i = 0; i < static_cast<int>(FieldCount); ++i) {
        const auto f = static_cast<Field>(i);
        if (auto *label = labelForField(f)) {
            label->setVisible(m_fieldVisibility.value(f, true));
        }
    }
}

void EditorInfoStatusBar::saveSettings() const {
    QSettings settings;
    settings.beginGroup(QStringLiteral("StatusBarFields"));
    settings.setValue(QStringLiteral("Language"), m_fieldVisibility.value(FieldLanguage, true));
    settings.setValue(QStringLiteral("DocSize"), m_fieldVisibility.value(FieldDocSize, true));
    settings.setValue(QStringLiteral("DocPos"), m_fieldVisibility.value(FieldDocPos, true));
    settings.setValue(QStringLiteral("UnicodeChar"), m_fieldVisibility.value(FieldUnicodeChar, true));
    settings.setValue(QStringLiteral("Occurrences"), m_fieldVisibility.value(FieldOccurrences, true));
    settings.setValue(QStringLiteral("Eol"), m_fieldVisibility.value(FieldEol, true));
    settings.setValue(QStringLiteral("Encoding"), m_fieldVisibility.value(FieldEncoding, true));
    settings.setValue(QStringLiteral("OverType"), m_fieldVisibility.value(FieldOverType, true));
    settings.endGroup();
}

void EditorInfoStatusBar::setFieldVisible(Field field, bool visible) {
    m_fieldVisibility[field] = visible;
    if (auto *label = labelForField(field)) {
        label->setVisible(visible);
    }
    saveSettings();
}

bool EditorInfoStatusBar::isFieldVisible(Field field) const {
    return m_fieldVisibility.value(field, true);
}

void EditorInfoStatusBar::resetToDefaults() {
    for (int i = 0; i < static_cast<int>(FieldCount); ++i) {
        const auto f = static_cast<Field>(i);
        m_fieldVisibility[f] = true;
        if (auto *label = labelForField(f)) {
            label->setVisible(true);
        }
    }
    saveSettings();
}

void EditorInfoStatusBar::showCustomizerDialog() {
    StatusBarCustomizerDialog dialog(this, window());
    dialog.exec();
}

void EditorInfoStatusBar::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.setTitle(tr("Status Bar Fields"));

    auto addFieldAction = [&](Field field, const QString &text) {
        QAction *action = menu.addAction(text);
        action->setCheckable(true);
        action->setChecked(isFieldVisible(field));
        connect(action, &QAction::toggled, this, [this, field](bool checked) { setFieldVisible(field, checked); });
    };

    addFieldAction(FieldLanguage, tr("Document Language"));
    addFieldAction(FieldDocSize, tr("Document Size (Length / Lines)"));
    addFieldAction(FieldDocPos, tr("Cursor Position & Selection"));
    addFieldAction(FieldUnicodeChar, tr("Unicode Character (U+XXXX)"));
    addFieldAction(FieldOccurrences, tr("Marked Occurrences Count"));
    addFieldAction(FieldEol, tr("End-of-Line Format"));
    addFieldAction(FieldEncoding, tr("File Encoding"));
    addFieldAction(FieldOverType, tr("Insert / Overtype Mode"));

    menu.addSeparator();

    const auto *dialogAction = menu.addAction(tr("Customize Status Bar Fields..."));
    connect(dialogAction, &QAction::triggered, this, &EditorInfoStatusBar::showCustomizerDialog);

    const auto *resetAction = menu.addAction(tr("Reset to Default Fields"));
    connect(resetAction, &QAction::triggered, this, &EditorInfoStatusBar::resetToDefaults);

    menu.exec(event->globalPos());
}

void EditorInfoStatusBar::setOccurrencesCount(const QString & /*query*/, int count) {
    if (occurrences) {
        if (count > 0) {
            occurrences->setText(tr("Occ: %L1").arg(count));
        } else {
            occurrences->setText(tr("Occ: 0"));
        }
    }
}

void EditorInfoStatusBar::clearOccurrencesCount() {
    if (occurrences) {
        occurrences->setText(tr("Occ: -"));
    }
}

void EditorInfoStatusBar::refresh(ScintillaNext *editor) {
    updateDocumentSize(editor);
    updateSelectionInfo(editor);
    updateUnicodeChar(editor);
    updateLanguage(editor);
    updateEol(editor);
    updateEncoding(editor);
    updateOverType(editor);
}

void EditorInfoStatusBar::connectToEditor(ScintillaNext *editor) {
    disconnect(editorUiUpdated);
    disconnect(documentLexerChanged);

    if (!editor)
        return;

    editorUiUpdated = connect(editor, &ScintillaNext::updateUi, this, &EditorInfoStatusBar::editorUpdated);
    documentLexerChanged =
        connect(editor, &ScintillaNext::lexerChanged, this, [this, editor]() { updateLanguage(editor); });

    refresh(editor);
}

void EditorInfoStatusBar::editorUpdated(Scintilla::Update updated) {
    auto *editor = qobject_cast<ScintillaNext *>(sender());
    if (!editor)
        return;

    using enum Scintilla::Update;
    if (Scintilla::FlagSet(updated, Text)) {
        updateDocumentSize(editor);
    }

    if (Scintilla::FlagSet(updated, Text) || Scintilla::FlagSet(updated, Selection)) {
        updateSelectionInfo(editor);
        updateUnicodeChar(editor);
    }
}

void EditorInfoStatusBar::updateDocumentSize(ScintillaNext *editor) {
    if (!editor || !docSize)
        return;
    QString sizeText = tr("Length: %L1    Lines: %L2").arg(editor->length()).arg(editor->lineCount());
    docSize->setText(sizeText);
}

void EditorInfoStatusBar::updateSelectionInfo(ScintillaNext *editor) {
    if (!editor || !docPos)
        return;
    QString selectionText;

    if (editor->selections() > 1) {
        selectionText = tr("    [%1 Cursors]").arg(editor->selections());
    } else {
        const int start = static_cast<int>(editor->selectionStart());
        const int end = static_cast<int>(editor->selectionEnd());
        if (start != end) {
            const int lines = static_cast<int>(editor->lineFromPosition(end) - editor->lineFromPosition(start) + 1);
            int charCount = 0;
            // On huge selections (e.g. Select All on tens of megabytes), countCharacters byte-by-byte
            // stalls the main thread. Fallback to byte range above 2MB.
            if (end - start > 2 * 1024 * 1024) {
                charCount = end - start;
            } else {
                charCount = static_cast<int>(editor->countCharacters(start, end));
            }
            selectionText = tr("Sel: %L1 | %L2").arg(charCount).arg(lines);
        }
    }

    const int pos = static_cast<int>(editor->currentPos());
    QString positionText =
        tr("Ln: %L1    Col: %L2    ").arg(editor->lineFromPosition(pos) + 1).arg(editor->column(pos) + 1);
    docPos->setText(positionText + selectionText);
}

void EditorInfoStatusBar::updateUnicodeChar(ScintillaNext *editor) {
    if (!editor || !unicodeChar || !isFieldVisible(FieldUnicodeChar))
        return;

    CharacterInfo::Details d = CharacterInfo::getDetailsAt(editor);
    if (d.isValid) {
        unicodeChar->setText(CharacterInfo::formatCodePoint(d.codePoint));
        unicodeChar->setToolTip(CharacterInfo::formatSummary(d) + tr("\n(Click for full details)"));
    } else {
        unicodeChar->setText(QStringLiteral("U+----"));
        unicodeChar->setToolTip(tr("No character at cursor"));
    }
}

void EditorInfoStatusBar::updateLanguage(const ScintillaNext *editor) {
    if (!editor || !docType)
        return;
    docType->setText(editor->languageName);
}

void EditorInfoStatusBar::updateEol(ScintillaNext *editor) {
    if (!editor || !eolFormat)
        return;
    switch (editor->eOLMode()) {
        case SC_EOL_CR:
            eolFormat->setText(tr("Macintosh (CR)"));
            break;
        case SC_EOL_CRLF:
            eolFormat->setText(tr("Windows (CR LF)"));
            break;
        case SC_EOL_LF:
            eolFormat->setText(tr("Unix (LF)"));
            break;
        default:
            eolFormat->setText(tr("Unknown"));
            break;
    }
}

void EditorInfoStatusBar::updateEncoding(const ScintillaNext *editor) {
    if (!editor || !unicodeType)
        return;
    unicodeType->setText(EncodingDetector::formatDisplayName(editor->encodingName(), editor->bom()));
}

void EditorInfoStatusBar::updateOverType(ScintillaNext *editor) {
    if (!editor || !overType)
        return;
    if (editor->overtype()) {
        overType->setText(tr("OVR"));
    } else {
        overType->setText(tr("INS"));
    }
}
