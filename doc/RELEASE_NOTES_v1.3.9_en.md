# SparkNotePad Release Notes — v1.3.9

**Release Version:** `v1.3.9`<br/>
**Release Date:** November 07, 2026<br/>
**Lead Maintainer:** Harry Dertin Sutisna Alsyundawy ([@alsyundawy](https://github.com/alsyundawy))<br/>
**Organization:** ALSYUNDAWY IT SOLUTION ([https://alsyundawy.com](https://alsyundawy.com))<br/>
**Repository:** [https://github.com/alsyundawy/SparkNotePad](https://github.com/alsyundawy/SparkNotePad)<br/>
**License:** GNU General Public License v3.0 (`GPL-3.0-or-later`)

---

## 🌟 What's New in v1.3.9

SparkNotePad v1.3.9 is a foundational engine and stability release that brings official upstream upgrades to **Scintilla 5.6.9** and **Lexilla 5.5.5**, addresses critical search-and-replace regular expression edge cases, introduces user-configurable default languages for new documents, enriches Markdown front-matter syntax styling, and passes rigorous 14-pillar static analysis and code review.

---

### 1. Official Scintilla 5.6.9 & Lexilla 5.5.5 Core Integration

- **Scintilla 5.6.9 Upstream Core**:
  - Restored full-color rendering for XPM images.
  - Accelerated line-start position queries by skipping redundant line layout calculations.
  - Increased cached lexeme capacity from 30 to 40 bytes for large identifier, column name, and SQL/JSON symbol handling.
  - Updated character category maps and case conversions to standard **Unicode 17**.
  - Fixed a crash in change history undo caused by bulk deletion optimization in Scintilla 5.3.8.
- **Lexilla 5.5.5 Upstream Lexers**:
  - Enhanced memory safety across multiple lexers: fixed out-of-bounds writes (CWE-787) in DMIS, Perl here-doc delimiters, Raku, TCMD, and TeX.
  - Fixed out-of-bounds reads (CWE-125) in Ruby and X12 lexers.
  - Fixed preprocessor macro invocation crash in C++ lexer.
  - Fixed deep recursion crash in Visual Prolog lexer.
- **Regenerated Qt Interface**:
  - Regenerated `ScintillaEdit.cpp` and `ScintillaEdit.h` via official `WidgetGen.py`.

---

### 2. Markdown Front-Matter Syntax Highlighting

In `src/languages/markdown.lua`, three new Lexilla 5.5.5 style IDs have been fully integrated:

- **`SCE_MARKDOWN_FRONT_MARK` (ID 22)**: Visual styling for metadata delimiters (`---` in YAML, `+++` in TOML, `{}` in JSON).
- **`SCE_MARKDOWN_FRONT` (ID 23)**: Visual styling for front-matter payload and property values.
- **`SCE_MARKDOWN_FRONT_KEY` (ID 24)**: Bold highlighting for metadata property keys and attributes.

---

### 3. Selective Feature & Stability Ports from NotepadNext

- **Infinite-Loop Fix in Replace All (CWE-835 / DoS)**:
  - Source: NotepadNext commit `ef75daf94613a9931fdd7ecdeb18393f45248129`.
  - In `src/Finder.cpp`, fixed an application hang when matching zero-length regular expressions (`^`, `$`, `\b`, lookarounds).
  - Enforced forward cursor progression using `editor->positionAfter()`.
- **Search & Replace Dialog Direction Invariant**:
  - Source: NotepadNext commit `adfd1280146b917560e1dd98c93e5bd3a31222f2`.
  - In `src/dialogs/FindReplaceDialog.cpp`, automatically unchecks and disables backwards direction searching when Regular Expression search mode is selected.
- **Default Language Setting for New Documents**:
  - Source: NotepadNext commit `be1e274cc3385900bf44c48cbba5c5c6a5299747`.
  - Added user-configurable `DefaultLanguage` in Preferences > Editor (`PreferencesDialog`), populated with all supported languages.
  - Automatically applies the preferred language when creating new untitled documents.

---

### 4. About Dialog Overhaul & Dynamic Metadata Architecture

- **Dynamic Version Parameterization**: In `src/dialogs/AboutDialog.cpp`, the Release Tag badge and URL are parameterized dynamically with `APP_VERSION`, guaranteeing that every release links directly to its own tag (`v1.3.9`) without stale hardcoded values.
- **Engine Versioning Synchronization**: Updated Scintilla 5.6.9 and Lexilla 5.5.5 labels across the About tab, Maintainer tab, and Diagnostics card.
- **System Diagnostics Copy**: Synchronized the plain-text system info clipboard export to accurately reflect the upgraded editing subsystem.
- **Ancestral Port Attribution**: Explicitly attributed `NotepadNext-MacOS` (`@alsyundawy`) as the ancestral macOS port repository from which SparkNotePad branched and evolved.

---

### 5. Full Qt 6.2 LTS Backward Compatibility (macOS Catalina & Ubuntu 20.04 LTS)

- **UTC Time Zone API**: Replaced `QTimeZone::UTC` with universal `QTimeZone::utc()` in `src/utils/TokenForensicUtils.cpp`, restoring full compatibility across Qt 6.2+.
- **String View Compatibility**: Replaced `QLatin1StringView` with `QLatin1String` in `src/dialogs/MainWindow.cpp` and `src/widgets/TabsQuickActionsBar.cpp`.
- **Compiler Warning Elimination**: Removed redundant `inline` specifiers in `src/widgets/TabsQuickActionsBar.h`.
- **Color Parsing Fallback**: Implemented `parseColor` in `src/features/ThemeManager.cpp` providing a robust fallback for `QColor::fromString` when compiled under Qt < 6.4.
- **Cryptographic Guards**: Guarded Qt 6.4+ cryptographic APIs in `src/utils/HashIntegrityManager.cpp` using `QT_VERSION_CHECK(6, 4, 0)`.

---

### 6. Multi-Architecture CI Matrix & Automated Release Publishing

- **Linux ARM64 Packaging**: Added native Linux ARM64 (`aarch64`) packaging for Ubuntu, Debian, and RPM distributions across Qt 6.8, 6.10, 6.11, and 6.12.
- **Restored Legacy Runners**: Re-enabled and verified Qt 6.2 LTS compilation on macOS Catalina and Ubuntu 20.04 runners.
- **Accelerated Publishing**: Prioritized x64 jobs and enabled immediate asset release publishing upon individual job completion.
- **Canonical Default Target**: Pinned canonical default release artifact to Qt 6.5 LTS.
- **Distribution Scope**: Expanded binary distribution catalog to 50+ packages.

---

### 7. macOS DMG Packaging & Aesthetic Polish

- **Official Styled Layout**: Eliminated unstyled `--skip-jenkins` fallback in CI to guarantee styled DMG layout (`dmg-background.png`, custom volume icon, Applications alias, 680x440 geometry).
- **Framework Bloatware Pruning**: Pruned unused Qt frameworks (`QtPdf`, `QtQml`, `QtQuick`, `QtVirtualKeyboard`) saving ~15 MB.
- **Local DMG Verification**: Verified local DMG generation: `build/SparkNotePad-v1.3.9-x64.dmg` (33 MB) is valid and checksum-verified via `hdiutil`.

---

### 8. 100% Bilingual Localization Parity (i18n)

- Complete synchronization of Indonesian (`SparkNotePad_id.ts`) and English (`SparkNotePad_en.ts`) translation catalogs.
- Compiled via `lrelease`: exactly 1,850 translations finished, 0 unfinished.

---

### 9. 14-Pillar Code Quality & Verification Suite

SparkNotePad v1.3.9 adheres strictly to the 14 Pillars of Production Software Engineering:

- Verified clean with 0 warnings on `clangd --check`, `cppcheck` (152 files in `src/`), `clang-tidy`, and SonarLint.
- Built strictly with default CMake `Unix Makefiles` (Ninja permanently prohibited per `AGENTS.md`).
- 100% test pass rate across all unit tests in `SparkNotePadTests` (51/51 passed).
- 100% module parity between Python 3.14 and 3.15 environments (73 identical packages).
