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

#include <QTest>
#include <QObject>
#include "dialogs/QuickSwitcherDialog.h"

class TestQuickSwitcher : public QObject {
    Q_OBJECT

private slots:
    void testFuzzyMatchingSubsequence() const {
        const int score1 = QuickSwitcherDialog::fuzzyScore(QStringLiteral("mnw"), QStringLiteral("MainWindow.cpp"));
        QVERIFY(score1 > 0);

        const int score2 =
            QuickSwitcherDialog::fuzzyScore(QStringLiteral("edp"), QStringLiteral("EditorConfigParser.cpp"));
        QVERIFY(score2 > 0);
    }

    void testNonMatchingAndOrderSensitivity() const {
        // "xyz" does not appear in MainWindow.cpp
        const int scoreFail = QuickSwitcherDialog::fuzzyScore(QStringLiteral("xyz"), QStringLiteral("MainWindow.cpp"));
        QVERIFY(scoreFail <= 0);

        // "wmn" does not appear in order in MainWindow.cpp
        const int scoreOrderFail =
            QuickSwitcherDialog::fuzzyScore(QStringLiteral("wmn"), QStringLiteral("MainWindow.cpp"));
        QVERIFY(scoreOrderFail <= 0);
    }

    void testExactAndPrefixScoring() const {
        const int exactScore = QuickSwitcherDialog::fuzzyScore(QStringLiteral("main.cpp"), QStringLiteral("main.cpp"));
        const int subseqScore = QuickSwitcherDialog::fuzzyScore(QStringLiteral("mcp"), QStringLiteral("main.cpp"));
        QVERIFY(exactScore > subseqScore);

        const int prefixScore =
            QuickSwitcherDialog::fuzzyScore(QStringLiteral("main"), QStringLiteral("MainWindow.cpp"));
        QVERIFY(prefixScore > subseqScore);
    }

    void testFilenameVsPathBonus() const {
        // Query "test":
        // Item A: "src/test.cpp" (filename is "test.cpp")
        // Item B: "src/testing_dir/worker.cpp" (match in path, but filename is "worker.cpp")
        const int scoreA = QuickSwitcherDialog::fuzzyScore(QStringLiteral("test"), QStringLiteral("src/test.cpp"));
        const int scoreB =
            QuickSwitcherDialog::fuzzyScore(QStringLiteral("test"), QStringLiteral("src/testing_dir/worker.cpp"));
        QVERIFY(scoreA > scoreB);
    }

    void testCaseInsensitivity() const {
        const int scoreLower =
            QuickSwitcherDialog::fuzzyScore(QStringLiteral("mainwindow"), QStringLiteral("MainWindow.cpp"));
        const int scoreUpper =
            QuickSwitcherDialog::fuzzyScore(QStringLiteral("MAINWINDOW"), QStringLiteral("MainWindow.cpp"));
        QVERIFY(scoreLower > 0);
        QVERIFY(scoreUpper > 0);
    }

    void testFilterItemsSorting() const {
        const QVector<QuickSwitcherDialog::Item> items = {
            {QStringLiteral("OtherFile.h"), QStringLiteral("/project/src/OtherFile.h"), nullptr, 0},
            {QStringLiteral("MainWindow.cpp"), QStringLiteral("/project/src/MainWindow.cpp"), nullptr, 0},
            {QStringLiteral("MainWindow.h"), QStringLiteral("/project/src/MainWindow.h"), nullptr, 0}};

        const auto results = QuickSwitcherDialog::filterItems(items, QStringLiteral("MainWindow.cpp"));
        QVERIFY(!results.isEmpty());
        QCOMPARE(results.first().title, QStringLiteral("MainWindow.cpp"));
    }
};
