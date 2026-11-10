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
Versi Rilis          : 1.4.6 (Ekspansi 41 Paket Ikon Toolbar Baru [Total 69 Koleksi Bawaan], Ergonomi Menu Konteks, Opsi Preferensi Umum, Perombakan Syntax Highlighting WCAG 2.1 & Standar DMG Rilis 30MB–40MB)
Tanggal Rilis        : 2026-11-10
Pengelola Utama      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organisasi           : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repositori           : https://github.com/alsyundawy/SparkNotePad
Mesin Penyunting     : Scintilla 5.6.9 & Lexilla 5.5.5 (129 Lexer Resmi)
Runtime Skrip        : Sandboxed Lua 5.5.1 & Python 3.13+ Bridge
Kerangka Kerja GUI   : Qt 6.5 LTS / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platform      : macOS 11.0–26.0 (Apple Silicon & Intel), Linux (x86_64 & ARM64), FreeBSD 13–15
Lisensi              : GNU General Public License v3.0 (GPL-3.0-or-later)
===============================================================================
--------------------------------------------------------------------------------

---

## 1. Ringkasan Eksekutif: Perubahan dari v1.4.5 ke v1.4.6

SparkNotePad v1.4.6 merupakan pembaruan fitur, ergonomi antarmuka, dan pemolesan visual komprehensif yang menyempurnakan rilis v1.4.5 melalui lima pilar rekayasa utama:

1. Ekspansi Paket Ikon Toolbar (Total 69 Koleksi Bawaan):
   - Dari basis 28 paket di v1.4.5, ditambahkan 41 paket ikon baru multi-resolusi beresolusi tinggi (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
     * 11 Paket Ikon Tematik: royal_blue, notepad_pro, pastel, fluent_modern, macos_bigsur_modern, dark_neon_pro, macos_glass, modern_flat_colorful, glossy_3d_skeuomorphic, retro_8bit_pixel, sketchy_doodle.
     * 30 Paket Ikon Modern: 01-macos-big-sur, 02-windows-11-fluent, 03-arc-browser-glow, 04-nordic-frost-minimal, 05-dracula-cyber-neon, 06-catppuccin-macchiato, 07-tokyo-night-storm, 08-synthwave-80s, 09-monokai-pro-vivid, 10-solarized-accent, 11-retro-commodore-64, 12-retro-win95-classic, 13-matrix-phosphor-green, 14-gameboy-nostalgia, 15-pixel-dungeon-rpg, 16-claymorphism-3d, 17-skeuomorphic-leather-brass, 18-neu-brutalism-bold, 19-isometric-voxel-3d, 20-origami-paper-fold, 21-cyberpunk-2077-yellow, 22-lofi-aesthetic-grain, 23-vaporwave-pastel-sunset, 24-astronomy-deep-space, 25-botanical-nature-green, 26-blueprint-schematic, 27-chalkboard-handdrawn, 28-gouache-art-paint, 29-stained-glass-mosaic, 30-swiss-minimal-3d.
   - Mengusung standar Notepad++ Classic yang bersih: tanpa bayangan lantai (drop shadows), tanpa halo abu-abu, dan tanpa garis timbul kasar, dengan margin nafas 18% agar ikon tidak terpotong pada layar Retina.
   - Invarian Toolbar Gelap Permanen: Latar toolbar utama dikunci pada warna gelap (#21252b) di seluruh mode tampilan dan tema untuk menjaga kontras ikon.

2. Peningkatan Ergonomi Menu Konteks Editor:
   - Tempel & Cocokkan Format (Paste & Match Style, Cmd+Alt+Shift+V / Ctrl+Shift+V): menempel teks dari clipboard dengan menghapus format asing serta menyesuaikan tingkat indentasi buffer editor saat ini.
   - Cari di Google (Search on Google, Cmd+Alt+G / Ctrl+Alt+G): menelusuri teks yang disorot atau kata di bawah kursor langsung ke peramban bawaan sistem.
   - Konversi Huruf Langsung: opsi HURUF BESAR (UPPERCASE) dan huruf kecil (lowercase) kini langsung dapat diakses pada menu konteks tanpa submenu bertingkat.
   - Pembersihan Menu: menghapus opsi usang Run dan Base64 Encode/Decode dari menu klik kanan agar alur kerja penulisan lebih bersih.

3. Penambahan Opsi Preferensi Umum (General Preferences):
   - Opsi sakelar Deteksi Otomatis Enkoding Karakter (enableEncodingAutoDetection) pada tab Pengaturan Umum.
   - Opsi sakelar Dialog Konfirmasi Simpan Semua (enableSaveAllConfirmation) pada tab Pengaturan Umum.

4. Perombakan Total Syntax Highlighting & Keselamatan Kontras WCAG 2.1:
   - Kalkulasi rasio kontras matematis berdasarkan luminansi relatif (0.2126*R + 0.7152*G + 0.0722*B), menjamin rasio kontras teks utama minimal 4.0:1 pada seluruh 70+ tema editor.
   - Lencana Emas Visibilitas Tinggi (#FFD700) untuk penanda penting: TODO, FIXME, BUG, HACK, NOTE.
   - Warna kuning amber hangat untuk urutan karakter escape string (\\n, \\t, \\xHH).
   - Kotak indikator penanda kemunculan kata kunci tersinkronisasi dinamis dengan tema aktif.

5. Standardisasi Pengemasan macOS DMG Ringan (30MB–40MB):
   - Mengkalibrasi ukuran berkas rilis DMG berada dalam rentang ketat 30MB–40MB (38MB–39MB) dengan mempertahankan keutuhan pustaka universal Qt dan plugin format gambar standar (libqjpeg, libqgif, libqwebp).
   - Menggunakan format kanonikal native hdiutil dengan preset tata letak .DS_Store (680x440), latar retina .background/dmg-background@2x.png, dan verifikasi checksum CRC32.

6. Penguatan Kualitas Kode 14 Pilar & Audit Linter:
   - 100% lulus bersih pada pengujian cppcheck (171 berkas, 0 error), clangd, clang-tidy, dan trunk check.
   - Kepatuhan penuh SonarLint: pembersihan parameter lambda dan penegakan pointer-to-const correctness.
   - 100% lulus uji unit otomatis (ctest --test-dir build).
   - 100% paritas terjemahan antarmuka dwibahasa (Bahasa Inggris dan Bahasa Indonesia).

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
Release Version     : 1.4.6 (41 New Modern Toolbar Icon Packs [69 Built-in Suites Total], Context Menu Ergonomics, General Preferences, WCAG 2.1 Syntax Highlighting Overhaul & 30MB–40MB Standard DMG)
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

## 1. Executive Summary: Changes from v1.4.5 to v1.4.6

SparkNotePad v1.4.6 delivers major enhancements across visual customization, editor context menu ergonomics, general preferences options, WCAG 2.1 contrast mathematical safety for syntax highlighting, and canonical 30MB–40MB macOS DMG packaging:

1. Toolbar Icon Packs Expansion (69 Built-in Suites Total):
   - Expanded from the 28 icon packs in v1.4.5 by adding 41 new multi-resolution icon suites across all resolutions (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
     * 11 Thematic Packs: royal_blue, notepad_pro, pastel, fluent_modern, macos_bigsur_modern, dark_neon_pro, macos_glass, modern_flat_colorful, glossy_3d_skeuomorphic, retro_8bit_pixel, sketchy_doodle.
     * 30 Modern Packs: 01-macos-big-sur, 02-windows-11-fluent, 03-arc-browser-glow, 04-nordic-frost-minimal, 05-dracula-cyber-neon, 06-catppuccin-macchiato, 07-tokyo-night-storm, 08-synthwave-80s, 09-monokai-pro-vivid, 10-solarized-accent, 11-retro-commodore-64, 12-retro-win95-classic, 13-matrix-phosphor-green, 14-gameboy-nostalgia, 15-pixel-dungeon-rpg, 16-claymorphism-3d, 17-skeuomorphic-leather-brass, 18-neu-brutalism-bold, 19-isometric-voxel-3d, 20-origami-paper-fold, 21-cyberpunk-2077-yellow, 22-lofi-aesthetic-grain, 23-vaporwave-pastel-sunset, 24-astronomy-deep-space, 25-botanical-nature-green, 26-blueprint-schematic, 27-chalkboard-handdrawn, 28-gouache-art-paint, 29-stained-glass-mosaic, 30-swiss-minimal-3d.
   - Clean Notepad++ Classic standard: completely eradicated floor drop shadows, halos, gray background tiles, and edge clipping across all packs, with 18% breathing margins preventing clipping on Retina displays.
   - Permanent Dark Toolbar Invariant: `#mainToolBar` permanently locked to `#21252b` across all themes and appearance modes to guarantee icon contrast.

2. Context Menu Ergonomics Overhaul:
   - Added "Paste & Match Style" (pasteAndMatchStyle, Cmd+Alt+Shift+V / Ctrl+Shift+V) to strip foreign formatting and match current indentation level.
   - Added "Search on Google" (searchOnGoogle, Cmd+Alt+G / Ctrl+Alt+G) to instantly query selected text or word under cursor in system browser.
   - Streamlined case conversion to direct UPPERCASE and lowercase actions, eliminating cluttered run/bash options.

3. General Preferences Additions:
   - Added option to Enable / Disable Character Encoding Auto-detection (enableEncodingAutoDetection) in Preferences > General.
   - Added option to Enable / Disable Save All Confirmation dialog (enableSaveAllConfirmation) in Preferences > General.

4. Syntax Highlighting & Theme Engine Mathematical Overhaul (WCAG 2.1):
   - Integrated WCAG 2.1 relative luminance and contrast calculations (0.2126*R + 0.7152*G + 0.0722*B), guaranteeing minimum 4.0:1 contrast ratio on primary text across all 70+ themes.
   - Semantic styling tailored to language families (C/C++, Python, Web/HTML/JS/CSS, Markdown progressive H1-H6, SQL, Shell, YAML, etc.).
   - High-visibility Gold Badges (#FFD700) for critical comment markers (TODO, FIXME, BUG, HACK, NOTE).
   - Warm golden amber for string escape sequences (\\n, \\t, \\xHH).
   - Theme-synchronized rounded border box indicator for SmartHighlighter word occurrences.

5. Standard Lightweight macOS DMG Packaging (30MB–40MB Standard):
   - Calibrated release DMG packaging strictly between 30MB and 40MB (38MB–39MB) preserving universal Qt libraries and standard image format plugins (libqjpeg, libqgif, libqwebp).
   - Packaged via canonical native hdiutil with custom retina background, layout preset .DS_Store (680x440), and verified CRC32 checksum.

6. 14-Pillar Code Quality & Static Analysis Gates:
   - 100% clean passes on cppcheck (171 files, 0 errors, 0 warnings), clangd, clang-tidy, and trunk check.
   - SonarLint compliance: resolved parameter warnings and enforced const correctness.
   - 100% automated test suite pass rate (ctest).
   - 100% bilingual translation coverage (English and Bahasa Indonesia).

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

### 🌟 Rilis: 41 Paket Ikon Toolbar Baru (Total 69 Koleksi Bawaan), Ergonomi Menu Konteks, Opsi Preferensi Umum, Perombakan Syntax Highlighting WCAG 2.1 & Standar DMG Rilis 30MB–40MB

SparkNotePad v1.4.6 menghadirkan ekspansi kustomisasi visual bilah alat, peningkatan kenyamanan editor, dan standar pengemasan berkualitas tinggi dari versi 1.4.5:

#### 🆕 Ditambahkan (Added)

- **41 Paket Ikon Toolbar Baru (Total 69 Koleksi Bawaan Multi-Resolusi)**:
  - Melipatgandakan koleksi ikon dari 28 paket di v1.4.5 menjadi 69 paket bawaan lengkap (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
    - 11 Paket Ikon Tematik: `royal_blue`, `notepad_pro`, `pastel`, `fluent_modern`, `macos_bigsur_modern`, `dark_neon_pro`, `macos_glass`, `modern_flat_colorful`, `glossy_3d_skeuomorphic`, `retro_8bit_pixel`, `sketchy_doodle`.
    - 30 Paket Ikon Modern: `01-macos-big-sur`, `02-windows-11-fluent`, `03-arc-browser-glow`, `04-nordic-frost-minimal`, `05-dracula-cyber-neon`, `06-catppuccin-macchiato`, `07-tokyo-night-storm`, `08-synthwave-80s`, `09-monokai-pro-vivid`, `10-solarized-accent`, `11-retro-commodore-64`, `12-retro-win95-classic`, `13-matrix-phosphor-green`, `14-gameboy-nostalgia`, `15-pixel-dungeon-rpg`, `16-claymorphism-3d`, `17-skeuomorphic-leather-brass`, `18-neu-brutalism-bold`, `19-isometric-voxel-3d`, `20-origami-paper-fold`, `21-cyberpunk-2077-yellow`, `22-lofi-aesthetic-grain`, `23-vaporwave-pastel-sunset`, `24-astronomy-deep-space`, `25-botanical-nature-green`, `26-blueprint-schematic`, `27-chalkboard-handdrawn`, `28-gouache-art-paint`, `29-stained-glass-mosaic`, `30-swiss-minimal-3d`.
  - Mengusung standar Notepad++ Classic: tanpa bayangan lantai, tanpa halo abu-abu, dan tanpa garis timbul kasar, dengan margin 18% agar tidak terpotong di layar Retina.
  - Invarian Toolbar Gelap Permanen (`#21252b`) di seluruh mode dan tema.
- **Fitur Baru Menu Konteks Editor**:
  - Menu **Tempel & Cocokkan Format (Paste & Match Style)** (`Cmd+Alt+Shift+V` / `Ctrl+Shift+V`): menempel teks tanpa format bawaan dan menyesuaikan indentasi editor.
  - Menu **Cari di Google (Search on Google)** (`Cmd+Alt+G` / `Ctrl+Alt+G`): membuka browser untuk mencari teks terpilih atau kata di kursor secara langsung.
  - Menu konversi huruf langsung: **HURUF BESAR (UPPERCASE)** dan **huruf kecil (lowercase)** tanpa submenu bertingkat.
- **Opsi Baru Preferensi Umum (General Preferences)**:
  - Opsi sakelar Deteksi Otomatis Enkoding Karakter (`enableEncodingAutoDetection`).
  - Opsi sakelar Dialog Konfirmasi Simpan Semua (`enableSaveAllConfirmation`).
- **Perombakan Pewarnaan Sintaksis WCAG 2.1**:
  - Garansi matematis kontras rasio minimal 4.0:1 pada seluruh 70+ tema editor.
  - Lencana Emas (#FFD700) untuk penanda penting TODO, FIXME, BUG, HACK, NOTE.
  - Warna kuning amber hangat untuk urutan karakter escape string (\\n, \\t, \\xHH).
  - Kotak indikator penanda kemunculan kata kunci tersinkronisasi dinamis dengan tema aktif.

#### 🔧 Diubah & Ditingkatkan (Changed & Improved)

- **Standar Pengemasan macOS DMG 30MB–40MB**:
  - Kalibrasi ukuran DMG rilis ke 38MB–39MB dengan mempertahankan pustaka universal Qt dan plugin format gambar standar (libqjpeg, libqgif, libqwebp).
  - Menggunakan format native hdiutil dengan preset tata letak .DS_Store (680x440), background retina, dan pengujian integritas checksum CRC32.
- **Pembersihan Menu Konteks**:
  - Menghapus opsi Run dan Bash encode/decode dari menu klik kanan editor agar lebih ringkas dan fokus.
- **Audit Kualitas Kode 14 Pilar & Audit Linter**:
  - 100% lulus bersih pada cppcheck (171 berkas, 0 kesalahan), clangd, clang-tidy, dan ctest.
  - Penyesuaian aturan SonarLint pada parameter lambda dan pointer-to-const.

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

### 🌟 Release: 41 New Modern Toolbar Icon Packs (69 Total Built-in Suites), Context Menu Ergonomics, General Preferences Options, WCAG 2.1 Syntax Highlighting Overhaul & Standard 30MB–40MB DMG Packaging

SparkNotePad v1.4.6 delivers major enhancements across visual customization, editor context menu ergonomics, general preferences options, WCAG 2.1 contrast mathematical safety for syntax highlighting, and canonical 30MB–40MB macOS DMG packaging over version 1.4.5:

#### 🆕 Added

- **41 New Toolbar Icon Packs (69 Total Built-in Multi-Resolution Suites)**:
  - Multiplied the icon library from 28 packs in v1.4.5 to 69 complete built-in suites (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
    - 11 Thematic Suites: `royal_blue`, `notepad_pro`, `pastel`, `fluent_modern`, `macos_bigsur_modern`, `dark_neon_pro`, `macos_glass`, `modern_flat_colorful`, `glossy_3d_skeuomorphic`, `retro_8bit_pixel`, `sketchy_doodle`.
    - 30 Modern Suites: `01-macos-big-sur`, `02-windows-11-fluent`, `03-arc-browser-glow`, `04-nordic-frost-minimal`, `05-dracula-cyber-neon`, `06-catppuccin-macchiato`, `07-tokyo-night-storm`, `08-synthwave-80s`, `09-monokai-pro-vivid`, `10-solarized-accent`, `11-retro-commodore-64`, `12-retro-win95-classic`, `13-matrix-phosphor-green`, `14-gameboy-nostalgia`, `15-pixel-dungeon-rpg`, `16-claymorphism-3d`, `17-skeuomorphic-leather-brass`, `18-neu-brutalism-bold`, `19-isometric-voxel-3d`, `20-origami-paper-fold`, `21-cyberpunk-2077-yellow`, `22-lofi-aesthetic-grain`, `23-vaporwave-pastel-sunset`, `24-astronomy-deep-space`, `25-botanical-nature-green`, `26-blueprint-schematic`, `27-chalkboard-handdrawn`, `28-gouache-art-paint`, `29-stained-glass-mosaic`, `30-swiss-minimal-3d`.
  - Pristine Notepad++ Classic clean standard: zero floor drop shadows, halos, gray background tiles, or embossed borders, with 18% breathing margins preventing clipping on Retina displays.
  - Permanent Dark Toolbar Invariant (`#21252b`) locked across all themes and appearance modes to preserve icon contrast.
- **Editor Context Menu Ergonomics**:
  - Added **Paste & Match Style** (`pasteAndMatchStyle`, `Cmd+Alt+Shift+V` / `Ctrl+Shift+V`) to strip external formatting and match current editor indentation.
  - Added **Search on Google** (`searchOnGoogle`, `Cmd+Alt+G` / `Ctrl+Alt+G`) to query selected text or word under cursor in default system browser.
  - Streamlined case conversion to direct **UPPERCASE** and **lowercase** actions without multi-tier submenus.
- **General Preferences Options**:
  - Added option to Enable / Disable Character Encoding Auto-detection (`enableEncodingAutoDetection`).
  - Added option to Enable / Disable Save All Confirmation dialog (`enableSaveAllConfirmation`).
- **WCAG 2.1 Contrast Safety & Syntax Highlighting Overhaul**:
  - Integrated WCAG 2.1 relative luminance and contrast calculations (0.2126*R + 0.7152*G + 0.0722*B) guaranteeing minimum 4.0:1 contrast ratio across all 70+ themes.
  - High-visibility Gold Badges (`#FFD700`, bold) for TODO, FIXME, BUG, HACK, NOTE.
  - Warm golden amber for string escape sequences (`\\n`, `\\t`, `\\xHH`).
  - Theme-synchronized rounded border box indicator for SmartHighlighter word occurrences.

#### 🔧 Changed & Improved

- **Canonical 30MB–40MB macOS DMG Packaging**:
  - Calibrated release DMG packaging to 38MB–39MB while retaining universal Qt libraries and standard image format plugins (`libqjpeg`, `libqgif`, `libqwebp`).
  - Packaged via canonical native `hdiutil` with custom retina background, layout preset `.DS_Store` (680x440), and verified CRC32 checksum.
- **Context Menu Noise Reduction**:
  - Removed Run and Bash encode/decode actions from right-click context menu for cleaner ergonomics.
- **14-Pillar Code Quality & Static Analysis Gates**:
  - 100% clean passes on `cppcheck` (171 files, 0 errors, 0 warnings), `clangd`, `clang-tidy`, and `ctest`.
  - SonarLint compliance: resolved parameter warnings and enforced const correctness.

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
