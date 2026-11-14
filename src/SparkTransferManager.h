#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Implements Workspace System Transfer & Portable Configuration Bundle (.sparkbundle)
 * with platform path translation (macOS <-> FreeBSD <-> Linux), Zip-Slip containment,
 * in-memory HMAC verification, and optional AES-256-CBC + HMAC-SHA256 encryption.
 *
 * Copyright 2025-2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef SPARKTRANSFERMANAGER_H
#define SPARKTRANSFERMANAGER_H

#include <QByteArray>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>

class SparkTransferManager {
public:
    static constexpr const char *BUNDLE_MAGIC = "SPKBUNDLE01";
    static constexpr size_t MAX_BUNDLE_SIZE = 100ULL * 1024ULL * 1024ULL;  // 100 MB safety cap

    struct ExportOptions {
        bool includePreferences{true};
        bool includeThemes{true};
        bool includeMacros{true};
        bool includeSessions{true};
        bool includeScripts{true};
        QString passphrase;  // Optional AES-256 encryption
    };

    struct TransferResult {
        bool success{false};
        QString message;
        int filesCount{0};
        qint64 totalBytes{0};
        QString bundleSha256;
    };

    // Bundle Operations
    static TransferResult exportBundle(const QString &targetFilePath, const ExportOptions &options);
    static TransferResult importBundle(const QString &bundleFilePath, const QString &passphrase = QString());
    static TransferResult inspectBundle(const QString &bundleFilePath, const QString &passphrase = QString(),
                                        QStringList *outFiles = nullptr);

    // Security & Path Normalization Helpers (CVE-2026-57233, CVE-2026-52886, CVE-2026-52885)
    static bool isPathContained(const QString &baseDir, const QString &targetPath);
    static QString sanitizeRelativePath(const QString &relPath);
    static QString getAppConfigDirectory();
    static QString translatePlatformPath(const QString &originalPath, const QString &sourceOs);
    static QByteArray normalizeLineEndings(const QByteArray &data);
};

#endif  // SPARKTRANSFERMANAGER_H
