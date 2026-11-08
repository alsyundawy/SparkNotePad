#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QObject>
#include <QTest>
#include "utils/ExtendedSearchParser.h"

class TestExtendedSearchParser : public QObject {
    Q_OBJECT

private slots:
    void testBasicEscapes() const {
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\n)")), QStringLiteral("\n"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\r)")), QStringLiteral("\r"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\t)")), QStringLiteral("\t"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\b)")), QStringLiteral("\b"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\f)")), QStringLiteral("\f"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\v)")), QStringLiteral("\v"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\0)")), QString(QChar(0)));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\\)")), QStringLiteral(R"(\)"));
    }

    void testHexEscapes() const {
        // \x41 = 'A', \x20 = ' ', \x1B = ESC (0x1B)
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\x41)")), QStringLiteral("A"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\x20)")), QStringLiteral(" "));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\x61)")), QStringLiteral("a"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(Code:\x1b[0m)")), QStringLiteral("Code:\x1b[0m"));
    }

    void testDecimalEscapes() const {
        // \d065 = 'A', \d65 = 'A', \d010 = '\n'
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\d065)")), QStringLiteral("A"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\d65)")), QStringLiteral("A"));
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\d010)")), QStringLiteral("\n"));
    }

    void testEscapedBackslashPreservation() const {
        // \\n should become \n (a literal backslash followed by n, NOT a newline)
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\\n)")), QStringLiteral(R"(\n)"));
        // \\x41 should become \x41 (NOT 'A')
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\\x41)")), QStringLiteral(R"(\x41)"));
        // \\\\ should become \\
        QCOMPARE(ExtendedSearchParser::parse(QStringLiteral(R"(\\\\)"))), QStringLiteral(R"(\\)"));
    }

    void testMixedString() const {
        const QString input = QStringLiteral(R"(Hello\tWorld\r\nHex:\x41\x42 Dec:\d067\d068 end)");
        const QString expected = QStringLiteral("Hello\tWorld\r\nHex:AB Dec:CD end");
        QCOMPARE(ExtendedSearchParser::parse(input), expected);
    }
};
