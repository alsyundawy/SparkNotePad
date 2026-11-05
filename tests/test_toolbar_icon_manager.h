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

#include <cmath>
#include <algorithm>
#include <string>

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

        // 40 built-in packs (including seven, whitesur, colloid, win11os_kde, etc.)
        QCOMPARE(packs.size(), 40);

        const QStringList expectedPackIds = {
            QString::fromLatin1("modernRibbon"),     QString::fromLatin1("fugue"),
            QString::fromLatin1("aero_vista"),       QString::fromLatin1("aero_win7"),
            QString::fromLatin1("windows7"),         QString::fromLatin1("fluent_regular"),
            QString::fromLatin1("fluent_filled"),    QString::fromLatin1("windows11"),
            QString::fromLatin1("windows10_flat"),   QString::fromLatin1("arch_blue"),
            QString::fromLatin1("notepad3_warm"),    QString::fromLatin1("notepadpp_green"),
            QString::fromLatin1("notepadPlusPlus"),  QString::fromLatin1("monoline_minimal"),
            QString::fromLatin1("glossy_orange"),    QString::fromLatin1("pixel_fugue_like"),
            QString::fromLatin1("material_sharp"),   QString::fromLatin1("windowsXp"),
            QString::fromLatin1("notepadNext"),      QString::fromLatin1("vibrantDuotone"),
            QString::fromLatin1("monochrome"),       QString::fromLatin1("eleganticons"),
            QString::fromLatin1("seven"),            QString::fromLatin1("mignon"),
            QString::fromLatin1("palette"),          QString::fromLatin1("diagonal"),
            QString::fromLatin1("argon"),            QString::fromLatin1("reversal"),
            QString::fromLatin1("bigsur"),           QString::fromLatin1("mkos_bigsur"),
            QString::fromLatin1("we10x"),            QString::fromLatin1("mactahoe"),
            QString::fromLatin1("uos"),              QString::fromLatin1("deepin"),
            QString::fromLatin1("whitesur"),         QString::fromLatin1("elementary_kde"),
            QString::fromLatin1("mcmojave_circle"),  QString::fromLatin1("colloid"),
            QString::fromLatin1("fluent_kde"),       QString::fromLatin1("win11os_kde"),
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
        QVERIFY(manager->findPackById(QStringLiteral("seven_icons")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("archLinux")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("notepadPlusPlus")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("filledFluentUi")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("whiteSur")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("win11os")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("fluentKde")) != nullptr);
        QVERIFY(manager->findPackById(QStringLiteral("mcmojaveCircle")) != nullptr);
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
            QString::fromLatin1("actionNew"),
            QString::fromLatin1("actionOpen"),
            QString::fromLatin1("actionSave"),
            QString::fromLatin1("actionCut"),
            QString::fromLatin1("actionCopy"),
            QString::fromLatin1("actionPaste"),
            QString::fromLatin1("actionUndo"),
            QString::fromLatin1("actionRedo"),
            QString::fromLatin1("actionRestart"),
            QString::fromLatin1("actionPreferences"),
            QString::fromLatin1("actionExit"),
            QString::fromLatin1("actionSelectAll"),
            QString::fromLatin1("actionIncreaseIndent"),
            QString::fromLatin1("actionDecreaseIndent"),
        };

        const QStringList packs = {
            QString::fromLatin1("modernRibbon"),     QString::fromLatin1("fugue"),
            QString::fromLatin1("aero_vista"),       QString::fromLatin1("aero_win7"),
            QString::fromLatin1("fluent_regular"),   QString::fromLatin1("fluent_filled"),
            QString::fromLatin1("windows10_flat"),   QString::fromLatin1("arch_blue"),
            QString::fromLatin1("notepad3_warm"),    QString::fromLatin1("notepadpp_green"),
            QString::fromLatin1("monoline_minimal"), QString::fromLatin1("glossy_orange"),
            QString::fromLatin1("pixel_fugue_like"), QString::fromLatin1("material_sharp"),
            QString::fromLatin1("windowsXp"),        QString::fromLatin1("notepadNext"),
            QString::fromLatin1("vibrantDuotone"),   QString::fromLatin1("monochrome"),
            QString::fromLatin1("fluentUi"),         QString::fromLatin1("windows11"),
            QString::fromLatin1("windows7"),         QString::fromLatin1("archLinux"),
            QString::fromLatin1("eleganticons"),     QString::fromLatin1("seven"),
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
