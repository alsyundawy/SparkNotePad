/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifdef isinf
#undef isinf
#endif
#ifdef isnan
#undef isnan
#endif

#include <array>
#include <QtGlobal>
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
        .description = tr("Authentic pixel-perfect Fugue 3.5.6 iconography by Yusuke Kamiyamane"),
        .basePath = QStringLiteral(":/icons/packs/fugue"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 3. Windows 7 Aero (Refined Glossy)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aero_win7"),
        .displayName = tr("Windows 7 Aero (Refined Glossy)"),
        .description = tr("Refined Windows 7 Aero translucent glass iconography"),
        .basePath = QStringLiteral(":/icons/packs/aero_win7"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 4. Windows 7 (Authentic)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windows7"),
        .displayName = tr("Windows 7"),
        .description = tr("Authentic Windows 7 toolbar iconography"),
        .basePath = QStringLiteral(":/icons/packs/windows7"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 5. Microsoft Fluent UI (Regular Outline)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("fluent_regular"),
        .displayName = tr("Microsoft Fluent UI (Regular Outline)"),
        .description = tr("Microsoft Fluent Design outlined iconography for Windows 11"),
        .basePath = QStringLiteral(":/icons/packs/fluent_regular"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 6. Microsoft Fluent UI (Filled Solid)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("fluent_filled"),
        .displayName = tr("Microsoft Fluent UI (Filled Solid)"),
        .description = tr("Microsoft Fluent Design filled solid iconography"),
        .basePath = QStringLiteral(":/icons/packs/fluent_filled"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 7. Windows 11 (Authentic Fluent)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windows11"),
        .displayName = tr("Windows 11"),
        .description = tr("Authentic Windows 11 Fluent toolbar iconography"),
        .basePath = QStringLiteral(":/icons/packs/windows11"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 8. Notepad++ (Classic)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepadPlusPlus"),
        .displayName = tr("Notepad++ (Classic)"),
        .description = tr("Authentic Notepad++ dual-resolution SVG and PNG suite"),
        .basePath = QStringLiteral(":/icons/packs/notepadPlusPlus"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 11. Glossy Orange (WinRAR Amber)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("glossy_orange"),
        .displayName = tr("Glossy Orange (WinRAR Amber)"),
        .description = tr("Warm glossy orange and amber skeuomorphic iconography"),
        .basePath = QStringLiteral(":/icons/packs/glossy_orange"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 10. Windows XP (Luna Classic)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("windowsXp"),
        .displayName = tr("Windows XP (Luna Classic)"),
        .description = tr("Windows XP Luna skeuomorphic authentic iconography"),
        .basePath = QStringLiteral(":/icons/packs/windowsXp"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 15. SparkNotepad Classic (Silk)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepadNext"),
        .displayName = tr("SparkNotepad Classic (Silk)"),
        .description = tr("Authentic 16x16 Silk bitmap icons from SparkNotepad"),
        .basePath = QStringLiteral(":/icons/packs/notepadNext"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 16. Vibrant DuoTone (Cyberpunk)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("vibrantDuotone"),
        .displayName = tr("Vibrant DuoTone (Cyberpunk)"),
        .description = tr("High-luminance electric cyan & radiant magenta dual-tone contrast"),
        .basePath = QStringLiteral(":/icons/packs/vibrantDuotone"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 17. Monochrome Outline (Minimalist)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("monochrome"),
        .displayName = tr("Monochrome Outline (Minimalist)"),
        .description = tr("Clean 1.8px monoline wireframes engineered for dark and light themes"),
        .basePath = QStringLiteral(":/icons/packs/monochrome"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 18. Elegant Themes (Nicholas Roach)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("eleganticons"),
        .displayName = tr("Elegant Themes (Nicholas Roach)"),
        .description = tr("Authentic classic 48x48 Elegant Themes iconography by Nicholas Roach"),
        .basePath = QStringLiteral(":/icons/packs/eleganticons"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 19. Seven Classic (Glossy Desktop)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("seven"),
        .displayName = tr("Seven Classic (Glossy Desktop)"),
        .description = tr("Classic Windows 7 / Seven glossy desktop iconography suite"),
        .basePath = QStringLiteral(":/icons/packs/seven"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 20. Mignon (Cute Pastel)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("mignon"),
        .displayName = tr("Mignon (Cute Pastel)"),
        .description = tr("Playful modern pastel desktop iconography by Igor Moraes"),
        .basePath = QStringLiteral(":/icons/packs/mignon"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 21. Palette (Modern Vibrant)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("palette"),
        .displayName = tr("Palette (Modern Vibrant)"),
        .description = tr("Vibrant modern flat vector iconography by mjkim0727"),
        .basePath = QStringLiteral(":/icons/packs/palette"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 22. Diagonal (Sleek Angular)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("diagonal"),
        .displayName = tr("Diagonal (Sleek Angular)"),
        .description = tr("Sleek geometric angular vector iconography by Tamas Csabi"),
        .basePath = QStringLiteral(":/icons/packs/diagonal"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 23. Aksara Arch Linux
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-arch-linux"),
        .displayName = tr("Aksara Arch Linux"),
        .description = tr("Minimalist & sharp: Arch cyan, fine lines, terminal aesthetic"),
        .basePath = QStringLiteral(":/icons/packs/aksara-arch-linux"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 24. Aksara elementary OS
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-elementary-os"),
        .displayName = tr("Aksara elementary OS"),
        .description = tr("Vibrant colors, 1px fine outlines, distinctive silhouettes and translucent elements"),
        .basePath = QStringLiteral(":/icons/packs/aksara-elementary-os"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 21. Aksara Rounded
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-icon-pack-rounded"),
        .displayName = tr("Aksara Rounded"),
        .description = tr("Smooth flowing curves and rounded geometric contours"),
        .basePath = QStringLiteral(":/icons/packs/aksara-icon-pack-rounded"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 22. Aksara macOS Big Sur
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-macos-bigsur"),
        .displayName = tr("Aksara macOS Big Sur"),
        .description = tr("Apple macOS modern aesthetic: soft squircle geometry, refined gradients, subtle depth"),
        .basePath = QStringLiteral(":/icons/packs/aksara-macos-bigsur"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 23. Aksara NPP Windows Vista
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-npp-vista"),
        .displayName = tr("Aksara NPP Windows Vista"),
        .description =
            tr("Windows Vista Aero Glass: bright cyan gradients, translucent reflections, 2006 skeuomorphism"),
        .basePath = QStringLiteral(":/icons/packs/aksara-npp-vista"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 34. Aksara NPP Windows 10
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-npp-win10"),
        .displayName = tr("Aksara NPP Windows 10"),
        .description = tr("Segoe MDL2 Assets: flat, clean monochrome, consistent thin strokes, seamless background"),
        .basePath = QStringLiteral(":/icons/packs/aksara-npp-win10"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 35. Aksara NPP Windows 11
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-npp-win11"),
        .displayName = tr("Aksara NPP Windows 11"),
        .description = tr("Segoe Fluent Icons: soft rounded contours, gentle blue accents, modern Mica aesthetic"),
        .basePath = QStringLiteral(":/icons/packs/aksara-npp-win11"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 36. Aksara NPP Windows XP
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-npp-xp"),
        .displayName = tr("Aksara NPP Windows XP"),
        .description = tr("Windows XP Luna: soft nostalgic blue gradients, rounded corners, glossy glass shine"),
        .basePath = QStringLiteral(":/icons/packs/aksara-npp-xp"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 37. Aksara Ubuntu Yaru
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-ubuntu-yaru"),
        .displayName = tr("Aksara Ubuntu Yaru"),
        .description = tr("Ubuntu Yaru / Suru design: warm aubergine & orange accents, squircle framing, soft shadows"),
        .basePath = QStringLiteral(":/icons/packs/aksara-ubuntu-yaru"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 38. Aksara Vista Aero
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("aksara-vista-aero"),
        .displayName = tr("Aksara Vista Aero"),
        .description =
            tr("Glossy and reflective: azure gradients, bright specular highlights, Aero Glass rounded tiles"),
        .basePath = QStringLiteral(":/icons/packs/aksara-vista-aero"),
        .isBuiltIn = true,
        .isPngBased = false,
    });

    // 29. Pastel Minimalist
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("pastel"),
        .displayName = tr("Pastel Minimalist"),
        .description = tr("Soft pastel palette with clean minimalist aesthetic"),
        .basePath = QStringLiteral(":/icons/packs/pastel"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 30. Notepad Pro (Vista 7 Style)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("notepad_pro"),
        .displayName = tr("Notepad Pro (Vista 7 Style)"),
        .description = tr("Skeuomorphic glossy Vista 7 style iconography with modern elegance"),
        .basePath = QStringLiteral(":/icons/packs/notepad_pro"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 31. Royal Blue (Aero Glossy)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("royal_blue"),
        .displayName = tr("Royal Blue (Aero Glossy)"),
        .description = tr("Rich royal blue glossy skeuomorphic desktop iconography"),
        .basePath = QStringLiteral(":/icons/packs/royal_blue"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 32. Fluent Modern Style
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("fluent_modern"),
        .displayName = tr("Fluent Modern Style"),
        .description = tr("Microsoft Fluent Design modern rounded squircle iconography"),
        .basePath = QStringLiteral(":/icons/packs/fluent_modern"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 33. macOS Big Sur Style (Modern)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("macos_bigsur_modern"),
        .displayName = tr("macOS Big Sur Style (Modern)"),
        .description = tr("Apple macOS modern frosted glass squircle iconography"),
        .basePath = QStringLiteral(":/icons/packs/macos_bigsur_modern"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 34. Dark Neon Pro
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("dark_neon_pro"),
        .displayName = tr("Dark Neon Pro"),
        .description = tr("Dark neumorphic squircle tiles with electric neon glowing accents"),
        .basePath = QStringLiteral(":/icons/packs/dark_neon_pro"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 35. macOS Style (Glass Effect)
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("macos_glass"),
        .displayName = tr("macOS Style (Glass Effect)"),
        .description = tr("Translucent glossy glass circular bubble iconography"),
        .basePath = QStringLiteral(":/icons/packs/macos_glass"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 36. Modern Flat & Colorful
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("modern_flat_colorful"),
        .displayName = tr("Modern Flat & Colorful"),
        .description = tr("Clean modern flat vector-styled colorful iconography"),
        .basePath = QStringLiteral(":/icons/packs/modern_flat_colorful"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 37. Glossy 3D Skeuomorphic
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("glossy_3d_skeuomorphic"),
        .displayName = tr("Glossy 3D Skeuomorphic"),
        .description = tr("Rich volumetric glossy 3D skeuomorphic iconography with lighting accents"),
        .basePath = QStringLiteral(":/icons/packs/glossy_3d_skeuomorphic"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 38. Retro 8-Bit Pixel Art
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("retro_8bit_pixel"),
        .displayName = tr("Retro 8-Bit Pixel Art"),
        .description = tr("Authentic retro 8-bit pixel art iconography with crisp contours"),
        .basePath = QStringLiteral(":/icons/packs/retro_8bit_pixel"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 39. Sketchy Hand-Drawn Doodle
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("sketchy_doodle"),
        .displayName = tr("Sketchy Hand-Drawn Doodle"),
        .description = tr("Artistic hand-drawn doodle iconography with sketchy organic strokes"),
        .basePath = QStringLiteral(":/icons/packs/sketchy_doodle"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 01-macos-big-sur
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("01-macos-big-sur"),
        .displayName = tr("macOS Big Sur (2020)"),
        .description =
            tr("Soft 3D, white/translucent squircle with vibrant skeuomorphic elements and subtle drop shadow."),
        .basePath = QStringLiteral(":/icons/packs/01-macos-big-sur"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 02-windows-11-fluent
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("02-windows-11-fluent"),
        .displayName = tr("Windows 11 Fluent (2021)"),
        .description = tr("Dark acrylic squircle with deep glossy blue/cyan backlight and crisp reflections."),
        .basePath = QStringLiteral(":/icons/packs/02-windows-11-fluent"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 03-fluent-glass
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("03-fluent-glass"),
        .displayName = tr("Fluent Glass (2024)"),
        .description = tr("Frosted glass squircle with heavy translucency, pastel gradients, and refractive edges."),
        .basePath = QStringLiteral(":/icons/packs/03-fluent-glass"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 04-dark-neon-pro
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("04-dark-neon-pro"),
        .displayName = tr("Dark Neon Pro (2020)"),
        .description = tr("Dark obsidian background with intense glowing neon purple, pink, and cyan outlines."),
        .basePath = QStringLiteral(":/icons/packs/04-dark-neon-pro"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 05-modern-colorful
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("05-modern-colorful"),
        .displayName = tr("Modern Colorful (2020)"),
        .description = tr("Bright soft 3D squircle with vibrant, saturated colors inspired by modern mobile OS."),
        .basePath = QStringLiteral(":/icons/packs/05-modern-colorful"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 06-metallic-pro
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("06-metallic-pro"),
        .displayName = tr("Metallic Pro (2023)"),
        .description = tr("Brushed metal and chrome squircle with machined silver/grey 3D bevels."),
        .basePath = QStringLiteral(":/icons/packs/06-metallic-pro"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 07-material-design-3
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("07-material-design-3"),
        .displayName = tr("Material Design 3 (2023)"),
        .description = tr("Dynamic color palettes with soft rounded 3D and tonal elevations."),
        .basePath = QStringLiteral(":/icons/packs/07-material-design-3"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 08-neumorphism
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("08-neumorphism"),
        .displayName = tr("Neumorphism (2019)"),
        .description = tr("Soft UI with extruded plastic surfaces and dual light/dark soft shadows."),
        .basePath = QStringLiteral(":/icons/packs/08-neumorphism"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 09-claymorphism
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("09-claymorphism"),
        .displayName = tr("Claymorphism (2022)"),
        .description = tr("Matte 3D clay with puffy, rounded soft geometry and playful tactile warmth."),
        .basePath = QStringLiteral(":/icons/packs/09-claymorphism"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 10-aurora-gradient
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("10-aurora-gradient"),
        .displayName = tr("Aurora Gradient (2021)"),
        .description = tr("Glowing mesh gradients with smooth 3D blends of cyan, purple, and emerald."),
        .basePath = QStringLiteral(":/icons/packs/10-aurora-gradient"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 11-bento-grid
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("11-bento-grid"),
        .displayName = tr("Bento Grid (2023)"),
        .description = tr("Modular, clean distinct colors with glossy card finishes and sharp micro-borders."),
        .basePath = QStringLiteral(":/icons/packs/11-bento-grid"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 12-cyber-y2k
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("12-cyber-y2k"),
        .displayName = tr("Cyber-Y2K (2024)"),
        .description = tr("Metallic liquid silver with vibrant neon pink and cyan specular reflections."),
        .basePath = QStringLiteral(":/icons/packs/12-cyber-y2k"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 13-glassmorphism-light
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("13-glassmorphism-light"),
        .displayName = tr("Glassmorphism Light (2020)"),
        .description = tr("Bright pristine frosted milk-glass with delicate 1px white border and backdrop blur."),
        .basePath = QStringLiteral(":/icons/packs/13-glassmorphism-light"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 14-windows-xp
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("14-windows-xp"),
        .displayName = tr("Windows XP (Retro) (2001)"),
        .description = tr("Glossy Luna blue and meadow green skeuomorphism with thick beveled borders."),
        .basePath = QStringLiteral(":/icons/packs/14-windows-xp"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 15-flat-20-squircle
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("15-flat-20-squircle"),
        .displayName = tr("Flat 2.0 Squircle (2016)"),
        .description = tr("Flat saturated colors elevated by strong, clean multi-tier drop shadows."),
        .basePath = QStringLiteral(":/icons/packs/15-flat-20-squircle"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 16-techno-blueprint
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("16-techno-blueprint"),
        .displayName = tr("Techno Blueprint (2018)"),
        .description = tr("Midnight navy blue with glowing cyan technical CAD schematics and grid markings."),
        .basePath = QStringLiteral(":/icons/packs/16-techno-blueprint"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 17-pastel-dream
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("17-pastel-dream"),
        .displayName = tr("Pastel Dream (2022)"),
        .description = tr("Ultra-soft 3D with muted lavender, mint, and peach pastel tones."),
        .basePath = QStringLiteral(":/icons/packs/17-pastel-dream"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 18-vaporwave
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("18-vaporwave"),
        .displayName = tr("Vaporwave (2016)"),
        .description = tr("Retro neon pink/cyan sunset gradients with 80s aesthetic and grid motifs."),
        .basePath = QStringLiteral(":/icons/packs/18-vaporwave"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 19-neo-brutalism
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("19-neo-brutalism"),
        .displayName = tr("Neo-Brutalism (2024)"),
        .description = tr("High-contrast, chunky 3D with thick solid black outlines (3px) and bold pop colors."),
        .basePath = QStringLiteral(":/icons/packs/19-neo-brutalism"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 20-monochrome-steel
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("20-monochrome-steel"),
        .displayName = tr("Monochrome Steel (2023)"),
        .description = tr("Greyscale machined steel 3D palette with razor-sharp crimson red accents."),
        .basePath = QStringLiteral(":/icons/packs/20-monochrome-steel"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 21-deep-space
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("21-deep-space"),
        .displayName = tr("Deep Space (2025)"),
        .description = tr("Cosmic obsidian with purple/indigo stellar nebula dust and star speculars."),
        .basePath = QStringLiteral(":/icons/packs/21-deep-space"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 22-liquid-glass
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("22-liquid-glass"),
        .displayName = tr("Liquid Glass (2024)"),
        .description = tr("Apple Vision Pro spatial UI, fluid optical refraction, caustics, and watery depth."),
        .basePath = QStringLiteral(":/icons/packs/22-liquid-glass"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 23-retro-mac-os-classic
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("23-retro-mac-os-classic"),
        .displayName = tr("Retro Mac OS Classic (1991)"),
        .description = tr("Platinum OS 9 / System 7 beveled 3D grey with subtle classic pixelated shading."),
        .basePath = QStringLiteral(":/icons/packs/23-retro-mac-os-classic"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 24-holographic
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("24-holographic"),
        .displayName = tr("Holographic (2022)"),
        .description = tr("Iridescent pearl gradient with shifting rainbow spectra and lustrous sheen."),
        .basePath = QStringLiteral(":/icons/packs/24-holographic"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 25-earthy-organic
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("25-earthy-organic"),
        .displayName = tr("Earthy Organic (2024)"),
        .description = tr("Matte natural ceramic 3D finish with deep sage greens, warm sandstone, and terracotta."),
        .basePath = QStringLiteral(":/icons/packs/25-earthy-organic"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 26-ai-generative
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("26-ai-generative"),
        .displayName = tr("AI Generative (2026)"),
        .description = tr("Neural holographic depth, fluid morphic geometry, and glowing synaptic edges."),
        .basePath = QStringLiteral(":/icons/packs/26-ai-generative"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 27-high-tech-carbon
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("27-high-tech-carbon"),
        .displayName = tr("High-Tech Carbon (2019)"),
        .description = tr("3D woven carbon fiber texture with racing red backlight and precision matte finish."),
        .basePath = QStringLiteral(":/icons/packs/27-high-tech-carbon"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 28-synthwave-neon
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("28-synthwave-neon"),
        .displayName = tr("Synthwave Neon (2018)"),
        .description = tr("Outrun grid lines with intense magenta, purple, and sunset orange neon glow."),
        .basePath = QStringLiteral(":/icons/packs/28-synthwave-neon"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 29-fluent-acrylic
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("29-fluent-acrylic"),
        .displayName = tr("Fluent Acrylic (2022)"),
        .description = tr("Windows acrylic blur effect with microscopic noise grain and deep translucency."),
        .basePath = QStringLiteral(":/icons/packs/29-fluent-acrylic"),
        .isBuiltIn = true,
        .isPngBased = true,
    });

    // 30-swiss-minimal-3d
    m_packs.append(ToolbarIconPack{
        .id = QStringLiteral("30-swiss-minimal-3d"),
        .displayName = tr("Swiss Minimal 3D (2015)"),
        .description =
            tr("International Typographic Style in 3D: Vermilion red, black, and white geometric precision."),
        .basePath = QStringLiteral(":/icons/packs/30-swiss-minimal-3d"),
        .isBuiltIn = true,
        .isPngBased = true,
    });
}

void ToolbarIconManager::initialize(ApplicationSettings *settings) {
    m_settings = settings;

    scanCustomPacks();

    if (m_settings) {
        const QString savedPack =
            m_settings->value(QStringLiteral("Gui/ToolbarIconSet"), QStringLiteral("modernRibbon")).toString();
        const ToolbarIconPack *pack = findPackById(savedPack);
        m_currentPackId = pack ? pack->id : QStringLiteral("modernRibbon");

        const int savedSize = m_settings->value(QStringLiteral("Gui/ToolbarIconSize"), 24).toInt();
        const int clampedSize = (savedSize >= 16 && savedSize <= 64) ? savedSize : 24;
        m_currentIconSize = QSize(clampedSize, clampedSize);
    }
}

const QList<ToolbarIconPack> &ToolbarIconManager::availablePacks() const {
    return m_packs;
}

namespace {
QString normalizePackId(const QString &packId) {
    static const QHash<QString, QString> s_aliasMap = {
        {QStringLiteral("fluentUi"), QStringLiteral("fluent_regular")},
        {QStringLiteral("filledFluentUi"), QStringLiteral("fluent_filled")},
        {QStringLiteral("seven_icons"), QStringLiteral("seven")},
        {QStringLiteral("sevenIcons"), QStringLiteral("seven")},
        {QStringLiteral("aksara_arch_linux"), QStringLiteral("aksara-arch-linux")},
        {QStringLiteral("aksara_elementary_os"), QStringLiteral("aksara-elementary-os")},
        {QStringLiteral("aksara_rounded"), QStringLiteral("aksara-icon-pack-rounded")},
        {QStringLiteral("aksara-rounded"), QStringLiteral("aksara-icon-pack-rounded")},
        {QStringLiteral("aksara_icon_pack_rounded"), QStringLiteral("aksara-icon-pack-rounded")},
        {QStringLiteral("aksara_macos_bigsur"), QStringLiteral("aksara-macos-bigsur")},
        {QStringLiteral("aksara_npp_vista"), QStringLiteral("aksara-npp-vista")},
        {QStringLiteral("aksara_npp_win10"), QStringLiteral("aksara-npp-win10")},
        {QStringLiteral("aksara_npp_win11"), QStringLiteral("aksara-npp-win11")},
        {QStringLiteral("aksara_npp_xp"), QStringLiteral("aksara-npp-xp")},
        {QStringLiteral("aksara_ubuntu_yaru"), QStringLiteral("aksara-ubuntu-yaru")},
        {QStringLiteral("aksara_vista_aero"), QStringLiteral("aksara-vista-aero")},
        {QStringLiteral("notepadPro"), QStringLiteral("notepad_pro")},
        {QStringLiteral("notepad-pro"), QStringLiteral("notepad_pro")},
        {QStringLiteral("royalBlue"), QStringLiteral("royal_blue")},
        {QStringLiteral("royal-blue"), QStringLiteral("royal_blue")},
        {QStringLiteral("fluentModern"), QStringLiteral("fluent_modern")},
        {QStringLiteral("fluent-modern"), QStringLiteral("fluent_modern")},
        {QStringLiteral("macosBigSurModern"), QStringLiteral("macos_bigsur_modern")},
        {QStringLiteral("macos-bigsur-modern"), QStringLiteral("macos_bigsur_modern")},
        {QStringLiteral("darkNeonPro"), QStringLiteral("dark_neon_pro")},
        {QStringLiteral("dark-neon-pro"), QStringLiteral("dark_neon_pro")},
        {QStringLiteral("macosGlass"), QStringLiteral("macos_glass")},
        {QStringLiteral("macos-glass"), QStringLiteral("macos_glass")},
        {QStringLiteral("modernFlatColorful"), QStringLiteral("modern_flat_colorful")},
        {QStringLiteral("modern-flat-colorful"), QStringLiteral("modern_flat_colorful")},
        {QStringLiteral("glossy3dSkeuomorphic"), QStringLiteral("glossy_3d_skeuomorphic")},
        {QStringLiteral("glossy-3d-skeuomorphic"), QStringLiteral("glossy_3d_skeuomorphic")},
        {QStringLiteral("retro8bitPixel"), QStringLiteral("retro_8bit_pixel")},
        {QStringLiteral("retro-8bit-pixel"), QStringLiteral("retro_8bit_pixel")},
        {QStringLiteral("sketchyDoodle"), QStringLiteral("sketchy_doodle")},
        {QStringLiteral("sketchy-doodle"), QStringLiteral("sketchy_doodle")},
        {QStringLiteral("macos-big-sur"), QStringLiteral("01-macos-big-sur")},
        {QStringLiteral("macos_big_sur"), QStringLiteral("01-macos-big-sur")},
        {QStringLiteral("macosBigSur"), QStringLiteral("01-macos-big-sur")},
        {QStringLiteral("01_macos_big_sur"), QStringLiteral("01-macos-big-sur")},
        {QStringLiteral("windows-11-fluent"), QStringLiteral("02-windows-11-fluent")},
        {QStringLiteral("windows_11_fluent"), QStringLiteral("02-windows-11-fluent")},
        {QStringLiteral("windows11Fluent"), QStringLiteral("02-windows-11-fluent")},
        {QStringLiteral("02_windows_11_fluent"), QStringLiteral("02-windows-11-fluent")},
        {QStringLiteral("fluent-glass"), QStringLiteral("03-fluent-glass")},
        {QStringLiteral("fluent_glass"), QStringLiteral("03-fluent-glass")},
        {QStringLiteral("fluentGlass"), QStringLiteral("03-fluent-glass")},
        {QStringLiteral("03_fluent_glass"), QStringLiteral("03-fluent-glass")},
        {QStringLiteral("dark-neon-pro"), QStringLiteral("04-dark-neon-pro")},
        {QStringLiteral("dark_neon_pro"), QStringLiteral("04-dark-neon-pro")},
        {QStringLiteral("darkNeonPro"), QStringLiteral("04-dark-neon-pro")},
        {QStringLiteral("04_dark_neon_pro"), QStringLiteral("04-dark-neon-pro")},
        {QStringLiteral("modern-colorful"), QStringLiteral("05-modern-colorful")},
        {QStringLiteral("modern_colorful"), QStringLiteral("05-modern-colorful")},
        {QStringLiteral("modernColorful"), QStringLiteral("05-modern-colorful")},
        {QStringLiteral("05_modern_colorful"), QStringLiteral("05-modern-colorful")},
        {QStringLiteral("metallic-pro"), QStringLiteral("06-metallic-pro")},
        {QStringLiteral("metallic_pro"), QStringLiteral("06-metallic-pro")},
        {QStringLiteral("metallicPro"), QStringLiteral("06-metallic-pro")},
        {QStringLiteral("06_metallic_pro"), QStringLiteral("06-metallic-pro")},
        {QStringLiteral("material-design-3"), QStringLiteral("07-material-design-3")},
        {QStringLiteral("material_design_3"), QStringLiteral("07-material-design-3")},
        {QStringLiteral("materialDesign3"), QStringLiteral("07-material-design-3")},
        {QStringLiteral("07_material_design_3"), QStringLiteral("07-material-design-3")},
        {QStringLiteral("neumorphism"), QStringLiteral("08-neumorphism")},
        {QStringLiteral("neumorphism"), QStringLiteral("08-neumorphism")},
        {QStringLiteral("neumorphism"), QStringLiteral("08-neumorphism")},
        {QStringLiteral("08_neumorphism"), QStringLiteral("08-neumorphism")},
        {QStringLiteral("claymorphism"), QStringLiteral("09-claymorphism")},
        {QStringLiteral("claymorphism"), QStringLiteral("09-claymorphism")},
        {QStringLiteral("claymorphism"), QStringLiteral("09-claymorphism")},
        {QStringLiteral("09_claymorphism"), QStringLiteral("09-claymorphism")},
        {QStringLiteral("aurora-gradient"), QStringLiteral("10-aurora-gradient")},
        {QStringLiteral("aurora_gradient"), QStringLiteral("10-aurora-gradient")},
        {QStringLiteral("auroraGradient"), QStringLiteral("10-aurora-gradient")},
        {QStringLiteral("10_aurora_gradient"), QStringLiteral("10-aurora-gradient")},
        {QStringLiteral("bento-grid"), QStringLiteral("11-bento-grid")},
        {QStringLiteral("bento_grid"), QStringLiteral("11-bento-grid")},
        {QStringLiteral("bentoGrid"), QStringLiteral("11-bento-grid")},
        {QStringLiteral("11_bento_grid"), QStringLiteral("11-bento-grid")},
        {QStringLiteral("cyber-y2k"), QStringLiteral("12-cyber-y2k")},
        {QStringLiteral("cyber_y2k"), QStringLiteral("12-cyber-y2k")},
        {QStringLiteral("cyberY2k"), QStringLiteral("12-cyber-y2k")},
        {QStringLiteral("12_cyber_y2k"), QStringLiteral("12-cyber-y2k")},
        {QStringLiteral("glassmorphism-light"), QStringLiteral("13-glassmorphism-light")},
        {QStringLiteral("glassmorphism_light"), QStringLiteral("13-glassmorphism-light")},
        {QStringLiteral("glassmorphismLight"), QStringLiteral("13-glassmorphism-light")},
        {QStringLiteral("13_glassmorphism_light"), QStringLiteral("13-glassmorphism-light")},
        {QStringLiteral("windows-xp"), QStringLiteral("14-windows-xp")},
        {QStringLiteral("windows_xp"), QStringLiteral("14-windows-xp")},
        {QStringLiteral("windowsXp"), QStringLiteral("14-windows-xp")},
        {QStringLiteral("14_windows_xp"), QStringLiteral("14-windows-xp")},
        {QStringLiteral("flat-20-squircle"), QStringLiteral("15-flat-20-squircle")},
        {QStringLiteral("flat_20_squircle"), QStringLiteral("15-flat-20-squircle")},
        {QStringLiteral("flat20Squircle"), QStringLiteral("15-flat-20-squircle")},
        {QStringLiteral("15_flat_20_squircle"), QStringLiteral("15-flat-20-squircle")},
        {QStringLiteral("techno-blueprint"), QStringLiteral("16-techno-blueprint")},
        {QStringLiteral("techno_blueprint"), QStringLiteral("16-techno-blueprint")},
        {QStringLiteral("technoBlueprint"), QStringLiteral("16-techno-blueprint")},
        {QStringLiteral("16_techno_blueprint"), QStringLiteral("16-techno-blueprint")},
        {QStringLiteral("pastel-dream"), QStringLiteral("17-pastel-dream")},
        {QStringLiteral("pastel_dream"), QStringLiteral("17-pastel-dream")},
        {QStringLiteral("pastelDream"), QStringLiteral("17-pastel-dream")},
        {QStringLiteral("17_pastel_dream"), QStringLiteral("17-pastel-dream")},
        {QStringLiteral("vaporwave"), QStringLiteral("18-vaporwave")},
        {QStringLiteral("vaporwave"), QStringLiteral("18-vaporwave")},
        {QStringLiteral("vaporwave"), QStringLiteral("18-vaporwave")},
        {QStringLiteral("18_vaporwave"), QStringLiteral("18-vaporwave")},
        {QStringLiteral("neo-brutalism"), QStringLiteral("19-neo-brutalism")},
        {QStringLiteral("neo_brutalism"), QStringLiteral("19-neo-brutalism")},
        {QStringLiteral("neoBrutalism"), QStringLiteral("19-neo-brutalism")},
        {QStringLiteral("19_neo_brutalism"), QStringLiteral("19-neo-brutalism")},
        {QStringLiteral("monochrome-steel"), QStringLiteral("20-monochrome-steel")},
        {QStringLiteral("monochrome_steel"), QStringLiteral("20-monochrome-steel")},
        {QStringLiteral("monochromeSteel"), QStringLiteral("20-monochrome-steel")},
        {QStringLiteral("20_monochrome_steel"), QStringLiteral("20-monochrome-steel")},
        {QStringLiteral("deep-space"), QStringLiteral("21-deep-space")},
        {QStringLiteral("deep_space"), QStringLiteral("21-deep-space")},
        {QStringLiteral("deepSpace"), QStringLiteral("21-deep-space")},
        {QStringLiteral("21_deep_space"), QStringLiteral("21-deep-space")},
        {QStringLiteral("liquid-glass"), QStringLiteral("22-liquid-glass")},
        {QStringLiteral("liquid_glass"), QStringLiteral("22-liquid-glass")},
        {QStringLiteral("liquidGlass"), QStringLiteral("22-liquid-glass")},
        {QStringLiteral("22_liquid_glass"), QStringLiteral("22-liquid-glass")},
        {QStringLiteral("retro-mac-os-classic"), QStringLiteral("23-retro-mac-os-classic")},
        {QStringLiteral("retro_mac_os_classic"), QStringLiteral("23-retro-mac-os-classic")},
        {QStringLiteral("retroMacOsClassic"), QStringLiteral("23-retro-mac-os-classic")},
        {QStringLiteral("23_retro_mac_os_classic"), QStringLiteral("23-retro-mac-os-classic")},
        {QStringLiteral("holographic"), QStringLiteral("24-holographic")},
        {QStringLiteral("holographic"), QStringLiteral("24-holographic")},
        {QStringLiteral("holographic"), QStringLiteral("24-holographic")},
        {QStringLiteral("24_holographic"), QStringLiteral("24-holographic")},
        {QStringLiteral("earthy-organic"), QStringLiteral("25-earthy-organic")},
        {QStringLiteral("earthy_organic"), QStringLiteral("25-earthy-organic")},
        {QStringLiteral("earthyOrganic"), QStringLiteral("25-earthy-organic")},
        {QStringLiteral("25_earthy_organic"), QStringLiteral("25-earthy-organic")},
        {QStringLiteral("ai-generative"), QStringLiteral("26-ai-generative")},
        {QStringLiteral("ai_generative"), QStringLiteral("26-ai-generative")},
        {QStringLiteral("aiGenerative"), QStringLiteral("26-ai-generative")},
        {QStringLiteral("26_ai_generative"), QStringLiteral("26-ai-generative")},
        {QStringLiteral("high-tech-carbon"), QStringLiteral("27-high-tech-carbon")},
        {QStringLiteral("high_tech_carbon"), QStringLiteral("27-high-tech-carbon")},
        {QStringLiteral("highTechCarbon"), QStringLiteral("27-high-tech-carbon")},
        {QStringLiteral("27_high_tech_carbon"), QStringLiteral("27-high-tech-carbon")},
        {QStringLiteral("synthwave-neon"), QStringLiteral("28-synthwave-neon")},
        {QStringLiteral("synthwave_neon"), QStringLiteral("28-synthwave-neon")},
        {QStringLiteral("synthwaveNeon"), QStringLiteral("28-synthwave-neon")},
        {QStringLiteral("28_synthwave_neon"), QStringLiteral("28-synthwave-neon")},
        {QStringLiteral("fluent-acrylic"), QStringLiteral("29-fluent-acrylic")},
        {QStringLiteral("fluent_acrylic"), QStringLiteral("29-fluent-acrylic")},
        {QStringLiteral("fluentAcrylic"), QStringLiteral("29-fluent-acrylic")},
        {QStringLiteral("29_fluent_acrylic"), QStringLiteral("29-fluent-acrylic")},
        {QStringLiteral("swiss-minimal-3d"), QStringLiteral("30-swiss-minimal-3d")},
        {QStringLiteral("swiss_minimal_3d"), QStringLiteral("30-swiss-minimal-3d")},
        {QStringLiteral("swissMinimal3d"), QStringLiteral("30-swiss-minimal-3d")},
        {QStringLiteral("30_swiss_minimal_3d"), QStringLiteral("30-swiss-minimal-3d")},
    };
    return s_aliasMap.value(packId, packId);
}
}  // namespace

const ToolbarIconPack *ToolbarIconManager::findPackById(const QString &packId) const {
    const QString targetId = normalizePackId(packId);
    const qsizetype packCount = m_packs.size();
    for (qsizetype i = 0; i < packCount; ++i) {
        if (m_packs.at(i).id == targetId) {
            return &m_packs.at(i);
        }
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
        {QStringLiteral("pasteandmatchstyle"), QStringLiteral("paste")},
        {QStringLiteral("undo"), QStringLiteral("undo")},
        {QStringLiteral("redo"), QStringLiteral("redo")},
        {QStringLiteral("find"), QStringLiteral("find")},
        {QStringLiteral("searchgoogle"), QStringLiteral("find")},
        {QStringLiteral("google"), QStringLiteral("find")},
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
        aliases << QStringLiteral("new") << QStringLiteral("new-file");
    } else if (canonical == QStringLiteral("find")) {
        aliases << QStringLiteral("zoom-reset") << QStringLiteral("search");
    } else if (canonical == QStringLiteral("findReplace")) {
        aliases << QStringLiteral("split-view") << QStringLiteral("replace");
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
            const int newLit = qBound(180, 240 - lit, 255);
            return QColor::fromHsl(hue, 0, newLit, a).rgba();
        }
        return px;
    }

    if (lit < 155) {
        const int newLit = qBound(155, 155 + (lit * 75 / 155), 235);
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
        if (const QPixmap px =
                QPixmap::fromImage(adapted.scaled(sz, sz, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
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

void addDpiPixmapsToIcon(QIcon &icon, const QPixmap &px, int sz, QIcon::Mode mode = QIcon::Normal,
                         QIcon::State state = QIcon::Off) {
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
            addDpiPixmapsToIcon(dualIcon, QPixmap::fromImage(adaptImageForTheme(offImg, isDark)), sz, QIcon::Normal,
                                QIcon::Off);
        }
        if (const QImage onImg(szDir + QStringLiteral("/stopRecord.png")); !onImg.isNull()) {
            addDpiPixmapsToIcon(dualIcon, QPixmap::fromImage(adaptImageForTheme(onImg, isDark)), sz, QIcon::Normal,
                                QIcon::On);
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
    const QString cacheKey =
        QStringLiteral("%1:%2:%3")
            .arg(targetPackId, canonical, m_isDarkMode ? QStringLiteral("1") : QStringLiteral("0"));

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
    bool hasMultiResolution = false;
    const qsizetype candidateCount = candidates.size();
    for (qsizetype i = 0; i < candidateCount; ++i) {
        if (QFileInfo::exists(pack->basePath + QStringLiteral("/24x24/") + candidates.at(i) + QStringLiteral(".png"))) {
            hasMultiResolution = true;
            break;
        }
    }
    if (hasMultiResolution) {
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
