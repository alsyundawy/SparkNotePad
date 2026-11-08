#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QTemporaryDir>
#include <QTest>
#include "features/AutosaveRecoveryManager.h"

class TestAutosaveRecoveryManager : public QObject {
    Q_OBJECT

private slots:
    void testSnapshotWritingAndPermissions() const {
        QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());

        AutosaveRecoveryManager manager;
        manager.setRecoveryDirectory(tempDir.path());

        const QString samplePath = QStringLiteral("/test/project/file.cpp");
        const QString sampleText = QStringLiteral("int main() { return 0; }\n");

        const QString snapshotFile = manager.saveSnapshot(samplePath, QStringLiteral("file.cpp"), sampleText, 42);
        QVERIFY(!snapshotFile.isEmpty());
        QVERIFY(QFile::exists(snapshotFile));

        // Check permissions (must be readable and writable by owner)
        QFile file(snapshotFile);
        const auto perms = file.permissions();
        QVERIFY(perms & QFileDevice::ReadOwner);
        QVERIFY(perms & QFileDevice::WriteOwner);
#if !defined(Q_OS_WIN)
        // Verify group and other permissions are NOT set (0600)
        QVERIFY(!(perms & QFileDevice::ReadGroup));
        QVERIFY(!(perms & QFileDevice::WriteGroup));
        QVERIFY(!(perms & QFileDevice::ReadOther));
        QVERIFY(!(perms & QFileDevice::WriteOther));
#endif

        // Verify recoverable snapshots enumeration
        const auto snapshots = manager.recoverableSnapshots();
        QCOMPARE(snapshots.size(), 1);
        QCOMPARE(snapshots.first().originalPath, samplePath);
        QCOMPARE(snapshots.first().title, QStringLiteral("file.cpp"));
        QCOMPARE(snapshots.first().content, sampleText);
        QCOMPARE(snapshots.first().cursorPosition, 42);

        // Remove snapshot
        manager.removeSnapshot(samplePath);
        QVERIFY(!QFile::exists(snapshotFile));
        QCOMPARE(manager.recoverableSnapshots().size(), 0);
    }

    void testUntitledDraftRecovery() const {
        QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());

        AutosaveRecoveryManager manager;
        manager.setRecoveryDirectory(tempDir.path());

        const QString untitledId = QStringLiteral("untitled_1");
        const QString draftContent = QStringLiteral("Some unsaved thoughts\nNotes for v1.4.5");

        const QString snapshotFile = manager.saveSnapshot(QString(), untitledId, draftContent, 10);
        QVERIFY(!snapshotFile.isEmpty());
        QVERIFY(QFile::exists(snapshotFile));

        const auto snapshots = manager.recoverableSnapshots();
        QCOMPARE(snapshots.size(), 1);
        QVERIFY(snapshots.first().isUntitled);
        QCOMPARE(snapshots.first().title, untitledId);
        QCOMPARE(snapshots.first().content, draftContent);

        manager.clearSnapshots();
        QCOMPARE(manager.recoverableSnapshots().size(), 0);
    }
};
