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

- **Integritas Pengemasan Bebas Bloatware**:
  - Mempertahankan integritas biner universal resmi Qt dan modul format gambar standar (`libqjpeg`, `libqgif`, `libqwebp`), menghasilkan berkas rilis DMG stabil berukuran 38MB–39MB.
  - Berada ketat di dalam batas rentang standar 30MB–40MB (tanpa modul bloatware yang tidak diperlukan seperti `QtPdf`, `QtQml`, `QtQuick`, atau `QtVirtualKeyboard`).
  - Dikemas menggunakan `hdiutil` native dengan mempertahankan preset tata letak `.DS_Store` (680x440), grafis latar belakang Retina (`dmg-background@2x.png`), ikon volume kustom (`.VolumeIcon.icns`), dan verifikasi CRC32 otomatis.

### 6. Rangkaian Kualitas Kode & Verifikasi 14 Pilar

SparkNotePad v1.4.6 lolos seluruh 14 Pilar Rekayasa Perangkat Lunak:

- **Nol Peringatan Linter**: Hasil verifikasi bersih pada `clangd --check`, `cppcheck` (171 berkas di `src/`), `clang-tidy`, SonarLint (`sonarsource.sonarlint-vscode`), dan Trunk linter.
- **Kompilasi Standar CMake Unix Makefiles**: Kompilasi ditegakkan menggunakan generator default Unix Makefiles dengan pembatasan konkurensi (Ninja dilarang permanen per `AGENTS.md`).
- **100% Kelulusan Unit Test**: Seluruh pengujian unit pada `SparkNotePadUnitTests` lulus dengan 0 kegagalan.
- **100% Paritas Dwibahasa**: 1.850 entri terjemahan tersinkronisasi dengan 0 entri tertunda pada katalog Bahasa Inggris dan Bahasa Indonesia.

---

## 📦 Berkas Biner & Aset Rilis Resmi

| Nama Aset                                      | Platform Target / Deskripsi                                          | Arsitektur    | Format                    |
| :--------------------------------------------- | :------------------------------------------------------------------- | :------------ | :------------------------ |
| **`SparkNotePad-v1.4.6-x64.dmg`**              | Rilis Kanonikal macOS (macOS 11.0+, Qt 6.5 LTS)                      | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-arm64.dmg`**            | Rilis Kanonikal macOS (macOS 11.0+, Qt 6.5 LTS)                      | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.2-x64.dmg`**        | Rilis macOS Catalina Legacy (macOS 10.15+, Qt 6.2 LTS)               | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.2-arm64.dmg`**      | Rilis Legacy macOS (macOS 11.0+, Qt 6.2 LTS)                         | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.5-x64.dmg`**        | Rilis Standar macOS (macOS 11.0+, Qt 6.5 LTS)                        | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.5-arm64.dmg`**      | Rilis Standar macOS (macOS 11.0+, Qt 6.5 LTS)                        | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.8-x64.dmg`**        | Rilis macOS (macOS 12.0+, Qt 6.8 LTS)                                | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.8-arm64.dmg`**      | Rilis macOS (macOS 12.0+, Qt 6.8 LTS)                                | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.10-x64.dmg`**       | Rilis macOS (macOS 13.0+, Qt 6.10)                                   | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.10-arm64.dmg`**     | Rilis macOS (macOS 14.0+, Qt 6.10)                                   | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.11-x64.dmg`**       | Rilis macOS (macOS 13.0+, Qt 6.11)                                   | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.11-arm64.dmg`**     | Rilis macOS (macOS 14.0+, Qt 6.11)                                   | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.12-x64.dmg`**       | Rilis macOS Mutakhir (macOS 13.0+, Qt 6.12)                          | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.6-qt6.12-arm64.dmg`**     | Rilis macOS Mutakhir (macOS 14.0+, Qt 6.12)                          | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`sparknotepad_1.4.6_amd64.deb`**             | Linux Debian / Ubuntu Kanonikal (`glibc 2.35+`, Qt 6.5 LTS)          | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.2_amd64.deb`**       | Linux Ubuntu 20.04 LTS Legacy (`glibc 2.31+`, Qt 6.2 LTS)            | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.5_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.5 LTS)                    | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.8_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.8 LTS)                    | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.8_arm64.deb`**       | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.8 LTS)              | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.10_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.10)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.10_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.10)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.11_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.11)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.11_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.11)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.12_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.39+`, Qt 6.12)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.6-qt6.12_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.39+`, Qt 6.12)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad-1.4.6-1.x86_64.rpm`**          | Linux Fedora / RHEL / openSUSE Kanonikal (`glibc 2.35+`, Qt 6.5 LTS) | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.2-1.x86_64.rpm`**    | Linux CentOS / RHEL Legacy (`glibc 2.31+`, Qt 6.2 LTS)               | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.5-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.5 LTS)                      | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.8-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.8 LTS)                      | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.8-1.aarch64.rpm`**   | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.8 LTS)                | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.10-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.10)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.10-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.10)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.11-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.11)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.11-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.11)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.12-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.39+`, Qt 6.12)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.6-qt6.12-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.39+`, Qt 6.12)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`SparkNotePad-Default-Editor.sh`**           | Skrip Pembantu Asosiasi Berkas POSIX macOS & Linux                   | Universal     | Skrip Shell Bash          |
| **`SparkNotePad-Default-Editor-Linux.sh`**     | Skrip Mesin Editor Default Linux 5 Lapisan                           | Linux         | Skrip Shell Bash          |

---

## 📜 Riwayat Perubahan Lengkap

- <https://github.com/alsyundawy/SparkNotePad/compare/v1.4.5...v1.4.6>
