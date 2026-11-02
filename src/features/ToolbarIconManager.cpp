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
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QToolBar>
#include <QWidget>
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

    // 1. Modern Ribbon (SparkNotePad Default SVG)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("modernRibbon"),
        .displayName = tr("Modern Ribbon (Default)"),
        .description = tr("High-DPI 20x20 monoline vector SVG suite with refined contours"),
        .basePath = QStringLiteral(":/icons"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 2. NotepadNext Classic (Original Silk PNG)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepadNext"),
        .displayName = tr("NotepadNext Classic (Silk)"),
        .description = tr("Authentic 16x16 Silk bitmap icons from NotepadNext"),
        .basePath = QStringLiteral(":/icons/packs/notepadNext"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 3. Notepad++ Retro (Classic Nostalgia SVG)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepadPlusPlus"),
        .displayName = tr("Notepad++ Retro (Classic Nostalgia)"),
        .description = tr("High-DPI vector recreations of classic Notepad++ iconography"),
        .basePath = QStringLiteral(":/icons/packs/notepadPlusPlus"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 4. Android Material 2026 (Material 3 / Material You)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("androidMaterial"),
        .displayName = tr("android material 2026"),
        .description = tr("Google Material You / M3 adaptive geometry and tonal depth"),
        .basePath = QStringLiteral(":/icons/packs/androidMaterial"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 5. Windows Vista 2026 (Aero Gloss Glass)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windowsVista"),
        .displayName = tr("Vista 2026"),
        .description = tr("Windows Vista Aero glass aesthetic with rich gloss and volumetric lighting"),
        .basePath = QStringLiteral(":/icons/packs/windowsVista"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 6. Windows XP 2026 (Luna Skeuomorphic)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windowsXp"),
        .displayName = tr("XP 2026"),
        .description = tr("Windows XP Luna skeuomorphic authentic 1024px and DOpus iconography"),
        .basePath = QStringLiteral(":/icons/packs/windowsXp"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 7. Windows 10 2026 (Metro MDL2 Monoline)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windows10"),
        .displayName = tr("Win10 2026"),
        .description = tr("Windows 10 Metro / MDL2 sharp wireframe monoline aesthetic"),
        .basePath = QStringLiteral(":/icons/packs/windows10"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 8. Windows 11 2026 (Fluent 2 Modern)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windows11"),
        .displayName = tr("Win11 2026"),
        .description = tr("Microsoft Fluent Design System 2 with authentic Directory Opus 32px iconography"),
        .basePath = QStringLiteral(":/icons/packs/windows11"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 9. Windows 7 2026 (Aero Glass Translucent)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windows7"),
        .displayName = tr("Win 7 2026"),
        .description = tr("Windows 7/8 Aero Glass Azure translucent glass and chrome iconography"),
        .basePath = QStringLiteral(":/icons/packs/windows7"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 10. macOS Monterey 2026 (SF Symbols Aesthetic)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("macosMonterey"),
        .displayName = tr("Monterey 2026"),
        .description = tr("macOS Monterey modern SF Symbols aesthetic with clean Apple styling"),
        .basePath = QStringLiteral(":/icons/packs/macosMonterey"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 11. NOTEPAD3 ICONPACK (Rizonesoft Technical Teal)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepad3"),
        .displayName = tr("NOTEPAD3 ICONPACK"),
        .description = tr("Rizonesoft Notepad3 technical teal and sharp developer-focused vectors"),
        .basePath = QStringLiteral(":/icons/packs/notepad3"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 12. NOTEPAD4 ICONPACK (Zufuliu Modern Indigo)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepad4"),
        .displayName = tr("NOTEPAD4 ICONPACK"),
        .description = tr("Zufuliu Notepad4 modern deep indigo flat vector iconography"),
        .basePath = QStringLiteral(":/icons/packs/notepad4"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 13. NOTEPAD2 ICONPACK (Florian Balmer Retro Nostalgia)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepad2"),
        .displayName = tr("NOTEPAD2 ICONPACK"),
        .description = tr("Florian Balmer Notepad2 classic retro 16-color nostalgic palette"),
        .basePath = QStringLiteral(":/icons/packs/notepad2"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 14. Monochrome Outline (Minimalist Dark/Light SVG)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("monochrome"),
        .displayName = tr("Monochrome Outline (Minimalist)"),
        .description = tr("Clean 1.8px monoline wireframes engineered for dark and light themes"),
        .basePath = QStringLiteral(":/icons/packs/monochrome"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 15. Vibrant DuoTone (Neon/Cyber Contrast SVG)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("vibrantDuotone"),
        .displayName = tr("Vibrant DuoTone (Neon/Cyber)"),
        .description = tr("High-luminance electric cyan & radiant magenta dual-tone contrast"),
        .basePath = QStringLiteral(":/icons/packs/vibrantDuotone"),
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
        const QString resolvedSavedPack =
            (savedPack == QLatin1String("fluentUi")) ? QStringLiteral("windows11") : savedPack;
        if (findPackById(resolvedSavedPack)) {
            m_currentPackId = resolvedSavedPack;
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
        targetId = QStringLiteral("windows11");
    }

    const auto it = std::ranges::find_if(m_packs, [&targetId](const ToolbarIconPack &p) { return p.id == targetId; });

    if (it != m_packs.end()) {
        return &(*it);
    }
    return nullptr;
}

void ToolbarIconManager::setPack(const QString &packId, QToolBar *toolBar) {
    const QString resolvedId = (packId == QLatin1String("fluentUi")) ? QStringLiteral("windows11") : packId;
    if (!findPackById(resolvedId)) {
        qWarning() << "ToolbarIconManager: Cannot locate icon pack with ID" << packId;
        return;
    }

    m_currentPackId = resolvedId;

    if (m_settings) {
        m_settings->setValue(QStringLiteral("Gui/ToolbarIconSet"), m_currentPackId);
    }

    if (toolBar) {
        applyToWindow(toolBar->window(), toolBar);
    }

    emit packChanged(m_currentPackId);
}

void ToolbarIconManager::setIconSize(const QSize &size, QToolBar *toolBar) {
    m_currentIconSize = size;

    if (m_settings) {
        m_settings->setValue(QStringLiteral("Gui/ToolbarIconSize"), size.width());
    }

    if (toolBar) {
        toolBar->setIconSize(size);
    }

    emit iconSizeChanged(size);
}

QString ToolbarIconManager::customPacksPath() {
    const QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(appData).filePath(QStringLiteral("toolbarIcons"));
}

void ToolbarIconManager::scanCustomPacks() {
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

    if (name.compare(QStringLiteral("New"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("newfile");
    }
    if (name.compare(QStringLiteral("Open"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("openFile");
    }
    if (name.compare(QStringLiteral("Save"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("saved");
    }
    if (name.compare(QStringLiteral("SaveAll"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("saveAll");
    }
    if (name.compare(QStringLiteral("Close"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("closeFile");
    }
    if (name.compare(QStringLiteral("CloseAll"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("closeAll");
    }
    if (name.compare(QStringLiteral("Print"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("printer");
    }
    if (name.compare(QStringLiteral("Cut"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("cut");
    }
    if (name.compare(QStringLiteral("Copy"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("copy");
    }
    if (name.compare(QStringLiteral("Paste"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("paste");
    }
    if (name.compare(QStringLiteral("Undo"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("undo");
    }
    if (name.compare(QStringLiteral("Redo"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("redo");
    }
    if (name.compare(QStringLiteral("Find"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("find");
    }
    if (name.compare(QStringLiteral("Replace"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("findReplace");
    }
    if (name.compare(QStringLiteral("ZoomIn"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("zoomIn");
    }
    if (name.compare(QStringLiteral("ZoomOut"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("zoomOut");
    }
    if (name.compare(QStringLiteral("WordWrap"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("wrap");
    }
    if (name.compare(QStringLiteral("ShowAllCharacters"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("invisibleChar");
    }
    if (name.compare(QStringLiteral("ShowIndentGuide"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("indentGuide");
    }
    if (name.compare(QStringLiteral("MacroRecording"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("startRecord");
    }
    if (name.compare(QStringLiteral("Playback"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("playRecord");
    }
    if (name.compare(QStringLiteral("RunMacroMultipleTimes"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("playRecord_m");
    }
    if (name.compare(QStringLiteral("SaveCurrentRecordedMacro"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("saveRecord");
    }
    if (name.compare(QStringLiteral("Restart"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("restart");
    }
    if (name.compare(QStringLiteral("Preferences"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("settings");
    }
    if (name.compare(QStringLiteral("Exit"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("exit");
    }
    if (name.compare(QStringLiteral("ReadOnly"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("readonly");
    }
    if (name.compare(QStringLiteral("SelectAll"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("select_all");
    }
    if (name.compare(QStringLiteral("Indent"), Qt::CaseInsensitive) == 0 ||
        name.compare(QStringLiteral("IncreaseIndent"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("text_indent");
    }
    if (name.compare(QStringLiteral("RemoveIndent"), Qt::CaseInsensitive) == 0 ||
        name.compare(QStringLiteral("DecreaseIndent"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("text_indent_remove");
    }
    if (name.compare(QStringLiteral("Delete"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("closeFile");
    }
    if (name.compare(QStringLiteral("CopyURL"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("copy");
    }
    if (name.compare(QStringLiteral("OpenURL"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("openFile");
    }
    if (name.compare(QStringLiteral("ShowCharacterInfo"), Qt::CaseInsensitive) == 0) {
        return QStringLiteral("invisibleChar");
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
    QString defaultSvg = QStringLiteral(":/icons/") + canonical + QStringLiteral(".svg");
    if (QFileInfo::exists(defaultSvg)) {
        return defaultSvg;
    }

    // Fallback to PNG
    QString defaultPng = QStringLiteral(":/icons/") + canonical + QStringLiteral(".png");
    if (QFileInfo::exists(defaultPng)) {
        return defaultPng;
    }

    return QString();
}

QIcon ToolbarIconManager::getActionIcon(const QString &actionName, const QString &packId) const {
    const QString targetPackId = packId.isEmpty() ? m_currentPackId : packId;
    const ToolbarIconPack *pack = findPackById(targetPackId);
    if (!pack) {
        pack = findPackById(QStringLiteral("modernRibbon"));
    }

    if (!pack) {
        return QIcon();
    }

    const QString canonical = canonicalNameForAction(actionName);
    const QString filePath = resolveIconFilePath(*pack, canonical);
    if (!filePath.isEmpty()) {
        if (canonical == QStringLiteral("startRecord")) {
            const QString onPath = resolveIconFilePath(*pack, QStringLiteral("stopRecord"));
            if (!onPath.isEmpty()) {
                QIcon dualIcon;
                dualIcon.addFile(filePath, QSize(), QIcon::Normal, QIcon::Off);
                dualIcon.addFile(onPath, QSize(), QIcon::Normal, QIcon::On);
                return dualIcon;
            }
        }
        return QIcon(filePath);
    }

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

        const QIcon icon = getActionIcon(objectName);
        if (!icon.isNull()) {
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

void ToolbarIconManager::applyToWindow(QWidget *window, QToolBar *toolBar) const {
    if (toolBar) {
        applyToToolbar(toolBar);
    }

    if (!window) {
        return;
    }

    const auto actions = window->findChildren<QAction *>();
    for (QAction *action : actions) {
        if (!action || action->isSeparator()) {
            continue;
        }

        if (!action->property("sparkDefaultIcon").isValid()) {
            action->setProperty("sparkDefaultIcon", action->icon());
        }

        const QString objectName = action->objectName();
        if (objectName.isEmpty()) {
            continue;
        }

        const QIcon icon = getActionIcon(objectName);
        if (!icon.isNull()) {
            action->setIcon(icon);
        } else {
            const QIcon defaultIcon = action->property("sparkDefaultIcon").value<QIcon>();
            if (!defaultIcon.isNull()) {
                action->setIcon(defaultIcon);
            }
        }
        action->setIconVisibleInMenu(true);
    }
}
