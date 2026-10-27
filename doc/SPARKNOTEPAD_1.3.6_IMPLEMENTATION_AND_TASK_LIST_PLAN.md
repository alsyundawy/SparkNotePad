# SparkNotePad 1.3.6 — Expanded Platform Support, Security Audit, UI Polish & Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use `superpowers:subagent-driven-development` (recommended) or `superpowers:executing-plans` to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

- **Date:** 2026-10-28
- **Target Release:** SparkNotePad v1.3.6
- **Current Baseline:** SparkNotePad v1.3.5 (2026-10-24)
- **Editor Engine:** Scintilla 5.6.7 + Lexilla 5.5.4 + Lua 5.5.1 + Qt 6.5–6.12
- **Lead Maintainer:** HARRY DERTIN SUTISNA ALSYUNDAWY (`@alsyundawy`)

---

## 1. Executive Summary & Research Context

### 1.1 Overview

This master document synthesizes the multi-paradigm text editor architecture research, empirical 2025–2026 vulnerability audit, and concrete engineering task specifications for the **SparkNotePad v1.3.6** release. It harmonizes the technical threat intelligence across upstream ecosystems (Notepad++, Notepad--, Notepad2/3/4, Kate, Pluma, Sublime Text, BBEdit, Scintilla, Lexilla, Lua, and the Qt Framework) with targeted visual polish (monoline SVG icons, dialog QComboBox ergonomics) and expanded operating system distribution matrices.

---

## 2. Multi-Paradigm Text Editor Architecture & Framework Analysis

### 2.1 Architectural Taxonomy

| Editor           | Framework / UI Subsystem           | Language Standard    | Text Rendering Engine                      | Regex Subsystem                   | Extension / Scripting                       | Sandboxing / Isolation                                        |
| :--------------- | :--------------------------------- | :------------------- | :----------------------------------------- | :-------------------------------- | :------------------------------------------ | :------------------------------------------------------------ |
| **SparkNotePad** | Qt 6.5–6.12 (Widgets, ADS Docking) | C++20                | Scintilla 5.6.7 + Lexilla 5.5.4            | Scintilla Native (`SCI_OWNREGEX`) | Sandboxed Lua 5.5.1 + JavaScriptCore        | Strict API restriction (`os`/`io`/`package`/`debug` stripped) |
| **Notepad++**    | Native Win32 API / STL             | C++17/C++20          | Scintilla 5.6.x + Lexilla 5.5.x            | PCRE / Boost.Regex                | External C/C++ DLL Plugins                  | Process-level token; WinGUp updater                           |
| **Notepad--**    | Qt 5.15 / Qt 6.x                   | C++17                | Scintilla / QScintilla Custom              | Qt `QRegularExpression` / PCRE2   | Native C++ Plugin System                    | OS file permissions                                           |
| **Notepad3**     | Native Win32 API                   | C++17                | Scintilla + Oniguruma Regex                | Oniguruma Regex Engine            | In-process Win32 hooks                      | Standard user session                                         |
| **Notepad4**     | Native Win32 API                   | C++20                | Scintilla 5.x + Lexilla 5.x                | Boost.Regex / PCRE                | Win32 DLL extensions                        | Standard user session                                         |
| **KDE Kate**     | Qt 6 + KDE Frameworks 6 (KF6)      | C++20                | KTextEditor (custom piecewise line buffer) | `QRegularExpression` (PCRE2)      | KTextEditor Scripting (JS/QML) + LSP Client | Flatpak / Wayland portal isolation                            |
| **Pluma**        | GTK 3.x / GObject                  | C99 / C11            | GtkSourceView (Pango / Cairo)              | GLib `GRegex` (PCRE2)             | Python 3 (libpeas)                          | Flatpak portal sandboxing                                     |
| **Sublime Text** | Custom proprietary (Skia-based)    | C++ & Python 3.3/3.8 | Custom hardware-accelerated text engine    | Oniguruma Regex                   | Out-of-process Python plugin host           | Subprocess isolation for plugins                              |
| **BBEdit**       | macOS AppKit / Cocoa               | Objective-C & C++    | Proprietary text rendering engine          | PCRE2 Regex                       | AppleScript, Shell, Text Filters            | macOS App Sandbox / Hardened Runtime                          |

---

## 3. Expanded Official Distro & Operating System Support Matrix

SparkNotePad is compiled with C++20 against Qt 6. Binaries generated on the official glibc baseline (**Ubuntu 22.04 LTS / glibc 2.35**) execute reliably across all modern downstream Linux distributions. AppImage and Flatpak distributions serve as portable fallbacks for distributions lacking native Qt 6 packages.

| Platform         | Version Range | Official Qt 6 Availability     | Recommended Packaging / Delivery              | Architectural Notes                                       |
| :--------------- | :------------ | :----------------------------- | :-------------------------------------------- | :-------------------------------------------------------- |
| **Ubuntu**       | 20.04 LTS     | None (Qt 5 only in main)       | AppImage or source build (Qt 6.2+)            | No official `qt6-base` in Ubuntu focal repositories       |
|                  | 22.04 LTS     | Qt 6.2.4                       | Native `.deb` (canonical baseline)            | glibc 2.35 baseline, GCC 12 toolchain                     |
|                  | 24.04 LTS     | Qt 6.4.2                       | Native `.deb`                                 | 64-bit `time_t` transition packages supported             |
|                  | 26.04 LTS     | Qt 6.10.x / 6.11.x             | Native `.deb`                                 | Verified Qt 6.10/6.11 ABI compatibility                   |
| **Debian**       | 11 (Bullseye) | Limited / backports            | AppImage or source build                      | Requires external backports for Qt 6                      |
|                  | 12 (Bookworm) | Qt 6.4.x                       | Native `.deb`                                 | Fully supported via alternative multi-distro dependencies |
|                  | 13 (Trixie)   | Qt 6.8.x                       | Native `.deb`                                 | Multi-arch compatible                                     |
|                  | 14 (Forky)    | Qt 6.10+ / 6.11                | Native `.deb`                                 | Rolling development branch                                |
| **FreeBSD**      | 13.x – 15.x   | `x11-toolkits/qt6` metapackage | Native port / pkg (`qt6-base`, `qt6-5compat`) | Validated native execution on KDE Plasma 6 & XFCE         |
| **Fedora**       | 40            | Qt 6.6 – 6.8                   | Native `.rpm`                                 | Automatic DNF dependency resolution                       |
|                  | 41 – 44       | Qt 6.8 – 6.11                  | Native `.rpm`                                 | Verified clean compilation on modern GCC 14/15            |
| **Alma / Rocky** | 8             | No official Qt 6 in BaseOS     | AppImage or EPEL build                        | BaseOS lacks Qt 6 libraries                               |
|                  | 9             | Qt 6.6+ (EPEL 9)               | Native `.rpm`                                 | Enterprise Linux 9 target                                 |
|                  | 10 – 11       | Qt 6.8+                        | Native `.rpm`                                 | Next-generation Enterprise Linux                          |
| **openSUSE**     | Leap 15.x     | Community / `KDE:Qt6` repo     | Zypper package or AppImage                    | Community repository required on Leap                     |
|                  | Tumbleweed    | Rolling Latest (Qt 6.12+)      | Native RPM                                    | Rolling release branch                                    |
| **Slackware**    | Current       | Manual / SlackBuilds           | Native SlackBuild from source                 | Verified compilation with standard toolchain              |
| **macOS**        | 11 (Big Sur)  | Qt 6.2 – 6.7 (legacy support)  | Universal App bundle                          | Legacy SDK constraints                                    |
|                  | 12 – 26       | Qt 6.5 – 6.12                  | Native `.dmg` (Universal arm64 / x86_64)      | Tested on Apple Silicon M-series & Intel Macs             |

### Platform Action Items for v1.3.6

- Document this comprehensive support matrix in `doc/PLATFORM_SUPPORT.md`.
- Maintain Ubuntu 22.04 LTS (glibc 2.35) as the lowest official binary release runner.
- Document AppImage and Flatpak distribution paths for legacy enterprise installations.

---

## 4. Comprehensive 2025–2026 Security Advisory & Vulnerability Audit

### 4.1 Primary-Source CVE / GHSA Matrix

| CVE / Advisory ID                         | Target Component     | Affected Versions             | CVSS v3.1 / CWE                      | Root Cause Summary                                                                                   | Impact                                          | Relevance to SparkNotePad                                                                                                                                |
| :---------------------------------------- | :------------------- | :---------------------------- | :----------------------------------- | :--------------------------------------------------------------------------------------------------- | :---------------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **CVE-2025-15556**                        | Notepad++ WinGUp     | Notepad++ < 8.8.9             | **8.8 (High)** / `CWE-494`           | Insecure update package integrity validation in WinGUp; metadata intercepted via MiTM/DNS poisoning. | Supply-Chain Arbitrary Code Execution           | **NOT APPLICABLE / MITIGATED**: SparkNotePad never auto-downloads or executes binaries; routes to verified browser HTTPS releases.                       |
| **CVE-2025-49144**                        | Notepad++ Installer  | Notepad++ <= 8.8.1            | **7.8 (High)** / `CWE-426`           | Insecure executable search path in NSIS installer execution chain.                                   | Local Privilege Escalation (LPE)                | **NOT APPLICABLE**: SparkNotePad uses CPack DMG (macOS) and native RPM/DEB packages without vulnerable NSIS search paths.                                |
| **CVE-2026-46710**                        | Notepad++ Shell Ext  | Notepad++ 8.9.4–8.9.6         | **7.8 (High)** / `CWE-426`           | Relative invocation of `powershell.exe` from user-writable context menu folder.                      | Elevated Command Execution                      | **NOT APPLICABLE**: SparkNotePad Linux integration scripts use absolute paths and POSIX shell without PowerShell dependencies.                           |
| **CVE-2026-48778**                        | Notepad++ Config     | Notepad++ <= 8.9.6            | **8.4 (High)** / `CWE-94`            | Inadequate validation of command strings inside `config.xml` and `shortcuts.xml`.                    | Arbitrary Binary Execution via Config Tampering | **CRITICAL DEFENSE INSIGHT**: SparkNotePad `QSettings`/session loader must sanitize and validate all command shortcuts.                                  |
| **CVE-2026-48800**                        | Notepad++ Core       | Notepad++ <= 8.9.6            | **8.4 (High)** / `CWE-73`            | External control of file name/path parameters in user macros.                                        | Arbitrary Command Execution                     | **ADDRESSED IN PLAN**: Strict command and macro execution parameter boundaries added to v1.3.6.                                                          |
| **CVE-2026-4744**                         | Notepad3 / Scintilla | Notepad3 <= 6.25.x            | **9.1 (Critical)** / `CWE-125`       | Out-of-bounds read in Oniguruma `regcomp.c` integrated into custom Scintilla regex build.            | Memory Disclosure / Heap Corruption             | **NOT APPLICABLE**: SparkNotePad explicitly compiles with `SCI_OWNREGEX`, completely bypassing vulnerable Oniguruma builds.                              |
| **CVE-2026-20841**                        | MS Windows Notepad   | Windows 11 Notepad 2026       | **8.8 (High)** / `CWE-20` / `CWE-79` | Improper URI scheme and link validation in Markdown preview, executing arbitrary protocols.          | Remote Code Execution via Clickable Links       | **HIGH VALUE TARGET**: SparkNotePad v1.3.6 Markdown preview strictly whitelists `http/https/mailto` and blocks local execution.                          |
| **CVE-2026-42214**                        | NotepadNext          | NotepadNext < 0.14            | **8.6 (High)** / `CWE-94`            | Unsanitized file extension string formatted via `QString::arg()` directly into Lua code chunk.       | Arbitrary Code Execution on File Open           | **PROVABLY MITIGATED & HARDENED**: SparkNotePad uses `setVariable("ext", extension)` via C API stack binding; v1.3.6 strips `os`/`io`/`package`/`debug`. |
| **CVE-2026-95620**                        | Qt Network           | Qt 6.x                        | **7.5 (High)** / `CWE-416`           | Use-after-free (UAF) in HTTP/2 frame stream handler under rapid cancellation.                        | Memory Corruption / Crash                       | **MITIGATED**: SparkNotePad update checking uses sequential HTTP/1.1 requests through standard `QNetworkAccessManager`.                                  |
| **CVE-2026-107157**                       | Qt Core              | Qt 6.5.0–6.8.8, 6.11.0–6.11.2 | **7.5 (High)** / `CWE-125`           | Out-of-bounds read in `QCborValue::fromCbor()` when parsing malformed CBOR byte arrays.              | Application Crash / DoS                         | **NOT APPLICABLE**: SparkNotePad does not ingest or parse untrusted CBOR binary streams.                                                                 |
| **CVE-2026-107590** / **CVE-2026-107472** | Qt 5 Compatibility   | Qt Core5Compat < 6.8.9 / 6.12 | **6.5 (Medium)** / `CWE-125`         | Out-of-bounds read in `QTextCodec` TSCII encoding converter on malformed 8-bit stream.               | Buffer Over-read / DoS                          | **MONITORED**: SparkNotePad uses `uchardet` and standard UTF-8/UTF-16/Latin1 codecs; fallback guard added in v1.3.6.                                     |
| **CVE-2026-76151**                        | Qt Network           | Qt 6.x                        | **5.3 (Medium)** / `CWE-126`         | Buffer over-read in `QtNetwork` HTTP Cache-Control header parsing on oversized payloads.             | Memory Leak / DoS                               | **ADDRESSED IN PLAN**: Update queries strictly cap header sizes and enable full offline mode in v1.3.6.                                                  |
| **CVE-2025-14575**                        | Qt Network / OpenSSL | Qt 6.0.0–6.8.1 (Linux/Unix)   | **7.4 (High)** / `CWE-426`           | Uncontrolled search path element in OpenSSL TLS backend allowing rogue CA cert loading from CWD.     | MITM TLS Certificate Spoofing                   | **MITIGATED & HARDENED**: Enforce system default SSL configuration and restrict CWD lookups in `NetworkSecurityPolicy`.                                  |
| **CVE-2025-14576**                        | Qt SVG               | Qt 6.x                        | **7.8 (High)** / `CWE-94`            | Insufficient node ID validation in Qt SVG allowing script injection in Qt Quick `VectorImage`.       | Arbitrary QML/JS Code Injection                 | **MITIGATED**: SparkNotePad does not use Qt Quick or QML `VectorImage`; renders static SVG icons via `QSvgRenderer`.                                     |
| **CVE-2026-41526**                        | KDE KShell           | KDE Frameworks 5/6            | **8.1 (High)** / `CWE-78`            | Argument injection and shell expansion flaw in `KShell::splitArgs`.                                  | Command Injection                               | **NOT APPLICABLE**: SparkNotePad contains no KDE KShell dependencies; uses POSIX arguments.                                                              |
| **CVE-2026-7006**                         | Sublime Text 3 & 4   | Sublime Text <= 4192          | **7.8 (High)** / `CWE-427`           | DLL planting in user-writable `%LOCALAPPDATA%` update staging directory.                             | Local Privilege Escalation                      | **NOT APPLICABLE**: macOS and Linux builds run from standard `/Applications` and `/usr/bin` read-only mount points.                                      |
| **CVE-2026-82604**                        | BBEdit               | BBEdit <= 15.5.5              | **5.5 (Medium)** / `CWE-674`         | Uncontrolled recursion in Java Language Tokenizer on deeply nested syntax trees.                     | Stack Exhaustion / App Hang                     | **MITIGATED**: Lexilla 5.5.4 utilizes iterative lexical state machines rather than recursive descent parsers.                                            |

---

## 5. SparkNotePad v1.3.6 Architecture & UI Ergonomics

### 5.1 Architecture & Defensive Feature Alignment

```text
┌────────────────────────────────────────────────────────────────────────────────┐
│                        SparkNotePad v1.3.6 New Features                        │
├─────────────────────────┬──────────────────────────────────────────────────────┤
│ Feature Component       │ Threat Addressed / Architectural Benefit             │
├─────────────────────────┼──────────────────────────────────────────────────────┤
│ 1. MarkdownSafePreview  │ Mitigates CVE-2026-20841 (Link Injection RCE)        │
│    Dock & Interceptor   │ Strict URI whitelist, disables JS/QML, safe HTML     │
├─────────────────────────┼──────────────────────────────────────────────────────┤
│ 2. Hash & Integrity     │ Mitigates CVE-2025-15556 (Supply Chain Tampering)    │
│    Verification Suite   │ SHA-256/SHA-512/BLAKE2b hash computation & match     │
├─────────────────────────┼──────────────────────────────────────────────────────┤
│ 3. Config & Session     │ Mitigates CVE-2026-48778 / CVE-2026-48800            │
│    Integrity Guard      │ Rejects unauthorized executable paths & shell cmds   │
├─────────────────────────┼──────────────────────────────────────────────────────┤
│ 4. Scintilla ReDoS &    │ Mitigates CVE-2026-4744 & Catastrophic Backtracking  │
│    Timeout Safeguard    │ Step quotas and execution timeout bounds on find     │
├─────────────────────────┼──────────────────────────────────────────────────────┤
│ 5. Strict Network TLS   │ Mitigates QtNetwork CVE-2025-14575 & CVE-2026-76151  │
│    & Air-Gap Policy     │ Disables CWD cert lookups; 1-click full offline mode │
├─────────────────────────┼──────────────────────────────────────────────────────┤
│ 6. Complete Monoline    │ HiDPI visual harmony across Retina and 4K displays   │
│    SVG Icon Suite       │ Eliminates legacy low-res PNG toolbar/menu assets    │
├─────────────────────────┼──────────────────────────────────────────────────────┤
│ 7. Dialog QComboBox     │ Resolves unstyled/ugly dropdowns across all dialogs  │
│    Style & Layout Polish│ Modern dropdown arrow, clean padding, popup contrast │
└─────────────────────────┴──────────────────────────────────────────────────────┘
```

---

## 6. Unified Implementation Plan & Step-by-Step Task List

### Phase 1: Platform Support Documentation & Packaging Configuration

- [x] **Task 1.1: Author Official Platform Support Document**
  - **Files:** `doc/PLATFORM_SUPPORT.md`
  - Document the full distribution compatibility matrix across Ubuntu, Debian, FreeBSD, Fedora, RHEL/Alma/Rocky, openSUSE, Slackware, and macOS.
  - Detail Qt 6 availability and official recommendations for AppImage and Flatpak.

- [x] **Task 1.2: Update README Platform Badges and Release Notes**
  - **Files:** `README.md`, `README-id.md`
  - Align platform compatibility badges and download guidelines with the updated v1.3.6 matrix.

---

### Phase 2: Core Security & Sandbox Hardening

- [x] **Task 2.1: Enforce Strict Lua Sandbox Stripping**
  - **Files:** `src/LuaState.cpp`, `src/LuaState.h`
  - In `LuaState::restrictDangerousApis()`, exhaustively neutralize all attack vectors:
    - Nullify `os.execute`, `os.exit`, `os.tmpname`, `os.remove`, `os.rename`, `os.getenv`, `os.setlocale`.
    - Nullify `io.popen`, `io.open`, `io.lines`, `io.input`, `io.output`, `io.tmpfile`.
    - Nullify `debug` library completely (`debug = nil`).
    - Nullify dynamic loader interfaces: `package.loadlib`, `package.cpath = ""`.
  - Keep math, string, table, and utf8 standard libraries safely accessible for language definitions.

- [x] **Task 2.2: Add Lua Sandbox Injection Unit Tests**
  - **Files:** `tests/TestLuaSandbox.cpp`, `tests/CMakeLists.txt`
  - Create automated test cases verifying that attempts to invoke `os.execute("echo pwned")`, `io.open(...)`, or access `debug` primitives fail safely without crashing or executing external processes.

- [x] **Task 2.3: Implement Configuration & Session Tamper Guard**
  - **Files:** `src/utils/ConfigIntegrityGuard.h`, `src/utils/ConfigIntegrityGuard.cpp`, `src/SessionManager.cpp`
  - Validate external tool strings from `SparkNotePad.ini` or session state:
    - Reject attempts to invoke system command interpreters (`cmd.exe`, `powershell.exe`, `/bin/sh`, `/bin/bash`, `sudo`).
    - Enforce file permission verification (`0600`) on session configuration files.

- [x] **Task 2.4: Implement Scintilla ReDoS Safeguard**
  - **Files:** `src/utils/RegexSafetyEngine.h`, `src/utils/RegexSafetyEngine.cpp`
  - Check regular expressions for exponential nested quantifiers (`(a+)+$`) and bound execution time within find/replace queries.

---

### Phase 3: High-Visibility UI Polish (Icons & Dialog Dropdowns)

- [x] **Task 3.1: Complete Monoline SVG Suite for Remaining PNG Icons**
  - **Files:** `src/icons/*.svg`, `src/SparkNotePad.qrc`
  - Replace legacy toolbar/menu PNG icons (`newfile.png`, `openFile.png`, `closeFile.png`, `closeAll.png`, `saveAll.png`, `saved.png`, `unsaved.png`, `cut.png`, `copy.png`, `paste.png`, `printer.png`, `readonly.png`, `text_indent.png`, `text_indent_remove.png`, `table_edit.png`, `table_delete.png`, `arrow_up.png`, `arrow_down.png`, `arrow_refresh.png`) with clean, consistent 24x24 monoline SVGs matching the official visual design language.
  - Update resource references in `src/dialogs/MainWindow.ui` and `src/dialogs/MainWindow.cpp`.

- [x] **Task 3.2: Application-Wide QComboBox and Dialog Style Modernization**
  - **Files:** `src/SparkNotePadApplication.cpp`, `src/dialogs/PreferencesDialog.cpp`, `src/dialogs/FindReplaceDialog.cpp`
  - Unify `QComboBox`, `QListWidget`, `QTreeWidget`, and dropdown views across all dialogs with a modern styling helper:
    - Subtle rounded border (`4px`), clean inset padding (`4px 8px`).
    - Platform-appropriate custom vector dropdown arrow.
    - Crisp hover and selected item background (`rgba(59, 130, 246, 0.2)` in dark mode, `#e0e7ff` in light mode).
    - Consistent typography and padding in popup list views (`QAbstractItemView`).

---

### Phase 4: Defensive Features (Markdown Preview & Hash Tool)

- [x] **Task 4.1: Implement Markdown Safe Preview Dock**
  - **Files:** `src/docks/MarkdownSafePreviewDock.h`, `src/docks/MarkdownSafePreviewDock.cpp`, `src/dialogs/MainWindow.cpp`
  - Sandboxed `QTextBrowser` with URL click interception:
    - Whitelist: `http`, `https`, `mailto`.
    - Block and warn: `file:`, `ms-msdt:`, `cmd:`, `powershell:`, `javascript:`, `data:`.
  - Connect with buffer synchronization debounced at 300ms.

- [x] **Task 4.2: Implement Cryptographic Hash & Authenticity Validator**
  - **Files:** `src/utils/HashIntegrityManager.h`, `src/utils/HashIntegrityManager.cpp`, `src/dialogs/HashIntegrityDialog.h`, `src/dialogs/HashIntegrityDialog.cpp`
  - Streaming SHA-256, SHA-512, SHA-3 computation with real-time match comparison against expected release hashes.

- [x] **Task 4.3: Implement Strict Network Policy & Air-Gap Offline Toggle**
  - **Files:** `src/utils/NetworkSecurityPolicy.h`, `src/utils/NetworkSecurityPolicy.cpp`, `src/dialogs/PreferencesDialog.cpp`
  - Enforce TLS 1.3 / 1.2 minimum, disable unvalidated certificate paths, and provide a single-click "Strict Offline Mode" setting.

---

### Phase 5: Build Configuration, Version Bump & Release Assets

- [x] **Task 5.1: Bump Version Metadata to v1.3.6**
  - **Files:** `CMakeLists.txt`, `src/SparkNotePadApplication.h`
  - Update `VERSION` and `APP_VERSION` to `1.3.6`.

- [x] **Task 5.2: Execute Test Suite and Static Security Scans**
  - Run all unit tests via `ctest --output-on-failure`.
  - Run `./scripts/run-security-audit.sh`.
  - Run `trunk check` across modified files.
  - Execute the 14-pillar full repository code review mandate.

- [x] **Task 5.3: Update Changelog and Documentation Notes**
  - **Files:** `CHANGELOG.md`, `doc/DOCNOTE.md`
  - Document all v1.3.6 improvements, threat mitigations, and platform matrix updates.

---

## 7. Final Deep Research, Search & 14-Pillar Code Review Mandate

> ### FINAL DEEP REASEARCH & SERACHING USING WEBTOOLS, CODE REVIEW CHECKLIST
>
> SEMUA FILES PADA SELURUH ISI DARI REPOSITORY INI termasuk seluruh code pada `/Users/alsyundawy/Downloads/GitHub/SparkNotePad/src`:
>
> - **BUG REVIEW.**
> - **SYNTAX REVIEW.**
> - **RUNTIME REVIEW.**
> - **LOGIC REVIEW.**
> - **MEMORY REVIEW.**
> - **DEAD CODE REVIEW.**
> - **DUPLICATE CODE REVIEW.**
> - **CIRCULAR DEPENDENCY REVIEW.**
> - **PERFORMANCE BOTTLENECK REVIEW.**
> - **SECURITY VULNERABILITY REVIEW.**
> - **MAINTAINABILITY REVIEW.**
> - **SCALABILITY REVIEW.**
> - **READABILITY REVIEW.**
> - **LINTER REVIEW.**

---

## 8. Execution Command Quick Reference

```bash
# 1. Configure and build project with tests
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON
cmake --build build -j$(sysctl -n hw.ncpu || nproc)

# 2. Execute automated test suite
ctest --test-dir build --output-on-failure

# 3. Execute static security audit
./scripts/run-security-audit.sh

# 4. Verify formatting and linting
trunk check
```
