# SparkNotePad 1.4.6 — Modern Iconography, Context Menu Ergonomics, Syntax Highlighting & Packaging Standard

> **For agentic workers:** REQUIRED SUB-SKILL: Use `superpowers:subagent-driven-development` (recommended) or `superpowers:executing-plans` to implement this plan task-by-task. Steps use checkbox (`- [x]`) syntax for tracking.

- **Date:** 2026-11-10
- **Target Version:** SparkNotePad v1.4.6
- **Base Version:** SparkNotePad v1.4.5 (released 2026-11-08)
- **Editor Engine:** Scintilla 5.6.9 + Lexilla 5.5.5 + Lua 5.5.1 + Qt 6.5–6.12 (ISO C++20)
- **Lead Maintainer:** HARRY DERTIN SUTISNA ALSYUNDAWY (`@alsyundawy`)
- **Repository:** [https://github.com/alsyundawy/SparkNotePad](https://github.com/alsyundawy/SparkNotePad)

---

## 1. Executive Summary & Architecture Overview

SparkNotePad v1.4.6 elevates user experience, visual clarity, and desktop integration through five major pillars:

1. **30 New Modern Toolbar Icon Packs (69 Built-In Suites Total):** High-fidelity multi-resolution icons (16x16, 20x20, 24x24, 32x32, 48x48, 64x64) supporting high-DPI Retina displays.
2. **Editor Context Menu Ergonomics:** Added **Paste & Match Style** (`pasteAndMatchStyle`) to strip external formatting and match indentation, **Search on Google** (`searchOnGoogle`) for rapid web lookup, and streamlined direct **UPPERCASE** / **lowercase** conversion actions.
3. **General Preferences Additions:** Added toggle settings for Character Encoding Auto-detection (`enableEncodingAutoDetection`) and Save All Confirmation dialog (`enableSaveAllConfirmation`).
4. **WCAG 2.1 Contrast Safety & Syntax Highlighting Overhaul:** Mathematical relative luminance calculation guaranteeing at least 4.0:1 text contrast across all 70+ themes, High-visibility Gold Badges (`#FFD700`) for TODO/FIXME markers, and theme-synchronized indicator box.
5. **Canonical 30MB–40MB Lightweight macOS DMG Standard:** Calibrated release DMG packaging strictly between 30MB and 40MB (38MB–39MB) preserving Qt frameworks and image plugins.

---

## 2. Completed Implementation Tasks

- [x] **Task 1: Toolbar Icon Packs Expansion (30 New Suites / 69 Total)**
  - [x] Register 30 modern icon packs in `ToolbarIconManager.cpp` and `ToolbarIconManager.h`.
  - [x] Integrate multi-DPI pixmaps (16x16 to 64x64) for all 30 packs in `src/icons/packs/`.
  - [x] Update Qt resource definitions in `src/toolbar_icons.qrc`.
  - [x] Implement robust alias fallback normalization in `ToolbarIconManager::normalizePackId()`.

- [x] **Task 2: Editor Context Menu Ergonomics**
  - [x] Implement `MainWindow::onPasteAndMatchStyleTriggered()` with indentation matching.
  - [x] Implement `MainWindow::onSearchGoogleTriggered()` with URL-encoded query launcher.
  - [x] Streamline case conversion to direct UPPERCASE and lowercase actions in editor context menu.
  - [x] Purge obsolete legacy Run and Base64 Decode/Encode actions from editor right-click menu.
  - [x] Bind keyboard shortcuts: `Cmd+Alt+Shift+V` / `Ctrl+Shift+V` and `Cmd+Alt+G` / `Ctrl+Alt+G`.

- [x] **Task 3: General Preferences Additions**
  - [x] Add UI checkboxes in `PreferencesDialog.ui` under General tab.
  - [x] Bind settings in `PreferencesDialog.cpp` to `ApplicationSettings`.
  - [x] Connect `enableEncodingAutoDetection` and `enableSaveAllConfirmation` to application flow.

- [x] **Task 4: WCAG 2.1 Syntax Highlighting & Contrast Safety**
  - [x] Implement relative luminance calculation `0.2126 * R + 0.7152 * G + 0.0722 * B` in `ThemeManager.cpp`.
  - [x] Guarantee 4.0:1 primary text contrast across all 70+ light and dark themes.
  - [x] Add High-visibility Gold Badges (`#FFD700`) for TODO, FIXME, BUG, and NOTE markers.
  - [x] Ensure theme-synchronized indicator box styles for search matches.

- [x] **Task 5: Lightweight DMG Packaging Standardization**
  - [x] Update `scripts/package-dmg.sh` to preserve Qt frameworks and image plugins.
  - [x] Enforce size range strictly between 30MB and 40MB ceiling (38MB–39MB).
  - [x] Preserve `.background/` assets and pre-baked `.DS_Store` layout.
  - [x] Automated mounting and checksum verification with `hdiutil verify`.

- [x] **Task 6: Localization & Documentation Parity**
  - [x] Extract and update translation catalogs `i18n/SparkNotePad_en.ts` and `SparkNotePad_id.ts`.
  - [x] Recompile binary catalogs (`.qm`) via `lrelease`.
  - [x] Regenerate master and localized docnotes and changelogs via `scripts/build_localized_docs.py`.
  - [x] Update Wiki guides (`02-Feature-Overview`, `02-Ikhtisar-Fitur`, `03-User-Interface-and-Themes`, `03-Antarmuka-dan-Tema`, `06-Keyboard-Shortcuts`, `06-Daftar-Pintasan-Keyboard`).
  - [x] Synchronize `README.md`, `README-id.md`, `doc/FEATURE_INVENTORY.md`, and `doc/USER_GUIDE.md`.

---

## 3. Verification & Quality Gates

- [x] `cppcheck --enable=all` passes with 0 errors.
- [x] `clangd --check` passes across all sources with 0 errors.
- [x] `trunk check` passes with 0 blocking errors.
- [x] `lint-check.sh` passes across all touched Markdown, XML, and translation files.
- [x] `ctest --test-dir build --output-on-failure` passes 100% test suites.
- [x] Local DMG package verification strictly between 30MB and 40MB ceiling.
