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
#include <QStringList>
#include <QFileInfo>
#include <QFile>

class ConfigIntegrityGuard {
public:
    ConfigIntegrityGuard() = default;

    /**
     * @brief Validates external tool commands from configuration or session state.
     * Mitigates CVE-2026-48778 and CVE-2026-48800 by rejecting shell interpreter
     * hijacking, privilege escalation wrappers, and unsafe command injection.
     */
    static bool isExternalCommandSafe(const QString &commandLine, QString *rejectedReason = nullptr);

    /**
     * @brief Enforces strict 0600 (owner read/write only) permissions on configuration
     * and session files to protect against unauthorized tampering on multi-user systems.
     */
    static bool enforceSecureFilePermissions(const QString &filePath);

    /**
     * @brief Verifies that a session or configuration file is safe to load,
     * ensuring it is not pointing to sensitive system paths or world-writable.
     */
    static bool verifyFileIntegrity(const QString &filePath, QString *errorMsg = nullptr);
};
