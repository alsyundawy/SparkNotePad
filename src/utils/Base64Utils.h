#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 Base64 conversion utilities.
 * Source: zufuliu/Notepad4 (src/Edit.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef BASE64UTILS_H
#define BASE64UTILS_H

#include <QString>
#include <QByteArray>

#if !defined(UNIT_TESTING)
class ScintillaNext;
#endif

class Base64Utils {
public:
    struct Result {
        bool success{false};
        bool wholeDocument{false};
        QString errorMessage;
    };

    // Encodes selection if present, otherwise encodes the entire document.
#if !defined(UNIT_TESTING)
    static Result encode(ScintillaNext *editor);
    static Result decode(ScintillaNext *editor);
#endif

    // Pure string encoding and decoding helpers
    static QString encode(const QString &plainText);
    static QString decode(const QString &base64Text, bool *ok = nullptr);
    static QString encodeUrlSafe(const QString &plainText);
    static QString decodeUrlSafe(const QString &base64Text, bool *ok = nullptr);
};

#endif  // BASE64UTILS_H
