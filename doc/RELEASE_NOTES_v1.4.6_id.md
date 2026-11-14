# Catatan Rilis SparkNotePad v1.4.6 — 41 Paket Ikon Toolbar Baru (Total 69 Koleksi), Ergonomi Menu Konteks, Opsi Preferensi Umum, Perombakan Syntax Highlighting WCAG 2.1 & DMG Rilis 30MB–40MB

**Versi Rilis:** `v1.4.6`<br/>
**Tanggal Rilis:** 2026-11-10<br/>
**Pengelola Utama:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Mesin Penyunting Inti:** Scintilla 5.6.9 (Vendored Resmi) & Lexilla 5.5.5 (Vendored Resmi)<br/>
**Runtime Lua Tersemat:** Sandboxed Lua 5.5.1 (Sumber Resmi)<br/>
**Runtime Skrip:** Python 3.13+ Engine Bridge<br/>
**Standar:** ISO C++20 / Qt 6.5–6.12<br/>
**Target Platform:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu 22.04–26.04, Debian 12–14, Fedora 40–44, RHEL/Rocky 9–11, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x (KDE Plasma 6 & XFCE)

---

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="assets/sparknotepad-desktop-banner.jpg" width="100%" alt="SparkNotePad Desktop Suite — Native Scintilla &amp; Qt Text and Source Code Editor for Linux &amp; macOS" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="logo.png" width="128" height="128" alt="SparkNotePad Native macOS &amp; Linux Application Icon" />
  </a>
</p>

<h1 align="center">SparkNotePad</h1>

<p align="center">
  <b>SparkNotePad — Editor teks dan kode sumber native Scintilla &amp; Qt yang ringan dan super cepat untuk Linux, FreeBSD, dan macOS.</b><br>
  <i>Dirancang dengan Qt 6 dan inti native Scintilla 5.6.9 / Lexilla 5.5.5 untuk performa eksekusi maksimal dan penggunaan memori minimal</i>
</p>

---

> Pengembang Utama & Pengelola:<br>
> **[`HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)`](https://github.com/alsyundawy)** — [`ALSYUNDAWY IT SOLUTION`](https://alsyundawy.com)<br>
>
> 🍏 **[`Rilis Terkini (v1.4.6)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
> 📖 **[`DocNote Rilis (DOCNOTE.md)`](DOCNOTE.md)** &nbsp;|&nbsp;
> 📜 **[`Riwayat Perubahan (CHANGELOG.md)`](CHANGELOG.md)** &nbsp;|&nbsp;
> 🌐 **[`Matriks Dukungan Platform`](doc/PLATFORM_SUPPORT.md)** &nbsp;|&nbsp;
> 🔱 **[`Panduan Kompilasi FreeBSD (KDE6 & XFCE)`](doc/BUILD_FREEBSD_KDE_XFCE.md)** &nbsp;|&nbsp;
> 📋 **[`Analisis & Daftar Tugas FreeBSD`](doc/FREEBSD_BUILD_ANALYSIS_AND_TASKLIST.md)** &nbsp;|&nbsp;
> ⚖️ **[`Perbandingan NotepadNext vs SparkNotePad`](doc/NOTEPADNEXT_VS_SPARKNOTEPAD_COMPARISON.md)** &nbsp;|&nbsp;
> 🐛 **[`Pelacak Masalah & Bug`](https://github.com/alsyundawy/SparkNotePad/issues)** &nbsp;|&nbsp;
> 💖 **[`Dukungan via PayPal`](https://www.paypal.me/alsyundawy)**

---

## ⚡ Ringkasan Eksekutif

SparkNotePad **v1.4.6** menghadirkan peningkatan besar pada kustomisasi visual bilah alat, kenyamanan menu konteks editor, opsi preferensi umum, perombakan pewarnaan sintaksis berstandar rasio kontras WCAG 2.1, serta standardisasi pengemasan berkas macOS DMG 30MB–40MB tanpa bloatware dibandingkan versi 1.4.5. Rilis ini menambahkan 41 paket ikon toolbar baru multi-resolusi (memperluas pustaka bawaan dari 28 menjadi 69 koleksi), memperkenalkan aksi Tempel & Sesuaikan Gaya (Paste & Match Style) serta Pencarian Google langsung di menu konteks editor, menyediakan pengaturan pengguna khusus untuk deteksi encoding otomatis dan konfirmasi simpan semua, meningkatkan pewarnaan sintaksis lintas 70+ tema dengan jaminan rasio kontras 4.0:1 dan lencana komentar emas, serta mengukuhkan pengemasan rilis DMG pada ukuran 38MB–39MB dengan verifikasi eksekusi runtime.

---

## 🌟 Sorotan & Inovasi Utama di v1.4.6

### 1. Ekspansi 41 Paket Ikon Toolbar Baru (Total 69 Koleksi Bawaan)

- **Kustomisasi Visual Multi-Resolusi Tanpa Tanding**:
  - Memperluas pustaka ikon toolbar bawaan dari 28 paket di v1.4.5 menjadi **69 koleksi multi-resolusi lengkap** (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
    - **11 Paket Ikon Tematik**: `royal_blue`, `notepad_pro`, `pastel`, `fluent_modern`, `macos_bigsur_modern`, `dark_neon_pro`, `macos_glass`, `modern_flat_colorful`, `glossy_3d_skeuomorphic`, `retro_8bit_pixel`, `sketchy_doodle`.
    - **30 Paket Ikon Modern**: `01-macos-big-sur`, `02-windows-11-fluent`, `03-arc-browser-glow`, `04-nordic-frost-minimal`, `05-dracula-cyber-neon`, `06-catppuccin-macchiato`, `07-tokyo-night-storm`, `08-synthwave-80s`, `09-monokai-pro-vivid`, `10-solarized-accent`, `11-retro-commodore-64`, `12-retro-win95-classic`, `13-matrix-phosphor-green`, `14-gameboy-nostalgia`, `15-pixel-dungeon-rpg`, `16-claymorphism-3d`, `17-skeuomorphic-leather-brass`, `18-neu-brutalism-bold`, `19-isometric-voxel-3d`, `20-origami-paper-fold`, `21-cyberpunk-2077-yellow`, `22-lofi-aesthetic-grain`, `23-vaporwave-pastel-sunset`, `24-astronomy-deep-space`, `25-botanical-nature-green`, `26-blueprint-schematic`, `27-chalkboard-handdrawn`, `28-gouache-art-paint`, `29-stained-glass-mosaic`, `30-swiss-minimal-3d`.
  - Mengikuti standar estetika Notepad++ Classic secara ketat: ikon merupakan elemen grafis murni terisolasi tanpa kotak latar belakang gelap, bayangan lantai artifisial, halo abu-abu, atau efek timbul kasar.
  - Tampil mengambang bersih di atas bilah alat gelap permanen (`#21252b`) dengan margin nafas 18% untuk mencegah pemotongan sudut pada layar Retina.

### 2. Ergonomi Menu Konteks Editor & Aksi Produktivitas

- **Penyempurnaan Alur Kerja Klik Kanan**:
  - Menambahkan menu **Tempel & Sesuaikan Gaya (Paste & Match Style)** (`pasteAndMatchStyle`, `Cmd+Alt+Shift+V` pada macOS, `Ctrl+Shift+V` pada Linux/BSD) untuk menempel teks bersih tanpa format eksternal dan menyesuaikan indentasi editor secara otomatis.
  - Menambahkan menu **Cari di Google (Search on Google)** (`searchOnGoogle`, `Cmd+Alt+G` pada macOS, `Ctrl+Alt+G` pada Linux/BSD) untuk membuka peramban web bawaan dan mencari teks yang disorot atau kata di bawah kursor secara seketika.
  - Menyederhanakan konversi huruf menjadi **HURUF BESAR (UPPERCASE)** dan **huruf kecil (lowercase)** langsung di menu konteks utama tanpa submenu bersarang bertingkat.
  - Menghapus opsi Run dan Bash encode/decode dari menu klik kanan guna mengeliminasi redundansi dan fokus pada kenyamanan penyuntingan.

### 3. Opsi Preferensi Umum Baru

- **Kontrol Granular pada Menu Preferensi > Umum**:
  - Menambahkan pengaturan aktif/nonaktif Deteksi Encoding Otomatis (`enableEncodingAutoDetection`).
  - Menambahkan pengaturan aktif/nonaktif Dialog Konfirmasi Simpan Semua (`enableSaveAllConfirmation`).
  - Seluruh setelan tersimpan secara persisten pada `ApplicationSettings` lintas sesi aplikasi.

### 4. Perombakan Penataan Sintaksis & Keamanan Kontras WCAG 2.1

- **Ergonomi Visual Berbasis Komputasi Matematika**:
  - Mengintegrasikan kalkulasi luminansi relatif dan rasio kontras WCAG 2.1 (`0.2126*R + 0.7152*G + 0.0722*B`), menjamin rasio kontras minimal 4.0:1 pada teks utama di seluruh 70+ tema warna.
  - Penataan gaya semantik khusus keluarga bahasa untuk C/C++, Python, Web/HTML/JS/CSS, Markdown progressive heading H1-H6, SQL, Shell script, dan YAML.
  - Lencana Emas Berkontras Tinggi (`#FFD700`, tebal) untuk penanda komentar khusus (TODO, FIXME, BUG, HACK, NOTE).
  - Warna amber hangat untuk urutan karakter escape teks (`\n`, `\t`, `\xHH`).
  - Indikator kotak bersudut tumpul untuk penanda kemunculan kata SmartHighlighter yang tersinkronisasi dinamis dengan tema aktif.

### 5. Standar Paket DMG Ringan macOS 30MB–40MB

- **Integritas Pengemasan Bebas Bloatware & Penguatan Qt 6.2**:
  - Mempertahankan integritas biner universal resmi Qt dan plugin rendering inti (`libqsvg`, `libqsvgicon`, `libqgif`, `libqico`), menghasilkan berkas rilis DMG stabil berukuran 36MB–39MB lintas Qt 6.2–6.12.
  - Berada ketat di dalam batas rentang standar 30MB–40MB (tanpa modul bloatware yang tidak diperlukan seperti `QtPdf`, `QtQml`, `QtQuick`, atau `QtVirtualKeyboard`).
  - Mengimplementasikan `scripts/deploy-mac-bundle.sh` dengan normalisasi header `otool` multi-arsitektur, penegakan `LC_RPATH` `@executable_path/../Frameworks`, stripping biner utama saja, serta verifikasi launch CLI `--version` headless.
  - Dikemas menggunakan `hdiutil` native dengan mempertahankan preset tata letak `.DS_Store` (680x440), grafis latar belakang Retina (`dmg-background@2x.png`), ikon volume kustom (`.VolumeIcon.icns`), dan verifikasi CRC32 otomatis.

### 6. Arsitektur Pewarnaan Tab Mode Split Modern & Harmonisasi Tema

- **Pembersihan Gradien Gelap & Garis Oranye Kuno**:
  - Menghilangkan injeksi stylesheet internal bergradien metalik gelap bawaan ADS dengan menetapkan `ads::CDockManager::setConfigFlag(ads::CDockManager::DisableStylesheet, true)`.
  - Mengganti garis batas oranye dan warna peach (`rgb(255, 202, 176)`) dengan warna aksen tema yang dikalkulasi secara dinamis sesuai palet.
- **Hierarki Visual Ganda (Dual-State) Adaptif Tema**:
  - **Tab Aktif pada Panel Split Terfokus**: Latar belakang kartu editor (`%5`), garis aksen tema atas (`2px solid %6`), teks primer tebal (`%2`, weight 600), dan menyatu mulus ke kanvas editor.
  - **Tab Aktif pada Panel Split Tidak Terfokus**: Latar belakang kartu editor (`%5`), garis indikator halus (`2px solid %12`), dan teks primer terbaca (`%2`, weight 500), menjamin 100% bebas bentrok tema pada seluruh palet terang, gelap, dan kustom.
  - **Tab Tidak Aktif**: Latar belakang wadah harmonis (`%1`), teks sekunder (`%4`), serta efek hover halus (`%3`, teks `%2`).
  - **Bilah Pemisah Splitter (`ads--CDockSplitter::handle`)**: Garis batas tema 1px (`%3`) dengan iluminasi aksen interaktif (`%6`) saat disentuh kursor.
  - **Tombol Tutup Tab & Tombol Tambah Tab**: Ikonografi SVG adaptif tema dengan efek sentuh koral lembut (`rgba(235, 87, 87, 0.25)`).

### 7. Penyempurnaan Auto-Completion, Harmonisasi Tema & Optimasi Sumber Daya

- **Penyempurnaan Auto-Completion & Default Non-Intrusif**:
  - Auto-completion dinonaktifkan secara default (`autoCompletion = false`) di `ApplicationSettings.cpp` untuk kenyamanan penulisan kode tanpa gangguan popup otomatis.
  - Menghapus pemicu auto-fill agresif (`autoCSetFillUps("")`), mencegah penyisipan kandidat saat menekan spasi atau kurung buka.
  - Menetapkan batas trigger minimal 3 karakter dengan perlindungan dokumen besar (>5MB).
- **Harmonisasi Nama Tema & Brand SparkNotepad**:
  - Mengubah nama palet default terang menjadi `sparkNotepad` ("SparkNotepad Default") dengan pemetaan kompatibilitas mundur untuk setelan `notepadNext`.
  - Memperbarui penamaan paket ikon toolbar dan aksi reset tema di antarmuka.
- **Perbaikan Siklus Hidup Live Preview Markdown & HTML**:
  - Menyelaraskan visibilitas dock preview dengan editor aktif di `MainWindow`: menutup tab atau berpindah ke berkas non-Markdown/HTML otomatis menyembunyikan dock preview jika auto-show aktif.
  - Otomatis menyembunyikan dock preview saat seluruh berkas ditutup (`editor == nullptr`).
- **Optimasi Performa Rendering & Memori**:
  - Menghapus ribuan kueri Scintilla IPC berulang di `HighlightedScrollBar::paintEvent` dengan pra-kalkulasi geometri di luar loop rendering tick marks.
  - Mengganti alokasi regex pemecah kata di `MarkdownSafePreviewDock` dengan pencacah linier nol alokasi heap.
  - Menambahkan deteksi cepat di `MarkdownPreviewEnhancer` untuk melompati pipeline regex markdown pada berkas teks polos.

### 8. Rangkaian Kualitas Kode & Verifikasi 14 Pilar

SparkNotePad v1.4.6 lolos seluruh 14 Pilar Rekayasa Perangkat Lunak:

- **Nol Peringatan Linter**: Hasil verifikasi bersih pada `clangd --check`, `cppcheck` (171 berkas di `src/`), `clang-tidy`, SonarLint (`sonarsource.sonarlint-vscode`), dan Trunk linter.
- **Kompilasi Standar CMake Unix Makefiles**: Kompilasi ditegakkan menggunakan generator default Unix Makefiles dengan pembatasan konkurensi (Ninja dilarang permanen per `AGENTS.md`).
- **100% Kelulusan Unit Test**: Seluruh pengujian unit pada `SparkNotePadUnitTests` lulus dengan 0 kegagalan.
- **100% Paritas Dwibahasa**: 2.085 entri terjemahan tersinkronisasi dengan 0 entri tertunda pada katalog Bahasa Inggris dan Bahasa Indonesia.

### 9. Engine Pembaruan Multi-Build updates.json & Force-Update Komprehensif

- **Cakupan Matriks Lengkap updates.json**:
  - Memperbarui `updates.json` ke versi 1.4.6 mencakup seluruh matriks rilis: macOS Apple Silicon (arm64) dan Intel (x86_64) lintas Qt 6.2–6.12, paket Linux Debian/Ubuntu (.deb) dan RedHat/Fedora (.rpm) untuk arsitektur x86_64 dan aarch64, serta skrip helper POSIX.
  - Mengaktifkan pembaruan wajib (force update) untuk semua versi sebelumnya guna menjamin keamanan, performa, dan keandalan penuh tanpa regresi.
- **Engine ReleaseManager 3-Tingkat yang Tangguh**:
  - Tier 1: Unduhan langsung `updates.json` melalui CDN raw GitHub tanpa batas kuota API dengan deteksi arsitektur CPU native (`QSysInfo::currentCpuArchitecture()`).
  - Tier 2: GitHub REST API resmi dengan ETag caching (HTTP 304) untuk efisiensi kuota.
  - Tier 3: Pengalihan lokasi HTTP Location header fallback.
- **Penyempurnaan Tampilan UpdateDialog**:
  - Menambahkan lencana `WAJIB` (MANDATORY UPDATE) merah mencolok dan deskripsi pembaruan wajib.
  - Tombol unduh langsung mengarahkan ke berkas paket biner yang cocok dengan sistem operasi dan arsitektur CPU pengguna.

### 10. Penyempurnaan Total Engine & Live Preview Markdown

- **Pemrosesan Markdown Kelas Industri**:
  - Riset mendalam dan integrasi praktik terbaik dari markedjs, markdown-it, stackedit, editor.md, milkdown, dan GitHub Flavored Markdown (GFM).
  - Konsolidasi dan deduplikasi mesin render callout ke `MarkdownPreviewEnhancer::enhanceCallouts`, menyingkirkan kode ganda lintas dock dan tab.
  - Implementasi Admonition / Callout resmi GitHub (`[!NOTE]`, `[!TIP]`, `[!IMPORTANT]`, `[!WARNING]`, `[!CAUTION]`) dengan ikon visual kontras tinggi, latar bernuansa lembut, dan batas aksen warna.
  - Penambahan generator Daftar Isi interaktif (`[TOC]` / `{:toc}`) berjenjang dengan penautan otomatis ke slug heading dokumen.
  - Penambahan slug anchor otomatis pada tag heading (`<h1 id="...">`) untuk navigasi loncat dan tautan internal.
  - Penambahan konversi emoji shortcode (seperti `:rocket:`, `:sparkles:`, `:white_check_mark:`, `:fire:`, `:star:`, `:memo:`, `:100:`, dll.) dengan proteksi blok kode.
  - Penambahan chip pintasan keyboard `[[Key]]` -> `<kbd>Key</kbd>`.
  - Penyelarasan fungsi `copyHtml()`, `openInBrowser()`, dan `DocumentExportManager::markdownToHtml` agar hasil ekspor dan salinan HTML memuat seluruh rumus math, diagram mermaid, daftar tugas (task lists), catatan kaki (footnotes), dan callout secara utuh.
  - Perbaikan database kompilasi (`compile_commands.json`) dengan sinkronisasi 685 header, menuntaskan 945+ pesan error IDE linter dan peringatan SonarLint.

---

## 📦 Berkas Biner & Aset Rilis Resmi

| Nama Aset                                     | Platform Target / Deskripsi                                          | Arsitektur    | Format                    |
| :-------------------------------------------- | :------------------------------------------------------------------- | :------------ | :------------------------ |
| **`SparkNotePad-v1.4.6-x64.dmg`**             | Rilis Kanonikal macOS (macOS 11.0+, Qt 6.5 LTS)                      | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-arm64.dmg`**           | Rilis Kanonikal macOS (macOS 11.0+, Qt 6.5 LTS)                      | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.2-x64.dmg`**       | Rilis macOS Catalina Legacy (macOS 10.15+, Qt 6.2 LTS)               | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.2-arm64.dmg`**     | Rilis Legacy macOS (macOS 11.0+, Qt 6.2 LTS)                         | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.5-x64.dmg`**       | Rilis Standar macOS (macOS 11.0+, Qt 6.5 LTS)                        | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.5-arm64.dmg`**     | Rilis Standar macOS (macOS 11.0+, Qt 6.5 LTS)                        | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.8-x64.dmg`**       | Rilis macOS (macOS 12.0+, Qt 6.8 LTS)                                | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.8-arm64.dmg`**     | Rilis macOS (macOS 12.0+, Qt 6.8 LTS)                                | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.10-x64.dmg`**      | Rilis macOS (macOS 13.0+, Qt 6.10)                                   | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.10-arm64.dmg`**    | Rilis macOS (macOS 14.0+, Qt 6.10)                                   | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.11-x64.dmg`**      | Rilis macOS (macOS 13.0+, Qt 6.11)                                   | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.11-arm64.dmg`**    | Rilis macOS (macOS 14.0+, Qt 6.11)                                   | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.12-x64.dmg`**      | Rilis macOS Mutakhir (macOS 13.0+, Qt 6.12)                          | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.12-arm64.dmg`**    | Rilis macOS Mutakhir (macOS 14.0+, Qt 6.12)                          | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`sparknotepad_1.4.6_amd64.deb`**            | Linux Debian / Ubuntu Kanonikal (`glibc 2.35+`, Qt 6.5 LTS)          | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.2_amd64.deb`**      | Linux Ubuntu 20.04 LTS Legacy (`glibc 2.31+`, Qt 6.2 LTS)            | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.5_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.5 LTS)                    | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.8_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.8 LTS)                    | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.8_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.8 LTS)              | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.10_amd64.deb`**     | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.10)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.10_arm64.deb`**     | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.10)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.11_amd64.deb`**     | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.11)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.11_arm64.deb`**     | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.11)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.12_amd64.deb`**     | Linux Debian / Ubuntu (`glibc 2.39+`, Qt 6.12)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.12_arm64.deb`**     | Linux Debian / Ubuntu ARM64 (`glibc 2.39+`, Qt 6.12)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad-1.4.6-1.x86_64.rpm`**         | Linux Fedora / RHEL / openSUSE Kanonikal (`glibc 2.35+`, Qt 6.5 LTS) | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.2-1.x86_64.rpm`**   | Linux CentOS / RHEL Legacy (`glibc 2.31+`, Qt 6.2 LTS)               | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.5-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.5 LTS)                      | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.8-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.8 LTS)                      | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.8-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.8 LTS)                | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.10-1.x86_64.rpm`**  | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.10)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.10-1.aarch64.rpm`** | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.10)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.11-1.x86_64.rpm`**  | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.11)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.11-1.aarch64.rpm`** | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.11)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.12-1.x86_64.rpm`**  | Linux Fedora / RHEL (`glibc 2.39+`, Qt 6.12)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.12-1.aarch64.rpm`** | Linux Fedora / RHEL ARM64 (`glibc 2.39+`, Qt 6.12)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`SparkNotePad-Default-Editor.sh`**          | Skrip Pembantu Asosiasi Berkas POSIX macOS & Linux                   | Universal     | Skrip Shell Bash          |
| **`SparkNotePad-Default-Editor-Linux.sh`**    | Skrip Mesin Editor Default Linux 5 Lapisan                           | Linux         | Skrip Shell Bash          |

---

## 📜 Riwayat Perubahan Lengkap

- <https://github.com/alsyundawy/SparkNotePad/compare/v1.4.5...v1.4.6>
