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
    void testBuiltInPacksExist() const {
        const auto *manager = ToolbarIconManager::instance();
        const QList<ToolbarIconPack> packs = manager->availablePacks();

        // 15 built-in packs required
        QVERIFY(packs.size() >= 15);

        const QStringList expectedPackIds = {
            QStringLiteral("modernRibbon"),    QStringLiteral("notepadNext"),  QStringLiteral("notepadPlusPlus"),
            QStringLiteral("androidMaterial"), QStringLiteral("windowsVista"), QStringLiteral("windowsXp"),
            QStringLiteral("windows10"),       QStringLiteral("windows11"),    QStringLiteral("windows7"),
            QStringLiteral("macosMonterey"),   QStringLiteral("notepad3"),     QStringLiteral("notepad4"),
            QStringLiteral("notepad2"),        QStringLiteral("monochrome"),   QStringLiteral("vibrantDuotone"),
        };

        for (const QString &id : expectedPackIds) {
            const ToolbarIconPack *pack = manager->findPackById(id);
            QVERIFY2(pack != nullptr, qPrintable(QString("Pack %1 should exist").arg(id)));
            QVERIFY(!pack->displayName.isEmpty());
            QVERIFY(!pack->basePath.isEmpty());
            QVERIFY(pack->isBuiltIn);
        }

        // Test backward compatibility alias
        const ToolbarIconPack *aliasedPack = manager->findPackById(QStringLiteral("fluentUi"));
        QVERIFY(aliasedPack != nullptr);
        QCOMPARE(aliasedPack->id, QStringLiteral("windows11"));
    }

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

    void testIconSizeSwitching() const {
        auto *manager = ToolbarIconManager::instance();

        manager->setIconSize(QSize(16, 16));
        QCOMPARE(manager->currentIconSize(), QSize(16, 16));

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
            QStringLiteral("modernRibbon"),    QStringLiteral("notepadNext"),  QStringLiteral("notepadPlusPlus"),
            QStringLiteral("androidMaterial"), QStringLiteral("windowsVista"), QStringLiteral("windowsXp"),
            QStringLiteral("windows10"),       QStringLiteral("windows11"),    QStringLiteral("windows7"),
            QStringLiteral("macosMonterey"),   QStringLiteral("notepad3"),     QStringLiteral("notepad4"),
            QStringLiteral("notepad2"),        QStringLiteral("monochrome"),   QStringLiteral("vibrantDuotone"),
        };

        for (const QString &packId : packs) {
            for (const QString &act : testActions) {
                const QIcon icon = manager->getActionIcon(act, packId);
                QVERIFY2(!icon.isNull(), qPrintable(QString("Icon for %1 in pack %2 should resolve").arg(act, packId)));
            }
        }
    }

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
