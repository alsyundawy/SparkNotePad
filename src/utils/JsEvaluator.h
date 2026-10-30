#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) Evaluate JS Expression feature.
 * Source: zufuliu/notepad4 (src/Edit.cpp, Web Tools)
 * Uses native JavaScriptCore on macOS and portable runtime fallback on Linux.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef JSEVALUATOR_H
#define JSEVALUATOR_H

#include <QString>

class ScintillaNext;

class JsEvaluator {
public:
    enum Action {
        ReplaceSelection,  // Replace selection with evaluation result
        AppendComment      // Append " // -> <result>"
    };

    struct Result {
        bool success{false};
        QString expression;
        QString resultString;
        QString errorMessage;
        double executionTimeMs{0.0};
    };

    // Evaluates a JavaScript expression or script snippet
    static Result evaluate(const QString &script);

    // Evaluates in active Scintilla editor (selection or current line)
    static Result evaluateInEditor(ScintillaNext *editor, Action action = ReplaceSelection);
};

#endif  // JSEVALUATOR_H
