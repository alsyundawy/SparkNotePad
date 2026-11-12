/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 auto-completion & context suggestions logic.
 * Source: zufuliu/Notepad4 (src/EditAutoC.cpp)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AutoCompletion.h"
#include "SparkNotePadApplication.h"

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
    editor->autoCSetFillUps("");
    editor->autoCSetChooseSingle(false);
    editor->autoCSetAutoHide(true);
    editor->autoCSetDropRestOfWord(true);

    connect(editor, &ScintillaNext::lexerChanged, this, [this]() {
        const auto *app = qobject_cast<const SparkNotePadApplication *>(qApp);
        if (app != nullptr) {
            keywords = app->getLanguageKeywords(this->editor->languageName);
        }
    });
}

void AutoCompletion::notify(const NotificationData *pscn) {
    if (pscn->nmhdr.code == Notification::CharAdded && isEnabled()) {
        if (!editor || editor->readOnly() || editor->autoCActive() || editor->length() > 5L * 1024L * 1024L) {
            return;
        }

        showAutoCompletion();
    }
}

void AutoCompletion::showAutoCompletion() {
    if (editor == nullptr) {
        return;
    }

    const auto curPos = static_cast<int>(editor->currentPos());
    const auto startPos = static_cast<int>(editor->wordStartPosition(curPos, true));
    const auto endPos = static_cast<int>(editor->wordEndPosition(curPos, true));

    // Trigger with 3 or more characters for non-disruptive, precise context completions
    if ((curPos - startPos) < 3) {
        return;
    }

    const QByteArray current_word = editor->get_text_range(startPos, curPos);
    if (const bool isValidIdentifier = std::ranges::all_of(current_word,
                                                           [](char c) {
                                                               return (c >= 'a' && c <= 'z') ||
                                                                      (c >= 'A' && c <= 'Z') ||
                                                                      (c >= '0' && c <= '9') || c == '_';
                                                           });
        !isValidIdentifier) {
        return;
    }

    const QByteArray regex = "\\b" + current_word + "[\\w]*";
    QSet<QByteArray> words;

    editor->setSearchFlags(SCFIND_REGEXP);

    // For large files (> 1MB or > 5,000 lines), constrain search to viewport +/- 200 lines
    Sci_PositionCR searchStart = 0;
    auto searchEnd = static_cast<Sci_PositionCR>(editor->length());
    if (editor->length() > 1024L * 1024L || editor->lineCount() > 5000) {
        const int firstLine = qMax(0, static_cast<int>(editor->firstVisibleLine()) - 200);
        const int lastLine = qMin(static_cast<int>(editor->lineCount()),
                                  static_cast<int>(editor->firstVisibleLine() + editor->linesOnScreen() + 200));
        searchStart = static_cast<Sci_PositionCR>(editor->positionFromLine(firstLine));
        searchEnd = static_cast<Sci_PositionCR>(editor->lineEndPosition(lastLine));
    }

    int wordCount = 0;
    // Find words in buffer before caret
    if (searchStart < startPos) {
        editor->forEachMatchInRange(regex,
                                    [&words, &wordCount, startPos, this](int start, int end) {
                                        words.insert(editor->get_text_range(start, end));
                                        if (++wordCount >= 100) {
                                            return startPos;
                                        }
                                        return end;
                                    },
                                    {searchStart, startPos});
    }

    // Find words in buffer after caret
    if (wordCount < 100 && endPos < searchEnd) {
        editor->forEachMatchInRange(regex,
                                    [&words, &wordCount, searchEnd, this](int start, int end) {
                                        words.insert(editor->get_text_range(start, end));
                                        if (++wordCount >= 100) {
                                            return static_cast<int>(searchEnd);
                                        }
                                        return end;
                                    },
                                    {endPos, searchEnd});
    }

    // Add matching language keywords
    const QString currentWordStr = QString::fromUtf8(current_word);
    for (const QString &kw : keywords) {
        if (kw.startsWith(currentWordStr, Qt::CaseInsensitive)) {
            words.insert(kw.toUtf8());
        }
    }

    if (!words.isEmpty()) {
        QList<QByteArray> sortedWords = words.values();
        std::ranges::sort(sortedWords,
                          [](const QByteArray &a, const QByteArray &b) { return a.toLower() < b.toLower(); });
        editor->autoCShow(current_word.length(), sortedWords.join(' '));
    }
}
