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
    if (const auto match = kNestedQuantifier.match(pattern); match.hasMatch()) {
        if (vulnerabilityDescription) {
            *vulnerabilityDescription =
                QCoreApplication::translate("RegexSafetyEngine",
                                            "Catastrophic backtracking vulnerability detected: nested quantifiers "
                                            "'(%1)' can cause application hang (ReDoS).")
                    .arg(match.captured(0));
        }
        return false;
    }

    // Detect nested range quantifiers: (x{1,5})+ or (x+){2,}
    static const QRegularExpression kNestedRangeQuantifier(QStringLiteral(R"(\([^\)\\]*\{\d+,?\d*\}\s*\)\s*[\+\*\{])"));
    if (const auto match = kNestedRangeQuantifier.match(pattern); match.hasMatch()) {
        if (vulnerabilityDescription) {
            *vulnerabilityDescription = QCoreApplication::translate(
                "RegexSafetyEngine", "Exponential nested range quantifier detected in regex pattern.");
        }
        return false;
    }

    // Detect wildcard nested loops: (.*[a-z])+, (.+[0-9])*
    static const QRegularExpression kWildcardNestedLoop(QStringLiteral(R"(\(\s*\.[*+]\s*[^)]*\)\s*[\+\*])"));
    if (const auto match = kWildcardNestedLoop.match(pattern); match.hasMatch()) {
        if (vulnerabilityDescription) {
            *vulnerabilityDescription = QCoreApplication::translate(
                "RegexSafetyEngine", "High-risk wildcard repetition loop detected in pattern.");
        }
        return false;
    }

    return true;
}

bool RegexSafetyEngine::executeWithTimeout(const std::function<void()> &operation, int timeoutMs) {
    if (!operation) {
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    // Execute directly and check elapsed duration
    operation();

    return timer.elapsed() <= timeoutMs;
}

QString RegexSafetyEngine::buildFuzzySubsequencePattern(const QString &query) {
    if (query.isEmpty()) {
        return QString();
    }

    const QString trimmed = query.left(64);
    QStringList tokens;

    for (const QChar &ch : trimmed) {
        if (ch == QLatin1Char('\r') || ch == QLatin1Char('\n')) {
            continue;
        }
        tokens << QRegularExpression::escape(QString(ch));
    }

    if (tokens.isEmpty()) {
        return QString();
    }

    return tokens.join(QStringLiteral(R"([^\r\n]*?)"));
}
