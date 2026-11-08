#!/usr/bin/env python3
"""
Generate localized DOCNOTE and CHANGELOG files for SparkNotePad v1.4.5.
Creates:
  - src/doc/DOCNOTE_id.txt (Bahasa Indonesia)
  - src/doc/DOCNOTE_en.txt (English)
  - src/doc/CHANGELOG_id.txt (Bahasa Indonesia)
  - src/doc/CHANGELOG_en.txt (English)
Synchronizes:
  - doc/ and root directory copies.
"""

DOCNOTE_ID = """# Catatan Rilis SparkNotePad — v1.4.5 (Bahasa Indonesia)

--------------------------------------------------------------------------------
================================================================================
Versi Rilis          : 1.4.5 (Tumpukan Tab Tertutup, Tag Warna Tab, Pemulihan Crash, Pencarian Lanjutan, Ekspor PDF, Pratinjau Mermaid/LaTeX, Paket Ikon Aksara (28 Koleksi) & Penguatan 14 Pilar)
Tanggal Rilis        : 2026-11-08
Pengelola Utama      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organisasi           : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repositori           : https://github.com/alsyundawy/SparkNotePad
Mesin Penyunting     : Scintilla 5.6.9 & Lexilla 5.5.5 (129 Lexer Resmi)
Runtime Skrip        : Sandboxed Lua 5.5.1 & Python 3.13+ Bridge
Kerangka Kerja GUI   : Qt 6.5 LTS / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platform      : macOS 11.0–26.0 (Apple Silicon & Intel), Linux (x86_64 & ARM64), FreeBSD 13–15
Lisensi              : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Ringkasan Eksekutif (Bahasa Indonesia): Rilis v1.4.5

SparkNotePad v1.4.5 adalah rilis fitur dan stabilitas besar yang menghadirkan 14 pilar arsitektur fundamental yang dirancang khusus untuk para pengembang profesional, administrator sistem, dan pengguna tingkat lanjut:

1. Tumpukan Tab Tertutup & Buka Kembali Tab (Ctrl+Shift+T / Cmd+Shift+T):
   - ClosedTabsManager mengelola antrean FIFO 50 tab yang baru saja ditutup, mempertahankan posisi kursor, rasio gulir, encoding teks, mode EOL, sintaksis bahasa, dan tag warna tab.
   - Aksi pemulihan instan tersedia pada menu konteks tab dan menu Berkas.

2. Pemberian Tag & Pengelompokan Warna Tab:
   - TabColorManager menyediakan 8 preset warna berstandar aksesibilitas tinggi (Merah, Oranye, Kuning, Hijau, Sian, Biru, Ungu, Magenta) dengan status persisten melalui ApplicationSettings.

3. Manajer Autosave & Pemulihan Crash (Interval 10 Detik, Izin Ketat POSIX 0600):
   - AutosaveRecoveryManager menyimpan cadangan latar belakang setiap 10 detik untuk dokumen yang dimodifikasi, diamankan dengan izin POSIX 0600 dan checksum integritas SHA-256. Dialog interaktif CrashRecoveryDialog memungkinkan pemulihan selektif saat aplikasi dijalankan kembali.

4. Mesin Karakter Escape Pencarian Lanjutan (Extended Search):
   - ExtendedSearchParser memproses \\xHH (byte heksadesimal), \\dDDD (byte desimal), \\0 (byte NUL), \\b (backspace), \\f (form-feed), dan \\v (tab vertikal) pada dialog Cari/Ganti dan Bilah Pencarian Cepat.

5. Audit 100+ Lexer & Ekspansi Bahasa Modern (Total 129 Bahasa):
   - Menambahkan lexer native baru: Crystal (.cr), Cangjie (.cj), LLVM IR (.ll), dan JSON5 (.json5).
   - Memodernisasi kata kunci bahasa: C23 (_BitInt, constexpr, nullptr), Rust 2024 (gen, async gen), Swift 6 (consuming, borrowing, sending), dan TypeScript 5.8.

6. Alur Ekspor Native Markdown / HTML / PDF:
   - Implementasi DocumentExportManager menggunakan QPrinter / Qt6::PrintSupport native dan fallback Pandoc.
   - Dialog interaktif ExportOptionsDialog mendukung ukuran kertas kustom (A4, Letter, Legal), orientasi, margin, dan CSS tersemat.

7. Peningkatan Pratinjau Markdown (Diagram Mermaid & Rumus Matematika LaTeX):
   - MarkdownPreviewEnhancer merender flowchart Mermaid, diagram urutan, diagram kelas, dan state machine via kontainer web terisolasi.
   - Dukungan penuh rumus matematika LaTeX ($$...$$ dan $...$) dengan proteksi simbol mata uang dollar.
   - Fitur Two-Way SyncScroll yang menyelaraskan posisi gulir editor Scintilla dengan viewport pratinjau Markdown.

8. Paket Peralatan Pengembang (Developer Tools):
   - DeveloperToolsEngine pada menu Peralatan: JSON Prettify & Minify, URL Encode & Decode, Unicode Escape & Unescape, serta generator UUID v4.
   - Dialog interaktif TimestampConverterDialog untuk konversi epoch Unix ke format UTC dan ISO Lokal.

9. Penanda Navigasi Bilah Gulir & Pengalih Header/Source (F4 / Alt+O):
   - CorrespondingFileSwitcher mendukung peralihan cepat dua arah antara berkas header C/C++ (.h, .hpp, .hxx) dan sumber (.c, .cpp, .cxx), serta berkas kode dan tes TypeScript/JavaScript (.test.ts, .spec.ts).
   - ScrollbarNavigationMarkers menampilkan bookmark dan sorotan hasil pencarian pada track bilah gulir.

10. Parser & Penegakan Otomatis .editorconfig:
    - EditorConfigParser mendukung aturan indent_style, indent_size, tab_width, end_of_line, dan penelusuran hierarki root = true secara otomatis saat berkas dibuka.

11. Dock Riwayat Clipboard & Info Karakter Unicode:
    - Dock ClipboardHistoryDock mencatat 20 riwayat salinan terakhir, pencarian cepat, dan penyisipan satu klik ke editor.
    - Dock CharacterInfoDock menyajikan pratinjau glyph Unicode real-time, kode U+XXXX, nama karakter, kategori, nilai heksadesimal UTF-8/16/32, dan entitas HTML.

12. Pengalih Cepat Fuzzy (Cmd+P / Ctrl+P) & Dock Minimap Dokumen:
    - Dialog frameless bergaya Spotlight QuickSwitcherDialog dengan pencocokan subsekuens fuzzy pada tab terbuka dan berkas terkini.
    - DocumentMinimapDock menyediakan tinjauan dokumen ringan dengan pelacakan viewport dan batas aman berkas besar (>50 MB).
    - Integrasi berkas terkini native macOS Dock via MacServicesHandler::updateDockRecentMenu.

13. Integrasi Rangkaian Paket Ikon Toolbar Aksara (Total 28 Paket) & Ergonomi Menu:
    - Penataan 10 paket ikon toolbar Aksara kurasi (340 file SVG dan PNG vektor) mencakup macOS Big Sur, Windows XP/Vista/10/11, Ubuntu Yaru, Arch Linux, Elementary OS, dan gaya modern (total 28 paket bawaan).
    - Relokasi menu Themes: Toolbar Icon Size ditempatkan di atas Toolbar Icon Packs, memindahkan menu Kustomisasi Toolbar ke menu Tools, dan menghapus pintasan folder custom pack dari menubar.
    - Terdaftar pada toolbar_icons.qrc dan ToolbarIconManager.cpp lengkap dengan normalisasi alias ID dan cakupan unit test.

14. Penuntasan Analisis Statis SonarLint & Clang-Tidy (0 Masalah):
    - Menuntaskan seluruh 25 peringatan analisis statis SonarLint/SonarQube dan clang-tidy pada direktori src/ (misc-const-correctness, misc-override-with-different-visibility, bugprone-implicit-widening-of-multiplication-result, readability-use-concise-preprocessor-directives, readability-braces-around-statements, readability-else-after-return, performance-unnecessary-value-param).
    - Terverifikasi 0 diagnostik eror pada clangd --check, cppcheck, dan lulus 100% unit test.
"""

DOCNOTE_EN = """# SparkNotePad Release DocNote — v1.4.5 (English)

--------------------------------------------------------------------------------
================================================================================
Release Version     : 1.4.5 (Closed Tab Stack, Tab Color Tagging, Crash Recovery, Extended Search, PDF Export, Mermaid/LaTeX, Developer Tools, Corresponding Switcher, EditorConfig, Clipboard History & Quick Switcher, Aksara Icon Packs (28 Suites) & 14-Pillar Hardening)
Release Date        : 2026-11-08
Lead Maintainer     : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organization        : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repository          : https://github.com/alsyundawy/SparkNotePad
Core Editing Engine : Scintilla 5.6.9 & Lexilla 5.5.5 (129 Vendored Official Lexers)
Embedded Lua Core   : Sandboxed Lua 5.5.1 (Official Source)
Scripting Runtime   : Python 3.13+ Engine Bridge
GUI Framework       : Qt 6.5 LTS / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platforms    : macOS 11.0–26.0 (Apple Silicon & Intel), Linux (x86_64 & ARM64), FreeBSD 13–15
License             : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Executive Summary (English): Release v1.4.5

SparkNotePad v1.4.5 represents a major feature and stabilization milestone delivering 14 foundational architectural pillars designed for professional developers, system administrators, and power users:

1. Closed Tab Stack & Reopen Closed Tab (Ctrl+Shift+T / Cmd+Shift+T):
   - ClosedTabsManager maintains a resilient 50-entry FIFO stack preserving caret position, line scroll ratio, encoding, EOL mode, language syntax styling, and tab color tags.
   - Context menu and File menu actions for instant tab restoration.

2. Tab Color Tagging & Grouping:
   - TabColorManager provides 8 high-contrast, accessible color presets (Red, Orange, Yellow, Green, Cyan, Blue, Purple, Magenta) with persistent state across app restarts via ApplicationSettings.

3. Autosave & Crash Recovery Manager (10s Interval, Strict 0600 POSIX Permissions):
   - AutosaveRecoveryManager writes background snapshots every 10 seconds for modified documents, enforced with POSIX 0600 permissions and SHA-256 integrity checksums. Interactive CrashRecoveryDialog allows selective recovery or discard on application relaunch.

4. Comprehensive Extended Find/Replace Escape Engine:
   - ExtendedSearchParser parses \\xHH (hexadecimal byte), \\dDDD (decimal byte), \\0 (NUL byte), \\b (backspace), \\f (form-feed), and \\v (vertical tab) across Find/Replace and Quick Find in Extended Search mode.

5. 100+ Lexers Audit & Modern Language Expansion (129 Total Languages):
   - Added new native lexers: Crystal (.cr), Cangjie (.cj), LLVM IR (.ll), and JSON5 (.json5).
   - Modernized standard keyword sets: C23 (_BitInt, constexpr, nullptr), Rust 2024 (gen, async gen), Swift 6 (consuming, borrowing, sending), and TypeScript 5.8.

6. Native Markdown / HTML / PDF Export Pipeline:
   - Implemented DocumentExportManager with native Qt 6 QPrinter / Qt6::PrintSupport and Pandoc fallback engine.
   - Interactive ExportOptionsDialog supporting custom page sizes (A4, Letter, Legal), orientation, margins, and embedded CSS styling.

7. Markdown Preview Enhancements (Mermaid Diagrams & LaTeX Math Formulas):
   - MarkdownPreviewEnhancer renders Mermaid flowcharts, sequence diagrams, class diagrams, and state machines via client containers.
   - Full LaTeX math rendering support ($$...$$ block and $...$ inline math) with currency/escaped dollar preservation.
   - Two-Way SyncScroll matching Scintilla editor line ratios with Markdown preview viewport.

8. Developer Tools Suite:
   - DeveloperToolsEngine in Tools menu: JSON Prettify & Minify, URL Encode & Decode, Unicode Escape & Unescape, UUID v4 generator.
   - Interactive TimestampConverterDialog converting Unix Epoch timestamps (seconds/milliseconds) to human-readable UTC and Local ISO formats with copy helpers.

9. Scrollbar Navigation Markers & Corresponding Header/Source Switcher (F4 / Alt+O):
   - CorrespondingFileSwitcher supporting instant bidirectional toggle between C/C++ header (.h, .hpp, .hxx) and source (.c, .cpp, .cxx), and TypeScript/JavaScript source and test files (.test.ts, .spec.ts).
   - Added ScrollbarNavigationMarkers tracking bookmark lines and search match highlights on the scrollbar track.

10. Native .editorconfig Automatic Parser & Enforcement:
    - EditorConfigParser supporting indent_style, indent_size, tab_width, end_of_line, and root = true parent directory traversal with automatic enforcement.

11. Clipboard History Dock & Character Info Dock:
    - Dockable ClipboardHistoryDock tracking recent clipboard history (FIFO queue of 20 items), search filtering, and one-click editor insertion.
    - Dockable CharacterInfoDock displaying real-time Unicode glyph preview, code point U+XXXX, Unicode character name, category, UTF-8/16/32 hex, and HTML entities.

12. Fuzzy Quick Switcher (Cmd+P / Ctrl+P) & Document Minimap Dock:
    - Spotlight-style frameless QuickSwitcherDialog with subsequence fuzzy matching scoring open tabs and recent files.
    - DocumentMinimapDock providing a lightweight overview minimap with viewport tracking and a >50 MB large file safeguard.
    - Native macOS Dock recent documents integration via MacServicesHandler::updateDockRecentMenu.

13. Aksara Toolbar Icon Pack Suite Integration (28 Packs Total) & Menu Ergonomics:
    - Integrated 10 curated Aksara toolbar icon packs (340 vector SVGs and PNGs) across macOS Big Sur, Windows XP/Vista/10/11, Ubuntu Yaru, Arch Linux, Elementary OS, and modern styles (28 built-in packs total).
    - Relocated Themes menu: Toolbar Icon Size placed prominently above Toolbar Icon Packs, moved Customize Toolbar into the Tools menu, and removed Open Custom Icon Pack from the menubar.
    - Integrated into toolbar_icons.qrc and ToolbarIconManager.cpp with alias normalization and comprehensive unit test coverage.

14. SonarLint & Clang-Tidy Static Analysis Eradication:
    - Resolved all 25 SonarLint/SonarQube and clang-tidy static analysis warnings across src/ (misc-const-correctness, misc-override-with-different-visibility, bugprone-implicit-widening-of-multiplication-result, readability-use-concise-preprocessor-directives, readability-braces-around-statements, readability-else-after-return, performance-unnecessary-value-param).
    - Verified 0 diagnostics and 0 errors across clangd --check, cppcheck, and unit tests.
"""

CHANGELOG_ID = """# Catatan Perubahan (Changelog)

Semua perubahan penting pada **SparkNotePad** didokumentasikan di berkas ini.

Format disusun berdasarkan panduan [Keep a Changelog](https://keepachangelog.com/id/1.1.0/),
dan proyek ini sepenuhnya mematuhi [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.4.5] - 2026-11-08

### 🌟 Rilis Utama: Tumpukan Tab Tertutup, Tag Warna Tab, Pemulihan Crash, Pencarian Lanjutan, Ekspor PDF, Pratinjau Mermaid/LaTeX, Peralatan Pengembang, Pengalih Header/Source, EditorConfig, Riwayat Clipboard, Quick Switcher, Paket Ikon Aksara & Penguatan 14 Pilar

SparkNotePad v1.4.5 adalah rilis fitur dan stabilitas besar yang menghadirkan 14 pilar arsitektur fundamental yang dirancang khusus untuk para pengembang profesional, administrator sistem, dan pengguna tingkat lanjut:

#### 🆕 Ditambahkan

- **Tumpukan Tab Tertutup & Buka Kembali Tab (`Ctrl+Shift+T` / `Cmd+Shift+T`)**:
  - Implementasi `ClosedTabsManager` yang mengelola antrean FIFO hingga 50 tab terakhir dengan pemulihan posisi kursor, rasio scroll, encoding, dan warna tab.
- **Pemberian Tag & Pengelompokan Warna Tab**:
  - Implementasi `TabColorManager` dengan 8 preset warna aksesibel dan indikator visual kontras tinggi pada tab bar.
- **Manajer Autosave & Pemulihan Crash**:
  - Implementasi `AutosaveRecoveryManager` yang menyimpan snapshot latar belakang setiap 10 detik dengan izin POSIX 0600 dan checksum integritas SHA-256.
- **Integrasi Rangkaian Paket Ikon Toolbar Aksara (Total 28 Paket) & Ergonomi Menu**:
  - Penataan 10 paket ikon toolbar Aksara kurasi (340 file SVG dan PNG vektor) mencakup macOS Big Sur, Windows XP/Vista/10/11, Ubuntu Yaru, Arch Linux, Elementary OS, dan gaya modern (total 28 paket bawaan).
  - Relokasi menu Themes: Toolbar Icon Size ditempatkan di atas Toolbar Icon Packs, memindahkan menu Kustomisasi Toolbar ke menu Tools, dan menghapus Open Custom Icon Pack dari menubar.
- **Ekspor Dokumen Markdown / PDF & Peningkatan Pratinjau**:
  - Ekspor PDF/HTML native via `QPrinter`, rendering diagram Mermaid, dan dukungan rumus matematika LaTeX penuh.
- **Peralatan Pengembang & Utilitas**:
  - Konverter timestamp, JSON prettify/minify, URL encode/decode, escape Unicode, UUID v4, parser `.editorconfig`, dan dock Riwayat Clipboard.

#### 🛠️ Diperbaiki & Diperkuat

- **Penuntasan Analisis Statis SonarLint & Clang-Tidy**:
  - Menuntaskan seluruh 25 peringatan analisis statis SonarLint/SonarQube dan clang-tidy tanpa eror.
- **Kepatuhan Kualitas 14 Pilar**:
  - 100% lulus uji unit `SparkNotePadTests` dan kompilasi aman tanpa Ninja.

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
  - Algoritma RegexSafetyEngine::buildFuzzySubsequencePattern() yang mengubah input query menjadi regex linier deterministik tanpa backtracking.
  - Kebal terhadap serangan ReDoS (Regular Expression Denial of Service).
"""

CHANGELOG_EN = """# Changelog

All notable changes to **SparkNotePad** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.4.5] - 2026-11-08

### 🌟 Major Release: Closed Tab Stack, Tab Color Tagging, Crash Recovery, Extended Search, PDF Export, Mermaid/LaTeX Preview, Developer Tools, Corresponding Switcher, EditorConfig, Clipboard History & Quick Switcher, Aksara Icon Packs (28 Suites) & 14-Pillar Hardening

SparkNotePad v1.4.5 is a major feature release delivering 14 primary architectural pillars inspired by modern IDE standards and power-user workflows:

#### 🆕 Added

- **Closed Tab Stack & Reopen Closed Tab (`Ctrl+Shift+T` / `Cmd+Shift+T`)**:
  - Implemented `ClosedTabsManager` maintaining a resilient FIFO stack of up to 50 recently closed document tabs.
  - Automatically preserves and restores caret position, line scroll ratio, text encoding, EOL mode, language syntax styling, and tab color tags.
- **Tab Color Tagging & Grouping**:
  - Implemented `TabColorManager` providing 8 accessible color presets with high-contrast tab bar indicators.
- **Autosave & Crash Recovery Manager (10s Interval, Strict 0600 POSIX Permissions)**:
  - Implemented `AutosaveRecoveryManager` writing background snapshots every 10 seconds for modified documents.
  - Protected with strict `0600` POSIX file permissions and SHA-256 integrity checksums.
  - Interactive `CrashRecoveryDialog` enabling selective document restoration or discard on startup.
- **Aksara Toolbar Icon Pack Suite Integration (28 Packs Total) & Menu Ergonomics**:
  - Integrated 10 curated Aksara toolbar icon packs (340 vector SVGs and 32-bit PNGs): `aksara-arch-linux`, `aksara-elementary-os`, `aksara-icon-pack-rounded`, `aksara-macos-bigsur`, `aksara-npp-vista`, `aksara-npp-win10`, `aksara-npp-win11`, `aksara-npp-xp`, `aksara-ubuntu-yaru`, `aksara-vista-aero`.
  - Registered packs in `toolbar_icons.qrc` and `ToolbarIconManager.cpp`, establishing the built-in toolbar icon collection at 28 packs with robust alias normalization and unit test coverage.
  - Relocated Themes menu: Toolbar Icon Size placed prominently above Toolbar Icon Packs, moved Customize Toolbar into the Tools menu (`Tools > Customize Toolbar Buttons...`), and removed Open Custom Icon Pack from the menubar.
- **Native Markdown / HTML / PDF Export Pipeline**:
  - Implemented `DocumentExportManager` with native Qt 6 `QPrinter` / `Qt6::PrintSupport` and Pandoc fallback engine.
  - Interactive `ExportOptionsDialog` supporting custom page sizes (A4, Letter, Legal), orientation, margins, and embedded CSS styling.
- **Markdown Preview Enhancements (Mermaid Diagrams & LaTeX Math Formulas)**:
  - Implemented `MarkdownPreviewEnhancer` rendering Mermaid flowcharts, sequence diagrams, class diagrams, and state machines.
  - Full LaTeX math rendering support (`$$...$$` block and `$...$` inline math) with currency/escaped dollar preservation.
  - Two-Way SyncScroll matching Scintilla editor line ratios with Markdown preview viewport.
- **Developer Tools Suite**:
  - Implemented `DeveloperToolsEngine` in Tools menu: JSON Prettify & Minify, URL Encode & Decode, Unicode Escape & Unescape, UUID v4 generator.
  - Interactive `TimestampConverterDialog` converting Unix Epoch timestamps to human-readable UTC and Local ISO formats with copy helpers.
- **Scrollbar Navigation Markers & Corresponding Header/Source Switcher (`F4` / `Alt+O`)**:
  - Implemented `CorrespondingFileSwitcher` supporting instant bidirectional toggle between C/C++ header and source, and TypeScript/JavaScript source and test files.
  - Added `ScrollbarNavigationMarkers` tracking bookmark lines and search match highlights on the scrollbar track.
- **Native `.editorconfig` Automatic Parser & Enforcement**:
  - Implemented `EditorConfigParser` supporting `indent_style`, `indent_size`, `tab_width`, `end_of_line`, and `root = true` parent directory traversal.
- **Clipboard History Dock & Character Info Dock**:
  - Implemented dockable `ClipboardHistoryDock` tracking recent clipboard history (FIFO queue of 20 items), search filtering, and one-click editor insertion.
  - Implemented dockable `CharacterInfoDock` displaying real-time Unicode glyph preview, code point `U+XXXX`, Unicode character name, category, UTF-8/16/32 hex, and HTML entities.
- **Fuzzy Quick Switcher (`Cmd+P` / `Ctrl+P`) & Document Minimap Dock**:
  - Implemented Spotlight-style frameless `QuickSwitcherDialog` with subsequence fuzzy matching scoring open tabs and recent files.
  - Implemented `DocumentMinimapDock` providing a lightweight overview minimap with viewport tracking and a >50 MB large file safeguard.

#### 🛠️ Fixed & Hardened

- **SonarLint & Clang-Tidy Static Analysis Resolution**:
  - Resolved all 25 SonarLint/SonarQube and clang-tidy static analysis warnings across `src/` (`misc-const-correctness`, `misc-override-with-different-visibility`, `bugprone-implicit-widening-of-multiplication-result`, `readability-use-concise-preprocessor-directives`, `readability-braces-around-statements`, `readability-else-after-return`, `performance-unnecessary-value-param`).
  - Fixed tweak AST diagnostics and control flow in `Finder.cpp` and `BetterMultiSelection.cpp`.
- **14-Pillar Quality Verification Gate**:
  - Verified 0 errors across `cppcheck`, `clangd --check`, `lint-check.sh`, and unit test suite `SparkNotePadTests`.
  - Built strictly with standard CMake Unix Makefiles without Ninja.

---

## [1.4.0] - 2026-11-07

### 🚀 Line Spacing, Enriched Tab Tooltips, Fuzzy Search, Batch Macros, MCP Server & 2025–2026 Security Hardening

SparkNotePad v1.4.0 introduces essential productivity features inspired by Nextpad++ research and modern editor capabilities, while fortifying system security against 2025–2026 vulnerabilities:

#### 🆕 Added

- **Configurable Line Spacing (Extra Ascent & Descent)**:
  - Added user-customizable extra ascent and extra descent (0–30 px) under Preferences > Margins/Padding.
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
