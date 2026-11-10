# Catatan Rilis SparkNotePad v1.4.0 — Spasi Baris Ekstra, Tooltip Tab Diperkaya, Pencarian Fuzzy Aman, Eksekusi Batch Makro, MCP Server & Mitigasi CVE 2025–2026

**Versi Rilis:** `v1.4.0`<br/>
**Tanggal Rilis:** 2026-11-07<br/>
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
> 🍏 **[`Rilis Terkini (v1.4.0)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
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

SparkNotePad **v1.4.0** menghadirkan peningkatan produktivitas penting yang terinspirasi dari riset mendalam terhadap Nextpad++, Notepad--, dan editor modern lainnya, dipadukan dengan mitigasi kerentanan keamanan 2025–2026, konektivitas Model Context Protocol (MCP) loopback lokal, serta rekayasa kualitas kode 14 pilar menyeluruh. Rilis ini memperkenalkan pengaturan spasi baris ekstra yang dapat dikonfigurasi pengguna (extra ascent dan extra descent), inspeksi metadata dokumen lengkap pada tooltip tab, mesin pencarian subsekuens fuzzy tanpa backtracking kebal ReDoS, eksekusi makro batch di semua dokumen terbuka, fungsionalitas server MCP JSON-RPC loopback lokal, serta pengetatan keamanan sistem dari celah CVE-2026-20841, CVE-2026-16762, dan CVE-2026-48778.

---

## 🌟 Sorotan & Inovasi Utama di v1.4.0

### 1. Pengaturan Spasi Baris Ekstra (Ascent & Descent)

- **Kendali Tipografi Tinggi Baris Presisi**:
  - Mengintegrasikan API Scintilla `SCI_SETEXTRAASCENT` dan `SCI_SETEXTRADESCENT` pada `src/ScintillaEdit.cpp`.
  - Menambahkan kontrol numerik intuitif (rentang 0–30 px) pada menu **Preferensi > Margin/Padding** (`PreferencesDialog`).
  - Memberikan ruang napas visual yang lebih lega bagi font pemrograman berkerapatan tinggi (JetBrains Mono, Fira Code, Source Code Pro, Cascadia Code) pada layar Retina dan monitor resolusi tinggi (HiDPI).
  - Pengaturan diterapkan secara instan dan real-time ke semua tab dokumen yang sedang aktif tanpa perlu memulai ulang aplikasi.

### 2. Tooltip Tab Diperkaya dengan Inspeksi Metadata Dokumen

- **Forensik Dokumen Instan Melalui Hover Tab**:
  - Pada `src/DockedEditor.cpp`, merombak generator tooltip tab untuk menyajikan informasi berkas secara komprehensif:
    - Path absolut kanonikal berkas pada sistem penyimpanan disk.
    - Ukuran berkas terformat otomatis (Byte, KB, MB).
    - Total jumlah baris dan total panjang karakter teks.
    - Encoding karakter aktif (misalnya UTF-8, UTF-16, ISO-8859-1, Windows-1252).
    - Format akhir baris / EOL (CRLF Windows, LF Unix/macOS, CR Mac Klasik).
    - Timestamp waktu modifikasi terakhir yang diambil langsung dari sistem berkas.
  - Menghilangkan keharusan membuka jendela properti berkas terpisah saat menavigasi banyak dokumen.

### 3. Mesin Pencarian Subsekuens Fuzzy Tanpa Backtracking & Penguatan ReDoS

- **Pencocokan Pola Fuzzy Waktu Linier**:
  - Merekayasa `RegexSafetyEngine` untuk memproduksi ekspresi reguler linier deterministik pada pencarian subsekuens fuzzy tanpa risiko backtracking eksponensial.
  - Sepenuhnya kebal terhadap celah kerentanan Regular Expression Denial of Service (ReDoS / CWE-1333) saat menelusuri berkas log raksasa atau kode sumber berukuran megabyte.
  - Mengintegrasikan opsi interaktif "Fuzzy Search" langsung ke dalam Dialog Cari & Ganti (`src/dialogs/FindReplaceDialog.cpp`) serta Bilah Pencarian Cepat (`src/widgets/QuickFindWidget.cpp`).

### 4. Eksekusi Makro Batch Lintas Dokumen Terbuka

- **Otomasi Pemrosesan Teks Skala Proyek**:
  - Memperluas dialog `MacroRunDialog` dengan selektor cakupan eksekusi: "Current Document" atau "All Open Documents".
  - Arsitektur status atomik: mencatat checkpoint undo per instance editor guna memastikan pemulihan aman jika diperlukan.
  - Menyediakan visualisasi proses progres saat melakukan operasi batch berat lintas puluhan dokumen terbuka.

### 5. Server MCP Loopback Lokal Berintegritas Tinggi

- **Konektivitas Alur Kerja Asisten AI**:
  - Mengimplementasikan server Model Context Protocol (MCP) tersemat yang beroperasi secara eksklusif pada interface loopback lokal `127.0.0.1`.
  - Beroperasi di atas standar JSON-RPC 2.0 dengan pengamanan otentikasi Bearer Token kriptografis (`mcp_token`).
  - Menyediakan endpoint aman berizin (`list_open_tabs`, `read_active_tab`, `get_editor_metadata`) yang memungkinkan agen AI dan otomasi CLI berinteraksi dengan editor secara aman.
  - Sandbox ketat: koneksi dari antarmuka jaringan eksternal (`0.0.0.0`) ditolak secara permanen pada lapisan binding socket.

### 6. Pengerasan Keamanan Menyeluruh Mitigasi CVE 2025–2026

- **Pemberantasan Kerentanan Multi-Vektor**:
  - **CVE-2026-20841 (Intersepsi Skema URI/URL Berbahaya)**: Penegakan filter skema URI yang ketat pada `NetworkSecurityPolicy`, mencegah eksekusi biner liar atau eksploitasi protokol handler melalui teks tertaut.
  - **CVE-2026-16762 (Pembatas Kedalaman Nesting XML/HTML)**: Penegakan batas kedalaman rekursi maksimal 32 tingkat dan batas payload 10 MB pada `ConfigIntegrityGuard`, memitigasi serangan XML Entity Expansion (Billion Laughs / CWE-776).
  - **CVE-2026-48778 & CVE-2026-48800 (Pencegahan Eskalasi Hak Istimewa & Izin Berkas)**: Penegakan izin berkas POSIX `0600` yang ketat pada seluruh berkas konfigurasi, status sesi, cadangan autosave, dan cache sementara.

### 7. Salin Info Diagnostik Debug Satu Klik

- **Pelaporan Bug dan Forensik Sistem Terpadu**:
  - Menambahkan menu **Bantuan > Debug Info** pada `src/dialogs/MainWindow.cpp`.
  - Mengagregasi informasi lingkungan runtime secara seketika: versi SparkNotePad, Scintilla 5.6.9, Lexilla 5.5.5, Lua 5.5.1, versi Qt, rilis OS, arsitektur CPU, geometri layar, dan tema aktif.
  - Mengonversi payload menjadi format GitHub Flavored Markdown dan menyalinnya ke clipboard sistem untuk pelaporan issue di GitHub.

### 8. Rangkaian Kualitas Kode & Verifikasi 14 Pilar

SparkNotePad v1.4.0 mematuhi standar 14 Pilar Rekayasa Perangkat Lunak:

- **Nol Peringatan Linter**: Terverifikasi bersih 100% pada `clangd --check`, `cppcheck` (152 berkas di `src/`), `clang-tidy`, SonarLint (`sonarsource.sonarlint-vscode`), dan Trunk linter.
- **Konkurensi Build Terkendali**: Kompilasi default CMake Unix Makefiles ditegakkan ketat dengan konkurensi terkontrol `-j2` (generator Ninja dilarang permanen per aturan `AGENTS.md`).
- **100% Kelulusan Unit Test**: Cakupan pengujian menyeluruh lulus pada seluruh rangkaian test di `SparkNotePadTests`.
- **Paritas Python**: Paritas 100% pada lingkungan runtime Python 3.14 dan 3.15.

---

## 📦 Berkas Biner & Aset Rilis Resmi

| Nama Aset                                      | Platform Target / Deskripsi                                          | Arsitektur    | Format                    |
| :--------------------------------------------- | :------------------------------------------------------------------- | :------------ | :------------------------ |
| **`SparkNotePad-v1.4.0-x64.dmg`**              | Rilis Kanonikal macOS (macOS 11.0+, Qt 6.5 LTS)                      | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-arm64.dmg`**            | Rilis Kanonikal macOS (macOS 11.0+, Qt 6.5 LTS)                      | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.2-x64.dmg`**        | Rilis macOS Catalina Legacy (macOS 10.15+, Qt 6.2 LTS)               | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.2-arm64.dmg`**      | Rilis Legacy macOS (macOS 11.0+, Qt 6.2 LTS)                         | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.5-x64.dmg`**        | Rilis Standar macOS (macOS 11.0+, Qt 6.5 LTS)                        | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.5-arm64.dmg`**      | Rilis Standar macOS (macOS 11.0+, Qt 6.5 LTS)                        | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.8-x64.dmg`**        | Rilis macOS (macOS 12.0+, Qt 6.8 LTS)                                | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.8-arm64.dmg`**      | Rilis macOS (macOS 12.0+, Qt 6.8 LTS)                                | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.10-x64.dmg`**       | Rilis macOS (macOS 13.0+, Qt 6.10)                                   | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.10-arm64.dmg`**     | Rilis macOS (macOS 14.0+, Qt 6.10)                                   | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.11-x64.dmg`**       | Rilis macOS (macOS 13.0+, Qt 6.11)                                   | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.11-arm64.dmg`**     | Rilis macOS (macOS 14.0+, Qt 6.11)                                   | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.12-x64.dmg`**       | Rilis macOS Mutakhir (macOS 13.0+, Qt 6.12)                          | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.0-qt6.12-arm64.dmg`**     | Rilis macOS Mutakhir (macOS 14.0+, Qt 6.12)                          | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`sparknotepad_1.4.0_amd64.deb`**             | Linux Debian / Ubuntu Kanonikal (`glibc 2.35+`, Qt 6.5 LTS)          | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.2_amd64.deb`**       | Linux Ubuntu 20.04 LTS Legacy (`glibc 2.31+`, Qt 6.2 LTS)            | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.5_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.5 LTS)                    | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.8_amd64.deb`**       | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.8 LTS)                    | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.8_arm64.deb`**       | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.8 LTS)              | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.10_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.10)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.10_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.10)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.11_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.11)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.11_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.11)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.12_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.39+`, Qt 6.12)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.0-qt6.12_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.39+`, Qt 6.12)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad-1.4.0-1.x86_64.rpm`**          | Linux Fedora / RHEL / openSUSE Kanonikal (`glibc 2.35+`, Qt 6.5 LTS) | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.2-1.x86_64.rpm`**    | Linux CentOS / RHEL Legacy (`glibc 2.31+`, Qt 6.2 LTS)               | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.5-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.5 LTS)                      | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.8-1.x86_64.rpm`**    | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.8 LTS)                      | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.8-1.aarch64.rpm`**   | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.8 LTS)                | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.10-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.10)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.10-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.10)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.11-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.11)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.11-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.11)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.12-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.39+`, Qt 6.12)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.0-qt6.12-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.39+`, Qt 6.12)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`SparkNotePad-Default-Editor.sh`**           | Skrip Pembantu Asosiasi Berkas POSIX macOS & Linux                   | Universal     | Skrip Shell Bash          |
| **`SparkNotePad-Default-Editor-Linux.sh`**     | Skrip Mesin Editor Default Linux 5 Lapisan                           | Linux         | Skrip Shell Bash          |

---

## 📜 Riwayat Perubahan Lengkap

- <https://github.com/alsyundawy/SparkNotePad/compare/v1.3.9...v1.4.0>
