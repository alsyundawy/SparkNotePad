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
#include "features/EditorConfigParser.h"

class TestEditorConfigParser : public QObject {
    Q_OBJECT

private slots:
    void testBasicResolutionAndRootStopping() const {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString rootConfigPath = dir.filePath(QStringLiteral(".editorconfig"));
        QFile fRoot(rootConfigPath);
        QVERIFY(fRoot.open(QIODevice::WriteOnly | QIODevice::Text));
        (void)fRoot.write(
            "root = true\n\n"
            "[*]\n"
            "end_of_line = lf\n"
            "insert_final_newline = true\n"
            "trim_trailing_whitespace = true\n\n"
            "[*.cpp]\n"
            "indent_style = space\n"
            "indent_size = 4\n"
            "tab_width = 4\n");
        fRoot.close();

        const QString cppFile = dir.filePath(QStringLiteral("main.cpp"));
        const auto settings = EditorConfigParser::resolveSettings(cppFile);

        QCOMPARE(settings.indentStyle, EditorConfigParser::IndentStyle::Space);
        QCOMPARE(settings.indentSize, 4);
        QCOMPARE(settings.tabWidth, 4);
        QCOMPARE(settings.endOfLine, EditorConfigParser::EndOfLine::Lf);
        QVERIFY(settings.trimTrailingWhitespace);
        QVERIFY(settings.insertFinalNewline);
    }

    void testBraceExpansionAndInheritance() const {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString rootConfigPath = dir.filePath(QStringLiteral(".editorconfig"));
        QFile fRoot(rootConfigPath);
        QVERIFY(fRoot.open(QIODevice::WriteOnly | QIODevice::Text));
        (void)fRoot.write(
            "root = true\n\n"
            "[*]\n"
            "indent_style = tab\n\n"
            "[*.{js,ts}]\n"
            "indent_style = space\n"
            "indent_size = 2\n");
        fRoot.close();

        const QString tsFile = dir.filePath(QStringLiteral("app.ts"));
        const auto tsSettings = EditorConfigParser::resolveSettings(tsFile);
        QCOMPARE(tsSettings.indentStyle, EditorConfigParser::IndentStyle::Space);
        QCOMPARE(tsSettings.indentSize, 2);

        const QString pyFile = dir.filePath(QStringLiteral("script.py"));
        const auto pySettings = EditorConfigParser::resolveSettings(pyFile);
        QCOMPARE(pySettings.indentStyle, EditorConfigParser::IndentStyle::Tab);
    }

    void testChildDirectoryOverride() const {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        // Root config
        const QString rootConfigPath = dir.filePath(QStringLiteral(".editorconfig"));
        QFile fRoot(rootConfigPath);
        QVERIFY(fRoot.open(QIODevice::WriteOnly | QIODevice::Text));
        (void)fRoot.write("root = true\n\n[*]\nindent_size = 8\n");
        fRoot.close();

        // Subdir config
        (void)QDir(dir.path()).mkdir(QStringLiteral("sub"));
        const QString subConfigPath = dir.filePath(QStringLiteral("sub/.editorconfig"));
        QFile fSub(subConfigPath);
        QVERIFY(fSub.open(QIODevice::WriteOnly | QIODevice::Text));
        (void)fSub.write("[*]\nindent_size = 2\n");
        fSub.close();

        const QString subFile = dir.filePath(QStringLiteral("sub/nested.txt"));
        const auto subSettings = EditorConfigParser::resolveSettings(subFile);
        QCOMPARE(subSettings.indentSize, 2);
    }
};
