#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) Compress and Pretty Code features.
 * Source: zufuliu/notepad4 (src/Edit.cpp, src/Styles.cpp)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef CODECOMPRESSPRETTY_H
#define CODECOMPRESSPRETTY_H

#include <cstdint>
#include <QString>

#if !defined(UNIT_TESTING)
class ScintillaNext;
#endif

class CodeCompressPretty {
public:
    enum Language : std::uint8_t { Auto, Json, Json5, Css, JavaScript };

    struct Result {
        bool success{false};
        bool wholeDocument{false};
        Language detectedLanguage{Auto};
        QString original;
        QString formatted;
        QString errorMessage;
    };

#if !defined(UNIT_TESTING)
    // Formats (Prettifies) selection or whole document in editor
    static Result prettify(ScintillaNext *editor, Language lang = Auto);

    // Minifies (Compresses) selection or whole document in editor
    static Result compress(ScintillaNext *editor, Language lang = Auto);
#endif

    // Direct string formatters
    static Result prettifyString(const QString &text, Language lang = Auto);
    static Result compressString(const QString &text, Language lang = Auto);

    // Language-specific helpers
    static Result prettifyJson(const QString &text, bool isJson5 = false);
    static Result compressJson(const QString &text, bool isJson5 = false);

    static Result prettifyCss(const QString &text);
    static Result compressCss(const QString &text);

    static Result prettifyJs(const QString &text);
    static Result compressJs(const QString &text);

    // Detect language from text sample or editor context
    static Language detectLanguage(const QString &sample = QString());
#if !defined(UNIT_TESTING)
    static Language detectLanguage(ScintillaNext *editor, const QString &sample = QString());
#endif
    static QString languageName(Language lang);
};

#endif  // CODECOMPRESSPRETTY_H
