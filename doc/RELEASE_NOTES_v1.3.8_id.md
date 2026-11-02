# Catatan Rilis SparkNotePad v1.3.8 — Rangkaian Paket Ikon Toolbar, Sinkronisasi Menu Dinamis & Dialog Kustomisasi

**Versi Rilis:** `v1.3.8`<br/>
**Tanggal Rilis:** 2026-11-02<br/>
**Pengelola Utama:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Mesin Inti Editor:** Scintilla 5.6.7 (Vendored) & Lexilla 5.5.4 (Vendored)<br/>
**Inti Embedded Lua:** Sandboxed Lua 5.5.1 / 5.4.7 (Official Source)<br/>
**Runtime Scripting:** Python 3.13+ Engine Bridge<br/>
**Standar:** ISO C++20 / Qt 6.5–6.12<br/>
**Platform Target:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu 22.04–26.04, Debian 12–14, Fedora 40–44, RHEL/Rocky 9–11, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x (KDE Plasma 6 & XFCE)

---

## Ringkasan Eksekutif

SparkNotePad v1.3.8 menghadirkan perombakan besar arsitektur visual dan personalisasi bilah alat (toolbar) yang terinspirasi oleh Notepad++, dilengkapi dengan sinkronisasi ikon dinamis ke seluruh menu bar dan menu klik kanan editor, paket ikon otentik Windows XP dan Windows 7/8 Aero Glass Azure resolusi tinggi, dukungan rentang ukuran ikon dari 16×16 hingga 64×64, serta pratinjau Markdown resmi bergaya GitHub.

---

## Fitur Utama & Penyempurnaan

### 1. Rangkaian 15 Paket Ikon Toolbar Bawaan

SparkNotePad v1.3.8 menyediakan 15 paket ikon vektor dan bitmap pilihan yang dapat diakses langsung melalui menu `Tema > Paket Ikon Toolbar`:

- **Modern Ribbon (Standar)** (`modernRibbon`): Suite vektor SVG monoline 20×20 High-DPI dengan kontur halus.
- **NotepadNext Klasik (Silk)** (`notepadNext`): Ikon bitmap Silk 16×16 otentik dari NotepadNext, direlokasi rapi ke folder `icons/packs/notepadNext/`.
- **Notepad++ Retro** (`notepadPlusPlus`): Rekreasi vektor High-DPI ikonografi klasik Notepad++.
- **android material 2026** (`androidMaterial`): Geometri adaptif Google Material You / M3 dan kedalaman tonal.
- **Vista 2026** (`windowsVista`): Estetika kaca Windows Vista Aero dengan kilau kaya dan pencahayaan volumetrik.
- **XP 2026** (`windowsXp`): Rekonstruksi nostalgia skeuomorfik 3D Windows XP Luna otentik dari master PNG resolusi tinggi 1024×1024 dan Directory Opus 10 XP suite (master 64×64 PNG).
- **Win10 2026** (`windows10`): Estetika monoline wireframe tajam Windows 10 Metro / MDL2.
- **Win11 2026** (`windows11`): Microsoft Fluent Design System 2 dengan geometri membulat dan kedalaman halus dari Directory Opus 32px.
- **Win 7 2026** (`windows7`): Rangkaian Windows 7/8 Aero Glass Azure otentik dari lembar sprite UltimateDesktops dengan gradien kaca sian tembus pandang, aksen bingkai krom, dan kilau volumetrik (master 64×64 PNG).
- **Monterey 2026** (`macosMonterey`): Estetika modern SF Symbols macOS Monterey dengan gaya bersih khas Apple.
- **NOTEPAD3 ICONPACK** (`notepad3`): Teal teknis Rizonesoft Notepad3 dan vektor tajam berorientasi pengembang.
- **NOTEPAD4 ICONPACK** (`notepad4`): Ikonografi vektor datar nila gelap modern Zufuliu Notepad4.
- **NOTEPAD2 ICONPACK** (`notepad2`): Palet nostalgia 16-warna retro klasik Florian Balmer Notepad2.
- **Monochrome Outline (Minimalis)** (`monochrome`): Wireframe monoline 1.8px bersih dirancang untuk tema gelap dan terang.
- **Vibrant DuoTone (Neon/Cyber)** (`vibrantDuotone`): Kontras dua warna sian elektrik luminansi tinggi & magenta bercahaya.

### 2. Sinkronisasi Ikon Menubar & Menu Konteks Klik Kanan Dinamis

Penggantian paket ikon toolbar kini secara otomatis menyinkronkan seluruh ikon aksi pada Menu Bar utama, submenu, dan menu klik kanan editor (Editor Context Menu) secara instan melalui `ToolbarIconManager::applyToWindow()`. Seluruh aksi menu dipastikan menampilkan ikon secara konsisten dengan penegakan `setIconVisibleInMenu(true)` pada macOS Retina dan desktop Linux.

### 3. Skala Ukuran Ikon Toolbar Dinamis (16×16 hingga 64×64)

Ubah ukuran ikon secara instan dan bebas restart melalui `Tema > Paket Ikon Toolbar > Ukuran Ikon Toolbar` atau klik kanan pada bilah alat:

- **16 × 16** (Kompak / Compact)
- **20 × 20** (Sedang / Medium)
- **24 × 24** (Standar / Disarankan)
- **32 × 32** (Besar / DPI Tinggi)
- **48 × 48** (Ekstra Besar / Extra Large)
- **64 × 64** (Ultra High-DPI / Resolusi Tinggi)

### 4. Pratinjau Markdown & HTML Bergaya GitHub (`MarkdownSafePreviewDock`)

- Mengimplementasikan tata gaya GitHub Markdown resmi (sesuai spesifikasi ekstensi VS Code `bierner.markdown-preview-github-styles`).
- Mendukung palet terang (`#ffffff` / `#1f2328`) dan gelap (`#0d1117` / `#e6edf3`) secara adaptif mengikuti tema aktif aplikasi.
- Tipografi resmi GitHub, garis bawah judul (h1/h2), callout blockquote dengan batas `0.25em`, blok kode berpagar, tabel belang zebra, tag `<kbd>`, dan ekspor peramban web mandiri.

### 5. Dialog Kustomisasi Tombol Toolbar Visual (`CustomizeToolbarDialog`)

- Antarmuka interaktif daftar ganda (dual-list) untuk menambah, menghapus, dan mengatur urutan tombol toolbar dengan drag-and-drop (`InternalMove`).
- Penyisipan garis pemisah (separator) kustom.
- Filter pencarian langsung untuk menemukan aksi dengan cepat.
- Tombol Reset to Defaults untuk memulihkan susunan standar 33 tombol bawaan.

### 6. Ergonomi Menu Klik Kanan Editor

- **Relokasi Convert Case**: Diposisikan tepat di bawah `Pilih Semua` (`ui->actionSelectAll`) untuk alur kerja transformasi teks yang lebih intuitif.
- **Tipografi & Ikon Ramping**: Font diperkecil 1–2px (11px) dan ikon disesuaikan ke ukuran 14×14px dengan padding 3px.
- **Pemulihan Gaya Bersih**: Mengaitkan sinyal `aboutToHide` untuk memulihkan gaya menu bar utama tanpa efek samping styling.

---

## Verifikasi Empiris

- **Pengujian Unit**: Tingkat kelulusan 100% (55/55 kasus uji pada 11 rangkaian uji coba).
- **Kepatuhan Linter**: Trunk check, markdownlint, dan cppcheck lulus dengan 0 kesalahan.
- **Status Kompilasi**: Dikompilasi bersih menggunakan Apple LLVM / Clang 23 di macOS dan GCC/Clang di Linux.
