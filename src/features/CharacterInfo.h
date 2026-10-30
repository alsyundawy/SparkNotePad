#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) and Notepad3 (rizonesoft/Notepad3).
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef CHARACTERINFO_H
#define CHARACTERINFO_H

#include <QString>
#include <QByteArray>
#include <cstdint>

class ScintillaNext;

class CharacterInfo {
public:
    struct Details {
        uint32_t codePoint{0};
        QString glyph;                // Raw character string
        QString displayGlyph;         // Printable visual glyph (e.g. for controls NUL, TAB, CR, LF, Space)
        QString name;                 // Unicode character name
        QString block;                // Unicode block (e.g. "Basic Latin", "Currency Symbols", "Emoticons")
        QString categoryCode;         // Category short code (e.g. "Lu", "Ll", "Sm", "Sc", "Cc")
        QString categoryDescription;  // Full category description (e.g. "Letter, Uppercase", "Symbol, Currency")
        QString script;               // Writing script (e.g. "Latin", "Greek", "Cyrillic", "Common")
        QString direction;            // Bidirectional class (e.g. "Left-to-Right", "Right-to-Left")
        QByteArray utf8Bytes;         // Raw UTF-8 bytes
        QString utf8Hex;              // Hex representation: "0xXX 0xXX..."
        QString utf8Dec;              // Dec representation: "NNN, NNN..."
        QString utf16Hex;             // UTF-16 code units: "0xXXXX" or "0xXXXX 0xXXXX"
        QString utf16Dec;             // UTF-16 decimal
        QString utf32Hex;             // UTF-32 hex: "0xXXXXXXXX"
        QString htmlNamedEntity;      // Named HTML entity (e.g. "&euro;", "&copy;")
        QString htmlDecEntity;        // Decimal entity: "&#NNN;"
        QString htmlHexEntity;        // Hex entity: "&#xXXXX;"
        QString cEscape;              // C/C++ escape sequence: "\uXXXX" or "\UXXXXXXXX"
        QString jsonEscape;           // JSON escape: "\uXXXX"
        QString cssEscape;            // CSS escape: "\XXXX "
        QString urlPercent;           // Percent-encoded: "%XX%XX..."
        bool isValid{false};
    };

    // Extracts character details at given position in editor.
    // If pos is -1, uses current selection start or cursor position.
    static Details getDetailsAt(ScintillaNext *editor, int pos = -1);

    // Extracts character details for any arbitrary 32-bit Unicode code point.
    static Details getDetailsForCodePoint(uint32_t codePoint);

    // Formatted summary text suitable for tooltips or CallTips.
    static QString formatSummary(const Details &d);

    // Formats standard U+XXXX code point string.
    static QString formatCodePoint(uint32_t cp);
};

#endif  // CHARACTERINFO_H
