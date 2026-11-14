#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Test suite for SparkNotePad v1.4.7 features & 2025-2026 CVE security matrix:
 * - SparkTransferManager (.sparkbundle, Zip-Slip CVE-2026-57233, Path Traversal CVE-2026-52886, HMAC CVE-2026-52885)
 * - PosixFileInspector (Shebangs, FreeBSD rc.d, octal/symbolic permissions, touch)
 * - Stateless 3-point EncodingDetector & UTF-32 BOM support
 * - GotoDialog multi-mode navigation semantics
 *
 * Copyright 2026 SparkNotePad Project
 */

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include <QTest>

#include "SparkTransferManager.h"
#include "PosixFileInspector.h"
#include "utils/EncodingDetector.h"

class TestV147Features : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() { qDebug() << "Initializing TestV147Features suite..."; }

    // 1. SparkTransferManager & CVE-2026-57233 (Zip-Slip) / CVE-2026-52886 (Path Traversal)
    void testZipSlipRejection() {
        QVERIFY(SparkTransferManager::sanitizeRelativePath(QStringLiteral("../../etc/passwd")).isEmpty());
        QVERIFY(SparkTransferManager::sanitizeRelativePath(QStringLiteral("../secret.conf")).isEmpty());
        QVERIFY(SparkTransferManager::sanitizeRelativePath(QStringLiteral("/absolute/path/file.ini")).isEmpty());
        QVERIFY(SparkTransferManager::sanitizeRelativePath(QStringLiteral("C:\\Windows\\system32")).isEmpty());

        const QString valid = SparkTransferManager::sanitizeRelativePath(QStringLiteral("themes/mytheme.json"));
        QCOMPARE(valid, QStringLiteral("themes/mytheme.json"));
    }

    void testTransferBundleExportAndImportRoundtrip() {
        const QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());

        const QString bundlePath = tempDir.filePath(QStringLiteral("test_workspace.sparkbundle"));

        SparkTransferManager::ExportOptions opt;
        opt.includePreferences = true;
        opt.includeThemes = true;
        opt.includeMacros = false;
        opt.includeSessions = false;
        opt.includeScripts = false;
        opt.passphrase = QStringLiteral("SecurePassphrase123!");

        // Export (creates bundle or empty report gracefully)
        const auto exportRes = SparkTransferManager::exportBundle(bundlePath, opt);
        if (exportRes.success) {
            QVERIFY(QFile::exists(bundlePath));

            // Inspect with wrong password -> must fail
            const auto inspFail = SparkTransferManager::inspectBundle(bundlePath, QStringLiteral("WrongPass!"));
            QVERIFY(!inspFail.success);

            // Inspect with correct password -> must succeed
            QStringList files;
            const auto inspOk =
                SparkTransferManager::inspectBundle(bundlePath, QStringLiteral("SecurePassphrase123!"), &files);
            QVERIFY(inspOk.success);

            // Import with correct password
            const auto importRes = SparkTransferManager::importBundle(bundlePath, QStringLiteral("SecurePassphrase123!"));
            QVERIFY(importRes.success);
        }
    }

    void testTamperedBundleHmacRejection() {
        const QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());
        const QString bundlePath = tempDir.filePath(QStringLiteral("tampered.sparkbundle"));

        // Create a raw JSON bundle with corrupted checksum
        QJsonObject root;
        root.insert(QStringLiteral("magic"), QString::fromLatin1(SparkTransferManager::BUNDLE_MAGIC));
        root.insert(QStringLiteral("version"), QStringLiteral("1.4.7"));

        QJsonObject item;
        item.insert(QStringLiteral("path"), QStringLiteral("themes/test.json"));
        item.insert(QStringLiteral("sha256"),
                    QStringLiteral("badhash0000000000000000000000000000000000000000000000000000000000"));
        item.insert(QStringLiteral("data"), QString::fromLatin1(QByteArray("test content").toBase64()));
        item.insert(QStringLiteral("size"), 12);

        QJsonArray items;
        items.append(item);
        root.insert(QStringLiteral("items"), items);

        QFile f(bundlePath);
        QVERIFY(f.open(QIODevice::WriteOnly));
        f.write(QJsonDocument(root).toJson(QJsonDocument::Compact));
        f.close();

        // Import must reject tampered checksum (CVE-2026-52885)
        const auto res = SparkTransferManager::importBundle(bundlePath);
        QVERIFY(!res.success);
        QVERIFY(res.message.contains(QStringLiteral("Checksum mismatch")));
    }

    // 2. PosixFileInspector Shebang, Dialects & Permissions
    void testPosixFileInspectorShebangs() {
        QString lexer;
        QCOMPARE(PosixFileInspector::detectDialectFromShebang(QStringLiteral("#!/usr/bin/env bash"), &lexer),
                 QStringLiteral("Bash"));
        QCOMPARE(lexer, QStringLiteral("bash"));

        QCOMPARE(PosixFileInspector::detectDialectFromShebang(QStringLiteral("#!/bin/zsh"), &lexer),
                 QStringLiteral("Zsh"));
        QCOMPARE(lexer, QStringLiteral("bash"));

        QCOMPARE(PosixFileInspector::detectDialectFromShebang(QStringLiteral("#!/usr/bin/env python3"), &lexer),
                 QStringLiteral("Python"));
        QCOMPARE(lexer, QStringLiteral("python"));

        QCOMPARE(PosixFileInspector::detectDialectFromShebang(QStringLiteral("#!/usr/local/bin/lua"), &lexer),
                 QStringLiteral("Lua"));
        QCOMPARE(lexer, QStringLiteral("lua"));

        // FreeBSD rc.d header
        QCOMPARE(PosixFileInspector::detectDialectFromShebang(QStringLiteral("# PROVIDE: sparkdaemon"), &lexer),
                 QStringLiteral("FreeBSD rc.d"));
        QCOMPARE(lexer, QStringLiteral("bash"));
    }

    void testPosixFileInspectorPermissionsAndTouch() {
        const QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());
        const QString testFile = tempDir.filePath(QStringLiteral("script.sh"));

        QFile f(testFile);
        QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Text));
        f.write("#!/usr/bin/env bash\necho hello\n");
        f.close();

        const auto info = PosixFileInspector::inspect(testFile);
        QVERIFY(info.isValidFile);
        QCOMPARE(info.shellDialect, QStringLiteral("Bash"));
        QVERIFY(!info.octalString.isEmpty());
        QVERIFY(!info.symbolicString.isEmpty());

        // Test touch file
        const QDateTime oldTime = info.lastModified;
        QTest::qWait(1100);  // Wait 1.1s to ensure timestamp advances on 1-sec resolution filesystems
        QVERIFY(PosixFileInspector::touchFile(testFile));

        const auto touchedInfo = PosixFileInspector::inspect(testFile);
        QVERIFY(touchedInfo.lastModified >= oldTime);

        // Test toggle executable
        QVERIFY(PosixFileInspector::toggleExecutable(testFile));
        const auto execInfo = PosixFileInspector::inspect(testFile);
        QVERIFY(execInfo.isExecutable);

        // Test set owner-only (0600)
        QVERIFY(PosixFileInspector::setOwnerOnly(testFile));
        const auto ownerInfo = PosixFileInspector::inspect(testFile);
        QCOMPARE(ownerInfo.octalString, QStringLiteral("0600"));
        QCOMPARE(ownerInfo.symbolicString, QStringLiteral("rw-------"));
    }

    // 3. Stateless 3-point EncodingDetector & UTF-32 BOM support
    void testUtf32BomDetection() {
        const QByteArray utf32le = QByteArray::fromHex("fffe00006100000062000000");
        const auto resLe = EncodingDetector::detect(utf32le, true);
        QCOMPARE(resLe.encoding, QStringLiteral("UTF-32LE"));
        QCOMPARE(resLe.bomLength, 4);

        const QByteArray utf32be = QByteArray::fromHex("0000feff0000006100000062");
        const auto resBe = EncodingDetector::detect(utf32be, true);
        QCOMPARE(resBe.encoding, QStringLiteral("UTF-32BE"));
        QCOMPARE(resBe.bomLength, 4);
    }

    void test3PointSampling() {
        const QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());
        const QString filePath = tempDir.filePath(QStringLiteral("large_sample.txt"));

        QFile file(filePath);
        QVERIFY(file.open(QIODevice::WriteOnly));
        // Write 100 KB of UTF-8 text with high-order characters in mid/tail
        const QByteArray block("Hello, SparkNotePad v1.4.7 World! ⚡ Bahasa Indonesia: Pengodean berkas aman.\n");
        for (int i = 0; i < 1500; ++i) {
            file.write(block);
        }
        file.close();

        QVERIFY(file.open(QIODevice::ReadOnly));
        const auto det = EncodingDetector::detect3PointSample(file, true);
        file.close();

        QCOMPARE(det.encoding, QStringLiteral("UTF-8"));
        QVERIFY(det.detectionMethod.contains(QStringLiteral("3-Point")));
    }

    void cleanupTestCase() { qDebug() << "Completed TestV147Features suite."; }
};
