#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) Calculate Expression feature.
 * Source: zufuliu/notepad4 (src/Edit.cpp, PR #1230)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef MATHEVALUATOR_H
#define MATHEVALUATOR_H

#include <cstdint>
#include <QString>

#if !defined(UNIT_TESTING)
class ScintillaNext;
#endif

class MathEvaluator {
public:
    enum Action : std::uint8_t {
        AppendResult,      // "15 * 4" -> "15 * 4 = 60"
        ReplaceExpression  // "15 * 4" -> "60"
    };

    struct Result {
        bool success{false};
        double value{0.0};
        QString expression;
        QString formattedResult;
        QString errorMessage;
    };

#if !defined(UNIT_TESTING)
    static Result evaluateInEditor(ScintillaNext *editor, Action action = AppendResult);
#endif

    // Direct string evaluation
    static Result evaluateString(const QString &expr);

    // Format double value cleanly (stripping trailing .0 for whole numbers)
    static QString formatValue(double val);
};

#endif  // MATHEVALUATOR_H
