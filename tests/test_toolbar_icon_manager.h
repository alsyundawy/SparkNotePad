#pragma once
/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <QObject>
#include <QTest>
#include <QToolBar>
#include <QAction>
#include "features/ToolbarIconManager.h"

class TestToolbarIconManager : public QObject {
    Q_OBJECT

private slots:
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testBuiltInPacksExist() const {
        const auto *manager = ToolbarIconManager::instance();
        const QList<ToolbarIconPack> packs = manager->availablePacks();

        // 21 built-in packs (including windows7, windows11, and notepadPlusPlus)
        QCOMPARE(packs.size(), 21);

        const QStringList expectedPackIds = {
            QStringLiteral("modernRibbon"),     QStringLiteral("fugue"),
            QStringLiteral("aero_vista"),       QStringLiteral("aero_win7"),
            QStringLiteral("windows7"),         QStringLiteral("fluent_regular"),
            QStringLiteral("fluent_filled"),    QStringLiteral("windows11"),
            QStringLiteral("windows10_flat"),   QStringLiteral("arch_blue"),
            QStringLiteral("notepad3_warm"),    QStringLiteral("notepadpp_green"),
            QStringLiteral("notepadPlusPlus"),  QStringLiteral("monoline_minimal"),
            QStringLiteral("glossy_orange"),    QStringLiteral("pixel_fugue_like"),
            QStringLiteral("material_sharp"),   QStringLiteral("windowsXp"),
            QStringLiteral("notepadNext"),      QStringLiteral("vibrantDuotone"),
            QStringLiteral("monochrome"),
        };

        for (const QString &id : expectedPackIds) {
            const ToolbarIconPack *pack = manager->findPackById(id);
            QVERIFY2(pack != nullptr, qPrintable(QString("Pack %1 should exist").arg(id)));
            QVERIFY(!pack->displayName.isEmpty());
            QVERIFY(!pack->basePath.isEmpty());
            QVERIFY(pack->isBuiltIn);
        }

        // Verify legacy aliases resolve smoothly
        QVERIFY(manager->findPackById(QStringLiteral("fluentUi")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("windows11")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("windows7")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("archLinux")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("notepadPlusPlus")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("filledFluentUi")) != nullptr);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testCanonicalActionNameMapping() const {
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionNew"), QString("newfile"));
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionOpen"), QString("openFile"));
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionSave"), QString("saved"));
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionSaveAll"), QString("saveAll"));
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionClose"), QString("closeFile"));
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionCloseAll"), QString("closeAll"));
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionPrint"), QString("printer"));
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionWordWrap"), QString("wrap"));
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionShowAllCharacters"), QString("invisibleChar"));
        QCOMPARE(ToolbarIconManager::canonicalNameForAction("actionPreferences"), QString("settings"));
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testIconSizeSwitching() const {
        auto *manager = ToolbarIconManager::instance();

        manager->setIconSize(QSize(16, 16));
        QCOMPARE(manager->currentIconSize(), QSize(16, 16));

        manager->setIconSize(QSize(20, 20));
        QCOMPARE(manager->currentIconSize(), QSize(20, 20));

        manager->setIconSize(QSize(24, 24));
        QCOMPARE(manager->currentIconSize(), QSize(24, 24));

        manager->setIconSize(QSize(32, 32));
        QCOMPARE(manager->currentIconSize(), QSize(32, 32));

        manager->setIconSize(QSize(48, 48));
        QCOMPARE(manager->currentIconSize(), QSize(48, 48));

        manager->setIconSize(QSize(64, 64));
        QCOMPARE(manager->currentIconSize(), QSize(64, 64));

        // Restore default
        manager->setIconSize(QSize(24, 24));
        QCOMPARE(manager->currentIconSize(), QSize(24, 24));
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testIconResolutionAcrossPacks() const {
        const auto *manager = ToolbarIconManager::instance();

        const QStringList testActions = {
            QStringLiteral("actionNew"),
            QStringLiteral("actionOpen"),
            QStringLiteral("actionSave"),
            QStringLiteral("actionCut"),
            QStringLiteral("actionCopy"),
            QStringLiteral("actionPaste"),
            QStringLiteral("actionUndo"),
            QStringLiteral("actionRedo"),
            QStringLiteral("actionRestart"),
            QStringLiteral("actionPreferences"),
            QStringLiteral("actionExit"),
            QStringLiteral("actionSelectAll"),
            QStringLiteral("actionIncreaseIndent"),
            QStringLiteral("actionDecreaseIndent"),
        };

        const QStringList packs = {
            QStringLiteral("modernRibbon"),     QStringLiteral("fugue"),
            QStringLiteral("aero_vista"),       QStringLiteral("aero_win7"),
            QStringLiteral("fluent_regular"),   QStringLiteral("fluent_filled"),
            QStringLiteral("windows10_flat"),   QStringLiteral("arch_blue"),
            QStringLiteral("notepad3_warm"),    QStringLiteral("notepadpp_green"),
            QStringLiteral("monoline_minimal"), QStringLiteral("glossy_orange"),
            QStringLiteral("pixel_fugue_like"), QStringLiteral("material_sharp"),
            QStringLiteral("windowsXp"),        QStringLiteral("notepadNext"),
            QStringLiteral("vibrantDuotone"),   QStringLiteral("monochrome"),
            QStringLiteral("fluentUi"),         QStringLiteral("windows11"),
            QStringLiteral("windows7"),         QStringLiteral("archLinux"),
        };

        for (const QString &packId : packs) {
            for (const QString &act : testActions) {
                const QIcon icon = manager->getActionIcon(act, packId);
                QVERIFY2(!icon.isNull(), qPrintable(QString("Icon for %1 in pack %2 should resolve").arg(act, packId)));
            }
        }
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testApplyToToolbar() const {
        auto *manager = ToolbarIconManager::instance();
        QToolBar toolBar;

        auto *actNew = toolBar.addAction(QStringLiteral("New"));
        actNew->setObjectName(QStringLiteral("actionNew"));

        auto *actSave = toolBar.addAction(QStringLiteral("Save"));
        actSave->setObjectName(QStringLiteral("actionSave"));

        manager->setPack(QStringLiteral("fluentUi"), &toolBar);
        QVERIFY(!actNew->icon().isNull());
        QVERIFY(!actSave->icon().isNull());

        manager->setPack(QStringLiteral("windowsXp"), &toolBar);
        QVERIFY(!actNew->icon().isNull());
        QVERIFY(!actSave->icon().isNull());

        manager->setPack(QStringLiteral("windows7"), &toolBar);
        QVERIFY(!actNew->icon().isNull());
        QVERIFY(!actSave->icon().isNull());

        manager->setPack(QStringLiteral("modernRibbon"), &toolBar);
        QVERIFY(!actNew->icon().isNull());
        QVERIFY(!actSave->icon().isNull());
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testApplyToWindow() const {
        auto *manager = ToolbarIconManager::instance();
        QWidget window;
        auto *actCut = new QAction(QStringLiteral("Cut"), &window);
        actCut->setObjectName(QStringLiteral("actionCut"));
        auto *actCopy = new QAction(QStringLiteral("Copy"), &window);
        actCopy->setObjectName(QStringLiteral("actionCopy"));

        manager->setPack(QStringLiteral("windowsXp"));
        manager->applyToWindow(&window);
        QVERIFY(!actCut->icon().isNull());
        QVERIFY(!actCopy->icon().isNull());
        QVERIFY(actCut->isIconVisibleInMenu());

        manager->setPack(QStringLiteral("windows7"));
        manager->applyToWindow(&window);
        QVERIFY(!actCut->icon().isNull());
        QVERIFY(!actCopy->icon().isNull());
    }
};
