/*
 *
 * This file is part of SparkNotepad.
 *
 * Copyright 2018-2026 Justin Dailey
 * Copyright 2025-2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * Notepad Next is free software: you can redistribute it and/or modify
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

#include "ScintillaCommenter.h"
#include "SelectionTracker.h"

ScintillaCommenter::ScintillaCommenter(ScintillaNext *editor) : editor(editor), st(editor), ua(editor) {}

void ScintillaCommenter::toggleSelection() {
    editor->forEachLineInSelection(editor->mainSelection(), [&](sptr_t line) { toggleLine(line); });
}

void ScintillaCommenter::commentSelection() {
    editor->forEachLineInSelection(editor->mainSelection(), [&](sptr_t line) { commentLine(line); });
}

void ScintillaCommenter::uncommentSelection() {
    editor->forEachLineInSelection(editor->mainSelection(), [&](sptr_t line) { uncommentLine(line); });
}

void ScintillaCommenter::toggleLine(sptr_t line) {
    const sptr_t indentPos = editor->lineIndentPosition(line);
    const sptr_t commentLen = static_cast<sptr_t>(editor->languageSingleLineComment.length());
    const sptr_t lineEnd = qMin(indentPos + commentLen, editor->lineEndPosition(line));
    const QByteArray commentText = editor->get_text_range(indentPos, lineEnd);

    if (commentText == editor->languageSingleLineComment) {
        editor->deleteRange(indentPos, commentLen);

        st.trackDeletion(static_cast<int>(indentPos), static_cast<int>(commentLen));
    } else {
        if (indentPos == editor->lineEndPosition(line)) {
            return;
        }

        editor->insertText(indentPos, editor->languageSingleLineComment.constData());

        st.trackInsertion(static_cast<int>(indentPos), static_cast<int>(commentLen));
    }
}

void ScintillaCommenter::commentLine(sptr_t line) {
    const sptr_t indentPos = editor->lineIndentPosition(line);

    // Don't comment lines with only indentation
    if (indentPos == editor->lineEndPosition(line)) {
        return;
    }

    editor->insertText(indentPos, editor->languageSingleLineComment.constData());

    const sptr_t commentLen = static_cast<sptr_t>(editor->languageSingleLineComment.length());
    st.trackInsertion(static_cast<int>(indentPos), static_cast<int>(commentLen));
}

void ScintillaCommenter::uncommentLine(sptr_t line) {
    const sptr_t indentPos = editor->lineIndentPosition(line);
    const sptr_t commentLen = static_cast<sptr_t>(editor->languageSingleLineComment.length());
    const sptr_t lineEnd = qMin(indentPos + commentLen, editor->lineEndPosition(line));
    const QByteArray commentText = editor->get_text_range(indentPos, lineEnd);

    if (commentText == editor->languageSingleLineComment) {
        editor->deleteRange(indentPos, commentLen);

        st.trackDeletion(static_cast<int>(indentPos), static_cast<int>(commentLen));
    }
}
