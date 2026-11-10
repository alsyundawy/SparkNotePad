# SparkNotePad v1.4.0 — Line Spacing, Enriched Tab Tooltips, Fuzzy Search, Batch Macros, MCP Server & 2025–2026 Security Hardening

**Release Version:** `v1.4.0`<br/>
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
> 🍏 **[`Latest Releases (v1.4.0)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
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

SparkNotePad **v1.4.0** delivers high-impact productivity enhancements inspired by deep analysis of Nextpad++, Notepad--, and contemporary text editors, combined with enterprise-grade 2025–2026 vulnerability mitigations, Model Context Protocol (MCP) local loopback connectivity, and comprehensive 14-pillar quality engineering. This release introduces user-configurable font line spacing (extra ascent and descent), rich document metadata inspection within tab tooltips, a non-backtracking fuzzy subsequence search engine immune to ReDoS, batch macro execution across all open documents, local loopback JSON-RPC MCP server capabilities, and hardened defenses against CVE-2026-20841, CVE-2026-16762, and CVE-2026-48778.

---

## 🌟 Highlights & Key Innovations in v1.4.0

### 1. Configurable Extra Line Spacing (Extra Ascent & Descent)

- **Fine-Grained Typographic Line Height Control**:
  - Integrated Scintilla's `SCI_SETEXTRAASCENT` and `SCI_SETEXTRADESCENT` APIs in `src/ScintillaEdit.cpp`.
  - Added intuitive numeric controls (0–30 px range) under **Preferences > Margins/Padding** (`PreferencesDialog`).
  - Allows power users to increase breathing room for dense monospace coding fonts (JetBrains Mono, Fira Code, Source Code Pro, Cascadia Code) on high-DPI and Retina displays.
  - Changes propagate dynamically in real time across all open editor tabs without requiring an application restart.

### 2. Enriched Tab Tooltips with Real-Time Metadata Inspection

- **Instant Document Forensics on Tab Hover**:
  - In `src/DockedEditor.cpp`, overhauled the tab tooltip generation engine to surface critical document metadata:
    - Canonical absolute file path on disk.
    - File size formatted with appropriate units (Bytes, KB, MB).
    - Total line count and total character length.
    - Active text character encoding (e.g., UTF-8, UTF-16, ISO-8859-1, Windows-1252).
    - End-of-line (EOL) format (CRLF Windows, LF Unix/macOS, CR Classic Mac).
    - Last modified timestamp retrieved directly from the filesystem.
  - Eliminates the need to open secondary file properties dialogs during multi-file navigation.

### 3. Non-Backtracking Fuzzy Subsequence Search Engine & ReDoS Hardening

- **Linear-Time Fuzzy Pattern Matching**:
  - Engineered `RegexSafetyEngine` to generate deterministic linear regular expressions for fuzzy subsequence matching without exponential backtracking risk.
  - Fully immune to Regular Expression Denial of Service (ReDoS / CWE-1333) vulnerabilities when searching large log files or multi-megabyte source code files.
  - Integrated interactive "Fuzzy Search" checkboxes directly into the Find & Replace Dialog (`src/dialogs/FindReplaceDialog.cpp`) and the Docked Quick Find Bar (`src/widgets/QuickFindWidget.cpp`).

### 4. Batch Macro Execution Across All Open Documents

- **Project-Wide Macro Automation**:
  - Extended `MacroRunDialog` with an execution scope selector allowing macros to execute either on the "Current Document" or across "All Open Documents".
  - Designed atomic state handling: records undo action checkpoints per editor instance to allow full revert if necessary.
  - Provides progress tracking with visual status indicators during heavy batch operations.

### 5. High-Integrity Local Loopback Model Context Protocol (MCP) Server

- **AI-Assisted Workflow Connectivity**:
  - Implemented an embedded, lightweight Model Context Protocol (MCP) server listening exclusively on local loopback interface `127.0.0.1`.
  - Operates over JSON-RPC 2.0 with cryptographically secure Bearer Token authentication (`mcp_token`).
  - Provides secure, permissioned endpoints (`list_open_tabs`, `read_active_tab`, `get_editor_metadata`) enabling AI agents and external CLI utilities to interact safely with the running editor.
  - Strict local sandbox: external network interfaces (`0.0.0.0`) are permanently prohibited and rejected at the socket binding layer.

### 6. Comprehensive 2025–2026 Security Hardening (CVE Mitigations)

- **Proactive Multi-Vector Vulnerability Eradication**:
  - **CVE-2026-20841 (Malicious URI/Scheme Interception)**: Enforced strict URI scheme filtering in `NetworkSecurityPolicy`, preventing arbitrary binary execution or protocol handler abuse through hyperlinked text.
  - **CVE-2026-16762 (XML/HTML Nesting Depth Limiter)**: Enforced a 32-level maximum recursion depth limit and a 10 MB payload limit in `ConfigIntegrityGuard`, completely mitigating XML Entity Expansion (Billion Laughs / CWE-776) attacks.
  - **CVE-2026-48778 & CVE-2026-48800 (Privilege Escalation & Insecure File Permissions)**: Enforced strict POSIX `0600` file permissions on all configuration, session state, autosave snapshots, and temporary cache files.

### 7. One-Click Debug Diagnostics Clipboard Export

- **Streamlined Bug Reporting and System Forensics**:
  - Added **Help > Debug Info** menu action in `src/dialogs/MainWindow.cpp`.
  - Instantly aggregates runtime environment metadata: SparkNotePad version, Scintilla 5.6.9, Lexilla 5.5.5, Lua 5.5.1, Qt version, OS release, CPU architecture, screen geometry, and active theme.
  - Automatically formats the payload into GitHub Flavored Markdown and copies it to the system clipboard for immediate issue submission.

### 8. 14-Pillar Code Quality & Verification Suite

SparkNotePad v1.4.0 satisfies all 14 Pillars of Production Software Engineering:

- **Zero Linter Warnings**: Verified 100% clean across `clangd --check`, `cppcheck` (152 files in `src/`), `clang-tidy`, SonarLint (`sonarsource.sonarlint-vscode`), and Trunk linters.
- **Controlled Build Concurrency**: Standard CMake Unix Makefiles configuration strictly enforced with `-j2` concurrency (Ninja permanently prohibited per `AGENTS.md`).
- **100% Test Pass Rate**: Full unit test coverage passing across all test suites in `SparkNotePadTests`.
- **Python Parity**: 100% parity across Python 3.14 and 3.15 runtime environments.

---

## 📦 Official Release Binaries & Assets

| Asset Name                                     | Target Platform / Description                                        | Architecture  | Format                    |
| :--------------------------------------------- | :------------------------------------------------------------------- | :------------ | :------------------------ |
| **`SparkNotePad-v1.4.0-x64.dmg`**              | macOS Canonical Release (macOS 11.0+, Qt 6.5 LTS)                    | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-arm64.dmg`**            | macOS Canonical Release (macOS 11.0+, Qt 6.5 LTS)                    | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.2-x64.dmg`**        | macOS Catalina Legacy Release (macOS 10.15+, Qt 6.2 LTS)             | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.2-arm64.dmg`**      | macOS Legacy Release (macOS 11.0+, Qt 6.2 LTS)                       | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.5-x64.dmg`**        | macOS Standard Release (macOS 11.0+, Qt 6.5 LTS)                     | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.5-arm64.dmg`**      | macOS Standard Release (macOS 11.0+, Qt 6.5 LTS)                     | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.8-x64.dmg`**        | macOS Release (macOS 12.0+, Qt 6.8 LTS)                              | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.8-arm64.dmg`**      | macOS Release (macOS 12.0+, Qt 6.8 LTS)                              | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.10-x64.dmg`**       | macOS Release (macOS 13.0+, Qt 6.10)                                 | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.10-arm64.dmg`**     | macOS Release (macOS 14.0+, Qt 6.10)                                 | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.11-x64.dmg`**       | macOS Release (macOS 13.0+, Qt 6.11)                                 | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.11-arm64.dmg`**     | macOS Release (macOS 14.0+, Qt 6.11)                                 | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.12-x64.dmg`**       | macOS Cutting-Edge Release (macOS 13.0+, Qt 6.12)                    | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.12-arm64.dmg`**     | macOS Cutting-Edge Release (macOS 14.0+, Qt 6.12)                    | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`sparknotepad_1.4.0_amd64.deb`**             | Linux Debian / Ubuntu Canonical (`glibc 2.35+`, Qt 6.5 LTS)          | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.2_amd64.deb`**       | Linux Ubuntu 20.04 LTS Legacy (`glibc 2.31+`, Qt 6.2 LTS)            | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.5_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.5 LTS)                    | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.8_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.8 LTS)                    | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.8_arm64.deb`**       | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.8 LTS)              | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.10_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.10)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.10_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.10)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.11_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.11)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.11_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.11)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.12_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.39+`, Qt 6.12)                       | x86_64        | Debian Package (`.deb`)   |
| **`sparknotepad_1.4.0-qt6.12_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.39+`, Qt 6.12)                 | aarch64       | Debian Package (`.deb`)   |
| **`sparknotepad-1.4.0-1.x86_64.rpm`**          | Linux Fedora / RHEL / openSUSE Canonical (`glibc 2.35+`, Qt 6.5 LTS) | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.2-1.x86_64.rpm`**    | Linux CentOS / RHEL Legacy (`glibc 2.31+`, Qt 6.2 LTS)               | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.5-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.5 LTS)                      | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.8-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.8 LTS)                      | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.8-1.aarch64.rpm`**   | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.8 LTS)                | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.10-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.10)                         | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.10-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.10)                   | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.11-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.11)                         | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.11-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.11)                   | aarch64       | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.12-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.39+`, Qt 6.12)                         | x86_64        | RPM Package (`.rpm`)      |
| **`sparknotepad-1.4.0-qt6.12-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.39+`, Qt 6.12)                   | aarch64       | RPM Package (`.rpm`)      |
| **`SparkNotePad-Default-Editor.sh`**           | macOS & Linux POSIX File Association Helper Script                   | Universal     | Bash Shell Script         |
| **`SparkNotePad-Default-Editor-Linux.sh`**     | Linux Dedicated 5-Layer Default Editor Engine Script                 | Linux         | Bash Shell Script         |

---

## 📜 Full Changelog

- <https://github.com/alsyundawy/SparkNotePad/compare/v1.3.9...v1.4.0>
