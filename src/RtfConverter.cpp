/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) and NotepadNext Copy as RTF syntax coloring.
 * Source: zufuliu/notepad4 (src/Edit.cpp, src/Styles.cpp)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "RtfConverter.h"
#include "ScintillaNext.h"

#include <QColor>
#include <QTextStream>
#include <QVector>

static inline QColor ScintillaColorToQColor(int color) {
    return QColor::fromRgb(color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
}

static void AddCharToRtf(QTextStream &stream, uint ucs4) {
    if (ucs4 == '\\') {
        stream << "\\\\";
    } else if (ucs4 == '{') {
        stream << "\\{";
    } else if (ucs4 == '}') {
        stream << "\\}";
    } else if (ucs4 == '\t') {
        stream << "\\tab ";
    } else if (ucs4 == '\n') {
        stream << "\\par\n";
    } else if (ucs4 == '\r') {
        // Skip CR; line endings handled by LF
    } else if (ucs4 >= 32 && ucs4 <= 126) {
        stream << static_cast<char>(ucs4);
    } else if (ucs4 <= 0xFFFF) {
        // RTF signed 16-bit integer for unicode
        const short val =
            (ucs4 < 32768) ? static_cast<short>(ucs4) : static_cast<short>(static_cast<int>(ucs4) - 65536);
        stream << "\\u" << val << '?';
    } else if (ucs4 <= 0x10FFFF) {
        // Astral plane surrogate pair
        const uint high = 0xD800 + ((ucs4 - 0x10000) >> 10);
        const uint low = 0xDC00 + ((ucs4 - 0x10000) & 0x3FF);
        const short val_h = static_cast<short>(static_cast<int>(high) - 65536);
        const short val_l = static_cast<short>(static_cast<int>(low) - 65536);
        stream << "\\u" << val_h << "?\\u" << val_l << '?';
    }
}

void RtfConverter::convertRange(QTextStream &stream, int start, int end) {
    if (!editor || start >= end) {
        return;
    }

    ensureDocumentStyled(start, end);

    QByteArray rtf_body;
    QTextStream rtf_stream(&rtf_body);
    QVector<QColor> styleColors;

    int prevStyle = -1;
    bool prevBold = false;
    bool prevItalic = false;
    bool prevUnderline = false;

    int pos = start;
    while (pos < end) {
        const int style = editor->styleAt(pos);

        if (style != prevStyle) {
            const QColor foreColor = ScintillaColorToQColor(editor->styleFore(style));
            if (!styleColors.contains(foreColor)) {
                styleColors.append(foreColor);
            }
            const int colorIndex = styleColors.indexOf(foreColor) + 1;  // 1-based index in RTF colortbl

            // Font weight (bold)
            const bool bold = editor->styleBold(style);
            if (bold != prevBold) {
                rtf_stream << (bold ? "\\b " : "\\b0 ");
                prevBold = bold;
            }

            // Italic
            const bool italic = editor->styleItalic(style);
            if (italic != prevItalic) {
                rtf_stream << (italic ? "\\i " : "\\i0 ");
                prevItalic = italic;
            }

            // Underline
            const bool underline = editor->styleUnderline(style);
            if (underline != prevUnderline) {
                rtf_stream << (underline ? "\\ul " : "\\ulnone ");
                prevUnderline = underline;
            }

            // Foreground color
            rtf_stream << "\\cf" << colorIndex << ' ';

            prevStyle = style;
        }

        const int nextPos = editor->positionRelative(pos, 1);
        const int charLen = (nextPos > pos) ? (nextPos - pos) : 1;

        const QByteArray bytes = editor->get_text_range(pos, pos + charLen);
        const QString utfText = QString::fromUtf8(bytes);
        for (const QChar &qc : utfText) {
            AddCharToRtf(rtf_stream, qc.unicode());
        }

        pos += charLen;
    }

    // Reset styles at the end
    if (prevBold)
        rtf_stream << "\\b0 ";
    if (prevItalic)
        rtf_stream << "\\i0 ";
    if (prevUnderline)
        rtf_stream << "\\ulnone ";

    rtf_stream.flush();

    // Determine font name
    QString fontName = editor->styleFont(STYLE_DEFAULT);
    if (fontName.isEmpty()) {
#if defined(Q_OS_MACOS)
        fontName = QStringLiteral("Menlo");
#elif defined(Q_OS_WIN)
        fontName = QStringLiteral("Consolas");
#else
        fontName = QStringLiteral("Monospace");
#endif
    }

    const int fontSizePt = editor->styleSize(STYLE_DEFAULT);
    const int fontSizeHalfPt = (fontSizePt > 0 ? fontSizePt : 10) * 2;

    // Header & font table
    stream << "{\\rtf1\\ansi\\ansicpg1252\\deff0\\nouicompat\\deflang1033\n";
    stream << "{\\fonttbl{\\f0\\fnil\\fcharset0 " << fontName << ";}}\n";

    // RTF Color Table (slot 0 is default/auto text)
    stream << "{\\colortbl ;";
    for (const auto &c : styleColors) {
        stream << "\\red" << c.red() << "\\green" << c.green() << "\\blue" << c.blue() << ";";
    }
    stream << "}\n\n";

    stream << "\\f0\\fs" << fontSizeHalfPt << "\\cf0\n";
    stream << "{\n";
    stream << rtf_body << "\n";
    stream << "}\n}\n";
    stream.flush();
}
