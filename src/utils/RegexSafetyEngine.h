#pragma once
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

#include <QString>
#include <QRegularExpression>
#include <chrono>
#include <functional>

class RegexSafetyEngine {
public:
    RegexSafetyEngine() = default;

    /**
     * @brief Inspects a regular expression for catastrophic backtracking (ReDoS) patterns.
     * Detects nested quantifiers like (a+)+, (.*a)+, or exponential repetition loops.
     * Mitigates CVE-2026-4744 and Scintilla regex engine freezes.
     */
    static bool isSafeRegex(const QString &pattern, QString *vulnerabilityDescription = nullptr);

    /**
     * @brief Bounds execution of a search or replace iteration within a hard timeout.
     * @param timeoutMs Maximum allowed execution time in milliseconds (default: 2000 ms).
     */
    static bool executeWithTimeout(const std::function<void()> &operation, int timeoutMs = 2000);
};
