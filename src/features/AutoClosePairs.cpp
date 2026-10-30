/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 auto-completion & auto-close pair logic.
 * Source: zufuliu/Notepad4 (src/EditAutoC.cpp, Edit.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AutoClosePairs.h"
#include "ScintillaNext.h"

#include <QEvent>
#include <QKeyEvent>

AutoClosePairs::AutoClosePairs(ScintillaNext *editor) : EditorDecorator(editor) {
    setObjectName(QStringLiteral("AutoClosePairs"));
    editor->installEventFilter(this);
}

void AutoClosePairs::notify(const Scintilla::NotificationData *pscn) {
    Q_UNUSED(pscn)
}

bool AutoClosePairs::isWordChar(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_';
}

bool AutoClosePairs::eventFilter(QObject *obj, QEvent *event) {
    Q_UNUSED(obj)

    if (event->type() == QEvent::KeyPress && isEnabled()) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (handleKeyPress(keyEvent)) {
            return true;
        }
    }

    return false;
}

bool AutoClosePairs::handleKeyPress(QKeyEvent *keyEvent) {
    if (!editor || !editor->selectionEmpty()) {
        return false;
    }

    const int curPos = editor->currentPos();
    const int docLen = editor->length();
    const char charAtPos = (curPos < docLen) ? editor->charAt(curPos) : 0;
    const char charBefore = (curPos > 0) ? editor->charAt(curPos - 1) : 0;
    const QString text = keyEvent->text();

    if (text.isEmpty()) {
        // Handle Backspace between empty pairs
        if (keyEvent->key() == Qt::Key_Backspace && curPos > 0 && curPos < docLen) {
            bool isPair = (charBefore == '(' && charAtPos == ')') || (charBefore == '[' && charAtPos == ']') ||
                          (charBefore == '{' && charAtPos == '}') || (charBefore == '"' && charAtPos == '"') ||
                          (charBefore == '\'' && charAtPos == '\'') || (charBefore == '`' && charAtPos == '`');
            if (isPair) {
                editor->deleteRange(curPos - 1, 2);
                return true;
            }
        }
        return false;
    }

    const QChar ch = text.at(0);

    // 1. Auto-close Brackets: (), [], {}
    if (m_autoCloseBrackets) {
        if (ch == '(') {
            editor->insertText(curPos, "()");
            editor->gotoPos(curPos + 1);
            return true;
        } else if (ch == '[') {
            editor->insertText(curPos, "[]");
            editor->gotoPos(curPos + 1);
            return true;
        } else if (ch == '{') {
            editor->insertText(curPos, "{}");
            editor->gotoPos(curPos + 1);
            return true;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            // Overtype / skip closing bracket if it already matches the next character
            if (charAtPos == ch.toLatin1()) {
                editor->gotoPos(curPos + 1);
                return true;
            }
        }
    }

    // 2. Auto-close Quotes: "", '', ``
    if (m_autoCloseQuotes) {
        if (ch == '"') {
            if (charAtPos == '"') {
                editor->gotoPos(curPos + 1);
                return true;
            }
            editor->insertText(curPos, "\"\"");
            editor->gotoPos(curPos + 1);
            return true;
        } else if (ch == '\'') {
            if (charAtPos == '\'') {
                editor->gotoPos(curPos + 1);
                return true;
            }
            // Avoid closing when typing apostrophe in contractions like "don't"
            if (!isWordChar(charBefore)) {
                editor->insertText(curPos, "''");
                editor->gotoPos(curPos + 1);
                return true;
            }
        } else if (ch == '`') {
            if (charAtPos == '`') {
                editor->gotoPos(curPos + 1);
                return true;
            }
            editor->insertText(curPos, "``");
            editor->gotoPos(curPos + 1);
            return true;
        }
    }

    return false;
}
