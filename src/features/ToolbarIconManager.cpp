/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ToolbarIconManager.h"
#include "../ApplicationSettings.h"

#include <QAction>
#include <QColor>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QHash>
#include <QImage>
#include <QStandardPaths>
#include <QToolBar>
#include <QMenu>
#include <QWidget>
#include <algorithm>
#include <array>
#include <memory>
#include <ranges>

ToolbarIconManager *ToolbarIconManager::instance() {
    static ToolbarIconManager s_instance;
    return &s_instance;
}

ToolbarIconManager::ToolbarIconManager(QObject *parent) : QObject(parent) {
    registerBuiltInPacks();
}

void ToolbarIconManager::registerBuiltInPacks() {
    m_packs.clear();

    // 1. Modern Ribbon (Default Vector SVG)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("modernRibbon"),
        .displayName = tr("Modern Ribbon (Default Vector)"),
        .description = tr("High-DPI 20x20 monoline vector SVG suite with refined contours"),
        .basePath = QStringLiteral(":/icons"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 2. Fugue Classic (Silk Mini-Icons)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("fugue"),
        .displayName = tr("Fugue Classic (Silk Mini-Icons)"),
        .description = tr("Detailed mini-icons with soft shadows by Yusuke Kamiyamane"),
        .basePath = QStringLiteral(":/icons/packs/fugue"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 3. Windows Vista Aero (Glossy)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aero_vista"),
        .displayName = tr("Windows Vista Aero (Glossy)"),
        .description = tr("Glossy Aero Vista Iconfactory prototypes style with highlights and glass depth"),
        .basePath = QStringLiteral(":/icons/packs/aero_vista"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 4. Windows 7 Aero (Refined Glossy)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aero_win7"),
        .displayName = tr("Windows 7 Aero (Refined Glossy)"),
        .description = tr("Refined Windows 7 Aero translucent glass iconography"),
        .basePath = QStringLiteral(":/icons/packs/aero_win7"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 5. Windows 7 (Authentic)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windows7"),
        .displayName = tr("Windows 7"),
        .description = tr("Authentic Windows 7 toolbar iconography"),
        .basePath = QStringLiteral(":/icons/packs/windows7"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 6. Microsoft Fluent UI (Regular Outline)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("fluent_regular"),
        .displayName = tr("Microsoft Fluent UI (Regular Outline)"),
        .description = tr("Microsoft Fluent Design outlined iconography for Windows 11"),
        .basePath = QStringLiteral(":/icons/packs/fluent_regular"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 7. Microsoft Fluent UI (Filled Solid)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("fluent_filled"),
        .displayName = tr("Microsoft Fluent UI (Filled Solid)"),
        .description = tr("Microsoft Fluent Design filled solid iconography"),
        .basePath = QStringLiteral(":/icons/packs/fluent_filled"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 8. Windows 11 (Authentic Fluent)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windows11"),
        .displayName = tr("Windows 11"),
        .description = tr("Authentic Windows 11 Fluent toolbar iconography"),
        .basePath = QStringLiteral(":/icons/packs/windows11"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 9. Windows 10 Modern (Flat)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windows10_flat"),
        .displayName = tr("Windows 10 Modern (Flat)"),
        .description = tr("Crisp geometric flat modern iconography from Windows 10"),
        .basePath = QStringLiteral(":/icons/packs/windows10_flat"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 10. Arch Linux (High-Contrast Blue)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("arch_blue"),
        .displayName = tr("Arch Linux (High-Contrast Blue)"),
        .description = tr("High-contrast Arch Linux cobalt and cyan iconography"),
        .basePath = QStringLiteral(":/icons/packs/arch_blue"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 11. Notepad3 Warm (Classic Amber)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepad3_warm"),
        .displayName = tr("Notepad3 Warm (Classic Amber)"),
        .description = tr("Notepad3 warm brown and amber classic text editor iconography"),
        .basePath = QStringLiteral(":/icons/packs/notepad3_warm"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 12. Notepad++ (Green Accents)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepadpp_green"),
        .displayName = tr("Notepad++ (Green Accents)"),
        .description = tr("Classic Notepad++ toolbar iconography with green accents"),
        .basePath = QStringLiteral(":/icons/packs/notepadpp_green"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 13. Notepad++ (Classic)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepadPlusPlus"),
        .displayName = tr("Notepad++ (Classic)"),
        .description = tr("Authentic Notepad++ dual-resolution SVG and PNG suite"),
        .basePath = QStringLiteral(":/icons/packs/notepadPlusPlus"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 14. Monoline Minimal (Ultra-Thin)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("monoline_minimal"),
        .displayName = tr("Monoline Minimal (Ultra-Thin)"),
        .description = tr("Ultra-thin 1.5px monoline wireframe vector styling"),
        .basePath = QStringLiteral(":/icons/packs/monoline_minimal"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 12. Glossy Orange (WinRAR Amber)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("glossy_orange"),
        .displayName = tr("Glossy Orange (WinRAR Amber)"),
        .description = tr("Warm glossy orange and amber skeuomorphic iconography"),
        .basePath = QStringLiteral(":/icons/packs/glossy_orange"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 13. Pixel Dense (Retro 16-Bit)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("pixel_fugue_like"),
        .displayName = tr("Pixel Dense (Retro 16-Bit)"),
        .description = tr("Pixel-precise dense retro iconography with crisp edges"),
        .basePath = QStringLiteral(":/icons/packs/pixel_fugue_like"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 14. Material Design (Sharp)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("material_sharp"),
        .displayName = tr("Material Design (Sharp)"),
        .description = tr("Google Material Design sharp geometric iconography"),
        .basePath = QStringLiteral(":/icons/packs/material_sharp"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 15. Windows XP (Luna Classic)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windowsXp"),
        .displayName = tr("Windows XP (Luna Classic)"),
        .description = tr("Windows XP Luna skeuomorphic authentic iconography"),
        .basePath = QStringLiteral(":/icons/packs/windowsXp"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 16. NotepadNext Classic (Silk)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepadNext"),
        .displayName = tr("NotepadNext Classic (Silk)"),
        .description = tr("Authentic 16x16 Silk bitmap icons from NotepadNext"),
        .basePath = QStringLiteral(":/icons/packs/notepadNext"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 17. Vibrant DuoTone (Cyberpunk)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("vibrantDuotone"),
        .displayName = tr("Vibrant DuoTone (Cyberpunk)"),
        .description = tr("High-luminance electric cyan & radiant magenta dual-tone contrast"),
        .basePath = QStringLiteral(":/icons/packs/vibrantDuotone"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 18. Monochrome Outline (Minimalist)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("monochrome"),
        .displayName = tr("Monochrome Outline (Minimalist)"),
        .description = tr("Clean 1.8px monoline wireframes engineered for dark and light themes"),
        .basePath = QStringLiteral(":/icons/packs/monochrome"),
        .isBuiltIn = true,
        .isPngBased = false,
    });
}

void ToolbarIconManager::initialize(ApplicationSettings *settings) {
    m_settings = settings;

    scanCustomPacks();

    if (m_settings) {
        const QString savedPack =
            m_settings->value(QStringLiteral("Gui/ToolbarIconSet"), QStringLiteral("modernRibbon")).toString();
        if (const ToolbarIconPack *pack = findPackById(savedPack)) {
            m_currentPackId = pack->id;
        } else {
            m_currentPackId = QStringLiteral("modernRibbon");
        }

        const int savedSize = m_settings->value(QStringLiteral("Gui/ToolbarIconSize"), 24).toInt();
        const int clampedSize = (savedSize >= 16 && savedSize <= 64) ? savedSize : 24;
        m_currentIconSize = QSize(clampedSize, clampedSize);
    }
}

const QList<ToolbarIconPack> &ToolbarIconManager::availablePacks() const {
    return m_packs;
}

const ToolbarIconPack *ToolbarIconManager::findPackById(const QString &packId) const {
    QString targetId = packId;
    if (targetId == QLatin1String("fluentUi")) {
        targetId = QStringLiteral("fluent_regular");
    } else if (targetId == QLatin1String("filledFluentUi")) {
        targetId = QStringLiteral("fluent_filled");
    } else if (targetId == QLatin1String("archLinux")) {
        targetId = QStringLiteral("arch_blue");
    }

    if (const auto it =
            std::ranges::find_if(m_packs, [&targetId](const ToolbarIconPack &p) { return p.id == targetId; });
        it != m_packs.end()) {
        return std::to_address(it);
    }
    return nullptr;
}

void ToolbarIconManager::setPack(const QString &packId, QToolBar *toolBar) {
    const ToolbarIconPack *pack = findPackById(packId);
    if (!pack) {
        qWarning() << "ToolbarIconManager: Cannot locate icon pack with ID" << packId;
        return;
    }

    const bool isDifferentPack = (m_currentPackId != pack->id);
    m_currentPackId = pack->id;

    if (m_settings && isDifferentPack) {
        m_settings->setValue(QStringLiteral("Gui/ToolbarIconSet"), m_currentPackId);
    }

    if (toolBar) {
        applyToWindow(toolBar->window(), toolBar);
    }

    if (isDifferentPack) {
        emit packChanged(m_currentPackId);
    }
}

void ToolbarIconManager::setIconSize(const QSize &size, QToolBar *toolBar) {
    m_currentIconSize = size;

    if (m_settings) {
        m_settings->setValue(QStringLiteral("Gui/ToolbarIconSize"), size.width());
    }

    if (toolBar) {
        toolBar->setIconSize(size);
        applyToToolbar(toolBar);
        toolBar->updateGeometry();
        toolBar->update();
    }

    emit iconSizeChanged(size);
}

QString ToolbarIconManager::customPacksPath() {
    const QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(appData).filePath(QStringLiteral("toolbarIcons"));
}

void ToolbarIconManager::scanCustomPacks() {
    clearCache();
    // Preserve built-in packs and re-scan external packs
    registerBuiltInPacks();

    const QString customDir = customPacksPath();
    const QDir dir(customDir);
    if (!dir.exists()) {
        return;
    }

    const QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    for (const QFileInfo &entry : entries) {
        const QString packId = entry.fileName();
        if (findPackById(packId)) {
            continue;
        }

        m_packs.append(ToolbarIconPack{
            .id = packId,
            .displayName = packId,
            .description = tr("Custom external icon pack from %1").arg(entry.filePath()),
            .basePath = entry.filePath(),
            .isBuiltIn = false,
            .isPngBased = false,
        });
    }
}

QString ToolbarIconManager::canonicalNameForAction(const QString &actionName) {
    QString name = actionName;
    if (name.startsWith(QStringLiteral("action"))) {
        name.remove(0, 6);
    }

    static const QHash<QString, QString> s_actionMap = {
        {QStringLiteral("new"), QStringLiteral("newfile")},
        {QStringLiteral("open"), QStringLiteral("openFile")},
        {QStringLiteral("openurl"), QStringLiteral("openFile")},
        {QStringLiteral("save"), QStringLiteral("saved")},
        {QStringLiteral("saveall"), QStringLiteral("saveAll")},
        {QStringLiteral("close"), QStringLiteral("closeFile")},
        {QStringLiteral("closeall"), QStringLiteral("closeAll")},
        {QStringLiteral("delete"), QStringLiteral("closeFile")},
        {QStringLiteral("print"), QStringLiteral("printer")},
        {QStringLiteral("cut"), QStringLiteral("cut")},
        {QStringLiteral("copy"), QStringLiteral("copy")},
        {QStringLiteral("copyurl"), QStringLiteral("copy")},
        {QStringLiteral("paste"), QStringLiteral("paste")},
        {QStringLiteral("undo"), QStringLiteral("undo")},
        {QStringLiteral("redo"), QStringLiteral("redo")},
        {QStringLiteral("find"), QStringLiteral("find")},
        {QStringLiteral("replace"), QStringLiteral("findReplace")},
        {QStringLiteral("zoomin"), QStringLiteral("zoomIn")},
        {QStringLiteral("zoomout"), QStringLiteral("zoomOut")},
        {QStringLiteral("wordwrap"), QStringLiteral("wrap")},
        {QStringLiteral("showallcharacters"), QStringLiteral("invisibleChar")},
        {QStringLiteral("showcharacterinfo"), QStringLiteral("invisibleChar")},
        {QStringLiteral("showindentguide"), QStringLiteral("indentGuide")},
        {QStringLiteral("macrorecording"), QStringLiteral("startRecord")},
        {QStringLiteral("playback"), QStringLiteral("playRecord")},
        {QStringLiteral("runmacromultipletimes"), QStringLiteral("playRecord_m")},
        {QStringLiteral("savecurrentrecordedmacro"), QStringLiteral("saveRecord")},
        {QStringLiteral("restart"), QStringLiteral("restart")},
        {QStringLiteral("preferences"), QStringLiteral("settings")},
        {QStringLiteral("exit"), QStringLiteral("exit")},
        {QStringLiteral("readonly"), QStringLiteral("readonly")},
        {QStringLiteral("selectall"), QStringLiteral("select_all")},
        {QStringLiteral("indent"), QStringLiteral("text_indent")},
        {QStringLiteral("increaseindent"), QStringLiteral("text_indent")},
        {QStringLiteral("removeindent"), QStringLiteral("text_indent_remove")},
        {QStringLiteral("decreaseindent"), QStringLiteral("text_indent_remove")},
        {QStringLiteral("closeallexceptactive"), QStringLiteral("closeAll")},
        {QStringLiteral("closealltoleft"), QStringLiteral("closeAll")},
        {QStringLiteral("closealltoright"), QStringLiteral("closeAll")},
        {QStringLiteral("saveas"), QStringLiteral("saved")},
        {QStringLiteral("rename"), QStringLiteral("table_edit")},
        {QStringLiteral("reload"), QStringLiteral("arrow_refresh")},
        {QStringLiteral("showinexplorer"), QStringLiteral("folder")},
        {QStringLiteral("openterminalhere"), QStringLiteral("terminal")},
        {QStringLiteral("copyfullpath"), QStringLiteral("copy")},
        {QStringLiteral("copyfilename"), QStringLiteral("copy")},
        {QStringLiteral("copyfiledirectory"), QStringLiteral("copy")},
        {QStringLiteral("previewmarkdown"), QStringLiteral("find")},
        {QStringLiteral("previewhtml"), QStringLiteral("find")},
        {QStringLiteral("previewhtmlinbrowser"), QStringLiteral("openFile")},
        {QStringLiteral("togglehexbinaryview"), QStringLiteral("invisibleChar")},
    };

    if (const auto it = s_actionMap.find(name.toLower()); it != s_actionMap.end()) {
        return it.value();
    }

    // Default to camelCase or direct name
    if (!name.isEmpty()) {
        name[0] = name[0].toLower();
    }
    return name;
}

QStringList ToolbarIconManager::aliasesForCanonical(const QString &canonical) {
    QStringList aliases;
    if (canonical == QStringLiteral("newfile")) {
        aliases << QStringLiteral("new");
    } else if (canonical == QStringLiteral("openFile")) {
        aliases << QStringLiteral("open");
    } else if (canonical == QStringLiteral("saved")) {
        aliases << QStringLiteral("save");
    } else if (canonical == QStringLiteral("closeFile")) {
        aliases << QStringLiteral("close") << QStringLiteral("delete");
    } else if (canonical == QStringLiteral("printer")) {
        aliases << QStringLiteral("print");
    } else if (canonical == QStringLiteral("findReplace")) {
        aliases << QStringLiteral("replace");
    } else if (canonical == QStringLiteral("invisibleChar")) {
        aliases << QStringLiteral("allChars");
    } else if (canonical == QStringLiteral("startRecord")) {
        aliases << QStringLiteral("record");
    } else if (canonical == QStringLiteral("playRecord")) {
        aliases << QStringLiteral("playback");
    } else if (canonical == QStringLiteral("playRecord_m")) {
        aliases << QStringLiteral("runMacro");
    } else if (canonical == QStringLiteral("saveRecord")) {
        aliases << QStringLiteral("saveMacro");
    } else if (canonical == QStringLiteral("settings")) {
        aliases << QStringLiteral("preferences") << QStringLiteral("cog");
    } else if (canonical == QStringLiteral("restart")) {
        aliases << QStringLiteral("arrow_refresh");
    } else if (canonical == QStringLiteral("exit")) {
        aliases << QStringLiteral("door_out") << QStringLiteral("door_in") << QStringLiteral("system_log_out");
    } else if (canonical == QStringLiteral("select_all")) {
        aliases << QStringLiteral("selection") << QStringLiteral("selectall");
    } else if (canonical == QStringLiteral("text_indent")) {
        aliases << QStringLiteral("indent") << QStringLiteral("format_indent_more");
    } else if (canonical == QStringLiteral("text_indent_remove")) {
        aliases << QStringLiteral("unindent") << QStringLiteral("outdent") << QStringLiteral("format_indent_less");
    } else if (canonical == QStringLiteral("wrap")) {
        aliases << QStringLiteral("wordWrap");
    } else if (canonical == QStringLiteral("arrow_refresh")) {
        aliases << QStringLiteral("restart") << QStringLiteral("reload");
    } else if (canonical == QStringLiteral("folder")) {
        aliases << QStringLiteral("folder_go") << QStringLiteral("openFile");
    } else if (canonical == QStringLiteral("terminal")) {
        aliases << QStringLiteral("application_osx_terminal");
    } else if (canonical == QStringLiteral("table_edit")) {
        aliases << QStringLiteral("newfile");
    }

    return aliases;
}

QString ToolbarIconManager::resolveIconFilePath(const ToolbarIconPack &pack, const QString &canonical) {
    const QStringList aliases = aliasesForCanonical(canonical);
    QStringList candidates;

    if (pack.isPngBased) {
        candidates << (canonical + QStringLiteral(".png"));
        for (const QString &alias : aliases) {
            candidates << (alias + QStringLiteral(".png"));
        }
    } else {
        candidates << (canonical + QStringLiteral(".svg"));
        candidates << (canonical + QStringLiteral(".png"));
        candidates << (canonical + QStringLiteral(".ico"));
        for (const QString &alias : aliases) {
            candidates << (alias + QStringLiteral(".svg"));
            candidates << (alias + QStringLiteral(".png"));
            candidates << (alias + QStringLiteral(".ico"));
        }
    }

    for (const QString &file : candidates) {
        const QString fullPath = pack.basePath + QStringLiteral("/") + file;
        if (QFileInfo::exists(fullPath)) {
            return fullPath;
        }
    }

    // Graceful fallback to default Modern Ribbon SVG
    if (const QString defaultSvg = QStringLiteral(":/icons/") + canonical + QStringLiteral(".svg");
        QFileInfo::exists(defaultSvg)) {
        return defaultSvg;
    }

    // Fallback to PNG
    if (const QString defaultPng = QStringLiteral(":/icons/") + canonical + QStringLiteral(".png");
        QFileInfo::exists(defaultPng)) {
        return defaultPng;
    }

    return QString();
}

namespace {
QRgb transformDarkPixel(QRgb px) {
    const int a = qAlpha(px);
    if (a < 15) {
        return px;
    }
    const QColor c(px);
    const int lit = c.lightness();
    const int sat = c.hslSaturation();
    const int hue = c.hslHue();

    if (sat < 30) {
        if (lit < 160) {
            const int newLit = std::clamp(240 - lit, 180, 255);
            return QColor::fromHsl(hue, 0, newLit, a).rgba();
        }
        return px;
    }

    if (lit < 155) {
        const int newLit = std::clamp(155 + (lit * 75 / 155), 155, 235);
        return QColor::fromHsl(hue, sat, newLit, a).rgba();
    }
    return px;
}

QImage adaptImageForTheme(const QImage &src, bool isDark) {
    if (src.isNull() || !isDark) {
        return src;
    }

    QImage direct = src.convertToFormat(QImage::Format_ARGB32);
    const int w = direct.width();
    const int h = direct.height();

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const QRgb px = direct.pixel(x, y);
            const QRgb transformed = transformDarkPixel(px);
            if (transformed != px) {
                direct.setPixel(x, y, transformed);
            }
        }
    }
    return direct;
}

void addHiDpiPixmaps(QIcon &icon, const QString &filePath, bool isPngBased, bool isDark) {
    if (filePath.endsWith(QStringLiteral(".svg"), Qt::CaseInsensitive)) {
        return;
    }

    const QImage baseImg(filePath);
    if (baseImg.isNull()) {
        return;
    }

    const QImage adapted = adaptImageForTheme(baseImg, isDark);
    static constexpr std::array<int, 6> s_dpiSizes = {16, 20, 24, 32, 48, 64};
    for (const int sz : s_dpiSizes) {
        if (const QPixmap px = QPixmap::fromImage(
                adapted.scaled(sz, sz, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            !px.isNull()) {
            icon.addPixmap(px);
            if (sz == 32 || sz == 48 || sz == 64) {
                QPixmap pxRetina = px;
                pxRetina.setDevicePixelRatio(2.0);
                icon.addPixmap(pxRetina);
            }
        }
    }

    if (!isPngBased) {
        return;
    }

    const qsizetype dotIdx = filePath.lastIndexOf(QLatin1Char('.'));
    if (dotIdx == -1) {
        return;
    }

    const QString at2x = filePath.left(dotIdx) + QStringLiteral("@2x.png");
    if (QFileInfo::exists(at2x)) {
        if (const QImage at2xImg(at2x); !at2xImg.isNull()) {
            QPixmap px2 = QPixmap::fromImage(adaptImageForTheme(at2xImg, isDark));
            px2.setDevicePixelRatio(2.0);
            icon.addPixmap(px2);
        }
    }
}

void addDpiPixmapsToIcon(QIcon &icon, const QPixmap &px, int sz, QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::Off) {
    icon.addPixmap(px, mode, state);
    if (sz == 32 || sz == 48 || sz == 64) {
        QPixmap px2 = px;
        px2.setDevicePixelRatio(2.0);
        icon.addPixmap(px2, mode, state);
    }
}

QIcon buildDualRecordMultiResIcon(const QString &basePath, bool isDark) {
    static constexpr std::array<int, 6> s_dpiSizes = {16, 20, 24, 32, 48, 64};
    QIcon dualIcon;
    for (const int sz : s_dpiSizes) {
        const QString szDir = QStringLiteral("%1/%2x%2").arg(basePath).arg(sz);
        if (const QImage offImg(szDir + QStringLiteral("/startRecord.png")); !offImg.isNull()) {
            addDpiPixmapsToIcon(dualIcon, QPixmap::fromImage(adaptImageForTheme(offImg, isDark)), sz, QIcon::Normal, QIcon::Off);
        }
        if (const QImage onImg(szDir + QStringLiteral("/stopRecord.png")); !onImg.isNull()) {
            addDpiPixmapsToIcon(dualIcon, QPixmap::fromImage(adaptImageForTheme(onImg, isDark)), sz, QIcon::Normal, QIcon::On);
        }
    }
    return dualIcon;
}

QIcon buildMultiResIcon(const QString &basePath, const QStringList &candidates, bool isDark) {
    static constexpr std::array<int, 6> s_dpiSizes = {16, 20, 24, 32, 48, 64};
    QIcon icon;
    for (const int sz : s_dpiSizes) {
        const QString szDir = QStringLiteral("%1/%2x%2").arg(basePath).arg(sz);
        for (const QString &c : candidates) {
            const QString filePath = szDir + QLatin1Char('/') + c + QStringLiteral(".png");
            if (const QImage img(filePath); !img.isNull()) {
                addDpiPixmapsToIcon(icon, QPixmap::fromImage(adaptImageForTheme(img, isDark)), sz);
                break;
            }
        }
    }
    return icon;
}

QIcon createMenuResolutionIcon(const QIcon &icon) {
    // Preserve full icon resolution and vector scalability for context menus and menubar
    return icon;
}

void syncActionIcon(QAction *action, const ToolbarIconManager &mgr, bool isMenuContext) {
    if (!action || action->isSeparator()) {
        return;
    }

    if (!action->property("sparkDefaultIcon").isValid()) {
        action->setProperty("sparkDefaultIcon", action->icon());
    }

    const QString objectName = action->objectName();
    if (objectName.isEmpty()) {
        action->setIconVisibleInMenu(true);
        return;
    }

    if (const QIcon icon = mgr.getActionIcon(objectName); !icon.isNull()) {
        action->setIcon(isMenuContext ? createMenuResolutionIcon(icon) : icon);
    } else if (const QIcon defaultIcon = action->property("sparkDefaultIcon").value<QIcon>(); !defaultIcon.isNull()) {
        action->setIcon(defaultIcon);
    }
    action->setIconVisibleInMenu(true);
}
}  // namespace

QIcon ToolbarIconManager::getActionIcon(const QString &actionName, const QString &packId) const {
    const QString targetPackId = packId.isEmpty() ? m_currentPackId : packId;
    const QString canonical = canonicalNameForAction(actionName);
    const QString cacheKey = QStringLiteral("%1:%2:%3").arg(targetPackId, canonical, m_isDarkMode ? QStringLiteral("1") : QStringLiteral("0"));

    if (const auto it = m_iconCache.constFind(cacheKey); it != m_iconCache.constEnd()) {
        return it.value();
    }

    const ToolbarIconPack *pack = findPackById(targetPackId);
    if (!pack) {
        pack = findPackById(QStringLiteral("modernRibbon"));
    }

    if (!pack) {
        m_iconCache.insert(cacheKey, QIcon());
        return QIcon();
    }

    const QStringList aliases = aliasesForCanonical(canonical);
    QStringList candidates;
    candidates << canonical;
    candidates << aliases;

    // Check if the pack contains multi-resolution folders (e.g. 16x16, 20x20, 24x24, etc.)
    if (const bool hasMultiResolution = std::ranges::any_of(candidates, [&](const QString &c) {
            return QFileInfo::exists(pack->basePath + QStringLiteral("/24x24/") + c + QStringLiteral(".png"));
        }); hasMultiResolution) {
        const bool isDark = m_isDarkMode;
        if (canonical == QStringLiteral("startRecord")) {
            if (const QIcon dualIcon = buildDualRecordMultiResIcon(pack->basePath, isDark); !dualIcon.isNull()) {
                m_iconCache.insert(cacheKey, dualIcon);
                return dualIcon;
            }
        }

        if (const QIcon icon = buildMultiResIcon(pack->basePath, candidates, isDark); !icon.isNull()) {
            m_iconCache.insert(cacheKey, icon);
            return icon;
        }
    }

    if (const QString filePath = resolveIconFilePath(*pack, canonical); !filePath.isEmpty()) {
        const bool isDark = m_isDarkMode;
        if (canonical == QStringLiteral("startRecord")) {
            const QString onPath = resolveIconFilePath(*pack, QStringLiteral("stopRecord"));
            if (!onPath.isEmpty()) {
                QIcon dualIcon;
                dualIcon.addFile(filePath, QSize(), QIcon::Normal, QIcon::Off);
                dualIcon.addFile(onPath, QSize(), QIcon::Normal, QIcon::On);
                addHiDpiPixmaps(dualIcon, filePath, pack->isPngBased, isDark);
                m_iconCache.insert(cacheKey, dualIcon);
                return dualIcon;
            }
        }

        QIcon icon;
        icon.addFile(filePath);
        addHiDpiPixmaps(icon, filePath, pack->isPngBased, isDark);
        m_iconCache.insert(cacheKey, icon);
        return icon;
    }

    m_iconCache.insert(cacheKey, QIcon());
    return QIcon();
}

void ToolbarIconManager::applyToToolbar(QToolBar *toolBar) const {
    if (!toolBar) {
        return;
    }

    toolBar->setIconSize(m_currentIconSize);

    for (QAction *action : toolBar->actions()) {
        if (!action || action->isSeparator()) {
            continue;
        }

        // Cache original default icon so it is never lost under any circumstance
        if (!action->property("sparkDefaultIcon").isValid()) {
            action->setProperty("sparkDefaultIcon", action->icon());
        }

        const QString objectName = action->objectName();
        if (objectName.isEmpty()) {
            continue;
        }

        if (const QIcon icon = getActionIcon(objectName); !icon.isNull()) {
            action->setIcon(icon);
        } else {
            // Restore default icon if pack does not supply one
            const QIcon defaultIcon = action->property("sparkDefaultIcon").value<QIcon>();
            if (!defaultIcon.isNull()) {
                action->setIcon(defaultIcon);
            }
        }
    }
}

void ToolbarIconManager::applyToMenu(const QMenu *menu) const {
    if (!menu) {
        return;
    }

    QList<const QMenu *> menuQueue;
    menuQueue.append(menu);

    while (!menuQueue.isEmpty()) {
        const QMenu *currentMenu = menuQueue.takeFirst();
        for (QAction *action : currentMenu->actions()) {
            syncActionIcon(action, *this, true);
            if (action && action->menu()) {
                menuQueue.append(action->menu());
            }
        }
    }
}

void ToolbarIconManager::applyToWindow(const QWidget *window, QToolBar *toolBar) const {
    if (toolBar) {
        applyToToolbar(toolBar);
    }

    if (!window) {
        return;
    }

    QSet<QAction *> visited;
    const auto actions = window->findChildren<QAction *>();
    visited.reserve(actions.size());
    for (QAction *action : actions) {
        if (action && !visited.contains(action)) {
            visited.insert(action);
            syncActionIcon(action, *this, false);
        }
    }
}
