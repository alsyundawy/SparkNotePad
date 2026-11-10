# SparkNotePad v1.4.5 — Closed Tab Stack, Tab Color Tagging, Crash Recovery, Extended Search, PDF Export, Mermaid/LaTeX Preview, Aksara Icon Packs (28 Suites) & 14-Pillar Hardening

**Release Version:** `v1.4.5`<br/>
**Release Date:** 2026-11-08<br/>
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
> 🍏 **[`Latest Releases (v1.4.5)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
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

SparkNotePad **v1.4.5** represents a major architectural milestone delivering 17 foundational pillars engineered for professional software engineers, system architects, and power users. This release introduces a resilient closed-tab restoration stack (`Ctrl+Shift+T` / `Cmd+Shift+T`), accessible tab color tagging and grouping, an autosave and crash recovery engine with strict POSIX `0600` sandboxing, an extended regex escape search engine, 129 modern programming language lexers, native Markdown/HTML/PDF printing and export, 30 classless Markdown preview themes with Mermaid diagrams and KaTeX/LaTeX math, a developer utilities suite, corresponding header/source switching, automated `.editorconfig` enforcement, dockable clipboard and character forensics, fuzzy quick tab switching, 28 Aksara toolbar icon packs, permanent dark-mode toolbar styling, and a verified 30MB–34MB lightweight macOS DMG packaging standard.

---

## 🌟 Highlights & Key Innovations in v1.4.5

### 1. Closed Tab Stack & Reopen Closed Tab (`Ctrl+Shift+T` / `Cmd+Shift+T`)

- **Resilient Multi-Attribute State Restoration**:
  - Implemented `ClosedTabsManager` maintaining a 50-entry FIFO stack of recently closed documents.
  - Fully records and restores document path, caret offset, first visible line scroll ratio, character encoding, EOL mode, language syntax styling ID, and applied tab color tags.
  - Accessible via standard keybindings (`Cmd+Shift+T` on macOS, `Ctrl+Shift+T` on Linux/BSD), the File menu, and editor tab bar context menus.

### 2. Tab Color Tagging & Grouping (8 Accessible Presets)

- **Visual Organization for Complex Workspaces**:
  - Designed `TabColorManager` providing 8 high-contrast, WCAG-accessible color presets: Red, Orange, Yellow, Green, Cyan, Blue, Purple, and Magenta.
  - Tab colors persist seamlessly across application restarts and workspace session reloads via `ApplicationSettings`.
  - Right-click context menu integration on tab headers enables instant color assignment or clearing.

### 3. Autosave & Crash Recovery Manager (10s Interval, Strict POSIX 0600)

- **Zero-Data-Loss Architecture**:
  - Implemented `AutosaveRecoveryManager` executing non-blocking background snapshot passes every 10 seconds for unsaved or modified buffers.
  - Enforces strict POSIX `0600` (`S_IRUSR | S_IWUSR`) permissions on disk, safeguarding sensitive data from multi-user access on shared workstations.
  - Integrates SHA-256 integrity checksums to detect disk corruption.
  - Interactive `CrashRecoveryDialog` appears automatically on startup after an unexpected crash or system power outage, offering granular file-by-file recovery or discard options.

### 4. Comprehensive Extended Find/Replace Escape Engine

- **Full Parity with Advanced Regex & Hex Sequences**:
  - Engineered `ExtendedSearchParser` supporting hexadecimal bytes (`\xHH`), decimal byte sequences (`\dDDD`), NUL bytes (`\0`), backspaces (`\b`), form-feeds (`\f`), and vertical tabs (`\v`).
  - Available across Find & Replace (`FindReplaceDialog`), Replace All, and Quick Find Bar (`QuickFindWidget`) in Extended Search mode.

### 5. 100+ Lexers Audit & Modern Language Expansion (129 Total Languages)

- **Cutting-Edge Language & Systems Programming Support**:
  - Added new native lexers: Crystal (`.cr`), Cangjie (`.cj`), LLVM IR (`.ll`), and JSON5 (`.json5`).
  - Modernized keywords across standard languages: C23 (`_BitInt`, `constexpr`, `nullptr`), Rust 2024 edition (`gen`, `async gen`), Swift 6 (`consuming`, `borrowing`, `sending`), and TypeScript 5.8.
  - Re-verified all 129 language definition configurations in `src/languages/`.

### 6. Native Markdown / HTML / PDF Export Pipeline

- **High-Fidelity Document Publishing**:
  - Implemented `DocumentExportManager` utilizing native Qt 6 `QPrinter` / `Qt6::PrintSupport` alongside a headless Pandoc fallback.
  - Interactive `ExportOptionsDialog` allows users to configure paper size (A4, US Letter, Legal), page orientation, custom print margins, header/footer text, and embedded CSS styling.

### 7. Markdown Preview Enhancements (30 Themes, Mermaid & LaTeX Formulas)

- **Comprehensive Visual Document Preview**:
  - Integrated 30 classless CSS preview themes (`GITHUB`, `GITHUB-DARK`, `ALMOND`, `AWSM`, `AXIST`, `BAMBOO`, `BULLFRAME`, `HOLIDAY`, `KACIT`, `LATEX`, `MARX`, `MINI`, `MODEST`, `NEW`, `NO-CLASS`, `PICO`, `RETRO`, `SAKURA`, `SAKURA-VADER`, `SEMANTIC`, `SIMPLE`, `STYLE-SANS`, `STYLE-SERIF`, `STYLIZE`, `SUPERSTYLIN`, `TACIT`, `VANILLA`, `WATER`, `WATER-DARK`, `WRIT`) based on `cleanrmd` / `simov/markdown-viewer`.
  - Strict format detection: `\.(?:markdown|mdown|mkdn|md|mkd|mdwn|mdtxt|mdtext|text)(?:#.*|\?.*)?$` for Markdown and `\.(?:html|htm|shtml|xhtml|xht|dhtml)(?:#.*|\?.*)?$` for HTML.
  - Full `markdown-it` standard support: GFM tables, interactive task lists (`- [ ]`, `- [x]`), strikethrough (`~~text~~`), highlight (`==text==`), subscript (`~text~`), superscript (`^text^`), insert (`++text++`), footnotes (`[^1]`), protected code blocks, Mermaid diagrams, and LaTeX math.
  - Two-Way SyncScroll matching Scintilla editor line ratios with Markdown preview viewport.

### 8. Developer Tools Suite (Formatters, Minifiers, UUID, Timestamp Converter)

- **Built-In Utilities for Daily Engineering**:
  - Added `DeveloperToolsEngine` under Tools menu: JSON Prettify & Minify, URL Encode & Decode, Unicode Escape & Unescape, and UUID v4 generator.
  - Interactive `TimestampConverterDialog` converting Unix Epoch timestamps (seconds and milliseconds) to human-readable UTC and Local ISO formats with copy helpers.

### 9. Scrollbar Navigation Markers & Corresponding Header/Source Switcher (`F4` / `Alt+O`)

- **Instant Source File Traversal**:
  - Implemented `CorrespondingFileSwitcher` supporting instant bidirectional switching between C/C++ header (`.h`, `.hpp`, `.hxx`) and source (`.c`, `.cpp`, `.cxx`), and TypeScript/JavaScript source and test files (`.test.ts`, `.spec.ts`).
  - Added `ScrollbarNavigationMarkers` rendering visual markers on the scrollbar track for bookmarks and search highlights.

### 10. Native `.editorconfig` Automatic Parser & Enforcement

- **Automatic Team Coding Standard Enforcement**:
  - Implemented `EditorConfigParser` supporting `indent_style` (space/tab), `indent_size`, `tab_width`, `end_of_line` (lf/crlf/cr), and `root = true` parent directory traversal.
  - Automatically configures editor tab settings upon opening any project file.

### 11. Clipboard History Dock & Character Info Dock

- **Deep Text Forensics & Clipboard Tracking**:
  - Dockable `ClipboardHistoryDock` tracking recent clipboard history (FIFO queue of 20 items), search filtering, and one-click editor insertion.
  - Dockable `CharacterInfoDock` displaying real-time Unicode glyph preview, code point `U+XXXX`, Unicode character name, category, UTF-8/16/32 hex, and HTML entities.

### 12. Fuzzy Quick Switcher (`Cmd+P` / `Ctrl+P`) & Document Minimap Dock

- **Spotlight-Style Navigation & Document Overview**:
  - Spotlight-style frameless `QuickSwitcherDialog` with subsequence fuzzy matching scoring open tabs and recent files.
  - `DocumentMinimapDock` providing a lightweight overview minimap with viewport tracking and a >50 MB large file safeguard.
  - Native macOS Dock recent documents integration via `MacServicesHandler::updateDockRecentMenu`.

### 13. Aksara Toolbar Icon Pack Suite Integration (28 Packs Total) & Menu Ergonomics

- **Multi-Resolution Visual Styling**:
  - Integrated 10 curated Aksara toolbar icon packs (340 vector SVGs and PNGs) across macOS Big Sur, Windows XP/Vista/10/11, Ubuntu Yaru, Arch Linux, Elementary OS, and modern styles (28 built-in packs total).
  - Relocated Themes menu: Toolbar Icon Size placed prominently above Toolbar Icon Packs, moved Customize Toolbar into the Tools menu, and removed Open Custom Icon Pack from the menubar.
  - Registered in `toolbar_icons.qrc` and `ToolbarIconManager.cpp` with alias normalization and comprehensive unit test coverage.

### 14. SonarLint & Clang-Tidy Static Analysis Eradication (Zero Issues)

- **Comprehensive Code Quality Overhaul**:
  - Resolved all 25 SonarLint/SonarQube and clang-tidy static analysis warnings across `src/` (`misc-const-correctness`, `misc-override-with-different-visibility`, `bugprone-implicit-widening-of-multiplication-result`, `readability-use-concise-preprocessor-directives`, `readability-braces-around-statements`, `readability-else-after-return`, `performance-unnecessary-value-param`).
  - Verified 0 diagnostics and 0 errors across `clangd --check`, `cppcheck`, and unit tests.

### 15. Permanent Dark Mode Toolbar Invariant Across All Modes and Themes

- **Flawless Visual Contrast & Icon Presentation**:
  - The main toolbar (`#mainToolBar`, `QToolBar`) is permanently locked to a dark background (`#21252b`) across all color themes, light/dark modes, and operating system appearance settings.
  - Prevents pastel and chromatic icons from rendering on light backgrounds, eliminating invalid Qt Style Sheet tokens (`:not()` and `!important`) to ensure 100% clean QSS parsing without Cocoa native bleed-through.

### 16. Canonical IANA Media Types Default Editor Integration (100+ Formats)

- **Seamless Desktop Operating System Integration**:
  - Expanded `SparkNotePad-Default-Editor.sh` and Linux default handler scripts to adhere to the official IANA Media Types registry.
  - Seamlessly associates 100+ file extensions across source code, configuration languages, systems programming, and web documents with SparkNotePad.

### 17. Permanent Lightweight DMG Packaging Standard (30MB–34MB, < 35MB Ceiling)

- **Bloatware-Free macOS Packaging Standard**:
  - Streamlined macOS release DMG bundling via `scripts/package-dmg.sh`, strictly enforcing a 30MB–34MB footprint (under the 35MB ceiling).
  - Preserves custom `.DS_Store` presentation (`dmg-ds-store`), Retina `@2x` background graphics (`.background/dmg-background@2x.png`), and custom volume icons (`.VolumeIcon.icns`).

---

## 📦 Official Release Binaries & Assets

| Asset Name                                     | Target Platform / Description                                        | Architecture  | Format                    |
| :--------------------------------------------- | :------------------------------------------------------------------- | :------------ | :------------------------ |
| **`SparkNotePad-v1.4.5-x64.dmg`**              | macOS Canonical Release (macOS 11.0+, Qt 6.5 LTS)                    | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-arm64.dmg`**            | macOS Canonical Release (macOS 11.0+, Qt 6.5 LTS)                    | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.2-x64.dmg`**        | macOS Catalina Legacy Release (macOS 10.15+, Qt 6.2 LTS)             | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.2-arm64.dmg`**      | macOS Legacy Release (macOS 11.0+, Qt 6.2 LTS)                       | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.5-x64.dmg`**        | macOS Standard Release (macOS 11.0+, Qt 6.5 LTS)                     | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.5-arm64.dmg`**      | macOS Standard Release (macOS 11.0+, Qt 6.5 LTS)                     | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.8-x64.dmg`**        | macOS Release (macOS 12.0+, Qt 6.8 LTS)                              | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.8-arm64.dmg`**      | macOS Release (macOS 12.0+, Qt 6.8 LTS)                              | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.10-x64.dmg`**       | macOS Release (macOS 13.0+, Qt 6.10)                                 | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.10-arm64.dmg`**     | macOS Release (macOS 14.0+, Qt 6.10)                                 | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.11-x64.dmg`**       | macOS Release (macOS 13.0+, Qt 6.11)                                 | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.11-arm64.dmg`**     | macOS Release (macOS 14.0+, Qt 6.11)                                 | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.12-x64.dmg`**       | macOS Cutting-Edge Release (macOS 13.0+, Qt 6.12)                    | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.12-arm64.dmg`**     | macOS Cutting-Edge Release (macOS 14.0+, Qt 6.12)                    | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`sparknotepad_1.4.5_amd64.deb`**             | Linux Debian / Ubuntu Canonical (`glibc 2.35+`, Qt 6.5 LTS)          | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.2_amd64.deb`**       | Linux Ubuntu 20.04 LTS Legacy (`glibc 2.31+`, Qt 6.2 LTS)            | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.5_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.5 LTS)                    | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.8_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.8 LTS)                    | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.8_arm64.deb`**       | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.8 LTS)              | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.10_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.10)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.10_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.10)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.11_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.11)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.11_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.11)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.12_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.39+`, Qt 6.12)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.5-qt6.12_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.39+`, Qt 6.12)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad-1.4.5-1.x86_64.rpm`**          | Linux Fedora / RHEL / openSUSE Canonical (`glibc 2.35+`, Qt 6.5 LTS) | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.2-1.x86_64.rpm`**    | Linux CentOS / RHEL Legacy (`glibc 2.31+`, Qt 6.2 LTS)               | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.5-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.5 LTS)                      | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.8-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.8 LTS)                      | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.8-1.aarch64.rpm`**   | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.8 LTS)                | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.10-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.10)                         | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.10-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.10)                   | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.11-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.11)                         | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.11-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.11)                   | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.12-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.39+`, Qt 6.12)                         | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.5-qt6.12-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.39+`, Qt 6.12)                   | aarch64       | RPM Package (`.rpm`)      |
| **`SparkNotePad-Default-Editor.sh`**           | macOS & Linux POSIX File Association Helper Script                   | Universal     | Bash Shell Script         |
| **`SparkNotePad-Default-Editor-Linux.sh`**     | Linux Dedicated 5-Layer Default Editor Engine Script                 | Linux         | Bash Shell Script         |

---

## 📜 Full Changelog

- <https://github.com/alsyundawy/SparkNotePad/compare/v1.4.0...v1.4.5>
