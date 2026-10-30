/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 number conversion logic.
 * Source: zufuliu/Notepad4 (src/Edit.cpp)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "NumberConverter.h"
#if !defined(UNIT_TESTING)
#include "ScintillaNext.h"
#endif

#include <algorithm>
#include <QByteArray>

NumberConverter::Result NumberConverter::convertString(const QString &input, TargetBase targetBase, bool uppercaseHex,
                                                       bool addPrefix) {
    const QString text = input.trimmed();
    if (text.isEmpty()) {
        return {false, input, QString(), QStringLiteral("Empty number string")};
    }

    bool ok = false;
    qulonglong value = 0;

    if (text.startsWith(QLatin1String("0x"), Qt::CaseInsensitive)) {
        value = text.mid(2).toULongLong(&ok, 16);
    } else if (text.startsWith(QLatin1String("0b"), Qt::CaseInsensitive)) {
        value = text.mid(2).toULongLong(&ok, 2);
    } else if (text.startsWith(QLatin1String("0o"), Qt::CaseInsensitive)) {
        value = text.mid(2).toULongLong(&ok, 8);
    } else {
        // Check if string contains hex digits A-F
        const bool hasHexAlpha = std::any_of(text.cbegin(), text.cend(), [](const QChar &ch) {
            return (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
        });

        if (hasHexAlpha) {
            value = text.toULongLong(&ok, 16);
        } else {
            value = text.toULongLong(&ok, 10);
            if (!ok) {
                value = text.toULongLong(&ok, 16);
            }
        }
    }

    if (!ok) {
        return {false, text, QString(), QStringLiteral("Could not parse number")};
    }

    QString formatted;
    switch (targetBase) {
        case ToHex: {
            QString hex = QString::number(value, 16);
            if (uppercaseHex) {
                hex = hex.toUpper();
            }
            formatted = addPrefix ? (QStringLiteral("0x") + hex) : hex;
            break;
        }
        case ToDec: {
            formatted = QString::number(value, 10);
            break;
        }
        case ToBin: {
            QString bin = QString::number(value, 2);
            formatted = addPrefix ? (QStringLiteral("0b") + bin) : bin;
            break;
        }
        case ToOct: {
            QString oct = QString::number(value, 8);
            formatted = addPrefix ? (QStringLiteral("0o") + oct) : oct;
            break;
        }
    }

    return {true, text, formatted, QString()};
}

#if !defined(UNIT_TESTING)
NumberConverter::Result NumberConverter::convert(ScintillaNext *editor, TargetBase targetBase, bool uppercaseHex,
                                                 bool addPrefix) {
    if (!editor) {
        return {false, QString(), QString(), QStringLiteral("No active editor")};
    }

    int startPos = editor->selectionStart();
    int endPos = editor->selectionEnd();

    // If no selection, expand to number token under cursor
    if (startPos == endPos) {
        const int curPos = editor->currentPos();
        const int docLen = editor->length();

        // Scan backward for word/number chars
        startPos = curPos;
        while (startPos > 0) {
            const char ch = editor->charAt(startPos - 1);
            if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == 'x' ||
                ch == 'X' || ch == 'b' || ch == 'B' || ch == 'o' || ch == 'O') {
                startPos--;
            } else {
                break;
            }
        }

        // Scan forward
        endPos = curPos;
        while (endPos < docLen) {
            const char ch = editor->charAt(endPos);
            if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
                endPos++;
            } else {
                break;
            }
        }

        if (startPos == endPos) {
            return {false, QString(), QString(), QStringLiteral("No number found under cursor")};
        }
    }

    const QByteArray raw = editor->get_text_range(startPos, endPos);
    const QString numStr = QString::fromUtf8(raw);

    auto result = convertString(numStr, targetBase, uppercaseHex, addPrefix);
    if (result.success) {
        editor->beginUndoAction();
        editor->setTargetRange(startPos, endPos);
        const QByteArray rep = result.converted.toUtf8();
        editor->replaceTarget(rep.length(), rep.constData());
        editor->setSelection(startPos + rep.length(), startPos);
        editor->endUndoAction();
    }

    return result;
}

#endif
