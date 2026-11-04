#!/usr/bin/env python3
"""
Generate localized DOCNOTE and CHANGELOG files for SparkNotePad v1.3.8.
Creates:
  - src/doc/DOCNOTE_id.txt (Bahasa Indonesia)
  - src/doc/DOCNOTE_en.txt (English)
  - src/doc/CHANGELOG_id.txt (Bahasa Indonesia)
  - src/doc/CHANGELOG_en.txt (English)
Synchronizes:
  - doc/ and root directory copies.
"""

DOCNOTE_ID = """# Catatan Rilis SparkNotePad — v1.3.8 (Bahasa Indonesia)

--------------------------------------------------------------------------------
================================================================================
Versi Rilis          : 1.3.8 (Toolbar Icon Packs Suite, In-Tab Preview & File Commander)
Tanggal Rilis        : 2026-11-04
Pengelola Utama      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organisasi           : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repositori           : https://github.com/alsyundawy/SparkNotePad
Mesin Penyunting     : Scintilla 5.6.7 & Lexilla 5.5.4 (125 Lexer Lengkap)
Runtime Skrip        : Sandboxed Lua 5.5.1 / 5.4.7 & Python 3.13+ Bridge
Kerangka Kerja GUI   : Qt 6.5 / 6.8 LTS / 6.12 (C++20 ISO Standard)
Target Platform      : macOS 11.0–26.0 (Apple Silicon & Intel), Linux, FreeBSD
Lisensi              : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Ringkasan Eksekutif (Bahasa Indonesia): Rilis v1.3.8

SparkNotePad v1.3.8 menghadirkan perombakan besar pada arsitektur personalisasi toolbar, 21 paket ikon resolusi tinggi, tab Preferensi khusus ("Themes & Toolbar" & "Custom Toolbar Config"), penataan ulang menu standar editor desktop, sinkronisasi ikon menyeluruh ke seluruh menu dan konteks klik kanan, tombol Tambah Tab (+) di samping tab, pratinjau HTML & Markdown di dalam tab baru, File Commander modern, Tampilan Hex/Biner reversibel, serta 100% kelengkapan 125 Lexer Lexilla:

1. Rangkaian 21 Paket Ikon Toolbar Bawaan (Toolbar Icon Packs Suite):
   - Mendukung 21 paket ikon terintegrasi beresolusi tinggi: Modern Ribbon (Default Vector), Microsoft Fluent UI (Win 11 Modern Vector), Windows 11 (Fluent 2 DOpus), Windows 7 (Aero Glass Azure), Windows XP (Luna Classic), Fugue Classic (Silk Suite), Notepad++ Klasik (Nostalgia), NotepadNext Klasik (Silk), Monochrome Outline (Minimalist), Vibrant DuoTone (Cyberpunk), Material Design Sharp, Pixel Design, Monoline Minimal, Notepad++ Green, Notepad3 Warm, Humanity GNOME, Elementary Flat, Breeze KDE, Papirus Rounded, macOS Cupertino, dan VS Code Codicons.
   - Dilengkapi aset visual multi-DPI (16x16, 20x20, 24x24, 32x32, 48x48, 64x64) serta aset retina @2x yang tajam dan jernih pada semua resolusi layar.
   - Tone mapping luminans cerdas HSL pada tema gelap mencegah ikon memudar pada latar belakang gelap.

2. Tab Khusus Preferensi ("Themes & Toolbar" & "Custom Toolbar Config"):
   - Tab 3 ("Themes & Toolbar"): Relokasi pengaturan gaya visual Tema (Tema Aktif, Opasitas Jendela, Interval Tail, Proteksi Layar) ke Tab 3 bersama pengatur Paket Ikon Toolbar lengkap (21 paket, kartu deskripsi dinamis, pengatur ukuran ikon, pratinjau langsung, dan pemuat paket kustom).
   - Tab 4 ("Custom Toolbar Config"): Penyesuai visual bilah alat (`CustomizeToolbarWidget`) terintegrasi langsung di Tab 4 dengan pencarian cepat, daftar aksi tersedia vs aktif, tambah/hapus aksi, pengurutan tombol, garis pemisah, dan hot-reload instan.

3. Penataan Ulang Menu Aplikasi Standar Desktop:
   - Relokasi Tampilan Hex / Biner tepat di bawah Bandingkan (Compare) pada menu tingkat atas (`Ctrl+Alt+H` / `Cmd+Alt+H`).
   - Menempatkan Mulai Ulang Aplikasi (Restart Application) di menu Aplikasi macOS tepat di atas Keluar (Quit) (`QAction::ApplicationSpecificRole`).
   - Menghapus aksi manual Emoji & Simbol demi mendukung palet emoji bawaan macOS (`Ctrl+Cmd+Space`).
   - Merampingkan menu Tampilan (View) dengan menghapus submenu Panel Samping dan aksi pratinjau markdown mandiri redundan.

4. Paritas Penuh 100% Katalog Dwibahasa (i18n):
   - Sinkronisasi menyeluruh katalog Bahasa Inggris (`SparkNotePad_en.ts`) dan Bahasa Indonesia (`SparkNotePad_id.ts`) pada 1.924 entri antarmuka pengguna dengan 0 entri belum selesai (unfinished).

5. Tombol Plus (+) pada Setiap Samping Akhir Tab:
   - Menambahkan tombol interaktif Plus (+) pada title bar di samping kanan deretan tab editor.
   - Memungkinkan pembukaan dokumen baru ("New 1", "New 2", dst.) secara instan hanya dengan sekali klik tanpa perlu pintasan keyboard.

6. Sinkronisasi Ikon Menubar & Menu Klik Kanan (Context Menu) Dinamis:
   - Penggantian paket ikon toolbar kini secara otomatis menyinkronkan seluruh ikon aksi pada Toolbar Utama, Menu Bar utama, submenu, menu klik kanan tab, dan menu klik kanan editor (Editor Context Menu).
   - Mengimplementasikan `ToolbarIconManager::applyToWindow()` dan `applyToMenu()` yang secara rekursif memperbarui seluruh `QAction` jendela aktif, serta memastikan visibilitas ikon di menu (`setIconVisibleInMenu(true)`).
   - Default ukuran icon toolbar adalah 24x24 dan icon menu adalah 16x16 (dengan aset retina 32x32).
   - Pengubahan ukuran ikon toolbar secara instan menyesuaikan ukuran geometri toolbar secara dinamis.

7. Pratinjau HTML & Markdown di Tab Editor Baru (PreviewTabWidget):
   - Menu klik kanan pada tab `.html` dan `.md` menyediakan opsi "Preview HTML in New Tab" dan "Preview Markdown in New Tab" yang membuka tampilan pratinjau di dalam tab editor tengah (`ads::CenterDockWidgetArea`), bukan di jendela baru terpisah.
   - Khusus berkas `.html`, tersedia opsi langsung "Preview in Default Browser" untuk membuka pratinjau di peramban web bawaan sistem operasi.
   - Desain pratinjau modern, elegan, dan profesional berstandar GitHub/DevDocs dengan kesetaraan tema Dark dan Light, kontrol perbesaran interaktif (zoom in/out/reset), tombol segarkan, dan isolasi keamanan dari kerentanan injeksi.

8. Folder & File Browser Bergaya File Commander Modern:
   - Antarmuka panel peramban berkas dirancang ulang dengan sentuhan modern File Commander.
   - Dilengkapi bilah pintasan cepat (Home, Documents, Downloads, Desktop, Workspace) untuk navigasi instan.
   - Menggunakan ikon SVG modern yang tajam untuk navigasi direktori.
   - Bilah status metrik di bagian bawah menampilkan jumlah berkas, folder, ruang disk kosong (`QStorageInfo`), dan detail item terpilih.
   - Penyesuaian tema dinamis yang serasi pada mode gelap maupun terang.

9. Pengadopsian Penuh 125 Lexer Lexilla (100% Lexilla Parity):
   - Mengadopsi seluruh 125 modul lexer resmi Lexilla di `src/languages/`, teregistrasi di `src/scripts.qrc` dan disinkronkan ke dalam `src/scripts/init.lua`.
   - Menghadirkan pewarnaan sintaksis lengkap untuk berbagai bahasa pemrograman modern, format konfigurasi, dan DSL teknis.

10. Resolusi Konflik Pintasan Keyboard (Shortcut Collision Fix):
    - Audit dan perbaikan menyeluruh terhadap konflik pintasan keyboard:
      - `Ctrl+Alt+Shift+E`: Enkripsi Dokumen AES-256
      - `Ctrl+Shift+R`: Jalankan Skrip Python
      - `Ctrl+Alt+Shift+T`: Alihkan Transparansi Jendela
      - `Ctrl+Alt+Shift+O`: Buka/Tutup File Browser Dock
      - `Ctrl+Alt+Shift+F`: Fokus ke Editor Utama
      - `Ctrl+Alt+H`: Beralih Tampilan Hex / Binary

11. Penegakan Format Plaintext (.txt) & Lokalisasi Otomatis Menu Help:
    - Tampilan Docnote dan Changelog di dalam AboutDialog dan menu Help disajikan otomatis sesuai bahasa aktif editor (Bahasa Indonesia saat editor berbahasa Indonesia, English saat editor berbahasa Inggris).
    - Format strictly plaintext `.txt` monospaced tanpa pemformatan markdown yang berlebih, menjamin pembacaan yang cepat, jernih, dan akurat.

---

## 2. Rilis Sebelumnya: Rilis v1.3.7

1. Menu Tools Notepad++ & 16-Action MIME Tools Suite (Base64, Quoted-Printable, URL encoding).
2. Hash & Authenticity Validator mandiri (MD5, SHA-1, SHA-256, SHA-512, SHA3).
3. Pemetaan dan Kustomisasi Pintasan Keyboard Interaktif (Preferences > Shortcuts).
4. Eliminasi Batasan API Rate Limit GitHub via HTTP ETag / 304 Caching.
"""

DOCNOTE_EN = """# SparkNotePad Release DocNote — v1.3.8 (English)

--------------------------------------------------------------------------------
================================================================================
Release Version      : 1.3.8 (Toolbar Icon Packs Suite, In-Tab Preview & File Commander)
Release Date         : 2026-11-04
Lead Maintainer      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organization         : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repository           : https://github.com/alsyundawy/SparkNotePad
Core Editing Engine  : Scintilla 5.6.7 & Lexilla 5.5.4 (Complete 125 Lexers)
Embedded Lua Core    : Sandboxed Lua 5.5.1 / 5.4.7 & Python 3.13+ Bridge
GUI Framework        : Qt 6.5 / 6.8 LTS / 6.12 (C++20 ISO Standard)
Target Platforms     : macOS 11.0–26.0 (Apple Silicon & Intel), Linux, FreeBSD
License              : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Executive Summary (English): Release v1.3.8

SparkNotePad v1.3.8 delivers a comprehensive overhaul of toolbar customization, 21 authentic high-definition icon packs, dedicated Preferences tabs ("Themes & Toolbar" & "Custom Toolbar Config"), desktop standard menu reorganization, dynamic whole-window and context menu icon synchronization, Plus (+) tab button, in-tab HTML and Markdown preview, File Commander modern explorer, reversible Hex/Binary viewing, and 100% Lexilla Lexers parity:

1. 21 Built-In Toolbar Icon Packs Suite:
   - 21 integrated high-definition icon suites: Modern Ribbon (Default Vector), Microsoft Fluent UI (Win 11 Modern Vector), Windows 11 (Fluent 2 DOpus), Windows 7 (Aero Glass Azure), Windows XP (Luna Classic), Fugue Classic (Silk Suite), Notepad++ Classic, NotepadNext Classic, Monochrome Outline (Minimalist), Vibrant DuoTone (Cyberpunk), Material Design Sharp, Pixel Design, Monoline Minimal, Notepad++ Green, Notepad3 Warm, Humanity GNOME, Elementary Flat, Breeze KDE, Papirus Rounded, macOS Cupertino, and VS Code Codicons.
   - Crisp multi-DPI assets across 16x16, 20x20, 24x24, 32x32, 48x48, 64x64, and Retina @2x scaling.
   - Intelligent Dark Mode HSL luminance tone mapping preventing icon washing-out on dark canvases.

2. Dedicated Preferences Tabs ("Themes & Toolbar" & "Custom Toolbar Config"):
   - Tab 3 ("Themes & Toolbar"): Relocated Themes styling (Active Theme, Window Opacity, Tail Interval, Display Safety) into a unified tab with Toolbar Icon Packs manager (21 packs, responsive description card layout, size selector, icon previews, custom pack folder launcher, and rescan).
   - Tab 4 ("Custom Toolbar Config"): Embedded visual toolbar customizer widget (`CustomizeToolbarWidget`) directly inside Preferences with live search, action adding/removing/reordering, separator insertion, and hot-reload.

3. Standard Desktop Editor Menu Hierarchy:
   - Reversible Hex/Binary View relocated directly below Compare in the top-level Tools/View menu (`Ctrl+Alt+H` / `Cmd+Alt+H`).
   - Restart Application relocated to macOS Application menu directly above Quit (`QAction::ApplicationSpecificRole`).
   - Duplicate manual Character Palette removed in favor of native macOS system palette (`Ctrl+Cmd+Space`).
   - View menu streamlined without duplicate Side Panels submenu.

4. 100% Bilingual Localization Parity across English & Indonesian:
   - Complete synchronization of 1,924 UI strings with 0 unfinished entries across `SparkNotePad_en.ts` and `SparkNotePad_id.ts`.

5. Plus (+) Button at the End of Every Tab Bar:
   - Added an interactive Plus (+) button on the title bar directly next to the tab row.
   - Allows instant creation of new documents ("New 1", "New 2", etc.) with a single click.

6. Dynamic Menubar & Right-Click Context Menu Icon Synchronization:
   - Changing toolbar icon packs now instantaneously propagates active iconography across the main toolbar, all top-level menubars, submenus, tab context menus, and the editor right-click context menu.
   - Implemented `ToolbarIconManager::applyToWindow()` and `applyToMenu()` recursively traversing all window `QAction` objects with graceful fallback to default icons.
   - Default toolbar icon size is 24x24 and menu icon size is 16x16 (with 32x32 retina pixmaps).
   - Real-time toolbar icon size scaling instantly adjusts toolbar geometry and layout.

7. In-Tab HTML & Markdown Preview (`PreviewTabWidget`):
   - Right-clicking `.html` or `.md` editor tabs provides an option to open the preview in a NEW TAB within the editor dock area (`ads::CenterDockWidgetArea`), rather than a separate floating window.
   - For `.html` files, a dedicated "Preview in Default Browser" action is provided to test documents in the system's default web browser.
   - Modern, professional GitHub/DevDocs styling with adaptive Dark and Light mode parity, zoom controls (zoom in/out/reset), refresh button, and sandboxing against CVE-2026-20841.

8. File Commander Modern Folder & File Browser:
   - Re-engineered file browser dock with quick bookmarks (Home, Documents, Downloads, Desktop, Workspace) for immediate navigation.
   - Crisp SVG navigation icons (Up, Folder, Refresh).
   - Bottom status bar metrics displaying file/folder counts, free storage space (`QStorageInfo`), and selection summary.
   - Adaptive styling harmonized across dark and light themes.

9. 100% Complete Lexilla Lexers Adoption (125 Languages):
   - Full adoption of all 125 Lexilla lexer modules in `src/languages/`, registered in `src/scripts.qrc` and synchronized in `src/scripts/init.lua`.
   - Comprehensive syntax highlighting for modern programming languages, configuration formats, and technical DSLs.

10. Keyboard Shortcut Collision Audit & Resolution:
    - Audited and resolved keybinding conflicts:
      - `Ctrl+Alt+Shift+E`: AES-256 Document Encryption
      - `Ctrl+Shift+R`: Run Python Script
      - `Ctrl+Alt+Shift+T`: Toggle Window Transparency
      - `Ctrl+Alt+Shift+O`: Toggle File Browser Dock
      - `Ctrl+Alt+Shift+F`: Focus Main Editor
      - `Ctrl+Alt+H`: Toggle Hex / Binary View

11. Plaintext (.txt) Format Enforcement & Localized Help Documentation:
    - Docnote and Changelog in AboutDialog and Help menu adapt automatically to the editor's active language (Indonesian or English).
    - Strictly presented in clean plaintext `.txt` monospaced viewer without markdown distraction.

---

## 2. Prior Release: Release v1.3.7

1. Notepad++ Style Tools Menu & 16-Action MIME Tools Suite.
2. Standalone Hash & Authenticity Validator.
3. Interactive Shortcut Mapper in Preferences > Shortcuts.
4. GitHub Rate Limit Elimination via ETag Caching.
"""

CHANGELOG_ID = """# Catatan Perubahan (Changelog) — SparkNotePad

Semua perubahan penting pada **SparkNotePad** didokumentasikan dalam berkas ini.

Format berkas ini mengacu pada [Keep a Changelog](https://keepachangelog.com/id/1.1.0/),
dan proyek ini mengikuti [Semantic Versioning](https://semver.org/lang/id/).

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
  - Tab 3 ("Themes & Toolbar"): Relokasi pengaturan gaya visual Tema (Tema Aktif, Opasitas Jendela, Interval Tail, Proteksi Layar) ke Tab 3 bersama pengatur Paket Ikon Toolbar lengkap (21 paket, kartu deskripsi dinamis, pengatur ukuran ikon, pratinjau langsung, dan pemuat paket kustom).
  - Tab 4 ("Custom Toolbar Config"): Penyesuai visual bilah alat (`CustomizeToolbarWidget`) terintegrasi langsung di Tab 4 dengan pencarian cepat, daftar aksi tersedia vs aktif, tambah/hapus aksi, pengurutan tombol, garis pemisah, dan hot-reload instan.
- **Penataan Ulang Menu Aplikasi Standar Desktop**:
  - Relokasi Tampilan Hex / Biner tepat di bawah Bandingkan (Compare) pada menu tingkat atas (`Ctrl+Alt+H` / `Cmd+Alt+H`).
  - Menempatkan Mulai Ulang Aplikasi (Restart Application) di menu Aplikasi macOS tepat di atas Keluar (Quit) (`QAction::ApplicationSpecificRole`).
  - Menghapus aksi manual Emoji & Simbol demi mendukung palet emoji bawaan macOS (`Ctrl+Cmd+Space`).
  - Merampingkan menu Tampilan (View) dengan menghapus submenu Panel Samping dan aksi pratinjau markdown mandiri redundan.
- **Paritas Penuh 100% Katalog Dwibahasa (i18n)**:
  - Sinkronisasi menyeluruh katalog Bahasa Inggris (`SparkNotePad_en.ts`) dan Bahasa Indonesia (`SparkNotePad_id.ts`) pada 1.924 entri antarmuka pengguna dengan 0 entri belum selesai (unfinished).
- **Tombol Plus (+) pada Setiap Samping Akhir Tab**:
  - Tombol interaktif Plus (+) diletakkan pada title bar di samping kanan tab editor untuk membuat berkas baru dengan cepat.
- **Sinkronisasi Ikon Menubar & Menu Konteks Dinamis (`applyToWindow`)**:
  - Perubahan paket ikon toolbar secara otomatis menyinkronkan seluruh ikon pada toolbar, menubar utama, menu konteks klik kanan tab, dan menu klik kanan Scintilla editor.
  - Ukuran default ikon toolbar adalah 24x24 dan ikon menu adalah 16x16 (dengan aset retina 32x32).
  - Pemilihan ukuran ikon toolbar secara dinamis menyesuaikan geometri dan tata letak toolbar.
- **Pratinjau HTML & Markdown di Tab Editor Baru (`PreviewTabWidget`)**:
  - Pratinjau langsung di tab editor tengah (`ads::CenterDockWidgetArea`), bukan di jendela baru terpisah.
  - Menu konteks tab menyediakan aksi "Preview HTML in New Tab" dan "Preview Markdown in New Tab".
  - Aksi "Preview in Default Browser" khusus berkas `.html` untuk membuka berkas di peramban sistem.
  - Desain modern GitHub/DevDocs dengan kesetaraan tema Dark/Light, tombol perbesaran (+/-), tombol segarkan, dan proteksi sandbox CVE-2026-20841.
- **Folder & File Browser Bergaya File Commander Modern (`FileBrowserDock`)**:
  - Bilah pintasan cepat (Home, Documents, Downloads, Desktop, Workspace).
  - Ikon navigasi SVG tajam (Up, Folder, Refresh).
  - Bilah status metrik menampilkan jumlah berkas, folder, ruang disk kosong (`QStorageInfo`), dan detail seleksi.
  - Keselarasan tema gelap dan terang.
- **Tampilan Hex / Binary Reversibel (`View > Toggle Hex / Binary View` - `Ctrl+Alt+H`)**:
  - Menampilkan offset 8-digit, 16 bita hex (8+8 grouping), dan kolom karakter ASCII.
  - Dapat dibatalkan seketika mengembalikan dokumen asli secara utuh.
- **100% Pengadopsian Lexilla Lexer (125 Bahasa)**:
  - Seluruh 125 lexer Lexilla didukung lengkap dengan deskriptor Lua di `src/languages/`, didaftarkan di `src/scripts.qrc` dan `src/scripts/init.lua`.
- **Audit Pintasan Keyboard & Resolusi Konflik**:
  - `Ctrl+Alt+Shift+E`: Enkripsi Dokumen
  - `Ctrl+Shift+R`: Jalankan Skrip Python
  - `Ctrl+Alt+Shift+T`: Alihkan Transparansi Jendela
  - `Ctrl+Alt+Shift+O`: Buka/Tutup File Browser
  - `Ctrl+Alt+Shift+F`: Fokus ke Editor
  - `Ctrl+Alt+H`: Tampilan Hex / Binary
- **Penegakan Format Plaintext (.txt) & Dokumentasi Bilingual**:
  - Menu Help Docnote dan Changelog disajikan otomatis dalam bahasa aktif editor (Indonesia / English).
  - Disajikan secara murni dalam format plaintext monospaced `.txt`.
"""

CHANGELOG_EN = """# Changelog — SparkNotePad

All notable changes to **SparkNotePad** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.3.8] - 2026-11-04

### 🎨 21 Toolbar Icon Packs Suite, Preferences Themes & Toolbar Tabs, Desktop Menu Ergonomics & 100% i18n Parity

SparkNotePad v1.3.8 features an extensive toolbar personalization architecture with 21 curated icon packs, dedicated Preferences tabs ("Themes & Toolbar" & "Custom Toolbar Config"), desktop standard menu reorganization, native vector SVG scaling, multi-resolution `@2x` Retina pixmaps, intelligent dark-mode luminance tone mapping, Plus (+) tab button, in-tab HTML and Markdown preview, modernized File Commander browser, reversible Hex/Binary viewing, and 100% Lexilla Lexers parity:

#### 🆕 Added

- **21 Distinct Multi-Resolution Toolbar Icon Packs Suite**:
  - Seamlessly switch among 21 built-in vector and pixel icon collections: Silk, Modern Ribbon, Fluent, Win7, Win11, Monoline, Material Sharp, Pixel Fugue, Monoline Minimal, Notepad++ Green, Notepad3 Warm, Humanity GNOME, Elementary Flat, Breeze KDE, Papirus, Cupertino, VS Code Codicons.
  - Dynamic multi-DPI icon scaling: 16x16, 20x20, 24x24 (Default), 32x32, 48x48, 64x64.
  - Crisp Retina @2x pixmaps without blur or distortion.
  - Intelligent Dark Mode HSL luminance tone mapping preventing icon washing-out on dark canvases.
- **Dedicated Preferences Tabs ("Themes & Toolbar" & "Custom Toolbar Config")**:
  - Tab 3 ("Themes & Toolbar"): Relocated Themes styling (Active Theme, Window Opacity, Tail Interval, Display Safety) into a unified tab with Toolbar Icon Packs manager (21 packs, responsive description card layout, size selector, icon previews, custom pack folder launcher, and rescan).
  - Tab 4 ("Custom Toolbar Config"): Embedded visual toolbar customizer widget (`CustomizeToolbarWidget`) directly inside Preferences with live search, action adding/removing/reordering, separator insertion, and hot-reload.
- **Standard Desktop Editor Menu Hierarchy**:
  - Reversible Hex/Binary View relocated directly below Compare in the top-level Tools/View menu (`Ctrl+Alt+H` / `Cmd+Alt+H`).
  - Restart Application relocated to macOS Application menu directly above Quit (`QAction::ApplicationSpecificRole`).
  - Duplicate manual Character Palette removed in favor of native macOS system palette (`Ctrl+Cmd+Space`).
  - View menu streamlined without duplicate Side Panels submenu.
- **100% Bilingual Localization Parity**:
  - Full synchronization of English (`SparkNotePad_en.ts`) and Indonesian (`SparkNotePad_id.ts`) translation catalogs across 1,924 UI strings with 0 unfinished entries.
- **Plus (+) Button at the End of Every Tab Bar**:
  - Interactive Plus (+) button located on the title bar next to editor tabs to open a new document instantly.
- **Dynamic Menubar & Context Menu Icon Synchronization (`applyToWindow`)**:
  - Changing icon packs now propagates dynamically to all top-level menus in `menuBar()`, tab right-click context menus, and the Scintilla editor right-click context menu.
  - Default toolbar icon size is 24x24 and default menu icon size is 16x16 (with 32x32 retina pixmaps).
  - Icon size selection immediately resizes and updates toolbar geometry dynamically.
- **In-Tab & Dock HTML/Markdown Live Preview (`PreviewTabWidget` & `MarkdownSafePreviewDock`)**:
  - Center dock area (`ads::CenterDockWidgetArea`) in-tab preview for `.html` and `.md` files without opening detached external windows.
  - Tab right-click context menu provides instant "Preview HTML in New Tab" and "Preview Markdown in New Tab".
  - Dedicated "Preview in Default Browser" action for `.html` and `.md` documents (`Ctrl+Alt+B`).
  - Modern, elegant GitHub/DevDocs styling with adaptive Dark and Light mode parity, zoom controls (+ / - / reset), refresh button, and CVE-2026-20841 sandboxing.
  - Live document stats pill badge displaying word count and estimated reading time (`%1 words • %2 min read`).
  - GitHub Alert Callout formatting for `[!NOTE]`, `[!TIP]`, `[!IMPORTANT]`, `[!WARNING]`, and `[!CAUTION]` with distinct theme accents, subtle background tints, and SVG icons.
- **Modern File Commander & Workspace TreeView Selection Highlight Fix**:
  - Resolved selection box clipping/cropping in `FolderAsWorkspaceDock` and `FileBrowserDock` via full-row selection (`show-decoration-selected: 1`, `branch:selected`, full focus).
  - Highlighting seamlessly tracks active theme selection colors with rounded corners and uniform row heights.
  - Quick bookmarks (Home, Documents, Downloads, Desktop, Workspace) with crisp SVG icons.
  - Bottom status bar displaying file count, folder count, available disk space (`QStorageInfo`), and selection summary.
- **Reversible Hex / Binary View (`View > Toggle Hex / Binary View` - `Ctrl+Alt+H`)**:
  - Real-time in-place hex inspection displaying 8-digit offset, 16 hex bytes with 8+8 grouping, and ASCII character column.
  - Reversible toggle restoring original text, language lexer, and read-only flags seamlessly.
- **100% Lexilla Lexers Adoption (125 Languages)**:
  - Complete adoption of all 125 Lexilla lexer modules in `src/languages/`, registered in `src/scripts.qrc` and synchronized in `src/scripts/init.lua`.
  - Comprehensive syntax highlighting for modern programming languages, configuration formats, and technical DSLs.
- **Keyboard Shortcut Collision Audit & Resolution**:
  - Audited and resolved keybinding conflicts:
    - `Ctrl+Alt+Shift+E`: AES-256 Document Encryption
    - `Ctrl+Shift+R`: Run Python Script
    - `Ctrl+Alt+Shift+T`: Toggle Window Transparency
    - `Ctrl+Alt+Shift+O`: Toggle File Browser Dock
    - `Ctrl+Alt+Shift+F`: Focus Main Editor
    - `Ctrl+Alt+H`: Toggle Hex / Binary View
- **Plaintext (.txt) Format Enforcement & Localized Help Documentation**:
  - Docnote and Changelog in AboutDialog and Help menu adapt automatically to the editor's active language (Indonesian or English).
  - Strictly presented in clean plaintext `.txt` monospaced viewer without markdown distraction.
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
