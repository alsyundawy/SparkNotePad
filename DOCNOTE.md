# SparkNotePad Release DocNote — v1.3.9

```text
================================================================================
Release Version     : 1.3.9 (Scintilla 5.6.9, Lexilla 5.5.5, Default Language & 14-Pillar Hardening)
Release Date        : 2026-11-07
Lead Maintainer     : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organization        : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repository          : https://github.com/alsyundawy/SparkNotePad
Core Editing Engine : Scintilla 5.6.9 (Vendored) & Lexilla 5.5.5 (Vendored)
Embedded Lua Core   : Sandboxed Lua 5.5.1 (Official Source)
Scripting Runtime   : Python 3.13+ Engine Bridge
GUI Framework       : Qt 6.5 / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platforms    : macOS 11.0–26.0 (Apple Silicon & Intel), Linux (Ubuntu, Debian, Fedora, RHEL, openSUSE), FreeBSD 13–15
License             : GNU General Public License v3.0 (GPL-3.0-or-later)
Architecture        : 100% Additive Safe Enhancements Inspired by KDE Kate, Pluma MATE, Notepad-- & Notepad++
CI/CD Status        : 100% Passing across all release matrices, MegaLinter & CodeQL
================================================================================
```

---

## 1. Executive Summary: Release v1.3.9

SparkNotePad **v1.3.9** brings official upstream updates for the core Scintilla and Lexilla editing engines, addresses security and algorithmic issues in search and replace, enriches syntax highlighting for Markdown front-matter, adds user-configurable default language preferences for new documents, and passes complete 14-pillar code review:

1. **Official Upstream Scintilla 5.6.9 Integration**:
   - Restores full-color rendering for XPM images.
   - Skips unnecessary line layout calculations during line-start position queries for accelerated scrolling and navigation.
   - Increases cached lexeme buffer capacity from 30 to 40 bytes to optimize large symbols and query identifiers.
   - Full Unicode 17 standard character category tables and case conversion support.
   - Resolves change history undo crashes during bulk deletion operations.

2. **Official Upstream Lexilla 5.5.5 Integration & Markdown Front-Matter**:
   - Introduces 3 new Lexilla 5.5.5 style IDs in `src/languages/markdown.lua` for YAML, TOML, and JSON front-matter:
     - `SCE_MARKDOWN_FRONT_MARK` (ID 22): Delimiters (`---`, `+++`, `{}`).
     - `SCE_MARKDOWN_FRONT` (ID 23): Front-matter body and value payload.
     - `SCE_MARKDOWN_FRONT_KEY` (ID 24): Front-matter attribute keys.
   - Mitigates memory safety issues including out-of-bounds writes (CWE-787) and reads (CWE-125) in Lexilla lexers (DMIS, Perl, Raku, Ruby, TCMD, TeX, Visual Prolog, X12).
   - Enhances HTML/JavaScript, PHP, and SQL lexer syntax handling.

3. **Selective NotepadNext Feature & Bug Fix Ports**:
   - **Fix Infinite Loop in Replace All (CWE-835 / DoS)**: Resolved an event loop hang in `Finder::replaceAll` when matching zero-length regular expressions (`^`, `$`, `\b`, lookarounds) by enforcing position advancement via `editor->positionAfter()`.
   - **Search Dialog Direction Invariant**: Automatically unchecks and disables backwards direction search in `FindReplaceDialog.cpp` when Regular Expression mode is selected.
   - **Default Language Setting**: Added a user-configurable `DefaultLanguage` setting in Preferences > Editor for untitled/new documents, with dynamic language selection.

4. **14-Pillar Code Quality & Zero-Defect Linter Verification**:
   - Verified 0 diagnostics and 0 errors across `clangd --check`, `cppcheck`, `clang-tidy`, and SonarLint.
   - Strictly enforced standard `Unix Makefiles` compilation with controlled `-j2` concurrency.

---

## 2. Historical Summary: Release v1.3.8

SparkNotePad **v1.3.8** delivers complete toolbar customization, an expansive 21-pack icon theme architecture, dedicated Preferences tabs, desktop menu reorganization, and 100% finished bilingual localization parity:

1. **21 Built-in Toolbar Icon Packs (Multi-DPI & Retina @2x)**:
   - Complete icon pack ecosystem: Modern Ribbon, Microsoft Fluent UI (Win 11), Windows 11 (Fluent 2), Windows 7 (Aero Glass), Windows XP (Luna Classic), Fugue Classic (Silk Suite), Notepad++ Classic, NotepadNext Classic, Monochrome Outline, Vibrant DuoTone, Material Design Sharp, Pixel Design, Monoline Minimal, Notepad++ Green, Notepad3 Warm, Humanity GNOME, Elementary OS Flat, Breeze KDE, Papirus Rounded, macOS Cupertino, and VS Code Codicons.
   - Dynamic icon scaling across 16x16, 20x20, 24x24, 32x32, 48x48, and 64x64.
   - Intelligent Dark Mode HSL luminance tone mapping preventing icon washing-out on dark canvases.

2. **Dedicated Preferences Tabs ("Themes & Toolbar" & "Custom Toolbar Config")**:
   - **Tab 3 ("Themes & Toolbar")**: Relocated Themes styling (Active Theme, Window Opacity, Tail Interval, Display Safety) into a unified tab with Toolbar Icon Packs manager (21 packs, responsive description card layout, size selector, icon previews, custom pack folder launcher, and rescan).
   - **Tab 4 ("Custom Toolbar Config")**: Embedded visual toolbar customizer widget (`CustomizeToolbarWidget`) directly inside Preferences with live search, action adding/removing/reordering, separator insertion, and hot-reload.

3. **Standard Desktop Editor Menu Hierarchy**:
   - Reversible Hex/Binary View relocated directly below Compare in the top-level Tools/View menu (`Ctrl+Alt+H` / `Cmd+Alt+H`).
   - Restart Application relocated to macOS Application menu directly above Quit (`QAction::ApplicationSpecificRole`).
   - Duplicate manual Character Palette removed in favor of native macOS system palette.
   - View menu streamlined without duplicate Side Panels submenu.

4. **100% Bilingual Localization Parity across English & Indonesian**:
   - Complete synchronization of 1,924 UI strings with 0 unfinished entries across `SparkNotePad_en.ts` and `SparkNotePad_id.ts`.

5. **In-Tab Markdown & HTML Live Preview**:
   - Central dock area (`ads::CenterDockWidgetArea`) live preview tabs with GitHub callout styling (`[!NOTE]`, `[!TIP]`, `[!WARNING]`, etc.), zoom controls, reading time metrics badges, and "Preview in Default Browser" for HTML.

6. **14-Pillar Code Quality & Zero-Defect Linter Hardening**:
   - Comprehensive audit across Bug, Syntax, Runtime, Logic, Memory, Dead Code, Duplicate Code, Circular Dependency, Performance Bottleneck, Security Vulnerability, Maintainability, Scalability, Readability, and IDE Linter pillars.
   - 100% test pass rate across all 51 unit tests in `SparkNotePadTests`.

---

## 2. Prior Release: Release v1.3.7

SparkNotePad **v1.3.7** delivers a comprehensive suite of high-velocity developer ergonomics inspired by primary-source research into **KDE Kate**, **Pluma (MATE)**, **Notepad-- (cxasm/ndd)**, and **Notepad++** without breaking or regressing any existing functionality:

0. **Zero-Diagnostic Trunk, Clang-Tidy, SonarQube & Cppcheck Certification**:
   - Established official repository `.clang-tidy` configuration with tailored Qt checks.
   - Wrapped `main()` entrypoint in standard ISO C++ `try-catch` handlers resolving `bugprone-exception-escape`.
   - Applied `std::move` on `ApplicationSetting` constructors eliminating unnecessary copies.
   - Removed redundant `inline` keywords on `ApplicationSetting` getters and refactored `else-after-return` in `main.cpp`.
   - Re-synchronized Qt UIC autogenerated header `build/src/SparkNotePad_autogen/include/ui_MainWindow.h` from `src/dialogs/MainWindow.ui`, ensuring `actionRestart` and `actionTrimTrailingSpace` members resolve cleanly in AST.
   - Resolved all remaining SonarLint, SonarQube, Trunk, Clangd, and Cppcheck diagnostics across all repository modules:
     - `DefaultDirectoryManager`: Adopted C++20 `using enum ApplicationSettings::DefaultDirectoryBehaviorEnum;`, converted branch condition to C++17 init-statement (`if (const auto e = settings->defaultDirectoryBehavior(); ...)`), and updated `editorActivated` to accept `const ScintillaNext *editor` in header and implementation.
     - `ThemeSwitcherDialog`: Replaced all raw `new` allocations for layouts (`filterLayout`, `btnLayout`) and `QTableWidgetItem` objects (`item0`, `item1`, `item2`, `item3`) with `std::make_unique` and `.release()`, wrapped single-statement branches in explicit braces, added explicit operator precedence parentheses `(c * 26) + 2`, and replaced `Q_UNUSED(column)` with unnamed comment `/*column*/`.
     - `FileTimeUtils`: Replaced legacy deprecated `Q_OS_MAC` with modern Qt standard `Q_OS_MACOS`, replaced C-style arrays `timespec[2]` and `timeval[2]` with standard `std::array<struct timespec, 2>` and `std::array<struct timeval, 2>`, applied C++17 if-init statements to `QFile file` and `QFileDevice::FileError err`, and updated comment headers to eliminate SonarLint commented-code false positives.
     - `MacroEditorDialog`: Migrated `ui` pointer to `std::unique_ptr<Ui::MacroEditorDialog>` with defaulted destructor, eliminated manual `delete ui;`, adopted C++20 `std::ranges::transform` for recordable messages, replaced deprecated `qAsConst` with `std::as_const`, converted `currentIndex` in `insertMacroStep` to C++17 if-init statement, marked `deleteMacroStep`, `moveMacroStepUp`, and `moveMacroStepDown` const, and removed obsolete `TODO` comment.
     - `SessionManager`: Declared `saveSession(const MainWindow *window) const` as const member accepting `const MainWindow *`, updated `loadSession(const MainWindow *window)` parameter to `const MainWindow *`, declared `currentEditor` and loop `editor` as pointer-to-const, and refactored `Type` check in `loadSession()` to C++17 if-init statement.
     - `MainWindow`: Changed `newEditor` in horizontal and vertical split actions to `const ScintillaNext *`, marked `switchToEditor`, `onNextDocumentTriggered`, and `onPreviousDocumentTriggered` const, converted marker decorator lookups to `const auto *decorator`, and replaced `if (!pEditor)` with `if (pEditor.isNull())` in `closeFile()` eliminating cppcheck `knownConditionTrueFalse` false positive.
     - `EditorConfigAppDecorator`: Added `Q_DISABLE_COPY_MOVE(PreventUnfolding)`, member initializer list for `foldFlags`, early return reducing cognitive complexity from 46 to < 20, structured bindings `[key, val]`, explicit casts for `sptr_t`, `default: break;` in switch, converted `indentStyle`, `indentSize`, `tabWidth`, `eol`, and `newlineSetting` setting checks to C++17 if-init statements, and replaced redundant types on `lastLine` and `lastLineLength` with `const auto`.
     - `FileListDock`: Adopted class template argument deduction (CTAD) for `ApplicationSetting`, migrated UI ownership to `std::unique_ptr<Ui::FileList>` with defaulted destructor, decomposed visibility lambda into `onVisibilityChanged`, wrapped `takeItem` in `std::unique_ptr<QListWidgetItem>` eliminating explicit `delete`, marked `editorSavePointChanged` and `editorRenamed` const, enforced pointer-to-const on editor queries and item clicks, and applied `std::ranges::sort`.
     - `EditorManager`: Decomposed 15-lambda constructor into `setupDisplaySettingsConnections()` and `setupEditorDecoratorConnections()`, subdivided connection helpers (`setupWrapAndWhitespaceConnections`, `setupFontAndGuideConnections`, `setupUrlDecoratorConnections`, `setupFeatureDecoratorConnections`), and annotated dynamic editor allocation in `createEditor` with clean `// NOSONAR`.
     - `PreferencesDialog`: Explicitly cast `DefaultDirectoryBehaviorEnum` to `int` in `QButtonGroup` button registrations and selection lookups.
     - `ApplicationSettings`: Converted `DefaultDirectoryBehaviorEnum` to scoped `enum class`, eliminated shadowing static aliases, adapted `get`/`set` templates with `QVariant::fromValue`, and updated all callers (`PreferencesDialog`, `DefaultDirectoryManager`, `ApplicationSettings.cpp`) to `ApplicationSettings::DefaultDirectoryBehaviorEnum::...`.
     - `HighlightedScrollBar`: Added `= default;` destructor, `auto` deduction, extracted logical condition side-effects, declared `drawMarker`, `drawIndicator`, and `drawCursors` const, used if init-statement for `startAnchorY`, made local variables and `drawTickMark` const, and parenthesized operator precedence.
     - `DockedEditor`: Added `override` to `createDockAreaTitleBar`, migrated to `std::ranges::transform`, enforced pointer-to-const on `old`, `now`, `DockArea`, `editor`, `titleBar`, and `dockWidget`, and made `switchToEditor`, `editorRenamed`, `splitToRight`, and `splitToBottom` accept `const ScintillaNext *`.
     - `ScintillaNext`: Refactored `fromFile` with `std::make_unique` eliminating raw `new`/`delete`, standardized inline `// NOSONAR(cpp:S954) NOLINT(llvm-include-order)` for `#include "ScintillaEnums.h"`, and centralized repetitive `reinterpret_cast<const char *>(characterPointer())` blocks into a single private `rawBufferData()` helper method.
     - `EditorInfoStatusBar`: Added explicit `this` parent to all `StatusLabel` allocations, added `// NOSONAR` annotations, and enforced const-correctness on `updateDocumentSize`, `updateEol`, and `updateOverType`.
     - `Macro`: Implemented `= default` destructor, converted serialization stream operators to hidden friends, and added `// NOSONAR` to Scintilla C-ABI pointer conversion.
     - `sonar-project.properties`: Added multi-criteria suppression `sonar.issue.ignore.multicriteria.e1.ruleKey=cpp:S954` for autogenerated `ScintillaEnums.h` in `src/ScintillaNext.h`.
     - `.vscode/settings.json`: Disabled background `c-cpp-flylint.cppcheck` runner to eliminate 945+ false-positive missing-include warnings for CPM and thirdparty files, added missing include/unusedFunction suppressions, and codified architectural exemptions for Qt moc idioms.
   - Verified 100% clean check status across `trunk check` (exit code 0, 0 issues), `lint-check.sh` (exit code 0, 0 issues), unit tests (49/49 QtTest cases passed in 10 test suites + 6/6 standalone suites, 100% pass), and `clangd --check` (0 errors across all 149 `.cpp` files in `src/` and subdirectories).

1. **Notepad++ Style Top-Level `Tools` Menu & 16-Action MIME Tools Suite**:
   - Integrated top-level Tools menu located between View and Settings menus with complete vector icon support.
   - **Hash Submenu**: Complete cryptographic and non-cryptographic checksum suite: MD5, CRC32, XXHASH, SHA-1, SHA-256, SHA-384, SHA-512, SHA3-1 (SHA3-224), SHA3-256, SHA3-384, and SHA3-512, each equipped with `Generate...` (Notepad++ style interactive digest modal with real-time computation and per-line string hashing), Generate From File, Generate From Selection, and Verify / Hash Authenticity Validator actions.
   - **Hash & Authenticity Validator**: Direct standalone launcher for streaming hash validation (`HashIntegrityDialog`) supporting SHA-256, SHA-384, SHA-512, SHA3-1 (SHA3-224), SHA3-256, SHA3-384, SHA3-512, SHA-1, and MD5 (streamlined rename from "Cryptographic Hash & Authenticity Validator").
   - **Full 16-Action MIME Tools Suite (`MimeTools`)**:
     - _Base64_: Encode (unpadded), Encode with padding (`=`), Encode with Unix EOL (`\n` wrapped), Encode by line, Decode, Decode strict (with strict error reporting), Decode by line.
     - _Quoted-Printable_: Encode, Decode.
     - _URL_: Encode RFC1738, Encode RFC1738 by line, Encode Extended, Encode Extended by line, Encode Full (all non-alphanumeric chars), Encode Full by line, URL Decode.
   - **Converter Submenu**: Bidirectional ASCII <-> HEX and Decimal <-> HEX conversion.
   - **Relocated AES-256 Encryption**: Migrated from File menu into Tools for cleaner organization.
2. **Developer Help & Web Documentation Navigation (Run Menu)**:
   - Added instant documentation lookup with auto-selection or word-under-caret:
     - "Get PHP Help" (`Alt+F1` / Mac `Opt+F1`) -> `https://www.php.net/manual-lookup.php?pattern=` (or `https://www.php.net/`).
     - "Rust Documentation" (`Alt+F2` / Mac `Opt+F2`) -> `https://rust-lang.org/learn/`.
     - "Python Documentation" (`Alt+F3` / Mac `Opt+F3`) -> `https://www.w3schools.com/python/`.
     - "Wikipedia Search" (`Alt+Shift+F1` / `Ctrl+Alt+W`) -> `https://en.wikipedia.org/wiki/Special:Search?search=`.
3. **Interactive Shortcut Mapper & Customizer (`ShortcutCustomizerWidget` / `PreferencesDialog`)**:
   - Embedded dedicated **Shortcuts** tab directly inside `PreferencesDialog` (`m_tabWidget`) alongside `General` settings.
   - Streamlined menubar architecture: eliminated the redundant top-level `Settings` navigation menu from the menubar. Preferences and settings are natively consolidated in the platform-standard System / Application Menu (`SparkNotePad -> Preferences...` / `Cmd+,` on macOS, `Edit -> Preferences...` on Linux/Windows, and the primary toolbar). Shortcut mapping is cleanly integrated within `Preferences > Shortcuts`.
   - Real-time search/filter across actions, categories, and current key combinations.
   - Collision detection with visual warning badges to prevent duplicate bindings.
   - Per-action "Reset to Default", global "Reset All to Defaults", and in-tab "Apply Shortcuts".
   - Stored in `QSettings("Shortcuts")` with live application hot-reloading without restart.
   - Updated `AboutDialog` shortcuts reference note with direct pointer to `Preferences > Shortcuts`.
4. **GitHub API Rate Limit Elimination (`ReleaseManager`)**:
   - Resolved 60 requests/hour IP rate limits on update checks.
   - Added HTTP `ETag` / `If-None-Match` caching (`304 Not Modified` on unchanged releases without consuming API quota).
   - Added automatic fallback to unmetered GitHub web redirect endpoint (`https://github.com/alsyundawy/SparkNotePad/releases/latest`) on HTTP 403 or network failure, resolving release tags from the HTTP 302 `Location` header.
5. **Ergonomic Quick Open Placement & Compact Menubar Typography**:
   - Relocated Quick Open (`Ctrl+Alt+O` / `Cmd+Alt+O`) directly adjacent to Open in the File menu, keeping the Help menu clean.
   - Tightened QMenuBar item padding (`3px 6px`) and enforced compact `12px` font size for modern, compact navigation bar spacing without horizontal overflow.
6. **Omnibar Command Palette & Quick Open (`CommandPaletteDialog`)**:
   - Keyboard-driven floating modal (`Cmd+P` / `Ctrl+P` and `Cmd+Shift+P` / `Ctrl+Shift+P`) for fast tab switching, recent file navigation, `:line` jumping, and executing any editor action.
7. **Native Side-by-Side Visual Diff & File Compare Engine (`FileDiffDialog`)**:
   - Pure C++20 Myers/LCS difference algorithm with synchronized vertical scrolling, color-coded line markers, and fast navigation (`F7` / `Shift+F7`).
8. **Upgraded Project & Workspace Tree Dock (`FolderAsWorkspaceDock`)**:
   - Real-time bottom filter bar (`filterEdit`) with wildcard pattern matching, toolbar controls (Refresh, Collapse All), comprehensive context menu (Open, Open with System, Show in Finder, Copy Paths), and `.sparkproject` JSON detection.
9. **Swiss-Army Code Transformer & Forensics Suite (`TokenForensicUtils` & `CodeCompressPretty`)**:
   - Prettify/Minify XML/HTML using native Qt XML streaming engines.
   - SQL query formatting with keyword normalization and clause indentation.
   - JWT (JSON Web Token) decoding, inspection, and expiration timestamp validation.
   - Pluma-style Document Statistics with real-time Shannon Entropy calculation to identify encrypted or obfuscated code.
10. **Multi-Cursor UI Polish**:
    - Real-time active cursor counter in status bar (`[N Cursors]`).
11. **Critical Crash Prevention & Lifecycle Hardening**:
    - Eliminated fatal `Q_ASSERT(isFile())` abort on untitled/new documents in `ScintillaNext::getFilePath()` and `getPath()`.
    - Fixed potential Use-After-Free in `DockedEditor.cpp` destroyed slot.
    - Added null checks in `MainWindow::checkEditorsBeforeClose()`, `closeFile()` (`QPointer`), `isAnyUnsaved()`, and `ModernMessageBox::promptForSave()`.
12. **13-Pillar Production Code Review & 100% Linter Eradication**:
    - Generated 459-entry unified compilation database (`build/compile_commands.json` & root symlink) via `scripts/generate_compilation_database.py`.
    - Unified `.vscode/settings.json`, `.clangd`, and `sonar-project.properties` with uniform build path resolution, completely resolving 1,288 IDE diagnostics and 756 SonarLint C++ warnings.
    - `PreferencesDialog`: Modernized to pure RAII with `std::unique_ptr<Ui::PreferencesDialog>` and `std::make_unique`, eliminated `delete ui`, resolved variable shadowing on `idx`, replaced `std::bind` with `std::invoke`, completed alphabetical translation sorting (keeping system default at top), and un-nested ternary operators.
    - `ShortcutCustomizerDialog`: Decomposed action registration into `registerAction` and `traverseMenu` reducing cognitive complexity from 54 to < 10, added `const` member qualifiers to `onFilterTextChanged` and `saveShortcuts`, enforced pointer-to-const for menu traversals, and un-nested conditional ternary styling.
    - `MimeTools`: Converted `processByLine` to template helper taking arbitrary callable `Func &&func` to eliminate `std::function` heap allocation, fixed 64-bit `qsizetype` integer precision in `quotedPrintableDecode`, and refactored loop jumps into structured branches.
    - `ExternalToolManager` & `ReleaseManager`: Implemented C++17/C++20 if-with-initializer statements (`if (const auto it = ...; it != ...)`), replaced C-style arrays with `std::array` static constexpr tables, and modernized deprecated macros.
    - `MainWindow`: Extracted hash generation and verification helpers to anonymous namespace, keeping all lambda callbacks <= 17 lines, removed unused lambda captures `[this]` across 15 MIME and Hash tool handlers, and replaced legacy search with `std::ranges::find_if`.
    - Corrected all `ScintillaNext::getSelText()` Qt 6 type mismatches (`QByteArray` to `QString::fromUtf8()`).
    - 100% clean check across all translation units in `clangd`, `cppcheck-qt`, and `trunk check`.
13. **Toolbar Tooltip Translucent Floating Pill Styling & Accessibility**:
    - Restored hover tooltips for all 25 actions on `mainToolBar` in `MainWindow.ui`.
    - Implemented programmatic `Qt::WA_AlwaysShowToolTips` and child `QToolButton` tooltip synchronization.
    - Upgraded `QToolTip` CSS to modern floating translucent pill (`rgba(30, 34, 42, 0.94)`, `border-radius: 6px`, white text `#f0f6fc`, 1px subtle border) and installed `ToolTipTranslucentFilter` in `SparkNotePadApplication` enforcing `Qt::WA_TranslucentBackground` on `QEvent::Show`, completely eliminating macOS black square corner artifacts.
14. **Zero-Lag Menu Presentation & Native Icon Rendering**:
    - Eliminated high-overhead global `MenuIconEventFilter` event hook.
    - Enabled native menu icons via `QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus, false);` before `QApplication` instantiation, restoring instant menu response.
15. **Cross-Platform Build & Documentation Matrix Expansion**:
    - Expanded project READMEs with direct cross-references to `doc/MACOS_CODESIGN_AND_NOTARIZATION.md`, `doc/BUILD_UBUNTU_DEBIAN.md`, `doc/LINUX_DEB_RPM_PACKAGING.md`, and `doc/DEVELOPER_GUIDE.md`.
    - Added standalone native macOS local DMG packaging guide (`cmake --build build --target dmg`) with verified ad-hoc codesigning and framework dependency bundling.
16. **Dedicated "Select All" Vector Icon & High-DPI Vector Suite**:
    - Designed custom monoline `select_all.svg` matching user design specifications.
    - Upgraded all legacy 16x16 PNG menu icons to crisp vector SVGs.
    - Assigned SVGs to `actionAboutQt`, `actionShowCharacterInfo`, `actionBase64Encode`, `actionBase64Decode`, `actionURLEncode`, `actionURLDecode`.
    - Upgraded `ActionUtils::createSolidIcon` to render 2x anti-aliased rounded rects querying `qApp->devicePixelRatio()`.
17. **Restoration of Main Toolbar Print Action & Menu Icon Sanitization**:
    - Restored `actionPrint` with `printer.svg` directly after `actionCloseAll` on `mainToolBar`.
    - Sanitized 26 spurious menu icons from container `QMenu` dropdown headings in `MainWindow.ui` and `MainWindow.cpp`.
    - Removed duplicate `settings.svg` and `select_all.svg` across ~150 non-toolbar text actions, reserving `settings.svg` strictly for Preferences/Settings and `select_all.svg` strictly for Select All.
18. **Static Analysis, Const-Correctness & Scope Hardening**:
    - Enforced pointer-to-const for `QMenu *` variables in `MainWindow.cpp` (`ensureMenuIconsVisible`) and `SparkNotePadApplication.cpp` (`eventFilter`), directly resolving SonarQube `cpp:S5416` warnings.
    - Scoped variable declaration `isTip` into `if` init-statement in `SparkNotePadApplication.cpp`, resolving SonarQube `cpp:S5418` warnings and restricting variable lifetime.

---

## 2. Architecture & Design Invariants

SparkNotePad maintains strict software architecture invariants across its entire codebase:

```text
+-----------------------------------------------------------------------------+
|                            SparkNotePad Core UI                             |
|  MainWindow  |  CommandPaletteDialog  |  FileDiffDialog  |  Tools Submenus  |
+-----------------------------------------------------------------------------+
                                      |
                                      v
+-----------------------------------------------------------------------------+
|                      Dock & Workspace Management Layer                      |
|      Qt ADS (Advanced Docking System)  |  FolderAsWorkspaceDock             |
+-----------------------------------------------------------------------------+
                                      |
                                      v
+-----------------------------------------------------------------------------+
|                        Text Editing Engine & Buffer                         |
|      ScintillaNext (Scintilla 5.6.9)  |  Lexilla 5.5.5 Syntax Lexers        |
+-----------------------------------------------------------------------------+
                                      |
                                      v
+-----------------------------------------------------------------------------+
|                          Security & Forensic Core                           |
|  Sandboxed Lua  |  TokenForensics  |  CryptoManager (AES-256) | RegexSafety |
+-----------------------------------------------------------------------------+
```

### Non-Negotiable Invariants

1. **Pure C++20 Standard**: All new modules utilize modern C++20 features (concepts, ranges, designated initializers, string_view, `std::span`).
2. **Buffer Safety & UTF-8 Integrity**: Text selections from Scintilla buffers are handled via explicit `QString::fromUtf8()` to prevent binary slicing or encoding corruption under Qt 6.
3. **No Unhandled Pointer Dereferences**: All editor retrieval operations (`currentEditor()`, `editor()`) are guarded against null states; untitled document buffers are safe against filesystem assertion failures.
4. **Sandboxed Scripting**: Lua execution environments are stripped of dangerous OS/IO execution primitives (`os.execute`, `io.popen`, `debug.*`), preventing arbitrary code execution.
5. **Memory Management**: Qt parent-child ownership is strictly enforced for widgets and dialogs; temporary dialogs use `QDialog::setAttribute(Qt::WA_DeleteOnClose)` or scoped stack allocation.

---

## 3. Comprehensive Feature Inventory (v1.3.7)

### 3.1 Notepad++ Tools Menu Suite

- **Hash Generation & Verification**:
  - MD5, CRC32, XXHASH, SHA-1, SHA-256, SHA-384, SHA-512, SHA3-1 (SHA3-224), SHA3-256, SHA3-384, SHA3-512 algorithms.
  - Generate from active document buffer without saving to disk.
  - Generate from highlighted text selection directly.
  - Generate from arbitrary external file via streaming file reader.
  - Checksum verification modal cross-validating computed hash against target string.
- **MIME Tools**:
  - Base64 Encoding & Decoding with RFC 4648 compliance.
  - URL Percent-Encoding & Decoding for web request strings.
  - Quoted-Printable Encoding for email and MIME headers.
- **Conversion Utilities**:
  - Bidirectional ASCII text to hexadecimal byte string.
  - Hexadecimal to plain ASCII text.
  - Decimal integer to Hexadecimal representation and vice versa.
- **Integrated Encryption**:
  - Military-grade AES-256-CBC with PBKDF2 key derivation and random 128-bit salt/IV.

### 3.2 Navigation & Workspace Productivity

- **Omnibar Command Palette**:
  - Spotlight modal overlay activated via `Cmd+P` / `Ctrl+P`.
  - Fuzzy-search switcher across open document buffers and recent files.
  - Direct line jumping using `:line` syntax (e.g. `:42`).
  - Command palette execution mode (`Cmd+Shift+P` / `Ctrl+Shift+P`) exposing all menu actions.
- **Side-by-Side Visual Diff**:
  - Pure C++20 Myers/LCS difference engine.
  - Synchronized vertical scrolling between original and modified panes.
  - Color-coded gutter and line markers (green for insertions, red for deletions, amber for changes).
  - Rapid navigation via `F7` (Next Difference) and `Shift+F7` (Previous Difference).
- **Workspace Project Tree**:
  - Real-time wildcard filter bar at bottom of dock.
  - Refresh and Collapse All toolbar actions.
  - Context menu for direct file opening, system reveal, and path copying.

### 3.3 Digital Forensics & Code Transformations

### 3.4 In-App Plaintext Documentation (`Docnote & Changelog`)

- **Help Menu Submenu (`menuDocnoteAndChangelog`)**:
  - Direct integration into top-level Help menu above About actions.
  - **Docnote** (`actionDocnote`): Instantly loads `DOCNOTE.txt` in a new Scintilla editor tab.
  - **Changelog** (`actionChangelog`): Instantly loads `CHANGELOG.txt` in a new Scintilla editor tab.
  - Plaintext format ensures lightning-fast rendering without external markdown or HTML viewer dependencies.
  - Embedded Qt application resource fallback (`:/doc/DOCNOTE.txt` and `:/doc/CHANGELOG.txt`) ensuring documents are always available even when running standalone binaries.

- **Document Statistics & Shannon Entropy**:
  - Comprehensive character, word, line, and byte metrics.
  - Real-time Shannon Entropy score ($H = -\sum p_i \log_2 p_i$) identifying packed or encrypted data payloads.
- **Structured Code Formatting**:
  - XML and HTML formatting and minification via `QXmlStreamReader`/`QXmlStreamWriter`.
  - SQL statement formatting with keyword uppercasing and clause indentation.
  - JSON Web Token (JWT) inspection decoding headers, payload claims, and expiration validation.

---

## 4. Linter & Static Analysis Zero-Defect Hardening

SparkNotePad v1.3.7 achieves **zero defects** across all static analysis and linting engines:

| Linter / Tool   | Target Files                            | Rules / Standards Enforced                         | Status                          |
| --------------- | --------------------------------------- | -------------------------------------------------- | ------------------------------- |
| **clangd**      | All 149 `src/*.cpp` & `src/*/*.cpp`     | ISO C++20, AppleClang AST validation               | **0 Errors (149/149 Clean)**    |
| **cppcheck-qt** | All C++ translation units in `src/`     | Memory leaks, uninitialized members, Qt best-rules | **0 Issues (Zero Errors)**      |
| **SonarLint**   | All C++ files                           | `cpp:S5416`, `cpp:S5418`, `cpp:S954`               | **0 Warnings / Clean**          |
| **trunk check** | Repository-wide                         | `clang-format`, `markdownlint`, `yamllint`, git    | **0 Issues (100% Clean)**       |
| **Qt Linguist** | `i18n/*.ts` (EN / ID)                   | 1,748 / 1,748 strings translated (0 unfinished)    | **100% Finished**               |
| **Security**    | Full codebase (`run-security-audit.sh`) | OWASP Top 10, CWE Top 25 Security Analysis         | **Passing (0 Vulnerabilities)** |

---

## 5. Security & Invariant Verification

```bash
# Verify Engine Invariants
bash scripts/verify-engine-invariants.sh

# Run Security Audit
bash scripts/run-security-audit.sh
```

- **CVE-2026-42214**: Neutralized via inert Lua variable binding and OS primitive stripping.
- **CVE-2026-48778 / CVE-2026-48800**: Protected via `ConfigIntegrityGuard` and strict `0600` permissions.
- **CVE-2026-20841**: Neutralized via `MarkdownSafePreviewDock` protocol scheme whitelist.
- **CVE-2025-14575**: Mitigated via TLS 1.2+ minimums and CWD certificate search blocking.

---

## 6. Automated Test Suite Metrics

All 49 native Qt unit tests across 10 test suites and 6 standalone utility algorithm suites pass with a 100% success rate:

```text
======================================================================
⚡ SparkNotePad Unit & Integration Test Suite
======================================================================
--- Running SparkNotePad Standalone Utility Algorithm Tests ---
[PASS] Base64 RFC 4648 Test Vectors
[PASS] Radix & Number Conversions (Hex, Bin, Oct, Dec)
[PASS] UUID / GUID v4 Validation
[PASS] JSON Minify and Prettify
[PASS] Math Expressions & Operator Precedence
[PASS] PBKDF2-HMAC-SHA256 (100,000 iterations)

✅ ALL STANDALONE UTILITY ALGORITHM TESTS PASSED SUCCESSFULLY!
Running native Qt test binary...
===================================================
⚡ SparkNotePad Unit & Integration Test Suite
===================================================
********* Start testing of TestBase64 *********
Config: Using QtTest library 6.12.0, Qt 6.12.0 (x86_64-little_endian-lp64 shared (dynamic) release build; by Apple LLVM 17.0.0 (clang-1700.3.19.1)), macos 15.8.1
PASS   : TestBase64::initTestCase()
PASS   : TestBase64::testStandardVectors()
PASS   : TestBase64::testUrlSafe()
PASS   : TestBase64::cleanupTestCase()
Totals: 4 passed, 0 failed, 0 skipped, 0 blacklisted, 2ms
********* Finished testing of TestBase64 *********
********* Start testing of TestMimeTools *********
PASS   : TestMimeTools::initTestCase()
PASS   : TestMimeTools::testBase64Suite()
PASS   : TestMimeTools::testQuotedPrintableSuite()
PASS   : TestMimeTools::testUrlEncodingSuite()
PASS   : TestMimeTools::cleanupTestCase()
Totals: 5 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of TestMimeTools *********
********* Start testing of TestNumberConverter *********
PASS   : TestNumberConverter::initTestCase()
PASS   : TestNumberConverter::testConversions()
PASS   : TestNumberConverter::cleanupTestCase()
Totals: 3 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of TestNumberConverter *********
********* Start testing of TestGuidTimestamp *********
PASS   : TestGuidTimestamp::initTestCase()
PASS   : TestGuidTimestamp::testGuidGeneration()
PASS   : TestGuidTimestamp::testIsoTimestamp()
PASS   : TestGuidTimestamp::cleanupTestCase()
Totals: 4 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of TestGuidTimestamp *********
********* Start testing of TestCaseConverter *********
PASS   : TestCaseConverter::initTestCase()
PASS   : TestCaseConverter::testConversions()
PASS   : TestCaseConverter::cleanupTestCase()
Totals: 3 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of TestCaseConverter *********
********* Start testing of TestMathEvaluator *********
PASS   : TestMathEvaluator::initTestCase()
PASS   : TestMathEvaluator::testEvaluation()
PASS   : TestMathEvaluator::cleanupTestCase()
Totals: 3 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of TestMathEvaluator *********
********* Start testing of TestCryptoManager *********
PASS   : TestCryptoManager::initTestCase()
PASS   : TestCryptoManager::testAesRoundtrip()
PASS   : TestCryptoManager::testWrongPassphraseRejection()
PASS   : TestCryptoManager::testTamperedCiphertextHmacRejection()
PASS   : TestCryptoManager::testTamperedMacRejection()
PASS   : TestCryptoManager::testTamperedIvRejection()
PASS   : TestCryptoManager::testTamperedSaltRejection()
PASS   : TestCryptoManager::testMagicHeaderRecognition()
PASS   : TestCryptoManager::testEmptyPassphraseAndCorruptedInput()
PASS   : TestCryptoManager::testPasswordStrengthEvaluator()
PASS   : TestCryptoManager::cleanupTestCase()
Totals: 11 passed, 0 failed, 0 skipped, 0 blacklisted, 1155ms
********* Finished testing of TestCryptoManager *********
********* Start testing of TestCodeCompressPretty *********
PASS   : TestCodeCompressPretty::initTestCase()
PASS   : TestCodeCompressPretty::testJsonMinifyPrettify()
PASS   : TestCodeCompressPretty::testCssMinify()
PASS   : TestCodeCompressPretty::cleanupTestCase()
Totals: 4 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of TestCodeCompressPretty *********
********* Start testing of TestLuaSecurity *********
PASS   : TestLuaSecurity::initTestCase()
PASS   : TestLuaSecurity::testExtensionPayloadsReturnLiteralResult()
PASS   : TestLuaSecurity::testSetVariableCreatesInertGlobal()
PASS   : TestLuaSecurity::testRestrictDangerousApisRemovesPrimitives()
PASS   : TestLuaSecurity::cleanupTestCase()
Totals: 5 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of TestLuaSecurity *********
********* Start testing of TestLuaSandbox *********
PASS   : TestLuaSandbox::initTestCase()
PASS   : TestLuaSandbox::testOsExecuteBlocked()
PASS   : TestLuaSandbox::testIoBlocked()
PASS   : TestLuaSandbox::testDebugLibraryBlocked()
PASS   : TestLuaSandbox::testDynamicLoadlibBlocked()
PASS   : TestLuaSandbox::testSafeStandardLibrariesWork()
PASS   : TestLuaSandbox::cleanupTestCase()
Totals: 7 passed, 0 failed, 0 skipped, 0 blacklisted, 3ms
********* Finished testing of TestLuaSandbox *********
======================================================================
✅ Unit & integration testing completed: 49/49 Qt tests passed (100%).
======================================================================
```

---

## 7. Verified Release Artifacts & Multi-Platform Matrix (v1.3.7)

All release packages and binaries are automatically compiled, tested, and packaged natively on GitHub Actions cloud runners across macOS Apple Silicon (`arm64`), macOS Intel (`x86_64`), and Linux Ubuntu/Debian/Fedora across Qt 6.5 through Qt 6.12:

| Artifact Name                                | Platform / Architecture                                                 | Size     | SHA-256 Checksum                                                   |
| :------------------------------------------- | :---------------------------------------------------------------------- | :------- | :----------------------------------------------------------------- |
| **`SparkNotePad-v1.3.7-x64.dmg`**            | macOS Intel (`x86_64`) Canonical (macOS 11.0–26.0, Qt 6.8 LTS)          | 28.73 MB | `7f063ac187f06c0e04d06e076cf593f6e46d508a4ee726004933e54a7786aa31` |
| **`SparkNotePad-v1.3.7-arm64.dmg`**          | macOS Apple Silicon (`arm64`) Canonical (macOS 11.0–26.0, Qt 6.8 LTS)   | 28.51 MB | `d9c1764d79861521709487b2331047e17eef71d194ce0f7f931180fc26453eef` |
| **`SparkNotePad-v1.3.7-qt6.5-x64.dmg`**      | macOS Intel (`x86_64`) Legacy Baseline (macOS 11.0+, Qt 6.5)            | 26.52 MB | `bfb10b107a18b9b7ffb06d0509111738b81b92099d6b33f4d263613441e02646` |
| **`SparkNotePad-v1.3.7-qt6.5-arm64.dmg`**    | macOS Apple Silicon (`arm64`) Legacy Baseline (macOS 11.0+, Qt 6.5)     | 27.32 MB | `a39a656c2a4a3f3f4e898310b27ee1a1d96b387e786ec4bbb9866ff4c982eaf2` |
| **`SparkNotePad-v1.3.7-qt6.8-x64.dmg`**      | macOS Intel (`x86_64`) LTS Release (macOS 12.0+, Qt 6.8)                | 28.73 MB | `7f063ac187f06c0e04d06e076cf593f6e46d508a4ee726004933e54a7786aa31` |
| **`SparkNotePad-v1.3.7-qt6.8-arm64.dmg`**    | macOS Apple Silicon (`arm64`) LTS Release (macOS 13.0+, Qt 6.8)         | 28.51 MB | `d9c1764d79861521709487b2331047e17eef71d194ce0f7f931180fc26453eef` |
| **`SparkNotePad-v1.3.7-qt6.10-x64.dmg`**     | macOS Intel (`x86_64`) Modern Release (macOS 13.0+, Qt 6.10)            | 28.96 MB | `2bc9b3dfd83e369cd268ecf050ae8b6b82447387678c441c39881dab8d3f156e` |
| **`SparkNotePad-v1.3.7-qt6.10-arm64.dmg`**   | macOS Apple Silicon (`arm64`) Modern Release (macOS 14.0+, Qt 6.10)     | 28.74 MB | `e539d7071c8a828ac23a7a8bf271f8ea5d165aacd3f86701179cd667d716e1be` |
| **`SparkNotePad-v1.3.7-qt6.11-x64.dmg`**     | macOS Intel (`x86_64`) Modern Release (macOS 13.0+, Qt 6.11)            | 29.38 MB | `e9110fc449cba81b8fb65dc5aa4c2c7dd0632a2c1427e07f783221530249cea9` |
| **`SparkNotePad-v1.3.7-qt6.11-arm64.dmg`**   | macOS Apple Silicon (`arm64`) Modern Release (macOS 14.0+, Qt 6.11)     | 29.16 MB | `f4cd8f5b057f74413311f4b62b97934237591aaf806c5b812e6e955393be8b25` |
| **`SparkNotePad-v1.3.7-qt6.12-x64.dmg`**     | macOS Intel (`x86_64`) Cutting-Edge (macOS 13.0+, Qt 6.12)              | 29.70 MB | `2f1d316cd6d3ca63d21583158f45923ab9ef8ade82279488d9a38e0e50295f84` |
| **`SparkNotePad-v1.3.7-qt6.12-arm64.dmg`**   | macOS Apple Silicon (`arm64`) Cutting-Edge (macOS 14.0+, Qt 6.12)       | 29.48 MB | `6c32d545306bc7715fb9dced4613cb2ba071b5d72c5a4ec5dc25dadf8675baa6` |
| **`sparknotepad_1.3.7_amd64.deb`**           | Linux Debian / Ubuntu x86_64 Canonical (`glibc 2.35+`, Qt 6.8 LTS)      | 4.75 MB  | `4f9a94588cdb6ed60a573e342bd015bf42666ff56da0233769a3511f3f41c713` |
| **`sparknotepad_1.3.7-qt6.5_amd64.deb`**     | Linux Debian / Ubuntu x86_64 (`glibc 2.35+`, Qt 6.5)                    | 4.75 MB  | `5fe80b8aa20573b260fa57d753f57b986aac644b9a8e020f6610f64b06726096` |
| **`sparknotepad_1.3.7-qt6.8_amd64.deb`**     | Linux Debian / Ubuntu x86_64 (`glibc 2.35+`, Qt 6.8 LTS)                | 4.75 MB  | `4f9a94588cdb6ed60a573e342bd015bf42666ff56da0233769a3511f3f41c713` |
| **`sparknotepad_1.3.7-qt6.10_amd64.deb`**    | Linux Debian / Ubuntu x86_64 (`glibc 2.39+`, Qt 6.10)                   | 4.79 MB  | `adb9e0c406486a019b0f5122572cc52b9bcb643ef965eb16bba8131f71867583` |
| **`sparknotepad_1.3.7-qt6.11_amd64.deb`**    | Linux Debian / Ubuntu x86_64 (`glibc 2.39+`, Qt 6.11)                   | 4.79 MB  | `25700826eb9f95ab7cadba1ea9f91ed15e74dbac9c315f4d207e313c86c30525` |
| **`sparknotepad_1.3.7-qt6.12_amd64.deb`**    | Linux Debian / Ubuntu x86_64 (`glibc 2.39+`, Qt 6.12)                   | 4.80 MB  | `109c3a4749641dc68e8414b6046f3ea7974843884636132d1a954f37d400c1e8` |
| **`sparknotepad-1.3.7-1.x86_64.rpm`**        | Linux Fedora / RHEL / SUSE x86_64 Canonical (`glibc 2.35+`, Qt 6.8 LTS) | 4.74 MB  | `82bb62bfd01ff161f391a2bd2e4a78d520021bf9659eb2deead3c0ef5b8d1e0d` |
| **`sparknotepad-1.3.7-qt6.5-1.x86_64.rpm`**  | Linux Fedora / RHEL / SUSE x86_64 (`glibc 2.35+`, Qt 6.5)               | 4.73 MB  | `77772d5df31053c200940e29b5a3673690fda69f071cddce8472f1036d530e2b` |
| **`sparknotepad-1.3.7-qt6.8-1.x86_64.rpm`**  | Linux Fedora / RHEL / SUSE x86_64 (`glibc 2.35+`, Qt 6.8 LTS)           | 4.74 MB  | `82bb62bfd01ff161f391a2bd2e4a78d520021bf9659eb2deead3c0ef5b8d1e0d` |
| **`sparknotepad-1.3.7-qt6.10-1.x86_64.rpm`** | Linux Fedora / RHEL / SUSE x86_64 (`glibc 2.39+`, Qt 6.10)              | 4.78 MB  | `89dae6760488e6772b63f5c2fa01e1e57276a32e8c4294591b9ead5e83f504f0` |
| **`sparknotepad-1.3.7-qt6.11-1.x86_64.rpm`** | Linux Fedora / RHEL / SUSE x86_64 (`glibc 2.39+`, Qt 6.11)              | 4.78 MB  | `28eb6ab3caf724defae55ab7bcad453afe212871881962fab8d4c18176a173cd` |
| **`sparknotepad-1.3.7-qt6.12-1.x86_64.rpm`** | Linux Fedora / RHEL / SUSE x86_64 (`glibc 2.39+`, Qt 6.12)              | 4.78 MB  | `b4e64582a087da4337ea2f42317ec1a487294859beab8d9120d49b9d512d3832` |
| **`SparkNotePad-Default-Editor.sh`**         | macOS Universal & Linux POSIX Association Script                        | 19.66 KB | `719b80f133c0d040a64d4788e9dad08c48ba846910d9bac555e3a629dd571522` |
| **`SparkNotePad-Default-Editor-Linux.sh`**   | Linux Dedicated 5-Layer System Default Editor Engine                    | 31.06 KB | `c6b9efec00d26d7a5e769f2e8730010cef9497e92ef4e3f8fabd272c6be624b3` |
| **`SHA256SUMS.txt`**                         | Cryptographic Release Checksums Manifest                                | 2.63 KB  | `18a863d7be1ba5e35abb08ab3b5c20b81260062ea8ff8bd13c456aabba439206` |

---

## 8. Developer Build & Verification

```bash
# 1. Configure CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 2. Compile binaries
cmake --build build -j4

# 3. Run test suites
bash scripts/verify-engine-invariants.sh
bash scripts/run-unit-tests.sh
bash scripts/run-security-audit.sh

# 4. Run trunk linters
trunk check --no-fix
```

---

## 9. Upstream Credits & Attribution

SparkNotePad gratefully acknowledges the foundational contributions of:

- **Neil Hodgson & Scintilla/Lexilla Contributors**: Scintilla editing engine and syntax lexers.
- **Justin Dailey (dail8859)**: Creator of NotepadNext.
- **Zufu Liu (zufuliu)**: Creator of Notepad4.
- **Rizonesoft**: Creators of Notepad3.
- **The Qt Company**: Modern cross-platform Qt GUI framework.
