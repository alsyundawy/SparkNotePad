# SparkNotePad v1.4.7 — Core Engine Upgrade (Scintilla 5.6.9, Lexilla 5.5.5, Patched Lua 5.5.1), System Transfer (.sparkbundle), POSIX File Inspector, 3-Point Encoding Detection & 2025–2026 CVE Security Hardening

**Release Version:** `v1.4.7`<br/>
**Release Date:** 2026-11-14<br/>
**Maintainer:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Core Editing Engine:** Scintilla 5.6.9 (Vendored Official) & Lexilla 5.5.5 (Vendored Official)<br/>
**Embedded Lua Core:** Sandboxed Lua 5.5.1 (Official Patched Source 1-4)<br/>
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
> 🍏 **[`Latest Releases (v1.4.7)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
> 📖 **[`Release DocNotes (DOCNOTE.md)`](DOCNOTE.md)** &nbsp;|&nbsp;
> 📜 **[`Changelog (CHANGELOG.md)`](CHANGELOG.md)** &nbsp;|&nbsp;
> 🌐 **[`Platform Support Matrix`](doc/PLATFORM_SUPPORT.md)** &nbsp;|&nbsp;
> 🔱 **[`FreeBSD Build Guide (KDE6 & XFCE)`](doc/BUILD_FREEBSD_KDE_XFCE.md)** &nbsp;|&nbsp;
> 📋 **[`FreeBSD Analysis & Tasklist`](doc/FREEBSD_BUILD_ANALYSIS_AND_TASKLIST.md)** &nbsp;|&nbsp;
> 🐛 **[`Issue Tracker`](https://github.com/alsyundawy/SparkNotePad/issues)** &nbsp;|&nbsp;
> 💖 **[`Support via PayPal`](https://www.paypal.me/alsyundawy)**

---

## ⚡ Executive Summary

SparkNotePad **v1.4.7** introduces significant advancements across the core editing engines, portable workspace mobility, POSIX file permissions and script analysis, robust character encoding detection, enhanced navigation dialogs, and comprehensive 2025–2026 CVE security hardening:

1. **Scintilla 5.6.9 & Lexilla 5.5.5 Upgrades**: Upgrades the core text engine with Unicode 17 character categories, 40-byte lexeme cache, line-start layout bypass, change history undo crash fix, restored XPM image colors, and 12 upstream Lexilla memory safety and OOB fixes.
2. **System Transfer & Portable Workspace Bundle (`SparkTransferManager`)**: Enables seamless export and import of user preferences, themes, macros, sessions, and scripts via `.sparkbundle` archives with Zip-Slip defense and in-memory HMAC validation.
3. **POSIX Shell Inspector & Touch Utility (`PosixFileInspector`)**: Real-time shebang dialect detection, octal and symbolic permission formatters, status bar executable bit toggle badge (`FieldPosixPerms`), and precision file touch utility.
4. **Stateless 3-Point `EncodingDetector` & Atomic Large-File Save Guard**: 3-point sampling across header, midpoint, and tail for robust encoding detection, UTF-32 LE/BE BOM support, and atomic saving via private sibling temporary files (`0600` permissions).
5. **Multi-Mode `GotoDialog` & Project Tree Safe Rename Preview**: Direct navigation by Line:Col, Character Offset, or Document Byte Offset, alongside interactive folder/file rename with dry-run collision detection.
6. **Comprehensive 2025–2026 CVE Security Hardening**: Automated test verification against CVE-2026-42214, CVE-2026-48778, CVE-2026-52885, CVE-2026-52886, CVE-2026-57233, CVE-2026-54758, and CVE-2026-77605.

---

## 🌟 Key Features & Innovations in v1.4.7

### 1. Core Engine Upgrade: Scintilla 5.6.9, Lexilla 5.5.5 & Patched Lua 5.5.1

- **Scintilla 5.6.9 (Vendored)**:
  - Unicode 17 character categories and case conversion tables.
  - Maximum cached lexeme size increased from 30 to 40 bytes.
  - Line-start layout bypass optimization dramatically accelerating large-document scrolling.
  - Critical crash fix in Change History Undo caused by bulk deletion optimization.
  - Restored XPM autocompletion and margin marker color rendering.
- **Lexilla 5.5.5 (Vendored)**:
  - Markdown YAML/TOML/JSON front-matter styling at document start.
  - HTML/JS nested template literal tokenization, regex slash in character classes, and JavaScript identifier enhancements.
  - 12 upstream memory safety, out-of-bounds write/read, and infinite recursion fixes.
- **Embedded Lua 5.5.1 Patches**:
  - Official bug-fix patches 1 through 4 guarding against negative parameter left-shift, NaN `for` loop limits, negative zero constant folding, and error unwinding preservation during `coroutine.close`.

### 2. System Transfer & Workspace Bundles (`.sparkbundle`)

- Export and import workspace configurations across macOS, Linux, and FreeBSD.
- Secure bundle format with in-memory SHA-256 HMAC digest verification and optional AES-256-CBC encryption.
- Built-in protection against Zip-Slip path traversal attacks (`CVE-2026-57233`).

### 3. POSIX Shell Inspector & File Permissions

- Detects shebang dialects: Bash, Zsh, Dash, Ksh, POSIX sh, Python, Lua, and FreeBSD `rc.d`.
- Status bar badge `FieldPosixPerms` showing octal and symbolic permissions with one-click `chmod +x` toggle.
- File menu actions: `Toggle Executable Bit (chmod +x)` and `Enforce Owner-Only Private Mode (chmod 0600)`.
- Precision file touch utility via POSIX `utimes`.

### 4. 3-Point Encoding Detection & Atomic Save Guard

- Stateless tri-point sampling (header 16KB, midpoint 16KB, tail 16KB) for reliable charset detection in large files.
- UTF-32 LE/BE BOM detection.
- Atomic file save in `ScintillaNext::writeToDisk` using private sibling temporary files (`.filename.sparktmp`), `0600` permissions, `fsync()`, and atomic POSIX `rename()`.

### 5. Multi-Mode Goto Dialog & Safe Rename Preview

- Modal `GotoDialog` supporting Line:Col, Character Offset (`Line:-Col`), and Document Byte Offset with real-time target calculations.
- Safe rename in `FolderAsWorkspaceDock` with live path traversal check, collision detection, and dry-run preview.

### 6. Visual Polish, Markdown Live Preview Dedicated Engine & 14-Pillar Code Quality

- **Markdown Live Preview Dedicated Engine & Full HTML Viewer Deprecation**: Completely stripped legacy HTML preview functionality and format toggles; SparkNotePad exclusively standardizes on a dedicated, lightweight, high-performance Markdown live preview engine with queued asynchronous downloading (`QQueue<QUrl> m_downloadQueue`, 256-entry LRU cache, 16 concurrent workers) and reactive document layout dirty tracking (`markContentsDirty`) ensuring all remote online assets (Shields.io badges, SVGs, documentation diagrams) render completely whenever connected to the internet without manual scrolling.
- **Credits Placeholder Fix**: Resolved `%2` placeholder index bug in `AboutDialog` Credits tab where raw color code `#0550ae` erroneously showed up as the section title.
- **Markdown Preview Math Inheritance**: Stripped hardcoded `#0550ae` on inline/block math equations in Markdown preview so equations dynamically adapt to active light/dark themes.
- **14-Pillar Linter Remediation**: Resolved clang-tidy braces around statements, virtual override visibility, and move semantics across `EditorInfoStatusBar`, `QuickFindWidget`, `StatusLabel`, and `FocusWatcher` with 0 linter errors.
- **Standard 30MB–40MB Lightweight macOS DMG Packaging**: Packaged and verified `build/SparkNotePad-v1.4.7-x64.dmg` (40M, 40,652,861 bytes) with custom background, layout preset `.DS_Store` (680x440), and verified CRC32 checksum.

---

## 🛡️ 2025–2026 CVE Security Hardening Matrix

| Vulnerability                      | Scope                                                 | SparkNotePad v1.4.7 Mitigation                                                            |
| :--------------------------------- | :---------------------------------------------------- | :---------------------------------------------------------------------------------------- |
| **CVE-2026-42214**                 | NotepadNext Lua code injection via filename extension | Passed via Lua C stack (`lua_pushlstring`); unsafe OS/IO libraries stripped               |
| **CVE-2026-48778 / 48800 / 52885** | Notepad++ RCE / TOCTOU config tampering               | In-memory HMAC-SHA256 validation; atomic 0600 disk writes                                 |
| **CVE-2026-52886 / 57233**         | Notepad++ path traversal / Zip-Slip overwrite         | Canonicalization via `weakly_canonical` / `canonicalFilePath`; strict traversal rejection |
| **CVE-2026-54758 / 85279 / 86054** | Notepad++ stack-based buffer overflows                | Zero fixed-size stack buffers; bounded `QString` and `std::string` throughout             |
| **CVE-2026-77605 / 25926 / 38972** | Untrusted CWD path / library hijacking                | Absolute canonical path resolution for external system utilities                          |
| **CVE-2025-14575 / 2026-20841**    | Qt OpenSSL CWD cert lookup & Markdown URI RCE         | Minimum TLS 1.2+; strict preview URI scheme whitelisting                                  |

---

## 📦 Downloads & Release Matrix

| Platform                    | Target Architecture  | Format | Download Link                                                                                                                          |
| :-------------------------- | :------------------- | :----: | :------------------------------------------------------------------------------------------------------------------------------------- |
| **macOS Intel (Canonical)** | x86_64               | `.dmg` | [SparkNotePad-v1.4.7-x64.dmg](https://github.com/alsyundawy/SparkNotePad/releases/download/v1.4.7/SparkNotePad-v1.4.7-x64.dmg)         |
| **macOS Apple Silicon**     | arm64 (M1–M4)        | `.dmg` | [SparkNotePad-v1.4.7-arm64.dmg](https://github.com/alsyundawy/SparkNotePad/releases/download/v1.4.7/SparkNotePad-v1.4.7-arm64.dmg)     |
| **Linux Ubuntu/Debian**     | x86_64 (glibc 2.35+) | `.deb` | [sparknotepad_1.4.7_amd64.deb](https://github.com/alsyundawy/SparkNotePad/releases/download/v1.4.7/sparknotepad_1.4.7_amd64.deb)       |
| **Linux Fedora/RHEL**       | x86_64 (glibc 2.35+) | `.rpm` | [sparknotepad-1.4.7-1.x86_64.rpm](https://github.com/alsyundawy/SparkNotePad/releases/download/v1.4.7/sparknotepad-1.4.7-1.x86_64.rpm) |
