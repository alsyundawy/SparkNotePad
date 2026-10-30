/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 auto-completion & context suggestions logic.
 * Source: zufuliu/Notepad4 (src/EditAutoC.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AutoCompletion.h"
#include "NotepadNextApplication.h"

#include <QApplication>
#include <QSet>
#include <QString>
#include <Qt>

using namespace Scintilla;

AutoCompletion::AutoCompletion(ScintillaNext *editor) : EditorDecorator(editor) {
    setObjectName(QStringLiteral("AutoCompletion"));

    editor->autoCSetOrder(SC_ORDER_PERFORMSORT);
    editor->autoCSetMaxHeight(10);
    editor->autoCSetIgnoreCase(true);
    editor->autoCSetFillUps(" (");

    connect(editor, &ScintillaNext::lexerChanged, this, [this]() {
        NotepadNextApplication *app = qobject_cast<NotepadNextApplication *>(qApp);
        if (app) {
            keywords = app->getLanguageKeywords(this->editor->languageName);
        }
    });
}

void AutoCompletion::notify(const NotificationData *pscn) {
    if (pscn->nmhdr.code == Notification::CharAdded && isEnabled()) {
        if (editor->autoCActive())
            return;

        showAutoCompletion();
    }
}

void AutoCompletion::showAutoCompletion() {
    if (!editor)
        return;

    int curPos = editor->currentPos();
    int startPos = editor->wordStartPosition(curPos, true);
    int endPos = editor->wordEndPosition(curPos, true);

    // Trigger with 2 or more characters for rapid response
    if ((curPos - startPos) < 2)
        return;

    const QByteArray current_word = editor->get_text_range(startPos, curPos);
    const QByteArray regex = "\\b" + current_word + "[\\w]*";
    QSet<QByteArray> words;

    editor->setSearchFlags(SCFIND_REGEXP);

    // Find words in buffer before caret
    editor->forEachMatchInRange(regex,
                                [&](int start, int end) {
                                    words.insert(editor->get_text_range(start, end));
                                    return end;
                                },
                                {0, startPos});

    // Find words in buffer after caret
    editor->forEachMatchInRange(regex,
                                [&](int start, int end) {
                                    words.insert(editor->get_text_range(start, end));
                                    return end;
                                },
                                {endPos, (Sci_PositionCR)editor->length()});

    // Add matching language keywords
    const QString currentWordStr = QString::fromUtf8(current_word);
    for (const QString &kw : keywords) {
        if (kw.startsWith(currentWordStr, Qt::CaseInsensitive)) {
            words.insert(kw.toUtf8());
        }
    }

    if (!words.isEmpty()) {
        QList<QByteArray> sortedWords = words.values();
        std::sort(sortedWords.begin(), sortedWords.end(),
                  [](const QByteArray &a, const QByteArray &b) { return a.toLower() < b.toLower(); });
        editor->autoCShow(current_word.length(), sortedWords.join(' '));
    }
}
