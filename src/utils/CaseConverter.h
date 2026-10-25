#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) and Notepad3 (rizonesoft/notepad3)
 * case conversion routines (Upper, Lower, Invert, Title, Sentence case)
 * with multi-selection support for SparkNotePad.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef CASECONVERTER_H
#define CASECONVERTER_H

#include <cstdint>
#include <QString>

#if !defined(UNIT_TESTING)
class ScintillaNext;
#endif

class CaseConverter {
public:
    enum class CaseType : std::uint8_t { UpperCase, LowerCase, InvertCase, TitleCase, SentenceCase };

    struct ConvertResult {
        bool success = false;
        int selectionsConverted = 0;
        int charactersChanged = 0;
        QString message;
        QString errorMessage;
    };

    // Pure string transformation methods (Unicode-aware)
    static QString toUpper(const QString &text);
    static QString toLower(const QString &text);
    static QString toInvertCase(const QString &text);
    static QString toTitleCase(const QString &text);
    static QString toSentenceCase(const QString &text);

    static QString convertText(const QString &text, CaseType caseType);

#if !defined(UNIT_TESTING)
    static ConvertResult convertEditor(ScintillaNext *editor, CaseType caseType);
    static ConvertResult upperCase(ScintillaNext *editor);
    static ConvertResult lowerCase(ScintillaNext *editor);
    static ConvertResult invertCase(ScintillaNext *editor);
    static ConvertResult titleCase(ScintillaNext *editor);
    static ConvertResult sentenceCase(ScintillaNext *editor);
#endif
};

#endif  // CASECONVERTER_H
