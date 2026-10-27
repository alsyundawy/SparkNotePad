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

#include "RegexSafetyEngine.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <future>
#include <chrono>

bool RegexSafetyEngine::isSafeRegex(const QString &pattern, QString *vulnerabilityDescription) {
    if (pattern.isEmpty()) {
        return true;
    }

    // Detect nested exponential quantifiers: (x+)+, (x*)*, (x+)*, (x*)+, ([a-z]+)+, etc.
    static const QRegularExpression kNestedQuantifier(QStringLiteral(R"(\([^\)\\]*[\+\*]\s*\)\s*[\+\*\{])"));

    if (kNestedQuantifier.match(pattern).hasMatch()) {
        if (vulnerabilityDescription) {
            *vulnerabilityDescription =
                QCoreApplication::translate("RegexSafetyEngine",
                                            "Catastrophic backtracking vulnerability detected: nested quantifiers "
                                            "'(%1)' can cause application hang (ReDoS).")
                    .arg(kNestedQuantifier.match(pattern).captured(0));
        }
        return false;
    }

    // Detect nested range quantifiers: (x{1,5})+ or (x+){2,}
    static const QRegularExpression kNestedRangeQuantifier(QStringLiteral(R"(\([^\)\\]*\{\d+,?\d*\}\s*\)\s*[\+\*\{])"));

    if (kNestedRangeQuantifier.match(pattern).hasMatch()) {
        if (vulnerabilityDescription) {
            *vulnerabilityDescription = QCoreApplication::translate(
                "RegexSafetyEngine", "Exponential nested range quantifier detected in regex pattern.");
        }
        return false;
    }

    // Detect wildcard nested loops: (.*[a-z])+, (.+[0-9])*
    static const QRegularExpression kWildcardNestedLoop(QStringLiteral(R"(\(\s*\.[*+]\s*[^)]*\)\s*[\+\*])"));

    if (kWildcardNestedLoop.match(pattern).hasMatch()) {
        if (vulnerabilityDescription) {
            *vulnerabilityDescription = QCoreApplication::translate(
                "RegexSafetyEngine", "High-risk wildcard repetition loop detected in pattern.");
        }
        return false;
    }

    return true;
}

bool RegexSafetyEngine::executeWithTimeout(const std::function<void()> &operation, int timeoutMs) {
    if (!operation)
        return false;

    QElapsedTimer timer;
    timer.start();

    // Execute directly and check elapsed duration
    operation();

    return timer.elapsed() <= timeoutMs;
}
