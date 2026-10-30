#pragma once
/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef INLINEMATHEVALUATOR_H
#define INLINEMATHEVALUATOR_H

#include <QString>

class ScintillaNext;

struct MathResult {
    bool success = false;
    double value = 0.0;
    QString formattedValue;
    QString errorMessage;
};

class InlineMathEvaluator {
public:
    static MathResult evaluate(const QString &expression);
    static bool evaluateInEditor(ScintillaNext *editor, bool appendResult, QString *outMessage = nullptr);
};

#endif  // INLINEMATHEVALUATOR_H
