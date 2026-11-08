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

## ⚡ Executive Summary

SparkNotePad **v1.4.5** represents a major feature and stabilization milestone delivering 14 foundational architectural pillars designed for professional developers, system administrators, and power users:

1. **Closed Tab Stack & Reopen Closed Tab (`Ctrl+Shift+T` / `Cmd+Shift+T`)**:
   - `ClosedTabsManager` maintains a resilient 50-entry FIFO stack preserving caret position, line scroll ratio, encoding, EOL mode, language syntax styling, and tab color tags.
   - Context menu and File menu actions for instant tab restoration.
2. **Tab Color Tagging & Grouping**:
   - `TabColorManager` provides 8 high-contrast, accessible color presets (Red, Orange, Yellow, Green, Cyan, Blue, Purple, Magenta) with persistent state across app restarts via `ApplicationSettings`.
3. **Autosave & Crash Recovery Manager (10s Interval, Strict 0600 POSIX Permissions)**:
   - `AutosaveRecoveryManager` writes background snapshots every 10 seconds for modified documents, enforced with POSIX `0600` permissions and SHA-256 integrity checksums. Interactive `CrashRecoveryDialog` allows selective recovery or discard on application relaunch.
4. **Comprehensive Extended Find/Replace Escape Engine**:
   - `ExtendedSearchParser` parses `\xHH` (hexadecimal byte), `\dDDD` (decimal byte), `\0` (NUL byte), `\b` (backspace), `\f` (form-feed), and `\v` (vertical tab) across Find/Replace and Quick Find in Extended Search mode.
5. **100+ Lexers Audit & Modern Language Expansion (129 Total Languages)**:
   - Added new native lexers: Crystal (`.cr`), Cangjie (`.cj`), LLVM IR (`.ll`), and JSON5 (`.json5`).
   - Modernized standard keyword sets: C23 (`_BitInt`, `constexpr`, `nullptr`), Rust 2024 (`gen`, `async gen`), Swift 6 (`consuming`, `borrowing`, `sending`), and TypeScript 5.8.
6. **Native Markdown / HTML / PDF Export Pipeline**:
   - Implemented `DocumentExportManager` with native Qt 6 `QPrinter` / `Qt6::PrintSupport` and Pandoc fallback engine.
   - Interactive `ExportOptionsDialog` supporting custom page sizes (A4, Letter, Legal), orientation, margins, and embedded CSS styling.
7. **Markdown Preview Enhancements (Mermaid Diagrams & LaTeX Math Formulas)**:
   - `MarkdownPreviewEnhancer` renders Mermaid flowcharts, sequence diagrams, class diagrams, and state machines via client containers.
   - Full LaTeX math rendering support (`$$...$$` block and `$...$` inline math) with currency/escaped dollar preservation.
   - Two-Way SyncScroll matching Scintilla editor line ratios with Markdown preview viewport.
8. **Developer Tools Suite**:
   - `DeveloperToolsEngine` in Tools menu: JSON Prettify & Minify, URL Encode & Decode, Unicode Escape & Unescape, UUID v4 generator.
   - Interactive `TimestampConverterDialog` converting Unix Epoch timestamps (seconds/milliseconds) to human-readable UTC and Local ISO formats with copy helpers.
9. **Scrollbar Navigation Markers & Corresponding Header/Source Switcher (`F4` / `Alt+O`)**:
   - `CorrespondingFileSwitcher` supporting instant bidirectional toggle between C/C++ header (`.h`, `.hpp`, `.hxx`) and source (`.c`, `.cpp`, `.cxx`), and TypeScript/JavaScript source and test files (`.test.ts`, `.spec.ts`).
   - Added `ScrollbarNavigationMarkers` tracking bookmark lines and search match highlights on the scrollbar track.
10. **Native `.editorconfig` Automatic Parser & Enforcement**:
    - `EditorConfigParser` supporting `indent_style`, `indent_size`, `tab_width`, `end_of_line`, and `root = true` parent directory traversal with automatic enforcement.
11. **Clipboard History Dock & Character Info Dock**:
    - Dockable `ClipboardHistoryDock` tracking recent clipboard history (FIFO queue of 20 items), search filtering, and one-click editor insertion.
    - Dockable `CharacterInfoDock` displaying real-time Unicode glyph preview, code point `U+XXXX`, Unicode character name, category, UTF-8/16/32 hex, and HTML entities.
12. **Fuzzy Quick Switcher (`Cmd+P` / `Ctrl+P`) & Document Minimap Dock**:
    - Spotlight-style frameless `QuickSwitcherDialog` with subsequence fuzzy matching scoring open tabs and recent files.
    - `DocumentMinimapDock` providing a lightweight overview minimap with viewport tracking and a >50 MB large file safeguard.
    - Native macOS Dock recent documents integration via `MacServicesHandler::updateDockRecentMenu`.
13. **Aksara Toolbar Icon Pack Suite Integration (28 Packs Total) & Menu Ergonomics**:
    - Integrated 10 curated Aksara toolbar icon packs (340 vector SVGs and PNGs) across macOS Big Sur, Windows XP/Vista/10/11, Ubuntu Yaru, Arch Linux, Elementary OS, and modern styles (28 built-in packs total).
    - Relocated Themes menu: Toolbar Icon Size placed prominently above Toolbar Icon Packs, moved Customize Toolbar into the Tools menu, and removed Open Custom Icon Pack from the menubar.
    - Integrated into `toolbar_icons.qrc` and `ToolbarIconManager.cpp` with alias normalization and comprehensive unit test coverage.
14. **SonarLint & Clang-Tidy Static Analysis Eradication**:
    - Resolved all 25 SonarLint/SonarQube and clang-tidy static analysis warnings across `src/` (`misc-const-correctness`, `misc-override-with-different-visibility`, `bugprone-implicit-widening-of-multiplication-result`, `readability-use-concise-preprocessor-directives`, `readability-braces-around-statements`, `readability-else-after-return`, `performance-unnecessary-value-param`).
    - Verified 0 diagnostics and 0 errors across `clangd --check`, `cppcheck`, and unit tests.

---

## 📦 Downloads & Verification

- `SparkNotePad-v1.4.5-x64.dmg` (macOS Intel x86_64, 33 MB, verified with `hdiutil`)
- `SparkNotePad-v1.4.5-arm64.dmg` (macOS Apple Silicon ARM64)
- `sparknotepad_1.4.5_amd64.deb` (Debian / Ubuntu x86_64)
- `sparknotepad_1.4.5_arm64.deb` (Debian / Ubuntu ARM64)
- `sparknotepad-1.4.5-1.x86_64.rpm` (Fedora / RHEL x86_64)
- `sparknotepad-1.4.5-1.aarch64.rpm` (Fedora / RHEL ARM64)
