# SparkNotePad v1.4.0 Master Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use `superpowers:subagent-driven-development` (recommended) or `superpowers:executing-plans` to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Deliver a production-grade, highly secure, performance-optimized upgrade for SparkNotePad v1.4.0 incorporating verified innovations from Nextpad++ (macOS & Linux) and upstream Scintilla 5.6.9 / Lexilla 5.5.5, while hardening against all identified 2025–2026 text editor vulnerabilities across 14 engineering review pillars.

**Architecture:**

- **Engine Layer:** Activate Scintilla 5.6.9 native Change History markers (`SCI_SETCHANGEHISTORY`) for visual tracking of modified, saved, and reverted lines; expose fine-grained line spacing controls (`SCI_SETEXTRAASCENT` and `SCI_SETEXTRADESCENT`).
- **macOS & Linux Desktop Integration Layer:** Integrate native macOS document title-bar proxy icons via `QMainWindow::setWindowFilePath` allowing drag-and-drop to Terminal/Finder and ⌘-click directory trail popups; implement rich tab tooltips with full file path, character/line counts, encoding, EOL format, and modification timestamps.
- **Search & Automation Layer:** Add an opt-in Fuzzy Subsequence Search mode in `QuickFindWidget` and `FindReplaceDialog`; expand `MacroRunDialog` with batch execution capabilities ("Run Macro on Open Files / Folder").
- **Diagnostics & Interoperability Layer:** Introduce "Help → Copy Debug Information" for reproducible issue reporting; establish a loopback-only (`127.0.0.1`), token-authenticated, read-only Model Context Protocol (MCP) server for local AI assistant pairing.
- **Security Hardening Layer:** Enforce strict URI protocol allowlists against CVE-2026-20841, recursion-depth-capped XML/JSON parsing against CVE-2026-16762, and non-shell process spawning against CVE-2026-25926.

**Tech Stack:** C++20, Qt 6.5–6.12 (QtWidgets, QtGui, QtCore, QtSvg), Scintilla 5.6.9, Lexilla 5.5.5, Lua 5.5.1, Qt Advanced Docking System (ADS 5.1.1), CMake 3.21+ (standard Unix Makefiles, STRICTLY NO NINJA).

**Spec:** Direct user requirements and architectural decisions derived from comprehensive research on Nextpad++, Notepad++, Notepad--, Notepad2/3/4, Scintilla 5.6.9, Lexilla 5.5.5, Lua 5.5.1, and 2025–2026 CVE threat models.

---

## Global Constraints

- **STRICT PROHIBITION OF NINJA**: As mandated by `AGENTS.md`, never run `ninja`, `-G Ninja`, or `cmake-ninja`. All builds must use standard CMake Unix Makefiles (`cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j2`).
- **SHELL EXECUTION**: All subshells must execute via `/usr/local/bin/bash -l` to source bash profiles correctly.
- **NON-DESTRUCTIVE ARCHITECTURE**: All new features must be additive and opt-in or strictly backward-compatible. Existing keybindings, theme palettes, custom icon packs, and editor behaviors must remain unaffected.
- **LINTER & SONARLINT COMPLIANCE**: Zero syntax errors, zero compiler warnings, zero SonarLint BLOCKER/CRITICAL/MAJOR issues on all touched files. Verify with `lint-check.sh` and restart SonarLint LS via `sonar-restart-ext.sh`.
- **EVIDENCE BEFORE ASSERTIONS**: Never claim tasks or tests are passing without running verification commands and inspecting output.

---

## Review Focus (14 Pillars & Security Focus)

1. **CVE-2026-20841 (Markdown URI Scheme Injection)**: Ensure all clickable links in editor and Markdown preview reject dangerous protocol handlers (`file://` execution of binaries, `cmd:`, `powershell:`, `ms-office:`), enforcing strict allowlists (`http`, `https`, or validated local paths with explicit user confirmation).
2. **CVE-2026-16762 (QXmlStreamReader & XML DoS)**: Cap XML/JSON nesting depth and file size in `ConfigIntegrityGuard` and session loaders to prevent CPU exhaustion.
3. **CVE-2026-25926 (Command Injection in Tool/File Launching)**: Never pass unescaped string concatenations to system shells. Ensure `ExternalToolManager` uses argument lists with `QProcess`.
4. **Change History Margin Conflict**: Ensure `SCI_SETCHANGEHISTORY` markers do not overwrite breakpoint, bookmark, or folding margins (allocate distinct margin indices).
5. **Memory & Concurrency Bounds**: Ensure batch macro execution runs on background worker threads with cancellation checks (`QAtomicInt` / `isInterruptionRequested()`), never blocking the UI event loop.

---

## Task Breakdown

### Phase 1: Scintilla 5.6.9 Change History & Line Spacing Integration

- [x] **Task 1.1: ScintillaNext Change History Engine Hooks**
  - **Files:** `src/ScintillaNext.h`, `src/ScintillaNext.cpp`
  - **Interfaces:** `void setChangeHistoryEnabled(bool enabled)`, `bool isChangeHistoryEnabled() const`
  - Configure Scintilla change history (`SCI_SETCHANGEHISTORY`, `SC_CHANGE_HISTORY_RECORD | SC_CHANGE_HISTORY_INDICATORS`) in dedicated margin 4 without disturbing bookmarks (margin 1) or folding (margin 2).
  - Add configurable styling for modified (orange), saved (green), and reverted markers.

- [x] **Task 1.2: Fine-Grained Line Spacing (Extra Ascent & Descent)**
  - **Files:** `src/ScintillaNext.h`, `src/ScintillaNext.cpp`, `src/ApplicationSettings.h`, `src/ApplicationSettings.cpp`
  - **Interfaces:** `void setExtraLineSpacing(int extraAscent, int extraDescent)`
  - Map settings to `SCI_SETEXTRAASCENT` and `SCI_SETEXTRADESCENT` with range validation [0..30] pixels.

- [x] **Task 1.3: Preferences UI for Change History and Line Spacing**
  - **Files:** `src/dialogs/PreferencesDialog.ui`, `src/dialogs/PreferencesDialog.cpp`
  - Add "Change History Margin" toggle and "Extra Ascent / Descent" spinboxes under Editor Settings.

### Phase 2: macOS Title-Bar Proxy Icon & Enriched Tab Tooltips

- [x] **Task 2.1: Native macOS Window File Path Proxy Icon**
  - **Files:** `src/MainWindow.cpp`, `src/SparkNotePadApplication.cpp`
  - When an editor tab gains focus, invoke `QMainWindow::setWindowFilePath(activeFilePath)` (or clear when untracked/untitled).
  - Enables macOS native document icon drag-and-drop to Terminal/Finder and ⌘-click directory breadcrumbs.

- [x] **Task 2.2: Enriched Tab Tooltips & Live Status Metrics**
  - **Files:** `src/DockedEditor.cpp`, `src/DockedEditor.h`
  - Formulate detailed HTML tooltips on tab hover:
    - Saved files: Full file path, file size, line count, character count, encoding, line ending format (CRLF/LF), and last modified timestamp.
    - Unsaved files: Document name, unsaved status indicator `[Modified]`, unsaved character delta, and time elapsed since last modification.

### Phase 3: Search Enhancements (Fuzzy Matching) & Batch Automation

- [x] **Task 3.1: Fuzzy Subsequence Search Matcher**
  - **Files:** `src/utils/RegexSafetyEngine.h`, `src/utils/RegexSafetyEngine.cpp`, `src/Finder.h`, `src/Finder.cpp`
  - **Interfaces:** `static QString buildFuzzySubsequencePattern(const QString &query)`
  - Convert query (e.g. `fnpars`) into a safe, non-backtracking regex subsequence (`f[^f]*?n[^n]*?p...`) with bounded lookahead to avoid ReDoS (CWE-1333).
  - Add "Fuzzy Match" checkbox in `FindReplaceDialog.ui` and `QuickFindWidget.ui`.

- [x] **Task 3.2: Batch Macro Processing ("Run Macro on Open Files / Folder")**
  - **Files:** `src/dialogs/MacroRunDialog.ui`, `src/dialogs/MacroRunDialog.h`, `src/dialogs/MacroRunDialog.cpp`
  - Add "Scope" selection: `Current Document`, `All Open Documents`, `Files in Folder`.
  - Include file filter (`*.cpp;*.h;*.txt`), recursive traversal toggle, dry-run preview, and progress bar with instant cancellation (Esc key / Cancel button).

### Phase 4: Diagnostic Transparency & Local MCP Tool Server (Read-Only)

- [x] **Task 4.1: Help → Copy Debug Information to Clipboard**
  - **Files:** `src/MainWindow.cpp`, `src/MainWindow.h`
  - Generate comprehensive diagnostic block: SparkNotePad version, Git commit hash, Qt version, OS version/architecture, active theme, active icon pack, compiler info, screen geometry, Scintilla/Lexilla/Lua versions, and loaded plugins.
  - Add menu action `actionCopyDebugInfo` in `Help` menu with confirmation toast indicator.

- [x] **Task 4.2: Local Read-Only Model Context Protocol (MCP) Server Interface**
  - **Files:** `src/features/LocalMcpServer.h`, `src/features/LocalMcpServer.cpp`
  - **Interfaces:** `void startServer(quint16 port = 0)`, `void stopServer()`
  - Implement a strictly loopback (`127.0.0.1`), token-authenticated TCP/WebSocket endpoint compliant with the Model Context Protocol (2026 spec).
  - Expose read-only tools: `get_open_tabs`, `read_active_tab`, `get_editor_selection`, `get_document_statistics`.
  - Strictly fail-closed: external network requests rejected; write/edit operations disabled unless explicitly confirmed by user via GUI dialog.

### Phase 5: Security Hardening & 2025–2026 CVE Mitigations

- [x] **Task 5.1: Strict URI Scheme Validator (CVE-2026-20841 Mitigation)**
  - **Files:** `src/utils/NetworkSecurityPolicy.h`, `src/utils/NetworkSecurityPolicy.cpp`
  - Validate all links in `MarkdownSafePreviewDock`, `PreviewTabWidget`, and clickable editor URLs.
  - Allowlist strictly `http://`, `https://`, and `mailto:`. Intercept `file://` or custom URI schemes and verify target path sits within safe directory bounds; prompt user with security confirmation dialog before opening external system handlers.

- [x] **Task 5.2: XML & JSON Parser Depth Limiter (CVE-2026-16762 Mitigation)**
  - **Files:** `src/utils/ConfigIntegrityGuard.h`, `src/utils/ConfigIntegrityGuard.cpp`
  - Enforce maximum recursion depth (32 levels) and maximum entity expansion limits on all `QXmlStreamReader` and `QJsonDocument` routines.

- [x] **Task 5.3: Safe Process Argument Quoting (CVE-2026-25926 Mitigation)**
  - **Files:** `src/utils/ExternalToolManager.cpp`, `src/dialogs/RunDialog.cpp`
  - Ensure all external tool invocations invoke `QProcess::start(program, arguments)` directly rather than passing concatenated commands to `/bin/sh -c` or `cmd.exe /c`.

### Phase 6: i18n Translation & UI Text Synchronization

- [x] **Task 6.1: String Extraction via `lupdate`**
  - Run `lupdate src/ -ts i18n/SparkNotePad_en.ts i18n/SparkNotePad_id.ts`.
- [x] **Task 6.2: Complete English and Indonesian Translations**
  - Ensure 100% translation coverage for new dialogs, settings, tooltips, and debug actions; verify `grep -c 'type="unfinished"'` returns 0.
- [x] **Task 6.3: Binary Catalog Compilation via `lrelease`**
  - Compile `.qm` files and verify resource bundles.

### Phase 7: Static Analysis, 14-Pillar Quality Audit & IDE Linter Clean

- [x] **Task 7.1: Automated Syntax & Static Analysis**
  - Run `cppcheck --enable=all`, `clangd --check`, and SonarLint language server restart (`sonar-restart-ext.sh`).
- [x] **Task 7.2: Atomic Diagnostic Remediation**
  - Fix any detected issues (const correctness, null checks, integer promotions, SonarLint rules).
- [x] **Task 7.3: Comprehensive 14-Pillar Audit Sign-off**
  - Verify zero defects across Bug, Syntax, Runtime, Logic, Memory, Dead Code, Duplicate Code, Circular Dependency, Performance, Security, Maintainability, Scalability, Readability, and Architecture pillars.

### Phase 8: Build Verification, Unit Testing & Documentation Rebuild

- [x] **Task 8.1: Controlled CMake Build (Unix Makefiles, No Ninja)**
  - Execute `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j2`.
- [x] **Task 8.2: Unit Test Suite Execution**
  - Run `ctest --test-dir build --output-on-failure` or `./build/tests/SparkNotePadTests`.
- [x] **Task 8.3: Local macOS DMG Packaging & Notarization Pre-Check**
  - Build DMG via `cmake --build build --target dmg -j2` and verify via `hdiutil verify`.
- [x] **Task 8.4: Documentation & Release Artifacts Synchronization**
  - Update `CMakeLists.txt` to version `1.4.0`.
  - Update `README.md`, `README-id.md`, `doc/CHANGELOG.txt`, `doc/DOCNOTE.txt`, `wiki/*.md`.
