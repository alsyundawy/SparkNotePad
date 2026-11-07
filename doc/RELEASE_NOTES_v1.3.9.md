# SparkNotePad v1.3.9 — Official Scintilla 5.6.9 & Lexilla 5.5.5 Core Integration, Sandboxed Lua 5.5.1, Markdown Front-Matter Syntax Styling & 14-Pillar Hardening

**Release Version:** `v1.3.9`<br/>
**Release Date:** 2026-11-07<br/>
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
> 🍏 **[`Latest Releases (v1.3.9)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
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

SparkNotePad **v1.3.9** is a major foundational engine upgrade and stability milestone. It replaces and upgrades the entire vendored editing subsystem to **Scintilla 5.6.9** and **Lexilla 5.5.5** directly from official upstream releases (160 files updated, 3,206 insertions), integrates official **sandboxed Lua 5.5.1**, adds syntax highlighting support for Markdown front-matter metadata (YAML, TOML, JSON), ports critical regular-expression search/replace fixes from NotepadNext, introduces user-configurable default language preferences for new documents, prunes bloatware frameworks from release DMGs, and guarantees zero warnings across SonarLint, `clangd --check`, `cppcheck`, `clang-tidy`, and Trunk linters.

---

## 🌟 Highlights & Key Innovations in v1.3.9

### 1. Official Upstream Scintilla 5.6.9 & Lexilla 5.5.5 Core Integration

- **Full Upstream Source Code Replacement (100% Official)**:
  - Upstream `scintilla569.tgz` and `lexilla555.tgz` imported into `thirdparty/scintilla` and `thirdparty/lexilla` (`version.txt` set to 569 and 555 respectively).
  - Regenerated Qt wrapper interface (`ScintillaEdit.cpp` and `ScintillaEdit.h`) using Scintilla's official `WidgetGen.py` generation script.
- **Scintilla 5.6.9 Performance & Stability Features**:
  - Restored full-color rendering for XPM images.
  - Accelerated line-start position queries by skipping redundant layout calculations during rapid scrolling.
  - Increased cached lexeme capacity from 30 to 40 bytes for large identifiers, SQL columns, and symbols.
  - Updated character category maps and case conversions to standard **Unicode 17**.
  - Fixed a crash in change history undo caused by bulk deletion optimization in Scintilla 5.3.8.
- **Lexilla 5.5.5 Security Hardening & Memory Safety**:
  - Fixed out-of-bounds writes (CWE-787) in DMIS, Perl here-doc delimiters, Raku, TCMD, and TeX lexers.
  - Fixed out-of-bounds reads (CWE-125) in Ruby and X12 lexers.
  - Fixed preprocessor macro invocation crash in C++ lexer.
  - Fixed deep recursion crash in Visual Prolog lexer.

### 2. Sandboxed Lua 5.5.1 Engine Integration

- Fully upgraded the embedded Lua runtime to official **Lua 5.5.1** in `thirdparty/lua`.
- Strict sandboxing enforced: disabled unsafe system APIs (`os.execute`, `io.*`, `debug.*`, `package.loadlib`) to prevent arbitrary code execution via untrusted extensions or theme scripts.

### 3. Markdown Front-Matter Syntax Highlighting

Full support for front-matter metadata blocks in `src/languages/markdown.lua` utilizing new Lexilla 5.5.5 styling IDs:

- **`SCE_MARKDOWN_FRONT_MARK` (ID 22)**: Visual styling for metadata delimiters (`---` in YAML, `+++` in TOML, `{}` in JSON).
- **`SCE_MARKDOWN_FRONT` (ID 23)**: Visual styling for front-matter payload and property values.
- **`SCE_MARKDOWN_FRONT_KEY` (ID 24)**: Highlighting for metadata property keys and attributes.

### 4. Selective Feature & Stability Ports from NotepadNext

- **Fix Infinite Loop in Replace All (CWE-835 / DoS)**:
  - Source: NotepadNext commit `ef75daf94613a9931fdd7ecdeb18393f45248129`.
  - In `src/Finder.cpp`, fixed an application freeze when matching zero-length regular expressions (`^`, `$`, `\b`, lookarounds) by enforcing forward cursor progression using `editor->positionAfter()`.
- **Search & Replace Dialog Direction Invariant**:
  - Source: NotepadNext commit `adfd1280146b917560e1dd98c93e5bd3a31222f2`.
  - In `src/dialogs/FindReplaceDialog.cpp`, automatically unchecks and disables backwards direction searching when Regular Expression search mode is selected.
- **Default Language Setting for New Documents**:
  - Source: NotepadNext commit `be1e274cc3385900bf44c48cbba5c5c6a5299747`.
  - Added user-configurable `DefaultLanguage` in Preferences > Editor (`PreferencesDialog`), populated with all supported languages.
  - Automatically applies the preferred language when creating new untitled documents.

### 4. About Dialog Overhaul & Dynamic Metadata Architecture

- In `src/dialogs/AboutDialog.cpp`, dynamically parameterized the Release Tag badge and URL with `APP_VERSION`, ensuring that every release points cleanly to `v1.3.9` without stale hardcoded tags.
- Updated Scintilla 5.6.9 & Lexilla 5.5.5 labels and version identifiers across the About, Maintainer, and Diagnostics cards.
- Synchronized the system diagnostics clipboard export to match Scintilla 5.6.9 and Lexilla 5.5.5.
- Explicitly maintained upstream attribution to `NotepadNext-MacOS` (`@alsyundawy`) as the ancestral macOS port repository.

### 5. Full Qt 6.2 LTS Backward Compatibility (macOS Catalina & Ubuntu 20.04 LTS)

- Replaced `QTimeZone::UTC` with universal `QTimeZone::utc()` in `src/utils/TokenForensicUtils.cpp` (Qt 6.2+).
- Replaced `QLatin1StringView` with `QLatin1String` in `src/dialogs/MainWindow.cpp` and `src/widgets/TabsQuickActionsBar.cpp`.
- Removed redundant `inline` specifiers in `src/widgets/TabsQuickActionsBar.h`.
- Implemented `parseColor` in `src/features/ThemeManager.cpp` providing robust fallback for `QColor::fromString` under Qt < 6.4.
- Guarded Qt 6.4+ cryptographic APIs in `src/utils/HashIntegrityManager.cpp` with `QT_VERSION_CHECK(6, 4, 0)`.

### 6. Multi-Architecture CI Matrix & Automated Release Publishing

- Added native Linux ARM64 (`aarch64`) packaging for Ubuntu, Debian, and RPM distributions across Qt 6.8, 6.10, 6.11, and 6.12.
- Restored macOS Catalina (Qt 6.2 LTS) and Ubuntu 20.04 LTS (Qt 6.2 LTS) matrix runners.
- Enabled immediate publishing of release assets per job completion without waiting for whole matrix completion.
- Prioritized x64 build pipeline execution and pinned canonical default release artifact to Qt 6.5 LTS.

### 7. Bloatware Pruning & Lean Packaging

- Release macOS DMG packages are pruned of unused frameworks (`QtPdf`, `QtQml`, `QtQuick`, `QtVirtualKeyboard`), reducing installation footprint from ~95 MB down to ~31–33 MB.
- Embedded verified `.DS_Store` disk layout ensuring consistent branded presentation, custom background graphics, 128px retina icons, and drag-and-drop installer aesthetics across all environments.
- Eliminated unstyled `--skip-jenkins` fallback in CI to guarantee styled DMG layout.
- Verified local DMG generation: `build/SparkNotePad-v1.3.9-x64.dmg` (33 MB) valid and verified with `hdiutil`.

### 8. 100% Bilingual Localization Parity (i18n)

- Complete synchronization of Indonesian (`SparkNotePad_id.ts`) and English (`SparkNotePad_en.ts`) translation catalogs.
- Compiled via `lrelease`: exactly 1,850 translations finished, 0 unfinished.

### 9. 14-Pillar Code Quality & Verification Suite

SparkNotePad v1.3.9 adheres strictly to the 14 Pillars of Production Software Engineering:

- **Zero Linter Errors**: Passed `clangd --check`, `cppcheck` (152 files in `src/`), `clang-tidy`, SonarLint (`sonarsource.sonarlint-vscode`), and Trunk linters (`trunk check`).
- **Standard CMake Unix Makefiles**: Compilation strictly enforced with default generators and controlled `-j2` concurrency (Ninja permanently prohibited per `AGENTS.md`).
- **100% Test Pass Rate**: All 51 unit tests in `SparkNotePadTests` passing across 11 test suites.
- **Python Parity**: 100% parity across Python 3.14 and 3.15 environments (73 identical packages).

---

## 📦 Official Release Binaries & Assets

| Asset Name                                     | Target Platform / Description                                        | Architecture  | Format                    |
| :--------------------------------------------- | :------------------------------------------------------------------- | :------------ | :------------------------ |
| **`SparkNotePad-v1.3.9-x64.dmg`**              | macOS Canonical Release (macOS 11.0+, Qt 6.8 LTS)                    | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-arm64.dmg`**            | macOS Canonical Release (macOS 11.0+, Qt 6.8 LTS)                    | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-catalina-x64.dmg`**     | macOS Catalina Legacy Release (macOS 10.15+, Qt 6.2 LTS)             | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-qt6.12-x64.dmg`**       | macOS Cutting-Edge Release (macOS 13.0+, Qt 6.12)                    | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-qt6.12-arm64.dmg`**     | macOS Cutting-Edge Release (macOS 14.0+, Qt 6.12)                    | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-qt6.11-x64.dmg`**       | macOS Release (macOS 13.0+, Qt 6.11)                                 | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-qt6.11-arm64.dmg`**     | macOS Release (macOS 14.0+, Qt 6.11)                                 | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-qt6.10-x64.dmg`**       | macOS Release (macOS 13.0+, Qt 6.10)                                 | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-qt6.10-arm64.dmg`**     | macOS Release (macOS 14.0+, Qt 6.10)                                 | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-qt6.5-x64.dmg`**        | macOS Standard Release (macOS 11.0+, Qt 6.5 LTS)                     | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.3.9-qt6.5-arm64.dmg`**      | macOS Standard Release (macOS 11.0+, Qt 6.5 LTS)                     | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`sparknotepad_1.3.9_amd64.deb`**             | Linux Debian / Ubuntu Canonical (`glibc 2.35+`, Qt 6.8 LTS)          | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.3.9-arm64.deb`**             | Linux Debian / Ubuntu Canonical ARM64 (`glibc 2.35+`, Qt 6.8 LTS)    | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.3.9-ubuntu20.04_amd64.deb`** | Linux Ubuntu 20.04 LTS Legacy (`glibc 2.31+`, Qt 6.2 LTS)            | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.3.9-qt6.12_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.39+`, Qt 6.12)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.3.9-qt6.12_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.39+`, Qt 6.12)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad-1.3.9-1.x86_64.rpm`**          | Linux Fedora / RHEL / openSUSE Canonical (`glibc 2.35+`, Qt 6.8 LTS) | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.3.9-1.aarch64.rpm`**         | Linux Fedora / RHEL / openSUSE Canonical ARM64 (Qt 6.8 LTS)          | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.3.9-qt6.12-1.x86_64.rpm`**   | Linux Fedora / RHEL / openSUSE (`glibc 2.39+`, Qt 6.12)              | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.3.9-qt6.12-1.aarch64.rpm`**  | Linux Fedora / RHEL / openSUSE ARM64 (Qt 6.12)                       | aarch64       | RPM Package (`.rpm`)      |
| **`SparkNotePad-Default-Editor.sh`**           | macOS & Linux POSIX File Association Helper Script                   | Universal     | Bash Shell Script         |
| **`SparkNotePad-Default-Editor-Linux.sh`**     | Linux Dedicated 5-Layer Default Editor Engine Script                 | Linux         | Bash Shell Script         |

---

## 📜 Full Changelog

- <https://github.com/alsyundawy/SparkNotePad/compare/v1.3.8...v1.3.9>
