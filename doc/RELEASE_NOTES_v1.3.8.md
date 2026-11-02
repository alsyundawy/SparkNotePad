# SparkNotePad v1.3.8 — Toolbar Icon Packs Suite, Customizer Dialog & Context Menu Ergonomics

**Release Version:** `v1.3.8`<br/>
**Release Date:** 2026-11-02<br/>
**Maintainer:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Core Editing Engine:** Scintilla 5.6.7 (Vendored) & Lexilla 5.5.4 (Vendored)<br/>
**Embedded Lua Core:** Sandboxed Lua 5.5.1 / 5.4.7 (Official Source)<br/>
**Scripting Runtime:** Python 3.13+ Engine Bridge<br/>
**Standard:** ISO C++20 / Qt 6.5–6.12<br/>
**Target Platforms:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu 22.04–26.04, Debian 12–14, Fedora 40–44, RHEL/Rocky 9–11, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x (KDE Plasma 6 & XFCE)

---

## Executive Summary

SparkNotePad v1.3.8 delivers a comprehensive toolbar iconography and button personalization engine inspired by Notepad++, paired with streamlined editor context menu ergonomics, High-DPI multi-pack rendering, and complete linter & SonarLint compliance.

---

## Key Features & Enhancements

### 1. 15 Built-in Toolbar Icon Packs Suite

SparkNotePad v1.3.8 features 15 distinct, curated vector and bitmap icon packs accessible directly from `Themes > Toolbar Icon Packs`:

- **Modern Ribbon (Default)** (`modernRibbon`): High-DPI 20×20 monoline vector SVG suite with refined contours.
- **NotepadNext Classic (Silk)** (`notepadNext`): Authentic 16×16 Silk bitmap icons from upstream NotepadNext release, relocated cleanly to `icons/packs/notepadNext/`.
- **Notepad++ Retro** (`notepadPlusPlus`): High-DPI vector recreation of classic Notepad++ toolbar iconography.
- **android material 2026** (`androidMaterial`): Google Material You / M3 adaptive geometry and tonal depth.
- **Vista 2026** (`windowsVista`): Windows Vista Aero glass aesthetic with rich gloss and volumetric lighting.
- **XP 2026** (`windowsXp`): Authentic Windows XP Luna 3D skeuomorphic nostalgia rebuilt from official 1024×1024 High-Resolution master icons and Cris van Minnen's Directory Opus 10 XP suite (master 64×64 PNG supporting 16×16 to 64×64).
- **Win10 2026** (`windows10`): Windows 10 Metro / MDL2 sharp wireframe monoline aesthetic.
- **Win11 2026** (`windows11`): Microsoft Fluent Design System 2 rebuilt with authentic Directory Opus Windows 11 32px iconography and matching high-definition vector/bitmap assets.
- **Win 7 2026** (`windows7`): Authentic Windows 7/8 Aero Glass Azure suite rebuilt from UltimateDesktops Aero Glass Azure sprite sheet with translucent cyan glass gradients, polished chrome borders, and volumetric glass reflections (master 64×64 PNG).
- **Monterey 2026** (`macosMonterey`): macOS Monterey modern SF Symbols aesthetic with clean Apple styling.
- **NOTEPAD3 ICONPACK** (`notepad3`): Rizonesoft Notepad3 technical teal and sharp developer-focused vectors.
- **NOTEPAD4 ICONPACK** (`notepad4`): Zufuliu Notepad4 modern deep indigo flat vector iconography.
- **NOTEPAD2 ICONPACK** (`notepad2`): Florian Balmer Notepad2 classic retro 16-color nostalgic palette.
- **Monochrome Outline (Minimalist)** (`monochrome`): Clean 1.8px monoline wireframes engineered for dark and light themes.
- **Vibrant DuoTone (Neon/Cyber)** (`vibrantDuotone`): High-luminance electric cyan & radiant magenta dual-tone contrast.

### 2. Dynamic Menubar & Right-Click Context Menu Icon Synchronization

Changing toolbar icon packs now instantaneously propagates active iconography across all top-level menubars, submenus, and the editor right-click context menu via `ToolbarIconManager::applyToWindow()`, with automatic fallback ensuring visual elegance and `setIconVisibleInMenu(true)` compliance.

### 3. Dynamic Toolbar Icon Sizing (16×16 to 64×64)

Switch toolbar icon sizes on the fly via `Themes > Toolbar Icon Packs > Toolbar Icon Size` or toolbar right-click:

- **16 × 16** (Compact)
- **20 × 20** (Medium)
- **24 × 24** (Default / Recommended)
- **32 × 32** (Large / High-DPI)
- **48 × 48** (Extra Large)
- **64 × 64** (Ultra High-DPI)

### 4. GitHub Markdown Styling for HTML & Markdown Preview (`MarkdownSafePreviewDock`)

- Full GitHub Markdown styling matching the `bierner.markdown-preview-github-styles` specification.
- Adaptive light (`#ffffff` / `#1f2328`) and dark (`#0d1117` / `#e6edf3`) palette rendering based on application theme changes.
- Authentic GitHub typography, blockquotes, code blocks, zebra tables, and browser export.

### 5. Visual Toolbar Customizer (`CustomizeToolbarDialog`)

- Intuitive dual-list dialog for adding, removing, and reordering toolbar buttons.
- Drag-and-drop support with `InternalMove`.
- Custom separator insertion support.
- Live search filter for rapid action discovery.
- Reset to Defaults button restoring the standard 33-button layout.

### 6. Editor Right-Click Context Menu Ergonomics

- **Convert Case Relocation**: Moved directly below `Select All` (`ui->actionSelectAll`) for logical text transformation workflows.
- **Compact Aesthetics**: Scaled font down by 1–2px (11px) and icons to 14×14px with sleek 3px padding.
- **Non-Destructive Restoration**: Connected `aboutToHide` signal on context menu to cleanly preserve standard menu bar styling upon menu dismissal.

---

## Empirical Verification

- **Unit Tests**: 100% pass rate (55/55 test cases across 11 test suites).
- **Linter Compliance**: Trunk check, markdownlint, and cppcheck pass with 0 errors.
- **Build Status**: Compiles cleanly with Apple LLVM / Clang 23 on macOS and GCC/Clang on Linux.
