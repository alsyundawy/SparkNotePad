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

## ⚡ Executive Summary

SparkNotePad **v1.4.0** delivers high-impact productivity enhancements inspired by deep analysis of Nextpad++, Notepad--, and contemporary text editors, combined with enterprise-grade 2025–2026 vulnerability mitigations and Model Context Protocol (MCP) connectivity:

1. **Configurable Extra Line Spacing (Ascent & Descent)**: Custom extra ascent and extra descent (0–30 px) under Preferences > Margins/Padding, powered by Scintilla's `SCI_SETEXTRAASCENT` and `SCI_SETEXTRADESCENT` with live propagation across all open editors.
2. **Enriched Tab Tooltips with Metadata Inspection**: Real-time tooltips in `DockedEditor` displaying canonical file path, file size, line count, character length, text encoding, EOL format, and modification timestamp.
3. **Non-Backtracking Fuzzy Subsequence Search Engine**: Linear regex generator in `RegexSafetyEngine` preventing ReDoS catastrophic backtracking, with interactive toggle in Find & Replace Dialog and Quick Find Widget.
4. **Batch Macro Execution Across All Open Documents**: Execution scope selector in `MacroRunDialog` ("Current Document" vs "All Open Documents") with progress tracking.
5. **High-Integrity Local Loopback MCP Server**: Model Context Protocol server over `127.0.0.1` JSON-RPC with cryptographically secure Bearer Token authentication (`list_open_tabs`, `read_active_tab`).
6. **2025–2026 CVE Security Hardening**:
   - **CVE-2026-20841**: Malicious URL/URI scheme interception in `NetworkSecurityPolicy`.
   - **CVE-2026-16762**: XML nesting depth limiter (32 levels max) and 10 MB payload limits in `ConfigIntegrityGuard`.
   - **CVE-2026-48778 & CVE-2026-48800**: Privilege escalation defense and 0600 file permission lockdown.
7. **One-Click Debug Diagnostics Clipboard Copy**: Help > Debug Info copies runtime configuration, Scintilla/Lexilla/Lua engines, and system details to clipboard.
8. **14-Pillar Quality Gate**: 100% test pass rate, 0 linter errors, and 0 SonarLint issues.

---

## 📦 Downloads & Verification

- `SparkNotePad-v1.4.0-x64.dmg` (macOS Intel x86_64, 33 MB, verified with `hdiutil`)
- `SparkNotePad-v1.4.0-arm64.dmg` (macOS Apple Silicon ARM64)
- `sparknotepad_1.4.0_amd64.deb` (Debian / Ubuntu x86_64)
- `sparknotepad_1.4.0_arm64.deb` (Debian / Ubuntu ARM64)
- `sparknotepad-1.4.0-1.x86_64.rpm` (Fedora / RHEL x86_64)
- `sparknotepad-1.4.0-1.aarch64.rpm` (Fedora / RHEL ARM64)
