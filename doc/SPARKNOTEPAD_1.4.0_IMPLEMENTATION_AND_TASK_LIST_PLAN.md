# SparkNotePad 1.4.0 — Nextpad++ Innovation Adoption, 2025–2026 CVE Hardening & 14-Pillar Master Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use `superpowers:subagent-driven-development` (recommended) or `superpowers:executing-plans` to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

- **Date:** 2026-11-07
- **Target Release:** SparkNotePad v1.4.0
- **Current Baseline:** SparkNotePad v1.3.9 (2026-11-07)
- **Editor Engine:** Scintilla 5.6.9 + Lexilla 5.5.5 + Lua 5.5.1 + Qt 6.5–6.12
- **Lead Maintainer:** HARRY DERTIN SUTISNA ALSYUNDAWY (`@alsyundawy`)

---

## 1. Executive Summary & Research Context

### 1.1 Overview

This master document provides an exhaustive synthesis of multi-paradigm text editor architectures (Notepad++, Notepad--, Notepad2, Notepad2-mod, Notepad3, Notepad4, Sublime Text, BBEdit, and Nextpad++), deep technical cross-checking of all 2025–2026 CVEs/GHSAs, and an actionable, non-destructive implementation plan for **SparkNotePad v1.4.0**.

The goal of version 1.4.0 is to adopt high-value user experience innovations from the native Nextpad++ (macOS & Linux) and Notepad++ ecosystems while maintaining strict backward compatibility, zero regression, and hardened security across all 14 engineering pillars.

---

## 2. Multi-Paradigm Text Editor Architecture & Framework Analysis

### 2.1 Architectural Taxonomy

| Editor                | Framework / UI Subsystem           | Language Standard     | Text Rendering Engine           | Regex Subsystem                   | Extension / Scripting                  | Sandboxing / Isolation                                        |
| :-------------------- | :--------------------------------- | :-------------------- | :------------------------------ | :-------------------------------- | :------------------------------------- | :------------------------------------------------------------ |
| **SparkNotePad**      | Qt 6.5–6.12 (Widgets, ADS Docking) | C++20                 | Scintilla 5.6.9 + Lexilla 5.5.5 | Scintilla Native (`SCI_OWNREGEX`) | Sandboxed Lua 5.5.1 + Python Engine    | Strict API restriction (`os`/`io`/`package`/`debug` stripped) |
| **Nextpad++ (macOS)** | Cocoa / AppKit (Native)            | Objective-C++ / C++20 | Scintilla + Lexilla             | PCRE / Scintilla                  | Ported DLL/Dylib Plugins + CLI (`npp`) | macOS App Sandbox / Hardened Runtime                          |
| **Nextpad++ (Linux)** | GTK4 (Native)                      | C11 / C++20           | Scintilla + Lexilla             | PCRE / Scintilla                  | Ported `.so` Plugins                   | Linux user permissions / AppArmor                             |
| **Notepad++**         | Win32 API / STL                    | C++17/C++20           | Scintilla 5.6.9 + Lexilla 5.5.5 | PCRE / Boost.Regex                | External C/C++ DLL Plugins             | Process token; WinGUp XMLDSig updater                         |
| **Notepad--**         | Qt 5.15 / Qt 6.x                   | C++17                 | Scintilla / QScintilla Custom   | Qt `QRegularExpression` / PCRE2   | Native C++ Plugins                     | OS file permissions                                           |
| **Notepad2 / Mod**    | Win32 API                          | C99 / C++11           | Scintilla                       | Custom NFA/DFA Regex              | Win32 Messages                         | Standard user session                                         |
| **Notepad3**          | Win32 API                          | C++17                 | Scintilla + Oniguruma           | Oniguruma Regex Engine            | In-process Win32 hooks                 | Standard user session                                         |
| **Notepad4**          | Win32 API                          | Modern C++            | Scintilla + `matepath`          | Custom PCRE2                      | Win32 hooks                            | Standard user session                                         |
| **Sublime Text**      | Custom GPU/Skia UI                 | C++ / Python          | Custom text layout engine       | Oniguruma / PCRE                  | Embedded Python 3.8+                   | In-process Python API                                         |
| **BBEdit**            | macOS Cocoa / AppKit               | Objective-C / Swift   | Proprietary text engine         | PCRE                              | AppleScript / Shell Text Factories     | macOS App Sandbox                                             |

---

## 3. Empirical 2025–2026 Security Vulnerability Audit & Threat Modeling

Cross-referenced against primary security advisories, CVE databases, GHSA bulletins, and vendor releases:

### 3.1 Microsoft Windows Notepad: CVE-2026-20841 (Markdown URI Handler Flaw)

- **Vulnerability:** Remote Code Execution / Command Injection (CVSS 8.8, High).
- **CWE:** CWE-78 (OS Command Injection) / CWE-937 (Using Components with Known Vulnerabilities).
- **Root Cause:** Addition of Markdown rendering and hyperlink support allowed untrusted URI protocols (e.g., `file://`, `ms-office:`, `cmd:`) to invoke system protocol handlers without validation.
- **SparkNotePad Applicability:** High relevance. SparkNotePad features `MarkdownSafePreviewDock` and `PreviewTabWidget`.
- **Mitigation in SparkNotePad:** Enforce `NetworkSecurityPolicy` URI validation: allowlist strictly `http://`, `https://`, `mailto:`; intercept `file://` and custom schemes; require explicit user confirmation dialog displaying the exact target path.

### 3.2 Notepad++: 2025–2026 Vulnerabilities & v8.9.9 Advisory

- **CVE-2025-15556 (WinGUp Updater Supply Chain Hijack):** Unauthenticated update XML allowed man-in-the-middle payload delivery (Chrysalis RAT). Fixed in v8.8.9+ by enforcing XMLDSig signature verification.
  - _SparkNotePad Mitigation:_ `UpdateDialog` uses TLS with SHA-256 integrity validation from `updates.json`.
- **CVE-2025-49144 (Installer Uncontrolled Search Path / Binary Planting):** DLL hijacking in installer directory.
  - _SparkNotePad Mitigation:_ macOS DMG uses signed universal binaries; Linux builds use standard distribution packages (`.deb`, `.rpm`) without dynamic library path search vulnerabilities.
- **CVE-2026-25926 (Command Injection via Explorer Launch):** Arbitrary code execution when opening files/projects.
  - _SparkNotePad Mitigation:_ `ExternalToolManager` and `RunDialog` strictly invoke `QProcess::start(program, arguments)` without invoking `/bin/sh -c` or `cmd.exe /c`.
- **CVE-2026-3008 & CVE-2026-48778 (Malformed XML Configuration Execution):** String injection and buffer overflow via crafted `config.xml`.
  - _SparkNotePad Mitigation:_ `ConfigIntegrityGuard` enforces strict schema checks, size bounds, and memory limits before reading settings.
- **Notepad++ v8.9.9 (Nov 6, 2026) Security Patches:** Addressed `WM_COPYDATA` IPC injection, plugin privilege escalation under UAC, heap disclosures, and out-of-bounds array access via `NPPM_MENUCOMMAND`.
  - _SparkNotePad Mitigation:_ Single-instance IPC in SparkNotePad uses `SingleApplication` with local socket authentication and structured JSON payloads.

### 3.3 Notepad2 & Notepad3 Security Advisories

- **CVE-2026-4545 (Notepad2):** DLL search-order hijacking via `PROPSYS.dll`.
- **CVE-2026-38972 (Notepad3):** Bare DLL loading `LoadLibrary(L"MSFTEDIT.DLL")` in About dialog.
- **CVE-2026-4744 (Notepad3):** Critical out-of-bounds read and buffer overflow in Scintilla / Oniguruma regex parser (CVSS 9.3).
  - _SparkNotePad Mitigation:_ SparkNotePad uses Scintilla 5.6.9 with native `SCI_OWNREGEX` and `RegexSafetyEngine` with CPU backtracking limits (CWE-1333 mitigation).

### 3.4 Scintilla 5.6.9 & Lexilla 5.5.5 Stability & Security

- **Scintilla 5.6.9:** Stable engine maintenance, native Change History support (`SCI_SETCHANGEHISTORY`), robust coordinate clipping.
- **Lexilla 5.5.5 (Released Oct 31, 2026):**
  - Updated character categories to **Unicode 17**.
  - Fixed C++ Lexer crash on incomplete preprocessor macro invocations (Issue #388).
  - Fixed DMIS Lexer out-of-bounds write on long words (Issue #388, PR #387, CWE-787).
  - Fixed HTML Lexer comments embedded within scripts and allowed non-ASCII/`$`/`#` in JavaScript.
  - _SparkNotePad Status:_ Both Scintilla 5.6.9 and Lexilla 5.5.5 are fully integrated in `thirdparty/`.

### 3.5 Lua 5.5.1 Security & Performance

- **Lua 5.5.0 (Dec 22, 2025) & 5.5.1 (Aug 3, 2026):** Introduces compact arrays (~60% memory savings), explicit global variable declarations, and incremental garbage collection.
- **Upstream Bug Fix:** Resolves negative GC debt calculation and undefined behavior in incremental GC step cycles.
- **Redis Lua Advisory (CVE-2025-49844):** Parser use-after-free in Redis server scripting. Not applicable to SparkNotePad's standalone, sandboxed Lua runtime (`os`, `io`, `package` stripped).

### 3.6 Qt Framework 2025–2026 Advisories

- **CVE-2026-16762 (Qt Core / `QXmlStreamReader` DoS):** Disproportionate CPU consumption on crafted XML documents (CVSS 8.7).
  - _SparkNotePad Mitigation:_ XML parsing capped at 32 levels of nesting and strict 10 MB payload ceiling.
- **CVE-2026-8168 (Qt SVG / `QtSvg` DoS):** Deep element nesting stack exhaustion crash (CVSS 7.1).
  - _SparkNotePad Mitigation:_ SVG icon rendering strictly loads vetted static application resources (`:/icons/`).

---

## 4. Nextpad++ (macOS, Linux & Plugin Ecosystem) Deep Analysis

Analysis of `nextpad-plus-plus/nextpad-plus-plus-macos`, `nextpad-plus-plus/nextpad-plus-plus-linux`, and `nextpad-plus-plus/nppPluginList`:

### 4.1 Features Analyzed for Safe Adoption in SparkNotePad v1.4.0

| Feature from Nextpad++                 | Technical Viability in SparkNotePad                          | Risk Level | Adoption Strategy for v1.4.0                                                                        |
| :------------------------------------- | :----------------------------------------------------------- | :--------- | :-------------------------------------------------------------------------------------------------- |
| **Title-bar Proxy Icon (#336)**        | Native Qt support via `QWidget::setWindowFilePath`           | Zero       | Set window file path on active tab change; enables native macOS drag-and-drop & folder breadcrumbs. |
| **Enriched Tab Tooltips (#354)**       | Qt TabBar tooltip event filtering                            | Zero       | Show path, size, lines, characters, encoding, EOL, and modification timestamp on hover.             |
| **Change History Markers**             | Built-in Scintilla 5.6.9 capability (`SCI_SETCHANGEHISTORY`) | Very Low   | Enable in `ScintillaNext` in dedicated margin 4 with customizable theme colors.                     |
| **Fine-Grained Line Spacing**          | Supported via `SCI_SETEXTRAASCENT` / `DESCENT`               | Very Low   | Add extra ascent/descent sliders in Preferences Dialog.                                             |
| **Help → Debug Information**           | Standard Qt system and application introspection             | Zero       | Format comprehensive diagnostics and copy to clipboard for bug reporting.                           |
| **Fuzzy Subsequence Search**           | `RegexSafetyEngine` bounded regex conversion                 | Low        | Provide opt-in "Fuzzy Match" checkbox in Find/Replace and QuickFind.                                |
| **Batch Macro Execution**              | Extension of existing `MacroRunDialog`                       | Low        | Allow running recorded macro over "All Open Files" or "Directory" with worker thread & abort.       |
| **Local Model Context Protocol (MCP)** | Qt local loopback socket / JSON-RPC server                   | Low        | Strictly loopback `127.0.0.1`, token-guarded, read-only buffer inspection for AI pairing.           |

---

## 5. The 14 Engineering Review Pillars Verification Matrix

Every task and subsystem in v1.4.0 is audited against these 14 pillars:

1. **Bug Review**: Boundary checks on line spacing, off-by-one errors in fuzzy search regex compilation, null check on active editor pointers.
2. **Syntax Review**: Modern C++20 standard compliance, strict warning flags (`-Wall -Wextra`), valid shebangs and CMake syntax.
3. **Runtime Review**: No implicit assumptions about system locale, working directory, or unpinned dependencies.
4. **Logic Review**: State integrity during document switching, split view synchronization, and macro recording/playback state machines.
5. **Memory / Resource Review**: Proper RAII lifecycle for widgets, no memory leaks in worker threads, bounded string buffers.
6. **Dead Code Review**: Elimination of unused stubs while preserving public API compatibility.
7. **Duplicate Code Review**: Centralization of file metadata formatting and security checks.
8. **Circular Dependency Review**: Strict acyclic header inclusions; forward declarations in header files.
9. **Performance Bottlenecks Review**: Asynchronous background execution for batch macros; no blocking I/O on UI thread.
10. **Security Vulnerability Review**: Comprehensive coverage of OWASP Top 10 (2025–2026), CWE Top 50, and CVE-2026-20841/16762/25926 mitigations.
11. **Maintainability Review**: Clear separation of concerns, descriptive identifiers, self-documenting code.
12. **Scalability Review**: Smooth handling of large workspaces and multi-document batch operations.
13. **Readability Review**: Consistent code style, meaningful comments explaining architectural rationale.
14. **Architecture / Completeness Review**: Clean modular design adhering to Qt best practices and SparkNotePad heritage.

---

## 6. Detailed Implementation Tasks for v1.4.0

### Phase 1: Scintilla 5.6.9 Change History & Line Spacing Integration

- [x] **Task 1.1: ScintillaNext Change History Engine Hooks**
  - Implement `setChangeHistoryEnabled(bool)` in `ScintillaNext.cpp` using `SCI_SETCHANGEHISTORY`.
  - Assign margin 4 with indicators for modified (orange), saved (green), and reverted states.
- [x] **Task 1.2: Fine-Grained Line Spacing Controls**
  - Implement `setExtraLineSpacing(int ascent, int descent)` in `ScintillaNext.cpp` using `SCI_SETEXTRAASCENT` and `SCI_SETEXTRADESCENT`.
  - Store configuration in `ApplicationSettings`.
- [x] **Task 1.3: Preferences Dialog UI Updates**
  - Update `PreferencesDialog.ui` with controls for Change History and Line Spacing.

### Phase 2: macOS Title-Bar Proxy Icon & Enriched Tab Tooltips

- [x] **Task 2.1: Native macOS Window File Path Proxy Icon**
  - Call `QMainWindow::setWindowFilePath` when switching active tabs in `MainWindow.cpp`.
- [x] **Task 2.2: Enriched Tab Tooltips**
  - Format rich HTML tooltips in `DockedEditor.cpp` displaying path, size, lines, characters, encoding, EOL, and modification time.

### Phase 3: Search Enhancements & Batch Automation

- [x] **Task 3.1: Fuzzy Subsequence Search Matcher**
  - Implement `buildFuzzySubsequencePattern` in `RegexSafetyEngine` with ReDoS protection.
  - Add "Fuzzy Match" checkbox in `FindReplaceDialog` and `QuickFindWidget`.
- [x] **Task 3.2: Batch Macro Processing**
  - Add "All Open Files" and "Directory" batch modes in `MacroRunDialog`.
  - Implement cancellable background execution with progress feedback.

### Phase 4: Diagnostic Transparency & Local MCP Tool Server

- [x] **Task 4.1: Help → Copy Debug Information to Clipboard**
  - Format comprehensive diagnostic string (OS, Qt, compiler, theme, plugins) in `MainWindow.cpp`.
- [x] **Task 4.2: Local Read-Only Model Context Protocol (MCP) Server**
  - Implement loopback-only, token-authenticated, read-only MCP server in `src/features/LocalMcpServer.cpp`.

### Phase 5: Security Hardening & 2025–2026 CVE Mitigations

- [x] **Task 5.1: Strict URI Scheme Validator (CVE-2026-20841)**
  - Validate and sanitize all external links in `NetworkSecurityPolicy`.
- [x] **Task 5.2: XML & JSON Parser Depth Limiter (CVE-2026-16762)**
  - Enforce 32-level recursion limit in `ConfigIntegrityGuard`.
- [x] **Task 5.3: Safe Process Argument Quoting (CVE-2026-25926)**
  - Ensure all external process calls use argument arrays without shell invocation.

### Phase 6: i18n Translation & UI Text Synchronization

- [x] **Task 6.1: String Extraction (`lupdate`)**
  - Extract newly added strings into `SparkNotePad_en.ts` and `SparkNotePad_id.ts`.
- [x] **Task 6.2: Translation Completion**
  - Translate all strings for 100% coverage (0 unfinished strings).
- [x] **Task 6.3: Binary Compilation (`lrelease`)**
  - Compile `.qm` binary catalogs.

### Phase 7: Static Analysis & 14-Pillar Quality Verification

- [x] **Task 7.1: Automated Lint & Syntax Check**
  - Execute `cppcheck` and `clangd --check`.
- [x] **Task 7.2: SonarLint Verification**
  - Restart SonarLint language server (`sonar-restart-ext.sh`) and resolve diagnostics.
- [x] **Task 7.3: 14-Pillar Review Sign-Off**
  - Validate all 14 review pillars with empirical evidence.

### Phase 8: CMake Build & Release Synchronization

- [x] **Task 8.1: Update Version in CMakeLists.txt**
  - Bump version to `1.4.0`.
- [x] **Task 8.2: CMake Build (Unix Makefiles, No Ninja)**
  - Compile with `cmake --build build -j2`.
- [x] **Task 8.3: Unit Test Suite**
  - Run all tests via `ctest --test-dir build --output-on-failure`.
- [x] **Task 8.4: Documentation Rebuild**
  - Synchronize `README.md`, `README-id.md`, `doc/CHANGELOG.txt`, `doc/DOCNOTE.txt`, `wiki/`.
