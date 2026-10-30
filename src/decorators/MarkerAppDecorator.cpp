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

#include "MarkerAppDecorator.h"
#include "EditorManager.h"
#include "ScintillaNext.h"

static const QList<QColor> &markerColors() {
    static const QList<QColor> colors = {QColor(0x00, 0xFF, 0xFF), QColor(0xFF, 0x80, 0x00), QColor(0xFF, 0xFF, 0x00)};
    return colors;
}

static int QColorToScintillaColour(const QColor &c) {
    return c.red() | (c.green() << 8) | (c.blue() << 16);
}

MarkerAppDecorator::MarkerAppDecorator(NotepadNextApplication *app) : ApplicationDecorator(app) {
    // Any time an editor is created go ahead and allocate/set the required indicators
    connect(app->getEditorManager(), &EditorManager::editorCreated, this, [](ScintillaNext *editor) {
        const auto &colors = markerColors();
        for (int i = 0; i < colors.size(); i++) {
            int indicator = editor->allocateIndicator(QString("marker_%1").arg(i));
            editor->indicSetFore(indicator, QColorToScintillaColour(colors[i]));
            editor->indicSetStyle(indicator, INDIC_ROUNDBOX);
            editor->indicSetOutlineAlpha(indicator, 150);
            editor->indicSetAlpha(indicator, 100);
            editor->indicSetUnder(indicator, true);
        }
    });
}

QColor MarkerAppDecorator::markerColor(int i) const {
    return markerColors().value(i);
}

void MarkerAppDecorator::mark(ScintillaNext *editor, int i) {
    int selectionStart = editor->selectionStart();
    int selectionEnd = editor->selectionEnd();
    bool isWholeWord = false;

    // If there is no selection, get the word at the cursor, else use it verbatim
    if (selectionStart == selectionEnd) {
        isWholeWord = true;
        const int curPos = editor->currentPos();
        selectionStart = editor->wordStartPosition(curPos, true);
        selectionEnd = editor->wordEndPosition(curPos, true);

        // If they are still in the same location, just abort
        if (selectionStart == selectionEnd) {
            return;
        }
    }

    // Limit the size of the selection
    if ((selectionEnd - selectionStart) > 1024) {
        return;
    }

    int indicator = editor->allocateIndicator(QStringLiteral("marker_%1").arg(i));
    editor->setIndicatorCurrent(indicator);

    const QByteArray selText = editor->get_text_range(selectionStart, selectionEnd);
    const int flags = isWholeWord ? SCFIND_WHOLEWORD : 0;

    editor->setSearchFlags(flags);
    editor->forEachMatch(selText, [&](int start, int end) {
        editor->indicatorFillRange(start, end - start);
        return end;
    });
}

void MarkerAppDecorator::clear(ScintillaNext *editor, int i) {
    int indicator = editor->allocateIndicator(QString("marker_%1").arg(i));
    editor->setIndicatorCurrent(indicator);
    editor->indicatorClearRange(0, editor->length());
}

void MarkerAppDecorator::clearAll(ScintillaNext *editor) {
    const auto &colors = markerColors();
    for (int i = 0; i < colors.size(); i++) {
        clear(editor, i);
    }
}