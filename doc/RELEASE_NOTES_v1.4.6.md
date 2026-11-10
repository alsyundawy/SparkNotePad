# SparkNotePad v1.4.6 — 41 New Toolbar Icon Packs (69 Total), Context Menu Ergonomics, General Preferences, WCAG 2.1 Contrast Safety & Standard 30MB–40MB DMG Packaging

**Release Version:** `v1.4.6`<br/>
**Release Date:** 2026-11-10<br/>
**Maintainer:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Core Editing Engine:** Scintilla 5.6.9 (Vendored Official) & Lexilla 5.5.5 (Vendored Official)<br/>
**Embedded Lua Core:** Sandboxed Lua 5.5.1 (Official Source)<br/>
**Scripting Runtime:** Python 3.13+ Engine Bridge<br/>
**Standard:** ISO C++20 / Qt 6.5–6.12<br/>
**Target Platforms:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu 22.04–26.04, Debian 12–14, Fedora 40–44, RHEL/Rocky 9–11, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x (KDE Plasma 6 & XFCE)

---

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="assets/sparknotepad-desktop-banner.jpg" width="100%" alt="SparkNotePad Desktop Suite — Native Scintilla &amp; Qt Text and Source Code Editor for Linux &amp; macOS" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="logo.png" width="128" height="128" alt="SparkNotePad Native macOS &amp; Linux Application Icon" />
  </a>
</p>

<h1 align="center">SparkNotePad</h1>

<p align="center">
  <b>SparkNotePad — Lightweight, blazing-fast native Scintilla &amp; Qt text and source code editor for Linux, FreeBSD, and macOS.</b><br>
  <i>Engineered with Qt 6 and native Scintilla 5.6.9 / Lexilla 5.5.5 core for maximum execution performance and minimal memory footprint</i>
</p>

---

> Lead Developer & Maintainer:<br>
> **[`HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)`](https://github.com/alsyundawy)** — [`ALSYUNDAWY IT SOLUTION`](https://alsyundawy.com)<br>
>
> 🍏 **[`Latest Releases (v1.4.6)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
> 📖 **[`Release DocNotes (DOCNOTE.md)`](DOCNOTE.md)** &nbsp;|&nbsp;
> 📜 **[`Changelog (CHANGELOG.md)`](CHANGELOG.md)** &nbsp;|&nbsp;
> 🌐 **[`Platform Support Matrix`](doc/PLATFORM_SUPPORT.md)** &nbsp;|&nbsp;
> 🔱 **[`FreeBSD Build Guide (KDE6 & XFCE)`](doc/BUILD_FREEBSD_KDE_XFCE.md)** &nbsp;|&nbsp;
> 📋 **[`FreeBSD Analysis & Tasklist`](doc/FREEBSD_BUILD_ANALYSIS_AND_TASKLIST.md)** &nbsp;|&nbsp;
> ⚖️ **[`NotepadNext vs SparkNotePad Comparison`](doc/NOTEPADNEXT_VS_SPARKNOTEPAD_COMPARISON.md)** &nbsp;|&nbsp;
> 🐛 **[`Issue Tracker`](https://github.com/alsyundawy/SparkNotePad/issues)** &nbsp;|&nbsp;
> 💖 **[`Support via PayPal`](https://www.paypal.me/alsyundawy)**

---

## ⚡ Executive Summary

SparkNotePad **v1.4.6** delivers high-impact visual customization, editor context menu ergonomics, general preferences options, WCAG 2.1 mathematical contrast safety for syntax highlighting, and a canonical 30MB–40MB lightweight macOS DMG packaging standard over version 1.4.5. This release adds 41 new multi-resolution toolbar icon packs (expanding the built-in library from 28 to 69 suites), introduces Paste & Match Style and Google search directly into the editor context menu, provides dedicated user preferences for encoding auto-detection and Save All confirmation, elevates syntax styling across 70+ themes with guaranteed 4.0:1 contrast ratios and gold comment badges, and hardens release packaging to 38MB–39MB with verified runtime launching.

---

## 🌟 Highlights & Key Innovations in v1.4.6

### 1. 41 New Toolbar Icon Packs Expansion (69 Total Built-In Suites)

- **Unrivaled Multi-Resolution Visual Customization**:
  - Expanded the built-in icon pack library from 28 packs in v1.4.5 to **69 complete multi-resolution suites** (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
    - **11 Thematic Suites**: `royal_blue`, `notepad_pro`, `pastel`, `fluent_modern`, `macos_bigsur_modern`, `dark_neon_pro`, `macos_glass`, `modern_flat_colorful`, `glossy_3d_skeuomorphic`, `retro_8bit_pixel`, `sketchy_doodle`.
    - **30 Modern Suites**: `01-macos-big-sur`, `02-windows-11-fluent`, `03-arc-browser-glow`, `04-nordic-frost-minimal`, `05-dracula-cyber-neon`, `06-catppuccin-macchiato`, `07-tokyo-night-storm`, `08-synthwave-80s`, `09-monokai-pro-vivid`, `10-solarized-accent`, `11-retro-commodore-64`, `12-retro-win95-classic`, `13-matrix-phosphor-green`, `14-gameboy-nostalgia`, `15-pixel-dungeon-rpg`, `16-claymorphism-3d`, `17-skeuomorphic-leather-brass`, `18-neu-brutalism-bold`, `19-isometric-voxel-3d`, `20-origami-paper-fold`, `21-cyberpunk-2077-yellow`, `22-lofi-aesthetic-grain`, `23-vaporwave-pastel-sunset`, `24-astronomy-deep-space`, `25-botanical-nature-green`, `26-blueprint-schematic`, `27-chalkboard-handdrawn`, `28-gouache-art-paint`, `29-stained-glass-mosaic`, `30-swiss-minimal-3d`.
  - Adheres strictly to the clean Notepad++ Classic visual standard: icons are pure isolated graphics free from dark background squares, fake floor shadows, gray halos, or embossed bevels.
  - Floating seamlessly above the permanent dark toolbar (`#21252b`) with 18% breathing margins preventing edge clipping on Retina displays.

### 2. Editor Context Menu Ergonomics & Productivity Actions

- **Refined Right-Click Workflow**:
  - Added **Paste & Match Style** (`pasteAndMatchStyle`, `Cmd+Alt+Shift+V` on macOS, `Ctrl+Shift+V` on Linux/BSD) to paste clipboard text stripped of external rich styling, automatically matching the current editor indentation level.
  - Added **Search on Google** (`searchOnGoogle`, `Cmd+Alt+G` on macOS, `Ctrl+Alt+G` on Linux/BSD) to immediately open the default web browser and query selected text or the word under the cursor.
  - Streamlined case conversion to direct **UPPERCASE** and **lowercase** actions directly within the primary context menu, eliminating multi-tier nested submenus.
  - Removed outdated Run and Bash encode/decode actions from the context menu to eliminate clutter and focus on core editing ergonomics.

### 3. General Preferences Additions

- **Granular Control in Preferences > General**:
  - Added toggle to Enable / Disable Character Encoding Auto-detection (`enableEncodingAutoDetection`).
  - Added toggle to Enable / Disable Save All Confirmation dialog (`enableSaveAllConfirmation`).
  - All preference values are persisted cleanly in `ApplicationSettings` across application sessions.

### 4. WCAG 2.1 Contrast Safety & Syntax Highlighting Overhaul

- **Mathematically Verified Visual Ergonomics**:
  - Integrated WCAG 2.1 relative luminance and contrast calculations (`0.2126*R + 0.7152*G + 0.0722*B`), guaranteeing a minimum contrast ratio of 4.0:1 across all 70+ color themes.
  - Language-family specific semantic styling tailored for C/C++, Python, Web/HTML/JS/CSS, Markdown progressive H1-H6 headings, SQL, Shell scripts, and YAML.
  - High-visibility Gold Badges (`#FFD700`, bold) for TODO, FIXME, BUG, HACK, and NOTE comment markers.
  - Warm golden amber coloring for string escape sequences (`\n`, `\t`, `\xHH`).
  - Dynamic theme-synchronized rounded border box indicator for SmartHighlighter word occurrences.

### 5. Standard 30MB–40MB Lightweight macOS DMG Packaging Standard

- **Zero-Bloatware Packaging Integrity**:
  - Maintained official Qt universal binaries and standard image format plugins (`libqjpeg`, `libqgif`, `libqwebp`), resulting in a 38MB–39MB release DMG.
  - Strictly within the mandated 30MB–40MB footprint ceiling (no bloatware frameworks such as `QtPdf`, `QtQml`, `QtQuick`, or `QtVirtualKeyboard`).
  - Packaged via native `hdiutil` preserving pre-baked `.DS_Store` layout preset (680x440), Retina background graphics (`dmg-background@2x.png`), custom volume icons (`.VolumeIcon.icns`), and automated CRC32 verification.

### 6. 14-Pillar Code Quality & Verification Suite

SparkNotePad v1.4.6 passes all 14 Pillars of Production Software Engineering:

- **Zero Linter Errors**: Clean results across `clangd --check`, `cppcheck` (171 files in `src/`), `clang-tidy`, SonarLint (`sonarsource.sonarlint-vscode`), and Trunk linters.
- **Default CMake Unix Makefiles**: Built using standard Unix Makefiles with controlled concurrency (Ninja permanently prohibited per `AGENTS.md`).
- **100% Test Pass Rate**: All unit tests in `SparkNotePadUnitTests` passing with 0 failures.
- **100% Bilingual Parity**: 1,850 translated entries synchronized with 0 unfinished entries across English and Indonesian catalogs.

---

## 📦 Official Release Binaries & Assets

| Asset Name                                     | Target Platform / Description                                        | Architecture  | Format                    |
| :--------------------------------------------- | :------------------------------------------------------------------- | :------------ | :------------------------ |
| **`SparkNotePad-v1.4.6-x64.dmg`**              | macOS Canonical Release (macOS 11.0+, Qt 6.5 LTS)                    | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-arm64.dmg`**            | macOS Canonical Release (macOS 11.0+, Qt 6.5 LTS)                    | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.2-x64.dmg`**        | macOS Catalina Legacy Release (macOS 10.15+, Qt 6.2 LTS)             | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.2-arm64.dmg`**      | macOS Legacy Release (macOS 11.0+, Qt 6.2 LTS)                       | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.5-x64.dmg`**        | macOS Standard Release (macOS 11.0+, Qt 6.5 LTS)                     | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.5-arm64.dmg`**      | macOS Standard Release (macOS 11.0+, Qt 6.5 LTS)                     | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.8-x64.dmg`**        | macOS Release (macOS 12.0+, Qt 6.8 LTS)                              | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.8-arm64.dmg`**      | macOS Release (macOS 12.0+, Qt 6.8 LTS)                              | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.10-x64.dmg`**       | macOS Release (macOS 13.0+, Qt 6.10)                                 | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.10-arm64.dmg`**     | macOS Release (macOS 14.0+, Qt 6.10)                                 | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.11-x64.dmg`**       | macOS Release (macOS 13.0+, Qt 6.11)                                 | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.11-arm64.dmg`**     | macOS Release (macOS 14.0+, Qt 6.11)                                 | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.12-x64.dmg`**       | macOS Cutting-Edge Release (macOS 13.0+, Qt 6.12)                    | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.12-arm64.dmg`**     | macOS Cutting-Edge Release (macOS 14.0+, Qt 6.12)                    | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`sparknotepad_1.4.6_amd64.deb`**             | Linux Debian / Ubuntu Canonical (`glibc 2.35+`, Qt 6.5 LTS)          | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.2_amd64.deb`**       | Linux Ubuntu 20.04 LTS Legacy (`glibc 2.31+`, Qt 6.2 LTS)            | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.5_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.5 LTS)                    | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.8_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.8 LTS)                    | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.8_arm64.deb`**       | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.8 LTS)              | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.10_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.10)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.10_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.10)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.11_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.11)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.11_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.11)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.12_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.39+`, Qt 6.12)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.6-qt6.12_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.39+`, Qt 6.12)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad-1.4.6-1.x86_64.rpm`**          | Linux Fedora / RHEL / openSUSE Canonical (`glibc 2.35+`, Qt 6.5 LTS) | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.2-1.x86_64.rpm`**    | Linux CentOS / RHEL Legacy (`glibc 2.31+`, Qt 6.2 LTS)               | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.5-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.5 LTS)                      | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.8-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.8 LTS)                      | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.8-1.aarch64.rpm`**   | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.8 LTS)                | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.10-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.10)                         | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.10-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.10)                   | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.11-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.11)                         | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.11-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.11)                   | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.12-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.39+`, Qt 6.12)                         | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.6-qt6.12-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.39+`, Qt 6.12)                   | aarch64       | RPM Package (`.rpm`)      |
| **`SparkNotePad-Default-Editor.sh`**           | macOS & Linux POSIX File Association Helper Script                   | Universal     | Bash Shell Script         |
| **`SparkNotePad-Default-Editor-Linux.sh`**     | Linux Dedicated 5-Layer Default Editor Engine Script                 | Linux         | Bash Shell Script         |

---

## 📜 Full Changelog

- <https://github.com/alsyundawy/SparkNotePad/compare/v1.4.5...v1.4.6>
