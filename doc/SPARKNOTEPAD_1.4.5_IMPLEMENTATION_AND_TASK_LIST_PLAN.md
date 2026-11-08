# SparkNotePad 1.4.5 — Multi-Repository Innovation Adoption, 100+ Lexers, Native Export & Master Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use `superpowers:subagent-driven-development` (recommended) or `superpowers:executing-plans` to implement this plan task-by-task. Steps use checkbox (`- [x]`) syntax for tracking.

- **Date:** 2026-11-08
- **Target Version:** SparkNotePad v1.4.5
- **Base Version:** SparkNotePad v1.4.0 (released 2026-11-07)
- **Editor Engine:** Scintilla 5.6.9 + Lexilla 5.5.5 + Lua 5.5.1 + Qt 6.5–6.12 (ISO C++20)
- **Lead Maintainer:** HARRY DERTIN SUTISNA ALSYUNDAWY (`@alsyundawy`)
- **Repository:** [https://github.com/alsyundawy/SparkNotePad](https://github.com/alsyundawy/SparkNotePad)

---

## 1. Executive Summary & Research Context

### 1.1 Overview & Baseline Status

Following deep empirical research across twelve open-source text editing, note-taking, and IDE repositories, combined with thorough validation of the current **SparkNotePad v1.4.0** codebase, this document provides the unified, optimized, and non-destructive implementation plan for **SparkNotePad v1.4.5**.

**SparkNotePad Current State (Verified v1.4.0 Baseline):**

- Engine: Native Scintilla 5.6.9 + Lexilla 5.5.5 (`SCI_OWNREGEX`) on Qt 6.5–6.12 (C++20).
- Security: AES-256 document encryption (PBKDF2 100k + HMAC-SHA256), URI allowlisting, sandboxed Lua/Python runtimes.
- Customization: 75 curated developer themes across 25 families meeting WCAG 2.1 AA/AAA standards.
- Intelligence: Local MCP Server (`LocalMcpServer.cpp`), batch macro runner, visual diff dialog, real-time log stream monitoring (`tail -f`).
- Efficiency: Idle resident memory under 35 MB RSS; sub-millisecond line rendering; zero webview bloat.
- Packaging & i18n: macOS Universal (Apple Silicon & Intel), Linux (DEB, RPM, Flatpak, AppImage), FreeBSD; fully bilingual (English & Bahasa Indonesia).

### 1.2 Core Principle: Non-Destructive Additive Enhancement ("Do No Harm")

Every feature incorporated in version 1.4.5 is strictly **additive and opt-in**:

- **Zero Regressions:** Existing menus, keyboard shortcuts, settings keys, lexer bindings, and theme definitions will NOT be altered, deprecated, or broken.
- **Strict Architectural Modularization:** Every new subsystem is implemented in isolated C++20 classes with clearly bounded interfaces.
- **Performance & Thermal Safety:** Idle memory remains strictly under 40 MB RSS. All dockable panels are lazy-loaded upon first activation. Permanent prohibition of Ninja build generator (Unix Makefiles only).

---

## 2. Comparative Deep Research Analysis of 12 Target Repositories

### 2.1 Cross-Repository Matrix & Feasibility

|  No.   | Repository                                                                                                        | Stack / Text Engine                      | Key Highlights & Architecture                                                                                                  | SparkNotePad v1.4.5 Adoption Scope                                                                                                           |
| :----: | :---------------------------------------------------------------------------------------------------------------- | :--------------------------------------- | :----------------------------------------------------------------------------------------------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------- |
| **1**  | [`texpile/texpile`](https://github.com/texpile/texpile)                                                           | TypeScript, Electron, React / Custom AST | Dual-mode Visual/Source, sub-40ms live preview via LuaLaTeX/Typst compiler hook, two-way SyncScroll.                           | **High**: Adopt two-way synchronized scrolling and KaTeX math formula rendering in Markdown preview.                                         |
| **2**  | [`ricrio-inc/plainhub`](https://github.com/ricrio-inc/plainhub)                                                   | Node.js, CLI / Git-backed MCP            | PlainHub CLI + MCP server (`plainhub-mcp`), Claude Code bridge, `.agents/` decision cards & ledger in Git.                     | **High**: Expand `LocalMcpServer` tools for buffer/tab control and agent decision cards viewer.                                              |
| **3**  | [`LucaSorvillo/vertical-tabs-notepad`](https://github.com/LucaSorvillo/vertical-tabs-notepad)                     | C++, WinUI 3, WinRT / Win32 Notepad      | Chrome-style vertical tabs with horizontal text, 9 tab color groups, `Ctrl+Shift+T` closed tab stack.                          | **Crucial**: Adopt Reopen Closed Tab (`Cmd+Shift+T` / `Ctrl+Shift+T`) stack, Tab Color Tagging, and Vertical Tabs Dock.                      |
| **4**  | [`stefankueng/BowPad`](https://github.com/stefankueng/BowPad)                                                     | C++, Win32 / Scintilla                   | Scrollbar navigation hints, tab color by folder, Corresponding file toggle (`Alt+O`), native `.editorconfig`.                  | **Crucial**: Adopt Scrollbar Navigation Markers, Corresponding File Switcher (`F4`/`Alt+O`), and native `.editorconfig`.                     |
| **5**  | [`Justhuydo/notepad-plus-plus-elevated-edition`](https://github.com/Justhuydo/notepad-plus-plus-elevated-edition) | Markdown / HTML / Concept                | Extended regex escape transformations (`\xHH`, `\dDDD`), AST-aware tooling.                                                    | **High**: Ground-truth validation of extended search escape parsing.                                                                         |
| **6**  | [`texstudio-org/texstudio`](https://github.com/texstudio-org/texstudio)                                           | C++, Qt 6 / QCodeEdit                    | Document structure tree dock (headings, sections, figures), live reference checking, split preview.                            | **High**: Universal Document Structure Dock for Markdown headers (`#`), JSON keys, and code symbols.                                         |
| **7**  | [`deepanjanxyz/notepad`](https://github.com/deepanjanxyz/notepad)                                                 | Kotlin, Compose / Room DB                | Interactive checklists (`- [x]`/`- [x]`), smart list continuation, per-note pinning, 100% offline.                             | **High**: Adopt Checklist Toggle shortcut (`Cmd+Shift+C`), list auto-continuation on Enter, and Pin Tabs.                                    |
| **8**  | [`BLumia/pineapple-notepad`](https://github.com/BLumia/pineapple-notepad)                                         | C++, Qt 6 / Scintilla                    | Fuzzy Quick Switcher (`Ctrl+P`), Document List Dock, Favorites Manager, file change watcher.                                   | **Crucial**: Adopt Fuzzy Quick Open / Document Switcher (`Cmd+P`) and Favorites Dock.                                                        |
| **9**  | [`danielkrinets/NoteCraft`](https://github.com/danielkrinets/NoteCraft)                                           | C11, SDL2 / Page Engine                  | Form-feed (`\f`) visual page boundary dividing, clean EPUB 3 export, paginated word wrap.                                      | **Medium**: Adopt Visual Form-Feed (`\f`) dividing line in Scintilla and clean document export.                                              |
| **10** | [`nextpad-plus-plus/nextpad-plus-plus-macos`](https://github.com/nextpad-plus-plus/nextpad-plus-plus-macos)       | Obj-C++, C++20 / Scintilla + Lexilla     | Document Minimap Dock, macOS Tahoe-style toolbar preset, macOS Dock recent documents menu, Clipboard History.                  | **Crucial**: Adopt Scintilla Document Minimap Dock, Clipboard History Dock, and macOS Dock Recent Files.                                     |
| **11** | [`TreeNote/TreeNote`](https://github.com/TreeNote/TreeNote)                                                       | Python 3, PyQt5 / Qt Tree                | Hierarchical outliner for notes/tasks, collapsible parent-child nodes.                                                         | **Medium**: Collapsible tree navigation in Document Structure Dock.                                                                          |
| **12** | [`qihaiyan/eton`](https://github.com/qihaiyan/eton)                                                               | C, Win32 / Scintilla + Lexilla           | 13 native Mermaid diagram families in preview, LaTeX math formulas, Extended Find escapes, Developer Tools, Autosave/Recovery. | **Crucial**: Adopt complete Extended Find/Replace escape parser, Developer Tools suite, Mermaid + Math preview, and Autosave/Draft Recovery. |

### 2.2 Intentionally Deprioritized Features (High Risk / Incompatible)

- **Heavily Sandboxed Web Runtimes (Texpile Electron / Collaborative Sync):** Would break the <35 MB idle memory invariant and violate the offline-first privacy model.
- **Hosted Cloud Repositories & Telemetry (PlainHub Web/Voice):** SparkNotePad operates strictly offline without telemetry or third-party servers.
- **Minecraft Game UI Elements (NoteCraft):** Novelty feature incompatible with a professional developer code editor.
- **Win32 DLL/Registry Hooks (ETON/BowPad Explorer context menus):** SparkNotePad targets macOS, Linux, and FreeBSD; native platform services are used instead.

---

## 3. SparkNotePad v1.4.5 Innovation Suite Architecture

```text
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                   SPARKNOTEPAD v1.4.5 ARCHITECTURE                                     │
├───────────────────────────────┬───────────────────────────────┬────────────────────────────────────────┤
│ 1. Tabs & Navigation          │ 2. Scintilla & Search Engine  │ 3. Lexers & Language Support           │
│ • Reopen Closed Tab           │ • Full Extended Escapes       │ • 100+ Lexilla 5.5.5 Language Lexers   │
│   (Cmd+Shift+T / LIFO Stack)  │   (\xHH, \dDDD, \b, \f, \0)   │ • Modern Languages (Rust, Go, Zig,     │
│ • Tab Color Tagging (8 Swatch)│ • Scrollbar Overview Markers  │   Kotlin, Swift, TypeScript, C23)      │
│ • Optional Vertical Tabs Dock │ • Corresponding File Switcher │ • Expanded Folding & Keyword Lists     │
│ • Fuzzy Quick Open (Cmd+P)    │   (Header <-> Source / Alt+O) │ • WCAG AAA All 75 Themes Tested        │
├───────────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ 4. Native Export Pipeline     │ 5. Autosave & Draft Recovery  │ 6. Developer Tools & Preview           │
│ • MD → HTML (Theme CSS)       │ • Configurable Autosave (10s) │ • JSON Prettify & Minify (Error Jump)  │
│ • MD → PDF (QPrinter Native)  │ • Crash Recovery Dialog       │ • URL & Unicode (\uXXXX) Encode/Decode │
│ • HTML → PDF Direct Print     │ • Strict 0600 File Security   │ • UUID v4 & Timestamp Converter        │
│ • Optional Pandoc Fallback    │ • Unsaved Buffer Journaling   │ • Mermaid Diagrams & KaTeX Math Render │
├───────────────────────────────┴───────────────────────────────┴────────────────────────────────────────┤
│ 7. Workspace & macOS Polish: Document Minimap Dock, Clipboard History Dock, macOS Dock Recent Files    │
└────────────────────────────────────────────────────────────────────────────────────────────────────────┘
```

---

## 4. Strict Non-Regression & Engineering Guardrails

1. **Memory Budget Invariant:** Idle resident set size (RSS) must remain strictly under 40 MB. All docks (Document Minimap, Clipboard History, Favorites) are instantiated lazily.
2. **Buffer Security & Permissions:** Autosave draft files and journal buffers must be created with strict `0600` (`S_IRUSR | S_IWUSR`) permissions in `~/.sparknotepad/recovery/`.
3. **Encoding & Line Ending Fidelity:** All string transformations, exports, and escape parsers must maintain UTF-8 integrity and preserve detected CRLF / LF line endings without byte corruption.
4. **Theme Harmonization:** Every UI component, new dock, dialog, and exported HTML template must dynamically inherit the active palette from `ThemeManager::instance()`.
5. **CMake Generator Mandate:** Permanent prohibition of Ninja. Builds must execute with standard CMake Unix Makefiles only.

---

## 5. Phased Roadmap & Actionable Master Todo List

### Phase 1: Foundation & Session Resilience (Week 1)

- [x] **1.1** Implement `ClosedTabsManager`: LIFO stack of last 25 closed tabs (file path, cursor position, scroll line, tab color, encoding, EOL) persisted in session JSON.
- [x] **1.2** Implement `actionReopenClosedTab` (`Cmd+Shift+T` / `Ctrl+Shift+T`), configurable in Shortcut Mapper, integrated with session restore.
- [x] **1.3** Implement `TabColorManager`: per-tab QColor property and tab context menu "Set Tab Color" (8 curated accessible presets: Grey, Blue, Red, Yellow, Green, Pink, Purple, Cyan/Orange).
- [x] **1.4** Implement `AutosaveRecoveryManager`: background timer (default 10s interval) journaling modified buffers and untitled drafts with strict `0600` permissions.
- [x] **1.5** Implement Crash Recovery Dialog on application launch when uncommitted recovery journal records are detected.
- [x] **1.6** Upgrade `ExtendedSearchParser`: comprehensive escape parsing (`\n`, `\r`, `\t`, `\0`, `\b`, `\f`, `\v`, `\\`, `\xHH`, `\dDDD`) in `FindReplaceDialog` and `QuickFindWidget`.
- [x] **1.7** Status bar refinements: live selection length, UTF-8 byte count, encoding, EOL mode, and zoom level.

### Phase 2: Navigation, Workspace & Scintilla Enhancements (Week 1–2)

- [x] **2.1** Optional Vertical Tabs Mode toggle in View menu, implemented as an optional dockable panel using `Qt-Advanced-Docking-System` without altering the default horizontal tab bar.
- [x] **2.2** Implement `ScrollbarNavigationMarkers`: paint visual indicator ticks on the editor scrollbar track for bookmarks, search matches, and unsaved changes.
- [x] **2.3** Implement `CorrespondingFileSwitcher` (`F4` / `Alt+O`): instant toggle between `.h`/`.hpp` and `.cpp`/`.c`/`.cc`/`.m`/`.mm` or `.ts` and `.spec.ts`/`.test.ts`.
- [x] **2.4** Implement `EditorConfigParser`: automatically scan parent directories for `.editorconfig` to enforce `indent_style`, `indent_size`, `tab_width`, `end_of_line`, and `trim_trailing_whitespace`.
- [x] **2.5** Implement `DocumentMinimapDock`: read-only Scintilla viewport overlay with auto-disable protection for documents > 50 MB.
- [x] **2.6** Implement `ClipboardHistoryDock`: monitor `QClipboard`, display the last 20 entries with double-click paste and one-click clear.
- [x] **2.7** Form-Feed (`\f` / `\x0C`) visual rendering: draw an elegant horizontal page boundary divider in Scintilla.

### Phase 3: Lexers, Syntax Highlighting & Language Support (100+ Target) (Week 2)

- [x] **3.1** Audit all Lexilla 5.5.5 lexers against existing `src/languages/*.lua` configurations to ensure 100% lexer coverage.
- [x] **3.2** Enrich keywords, folding regions, and style definitions for modern languages: Rust, Go, Zig, Kotlin, Swift, TypeScript, Dart, Elixir, Crystal.
- [x] **3.3** Enhance systems languages: C/C++ (C23 / C++23 keyword updates), WebAssembly, LLVM IR.
- [x] **3.4** Enhance markup and configuration languages: Markdown with front-matter, TOML, YAML, JSON5, Dockerfile, Terraform.
- [x] **3.5** Enhance specialized developer languages (inspired by Notepad4): AutoIt, AviSynth, CMake, NSIS, PowerShell, SQL dialects.
- [x] **3.6** Verify WCAG AAA theme compatibility: all 100+ language styles verified across all 75 developer themes.
- [x] **3.7** Update language detection heuristics and Preferences > Default Language dropdown selector.
- [x] **3.8** Performance benchmark: verify syntax highlighting causes no slowdown on 100 MB+ files.

### Phase 4: Native Export Pipeline & Developer Tooling Suite (Week 2)

- [x] **4.1** Implement pure Qt 6 MD → HTML exporter: `QTextDocument::setMarkdown()` to standalone HTML with embedded theme CSS.
- [x] **4.2** Implement pure Qt 6 MD → PDF and HTML → PDF exporter via `QPrinter` with page setup dialog (A4, Letter, margins, orientation).
- [x] **4.3** Wire Export actions to File menu and `MarkdownSafePreviewDock` toolbar.
- [x] **4.4** Optional external tool path: detect `pandoc` on PATH for advanced PDF formatting via sandboxed `QProcess`.
- [x] **4.5** Markdown Preview enhancements: lightweight offline Mermaid rendering (flowchart, sequence, class, state) and KaTeX math formulas (`$..$`, `$$..$$`).
- [x] **4.6** Implement Two-Way SyncScroll between active Scintilla scroll ratio and Markdown preview scroll position.
- [x] **4.7** Implement Interactive Checklists (`Cmd+Shift+C`) and smart list continuation on `Enter`.
- [x] **4.8** Implement `DeveloperToolsEngine`: JSON Prettify/Minify with line/col error jumping, URL Encode/Decode, Unicode Escape/Unescape, UUID v4, and Unix Timestamp Converter dialog.
- [x] **4.9** Implement `CharacterInfoPanel`: inspect selected character Unicode code point (`U+XXXX`), name, script, category, and UTF-8/UTF-16 encoding.

### Phase 5: Verification, Benchmarking, Security Hardening & Release Prep (Week 3)

- [x] **5.1** Unit tests for all new modules (`ClosedTabsManager`, `AutosaveRecoveryManager`, `ExtendedSearchParser`, `EditorConfigParser`, `DeveloperToolsEngine`, `MarkdownExport`).
- [x] **5.2** Multi-platform QA: macOS (Apple Silicon ARM64 & Intel x86_64), Ubuntu/Debian, Fedora/RPM, FreeBSD 14.
- [x] **5.3** Memory regression gate: idle RSS verified under 40 MB; zero memory leaks detected under Valgrind/ASan.
- [x] **5.4** Security review: verify `0600` permissions on draft journals; strict URI protocol allowlisting; no subshell command injections.
- [x] **5.5** Full bilingual synchronization: verify 100% completed translations (0 unfinished) in English and Bahasa Indonesia.
- [x] **5.6** Synchronize documentation: `README.md`, `README-id.md`, `doc/DOCNOTE.md`, `doc/CHANGELOG.txt`.
- [x] **5.7** Version bump to `1.4.5` in `CMakeLists.txt`, `src/version.h`, and `updates.json`.
- [x] **5.8** Clean build verification with Unix Makefiles (0 compiler warnings, 0 SonarLint issues).

---

## 6. Detailed Task-by-Task Implementation Plan (TDD-Driven)

```markdown
### Task 1: Closed Tab Stack & Reopen Closed Tab (`Ctrl+Shift+T` / `Cmd+Shift+T`)

**Files:**

- Create: `src/features/ClosedTabsManager.h`
- Create: `src/features/ClosedTabsManager.cpp`
- Modify: `src/EditorManager.h:40-70`
- Modify: `src/EditorManager.cpp:120-160`
- Modify: `src/dialogs/MainWindow.h:90-120`
- Modify: `src/dialogs/MainWindow.cpp:800-840`
- Test: `tests/ClosedTabsManagerTests.cpp`

**Interfaces:**

- Consumes: `EditorManager::closeTab(int index)`, `DockedEditor::filePath()`, `ScintillaNext::currentPosition()`
- Produces: `ClosedTabsManager::pushClosedTab(const ClosedTabInfo &info)`, `ClosedTabsManager::popClosedTab() -> std::optional<ClosedTabInfo>`, `ClosedTabsManager::canReopen() -> bool`

- [x] **Step 1: Write unit test for ClosedTabsManager**
      Write tests in `tests/ClosedTabsManagerTests.cpp` verifying LIFO push/pop of file path, cursor position, scroll offset, and max stack depth (capped at 25 entries).

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R ClosedTabsManagerTests --output-on-failure'`
      Expected: FAIL (class not implemented).

- [x] **Step 3: Implement `ClosedTabsManager`**
      Implement `ClosedTabsManager.h` and `ClosedTabsManager.cpp` tracking closed documents with path, line, column, and timestamp.

- [x] **Step 4: Wire `ClosedTabsManager` into `EditorManager` and `MainWindow`**
      Record closed tab metadata when closing a document; add `actionReopenClosedTab` to File menu and Tab context menu with shortcut `Qt::CTRL | Qt::SHIFT | Qt::Key_T`.

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R ClosedTabsManagerTests --output-on-failure'`
      Expected: PASS.
```

```markdown
### Task 2: Tab Color Tagging & Grouping

**Files:**

- Create: `src/features/TabColorManager.h`
- Create: `src/features/TabColorManager.cpp`
- Modify: `src/widgets/TabsQuickActionsBar.h:20-50`
- Modify: `src/widgets/TabsQuickActionsBar.cpp:80-140`
- Modify: `src/DockedEditor.h:30-60`
- Modify: `src/DockedEditor.cpp:110-150`
- Test: `tests/TabColorManagerTests.cpp`

**Interfaces:**

- Consumes: `QSettings`, `ThemeManager::instance()`
- Produces: `TabColorManager::setTabColor(const QString &filePath, const QColor &color)`, `TabColorManager::tabColor(const QString &filePath) -> QColor`, `TabColorManager::paletteColors() -> QList<QColor>`

- [x] **Step 1: Write unit test for TabColorManager**
      Test setting, querying, persisting in QSettings, and clearing tab colors across 8 color presets (Red, Orange, Yellow, Green, Cyan, Blue, Purple, Magenta).

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R TabColorManagerTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `TabColorManager`**
      Store persistent color mappings by file path and provide standard accessible color swatches.

- [x] **Step 4: Connect Tab Color Menu in TabBar context menu**
      Add "Tab Color" submenu in tab right-click context menu with color circles and "Clear Color" option; paint colored underline or accent tab bar indicator.

- [x] **Step 5: Verify build and tests pass**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R TabColorManagerTests --output-on-failure'`
      Expected: PASS.
```

```markdown
### Task 3: Autosave & Crash Recovery Manager (10s Interval, Strict 0600 Permissions)

**Files:**

- Create: `src/features/AutosaveRecoveryManager.h`
- Create: `src/features/AutosaveRecoveryManager.cpp`
- Create: `src/dialogs/CrashRecoveryDialog.h`
- Create: `src/dialogs/CrashRecoveryDialog.cpp`
- Modify: `src/EditorManager.h:50-80`
- Modify: `src/EditorManager.cpp:200-240`
- Modify: `src/dialogs/MainWindow.cpp:150-190`
- Test: `tests/AutosaveRecoveryManagerTests.cpp`

**Interfaces:**

- Consumes: `EditorManager::allEditors()`, `QSettings`
- Produces: `AutosaveRecoveryManager::start()`, `AutosaveRecoveryManager::saveSnapshotNow()`, `AutosaveRecoveryManager::recoverableSnapshots() -> QList<RecoverySnapshot>`, `AutosaveRecoveryManager::clearSnapshots()`

- [x] **Step 1: Write unit test for AutosaveRecoveryManager**
      Verify journal write with POSIX file permissions `0600`, untitled draft recovery, snapshot cleanup on normal document save/close, and recovery snapshot enumeration.

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R AutosaveRecoveryManagerTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `AutosaveRecoveryManager` and `CrashRecoveryDialog`**
      Implement atomic snapshot writing with temporary files and rename, enforcing `QFile::setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner)`.

- [x] **Step 4: Integrate startup recovery detection in MainWindow**
      Prompt the user on startup if recoverable crash snapshots exist, offering "Recover Selected" or "Discard Drafts".

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R AutosaveRecoveryManagerTests --output-on-failure'`
      Expected: PASS.
```

```markdown
### Task 4: Comprehensive Extended Find/Replace Escape Engine (`\xHH`, `\dDDD`, `\0`, `\b`, `\f`, `\v`)

**Files:**

- Create: `src/utils/ExtendedSearchParser.h`
- Create: `src/utils/ExtendedSearchParser.cpp`
- Modify: `src/dialogs/FindReplaceDialog.cpp:40-60`
- Modify: `src/widgets/QuickFindWidget.h:25-50`
- Modify: `src/widgets/QuickFindWidget.cpp:120-170`
- Test: `tests/ExtendedSearchParserTests.cpp`

**Interfaces:**

- Consumes: `const QString &rawInput`
- Produces: `ExtendedSearchParser::parse(const QString &input, bool *hasError = nullptr) -> QString`

- [x] **Step 1: Write unit test for ExtendedSearchParser**
      Test conversion of `\n`, `\r`, `\t`, `\0`, `\b`, `\f`, `\v`, `\\`, hex byte `\x1B`, `\x41` ('A'), and decimal byte `\d065` ('A'), with proper unescaped backslash preservation.

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R ExtendedSearchParserTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `ExtendedSearchParser`**
      Implement linear single-pass tokenizer in C++20 ensuring safe escape parsing without recursive replacement vulnerabilities.

- [x] **Step 4: Integrate into FindReplaceDialog and QuickFindWidget**
      Replace old naive `str.replace` in `FindReplaceDialog.cpp`; add Extended Mode toggle to `QuickFindWidget`.

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R ExtendedSearchParserTests --output-on-failure'`
      Expected: PASS.
```

```markdown
### Task 5: 100+ Lexers Audit & Priority Modern Language Enhancements

**Files:**

- Audit & Modify: `src/languages/*.lua`
- Create: New `.lua` language definitions for missing Lexilla 5.5.5 languages (Zig, Elixir, Crystal, Cangjie, LLVM IR, WebAssembly, JSON5, Terraform)
- Modify: `src/languages/LanguageManager.cpp:100-180`
- Test: `tests/LanguageLexerCoverageTests.cpp`

**Interfaces:**

- Consumes: Lexilla 5.5.5 lexer IDs, Lua configuration table
- Produces: `LanguageManager::supportedLanguages() -> QList<LanguageInfo>` (100+ count), `LanguageManager::applyLanguageToEditor(ScintillaNext *editor, const QString &langId)`

- [x] **Step 1: Write unit test verifying 100+ active language lexers**
      Assert that `LanguageManager::supportedLanguages().count() >= 100`, all lexer IDs match Lexilla 5.5.5 catalog, and no Lua syntax errors exist across `src/languages/*.lua`.

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R LanguageLexerCoverageTests --output-on-failure'`
      Expected: FAIL (< 100 languages).

- [x] **Step 3: Enrich existing top 40 language definitions and create missing ones**
      Expand keyword lists for modern standards (C++23, C23, Rust 2024 edition, Go 1.24, Swift 6, TypeScript 5.8).

- [x] **Step 4: Verify theme palette mapping across all 75 themes**
      Ensure all syntax tokens (types, keywords, strings, comments, numbers, preprocessor) have high-contrast WCAG AAA mapping.

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R LanguageLexerCoverageTests --output-on-failure'`
      Expected: PASS.
```

```markdown
### Task 6: Native Markdown / HTML / PDF Export Pipeline (Pure Qt 6 & Pandoc Fallback)

**Files:**

- Create: `src/features/DocumentExportManager.h`
- Create: `src/features/DocumentExportManager.cpp`
- Create: `src/dialogs/ExportOptionsDialog.h`
- Create: `src/dialogs/ExportOptionsDialog.cpp`
- Modify: `src/docks/MarkdownSafePreviewDock.h:40-70`
- Modify: `src/docks/MarkdownSafePreviewDock.cpp:200-250`
- Modify: `src/dialogs/MainWindow.cpp:650-690`
- Test: `tests/DocumentExportManagerTests.cpp`

**Interfaces:**

- Consumes: Document buffer (Markdown or HTML)
- Produces: `DocumentExportManager::exportMarkdownToHtml(const QString &md, const QString &outputPath, const ExportOptions &opt) -> bool`, `DocumentExportManager::exportToPdf(const QString &content, bool isMarkdown, const QString &outputPath, const ExportOptions &opt) -> bool`

- [x] **Step 1: Write unit test for DocumentExportManager**
      Verify pure Qt 6 export to standalone HTML with embedded CSS, PDF generation via QPrinter, page margin calculations, and pandoc detection fallback.

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R DocumentExportManagerTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `DocumentExportManager` and `ExportOptionsDialog`**
      Use `QTextDocument::setMarkdown()`, `toHtml()`, and `document->print(&printer)` with `QPrinter::PdfFormat`.

- [x] **Step 4: Add Export actions in File menu and Markdown Preview dock**
      Add actions "Export Markdown to HTML...", "Export Markdown to PDF...", and "Export HTML to PDF...".

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R DocumentExportManagerTests --output-on-failure'`
      Expected: PASS.
```

`````markdown
### Task 7: Markdown Preview Enhancements: Mermaid Diagrams & LaTeX Math Formulas

**Files:**

- Modify: `src/docks/MarkdownSafePreviewDock.h:35-70`
- Modify: `src/docks/MarkdownSafePreviewDock.cpp:180-260`
- Create: `src/utils/MarkdownPreviewEnhancer.h`
- Create: `src/utils/MarkdownPreviewEnhancer.cpp`
- Test: `tests/MarkdownPreviewEnhancerTests.cpp`

**Interfaces:**

- Consumes: Raw Markdown content
- Produces: `MarkdownPreviewEnhancer::processMermaidBlocks(const QString &md) -> QString`, `MarkdownPreviewEnhancer::processMathExpressions(const QString &md) -> QString`

- [x] **Step 1: Write unit test for MarkdownPreviewEnhancer**
      Test extraction and SVG/KaTeX substitution of ````mermaid`blocks and`$inline$`/`$$block$$` formulas with XSS sanitization.

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R MarkdownPreviewEnhancerTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `MarkdownPreviewEnhancer`**
      Integrate bundled offline KaTeX/Mermaid rendering support in `MarkdownSafePreviewDock` with full CSP security restrictions.

- [x] **Step 4: Wire Two-Way SyncScroll**
      Synchronize scroll position between Scintilla active line ratio and Markdown preview scrollbar.

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R MarkdownPreviewEnhancerTests --output-on-failure'`
      Expected: PASS.
`````

```markdown
### Task 8: Developer Tools Suite (JSON Prettify/Minify, URL, Unicode, UUID v4, Timestamp)

**Files:**

- Create: `src/features/DeveloperToolsEngine.h`
- Create: `src/features/DeveloperToolsEngine.cpp`
- Create: `src/dialogs/TimestampConverterDialog.h`
- Create: `src/dialogs/TimestampConverterDialog.cpp`
- Modify: `src/dialogs/MainWindow.h:110-140`
- Modify: `src/dialogs/MainWindow.cpp:1050-1120`
- Modify: `src/dialogs/MainWindow.ui`
- Test: `tests/DeveloperToolsEngineTests.cpp`

**Interfaces:**

- Consumes: Scintilla text selection or full document buffer
- Produces: `DeveloperToolsEngine::formatJson(const QString &json, int indentSpaces, QString *errorMsg) -> QString`, `DeveloperToolsEngine::minifyJson(const QString &json) -> QString`, `DeveloperToolsEngine::urlEncode(const QString &text) -> QString`, `DeveloperToolsEngine::urlDecode(const QString &text) -> QString`, `DeveloperToolsEngine::unicodeEscape(const QString &text) -> QString`, `DeveloperToolsEngine::unicodeUnescape(const QString &text) -> QString`, `DeveloperToolsEngine::generateUuidV4() -> QString`

- [x] **Step 1: Write unit tests for DeveloperToolsEngine**
      Cover JSON format/minify (validating line/column reporting on invalid JSON), URL encode/decode, Unicode `\uXXXX` escape/unescape, UUID v4 regex validation, and timestamp conversion.

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R DeveloperToolsEngineTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `DeveloperToolsEngine` and `TimestampConverterDialog`**
      Build robust in-memory converters using Qt's `QJsonDocument`, `QUrl`, and `QDateTime`.

- [x] **Step 4: Add "Developer Tools" submenu to MainWindow Tools menu**
      Hook actions up to replace active selection (or entire document if no selection) with undo support (`editor->beginUndoAction()`).

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R DeveloperToolsEngineTests --output-on-failure'`
      Expected: PASS.
```

```markdown
### Task 9: Scrollbar Navigation Markers & Corresponding Header/Source Switcher

**Files:**

- Create: `src/features/ScrollbarNavigationMarkers.h`
- Create: `src/features/ScrollbarNavigationMarkers.cpp`
- Create: `src/features/CorrespondingFileSwitcher.h`
- Create: `src/features/CorrespondingFileSwitcher.cpp`
- Modify: `src/ScintillaNext.h:80-110`
- Modify: `src/ScintillaNext.cpp:250-300`
- Modify: `src/dialogs/MainWindow.cpp:950-980`
- Test: `tests/CorrespondingFileSwitcherTests.cpp`

**Interfaces:**

- Consumes: Scintilla bookmarks, search match positions, active file path
- Produces: `CorrespondingFileSwitcher::findCorrespondingPath(const QString &currentPath) -> QString`, `ScrollbarNavigationMarkers::paintMarkers(QPainter *painter, const QRect &scrollbarRect)`

- [x] **Step 1: Write unit test for CorrespondingFileSwitcher**
      Verify matching `.h` <-> `.cpp`/`.c`/`.cc`/`.m`/`.mm` in same directory and adjacent `include/` / `src/` folders, and `.ts` <-> `.spec.ts`/`.test.ts`.

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R CorrespondingFileSwitcherTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `CorrespondingFileSwitcher`**
      Implement fast heuristic filesystem lookup without blocking GUI thread.

- [x] **Step 4: Implement `ScrollbarNavigationMarkers` and connect to Scintilla scroll events**
      Paint subtle colored tick marks for bookmarks (blue) and search matches (orange/yellow) alongside editor vertical scrollbar.

- [x] **Step 5: Add shortcut `F4` / `Alt+O` to MainWindow for Header/Source toggle**
      Wire action to open or switch to corresponding tab. Run tests.
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R CorrespondingFileSwitcherTests --output-on-failure'`
      Expected: PASS.
```

```markdown
### Task 10: Native `.editorconfig` Automatic Parser & Enforcement

**Files:**

- Create: `src/features/EditorConfigParser.h`
- Create: `src/features/EditorConfigParser.cpp`
- Modify: `src/EditorManager.cpp:180-220`
- Modify: `src/ScintillaNext.cpp:320-360`
- Test: `tests/EditorConfigParserTests.cpp`

**Interfaces:**

- Consumes: File path on disk
- Produces: `EditorConfigParser::resolveSettings(const QString &filePath) -> EditorConfigSettings` (indent_style, indent_size, tab_width, end_of_line, trim_trailing_whitespace, insert_final_newline)

- [x] **Step 1: Write unit test for EditorConfigParser**
      Test glob pattern matching (`*.cpp`, `*.{js,ts}`, `Makefile`), section inheritance, and `root = true` boundary stopping.

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R EditorConfigParserTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `EditorConfigParser`**
      Lightweight INI/glob parser traversing parent directories up to root.

- [x] **Step 4: Hook into `EditorManager::openFile`**
      Automatically apply indent style (Tabs vs Spaces), tab width, and EOL mode to Scintilla editor without overriding explicit user manual toggles.

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R EditorConfigParserTests --output-on-failure'`
      Expected: PASS.
```

```markdown
### Task 11: Clipboard History Dock & Character Info Panel

**Files:**

- Create: `src/docks/ClipboardHistoryDock.h`
- Create: `src/docks/ClipboardHistoryDock.cpp`
- Create: `src/docks/CharacterInfoDock.h`
- Create: `src/docks/CharacterInfoDock.cpp`
- Modify: `src/dialogs/MainWindow.h:130-160`
- Modify: `src/dialogs/MainWindow.cpp:1150-1200`
- Test: `tests/ClipboardHistoryDockTests.cpp`

**Interfaces:**

- Consumes: `QClipboard::dataChanged()`, active selection in `ScintillaNext`
- Produces: `ClipboardHistoryDock::entries() -> QStringList`, `CharacterInfoDock::updateCharacter(uint32_t codePoint)`

- [x] **Step 1: Write unit test for ClipboardHistoryDock storage**
      Verify FIFO queue of 20 items, deduplication of consecutive identical clips, and manual clear functionality.

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R ClipboardHistoryDockTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `ClipboardHistoryDock` and `CharacterInfoDock`**
      Connect `QClipboard` signals and update Unicode inspector on cursor position changes.

- [x] **Step 4: Register docks in MainWindow View menu**
      Allow user to toggle and dock panels alongside ADS layout.

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R ClipboardHistoryDockTests --output-on-failure'`
      Expected: PASS.
```

```markdown
### Task 12: Fuzzy Quick Switcher (`Cmd+P`) & Scintilla Document Minimap Dock

**Files:**

- Create: `src/dialogs/QuickSwitcherDialog.h`
- Create: `src/dialogs/QuickSwitcherDialog.cpp`
- Create: `src/docks/DocumentMinimapDock.h`
- Create: `src/docks/DocumentMinimapDock.cpp`
- Modify: `src/features/MacServicesHandler.mm:40-90`
- Modify: `src/dialogs/MainWindow.cpp:1250-1300`
- Test: `tests/QuickSwitcherTests.cpp`

**Interfaces:**

- Consumes: `EditorManager::allEditors()`, `ScintillaNext` document pointer (`SCI_GETDOCPOINTER`)
- Produces: `QuickSwitcherDialog::execFuzzySearch(const QString &query)`, `DocumentMinimapDock::syncWithEditor(ScintillaNext *editor)`, `MacServicesHandler::updateDockRecentMenu(const QStringList &recentFiles)`

- [x] **Step 1: Write unit test for QuickSwitcher filtering logic**
      Test subsequence fuzzy scoring algorithm (e.g. `mnw` matching `MainWindow.cpp`, scoring path vs filename).

- [x] **Step 2: Run test to verify it fails**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R QuickSwitcherTests --output-on-failure'`
      Expected: FAIL.

- [x] **Step 3: Implement `QuickSwitcherDialog` and `DocumentMinimapDock`**
      Build frameless, spotlight-style quick switcher with dark/light theme support, and lightweight Scintilla minimap with visible viewport tracker overlay (auto-disabled > 50 MB).

- [x] **Step 4: Wire `Cmd+P` / `Ctrl+P` and macOS Dock Recent Files**
      Register global shortcut in MainWindow and populate native Cocoa `NSApp.dockTile` context menu.

- [x] **Step 5: Run tests and verify exit code 0**
      Run: `/usr/local/bin/bash -l -c 'ctest --test-dir build -R QuickSwitcherTests --output-on-failure'`
      Expected: PASS.
```

---

## 7. Comprehensive Verification, Security & Quality Gate Matrix

| Verification Pillar        | Tool / Command                                            | Target Threshold                    | Scope                                         |
| :------------------------- | :-------------------------------------------------------- | :---------------------------------- | :-------------------------------------------- |
| **C++20 Syntax & Build**   | `cmake --build build -j2` (CMake / Makefiles only)        | 0 compilation errors, 0 warnings    | All modified source and header files          |
| **Static Analysis**        | `/usr/local/bin/cppcheck --enable=all --error-exitcode=1` | 0 errors                            | `src/features/`, `src/utils/`, `src/dialogs/` |
| **Clang-Tidy**             | `/usr/local/opt/llvm/bin/clang-tidy -p build/`            | 0 SonarLint Blocker/Critical issues | Newly added C++20 classes                     |
| **Unit Testing**           | `ctest --test-dir build --output-on-failure`              | 100% Passing (0 failures)           | All regression & new unit tests               |
| **Memory Footprint**       | `ps -o rss -p <PID>`                                      | Idle < 40 MB RSS                    | SparkNotePad runtime verification             |
| **File Permissions**       | `stat -f %A ~/.sparknotepad/recovery/*`                   | `0600` strictly                     | Autosave journal drafts                       |
| **Backward Compatibility** | `QSettings` diff check                                    | 100% backward compatible            | Existing config keys preserved                |

---

## 8. Version Synchronization & Release Checklist for v1.4.5

- [x] Update `CMakeLists.txt` project version from `1.4.0` to `1.4.5`.
- [x] Update `src/version.h` / `APP_VERSION` to `"1.4.5"`.
- [x] Update documentation files (`README.md`, `README-id.md`, `doc/DOCNOTE.md`, `doc/CHANGELOG.txt`).
- [x] Synchronize `translations/` `.ts` files for new UI strings (0 unfinished translations).
- [x] Verify macOS codesign & DMG packaging script execution.

---

## 9. Mandatory Post-Completion Verification & Comprehensive 14-Pillar Code Review Protocol

> **CRITICAL EXECUTION MANDATE FOR AGENTIC WORKERS:**  
> Upon completing implementation tasks, the following verification commands and exhaustive multi-axis code reviews MUST be executed in full before declaring completion. If ANY error, diagnostic failure, or vulnerability is found, fix it immediately on the spot without waiting for further instructions.  
> **BE CAREFUL AND PAY CLOSE ATTENTION: THIS REPOSITORY IS `SparkNotePad`, NOT `NotepadNext-MacOS`.**

### 9.1 Static Analysis, Diagnostic Checks & Compilation Verification

Execute the comprehensive toolchain checks across all modified and newly created C++ files:

- [x] **Clangd In-Depth Diagnostic Check:**
      Run `clangd --check` across all files in `/Users/alsyundawy/Downloads/GitHub/SparkNotePad/src/*.cpp` and `/Users/alsyundawy/Downloads/GitHub/SparkNotePad/src/*/*.cpp`:

  ```bash
  for f in src/*.cpp src/*/*.cpp; do
    echo "Checking $f with clangd..."
    /usr/local/opt/llvm/bin/clangd --check="$f" --compile-commands-dir=build 2>&1 | grep -E "error:|warning:" || true
  done
  ```

- [x] **Cppcheck Deep Static Analysis:**
      Run cppcheck with zero-error exit code enforcement:

  ```bash
  /usr/local/bin/cppcheck --enable=all --inconclusive --error-exitcode=1 -I src -I src/features -I src/widgets -I src/dialogs -I src/docks -I src/utils src/
  ```

- [x] **SonarLint & Clang-Tidy Quality Gate:**
      Run Clang-Tidy with compile commands:

  ```bash
  /usr/local/opt/llvm/bin/clang-tidy -p build src/*.cpp src/*/*.cpp
  ```

  Ensure 0 `BLOCKER`, 0 `CRITICAL`, and 0 `MAJOR` SonarLint diagnostic issues remain.

- [x] **Universal Fast Linter Script Check:**
      Run the automated linter script on all modified files:

  ```bash
  /usr/local/bin/bash ~/.gemini/config/skills/ide-linter-error-fix/scripts/lint-check.sh <modified-files>
  ```

### 9.2 Comprehensive 14-Pillar Code Review Checklist

Conduct a final, thorough investigation and search using web tools; perform a code review—using this checklist—of every single file within this folder and repository, including those located at `/Users/alsyundawy/Downloads/GitHub/SparkNotePad/src`:

- [x] **1. BUG REVIEW:** Investigate edge cases, boundary conditions, off-by-one errors, null pointer dereferences, unhandled exceptions, and unexpected return paths across all touched functions.
- [x] **2. SYNTAX REVIEW:** Enforce strict ISO C++20 compliance, verified with Clang 23 and GCC 16, ensuring zero compiler warnings (`-Wall -Wextra -Wpedantic`).
- [x] **3. RUNTIME REVIEW:** Inspect event loop behavior, Qt signal-slot bindings, thread safety, and UI responsiveness during heavy operations or document loading.
- [x] **4. LOGIC REVIEW:** Verify that the algorithmic design and data flow strictly match the specification without inverted conditions, dead branches, or unintended side effects.
- [x] **5. MEMORY REVIEW:** Inspect resource ownership semantics (RAII, `std::unique_ptr`, `QObject` parent-child memory trees), preventing memory leaks, double frees, and use-after-free conditions.
- [x] **6. DEAD CODE REVIEW:** Identify and eliminate unused functions, unreachable code blocks, leftover debug statements, and redundant commented-out snippets.
- [x] **7. DUPLICATE CODE REVIEW:** Refactor duplicate logic, repeated conversion functions, or copy-pasted UI routines into shared utility helpers in `src/utils/`.
- [x] **8. CIRCULAR DEPENDENCY REVIEW:** Ensure strict acyclic include graphs between `src/features/`, `src/dialogs/`, `src/widgets/`, `src/docks/`, and root headers; use forward declarations where appropriate.
- [x] **9. PERFORMANCE BOTTLENECK REVIEW:** Profile critical paths, eliminate redundant string copies (`QStringView`, `const QString &`), optimize Scintilla buffer calls, and guarantee idle memory stays < 40 MB RSS.
- [x] **10. SECURITY / CWE / CVE VULNERABILITY REVIEW:** Audit against OWASP Top 10, CWE-78 (Command Injection), CWE-22 (Path Traversal), CWE-1333 (ReDoS), and CVE-2026-20841 (Markdown URI Protocol Injection); verify `0600` permissions on autosave drafts.
- [x] **11. MAINTAINABILITY REVIEW:** Preserve code clarity, self-documenting naming conventions, clean separation of concerns, and documentation integrity.
- [x] **12. SCALABILITY REVIEW:** Verify that large files (> 50 MB, > 100k lines) open smoothly without UI lockups; ensure auto-disable on minimap and heavy preview renders.
- [x] **13. READABILITY REVIEW:** Check consistent indentation (matching existing 4 spaces), idiomatic Qt/C++ conventions, clear variable names, and cohesive class structures.
- [x] **14. LINTER REVIEW:** Run all ecosystem linters (`markdownlint`, `shellcheck`, `cppcheck`, `clang-format`, SonarLint) ensuring 0 errors across the entire codebase.
