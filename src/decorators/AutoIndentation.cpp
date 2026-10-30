/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 smart auto-indent rules.
 * Source: zufuliu/Notepad4 (src/Edit.cpp, EditAutoC.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AutoIndentation.h"
#include <QByteArray>

using namespace Scintilla;

AutoIndentation::AutoIndentation(ScintillaNext *editor) : EditorDecorator(editor) {
    setObjectName(QStringLiteral("AutoIndentation"));
}

void AutoIndentation::notify(const NotificationData *pscn) {
    if (pscn->nmhdr.code == Notification::CharAdded && isEnabled()) {
        const int ch = pscn->ch;
        const int eolMode = editor->eOLMode();

        if (((eolMode == SC_EOL_CRLF || eolMode == SC_EOL_LF) && ch == '\n') || (eolMode == SC_EOL_CR && ch == '\r')) {
            autoIndentLine(editor->lineFromPosition(editor->currentPos()));
        }
    }
}

void AutoIndentation::autoIndentLine(int line) const {
    if (line <= 0 || !editor)
        return;

    const int previousLine = line - 1;
    int targetIndentation = editor->lineIndentation(previousLine);

    // Smart Indentation: inspect previous line's trimmed end character
    const int prevStart = editor->positionFromLine(previousLine);
    const int prevEnd = editor->lineEndPosition(previousLine);
    const QByteArray prevLineText = editor->get_text_range(prevStart, prevEnd).trimmed();

    int indentWidth = editor->indent();
    if (indentWidth <= 0) {
        indentWidth = editor->tabWidth();
    }
    if (indentWidth <= 0) {
        indentWidth = 4;
    }

    if (!prevLineText.isEmpty()) {
        const char lastChar = prevLineText.at(prevLineText.length() - 1);
        if (lastChar == '{' || lastChar == '[' || lastChar == '(' || lastChar == ':') {
            targetIndentation += indentWidth;
        }
    }

    // If current line begins with closing brace/bracket, adjust indentation back
    const int curStart = editor->positionFromLine(line);
    const int curEnd = editor->lineEndPosition(line);
    const QByteArray curLineText = editor->get_text_range(curStart, curEnd).trimmed();

    if (!curLineText.isEmpty()) {
        const char firstChar = curLineText.at(0);
        if ((firstChar == '}' || firstChar == ']' || firstChar == ')') && targetIndentation >= indentWidth) {
            targetIndentation -= indentWidth;
        }
    }

    if (targetIndentation > 0) {
        editor->setLineIndentation(line, targetIndentation);
        editor->gotoPos(editor->findColumn(line, targetIndentation));
    }
}
