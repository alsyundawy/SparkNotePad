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
Versi Rilis          : 1.3.9 (Scintilla 5.6.9, Lexilla 5.5.5, Default Language, Qt 6.2 LTS & 14-Pillar Hardening)
Tanggal Rilis        : 2026-11-07
Pengelola Utama      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organisasi           : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repositori           : https://github.com/alsyundawy/SparkNotePad
Mesin Penyunting     : Scintilla 5.6.9 & Lexilla 5.5.5 (125 Lexer Resmi)
Runtime Skrip        : Sandboxed Lua 5.5.1 & Python 3.13+ Bridge
Kerangka Kerja GUI   : Qt 6.2 LTS / 6.5 LTS / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platform      : macOS 10.15–26.0 (Apple Silicon & Intel), Linux (x86_64 & ARM64), FreeBSD 13–15
Lisensi              : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Ringkasan Eksekutif (Bahasa Indonesia): Rilis v1.3.9

SparkNotePad v1.3.9 adalah rilis peningkatan fondasional dan stabilitas besar yang merangkum seluruh evolusi teknis dari v1.3.8 ke v1.3.9. Rilis ini mencakup peningkatan mesin inti Scintilla dan Lexilla langsung dari upstream resmi, resolusi kerentanan regex loop, penambahan preferensi bahasa default, pewarnaan front-matter Markdown, restorasi penuh kompatibilitas Qt 6.2 LTS (macOS Catalina & Ubuntu 20.04), arsitektur rilis multi-arsitektur Linux ARM64 & x64 (50+ paket biner), perampingan dan penyempurnaan packaging DMG macOS, serta lolos audit kualitas 14 pilar tanpa kompromi:

1. Integrasi Penuh Upstream Scintilla 5.6.9 & Lexilla 5.5.5:
   - Pembaruan menyeluruh source code Scintilla ke 5.6.9 dan Lexilla ke 5.5.5 resmi dari scintilla.org di thirdparty/ (160 berkas diperbarui, 3.206 baris penambahan).
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

5. Pembaruan Dialog About & Dinamisasi Metadata:
   - Di src/dialogs/AboutDialog.cpp, Release Tag kini terhubung langsung secara dinamis ke APP_VERSION sehingga selalu menunjuk ke v1.3.9 tanpa hardcode tag usang.
   - Pembaruan label mesin inti menjadi Scintilla 5.6.9 & Lexilla 5.5.5 pada tab About, Maintainer, dan kartu Diagnostik.
   - Sinkronisasi teks salin informasi sistem (clipboard system info) mencerminkan Scintilla 5.6.9 dan Lexilla 5.5.5.
   - Pengakuan eksplisit NotepadNext-MacOS (@alsyundawy) sebagai repositori port fondasional macOS.

6. Kompatibilitas Penuh Qt 6.2 LTS (macOS Catalina 10.15 & Ubuntu 20.04 LTS):
   - Di src/utils/TokenForensicUtils.cpp, penggantian QTimeZone::UTC (Qt 6.5+) menjadi QTimeZone::utc() yang didukung universal sejak Qt 6.2.
   - Di src/dialogs/MainWindow.cpp, penggantian QLatin1StringView (Qt 6.4+) menjadi QLatin1String (Qt 6.2+).
   - Di src/widgets/TabsQuickActionsBar.cpp & .h, perbaikan QLatin1StringView dan pembersihan specifier inline redundan.
   - Di src/features/ThemeManager.cpp, penyediaan fungsi pembantu parseColor sebagai fallback QColor::fromString untuk Qt < 6.4.
   - Di src/utils/HashIntegrityManager.cpp, pemagaran fungsi kriptografi Qt 6.4+ dengan pengecekan QT_VERSION_CHECK(6, 4, 0).

7. Restorasi Matriks CI & Distribusi Rilis Multi-Arsitektur (50+ Berkas):
   - Pengembalian penuh kompilasi Qt 6.2 LTS untuk runner macOS Catalina dan Ubuntu 20.04.
   - Penambahan packaging biner native untuk Linux ARM64 (aarch64) lintas Qt 6.8, 6.10, 6.11, dan 6.12.
   - Prioritas eksekusi dan publikasi x64 lebih awal, serta penerbitan aset rilis instan per-job.
   - Default build rilis dan artefak dipatok pada Qt 6.5 LTS.

8. Perampingan Framework & Peningkatan Estetika DMG macOS:
   - Pembersihan framework Qt yang tidak digunakan (QtPdf, QtQml, QtQuick, QtVirtualKeyboard) pada paket DMG untuk mengurangi ukuran binary (~15 MB dihemat).
   - Penyempurnaan pipeline packaging DMG dengan background kustom, indikator drag-and-drop, jendela 680x440, eliminasi fallback unstyled --skip-jenkins di CI, serta validasi integritas checksum hdiutil.
   - Build DMG lokal SparkNotePad-v1.3.9-x64.dmg (33 MB) berhasil dibuat dan diverifikasi valid secara lokal.

9. Paritas Terjemahan 100% (i18n):
   - Sinkronisasi 1.850 entri terjemahan dwibahasa (Bahasa Indonesia & Inggris) dengan 0 entri belum selesai.

10. Verifikasi Kualitas Kode 14 Pilar & Audit Tanpa Cacat:
    - Nol peringatan pada clangd --check, cppcheck (152 berkas src/), clang-tidy, dan SonarLint.
    - Penegakan kompilasi Unix Makefiles standar dengan konkurensi terkontrol -j2 (Ninja dilarang permanen).
    - Lolos 51/51 unit tests pada SparkNotePadTests.
    - Paritas 100% modul Python 3.14 dan 3.15 (73 modul identik).

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
Release Version      : 1.3.9 (Scintilla 5.6.9, Lexilla 5.5.5, Default Language, Qt 6.2 LTS & 14-Pillar Hardening)
Release Date         : 2026-11-07
Lead Maintainer      : Harry Dertin Sutisna Alsyundawy (@alsyundawy)
Organization         : ALSYUNDAWY IT SOLUTION (https://alsyundawy.com)
Repository           : https://github.com/alsyundawy/SparkNotePad
Core Editing Engine  : Scintilla 5.6.9 & Lexilla 5.5.5 (Complete 125 Official Lexers)
Embedded Lua Core    : Sandboxed Lua 5.5.1 & Python 3.13+ Bridge
GUI Framework        : Qt 6.2 LTS / 6.5 LTS / 6.8 LTS / 6.10 / 6.11 / 6.12 (C++20 ISO Standard)
Target Platforms     : macOS 10.15–26.0 (Apple Silicon & Intel), Linux (x86_64 & ARM64), FreeBSD 13–15
License              : GNU General Public License v3.0 (GPL-3.0-or-later)
================================================================================
--------------------------------------------------------------------------------

---

## 1. Executive Summary (English): Release v1.3.9

SparkNotePad v1.3.9 is a major foundational engine upgrade and stability milestone encapsulating the complete architectural progression from v1.3.8 to v1.3.9. It delivers upstream core editing engine upgrades to Scintilla 5.6.9 and Lexilla 5.5.5, resolves algorithmic regex loop vulnerabilities, introduces user-configurable default language preferences, expands Markdown front-matter syntax styling, restores full Qt 6.2 LTS backward compatibility (macOS Catalina & Ubuntu 20.04), establishes a 50+ binary release matrix with native Linux ARM64 packaging, prunes and refines macOS DMG distribution, and passes comprehensive 14-pillar code review:

1. Official Upstream Scintilla 5.6.9 & Lexilla 5.5.5 Core Integration:
   - Full upstream replacement of Scintilla 5.6.9 and Lexilla 5.5.5 directly from scintilla.org into thirdparty/ (160 files updated, 3,206 insertions).
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

5. About Dialog Overhaul & Dynamic Metadata Architecture:
   - In src/dialogs/AboutDialog.cpp, dynamically parameterized the Release Tag link with APP_VERSION so it always links to v1.3.9 without hardcoded stale tags.
   - Updated engine attribution to Scintilla 5.6.9 & Lexilla 5.5.5 across About, Maintainer, and Diagnostics cards.
   - Synchronized plain-text system info copy clipboard to reflect Scintilla 5.6.9 and Lexilla 5.5.5.
   - Explicitly preserved attribution to NotepadNext-MacOS (@alsyundawy) as the foundational macOS port repository.

6. Full Qt 6.2 LTS Backward Compatibility (macOS Catalina 10.15 & Ubuntu 20.04 LTS):
   - In src/utils/TokenForensicUtils.cpp, replaced QTimeZone::UTC (Qt 6.5+) with universal QTimeZone::utc() supported across all Qt 6 releases.
   - In src/dialogs/MainWindow.cpp, replaced QLatin1StringView (Qt 6.4+) with universal QLatin1String (Qt 6.2+).
   - In src/widgets/TabsQuickActionsBar.cpp & .h, replaced QLatin1StringView and cleaned redundant inline specifiers.
   - In src/features/ThemeManager.cpp, implemented parseColor fallback for Qt < 6.4 where QColor::fromString is absent.
   - In src/utils/HashIntegrityManager.cpp, guarded Qt 6.4+ cryptographic APIs with QT_VERSION_CHECK(6, 4, 0).

7. Restored Multi-Architecture CI Matrix & Automated Release Publishing:
   - Restored and verified Qt 6.2 LTS matrix jobs on macOS Catalina and Ubuntu 20.04 runners.
   - Added native Linux ARM64 (aarch64) packaging across Ubuntu, Debian, and RPM distros for Qt 6.8, 6.10, 6.11, and 6.12.
   - Prioritized x64 jobs first, publishing release artifacts immediately upon individual job completion.
   - Pinned canonical default release artifact to Qt 6.5 LTS.
   - Expanded release artifact catalog to 50+ distribution binaries.

8. Framework Pruning & macOS DMG Aesthetics Enhancement:
   - Pruned unused Qt frameworks (QtPdf, QtQml, QtQuick, QtVirtualKeyboard) from release DMGs, saving ~15MB.
   - Enhanced DMG packaging pipeline with custom background, drag-and-drop indicator, 680x440 window layout, eliminated unstyled fallback --skip-jenkins in CI, and validated checksum integrity via hdiutil.
   - Verified local DMG generation: build/SparkNotePad-v1.3.9-x64.dmg (33 MB) successfully created and verified.

9. 100% Bilingual Localization Parity (i18n):
   - Synchronized 1,850 translation strings across Indonesian and English catalogs with 0 unfinished entries.

10. 14-Pillar Code Quality & Zero-Defect Linter Verification:
    - Zero warnings across clangd --check, cppcheck (152 src/ files), clang-tidy, and SonarLint.
    - Strict Unix Makefiles compilation enforcement with controlled -j2 concurrency (Ninja permanently prohibited).
    - 51/51 unit tests passed in SparkNotePadTests.
    - 100% parity across Python 3.14 and 3.15 environments (73 identical modules).

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

### ⚡ Pembaruan Mesin Scintilla 5.6.9 & Lexilla 5.5.5, Pengaturan Bahasa Default, Kompatibilitas Qt 6.2 LTS, Matriks Rilis Linux ARM64 & Pengerasan 14 Pilar

SparkNotePad v1.3.9 adalah rilis fondasional besar yang merangkum seluruh perubahan arsitektural dari v1.3.8 ke v1.3.9. Rilis ini memperbarui mesin inti Scintilla 5.6.9 dan Lexilla 5.5.5, menyelesaikan kerentanan ekspresi reguler, menambahkan preferensi bahasa default, memperluas penyorotan sintaksis front-matter Markdown, memulihkan kompatibilitas penuh Qt 6.2 LTS (macOS Catalina & Ubuntu 20.04), menghadirkan matriks rilis Linux ARM64 & x64 (50+ paket biner), serta lolos audit kualitas kode 14 pilar tanpa peringatan:

#### 🆕 Ditambahkan

- **Integrasi Penuh Scintilla 5.6.9 & Lexilla 5.5.5 Upstream Resmi**:
  - Pembaruan Scintilla dari 5.6.7 ke 5.6.9 resmi scintilla.org di thirdparty/scintilla (160 berkas diperbarui, 3.206 baris penambahan).
  - Pembaruan Lexilla dari 5.5.4 ke 5.5.5 resmi scintilla.org di thirdparty/lexilla.
  - Regenerasi Qt wrapper (ScintillaEdit.cpp dan ScintillaEdit.h) via WidgetGen.py resmi.
  - Peta kategori karakter dan konversi teks standar Unicode 17 lengkap.
  - Restorasi rendering warna penuh untuk format gambar XPM.
  - Akselerasi kueri posisi baris awal saat scrolling cepat.
  - Peningkatan kapasitas cache lexeme dari 30 ke 40 bita untuk identifier panjang dan kolom query SQL.
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
- **Matriks Rilis Linux ARM64 & x64 Multi-Distro (50+ Berkas Biner)**:
  - Penambahan paket rilis biner native Linux ARM64 (aarch64) untuk DEB dan RPM di seluruh matriks Qt 6.8, 6.10, 6.11, dan 6.12.
  - Pemulihan runner macOS Catalina (Qt 6.2 LTS) dan Ubuntu 20.04 LTS (Qt 6.2 LTS).
  - Publikasi langsung aset rilis saat job kompilasi selesai tanpa menunggu seluruh matriks tuntas.
  - Pengutamaan proses build arsitektur x64 terlebih dahulu.
  - Penetapan target build rilis dan artefak default ke Qt 6.5 LTS.
- **Dinamisasi Metadata Dialog About**:
  - Release Tag terhubung secara dinamis ke APP_VERSION sehingga selalu menunjuk ke v1.3.9.
  - Pemutakhiran string Scintilla 5.6.9 dan Lexilla 5.5.5 pada tab About, Maintainer, dan Diagnostik.
  - Pengakuan eksplisit NotepadNext-MacOS (@alsyundawy) sebagai repositori port fondasional macOS.

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
- **Stabilitas & Kompatibilitas Qt 6.2 LTS**:
  - Mengganti QTimeZone::UTC menjadi QTimeZone::utc() di TokenForensicUtils.cpp untuk kompatibilitas Qt 6.2+.
  - Mengganti QLatin1StringView menjadi QLatin1String di MainWindow.cpp dan TabsQuickActionsBar.cpp.
  - Menghapus specifier inline redundan pada TabsQuickActionsBar.h.
  - Menyediakan fungsi pembantu parseColor sebagai fallback QColor::fromString untuk Qt < 6.4.
  - Pemagaran fungsi kriptografi Qt 6.4+ di HashIntegrityManager.cpp.
- **Penyempurnaan Packaging macOS DMG**:
  - Menghilangkan fallback unstyled --skip-jenkins di CI agar DMG selalu ber-layout grafis resmi.
  - Pembersihan framework Qt bloatware tak terpakai (QtPdf, QtQml, QtQuick, QtVirtualKeyboard) menghemat ~15 MB.
  - Pembuatan DMG lokal SparkNotePad-v1.3.9-x64.dmg (33 MB) terverifikasi valid dengan hdiutil.

#### 🔧 Kualitas Kode, i18n & Verifikasi 14 Pilar

- 100% paritas terjemahan dwibahasa (1.850 selesai, 0 belum selesai) untuk Bahasa Indonesia dan Inggris.
- Nol peringatan pada clangd --check, cppcheck (152 berkas src/), clang-tidy, dan SonarLint.
- Penegakan kompilasi Unix Makefiles standar dengan kontrol batas beban -j2 (Ninja dilarang permanen).
- Lolos 51/51 unit tests pada SparkNotePadTests.
- Paritas 100% modul Python 3.14 dan 3.15 (73 modul identik).

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

### ⚡ Scintilla 5.6.9 & Lexilla 5.5.5 Engine Upgrade, Default Language Setting, Qt 6.2 LTS Compatibility, Linux ARM64 Matrix & 14-Pillar Hardening

SparkNotePad v1.3.9 is a major foundational milestone consolidating all architectural evolutions from v1.3.8 to v1.3.9. It upgrades the core editing subsystem to Scintilla 5.6.9 and Lexilla 5.5.5, resolves algorithmic regular expression vulnerabilities, adds user-configurable default language preferences, expands Markdown front-matter syntax styling, restores complete Qt 6.2 LTS compatibility (macOS Catalina & Ubuntu 20.04), establishes a 50+ binary release matrix with native Linux ARM64 packaging, and passes rigorous 14-pillar static analysis and code review:

#### 🆕 Added

- **Official Scintilla 5.6.9 & Lexilla 5.5.5 Core Integration**:
  - Upgraded embedded Scintilla engine from 5.6.7 to 5.6.9 (official upstream release from scintilla.org) in thirdparty/scintilla (160 files updated, 3,206 insertions).
  - Upgraded embedded Lexilla lexer library from 5.5.4 to 5.5.5 (official upstream release from scintilla.org) in thirdparty/lexilla.
  - Regenerated Qt wrapper interface (ScintillaEdit.cpp and ScintillaEdit.h) via upstream WidgetGen.py.
  - Full Unicode 17 character category mapping and case conversions.
  - Restored full-color rendering for XPM images.
  - Accelerated line-start position queries by skipping unnecessary line layout calculations during rapid scrolling.
  - Expanded cached lexeme buffer from 30 to 40 bytes for large identifiers and SQL query columns.
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
- **Multi-Architecture Linux ARM64 & x64 Release Matrix (50+ Distribution Binaries)**:
  - Added native Linux ARM64 (aarch64) packaging for Ubuntu, Debian, and RPM distributions across Qt 6.8, 6.10, 6.11, and 6.12.
  - Restored macOS Catalina (Qt 6.2 LTS) and Ubuntu 20.04 LTS (Qt 6.2 LTS) matrix runners.
  - Immediate publishing of release assets per job completion without waiting for whole matrix completion.
  - Prioritized x64 build pipeline execution.
  - Pinned canonical default release artifact to Qt 6.5 LTS.
- **About Dialog Overhaul & Dynamic Metadata Architecture**:
  - Release Tag dynamically parameterized with APP_VERSION so it always links to v1.3.9.
  - Updated Scintilla 5.6.9 and Lexilla 5.5.5 labels across About, Maintainer, and Diagnostics tabs.
  - Explicitly recognized NotepadNext-MacOS (@alsyundawy) as the ancestral macOS port repository.

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
- **Qt 6.2 LTS Stability & Backward Compatibility**:
  - Replaced QTimeZone::UTC with universal QTimeZone::utc() in TokenForensicUtils.cpp for Qt 6.2+ support.
  - Replaced QLatin1StringView with QLatin1String in MainWindow.cpp and TabsQuickActionsBar.cpp.
  - Removed redundant inline specifiers in TabsQuickActionsBar.h.
  - Implemented parseColor fallback for Qt < 6.4 where QColor::fromString is absent.
  - Guarded Qt 6.4+ cryptographic APIs in HashIntegrityManager.cpp.
- **macOS DMG Packaging & Aesthetic Polish**:
  - Eliminated unstyled --skip-jenkins fallback in CI so DMG always builds with official background, icon, and 680x440 geometry.
  - Pruned bloatware Qt frameworks (QtPdf, QtQml, QtQuick, QtVirtualKeyboard) from DMG bundles, saving ~15 MB.
  - Verified local DMG generation: build/SparkNotePad-v1.3.9-x64.dmg (33 MB) valid and verified with hdiutil.

#### 🔧 Code Quality, i18n & 14-Pillar Verification

- 100% bilingual translation parity (1,850 finished, 0 unfinished) across Indonesian and English catalogs.
- Zero warnings across clangd --check, cppcheck (152 src/ files), clang-tidy, and SonarLint.
- Strict Unix Makefiles compilation enforcement with controlled -j2 concurrency (Ninja permanently prohibited).
- 51/51 unit tests passed in SparkNotePadTests.
- 100% parity across Python 3.14 and 3.15 environments (73 identical modules).

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
