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

### 1. 10 Built-in Toolbar Icon Packs Suite

SparkNotePad v1.3.8 features 10 distinct, curated vector and bitmap icon packs accessible directly from `Themes > Toolbar Icon Packs`:

- **Notepad++ Classic** (`notepadPlusPlus`): Crisp vector and bitmap recreation of iconic Notepad++ toolbar iconography.
- **Fluent UI** (`fluentUi`): Modern Microsoft Fluent design with rounded geometry, clean styling, and high-definition multi-DPI assets.
- **Fugue** (`fugue`): Renowned pixel-perfect 16×16 and 32×32 iconography designed by Yusuke Kamiyamane.
- **Windows 11** (`windows11`): Microsoft Windows 11 Fluent icons with depth and subtle drop shadows.
- **VS Code Modern** (`vscodeModern`): Clean, minimalist developer iconography inspired by Visual Studio Code.
- **Flat Color** (`flatColor`): High-contrast, vibrant flat color iconography tailored for modern high-resolution displays.
- **Material Design** (`material`): Google Material Design symbol guidelines with balanced weights and curves.
- **Solarized** (`solarized`): Specially tuned color harmonies optimized for Solarized Dark and Light editor themes.
- **High Contrast** (`highContrast`): Ultra-sharp, accessible monochrome outline icons for high-visibility workflows.
- **Windows Vista** (`windowsVista`): Classic Windows Vista Aero Glass vector styling with polished gradients and chrome accents.

### 2. Dynamic Menubar & Right-Click Context Menu Icon Synchronization

Changing toolbar icon packs now instantaneously propagates active iconography across all top-level menubars, submenus, and the editor right-click context menu via `ToolbarIconManager::applyToWindow()`, with automatic fallback ensuring visual elegance and `setIconVisibleInMenu(true)` compliance.

### 3. Dynamic Toolbar Icon Sizing (16×16 to 64×64)

Switch toolbar icon sizes on the fly via `Themes > Toolbar Icon Packs > Toolbar Icon Size` or toolbar right-click:

- **16 × 16** (Compact / Menus)
- **20 × 20** (Medium)
- **24 × 24** (Default Toolbar / Recommended)
- **32 × 32** (Large / High-DPI)
- **48 × 48** (Extra Large)
- **64 × 64** (Ultra High-DPI)

### 4. Tab Bar Plus (+) Button for Instant Document Creation

Editor tab bars now feature an elegant, integrated Plus (`+`) button placed right beside the active tabs (`tabBarPlusButton`), allowing users to spawn a new document buffer with a single click, perfectly supplementing the `Ctrl+N` / `Cmd+N` shortcut.

### 5. Localized Help Menu: DOCNOTE & CHANGELOG

The Help menu now dynamically synchronizes release notes and change logs with the editor's active interface language:

- **Bahasa Indonesia:** Automatically serves `DOCNOTE_id.txt` and `CHANGELOG_id.txt` when Indonesian is selected.
- **English:** Serves `DOCNOTE_en.txt` and `CHANGELOG_en.txt` for English and universal locales.

### 6. Universal High-DPI, Retina & Multi-Monitor Scaling

Scintilla and Qt rendering engines are fully optimized to handle diverse monitor hardware without blurring or scaling artifacts:

- **Generic Displays:** 1366×768, 1280×800, 720p, and standard 1080p monitors.
- **Retina MacBooks:** Intel & Apple Silicon Retina (2x scale factor with ProMotion support).
- **High-Resolution & External Screens:** 2K, 4K, 5K, 6K displays, and Linux fractional scaling (125%, 150%, 175%).
- **LCD-Optimized Font Rendering:** Enabled `SC_EFF_QUALITY_LCD_OPTIMIZED` and double-buffered drawing (`setBufferedDraw(true)`) with instant repaint on `DevicePixelRatioChange` and `ScreenChangeInternal`.

### 7. GitHub Markdown Styling for HTML & Markdown Preview (`MarkdownSafePreviewDock`)

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
