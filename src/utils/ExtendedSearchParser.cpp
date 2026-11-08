/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ExtendedSearchParser.h"

namespace {

bool parseHexEscape(const QString &input, qsizetype &i, qsizetype len, QString &result) {
    qsizetype hexLen = 0;
    while (hexLen < 2 && (i + 2 + hexLen) < len) {
        const QChar hc = input.at(i + 2 + hexLen);
        const ushort u = hc.unicode();
        if ((u >= '0' && u <= '9') || (u >= 'a' && u <= 'f') || (u >= 'A' && u <= 'F')) {
            hexLen++;
        } else {
            break;
        }
    }
    if (hexLen > 0) {
        bool ok = false;
        const int val = input.sliced(i + 2, hexLen).toInt(&ok, 16);
        if (ok) {
            result.append(QChar(val));
            i += 2 + hexLen;
            return true;
        }
    }
    return false;
}

bool parseDecEscape(const QString &input, qsizetype &i, qsizetype len, QString &result) {
    qsizetype decLen = 0;
    while (decLen < 3 && (i + 2 + decLen) < len) {
        const QChar dc = input.at(i + 2 + decLen);
        if (dc.isDigit()) {
            decLen++;
        } else {
            break;
        }
    }
    if (decLen > 0) {
        bool ok = false;
        const int val = input.sliced(i + 2, decLen).toInt(&ok, 10);
        if (ok && val >= 0 && val <= 255) {
            result.append(QChar(val));
            i += 2 + decLen;
            return true;
        }
    }
    return false;
}

}  // namespace

QString ExtendedSearchParser::parse(const QString &input, bool *hasError) {
    if (hasError) {
        *hasError = false;
    }
    QString result;
    result.reserve(input.size());

    const qsizetype len = input.length();
    qsizetype i = 0;
    while (i < len) {
        const QChar c = input.at(i);
        if (c == QLatin1Char('\\') && i + 1 < len) {
            const QChar next = input.at(i + 1);
            switch (next.unicode()) {
                case 'n':
                    result.append(QLatin1Char('\n'));
                    i += 2;
                    break;
                case 'r':
                    result.append(QLatin1Char('\r'));
                    i += 2;
                    break;
                case 't':
                    result.append(QLatin1Char('\t'));
                    i += 2;
                    break;
                case '0':
                    result.append(QChar(0));
                    i += 2;
                    break;
                case 'b':
                    result.append(QLatin1Char('\b'));
                    i += 2;
                    break;
                case 'f':
                    result.append(QLatin1Char('\f'));
                    i += 2;
                    break;
                case 'v':
                    result.append(QLatin1Char('\v'));
                    i += 2;
                    break;
                case '\\':
                    result.append(QLatin1Char('\\'));
                    i += 2;
                    break;
                case 'x': {
                    if (!parseHexEscape(input, i, len, result)) {
                        if (hasError) {
                            *hasError = true;
                        }
                        result.append(c);
                        result.append(next);
                        i += 2;
                    }
                    break;
                }
                case 'd': {
                    if (!parseDecEscape(input, i, len, result)) {
                        if (hasError) {
                            *hasError = true;
                        }
                        result.append(c);
                        result.append(next);
                        i += 2;
                    }
                    break;
                }
                default:
                    // Unknown escape: keep literal backslash and character
                    result.append(c);
                    result.append(next);
                    i += 2;
                    break;
            }
        } else {
            result.append(c);
            i++;
        }
    }
    return result;
}
