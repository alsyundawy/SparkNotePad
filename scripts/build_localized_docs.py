#!/usr/bin/env python3
"""
Generate localized DOCNOTE and CHANGELOG files for SparkNotePad v1.3.9.
Creates:
  - src/doc/DOCNOTE_id.txt (Bahasa Indonesia)
  - src/doc/DOCNOTE_en.txt (English)
  - src/doc/CHANGELOG_id.txt (Bahasa Indonesia)
  - src/doc/CHANGELOG_en.txt (English)
Synchronizes:
  - doc/ and root directory copies.
"""

DOCNOTE_ID = """# Catatan Rilis SparkNotePad — v1.3.9 (Bahasa Indonesia)

--------------------------------------------------------------------------------
================================================================================
Versi Rilis          : 1.3.9 (Scintilla 5.6.9, Lexilla 5.5.5, Default Language & 14-Pillar Hardening)
Tanggal Rilis        : 2026-11-07
Pengelola Utama      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organisasi           : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repositori           : https://github.com/alsyundawy/SparkNotePad
Mesin Penyunting     : Scintilla 5.6.9 & Lexilla 5.5.5 (125 Lexer Resmi)
Runtime Skrip        : Sandboxed Lua 5.5.1 & Python 3.13+ Bridge
Kerangka Kerja GUI   : Qt 6.5 / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platform      : macOS 11.0–26.0 (Apple Silicon & Intel), Linux, FreeBSD 13–15
Lisensi              : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Ringkasan Eksekutif (Bahasa Indonesia): Rilis v1.3.9

SparkNotePad v1.3.9 menghadirkan pembaruan inti editor Scintilla dan Lexilla langsung dari rilis resmi upstream, penanganan keamanan algoritma pencarian regex, pewarnaan sintaksis metadata front-matter Markdown, pengaturan bahasa default dokumen baru, estetika paket instalasi DMG macOS, serta lolos audit kualitas 14 pilar tanpa peringatan:

1. Integrasi Penuh Upstream Scintilla 5.6.9 & Lexilla 5.5.5:
   - Pembaruan menyeluruh source code Scintilla ke 5.6.9 dan Lexilla ke 5.5.5 resmi dari scintilla.org di thirdparty/ (160 berkas diperbarui).
   - Regenerasi wrapper Qt (ScintillaEdit.cpp dan ScintillaEdit.h) menggunakan script resmi upstream WidgetGen.py.
   - Pemulihan rendering warna penuh untuk format gambar XPM.
   - Peningkatan performa query posisi awal baris dengan melewati kalkulasi layout yang tidak diperlukan saat scrolling cepat.
   - Peningkatan kapasitas cache lexeme dari 30 menjadi 40 bita untuk identifier panjang, kolom SQL, dan simbol.
   - Pemutakhiran peta kategori karakter dan konversi huruf besar/kecil ke standar Unicode 17.
   - Perbaikan crash pada undo histori perubahan akibat optimasi bulk deletion.
   - Penguatan keamanan memori Lexilla: penambalan buffer out-of-bounds write (CWE-787) dan read (CWE-125) pada lexer DMIS, Perl, Raku, Ruby, TCMD, TeX, Visual Prolog, dan X12.

2. Integrasi Runtime Lua 5.5.1 Terisolasi (Sandboxed):
   - Upgrade runtime embedded Lua ke versi resmi Lua 5.5.1 di thirdparty/lua.
   - Penerapan sandboxing ketat: penonaktifan API sistem berisiko (os.execute, io.*, debug.*, package.loadlib) guna mencegah eksekusi kode liar melalui ekstensi atau tema pihak ketiga.

3. Pewarnaan Sintaksis Front-Matter Markdown:
   - Dukungan penuh blok metadata front-matter (YAML, TOML, JSON) di src/languages/markdown.lua memanfaatkan ID gaya Lexilla 5.5.5 terbaru:
     - SCE_MARKDOWN_FRONT_MARK (ID 22): Pembatas metadata (---, +++, {}).
     - SCE_MARKDOWN_FRONT (ID 23): Isi dan nilai properti metadata.
     - SCE_MARKDOWN_FRONT_KEY (ID 24): Kunci dan atribut properti metadata.

4. Port Fitur Pilihan & Perbaikan Bug dari NotepadNext:
   - Perbaikan Infinite Loop pada Replace All (CWE-835 / DoS): Di src/Finder.cpp, perbaikan pembekuan aplikasi saat mencocokkan ekspresi reguler panjang-nol (^, $, \\b, lookarounds) dengan penegakan cursor maju via editor->positionAfter().
   - Invarian Dialog Cari & Ganti: Di src/dialogs/FindReplaceDialog.cpp, opsi arah pencarian mundur otomatis dimatikan saat mode Regular Expression dipilih.
   - Pengaturan Bahasa Default Dokumen Baru: Penambahan konfigurasi DefaultLanguage pada Preferensi > Editor untuk dokumen baru tanpa judul, dengan daftar bahasa lengkap.

5. Perampingan Framework & Peningkatan Estetika DMG macOS:
   - Pembersihan framework Qt yang tidak digunakan (QtPdf, QtQml, QtQuick, QtVirtualKeyboard) pada paket DMG untuk mengurangi ukuran binary.
   - Penyempurnaan pipeline packaging DMG dengan background kustom, indikator drag-and-drop, jendela 680x440, serta perlindungan metadata tersembunyi.

6. Verifikasi Kualitas Kode 14 Pilar & Audit Tanpa Cacat:
   - Nol peringatan pada clangd --check, cppcheck, clang-tidy, dan SonarLint.
   - Penegakan kompilasi Unix Makefiles standar dengan konkurensi terkontrol -j2.

---

## 2. Ringkasan Rilis Sebelumnya: Rilis v1.3.8

SparkNotePad v1.3.8 menghadirkan personalisasi toolbar lengkap dengan 21 paket ikon terkurasi, tab Preferensi khusus ("Themes & Toolbar" & "Custom Toolbar Config"), penataan ulang menu standar desktop, sinkronisasi menyeluruh ke menu dan konteks klik kanan, tombol Tambah Tab (+), pratinjau HTML dan Markdown langsung di tab editor baru, penjelajah berkas modern bergaya File Commander, tampilan Hex/Biner reversibel, serta 100% kelengkapan 125 Lexer Lexilla.

---

## 3. Rilis Sebelumnya: Rilis v1.3.7

1. Menu Tools Notepad++ & 16-Action MIME Tools Suite (Base64, Quoted-Printable, URL encoding).
2. Hash & Authenticity Validator mandiri (MD5, SHA-1, SHA-256, SHA-512, SHA3).
3. Pemetaan dan Kustomisasi Pintasan Keyboard Interaktif (Preferences > Shortcuts).
4. Eliminasi Batasan API Rate Limit GitHub via HTTP ETag / 304 Caching.
"""

DOCNOTE_EN = """# SparkNotePad Release DocNote — v1.3.9 (English)

--------------------------------------------------------------------------------
================================================================================
Release Version      : 1.3.9 (Scintilla 5.6.9, Lexilla 5.5.5, Default Language & 14-Pillar Hardening)
Release Date         : 2026-11-07
Lead Maintainer      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organization         : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repository           : https://github.com/alsyundawy/SparkNotePad
Core Editing Engine  : Scintilla 5.6.9 & Lexilla 5.5.5 (Complete 125 Official Lexers)
Embedded Lua Core    : Sandboxed Lua 5.5.1 & Python 3.13+ Bridge
GUI Framework        : Qt 6.5 / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platforms     : macOS 11.0–26.0 (Apple Silicon & Intel), Linux, FreeBSD 13–15
License              : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Executive Summary (English): Release v1.3.9

SparkNotePad v1.3.9 is a major foundational engine upgrade and stability release, featuring upstream core editing engine upgrades to Scintilla 5.6.9 and Lexilla 5.5.5, search/replace algorithmic security fixes, Markdown front-matter syntax styling, user-configurable default language preference, macOS DMG aesthetics enhancement, and zero-defect 14-pillar verification:

1. Official Upstream Scintilla 5.6.9 & Lexilla 5.5.5 Core Integration:
   - Full upstream replacement of Scintilla 5.6.9 and Lexilla 5.5.5 directly from scintilla.org into thirdparty/ (160 files updated).
   - Regenerated Qt wrapper interface (ScintillaEdit.cpp and ScintillaEdit.h) using Scintilla's official WidgetGen.py script.
   - Restored full-color rendering for XPM images.
   - Accelerated line-start position queries by skipping redundant layout calculations during rapid scrolling.
   - Increased cached lexeme capacity from 30 to 40 bytes for large identifiers, SQL columns, and symbols.
   - Updated character category maps and case conversions to standard Unicode 17.
   - Fixed crash in change history undo caused by bulk deletion optimization.
   - Lexilla memory safety hardening: fixed out-of-bounds writes (CWE-787) and reads (CWE-125) in DMIS, Perl, Raku, Ruby, TCMD, TeX, Visual Prolog, and X12 lexers.

2. Sandboxed Lua 5.5.1 Engine Integration:
   - Fully upgraded the embedded Lua runtime to official Lua 5.5.1 in thirdparty/lua.
   - Strict sandboxing enforced: disabled unsafe system APIs (os.execute, io.*, debug.*, package.loadlib) to prevent arbitrary code execution via untrusted extensions or theme scripts.

3. Markdown Front-Matter Syntax Highlighting:
   - Full support for front-matter metadata blocks (YAML, TOML, JSON) in src/languages/markdown.lua utilizing new Lexilla 5.5.5 styling IDs:
     - SCE_MARKDOWN_FRONT_MARK (ID 22): Visual styling for metadata delimiters (---, +++, {}).
     - SCE_MARKDOWN_FRONT (ID 23): Visual styling for front-matter payload and property values.
     - SCE_MARKDOWN_FRONT_KEY (ID 24): Highlighting for metadata property keys and attributes.

4. Selective Feature & Stability Ports from NotepadNext:
   - Fix Infinite Loop in Replace All (CWE-835 / DoS): In src/Finder.cpp, fixed application freeze when matching zero-length regular expressions (^, $, \\b, lookarounds) by enforcing forward cursor progression using editor->positionAfter().
   - Search & Replace Dialog Direction Invariant: In src/dialogs/FindReplaceDialog.cpp, automatically unchecks and disables backwards direction searching when Regular Expression search mode is selected.
   - Default Language Setting for New Documents: Added user-configurable DefaultLanguage in Preferences > Editor for untitled/new documents, with dynamic language selection.

5. Framework Pruning & macOS DMG Aesthetics Enhancement:
   - Pruned unused Qt frameworks (QtPdf, QtQml, QtQuick, QtVirtualKeyboard) from release DMGs to eliminate bloatware.
   - Enhanced DMG packaging pipeline with custom background, drag-and-drop indicator, 680x440 window layout, and hidden metadata preservation.

6. 14-Pillar Code Quality & Zero-Defect Linter Verification:
   - Zero warnings across clangd --check, cppcheck, clang-tidy, and SonarLint.
   - Strict Unix Makefiles compilation enforcement with controlled -j2 concurrency.

---

## 2. Prior Release Summary: Release v1.3.8

SparkNotePad v1.3.8 delivers a comprehensive overhaul of toolbar customization, 21 authentic high-definition icon packs, dedicated Preferences tabs ("Themes & Toolbar" & "Custom Toolbar Config"), desktop standard menu reorganization, dynamic whole-window and context menu icon synchronization, Plus (+) tab button, in-tab HTML and Markdown preview, File Commander modern explorer, reversible Hex/Binary viewing, and 100% Lexilla Lexers parity.

---

## 3. Prior Release: Release v1.3.7

1. Notepad++ Style Tools Menu & 16-Action MIME Tools Suite.
2. Standalone Hash & Authenticity Validator (MD5, SHA-1, SHA-256, SHA-512, SHA3).
3. Interactive Shortcut Mapper in Preferences > Shortcuts.
4. GitHub Rate Limit Elimination via ETag Caching.
"""

CHANGELOG_ID = """# Catatan Perubahan (Changelog) — SparkNotePad

Semua perubahan penting pada **SparkNotePad** didokumentasikan dalam berkas ini.

Format berkas ini mengacu pada [Keep a Changelog](https://keepachangelog.com/id/1.1.0/),
dan proyek ini mengikuti [Semantic Versioning](https://semver.org/lang/id/).

---

## [1.3.9] - 2026-11-07

### ⚡ Pembaruan Mesin Scintilla 5.6.9 & Lexilla 5.5.5, Pengaturan Bahasa Default, Perbaikan Infinite-Loop Regex & Pengerasan 14 Pilar

SparkNotePad v1.3.9 menghadirkan pembaruan mesin editor ke Scintilla 5.6.9 dan Lexilla 5.5.5, menyelesaikan kerentanan algoritmik ekspresi reguler, memperkenalkan pengaturan bahasa default untuk dokumen baru, memperluas pewarnaan sintaksis front-matter Markdown, dan lolos audit kode statis 14 pilar:

#### 🆕 Ditambahkan

- **Integrasi Penuh Scintilla 5.6.9 & Lexilla 5.5.5 Upstream Resmi**:
  - Pembaruan Scintilla dari 5.6.7 ke 5.6.9 resmi scintilla.org.
  - Pembaruan Lexilla dari 5.5.4 ke 5.5.5 resmi scintilla.org.
  - Regenerasi Qt wrapper (ScintillaEdit.cpp dan ScintillaEdit.h) via WidgetGen.py resmi.
  - Peta kategori karakter dan konversi teks standar Unicode 17 lengkap.
- **Pewarnaan Sintaksis Front-Matter Markdown**:
  - Tiga ID gaya Lexilla 5.5.5 di src/languages/markdown.lua:
    - SCE_MARKDOWN_FRONT_MARK (ID 22): Pembatas metadata YAML (---), TOML (+++), JSON ({}).
    - SCE_MARKDOWN_FRONT (ID 23): Konten dan nilai properti front-matter.
    - SCE_MARKDOWN_FRONT_KEY (ID 24): Kunci dan atribut properti front-matter.
- **Pengaturan Bahasa Default Dokumen Baru**:
  - Kunci konfigurasi DefaultLanguage pada ApplicationSettings (default: "Text").
  - Dropdown "Default Language" di Preferences > Editor (PreferencesDialog).
  - Otomatis menetapkan bahasa pilihan pengguna saat membuat dokumen baru tanpa judul.
- **Peningkatan Kemampuan Lexer**:
  - HTML & JavaScript: Dukungan $, #, identifier non-ASCII, dan template literal bersarang.
  - PHP: Dukungan variabel variabel ($$var) dengan gaya SCE_HPHP_VARIABLE.
  - SQL: Resolusi kutipan komplemen Oracle (GetQuoteComplement).

#### 🛡️ Keamanan & Perbaikan Bug

- **Perbaikan Infinite-Loop Regex Replace-All (CWE-835 / DoS)**:
  - Memperbaiki pembekuan event loop pada Finder::replaceAll (src/Finder.cpp) saat mencocokkan ekspresi reguler panjang-nol (^, $, \\b, lookarounds).
  - Penegakan pergerakan cursor maju menggunakan editor->positionAfter().
- **Perbaikan Invarian Dialog Cari & Ganti**:
  - Otomatis mematikan opsi pencarian arah mundur saat mode Regular Expression dipilih di FindReplaceDialog.cpp.
- **Pengerasan Keamanan Memori Lexilla**:
  - Mitigasi kerentanan out-of-bounds write (CWE-787) pada lexer DMIS, Perl here-doc, Raku, TCMD, dan TeX.
  - Mitigasi kerentanan out-of-bounds read (CWE-125) pada lexer Ruby dan X12.
  - Perbaikan crash makro preprocessor pada lexer C++ dan deep recursion pada Visual Prolog.
- **Stabilitas & Performa Scintilla**:
  - Pemulihan rendering warna penuh untuk gambar XPM.
  - Percepatan query posisi awal baris saat scrolling cepat.
  - Peningkatan kapasitas buffer lexeme dari 30 ke 40 bita.
  - Perbaikan crash undo riwayat perubahan pada bulk deletion.

#### 🔧 Kualitas Kode & Verifikasi 14 Pilar

- Nol peringatan pada clangd --check, cppcheck, clang-tidy, dan SonarLint.
- Penegakan kompilasi Unix Makefiles standar dengan kontrol batas beban -j2.

---

## [1.3.8] - 2026-11-04

### 🎨 Rangkaian 21 Paket Ikon Toolbar, Tab Preferensi Themes & Toolbar, Ergonomi Menu Desktop & Paritas Penuh i18n

SparkNotePad v1.3.8 memperkenalkan arsitektur personalisasi toolbar lengkap dengan 21 paket ikon terkurasi, tab Preferensi khusus ("Themes & Toolbar" & "Custom Toolbar Config"), penataan ulang menu standar desktop, sinkronisasi menyeluruh ke menu dan konteks klik kanan, tombol Tambah Tab (+), pratinjau HTML dan Markdown langsung di tab editor baru, penjelajah berkas modern bergaya File Commander, tampilan Hex/Biner reversibel, serta 100% kelengkapan 125 Lexer Lexilla:

#### 🆕 Ditambahkan

- **Rangkaian 21 Paket Ikon Toolbar Multi-Resolusi**:
  - Modern Ribbon (Default Vector SVG), Microsoft Fluent UI (Win 11), Windows 11 (Fluent 2), Windows 7 (Aero Glass), Windows XP (Luna Classic), Fugue Classic (Silk Suite), Notepad++ Klasik, NotepadNext Klasik, Monochrome Outline (Minimalist), Vibrant DuoTone (Cyberpunk), Material Design Sharp, Pixel Design, Monoline Minimal, Notepad++ Green, Notepad3 Warm, Humanity GNOME, Elementary Flat, Breeze KDE, Papirus Rounded, macOS Cupertino, dan VS Code Codicons.
  - Skala ikon dinamis multi-DPI: 16x16, 20x20, 24x24 (Default), 32x32, 48x48, 64x64.
  - Varian pixmap Retina @2x tajam dan jernih tanpa distorsi.
  - Tone mapping luminans cerdas HSL pada tema gelap mencegah ikon memudar pada latar belakang gelap.
- **Tab Khusus Preferensi ("Themes & Toolbar" & "Custom Toolbar Config")**:
  - Tab 3 ("Themes & Toolbar"): Relokasi pengaturan gaya visual Tema bersama pengatur Paket Ikon Toolbar lengkap.
  - Tab 4 ("Custom Toolbar Config"): Penyesuai visual bilah alat (CustomizeToolbarWidget) terintegrasi langsung di Tab 4.
- **Penataan Ulang Menu Aplikasi Standar Desktop**:
  - Relokasi Tampilan Hex / Biner tepat di bawah Bandingkan (Compare) pada menu tingkat atas (Ctrl+Alt+H / Cmd+Alt+H).
  - Menempatkan Mulai Ulang Aplikasi di menu Aplikasi macOS tepat di atas Keluar.
  - Menghapus aksi manual Emoji & Simbol demi mendukung palet emoji bawaan macOS (Ctrl+Cmd+Space).
  - Merampingkan menu Tampilan (View) dengan menghapus submenu Panel Samping redundan.
- **Paritas Penuh 100% Katalog Dwibahasa (i18n)**:
  - Sinkronisasi menyeluruh katalog Bahasa Inggris (SparkNotePad_en.ts) dan Bahasa Indonesia (SparkNotePad_id.ts) pada 1.924 entri dengan 0 entri belum selesai.
- **Tombol Plus (+) pada Setiap Samping Akhir Tab**:
  - Tombol interaktif Plus (+) diletakkan pada title bar di samping kanan tab editor untuk membuat berkas baru dengan cepat.
- **Sinkronisasi Ikon Menubar & Menu Konteks Dinamis (applyToWindow)**:
  - Perubahan paket ikon toolbar secara otomatis menyinkronkan seluruh ikon pada toolbar, menubar utama, menu konteks klik kanan tab, dan menu klik kanan Scintilla editor.
- **Pratinjau HTML & Markdown di Tab Editor Baru (PreviewTabWidget)**:
  - Pratinjau langsung di tab editor tengah (ads::CenterDockWidgetArea).
  - Aksi "Preview in Default Browser" khusus berkas .html.
- **Folder & File Browser Bergaya File Commander Modern (FileBrowserDock)**:
  - Bilah pintasan cepat (Home, Documents, Downloads, Desktop, Workspace).
  - Ikon navigasi SVG tajam (Up, Folder, Refresh).
  - Bilah status metrik menampilkan jumlah berkas, folder, ruang disk kosong, dan detail seleksi.
- **Tampilan Hex / Binary Reversibel (Ctrl+Alt+H)**:
  - Menampilkan offset 8-digit, 16 bita hex (8+8 grouping), dan kolom karakter ASCII.
- **100% Pengadopsian Lexilla Lexer (125 Bahasa)**:
  - Seluruh 125 lexer Lexilla didukung lengkap dengan deskriptor Lua di src/languages/.
- **Audit Pintasan Keyboard & Resolusi Konflik**:
  - Resolusi benturan shortcut keyboard pada enkripsi, skrip python, transparansi, file browser, dan tampilan hex.
"""

CHANGELOG_EN = """# Changelog — SparkNotePad

All notable changes to **SparkNotePad** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.3.9] - 2026-11-07

### ⚡ Scintilla 5.6.9 & Lexilla 5.5.5 Engine Upgrade, Default Language Setting, Regex Infinite-Loop Fix & 14-Pillar Hardening

SparkNotePad v1.3.9 delivers upstream core engine upgrades to Scintilla 5.6.9 and Lexilla 5.5.5, resolves critical algorithmic vulnerabilities in regular expression processing, introduces user-configurable default language preferences for new documents, expands Markdown front-matter syntax highlighting, and passes comprehensive 14-pillar static analysis and code review:

#### 🆕 Added

- **Official Scintilla 5.6.9 & Lexilla 5.5.5 Core Integration**:
  - Upgraded embedded Scintilla engine from 5.6.7 to 5.6.9 (official upstream release from scintilla.org).
  - Upgraded embedded Lexilla lexer library from 5.5.4 to 5.5.5 (official upstream release from scintilla.org).
  - Regenerated Qt wrapper interface (ScintillaEdit.cpp and ScintillaEdit.h) via upstream WidgetGen.py.
  - Full Unicode 17 character category mapping and case conversions.
- **Markdown Front-Matter Syntax Highlighting**:
  - Implemented 3 new Lexilla 5.5.5 style definitions in src/languages/markdown.lua:
    - SCE_MARKDOWN_FRONT_MARK (ID 22): Boundary delimiters for YAML (---), TOML (+++), and JSON ({}) front matter.
    - SCE_MARKDOWN_FRONT (ID 23): Front matter body and property values.
    - SCE_MARKDOWN_FRONT_KEY (ID 24): Front matter property keys and attributes.
- **Default Language Setting for New Documents**:
  - Added DefaultLanguage configuration key to ApplicationSettings (defaulting to "Text").
  - Added "Default Language" dropdown to Preferences > Editor (PreferencesDialog), populated with all supported languages.
  - Automatically applies the user's preferred language when creating new untitled documents.
- **Enhanced Lexer Capabilities**:
  - HTML & JavaScript: Support for $, #, non-ASCII identifiers, and nested template literals.
  - PHP: Support for variable variables ($$var) styling as SCE_HPHP_VARIABLE.
  - SQL: Enhanced Oracle complement quote resolution (GetQuoteComplement).

#### 🛡️ Security & Bug Fixes

- **Infinite-Loop Regex Replace-All Fix (CWE-835 / DoS)**:
  - Fixed application hang and event loop freeze in Finder::replaceAll (src/Finder.cpp) when matching zero-length regular expressions (^, $, \\b, lookarounds).
  - Enforced forward cursor progression using editor->positionAfter().
- **Search & Replace Dialog State Invariant Fix**:
  - Automatically unchecks and disables backwards direction searching when Regular Expression search mode is selected in FindReplaceDialog.cpp.
- **Lexilla Memory Safety Hardening**:
  - Mitigated out-of-bounds write vulnerabilities (CWE-787) in DMIS, Perl here-doc delimiters, Raku, TCMD, and TeX lexers.
  - Mitigated out-of-bounds read vulnerabilities (CWE-125) in Ruby and X12 lexers.
  - Fixed preprocessor macro invocation crash in C++ lexer and deep nesting recursion crash in Visual Prolog lexer.
- **Scintilla Performance & Stability**:
  - Restored full-color rendering for XPM images.
  - Accelerated line-start position queries by skipping unnecessary line layout calculations.
  - Expanded cached lexeme buffer from 30 to 40 bytes for large identifiers.
  - Fixed crash with change history undo caused by bulk deletion optimization.

#### 🔧 Code Quality & 14-Pillar Verification

- Zero warnings across clangd --check, cppcheck, clang-tidy, and SonarLint.
- Strict Unix Makefiles compilation enforcement with controlled -j2 concurrency.

---

## [1.3.8] - 2026-11-04

### 🎨 21 Toolbar Icon Packs Suite, Preferences Themes & Toolbar Tabs, Desktop Menu Ergonomics & 100% i18n Parity

SparkNotePad v1.3.8 features an extensive toolbar personalization architecture with 21 curated icon packs, dedicated Preferences tabs ("Themes & Toolbar" & "Custom Toolbar Config"), desktop standard menu reorganization, native vector SVG scaling, multi-resolution @2x Retina pixmaps, intelligent dark-mode luminance tone mapping, Plus (+) tab button, in-tab HTML and Markdown preview, modernized File Commander browser, reversible Hex/Binary viewing, and 100% Lexilla Lexers parity:

#### 🆕 Added

- **21 Distinct Multi-Resolution Toolbar Icon Packs Suite**:
  - Seamlessly switch among 21 built-in vector and pixel icon collections.
  - Dynamic multi-DPI icon scaling: 16x16, 20x20, 24x24 (Default), 32x32, 48x48, 64x64.
  - Crisp Retina @2x pixmaps without blur or distortion.
  - Intelligent Dark Mode HSL luminance tone mapping preventing icon washing-out on dark canvases.
- **Dedicated Preferences Tabs ("Themes & Toolbar" & "Custom Toolbar Config")**:
  - Tab 3 ("Themes & Toolbar"): Relocated Themes styling into a unified tab with Toolbar Icon Packs manager.
  - Tab 4 ("Custom Toolbar Config"): Embedded visual toolbar customizer widget (CustomizeToolbarWidget) directly inside Preferences.
- **Standard Desktop Editor Menu Hierarchy**:
  - Reversible Hex/Binary View relocated directly below Compare in the top-level Tools/View menu (Ctrl+Alt+H / Cmd+Alt+H).
  - Restart Application relocated to macOS Application menu directly above Quit (QAction::ApplicationSpecificRole).
  - Duplicate manual Character Palette removed in favor of native macOS system palette (Ctrl+Cmd+Space).
  - View menu streamlined without duplicate Side Panels submenu.
- **100% Bilingual Localization Parity**:
  - Full synchronization of English (SparkNotePad_en.ts) and Indonesian (SparkNotePad_id.ts) translation catalogs across 1,924 UI strings with 0 unfinished entries.
- **Plus (+) Button at the End of Every Tab Bar**:
  - Interactive Plus (+) button located on the title bar next to editor tabs to open a new document instantly.
- **Dynamic Menubar & Context Menu Icon Synchronization (applyToWindow)**:
  - Changing icon packs now propagates dynamically to all top-level menus in menuBar(), tab right-click context menus, and the Scintilla editor right-click context menu.
- **In-Tab & Dock HTML/Markdown Live Preview (PreviewTabWidget & MarkdownSafePreviewDock)**:
  - Center dock area (ads::CenterDockWidgetArea) in-tab preview for .html and .md files without opening detached external windows.
  - Dedicated "Preview in Default Browser" action for .html and .md documents (Ctrl+Alt+B).
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
