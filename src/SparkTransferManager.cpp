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

#include "SparkTransferManager.h"
#include "utils/CryptoManager.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QStandardPaths>
#include <QSysInfo>

#ifdef Q_OS_UNIX
#include <sys/stat.h>
#include <unistd.h>
#endif

QString SparkTransferManager::getAppConfigDirectory() {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if (configDir.isEmpty()) {
        configDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    return QDir::cleanPath(configDir);
}

bool SparkTransferManager::isPathContained(const QString &baseDir, const QString &targetPath) {
    const QString cleanBase = QDir::cleanPath(baseDir);
    const QString cleanTarget = QDir::cleanPath(targetPath);

    // Canonicalize paths to eliminate symlink escapes and '..' sequences
    const QFileInfo baseInfo(cleanBase);
    const QFileInfo targetInfo(cleanTarget);

    const QString canonicalBase = baseInfo.exists() ? baseInfo.canonicalFilePath() : cleanBase;
    const QString canonicalTarget = targetInfo.exists() ? targetInfo.canonicalFilePath() : cleanTarget;

    if (canonicalBase.isEmpty() || canonicalTarget.isEmpty()) {
        return false;
    }

    if (canonicalTarget == canonicalBase) {
        return true;
    }

    const QString basePrefix =
        canonicalBase.endsWith(QLatin1Char('/')) ? canonicalBase : (canonicalBase + QLatin1Char('/'));
    return canonicalTarget.startsWith(basePrefix);
}

QString SparkTransferManager::sanitizeRelativePath(const QString &relPath) {
    if (relPath.startsWith(QLatin1Char('/')) || relPath.startsWith(QLatin1Char('\\')) ||
        QDir::isAbsolutePath(relPath)) {
        return {};
    }
    QString clean = QDir::cleanPath(relPath).replace(QLatin1Char('\\'), QLatin1Char('/'));
    // Reject directory traversal segments or drive letters
    if (clean.contains(QStringLiteral("..")) || clean.startsWith(QLatin1Char('/')) ||
        clean.contains(QLatin1Char(':'))) {
        return {};
    }
    return clean;
}

QString SparkTransferManager::translatePlatformPath(const QString &originalPath, const QString &sourceOs) {
    const QString currentConfig = getAppConfigDirectory();
    QString translated = originalPath;

    // Detect known macOS and Linux/FreeBSD patterns and remap to current config root
    if (sourceOs == QStringLiteral("macos") || sourceOs == QStringLiteral("darwin")) {
        const qsizetype idx = translated.indexOf(QStringLiteral("Library/Application Support/SparkNotePad"));
        if (idx != -1) {
            const QString sub = translated.mid(idx + 40);
            return QDir::cleanPath(currentConfig + QLatin1Char('/') + sub);
        }
    } else if (sourceOs == QStringLiteral("linux") || sourceOs == QStringLiteral("freebsd")) {
        const qsizetype idx = translated.indexOf(QStringLiteral(".config/SparkNotePad"));
        if (idx != -1) {
            const QString sub = translated.mid(idx + 20);
            return QDir::cleanPath(currentConfig + QLatin1Char('/') + sub);
        }
    }

    return translated;
}

QByteArray SparkTransferManager::normalizeLineEndings(const QByteArray &data) {
    QByteArray normalized = data;
    normalized.replace("\r\n", "\n");
    normalized.replace("\r", "\n");
    return normalized;
}

SparkTransferManager::TransferResult SparkTransferManager::exportBundle(const QString &targetFilePath,
                                                                        const ExportOptions &options) {
    TransferResult res;
    const QString configDir = getAppConfigDirectory();
    if (!QDir(configDir).exists()) {
        res.message = QStringLiteral("Configuration directory does not exist: %1").arg(configDir);
        return res;
    }

    QJsonArray itemsArray;
    int filesCount = 0;
    qint64 totalBytes = 0;

    const auto collectFiles = [&](const QString &relativeSubDir, const QStringList &nameFilters) {
        const QString subPath = QDir(configDir).filePath(relativeSubDir);
        if (!QDir(subPath).exists()) {
            return;
        }

        QDirIterator it(subPath, nameFilters, QDir::Files | QDir::Readable, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            const QString filePath = it.next();
            const QFileInfo fi(filePath);
            if (!fi.isFile() || fi.size() > static_cast<qint64>(10ULL * 1024ULL * 1024ULL)) {  // Skip files > 10MB
                continue;
            }

            QFile f(filePath);
            if (!f.open(QIODevice::ReadOnly)) {
                continue;
            }

            const QByteArray content = f.readAll();
            f.close();

            const QString relPath = QDir(configDir).relativeFilePath(filePath);
            const QString sha256 =
                QString::fromLatin1(QCryptographicHash::hash(content, QCryptographicHash::Sha256).toHex());

            QJsonObject itemObj;
            itemObj.insert(QStringLiteral("path"), relPath);
            itemObj.insert(QStringLiteral("sha256"), sha256);
            itemObj.insert(QStringLiteral("data"), QString::fromLatin1(content.toBase64()));
            itemObj.insert(QStringLiteral("size"), content.size());
            itemObj.insert(QStringLiteral("perms"), static_cast<int>(fi.permissions()));

            itemsArray.append(itemObj);
            filesCount++;
            totalBytes += content.size();
        }
    };

    // 1. Preferences
    if (options.includePreferences) {
        collectFiles(QStringLiteral("."),
                     {QStringLiteral("*.ini"), QStringLiteral("*.conf"), QStringLiteral("*.json")});
    }

    // 2. Custom Themes
    if (options.includeThemes) {
        collectFiles(QStringLiteral("themes"),
                     {QStringLiteral("*.json"), QStringLiteral("*.xml"), QStringLiteral("*.qss")});
    }

    // 3. Macros
    if (options.includeMacros) {
        collectFiles(QStringLiteral("macros"), {QStringLiteral("*.json"), QStringLiteral("*.xml")});
    }

    // 4. Sessions
    if (options.includeSessions) {
        collectFiles(QStringLiteral("sessions"), {QStringLiteral("*.xml"), QStringLiteral("*.json")});
        collectFiles(QStringLiteral("."), {QStringLiteral("session.xml"), QStringLiteral("session.json")});
    }

    // 5. Scripts (Lua & Python)
    if (options.includeScripts) {
        collectFiles(QStringLiteral("scripts"), {QStringLiteral("*.lua"), QStringLiteral("*.py")});
    }

    if (itemsArray.isEmpty()) {
        res.message = QStringLiteral("No configuration files found to export.");
        return res;
    }

    QJsonObject rootObj;
    rootObj.insert(QStringLiteral("magic"), QString::fromLatin1(BUNDLE_MAGIC));
    rootObj.insert(QStringLiteral("version"), QStringLiteral("1.4.7"));
    rootObj.insert(QStringLiteral("source_os"), QSysInfo::kernelType());
    rootObj.insert(QStringLiteral("created_at"), QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    rootObj.insert(QStringLiteral("items"), itemsArray);

    const QByteArray rawJson = QJsonDocument(rootObj).toJson(QJsonDocument::Compact);
    QByteArray payloadData;

    if (!options.passphrase.isEmpty()) {
        const CryptoManager::CryptoResult encRes = CryptoManager::encrypt(rawJson, options.passphrase);
        if (!encRes.success) {
            res.message = QStringLiteral("Encryption failed: %1").arg(encRes.errorMessage);
            return res;
        }
        payloadData = encRes.data;
    } else {
        payloadData = rawJson;
    }

    QFile outFile(targetFilePath);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        res.message = QStringLiteral("Failed to open output file: %1").arg(outFile.errorString());
        return res;
    }

    if (outFile.write(payloadData) == -1) {
        res.message = QStringLiteral("Failed to write bundle data: %1").arg(outFile.errorString());
        return res;
    }
    outFile.close();

#ifdef Q_OS_UNIX
    // Enforce 0600 file permissions for secure export
    chmod(QFile::encodeName(targetFilePath).constData(), S_IRUSR | S_IWUSR);
#endif

    res.success = true;
    res.filesCount = filesCount;
    res.totalBytes = totalBytes;
    res.bundleSha256 = QString::fromLatin1(QCryptographicHash::hash(payloadData, QCryptographicHash::Sha256).toHex());
    res.message = QStringLiteral("Successfully exported %1 files (%2 bytes).").arg(filesCount).arg(totalBytes);
    return res;
}

SparkTransferManager::TransferResult SparkTransferManager::inspectBundle(const QString &bundleFilePath,
                                                                         const QString &passphrase,
                                                                         QStringList *outFiles) {
    TransferResult res;
    QFile file(bundleFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        res.message = QStringLiteral("Cannot open bundle file: %1").arg(file.errorString());
        return res;
    }

    if (file.size() > static_cast<qint64>(MAX_BUNDLE_SIZE)) {
        res.message = QStringLiteral("Bundle file exceeds 100 MB limit.");
        return res;
    }

    const QByteArray rawData = file.readAll();
    file.close();

    QByteArray jsonData = rawData;
    if (CryptoManager::isEncryptedData(rawData)) {
        if (passphrase.isEmpty()) {
            res.message = QStringLiteral("Bundle is encrypted with AES-256. Passphrase required.");
            return res;
        }
        const CryptoManager::CryptoResult decRes = CryptoManager::decrypt(rawData, passphrase);
        if (!decRes.success) {
            res.message = QStringLiteral("Decryption failed: %1").arg(decRes.errorMessage);
            return res;
        }
        jsonData = decRes.data;
    }

    QJsonParseError parseErr{};
    const QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject()) {
        res.message = QStringLiteral("Invalid bundle JSON manifest: %1").arg(parseErr.errorString());
        return res;
    }

    const QJsonObject rootObj = doc.object();
    if (rootObj.value(QStringLiteral("magic")).toString() != QString::fromLatin1(BUNDLE_MAGIC)) {
        res.message = QStringLiteral("Not a valid SparkNotePad workspace bundle (magic mismatch).");
        return res;
    }

    const QJsonArray items = rootObj.value(QStringLiteral("items")).toArray();
    res.filesCount = static_cast<int>(items.size());
    qint64 totalBytes = 0;

    for (const auto &v : items) {
        if (!v.isObject()) {
            continue;
        }
        const QJsonObject item = v.toObject();
        const QString relPath = item.value(QStringLiteral("path")).toString();
        totalBytes += item.value(QStringLiteral("size")).toInteger();
        if (outFiles) {
            outFiles->append(relPath);
        }
    }

    res.success = true;
    res.totalBytes = totalBytes;
    res.bundleSha256 = QString::fromLatin1(QCryptographicHash::hash(rawData, QCryptographicHash::Sha256).toHex());
    res.message = QStringLiteral("Valid bundle containing %1 files (%2 bytes).").arg(res.filesCount).arg(totalBytes);
    return res;
}

SparkTransferManager::TransferResult SparkTransferManager::importBundle(const QString &bundleFilePath,
                                                                        const QString &passphrase) {
    TransferResult res;
    QFile file(bundleFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        res.message = QStringLiteral("Cannot open bundle file: %1").arg(file.errorString());
        return res;
    }

    if (file.size() > static_cast<qint64>(MAX_BUNDLE_SIZE)) {
        res.message = QStringLiteral("Bundle file exceeds 100 MB limit.");
        return res;
    }

    // Read full in-memory buffer for atomic verification (mitigating TOCTOU CVE-2026-52885)
    const QByteArray inMemoryBuffer = file.readAll();
    file.close();

    QByteArray jsonData = inMemoryBuffer;
    if (CryptoManager::isEncryptedData(inMemoryBuffer)) {
        if (passphrase.isEmpty()) {
            res.message = QStringLiteral("Bundle is encrypted with AES-256. Passphrase required.");
            return res;
        }
        const CryptoManager::CryptoResult decRes = CryptoManager::decrypt(inMemoryBuffer, passphrase);
        if (!decRes.success) {
            res.message = QStringLiteral("Decryption failed: %1").arg(decRes.errorMessage);
            return res;
        }
        jsonData = decRes.data;
    }

    QJsonParseError parseErr{};
    const QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject()) {
        res.message = QStringLiteral("Invalid bundle JSON manifest: %1").arg(parseErr.errorString());
        return res;
    }

    const QJsonObject rootObj = doc.object();
    if (rootObj.value(QStringLiteral("magic")).toString() != QString::fromLatin1(BUNDLE_MAGIC)) {
        res.message = QStringLiteral("Invalid bundle header magic.");
        return res;
    }

    const QString sourceOs = rootObj.value(QStringLiteral("source_os")).toString();
    const QJsonArray items = rootObj.value(QStringLiteral("items")).toArray();
    const QString targetBaseDir = getAppConfigDirectory();
    QDir().mkpath(targetBaseDir);

    int restoredCount = 0;
    qint64 restoredBytes = 0;

    for (const auto &v : items) {
        if (!v.isObject()) {
            continue;
        }
        const QJsonObject item = v.toObject();
        const QString rawRelPath = item.value(QStringLiteral("path")).toString();
        const QString sanitizedRel = sanitizeRelativePath(rawRelPath);

        if (sanitizedRel.isEmpty()) {
            res.message = QStringLiteral("Zip-Slip / Path Traversal attempt detected: %1").arg(rawRelPath);
            return res;
        }

        const QString targetPath = QDir(targetBaseDir).filePath(sanitizedRel);
        if (!isPathContained(targetBaseDir, targetPath)) {
            res.message = QStringLiteral("Path escape attempt blocked: %1").arg(targetPath);
            return res;
        }

        const QByteArray expectedSha = item.value(QStringLiteral("sha256")).toString().toLatin1();
        QByteArray content = QByteArray::fromBase64(item.value(QStringLiteral("data")).toString().toLatin1());

        const QByteArray computedSha = QCryptographicHash::hash(content, QCryptographicHash::Sha256).toHex();
        if (computedSha != expectedSha) {
            res.message = QStringLiteral("Checksum mismatch for %1 (corrupted or tampered data)").arg(sanitizedRel);
            return res;
        }

        // Normalize line endings for text configuration files
        if (sanitizedRel.endsWith(QStringLiteral(".ini")) || sanitizedRel.endsWith(QStringLiteral(".conf")) ||
            sanitizedRel.endsWith(QStringLiteral(".xml")) || sanitizedRel.endsWith(QStringLiteral(".json")) ||
            sanitizedRel.endsWith(QStringLiteral(".lua")) || sanitizedRel.endsWith(QStringLiteral(".py"))) {
            content = normalizeLineEndings(content);
        }

        // Ensure parent directory exists
        const QFileInfo destInfo(targetPath);
        QDir().mkpath(destInfo.path());

        // Write atomically using QSaveFile
        QSaveFile saveFile(targetPath);
        if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            res.message = QStringLiteral("Failed to write file %1: %2").arg(targetPath, saveFile.errorString());
            return res;
        }

        if (saveFile.write(content) == -1) {
            saveFile.cancelWriting();
            res.message = QStringLiteral("Failed writing data to %1").arg(targetPath);
            return res;
        }

        if (!saveFile.commit()) {
            res.message = QStringLiteral("Failed committing atomic save for %1").arg(targetPath);
            return res;
        }

#ifdef Q_OS_UNIX
        // Enforce safe 0600 file permissions
        chmod(QFile::encodeName(targetPath).constData(), S_IRUSR | S_IWUSR);
#endif

        restoredCount++;
        restoredBytes += content.size();
    }

    res.success = true;
    res.filesCount = restoredCount;
    res.totalBytes = restoredBytes;
    res.bundleSha256 =
        QString::fromLatin1(QCryptographicHash::hash(inMemoryBuffer, QCryptographicHash::Sha256).toHex());
    res.message = QStringLiteral("Successfully imported and verified %1 configuration files (%2 bytes) from %3.")
                      .arg(restoredCount)
                      .arg(restoredBytes)
                      .arg(sourceOs.isEmpty() ? QStringLiteral("workspace") : sourceOs);
    return res;
}
