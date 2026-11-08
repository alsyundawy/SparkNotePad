#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QDir>
#include <QFile>
#include <QObject>
#include <QTemporaryDir>
#include <QTest>
#include "features/CorrespondingFileSwitcher.h"

class TestCorrespondingFileSwitcher : public QObject {
    Q_OBJECT

private slots:
    void testSameDirectoryCppHeaderToggle() const {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString hPath = dir.filePath(QStringLiteral("Widget.h"));
        const QString cppPath = dir.filePath(QStringLiteral("Widget.cpp"));

        QFile f1(hPath);
        QVERIFY(f1.open(QIODevice::WriteOnly));
        (void)f1.write("// header\n");
        f1.close();

        QFile f2(cppPath);
        QVERIFY(f2.open(QIODevice::WriteOnly));
        (void)f2.write("// source\n");
        f2.close();

        // From .h -> .cpp
        const QString foundCpp = CorrespondingFileSwitcher::findCorrespondingPath(hPath);
        QCOMPARE(foundCpp, cppPath);

        // From .cpp -> .h
        const QString foundH = CorrespondingFileSwitcher::findCorrespondingPath(cppPath);
        QCOMPARE(foundH, hPath);
    }

    void testIncludeAndSrcDirectoryMatching() const {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString root = dir.path();
        (void)QDir(root).mkdir(QStringLiteral("include"));
        (void)QDir(root).mkdir(QStringLiteral("src"));

        const QString hPath = dir.filePath(QStringLiteral("include/Document.h"));
        const QString cppPath = dir.filePath(QStringLiteral("src/Document.cpp"));

        QFile f1(hPath);
        QVERIFY(f1.open(QIODevice::WriteOnly));
        (void)f1.write("// doc header\n");
        f1.close();

        QFile f2(cppPath);
        QVERIFY(f2.open(QIODevice::WriteOnly));
        (void)f2.write("// doc source\n");
        f2.close();

        // From include/Document.h -> src/Document.cpp
        const QString foundCpp = CorrespondingFileSwitcher::findCorrespondingPath(hPath);
        QCOMPARE(foundCpp, cppPath);

        // From src/Document.cpp -> include/Document.h
        const QString foundH = CorrespondingFileSwitcher::findCorrespondingPath(cppPath);
        QCOMPARE(foundH, hPath);
    }

    void testTypeScriptTestFileMatching() const {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString tsPath = dir.filePath(QStringLiteral("service.ts"));
        const QString specPath = dir.filePath(QStringLiteral("service.spec.ts"));

        QFile f1(tsPath);
        QVERIFY(f1.open(QIODevice::WriteOnly));
        (void)f1.write("// ts\n");
        f1.close();

        QFile f2(specPath);
        QVERIFY(f2.open(QIODevice::WriteOnly));
        (void)f2.write("// spec\n");
        f2.close();

        const QString foundSpec = CorrespondingFileSwitcher::findCorrespondingPath(tsPath);
        QCOMPARE(foundSpec, specPath);

        const QString foundTs = CorrespondingFileSwitcher::findCorrespondingPath(specPath);
        QCOMPARE(foundTs, tsPath);
    }

    void testNonExistingFileReturnsEmpty() const {
        const QString nonExistent = QStringLiteral("/non/existent/random/file.cpp");
        const QString result = CorrespondingFileSwitcher::findCorrespondingPath(nonExistent);
        QVERIFY(result.isEmpty());
    }
};
