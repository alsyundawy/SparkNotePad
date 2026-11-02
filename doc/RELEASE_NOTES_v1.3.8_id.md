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

### 1. Rangkaian 10 Paket Ikon Toolbar Bawaan

SparkNotePad v1.3.8 menyediakan 10 paket ikon vektor dan bitmap pilihan yang dapat diakses langsung melalui menu `Tema > Paket Ikon Toolbar`:

- **Notepad++ Classic** (`notepadPlusPlus`): Rekreasi vektor dan bitmap ikonografi klasik Notepad++.
- **Fluent UI** (`fluentUi`): Desain modern Microsoft Fluent dengan geometri membulat dan aset High-DPI jernih.
- **Fugue** (`fugue`): Ikonografi legendaris 16×16 & 32×32 pixel-perfect rancangan Yusuke Kamiyamane.
- **Windows 11** (`windows11`): Ikon Fluent Microsoft Windows 11 dengan kedalaman halus dan bayangan elegan.
- **VS Code Modern** (`vscodeModern`): Ikonografi pengembang yang bersih dan minimalis terinspirasi Visual Studio Code.
- **Flat Color** (`flatColor`): Ikon warna datar kontras tinggi yang dirancang untuk layar beresolusi modern.
- **Material Design** (`material`): Pedoman simbol Google Material Design dengan bobot seimbang.
- **Solarized** (`solarized`): Harmoni warna yang disetel khusus untuk tema editor Solarized Gelap dan Terang.
- **High Contrast** (`highContrast`): Ikon wireframe monokrom ultra-tajam untuk alur kerja visibilitas tinggi.
- **Windows Vista** (`windowsVista`): Gaya Aero Glass Windows Vista klasik dengan gradien berkilau dan aksen krom.

### 2. Sinkronisasi Ikon Menubar & Menu Konteks Klik Kanan Dinamis

Penggantian paket ikon toolbar kini secara otomatis menyinkronkan seluruh ikon aksi pada Menu Bar utama, submenu, dan menu klik kanan editor (Editor Context Menu) secara instan melalui `ToolbarIconManager::applyToWindow()`. Seluruh aksi menu dipastikan menampilkan ikon secara konsisten dengan penegakan `setIconVisibleInMenu(true)` pada macOS Retina dan desktop Linux.

### 3. Skala Ukuran Ikon Toolbar Dinamis (16×16 hingga 64×64)

Ubah ukuran ikon secara instan dan bebas restart melalui `Tema > Paket Ikon Toolbar > Ukuran Ikon Toolbar` atau klik kanan pada bilah alat:

- **16 × 16** (Kompak / Menu)
- **20 × 20** (Sedang / Medium)
- **24 × 24** (Standar Toolbar / Disarankan)
- **32 × 32** (Besar / DPI Tinggi)
- **48 × 48** (Ekstra Besar / Extra Large)
- **64 × 64** (Ultra High-DPI / Resolusi Tinggi)

### 4. Tombol Plus (+) pada Tab Bar untuk Pembuatan Dokumen Instan

Bilah tab editor kini dilengkapi tombol Plus (`+`) elegan (`tabBarPlusButton`) tepat di samping tab aktif untuk membuka tab dokumen baru secara instan dengan satu klik, melengkapi pintasan `Ctrl+N` / `Cmd+N`.

### 5. Lokalisasi Menu Bantuan: DOCNOTE & CHANGELOG

Menu Bantuan (Help) kini secara dinamis menyinkronkan catatan rilis dan log perubahan sesuai bahasa editor yang aktif:

- **Bahasa Indonesia:** Menampilkan `DOCNOTE_id.txt` dan `CHANGELOG_id.txt` saat bahasa Indonesia aktif.
- **English:** Menampilkan `DOCNOTE_en.txt` dan `CHANGELOG_en.txt` untuk bahasa Inggris dan bahasa umum lainnya.

### 6. Skalabilitas Universal High-DPI, Retina & Multi-Monitor

Mesin render Scintilla dan Qt dioptimalkan sepenuhnya untuk berbagai perangkat keras monitor tanpa buram atau artefak penskalaan:

- **Monitor Generik:** Resolusi 1366×768, 1280×800, 720p, dan 1080p standar.
- **MacBook Retina:** Apple Silicon dan Intel Retina (skala 2x dengan dukungan ProMotion refresh tinggi).
- **Layar Eksternal Resolusi Tinggi:** Monitor 2K, 4K, 5K, 6K, dan penskalaan fraksional Linux (125%, 150%, 175%).
- **Rendering Font Teroptimasi LCD:** Mengaktifkan `SC_EFF_QUALITY_LCD_OPTIMIZED` dan double-buffering (`setBufferedDraw(true)`) dengan repaint instan saat `DevicePixelRatioChange` dan `ScreenChangeInternal`.

### 7. Pratinjau Markdown & HTML Bergaya GitHub (`MarkdownSafePreviewDock`)

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
