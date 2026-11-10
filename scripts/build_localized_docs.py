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
Versi Rilis          : 1.4.6 (30 Paket Ikon Toolbar Baru [Total 69 Koleksi], Ergonomi Menu Konteks, Opsi Preferensi Umum, Perombakan Syntax Highlighting WCAG 2.1 & DMG Rilis 30MB–40MB)
Tanggal Rilis        : 2026-11-10
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

SparkNotePad v1.4.6 menghadirkan peningkatan besar pada kustomisasi visual bilah alat, kenyamanan menu konteks editor, opsi preferensi umum, perombakan pewarnaan sintaksis berstandar WCAG 2.1, serta standardisasi pengemasan macOS DMG 30MB–40MB tanpa bloatware:

1. 30 Paket Ikon Toolbar Modern Baru (Total 69 Koleksi Bawaan):
   - Menambahkan 30 paket ikon modern beresolusi tinggi (16x16, 20x20, 24x24, 32x32, 48x48, 64x64): 01-macos-big-sur, 02-windows-11-fluent, 03-arc-browser-glow, 04-nordic-frost-minimal, 05-dracula-cyber-neon, 06-catppuccin-macchiato, 07-tokyo-night-storm, 08-synthwave-80s, 09-monokai-pro-vivid, 10-solarized-accent, 11-retro-commodore-64, 12-retro-win95-classic, 13-matrix-phosphor-green, 14-gameboy-nostalgia, 15-pixel-dungeon-rpg, 16-claymorphism-3d, 17-skeuomorphic-leather-brass, 18-neu-brutalism-bold, 19-isometric-voxel-3d, 20-origami-paper-fold, 21-cyberpunk-2077-yellow, 22-lofi-aesthetic-grain, 23-vaporwave-pastel-sunset, 24-astronomy-deep-space, 25-botanical-nature-green, 26-blueprint-schematic, 27-chalkboard-handdrawn, 28-gouache-art-paint, 29-stained-glass-mosaic, 30-swiss-minimal-3d.
   - Bersih dari bayangan lantai, halo abu-abu, dan artefak timbul, mengambang sempurna di atas toolbar gelap permanen (#21252b).

2. Penyempurnaan Ergonomi Menu Konteks Klik Kanan:
   - Menambahkan menu Tempel & Sesuaikan Gaya (Paste & Match Style) untuk menempel teks bersih tanpa format.
   - Menambahkan menu Cari di Google (Search on Google) langsung dari teks yang diblok atau kata di bawah kursor.
   - Menyederhanakan konversi huruf menjadi Huruf Besar (UPPERCASE) dan Huruf Kecil (lowercase) langsung tanpa submenu bertingkat, serta membersihkan opsi run/bash yang tidak relevan.

3. Opsi Baru pada Preferensi Umum:
   - Pengaturan aktif/nonaktif Deteksi Encoding Otomatis (Auto-detection character encoding).
   - Pengaturan aktif/nonaktif Konfirmasi Simpan Semua (Save All confirmation dialog).

4. Perombakan Mesin Syntax Highlighting & Tema (WCAG 2.1):
   - Perhitungan luminansi relatif dan rasio kontras otomatis dengan jaminan kontras minimal 4.0:1 pada teks utama di seluruh 70+ tema editor.
   - Pewarnaan semantik khusus keluarga bahasa (C/C++, Python, Web/HTML/JS/CSS, Markdown H1-H6, SQL, Shell, YAML, dll.).
   - Lencana Emas Menarik (#FFD700) untuk penanda komentar penting (TODO, FIXME, BUG, HACK, NOTE).
   - Warna amber hangat untuk urutan karakter escape (\\n, \\t, \\xHH).

5. Standardisasi Pengemasan macOS DMG Ringan (30MB–40MB):
   - Mempertahankan integritas binary universal resmi Qt tanpa lipo -thin berlebih, mempertahankan plugin format gambar standar (libqjpeg, libqgif, libqwebp).
   - Menghasilkan DMG rilis 38MB–39MB dengan tata letak visual kustom .DS_Store, background retina, dan checksum CRC32 valid.

---

## 2. Arsip Ringkasan: Rilis v1.4.5

SparkNotePad v1.4.5 adalah rilis fitur dan stabilitas besar yang menghadirkan 14 pilar arsitektur fundamental yang dirancang khusus untuk para pengembang profesional, administrator sistem, dan pengguna tingkat lanjut:

1. Tumpukan Tab Tertutup & Buka Kembali Tab (Ctrl+Shift+T / Cmd+Shift+T):
   - ClosedTabsManager mengelola antrean FIFO 50 tab yang baru saja ditutup, mempertahankan posisi kursor, rasio gulir, encoding teks, mode EOL, sintaksis bahasa, dan tag warna tab.
2. Pemberian Tag & Pengelompokan Warna Tab:
   - TabColorManager menyediakan 8 preset warna berstandar aksesibilitas tinggi (Merah, Oranye, Kuning, Hijau, Sian, Biru, Ungu, Magenta) dengan status persisten melalui ApplicationSettings.
3. Manajer Autosave & Pemulihan Crash (Interval 10 Detik, Izin Ketat POSIX 0600):
   - AutosaveRecoveryManager menyimpan cadangan latar belakang setiap 10 detik untuk dokumen yang dimodifikasi, diamankan dengan izin POSIX 0600 dan checksum integritas SHA-256.
4. Mesin Karakter Escape Pencarian Lanjutan (Extended Search):
   - ExtendedSearchParser memproses \\xHH, \\dDDD, \\0, \\b, \\f, dan \\v pada dialog Cari/Ganti dan Bilah Pencarian Cepat.
5. Audit 100+ Lexer & Ekspansi Bahasa Modern (Total 129 Bahasa).
6. Alur Ekspor Native Markdown / HTML / PDF via QPrinter dan Pandoc.
7. Peningkatan Pratinjau Markdown (Diagram Mermaid & Rumus Matematika LaTeX).
"""

DOCNOTE_EN = """# SparkNotePad Release DocNote — v1.4.6 (English)

--------------------------------------------------------------------------------
================================================================================
Release Version     : 1.4.6 (30 New Modern Toolbar Icon Packs [69 Total Suites], Context Menu Ergonomics, General Preferences, WCAG 2.1 Syntax Highlighting Overhaul & 30MB–40MB Standard DMG)
Release Date        : 2026-11-10
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

SparkNotePad v1.4.6 delivers major enhancements across visual customization, editor context menu ergonomics, general preferences options, WCAG 2.1 contrast mathematical safety for syntax highlighting, and canonical 30MB–40MB macOS DMG packaging:

1. 30 New Modern Toolbar Icon Packs (69 Built-in Suites Total):
   - Added 30 high-resolution curated modern icon packs (16x16, 20x20, 24x24, 32x32, 48x48, 64x64): 01-macos-big-sur, 02-windows-11-fluent, 03-arc-browser-glow, 04-nordic-frost-minimal, 05-dracula-cyber-neon, 06-catppuccin-macchiato, 07-tokyo-night-storm, 08-synthwave-80s, 09-monokai-pro-vivid, 10-solarized-accent, 11-retro-commodore-64, 12-retro-win95-classic, 13-matrix-phosphor-green, 14-gameboy-nostalgia, 15-pixel-dungeon-rpg, 16-claymorphism-3d, 17-skeuomorphic-leather-brass, 18-neu-brutalism-bold, 19-isometric-voxel-3d, 20-origami-paper-fold, 21-cyberpunk-2077-yellow, 22-lofi-aesthetic-grain, 23-vaporwave-pastel-sunset, 24-astronomy-deep-space, 25-botanical-nature-green, 26-blueprint-schematic, 27-chalkboard-handdrawn, 28-gouache-art-paint, 29-stained-glass-mosaic, 30-swiss-minimal-3d.
   - Clean, isolated iconography floating on the permanent dark toolbar (#21252b) with 18% breathing margins preventing clipping.

2. Context Menu Ergonomics Overhaul:
   - Added "Paste & Match Style" (pasteAndMatchStyle) to strip formatting and match current indentation.
   - Added "Search on Google" (searchOnGoogle) to instantly query selected text or word under cursor in default browser.
   - Streamlined case conversion to direct UPPERCASE and lowercase actions, eliminating cluttered run/bash options.

3. General Preferences Additions:
   - Added option to Enable / Disable Character Encoding Auto-detection (enableEncodingAutoDetection).
   - Added option to Enable / Disable Save All Confirmation dialog (enableSaveAllConfirmation).

4. Syntax Highlighting & Theme Engine Mathematical Overhaul:
   - Integrated WCAG 2.1 relative luminance and contrast calculations guaranteeing minimum 4.0:1 contrast ratio across all 70+ themes.
   - Semantic styling tailored to language families (C/C++, Python, Web/HTML/JS/CSS, Markdown progressive H1-H6, SQL, Shell, YAML, etc.).
   - Gold Badges (#FFD700) for attention-grabbing comment markers (TODO, FIXME, BUG, HACK, NOTE).
   - Warm golden amber for string escape sequences (\\n, \\t, \\xHH).

5. Standard Lightweight macOS DMG Packaging (30MB–40MB Standard):
   - Preserved universal binary framework integrity without destructive lipo thinning, retaining standard image format decoders.
   - Packaged 38MB–39MB DMG with custom retina background, layout preset .DS_Store (680x440), and verified CRC32 checksum.

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

## [1.4.6] - 2026-11-10

### 🌟 Rilis: 30 Paket Ikon Toolbar Baru (Total 69 Koleksi), Ergonomi Menu Konteks, Opsi Preferensi Umum, Perombakan Syntax Highlighting WCAG 2.1 & DMG Rilis Standar 30MB–40MB

SparkNotePad v1.4.6 menghadirkan ekspansi kustomisasi visual bilah alat, peningkatan kenyamanan editor, dan standar pengemasan berkualitas tinggi:

#### 🆕 Ditambahkan (Added)

- **30 Paket Ikon Toolbar Modern Baru (Total 69 Koleksi Bawaan)**:
  - Menambahkan 30 set ikon modern multi-resolusi (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
    `01-macos-big-sur`, `02-windows-11-fluent`, `03-arc-browser-glow`, `04-nordic-frost-minimal`, `05-dracula-cyber-neon`, `06-catppuccin-macchiato`, `07-tokyo-night-storm`, `08-synthwave-80s`, `09-monokai-pro-vivid`, `10-solarized-accent`, `11-retro-commodore-64`, `12-retro-win95-classic`, `13-matrix-phosphor-green`, `14-gameboy-nostalgia`, `15-pixel-dungeon-rpg`, `16-claymorphism-3d`, `17-skeuomorphic-leather-brass`, `18-neu-brutalism-bold`, `19-isometric-voxel-3d`, `20-origami-paper-fold`, `21-cyberpunk-2077-yellow`, `22-lofi-aesthetic-grain`, `23-vaporwave-pastel-sunset`, `24-astronomy-deep-space`, `25-botanical-nature-green`, `26-blueprint-schematic`, `27-chalkboard-handdrawn`, `28-gouache-art-paint`, `29-stained-glass-mosaic`, `30-swiss-minimal-3d`.
- **Fitur Baru Menu Konteks Editor**:
  - Menu **Tempel & Sesuaikan Gaya (Paste & Match Style)**: menempel teks tanpa format bawaan dan menyesuaikan indentasi editor.
  - Menu **Cari di Google (Search on Google)**: membuka browser untuk mencari teks terpilih secara langsung.
  - Menu konversi huruf langsung: **Huruf Besar (UPPERCASE)** dan **Huruf Kecil (lowercase)**.
- **Opsi Baru Preferensi Umum (General Preferences)**:
  - Opsi aktifkan / nonaktifkan deteksi encoding otomatis (`enableEncodingAutoDetection`).
  - Opsi aktifkan / nonaktifkan konfirmasi simpan semua (`enableSaveAllConfirmation`).
- **Perombakan Pewarnaan Sintaksis WCAG 2.1**:
  - Garansi matematis kontras rasio minimal 4.0:1 pada seluruh 70+ tema editor.
  - Lencana Emas (#FFD700) untuk TODO, FIXME, BUG, HACK, NOTE.
  - Warna amber hangat untuk urutan karakter escape string (\\n, \\t, \\xHH).

#### 🔧 Diubah & Ditingkatkan (Changed & Improved)

- **Standar Pengemasan macOS DMG 30MB–40MB**:
  - Kalibrasi ukuran DMG rilis ke 38MB–39MB dengan mempertahankan pustaka universal Qt dan plugin format gambar standar.
  - Pengujian integritas checksum CRC32 dan verifikasi peluncuran aplikasi otomatis.
- **Pembersihan Menu Konteks**:
  - Menghapus opsi Run dan Bash encode/decode dari menu klik kanan agar lebih ringkas dan fokus.

---

## [1.4.5] - 2026-11-08

### 🌟 Major Release: Closed Tab Stack, Tab Color Tagging, Crash Recovery, Extended Search, PDF Export, Mermaid/LaTeX Preview, Developer Tools, Corresponding Switcher, EditorConfig, Clipboard History & Quick Switcher, Aksara Icon Packs (28 Suites) & 14-Pillar Hardening

SparkNotePad v1.4.5 adalah rilis fitur besar yang menghadirkan 14 pilar arsitektur fundamental yang dirancang untuk alur kerja pengguna tingkat lanjut.
"""

CHANGELOG_EN = """# Changelog

All notable changes to **SparkNotePad** are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.4.6] - 2026-11-10

### 🌟 Release: 30 New Modern Toolbar Icon Packs (69 Total Suites), Context Menu Ergonomics, General Preferences Options, WCAG 2.1 Syntax Highlighting Overhaul & Standard 30MB–40MB DMG Packaging

SparkNotePad v1.4.6 delivers major enhancements across visual customization, editor context menu ergonomics, general preferences options, WCAG 2.1 contrast mathematical safety for syntax highlighting, and canonical 30MB–40MB macOS DMG packaging:

#### 🆕 Added

- **30 New Modern Toolbar Icon Packs (69 Built-in Suites Total)**:
  - Added 30 high-resolution multi-scale icon packs (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
    `01-macos-big-sur`, `02-windows-11-fluent`, `03-arc-browser-glow`, `04-nordic-frost-minimal`, `05-dracula-cyber-neon`, `06-catppuccin-macchiato`, `07-tokyo-night-storm`, `08-synthwave-80s`, `09-monokai-pro-vivid`, `10-solarized-accent`, `11-retro-commodore-64`, `12-retro-win95-classic`, `13-matrix-phosphor-green`, `14-gameboy-nostalgia`, `15-pixel-dungeon-rpg`, `16-claymorphism-3d`, `17-skeuomorphic-leather-brass`, `18-neu-brutalism-bold`, `19-isometric-voxel-3d`, `20-origami-paper-fold`, `21-cyberpunk-2077-yellow`, `22-lofi-aesthetic-grain`, `23-vaporwave-pastel-sunset`, `24-astronomy-deep-space`, `25-botanical-nature-green`, `26-blueprint-schematic`, `27-chalkboard-handdrawn`, `28-gouache-art-paint`, `29-stained-glass-mosaic`, `30-swiss-minimal-3d`.
- **Editor Context Menu Ergonomics**:
  - Added **Paste & Match Style** (`pasteAndMatchStyle`) to strip formatting and match current editor indentation.
  - Added **Search on Google** (`searchOnGoogle`) to query selected text or word under cursor in default browser.
  - Streamlined case conversion to direct **UPPERCASE** and **lowercase** actions without multi-tier submenus.
- **General Preferences Options**:
  - Added option to Enable / Disable Character Encoding Auto-detection (`enableEncodingAutoDetection`).
  - Added option to Enable / Disable Save All Confirmation dialog (`enableSaveAllConfirmation`).
- **WCAG 2.1 Contrast Safety & Syntax Highlighting Overhaul**:
  - Integrated WCAG 2.1 relative luminance and contrast calculations guaranteeing minimum 4.0:1 contrast ratio across all 70+ themes.
  - Gold Badges (`#FFD700`, bold) for TODO, FIXME, BUG, HACK, NOTE.
  - Warm golden amber for string escape sequences (`\\n`, `\\t`, `\\xHH`).
  - Theme-synchronized rounded border box indicator for SmartHighlighter.

#### 🔧 Changed & Improved

- **Canonical 30MB–40MB macOS DMG Packaging**:
  - Calibrated release DMG packaging to 38MB–39MB while retaining universal Qt libraries and standard image format plugins.
  - Automated verification gate with valid CRC32 checksum and runtime launch test.
- **Context Menu Noise Reduction**:
  - Removed Run and Bash encode/decode actions from right-click context menu for cleaner ergonomics.

---

## [1.4.5] - 2026-11-08

### 🌟 Major Release: Closed Tab Stack, Tab Color Tagging, Crash Recovery, Extended Search, PDF Export, Mermaid/LaTeX Preview, Developer Tools, Corresponding Switcher, EditorConfig, Clipboard History & Quick Switcher, Aksara Icon Packs (28 Suites) & 14-Pillar Hardening

SparkNotePad v1.4.5 is a major feature release delivering 14 foundational architectural pillars designed for professional developers, system administrators, and power users.
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
