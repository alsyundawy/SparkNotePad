#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QColor>
#include <QObject>
#include <QSettings>
#include <QTest>
#include "features/TabColorManager.h"

class TestTabColorManager : public QObject {
    Q_OBJECT

private slots:
    void init() const {
        QSettings settings;
        settings.remove(QStringLiteral("TabColors"));
    }

    void testDefaultPaletteCount() const {
        TabColorManager manager;
        const auto palette = manager.paletteColors();
        QCOMPARE(palette.size(), 8);
        for (const auto &c : palette) {
            QVERIFY(c.isValid());
        }
    }

    void testSetAndGetColor() const {
        TabColorManager manager;
        const QString path = QStringLiteral("/demo/file.cpp");
        QVERIFY(!manager.hasTabColor(path));
        QVERIFY(!manager.tabColor(path).isValid());

        const QColor red(QStringLiteral("#cf222e"));
        manager.setTabColor(path, red);

        QVERIFY(manager.hasTabColor(path));
        QCOMPARE(manager.tabColor(path), red);
    }

    void testClearColor() const {
        TabColorManager manager;
        const QString path = QStringLiteral("/demo/test.py");
        manager.setTabColor(path, QColor(QStringLiteral("#1a7f37")));
        QVERIFY(manager.hasTabColor(path));

        manager.clearTabColor(path);
        QVERIFY(!manager.hasTabColor(path));
        QVERIFY(!manager.tabColor(path).isValid());
    }

    void testPersistence() const {
        {
            TabColorManager manager1;
            manager1.setTabColor(QStringLiteral("/persist/doc.md"), QColor(QStringLiteral("#8250df")));
            manager1.setTabColor(QStringLiteral("/persist/main.rs"), QColor(QStringLiteral("#0969da")));
        }

        TabColorManager manager2;
        manager2.loadSettings();
        QVERIFY(manager2.hasTabColor(QStringLiteral("/persist/doc.md")));
        QCOMPARE(manager2.tabColor(QStringLiteral("/persist/doc.md")), QColor(QStringLiteral("#8250df")));
        QVERIFY(manager2.hasTabColor(QStringLiteral("/persist/main.rs")));
        QCOMPARE(manager2.tabColor(QStringLiteral("/persist/main.rs")), QColor(QStringLiteral("#0969da")));
    }
};
