#!/usr/bin/env python3
"""
Generate localized DOCNOTE and CHANGELOG files for SparkNotePad v1.4.6.
Creates:
  - src/doc/DOCNOTE_id.txt (Bahasa Indonesia)
  - src/doc/DOCNOTE_en.txt (English)
  - src/doc/CHANGELOG_id.txt (Bahasa Indonesia)
  - src/doc/CHANGELOG_en.txt (English)
Synchronizes:
  - doc/ and root directory copies.
"""

DOCNOTE_ID = """# Catatan Rilis SparkNotePad — v1.4.6 (Bahasa Indonesia)

--------------------------------------------------------------------------------
================================================================================
Versi Rilis          : 1.4.6 (11 Paket Ikon Toolbar Baru [Total 39 Koleksi], Standar Bersih Notepad++ Classic, Kompresi Aset Lossless, Penguatan Linter 14 Pilar & DMG Rilis Ringan)
Tanggal Rilis        : 2026-11-09
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

## 1. Ringkasan Eksekutif (Bahasa Indonesia): Rilis v1.4.6

SparkNotePad v1.4.6 memperluas kustomisasi visual bilah alat dan standar pengemasan rilis dengan 11 paket ikon baru hasil kurasi profesional, pembersihan total bayangan/halo untuk menyamai standar bersih Notepad++ Classic, kompresi aset PNG tanpa penurunan kualitas (lossless), penguatan linter nol-cacat dengan basis data kompilasi 376-entri, dan pengemasan DMG rilis lokal yang sangat ringan:

1. 11 Paket Ikon Toolbar Baru (Total 39 Koleksi Bawaan):
   - royal_blue (Royal Blue Aero Glossy), notepad_pro (Notepad Pro Vista 7 Style), pastel (Pastel Modern), fluent_modern (Microsoft Fluent Modern), macos_bigsur_modern (macOS Big Sur Style), dark_neon_pro (Dark Neon Style), macos_glass (macOS Glassmorphism), modern_flat_colorful (Modern Flat & Colorful), glossy_3d_skeuomorphic (Glossy 3D Skeuomorphic), retro_8bit_pixel (Retro 8-Bit Pixel Art), sketchy_doodle (Sketchy Hand-Drawn Doodle).

2. Standar Bersih Notepad++ Classic (Bebas Halo & Bayangan Jatuh):
   - Menghapus seluruh bayangan jatuh lantai, halo oklusi sekitar, ubin latar belakang abu-abu, dan artefak timbul/garis tepi di seluruh 11 paket baru.
   - Ikon murni terisolasi mengambang bersih di atas bilah alat gelap permanen (#21252b) dengan margin pernapasan 18% untuk mencegah pemotongan tepi.

3. Kompresi Aset PNG Lossless & Efisiensi Disk:
   - Kuantisasi palet octree cepat dengan saluran alfa penuh dan kompresi zlib level 9 pada 1.736 aset ikon, mengurangi jejak disk sebesar 2,17 MB (pengurangan 28,9%).
   - Mengaktifkan AUTORCC_OPTIONS "--compress;9;--threshold;0" pada CMake untuk kompresi maksimal bundle Qt.

4. Pengemasan DMG Ringan Standar Ketat (~33MB, Batas Maksimal < 35MB):
   - Pembersihan framework yang tidak terpakai (QtOpenGL, QtPdf, QtQml, QtQuick, QtVirtualKeyboard) dan plugin redundan namun mempertahankan QtDBus untuk stabilitas QtGui.
   - Berkas DMG dikemas menggunakan scripts/package-dmg.sh dengan layout .DS_Store (680x440), latar belakang retina, ikon volume kustom, dan checksum CRC32 valid.

5. Penguatan Linter & Peninjauan Kode 14 Pilar:
   - Sinkronisasi basis data kompilasi 376-entri sehingga clangd, SonarLint, cppcheck, dan ekstensi C++ IDE berjalan dengan 0 eror.
   - 100% kelulusan trunk check dan ctest.

---

## 2. Arsip Ringkasan: Rilis v1.4.5

SparkNotePad v1.4.5 adalah rilis fitur dan stabilitas besar yang menghadirkan 14 pilar arsitektur fundamental yang dirancang khusus untuk para pengembang profesional, administrator sistem, dan pengguna tingkat lanjut:

1. Tumpukan Tab Tertutup & Buka Kembali Tab (Ctrl+Shift+T / Cmd+Shift+T):
   - ClosedTabsManager mengelola antrean FIFO 50 tab yang baru saja ditutup, mempertahankan posisi kursor, rasio gulir, encoding teks, mode EOL, sintaksis bahasa, dan tag warna tab.
2. Pemberian Tag & Pengelompokan Warna Tab:
   - TabColorManager menyediakan 8 preset warna berstandar aksesibilitas tinggi (Merah, Oranye, Kuning, Hijau, Sian, Biru, Ungu, Magenta) dengan status persisten melalui ApplicationSettings.
3. Manajer Autosave & Pemulihan Crash (Interval 10 Detik, Izin Ketat POSIX 0600):
   - AutosaveRecoveryManager menyimpan cadangan latar belakang setiap 10 detik untuk dokumen yang dimodifikasi, diamankan dengan izin POSIX 0600 dan checksum integritas SHA-256. Dialog interaktif CrashRecoveryDialog memungkinkan pemulihan selektif saat aplikasi dijalankan kembali.
4. Mesin Karakter Escape Pencarian Lanjutan (Extended Search):
   - ExtendedSearchParser memproses \\xHH, \\dDDD, \\0, \\b, \\f, dan \\v pada dialog Cari/Ganti dan Bilah Pencarian Cepat.
5. Audit 100+ Lexer & Ekspansi Bahasa Modern (Total 129 Bahasa):
   - Menambahkan lexer native baru: Crystal (.cr), Cangjie (.cj), LLVM IR (.ll), dan JSON5 (.json5).
6. Alur Ekspor Native Markdown / HTML / PDF via QPrinter dan Pandoc.
7. Peningkatan Pratinjau Markdown (Diagram Mermaid & Rumus Matematika LaTeX).
"""

DOCNOTE_EN = """# SparkNotePad Release DocNote — v1.4.6 (English)

--------------------------------------------------------------------------------
================================================================================
Release Version     : 1.4.6 (11 New Curated Toolbar Icon Packs [39 Total Suites], Pristine Notepad++ Classic Clean Standard, Lossless Asset Compression, 14-Pillar Linter Hardening & Lightweight Release DMG)
Release Date        : 2026-11-09
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

## 1. Executive Summary (English): Release v1.4.6

SparkNotePad v1.4.6 expands toolbar visual customization and packaging standards with 11 new professionally curated toolbar icon packs, full shadow/halo elimination matching the pristine Notepad++ Classic clean standard, lossless PNG asset compression, zero-defect linter hardening with a 376-entry compilation database, and strict lightweight DMG release packaging:

1. 11 New Curated Toolbar Icon Packs (39 Built-in Suites Total):
   - Added royal_blue (Royal Blue Aero Glossy), notepad_pro (Notepad Pro Vista 7 Style), pastel (Pastel Modern), fluent_modern (Microsoft Fluent Modern), macos_bigsur_modern (macOS Big Sur Style), dark_neon_pro (Dark Neon Style), macos_glass (macOS Glassmorphism), modern_flat_colorful (Modern Flat & Colorful), glossy_3d_skeuomorphic (Glossy 3D Skeuomorphic), retro_8bit_pixel (Retro 8-Bit Pixel Art), sketchy_doodle (Sketchy Hand-Drawn Doodle).

2. Pristine Notepad++ Classic Clean Standard (Zero Halos & Drop Shadows):
   - Stripped all floor drop shadows, ambient occlusion halos, gray background tiles, and emboss/stroke artifacts across all 11 new packs.
   - Pure isolated graphics floating cleanly on the permanent dark toolbar (#21252b) with 18% breathing margins to prevent edge clipping.

3. Lossless PNG Asset Compression & Disk Footprint Reduction:
   - Fast octree palette quantization and zlib level 9 compression applied to 1,736 PNG assets, reducing icon footprint by 2.17 MB (28.9% reduction).
   - Enabled AUTORCC_OPTIONS "--compress;9;--threshold;0" in CMake for compile-time Qt resource compression.

4. Strict Bloatware-Free Lightweight DMG Standard (~33MB, < 35MB Ceiling):
   - Pruned unused frameworks (QtOpenGL, QtPdf, QtQml, QtQuick, QtVirtualKeyboard) while retaining required QtDBus for runtime stability.
   - Packaged with scripts/package-dmg.sh, pre-baked .DS_Store (680x440), retina background, custom volume icon, and verified CRC32 checksum.

5. Zero-Defect Linter Hardening & 14-Pillar Code Quality Gate:
   - 376-entry synchronized compilation database eliminating all include and symbol diagnostics across clangd, SonarLint, cppcheck, and IDE extensions.
   - 100% clean passes on trunk check and ctest unit test runner.

---

## 2. Archive Summary: Release v1.4.5

SparkNotePad v1.4.5 represents a major feature and stabilization milestone delivering 14 foundational architectural pillars designed for professional developers, system administrators, and power users:

1. Closed Tab Stack & Reopen Closed Tab (Ctrl+Shift+T / Cmd+Shift+T):
   - ClosedTabsManager maintains a resilient 50-entry FIFO stack preserving caret position, line scroll ratio, encoding, EOL mode, language syntax styling, and tab color tags.
2. Tab Color Tagging & Grouping:
   - TabColorManager provides 8 high-contrast, accessible color presets with persistent state.
3. Autosave & Crash Recovery Manager (10s Interval, Strict 0600 POSIX Permissions):
   - AutosaveRecoveryManager writes background snapshots every 10 seconds for modified documents.
4. Comprehensive Extended Find/Replace Escape Engine:
   - ExtendedSearchParser parses \\xHH, \\dDDD, \\0, \\b, \\f, and \\v across Find/Replace and Quick Find.
5. 100+ Lexers Audit & Modern Language Expansion (129 Total Languages).
6. Native Markdown / HTML / PDF Export Pipeline.
7. Markdown Preview Enhancements (Mermaid Diagrams & LaTeX Math Formulas).
"""

CHANGELOG_ID = """# Catatan Perubahan (Changelog)

Semua perubahan penting pada **SparkNotePad** didokumentasikan di berkas ini.

Format disusun berdasarkan panduan [Keep a Changelog](https://keepachangelog.com/id/1.1.0/),
dan proyek ini sepenuhnya mematuhi [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.4.6] - 2026-11-09

### 🌟 Rilis: 11 Paket Ikon Toolbar Baru (Total 39 Koleksi), Standar Bersih Notepad++ Classic, Kompresi Aset Lossless, Penguatan Linter 14 Pilar & DMG Rilis Ringan

SparkNotePad v1.4.6 menghadirkan ekspansi kustomisasi visual bilah alat dan standar rilis berkualitas tinggi:

#### 🆕 Ditambahkan

- **11 Paket Ikon Toolbar Baru (Total 39 Koleksi Bawaan)**:
  - Menambahkan `royal_blue` (Royal Blue Aero Glossy), `notepad_pro` (Notepad Pro Vista 7 Style), `pastel` (Pastel Modern), `fluent_modern` (Microsoft Fluent Modern), `macos_bigsur_modern` (macOS Big Sur Style), `dark_neon_pro` (Dark Neon Style), `macos_glass` (macOS Glassmorphism), `modern_flat_colorful` (Modern Flat & Colorful), `glossy_3d_skeuomorphic` (Glossy 3D Skeuomorphic), `retro_8bit_pixel` (Retro 8-Bit Pixel Art), `sketchy_doodle` (Sketchy Hand-Drawn Doodle).

#### 🛠️ Diperbaiki & Diperkuat

- **Pembersihan Total Ikon Menyamai Notepad++ Classic**:
  - Menghapus bayangan jatuh, halo sekitar, ubin abu-abu, dan garis timbul pada seluruh 11 paket baru, memastikan grafis terisolasi bersih mengambang di atas bilah alat gelap permanen (`#21252b`).
- **Kompresi Aset PNG Lossless**:
  - Kompresi 1.736 aset PNG dengan kuantisasi palet octree dan zlib 9, menghemat ukuran disk sebesar 2,17 MB (28,9%).
- **Penguatan Linter & Peninjauan Kode 14 Pilar**:
  - Basis data kompilasi 376-entri menyelaraskan clangd, SonarLint, cppcheck, dan ekstensi C++ tanpa eror diagnostik.
- **Standar Pengemasan DMG Ringan Bebas Bloatware (~33MB, < 35MB)**:
  - Memangkas framework yang tidak dibutuhkan (QtPdf, QtQml, QtQuick, QtVirtualKeyboard) dan mempertahankan QtDBus; memverifikasi peluncuran biner dari DMG terpasang.

---

## [1.4.5] - 2026-11-08

### 🌟 Rilis Utama: Tumpukan Tab Tertutup, Tag Warna Tab, Pemulihan Crash, Pencarian Lanjutan, Ekspor PDF, Pratinjau Mermaid/LaTeX, Peralatan Pengembang, Pengalih Header/Source, EditorConfig, Riwayat Clipboard, Quick Switcher, Paket Ikon Aksara & Penguatan 14 Pilar

SparkNotePad v1.4.5 adalah rilis fitur dan stabilitas besar yang menghadirkan 14 pilar arsitektur fundamental yang dirancang khusus untuk para pengembang profesional, administrator sistem, dan pengguna tingkat lanjut.
"""

CHANGELOG_EN = """# Changelog

All notable changes to **SparkNotePad** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.4.6] - 2026-11-09

### 🌟 Release: 11 New Curated Toolbar Icon Packs (39 Total), Pristine Notepad++ Classic Clean Standard, Lossless Asset Compression, 14-Pillar Linter Hardening & Lightweight Release DMG

SparkNotePad v1.4.6 expands toolbar customization with 11 new professionally curated icon packs, full shadow/halo elimination to match the clean Notepad++ Classic standard, lossless PNG asset compression, zero-defect linter hardening, and strict lightweight DMG release packaging:

#### 🆕 Added

- **11 New Curated Toolbar Icon Packs (39 Built-in Suites Total)**:
  - Added `royal_blue` (Royal Blue Aero Glossy): rich skeuomorphic glass iconography.
  - Added `notepad_pro` (Notepad Pro Vista 7 Style): modern elegant Vista/7 aero translucent iconography.
  - Added `pastel` (Pastel Modern): friendly minimalist pastel color palette.
  - Added `fluent_modern` (Microsoft Fluent Modern): Windows 11 Fluent 3D layered iconography.
  - Added `macos_bigsur_modern` (macOS Big Sur Style): sleek macOS squircle aesthetics with vibrant accents.
  - Added `dark_neon_pro` (Dark Neon Style): high-contrast cyberpunk neon glowing on dark palettes.
  - Added `macos_glass` (macOS Glassmorphism): translucent frosted glass aero acrylic aesthetic.
  - Added `modern_flat_colorful` (Modern Flat & Colorful): clean vector flat style with high-contrast distinct colors.
  - Added `glossy_3d_skeuomorphic` (Glossy 3D Skeuomorphic): tactile volumetric glossy 3D clay aesthetic.
  - Added `retro_8bit_pixel` (Retro 8-Bit Pixel Art): pixel-perfect retro arcade gaming iconography.
  - Added `sketchy_doodle` (Sketchy Hand-Drawn Doodle): expressive hand-drawn artistic doodle iconography.

#### 🔧 Changed & Improved

- **Pristine Icon Sanitization Matching Notepad++ (Classic)**:
  - Stripped all floor drop shadows, ambient occlusion halos, gray background tiles, and emboss/stroke artifacts across all 11 new packs.
  - Pure isolated graphics floating cleanly on the permanent dark toolbar (`#21252b`).
  - Applied 18% breathing margins ensuring zero edge clipping and zero dirty perimeter pixels.
- **Lossless PNG Asset Optimization**:
  - Quantized 1,736 PNG assets using fast octree palette quantization and zlib level 9 compression, reducing icon disk footprint by 2.17 MB (28.9% reduction).
  - Enabled compile-time resource compression `AUTORCC_OPTIONS "--compress;9;--threshold;0"` on `toolbar_icons.qrc` and `resources.qrc`.
- **Zero-Defect Linter Hardening & 14-Pillar Code Quality Gate**:
  - Synchronized 376-entry compilation database eradicating missing include and symbol diagnostics across clangd, SonarLint, and cppcheck.
  - Passing 100% on `trunk check`, `lint-check.sh`, and `ctest` test execution.
- **Lightweight Bloatware-Free DMG Release Packaging (~33MB, < 35MB Ceiling)**:
  - Pruned unused `QtOpenGL.framework`, `QtPdf.framework`, `QtQml.framework`, `QtQuick.framework`, `QtVirtualKeyboard.framework`, and redundant plugins while retaining required `QtDBus.framework` for QtGui runtime stability.
  - Output DMG `SparkNotePad-v1.4.6-x64.dmg` packaged within the strict 30MB–34MB target (< 35MB ceiling).
  - Verified valid CRC32 checksum, preserved custom volume icon, and 680x440 `.DS_Store` presentation.

---

## [1.4.5] - 2026-11-08

### 🌟 Major Release: Closed Tab Stack, Tab Color Tagging, Crash Recovery, Extended Search, PDF Export, Mermaid/LaTeX Preview, Developer Tools, Corresponding Switcher, EditorConfig, Clipboard History & Quick Switcher, Aksara Icon Packs (28 Suites) & 14-Pillar Hardening

SparkNotePad v1.4.5 is a major feature release delivering 14 primary architectural pillars inspired by modern IDE standards and power-user workflows:

#### 🆕 Added

- **Closed Tab Stack & Reopen Closed Tab (`Ctrl+Shift+T` / `Cmd+Shift+T`)**:
  - Implemented `ClosedTabsManager` maintaining a resilient FIFO stack of up to 50 recently closed document tabs.
- **Tab Color Tagging & Grouping**:
  - Implemented `TabColorManager` providing 8 accessible color presets with high-contrast tab bar indicators.
- **Autosave & Crash Recovery Manager (10s Interval, Strict 0600 POSIX Permissions)**:
  - Implemented `AutosaveRecoveryManager` writing background snapshots every 10 seconds for modified documents.
- **Aksara Toolbar Icon Pack Suite Integration (28 Packs Total) & Menu Ergonomics**:
  - Integrated 10 curated Aksara toolbar icon packs (340 vector SVGs and PNGs).
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
    master_docnote = DOCNOTE_ID + "\\n\\n---\\n\\n" + DOCNOTE_EN
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
