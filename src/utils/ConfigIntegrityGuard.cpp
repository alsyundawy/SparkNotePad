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

#include "ConfigIntegrityGuard.h"
#include <QRegularExpression>
#include <QCoreApplication>

bool ConfigIntegrityGuard::isExternalCommandSafe(const QString &commandLine, QString *rejectedReason) {
    const QString trimmed = commandLine.trimmed();
    if (trimmed.isEmpty()) {
        if (rejectedReason)
            *rejectedReason = QCoreApplication::translate("ConfigIntegrityGuard", "Empty command string.");
        return false;
    }

    // List of banned binary / privilege escalation targets (CVE-2026-48778 / CVE-2026-48800)
    static const QStringList kBannedPrefixes = {
        QStringLiteral("sudo "),   QStringLiteral("sudo\t"),         QStringLiteral("doas "),
        QStringLiteral("su "),     QStringLiteral("su\t"),           QStringLiteral("pkexec "),
        QStringLiteral("cmd.exe"), QStringLiteral("powershell.exe"), QStringLiteral("powershell "),
        QStringLiteral("pwsh "),   QStringLiteral("pwsh.exe")};

    const QString lower = trimmed.toLower();

    const auto bannedIt =
        std::find_if(kBannedPrefixes.cbegin(), kBannedPrefixes.cend(),
                     [&lower](const QString &banned) { return lower.startsWith(banned) || lower == banned.trimmed(); });
    if (bannedIt != kBannedPrefixes.cend()) {
        if (rejectedReason) {
            *rejectedReason = QCoreApplication::translate(
                                  "ConfigIntegrityGuard",
                                  "Execution of interpreter/privilege escalation '%1' is blocked for security.")
                                  .arg(bannedIt->trimmed());
        }
        return false;
    }

    // Check for suspicious direct shell chaining attempting pipe injection
    static const QRegularExpression kDangerousChaining(
        QStringLiteral("(^|[;&|])\\s*(sudo|doas|pkexec|su|rm\\s+-rf\\s+/)\\b"),
        QRegularExpression::CaseInsensitiveOption);

    if (kDangerousChaining.match(trimmed).hasMatch()) {
        if (rejectedReason) {
            *rejectedReason =
                QCoreApplication::translate("ConfigIntegrityGuard", "Disallowed dangerous command sequence detected.");
        }
        return false;
    }

    return true;
}

bool ConfigIntegrityGuard::enforceSecureFilePermissions(const QString &filePath) {
    QFileInfo fi(filePath);
    if (!fi.exists()) {
        return false;
    }

    // Set Owner Read & Write only (0600)
    QFile::Permissions safePermissions = QFileDevice::ReadOwner | QFileDevice::WriteOwner;
    return QFile::setPermissions(filePath, safePermissions);
}

bool ConfigIntegrityGuard::verifyFileIntegrity(const QString &filePath, QString *errorMsg) {
    QFileInfo fi(filePath);
    if (!fi.exists()) {
        if (errorMsg)
            *errorMsg = QCoreApplication::translate("ConfigIntegrityGuard", "File does not exist: %1").arg(filePath);
        return false;
    }

    // Check against canonical system targets to prevent symlink traversal attacks
    const QString canonical = fi.canonicalFilePath();
    static const QStringList kSensitiveSystemPaths = {QStringLiteral("/etc/passwd"), QStringLiteral("/etc/shadow"),
                                                      QStringLiteral("/etc/sudoers"), QStringLiteral("/bin/sh"),
                                                      QStringLiteral("/bin/bash")};

    const bool isSensitive = std::any_of(kSensitiveSystemPaths.cbegin(), kSensitiveSystemPaths.cend(),
                                         [&canonical](const QString &sysPath) { return canonical == sysPath; });
    if (isSensitive) {
        if (errorMsg)
            *errorMsg =
                QCoreApplication::translate("ConfigIntegrityGuard", "Attempted access to restricted system file: %1")
                    .arg(canonical);
        return false;
    }

    // Enforce 0600 permissions if file is currently world-writable
    if (fi.permissions().testFlag(QFileDevice::WriteOther)) {
        enforceSecureFilePermissions(filePath);
    }

    return true;
}
