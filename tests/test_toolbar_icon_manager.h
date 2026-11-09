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
#ifdef isinf
#undef isinf
#endif
#ifdef isnan
#undef isnan
#endif

#include <QObject>
#include <QTest>
#include <QToolBar>
#include <QAction>
#include <QString>
#include <QStringList>
#include "features/ToolbarIconManager.h"

class TestToolbarIconManager : public QObject {
    Q_OBJECT

private slots:
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testBuiltInPacksExist() const {
        const auto *manager = ToolbarIconManager::instance();
        const QList<ToolbarIconPack> packs = manager->availablePacks();

        // 39 built-in packs (25 base + 10 Aksara suites + 4 Sheet 4 packs)
        QCOMPARE(packs.size(), 39);

        const QStringList expectedPackIds = {
            QString::fromLatin1("modernRibbon"),
            QString::fromLatin1("fugue"),
            QString::fromLatin1("aero_win7"),
            QString::fromLatin1("windows7"),
            QString::fromLatin1("fluent_regular"),
            QString::fromLatin1("fluent_filled"),
            QString::fromLatin1("windows11"),
            QString::fromLatin1("notepadPlusPlus"),
            QString::fromLatin1("glossy_orange"),
            QString::fromLatin1("windowsXp"),
            QString::fromLatin1("notepadNext"),
            QString::fromLatin1("vibrantDuotone"),
            QString::fromLatin1("monochrome"),
            QString::fromLatin1("eleganticons"),
            QString::fromLatin1("seven"),
            QString::fromLatin1("mignon"),
            QString::fromLatin1("palette"),
            QString::fromLatin1("diagonal"),
            QString::fromLatin1("aksara-arch-linux"),
            QString::fromLatin1("aksara-elementary-os"),
            QString::fromLatin1("aksara-icon-pack-rounded"),
            QString::fromLatin1("aksara-macos-bigsur"),
            QString::fromLatin1("aksara-npp-vista"),
            QString::fromLatin1("aksara-npp-win10"),
            QString::fromLatin1("aksara-npp-win11"),
            QString::fromLatin1("aksara-npp-xp"),
            QString::fromLatin1("aksara-ubuntu-yaru"),
            QString::fromLatin1("aksara-vista-aero"),
            QString::fromLatin1("pastel"),
            QString::fromLatin1("notepad_pro"),
            QString::fromLatin1("royal_blue"),
            QString::fromLatin1("fluent_modern"),
            QString::fromLatin1("macos_bigsur_modern"),
            QString::fromLatin1("dark_neon_pro"),
            QString::fromLatin1("macos_glass"),
            QString::fromLatin1("modern_flat_colorful"),
            QString::fromLatin1("glossy_3d_skeuomorphic"),
            QString::fromLatin1("retro_8bit_pixel"),
            QString::fromLatin1("sketchy_doodle"),
        };

        for (const QString &id : expectedPackIds) {
            const ToolbarIconPack *pack = manager->findPackById(id);
            QVERIFY2(pack != nullptr,
                     qPrintable(QString::fromLatin1("Pack ") + id + QString::fromLatin1(" should exist")));
            QVERIFY(!pack->displayName.isEmpty());
            QVERIFY(!pack->basePath.isEmpty());
            QVERIFY(pack->isBuiltIn);
        }

        // Verify legacy and Aksara aliases resolve smoothly
        QVERIFY(manager->findPackById(QString::fromLatin1("fluentUi")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("windows11")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("windows7")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("seven_icons")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("notepadPlusPlus")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("filledFluentUi")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("aksara_npp_win11")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("notepadPro")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("royalBlue")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("fluentModern")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("darkNeonPro")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("macosGlass")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("modernFlatColorful")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("glossy3dSkeuomorphic")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("retro8bitPixel")) != nullptr);
        QVERIFY(manager->findPackById(QString::fromLatin1("sketchyDoodle")) != nullptr);
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
            QString::fromLatin1("modernRibbon"),
            QString::fromLatin1("fugue"),
            QString::fromLatin1("aero_win7"),
            QString::fromLatin1("windows7"),
            QString::fromLatin1("fluent_regular"),
            QString::fromLatin1("fluent_filled"),
            QString::fromLatin1("glossy_orange"),
            QString::fromLatin1("windowsXp"),
            QString::fromLatin1("notepadNext"),
            QString::fromLatin1("vibrantDuotone"),
            QString::fromLatin1("monochrome"),
            QString::fromLatin1("windows11"),
            QString::fromLatin1("fluentUi"),
            QString::fromLatin1("eleganticons"),
            QString::fromLatin1("seven"),
            QString::fromLatin1("mignon"),
            QString::fromLatin1("palette"),
            QString::fromLatin1("diagonal"),
            QString::fromLatin1("pastel"),
            QString::fromLatin1("notepad_pro"),
            QString::fromLatin1("royal_blue"),
            QString::fromLatin1("fluent_modern"),
            QString::fromLatin1("macos_bigsur_modern"),
            QString::fromLatin1("dark_neon_pro"),
            QString::fromLatin1("macos_glass"),
            QString::fromLatin1("modern_flat_colorful"),
            QString::fromLatin1("glossy_3d_skeuomorphic"),
            QString::fromLatin1("retro_8bit_pixel"),
            QString::fromLatin1("sketchy_doodle"),
        };

        for (const QString &packId : packs) {
            for (const QString &act : testActions) {
                const QIcon icon = manager->getActionIcon(act, packId);
                QVERIFY2(!icon.isNull(),
                         qPrintable(QString::fromLatin1("Icon for ") + act + QString::fromLatin1(" in pack ") + packId +
                                    QString::fromLatin1(" should resolve")));
            }
        }
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testApplyToToolbar() const {
        auto *manager = ToolbarIconManager::instance();
        QToolBar toolBar;

        auto *actNew = new QAction(QString::fromLatin1("New"), &toolBar);
        actNew->setObjectName(QString::fromLatin1("actionNew"));
        toolBar.addAction(actNew);

        auto *actSave = new QAction(QString::fromLatin1("Save"), &toolBar);
        actSave->setObjectName(QString::fromLatin1("actionSave"));
        toolBar.addAction(actSave);

        manager->setPack(QString::fromLatin1("fluentUi"), &toolBar);
        QVERIFY(!actNew->icon().isNull());
        QVERIFY(!actSave->icon().isNull());

        manager->setPack(QString::fromLatin1("windowsXp"), &toolBar);
        QVERIFY(!actNew->icon().isNull());
        QVERIFY(!actSave->icon().isNull());

        manager->setPack(QString::fromLatin1("windows7"), &toolBar);
        QVERIFY(!actNew->icon().isNull());
        QVERIFY(!actSave->icon().isNull());

        manager->setPack(QString::fromLatin1("modernRibbon"), &toolBar);
        QVERIFY(!actNew->icon().isNull());
        QVERIFY(!actSave->icon().isNull());
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void testApplyToWindow() const {
        auto *manager = ToolbarIconManager::instance();
        QWidget window;
        auto *actCut = new QAction(QString::fromLatin1("Cut"), &window);
        actCut->setObjectName(QString::fromLatin1("actionCut"));
        auto *actCopy = new QAction(QString::fromLatin1("Copy"), &window);
        actCopy->setObjectName(QString::fromLatin1("actionCopy"));

        manager->setPack(QString::fromLatin1("windowsXp"));
        manager->applyToWindow(&window);
        QVERIFY(!actCut->icon().isNull());
        QVERIFY(!actCopy->icon().isNull());
        QVERIFY(actCut->isIconVisibleInMenu());

        manager->setPack(QString::fromLatin1("windows7"));
        manager->applyToWindow(&window);
        QVERIFY(!actCut->icon().isNull());
        QVERIFY(!actCopy->icon().isNull());
    }
};
