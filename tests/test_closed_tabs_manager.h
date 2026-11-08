#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QColor>
#include <QJsonArray>
#include <QObject>
#include <QTest>
#include "features/ClosedTabsManager.h"

class TestClosedTabsManager : public QObject {
    Q_OBJECT

private slots:
    void testEmptyStack() const {
        ClosedTabsManager manager;
        QCOMPARE(manager.count(), 0);
        QCOMPARE(manager.canReopen(), false);
        QVERIFY(!manager.popClosedTab().has_value());
    }

    void testPushPopLIFO() const {
        ClosedTabsManager manager;
        ClosedTabInfo info1;
        info1.filePath = QStringLiteral("/path/to/file1.cpp");
        info1.cursorPosition = 120;
        info1.firstVisibleLine = 10;
        info1.tabColor = QColor(Qt::red);

        ClosedTabInfo info2;
        info2.filePath = QStringLiteral("/path/to/file2.h");
        info2.cursorPosition = 450;
        info2.firstVisibleLine = 25;
        info2.tabColor = QColor(Qt::blue);

        manager.pushClosedTab(info1);
        manager.pushClosedTab(info2);

        QCOMPARE(manager.count(), 2);
        QCOMPARE(manager.canReopen(), true);

        const auto popped1 = manager.popClosedTab();
        QVERIFY(popped1.has_value());
        QCOMPARE(popped1->filePath, QStringLiteral("/path/to/file2.h"));
        QCOMPARE(popped1->cursorPosition, 450);
        QCOMPARE(popped1->firstVisibleLine, 25);
        QCOMPARE(popped1->tabColor, QColor(Qt::blue));

        const auto popped2 = manager.popClosedTab();
        QVERIFY(popped2.has_value());
        QCOMPARE(popped2->filePath, QStringLiteral("/path/to/file1.cpp"));
        QCOMPARE(popped2->cursorPosition, 120);

        QCOMPARE(manager.canReopen(), false);
        QCOMPARE(manager.count(), 0);
    }

    void testMaxCapacityCappedAt25() const {
        ClosedTabsManager manager;
        for (int i = 0; i < 30; ++i) {
            ClosedTabInfo info;
            info.filePath = QStringLiteral("/path/file%1.txt").arg(i);
            info.cursorPosition = i * 10;
            manager.pushClosedTab(info);
        }

        QCOMPARE(manager.count(), 25);
        // The last pushed was file29, top of stack should be file29
        const auto top = manager.popClosedTab();
        QVERIFY(top.has_value());
        QCOMPARE(top->filePath, QStringLiteral("/path/file29.txt"));

        // Drain until empty, should have 24 remaining
        int drained = 0;
        QString lastDrainedPath;
        while (manager.canReopen()) {
            const auto item = manager.popClosedTab();
            lastDrainedPath = item->filePath;
            drained++;
        }
        QCOMPARE(drained, 24);
        // Oldest retained should be file5 (files 0-4 evicted)
        QCOMPARE(lastDrainedPath, QStringLiteral("/path/file5.txt"));
    }

    void testJsonSerialization() const {
        ClosedTabsManager manager1;
        ClosedTabInfo info;
        info.filePath = QStringLiteral("/demo/test.py");
        info.cursorPosition = 99;
        info.firstVisibleLine = 12;
        info.tabColor = QColor(QStringLiteral("#1a7f37"));
        info.encoding = QStringLiteral("UTF-8");
        info.eolMode = 1;
        info.timestamp = 1731000000;
        manager1.pushClosedTab(info);

        const QJsonArray arr = manager1.toJson();
        QCOMPARE(arr.size(), 1);

        ClosedTabsManager manager2;
        manager2.fromJson(arr);
        QCOMPARE(manager2.count(), 1);

        const auto restored = manager2.popClosedTab();
        QVERIFY(restored.has_value());
        QCOMPARE(restored->filePath, QStringLiteral("/demo/test.py"));
        QCOMPARE(restored->cursorPosition, 99);
        QCOMPARE(restored->firstVisibleLine, 12);
        QCOMPARE(restored->tabColor, QColor(QStringLiteral("#1a7f37")));
        QCOMPARE(restored->encoding, QStringLiteral("UTF-8"));
        QCOMPARE(restored->eolMode, 1);
    }

    void testClear() const {
        ClosedTabsManager manager;
        ClosedTabInfo info;
        info.filePath = QStringLiteral("/test.txt");
        manager.pushClosedTab(info);
        QCOMPARE(manager.count(), 1);
        manager.clear();
        QCOMPARE(manager.count(), 0);
        QCOMPARE(manager.canReopen(), false);
    }
};
