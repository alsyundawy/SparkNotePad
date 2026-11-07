#!/usr/bin/env python3
"""
Generate localized DOCNOTE and CHANGELOG files for SparkNotePad v1.4.0.
Creates:
  - src/doc/DOCNOTE_id.txt (Bahasa Indonesia)
  - src/doc/DOCNOTE_en.txt (English)
  - src/doc/CHANGELOG_id.txt (Bahasa Indonesia)
  - src/doc/CHANGELOG_en.txt (English)
Synchronizes:
  - doc/ and root directory copies.
"""

DOCNOTE_ID = """# Catatan Rilis SparkNotePad — v1.4.0 (Bahasa Indonesia)

--------------------------------------------------------------------------------
================================================================================
Versi Rilis          : 1.4.0 (Spasi Baris Ekstra, Tooltip Tab Diperkaya, Pencarian Fuzzy Aman, Eksekusi Batch Makro, MCP Server & Mitigasi CVE 2025–2026)
Tanggal Rilis        : 2026-11-07
Pengelola Utama      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organisasi           : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repositori           : https://github.com/alsyundawy/SparkNotePad
Mesin Penyunting     : Scintilla 5.6.9 & Lexilla 5.5.5 (125 Lexer Resmi)
Runtime Skrip        : Sandboxed Lua 5.5.1 & Python 3.13+ Bridge
Kerangka Kerja GUI   : Qt 6.5 LTS / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platform      : macOS 11.0–26.0 (Apple Silicon & Intel), Linux (x86_64 & ARM64), FreeBSD 13–15
Lisensi              : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Ringkasan Eksekutif (Bahasa Indonesia): Rilis v1.4.0

SparkNotePad v1.4.0 adalah rilis fitur dan keamanan besar yang terinspirasi dari hasil riset mendalam terhadap Nextpad++, Notepad--, dan editor teks modern lainnya. Rilis ini menambahkan fitur produktivitas tinggi serta mitigasi kerentanan keamanan 2025–2026:

1. Pengaturan Spasi Baris Ekstra (Ascent & Descent):
   - Konfigurasi penambahan tinggi baris atas (ascent) dan bawah (descent) 0–30 px pada Preferensi > Margins/Padding via API Scintilla SCI_SETEXTRAASCENT dan SCI_SETEXTRADESCENT.
   - Terintegrasi langsung dengan ScintillaNext::setExtraLineSpacing() dan disinkronkan secara real-time ke seluruh tab editor yang terbuka melalui EditorManager.

2. Tooltip Tab Diperkaya dengan Inspeksi Metadata Dokumen:
   - Implementasi DockedEditor::buildEnrichedToolTip() yang menyajikan path absolut, ukuran berkas di disk, jumlah baris, karakter, encoding teks (UTF-8, UTF-16, dll.), format baris baru (CRLF/LF/CR), serta timestamp modifikasi berkas.
   - Pembaruan dinamis secara real-time saat tab berganti, dokumen disimpan, atau nama berkas diubah.

3. Mesin Pencarian Subsekuens Fuzzy Tanpa Backtracking:
   - Algoritma RegexSafetyEngine::buildFuzzySubsequencePattern() yang mengubah query pencarian menjadi ekspresi reguler linier deterministik ([^char]*char...).
   - Opsi pencarian fuzzy interaktif pada Dialog Cari & Ganti serta Quick Find Bar (~).
   - Kebal terhadap serangan ReDoS (Regular Expression Denial of Service).

4. Eksekusi Makro Batch Lintas Dokumen Terbuka:
   - Opsi cakupan eksekusi di MacroRunDialog: "Current Document" atau "All Open Documents".
   - Eksekusi makro berurutan ke semua tab aktif dalam satu kali klik.

5. Server MCP Loopback Lokal Berintegritas Tinggi:
   - Server Model Context Protocol loopback lokal 127.0.0.1 berbasis JSON-RPC dengan otentikasi Bearer Token kriptografis untuk integrasi AI coding agent (list_open_tabs, read_active_tab).

6. Pengerasan Keamanan CVE 2025–2026:
   - CVE-2026-20841: Perlindungan skema URL/URI berbahaya (ms-office:, cmd:, powershell:) dan pemblokiran eksekusi biner langsung di NetworkSecurityPolicy.
   - CVE-2026-16762: Pembatasan kedalaman nesting XML (maksimal 32 tingkat) dan batas ukuran berkas 10 MB di ConfigIntegrityGuard.
   - CVE-2026-48778 & CVE-2026-48800: Pencegahan eskalasi hak istimewa (sudo, pkexec, doas) dan penegakan izin berkas konfigurasi ketat 0600.

7. Salin Info Diagnostik Debug Satu Klik:
   - Bantuan > Debug Info menyalin seluruh detail konfigurasi runtime, versi Scintilla/Lexilla/Lua, dan platform OS ke clipboard.

8. Standar Kualitas 14 Pilar:
   - 100% kelulusan unit test, 0 error linter, dan 0 pelanggaran SonarLint.

---

## 2. Ringkasan Rilis Sebelumnya: Rilis v1.3.9

SparkNotePad v1.3.9 adalah rilis peningkatan fondasional dan stabilitas besar yang merangkum seluruh evolusi teknis dari v1.3.8 ke v1.3.9. Rilis ini mencakup peningkatan mesin inti Scintilla dan Lexilla langsung dari upstream resmi, resolusi kerentanan regex loop, penambahan preferensi bahasa default, pewarnaan front-matter Markdown, restorasi penuh kompatibilitas Qt 6.2 LTS (macOS Catalina & Ubuntu 20.04), arsitektur rilis multi-arsitektur Linux ARM64 & x64 (50+ paket biner), perampingan dan penyempurnaan packaging DMG macOS, serta lolos audit kualitas 14 pilar tanpa kompromi.
"""

DOCNOTE_EN = """# SparkNotePad Release DocNote — v1.4.0 (English)

--------------------------------------------------------------------------------
================================================================================
Release Version      : 1.4.0 (Line Spacing, Tab Tooltips, Fuzzy Search, Batch Macros, MCP Server & Security Hardening)
Release Date         : 2026-11-07
Lead Maintainer      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organization         : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repository           : https://github.com/alsyundawy/SparkNotePad
Core Editing Engine  : Scintilla 5.6.9 & Lexilla 5.5.5 (Complete 125 Official Lexers)
Embedded Lua Core    : Sandboxed Lua 5.5.1 & Python 3.13+ Bridge
GUI Framework        : Qt 6.5 LTS / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platforms     : macOS 11.0–26.0 (Apple Silicon & Intel), Linux (x86_64 & ARM64), FreeBSD 13–15
License              : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Executive Summary (English): Release v1.4.0

SparkNotePad v1.4.0 delivers high-impact productivity enhancements inspired by deep analysis of Nextpad++, Notepad--, and contemporary text editors, combined with enterprise-grade 2025–2026 vulnerability mitigations and model context protocol connectivity:

1. Configurable Extra Line Spacing (Ascent & Descent):
   - Configurable from Preferences > Margins/Padding (0 to 30 px) via Scintilla's SCI_SETEXTRAASCENT and SCI_SETEXTRADESCENT APIs.
   - Implemented in ScintillaNext::setExtraLineSpacing() and synchronized across all open editor tabs via EditorManager.

2. Enriched Document Tab Tooltips with Complete Metadata:
   - Implemented DockedEditor::buildEnrichedToolTip() displaying canonical file path, byte/KB size, line count, character length, text encoding name, EOL format (CRLF, LF, CR), and file modification timestamp.
   - Dynamic real-time updates triggered on tab selection, file save points, and renames.

3. Non-Backtracking Fuzzy Subsequence Matching Engine:
   - RegexSafetyEngine::buildFuzzySubsequencePattern() converts input tokens into deterministic linear regex patterns ([^char]*char...).
   - Integrated into Find/Replace Dialog (checkBoxFuzzy) and Quick Find Bar (~).
   - Fully immune to catastrophic backtracking and Regular Expression Denial of Service (ReDoS).

4. Batch Macro Execution Across All Open Documents:
   - Extended MacroRunDialog with execution scope selection: "Current Document" or "All Open Documents".
   - Automatically loops over all managed tabs sequentially with transparent execution progress.

5. High-Integrity Local Loopback MCP Server:
   - High-integrity local server in src/features/LocalMcpServer listening strictly on 127.0.0.1 with cryptographically secure bearer token authentication.
   - Exposes standardized AI coding tools (list_open_tabs, read_active_tab).

6. Enterprise Security Hardening (2025–2026 CVE Mitigations):
   - CVE-2026-20841: Dangerous URI scheme interception (NetworkSecurityPolicy::isSafeUriScheme()) prohibiting dangerous schemes (ms-office:, cmd:, powershell:) and binary execution.
   - CVE-2026-16762: XML/JSON parser recursion limit (ConfigIntegrityGuard::isPayloadStructureSafe()) capping file size at 10 MB and recursion depth at 32 levels.
   - CVE-2026-48778 / CVE-2026-48800: Command injection prevention in external tools and enforcement of strict 0600 file permissions.

7. One-Click Diagnostic Reporting:
   - Help > Debug Info copies system specs, Scintilla 5.6.9, Lexilla 5.5.5, Lua 5.5.1, Qt version, and display metrics directly to the system clipboard.

8. 14-Pillar Quality Gate:
   - 100% unit test pass rate, 0 linter errors, and 0 SonarLint issues.

---

## 2. Previous Release Summary: Release v1.3.9

SparkNotePad v1.3.9 is a major foundational engine upgrade and stability milestone encapsulating the complete architectural progression from v1.3.8 to v1.3.9. It delivers upstream core editing engine upgrades to Scintilla 5.6.9 and Lexilla 5.5.5, resolves algorithmic regex loop vulnerabilities, introduces user-configurable default language preferences, expands Markdown front-matter syntax styling, restores full Qt 6.2 LTS backward compatibility (macOS Catalina & Ubuntu 20.04), establishes a 50+ binary release matrix with native Linux ARM64 packaging, prunes and refines macOS DMG distribution, and passes comprehensive 14-pillar code review.
"""

CHANGELOG_ID = """# Catatan Perubahan (Changelog)

Semua perubahan penting pada **SparkNotePad** didokumentasikan di berkas ini.

Format disusun berdasarkan panduan [Keep a Changelog](https://keepachangelog.com/id/1.1.0/),
dan proyek ini sepenuhnya mematuhi [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.4.0] - 2026-11-07

### 🚀 Spasi Baris Ekstra, Tooltip Tab Diperkaya, Pencarian Fuzzy Aman, Eksekusi Batch Makro, MCP Server & Mitigasi CVE 2025–2026

SparkNotePad v1.4.0 menghadirkan fitur produktivitas esensial yang terinspirasi dari riset Nextpad++ dan kemampuan editor modern, sekaligus memperkuat sistem terhadap kerentanan 2025–2026:

#### 🆕 Ditambahkan

- **Pengaturan Spasi Baris Ekstra (Ascent & Descent)**:
  - Penambahan tinggi baris atas (ascent) dan bawah (descent) 0–30 px pada Preferensi > Margins/Padding.
  - Implementasi ScintillaNext::setExtraLineSpacing() menggunakan SCI_SETEXTRAASCENT dan SCI_SETEXTRADESCENT.
  - Propagasi langsung ke seluruh tab terbuka via EditorManager.
- **Tooltip Tab Diperkaya dengan Inspeksi Metadata Dokumen**:
  - Implementasi DockedEditor::buildEnrichedToolTip() menampilkan path absolut, ukuran di disk, jumlah baris, karakter, encoding (UTF-8, UTF-16, dll.), format baris baru (CRLF, LF, CR), dan waktu modifikasi.
  - Pembaruan otomatis saat perpindahan tab, penyimpanan berkas, atau perubahan nama.
- **Mesin Pencarian Subsekuens Fuzzy Tanpa Backtracking**:
  - Algoritma RegexSafetyEngine::buildFuzzySubsequencePattern() mengubah query menjadi pola regex linier non-backtracking ([^char]*char...).
  - Opsi pencarian fuzzy interaktif pada Dialog Cari & Ganti serta Quick Find Bar (~).
  - Terintegrasi penuh ke dalam mesin pencarian Finder.
- **Eksekusi Makro Batch Lintas Dokumen Terbuka**:
  - Pemilih cakupan eksekusi di MacroRunDialog: "Current Document" atau "All Open Documents".
  - Menjalankan makro berurutan ke semua tab dalam satu panggilan.
- **Server MCP Loopback Lokal Berintegritas Tinggi**:
  - LocalMcpServer port loopback 127.0.0.1 berbasis JSON-RPC dengan otentikasi Bearer Token kriptografis (list_open_tabs, read_active_tab).
- **Salin Info Diagnostik Debug Satu Klik**:
  - Menu Bantuan > Debug Info menyalin seluruh detail konfigurasi runtime, Scintilla 5.6.9, Lexilla 5.5.5, Lua 5.5.1, Qt, dan platform OS ke clipboard.

#### 🛡️ Pengerasan Keamanan (Mitigasi CVE 2025–2026)

- **CVE-2026-20841 (Mitigasi Skema URI Berbahaya)**:
  - NetworkSecurityPolicy::isSafeUriScheme() memblokir eksploitasi skema URI berbahaya (ms-office:, cmd:, powershell:) dan eksekusi biner langsung.
- **CVE-2026-16762 (Mitigasi Kedalaman Nesting XML/JSON DoS)**:
  - ConfigIntegrityGuard::isPayloadStructureSafe() membatasi kedalaman nesting XML hingga 32 level dan batas ukuran 10 MB untuk mencegah ekspansi entitas XML dan crash rekursi.
- **CVE-2026-48778 & CVE-2026-48800 (Mitigasi Perusakan Perintah Tool Eksternal)**:
  - ConfigIntegrityGuard::isExternalCommandSafe() memblokir eskalasi hak istimewa (sudo, pkexec, doas) dan chaining pipa shell.
  - Penegakan izin berkas konfigurasi ketat 0600.

#### 🌐 Lokalisasi

- Pembaruan i18n/SparkNotePad_en.ts dan i18n/SparkNotePad_id.ts dengan 2.017 entri terjemahan (0 unfinished).

---

## [1.3.9] - 2026-11-07

### ⚡ Pembaruan Scintilla 5.6.9 & Lexilla 5.5.5, Pengaturan Bahasa Default, Kompatibilitas Qt 6.2 LTS & Penguatan 14 Pilar

SparkNotePad v1.3.9 adalah rilis fondasional besar yang memperbarui subsistem penyuntingan inti ke Scintilla 5.6.9 dan Lexilla 5.5.5, memperbaiki bug regex infinite loop, menambahkan preferensi bahasa default, mendukung pewarnaan front-matter Markdown, dan memulihkan kompatibilitas Qt 6.2 LTS.
"""

CHANGELOG_EN = """# Changelog

All notable changes to **SparkNotePad** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.4.0] - 2026-11-07

### 🚀 Line Spacing, Enriched Tab Tooltips, Fuzzy Search, Batch Macros, MCP Server & 2025–2026 Security Hardening

SparkNotePad v1.4.0 introduces essential productivity features inspired by Nextpad++ research and modern editor capabilities, while fortifying system security against 2025–2026 vulnerabilities:

#### 🆕 Added

- **Configurable Line Spacing (Extra Ascent & Descent)**:
  - Added user-customizable extra ascent and extra descent (0–30 px) under Preferences > Margins/Padding.
  - Implemented `ScintillaNext::setExtraLineSpacing()` using Scintilla's `SCI_SETEXTRAASCENT` and `SCI_SETEXTRADESCENT`.
  - Dynamic live propagation across all open editors via `EditorManager`.
- **Enriched Tab Tooltips with Metadata Inspection**:
  - Implemented `DockedEditor::buildEnrichedToolTip()` displaying full canonical file path, file size on disk, line count, character length, text encoding (`UTF-8`, `UTF-16`, etc.), EOL format (`CRLF`, `LF`, `CR`), and last modified timestamp.
  - Live tooltip updates triggered on buffer modifications, save points, and file renames.
- **Non-Backtracking Fuzzy Subsequence Search Engine**:
  - Added `RegexSafetyEngine::buildFuzzySubsequencePattern()` converting loose search strings into linear, non-backtracking regex patterns (`[^char]*char...`).
  - Added Fuzzy Match option in both Find & Replace Dialog (`checkBoxFuzzy`) and Quick Find Widget (`buttonFuzzy` `~`).
  - Fully integrated into `Finder` search engine.
- **Batch Macro Execution Across All Open Documents**:
  - Added execution scope selector to Macro Run Dialog: "Current Document" or "All Open Documents".
  - Executes recorded macros sequentially across all tabs in a single invocation.
- **High-Integrity Local Loopback MCP Server**:
  - Added `LocalMcpServer` exposing local loopback JSON-RPC Model Context Protocol tools (`list_open_tabs`, `read_active_tab`).
  - Authenticated via secure cryptographically random bearer token.
- **One-Click Debug Diagnostics Clipboard Copy**:
  - Help > Debug Info (`actionDebugInfo`) copies full runtime configuration, Scintilla 5.6.9, Lexilla 5.5.5, Lua 5.5.1, Qt version, and OS details directly to the system clipboard.

#### 🛡️ Security Hardening (2025–2026 CVE Mitigations)

- **CVE-2026-20841 (Dangerous URI Protocol & Shell Hijacking Mitigation)**:
  - `NetworkSecurityPolicy::isSafeUriScheme()` blocks exploitation via malicious URI schemes (`ms-office:`, `cmd:`, `powershell:`) and direct executable execution.
- **CVE-2026-16762 (XML/JSON Nesting Depth & Parser Stack Overflow DoS Mitigation)**:
  - `ConfigIntegrityGuard::isPayloadStructureSafe()` bounds XML element nesting depth to 32 levels and caps payload size at 10 MB to prevent XML entity expansion and parser recursion crashes.
- **CVE-2026-48778 & CVE-2026-48800 (External Tool Command Tampering Mitigation)**:
  - `ConfigIntegrityGuard::isExternalCommandSafe()` blocks unauthorized privilege escalation (`sudo`, `pkexec`, `doas`) and shell pipe chaining in external tool definitions.
  - Enforces strict `0600` owner-only file permissions on session and configuration files.

#### 🌐 Localization

- Updated `i18n/SparkNotePad_en.ts` and `i18n/SparkNotePad_id.ts` with 2,017 translation entries (0 unfinished).

---

## [1.3.9] - 2026-11-07

### ⚡ Scintilla 5.6.9 & Lexilla 5.5.5 Engine Upgrade, Default Language Setting, Qt 6.2 LTS Compatibility, Linux ARM64 Matrix & 14-Pillar Hardening

SparkNotePad v1.3.9 is a major foundational milestone consolidating all architectural evolutions from v1.3.8 to v1.3.9. It upgrades the core editing subsystem to Scintilla 5.6.9 and Lexilla 5.5.5, resolves algorithmic regular expression vulnerabilities, adds user-configurable default language preferences, expands Markdown front-matter syntax styling, restores complete Qt 6.2 LTS compatibility (macOS Catalina & Ubuntu 20.04), establishes a 50+ binary release matrix with native Linux ARM64 packaging, and passes rigorous 14-pillar static analysis and code review:

#### 🆕 Added

- **Official Scintilla 5.6.9 & Lexilla 5.5.5 Core Integration**:
  - Upgraded embedded Scintilla engine from 5.6.7 to **5.6.9** (official upstream release from `scintilla.org`) in `thirdparty/scintilla` (160 files updated, 3,206 insertions).
  - Upgraded embedded Lexilla lexer library from 5.5.4 to **5.5.5** (official upstream release from `scintilla.org`) in `thirdparty/lexilla`.
  - Regenerated Qt wrapper interface (`ScintillaEdit.cpp` and `ScintillaEdit.h`) via upstream `WidgetGen.py`.
  - Full Unicode 17 character category mapping and case conversions.
  - Restored full-color rendering for XPM images.
  - Accelerated line-start position queries by skipping unnecessary line layout calculations during rapid scrolling.
  - Expanded cached lexeme buffer from 30 to 40 bytes for large identifiers and SQL query columns.
- **Markdown Front-Matter Syntax Highlighting**:
  - Implemented 3 new Lexilla 5.5.5 style definitions in `src/languages/markdown.lua` (YAML, TOML, JSON).
- **Default Language Setting for New Documents**:
  - Added `DefaultLanguage` configuration key in Preferences > Editor (`PreferencesDialog`).
"""


def main():
    files = {
        "src/doc/DOCNOTE_id.txt": DOCNOTE_ID,
        "src/doc/DOCNOTE_en.txt": DOCNOTE_EN,
        "src/doc/CHANGELOG_id.txt": CHANGELOG_ID,
        "src/doc/CHANGELOG_en.txt": CHANGELOG_EN,
        "doc/DOCNOTE_id.txt": DOCNOTE_ID,
        "doc/DOCNOTE_en.txt": DOCNOTE_EN,
        "doc/CHANGELOG_id.txt": CHANGELOG_ID,
        "doc/CHANGELOG_en.txt": CHANGELOG_EN,
        "DOCNOTE_id.txt": DOCNOTE_ID,
        "DOCNOTE_en.txt": DOCNOTE_EN,
        "CHANGELOG_id.txt": CHANGELOG_ID,
        "CHANGELOG_en.txt": CHANGELOG_EN,
    }

    for path, content in files.items():
        with open(path, "w", encoding="utf-8") as f:
            f.write(content)
        print(f"Written: {path}")

    # Also update master DOCNOTE.txt and CHANGELOG.txt
    master_docnote = DOCNOTE_ID + "\n\n---\n\n" + DOCNOTE_EN
    master_changelog = CHANGELOG_EN

    for p in ["src/doc/DOCNOTE.txt", "doc/DOCNOTE.txt", "DOCNOTE.txt"]:
        with open(p, "w", encoding="utf-8") as f:
            f.write(master_docnote)
        print(f"Updated master: {p}")

    for p in ["src/doc/CHANGELOG.txt", "doc/CHANGELOG.txt", "CHANGELOG.txt"]:
        with open(p, "w", encoding="utf-8") as f:
            f.write(master_changelog)
        print(f"Updated master: {p}")


if __name__ == "__main__":
    main()
