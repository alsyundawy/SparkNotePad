# Catatan Rilis SparkNotePad v1.4.5 — Tumpukan Tab Tertutup, Tag Warna Tab, Pemulihan Crash, Pencarian Lanjutan, Ekspor PDF, Pratinjau Mermaid/LaTeX, Paket Ikon Aksara (28 Koleksi) & Penguatan 14 Pilar

**Versi Rilis:** `v1.4.5`<br/>
**Tanggal Rilis:** 2026-11-08<br/>
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
> 🍏 **[`Rilis Terkini (v1.4.5)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
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

SparkNotePad **v1.4.5** merupakan rilis arsitektur fundamental yang menghadirkan 17 pilar rekayasa perangkat lunak untuk pengembang profesional, arsitek sistem, dan pengguna tingkat lanjut. Rilis ini memperkenalkan tumpukan pemulihan tab tertutup yang tangguh (`Ctrl+Shift+T` / `Cmd+Shift+T`), penandaan dan pengelompokan warna tab yang mudah diakses, manajer autosave dan pemulihan crash dengan izin ketat POSIX `0600`, mesin karakter escape pencarian lanjutan, 129 lexer bahasa pemrograman modern, alur ekspor dan pencetakan native Markdown/HTML/PDF, 30 tema CSS pratinjau Markdown dengan diagram Mermaid dan rumus LaTeX, utilitas pengembang, pengalih header/source instan, penegakan `.editorconfig` otomatis, dock riwayat clipboard dan inspeksi karakter Unicode, pengalih tab cepat fuzzy, 28 koleksi ikon toolbar Aksara, penguncian permanen latar belakang bilah alat gelap, serta standardisasi pengemasan berkas macOS DMG ringan 30MB–34MB.

---

## 🌟 Sorotan & Inovasi Utama di v1.4.5

### 1. Tumpukan Tab Tertutup & Buka Kembali Tab (`Ctrl+Shift+T` / `Cmd+Shift+T`)

- **Pemulihan Status Multi-Atribut yang Tangguh**:
  - Mengimplementasikan `ClosedTabsManager` yang mengelola tumpukan FIFO 50 berkas yang baru saja ditutup.
  - Merekam dan memulihkan secara utuh path berkas, offset posisi kursor, rasio gulir baris awal, encoding karakter, mode EOL, ID penataan sintaksis bahasa, dan tag warna tab.
  - Dapat diakses melalui kombinasi tombol standar (`Cmd+Shift+T` pada macOS, `Ctrl+Shift+T` pada Linux/BSD), menu Berkas, dan menu konteks bilah tab.

### 2. Penandaan & Pengelompokan Warna Tab (8 Preset Aksesibilitas)

- **Organisasi Visual untuk Ruang Kerja Kompleks**:
  - Merekayasa `TabColorManager` yang menyediakan 8 preset warna kontras tinggi yang ramah standar aksesibilitas WCAG: Merah, Oranye, Kuning, Hijau, Sian, Biru, Ungu, dan Magenta.
  - Warna tab tersimpan secara persisten lintas restart aplikasi dan sesi ruang kerja melalui `ApplicationSettings`.
  - Integrasi menu klik kanan pada header tab memudahkan penetapan atau pembersihan warna secara instan.

### 3. Manajer Autosave & Pemulihan Crash (Interval 10 Detik, Izin POSIX 0600)

- **Arsitektur Perlindungan Data Tanpa Celah**:
  - Mengimplementasikan `AutosaveRecoveryManager` yang mengeksekusi proses snapshot latar belakang non-blocking setiap 10 detik untuk dokumen yang dimodifikasi.
  - Menegakkan izin berkas ketat POSIX `0600` (`S_IRUSR | S_IWUSR`) pada penyimpanan disk, melindungi kerahasiaan dokumen dari akses multi-pengguna.
  - Mengintegrasikan checksum integritas SHA-256 untuk mendeteksi korupsi berkas pada disk.
  - Dialog interaktif `CrashRecoveryDialog` muncul otomatis saat aplikasi dimulai ulang setelah crash sistem atau pemadaman daya listrik, menyediakan opsi pemulihan atau pembuangan berkas secara granular.

### 4. Mesin Karakter Escape Pencarian Lanjutan (Extended Search)

- **Paritas Penuh dengan Karakter Regex & Byte Heksadesimal**:
  - Mengembangkan `ExtendedSearchParser` yang memproses byte heksadesimal (`\xHH`), urutan byte desimal (`\dDDD`), byte NUL (`\0`), backspace (`\b`), form-feed (`\f`), dan tab vertikal (`\v`).
  - Beroperasi penuh pada Dialog Cari & Ganti (`FindReplaceDialog`), Ganti Semua, dan Bilah Pencarian Cepat (`QuickFindWidget`) dalam mode Extended Search.

### 5. Audit 100+ Lexer & Ekspansi Bahasa Modern (Total 129 Bahasa)

- **Dukungan Bahasa Pemrograman Terkini & Rekayasa Sistem**:
  - Menambahkan lexer native baru: Crystal (`.cr`), Cangjie (`.cj`), LLVM IR (`.ll`), dan JSON5 (`.json5`).
  - Memodernisasi kata kunci bahasa standar: C23 (`_BitInt`, `constexpr`, `nullptr`), Rust 2024 (`gen`, `async gen`), Swift 6 (`consuming`, `borrowing`, `sending`), dan TypeScript 5.8.
  - Memverifikasi ulang seluruh 129 berkas konfigurasi bahasa pada direktori `src/languages/`.

### 6. Alur Ekspor Native Markdown / HTML / PDF

- **Penerbitan Dokumen Presisi Tinggi**:
  - Mengimplementasikan `DocumentExportManager` menggunakan `QPrinter` / `Qt6::PrintSupport` native Qt 6 dipadukan dengan mesin Pandoc tanpa antarmuka.
  - Dialog interaktif `ExportOptionsDialog` memungkinkan pengguna mengonfigurasi ukuran kertas (A4, Letter, Legal), orientasi halaman, margin kustom, teks header/footer, dan styling CSS tersemat.

### 7. Peningkatan Pratinjau Markdown (30 Tema, Diagram Mermaid & Rumus LaTeX)

- **Pratinjau Dokumen Visual yang Kaya dan Responsif**:
  - Mengintegrasikan 30 tema CSS classless (`GITHUB`, `GITHUB-DARK`, `ALMOND`, `AWSM`, `AXIST`, `BAMBOO`, `BULLFRAME`, `HOLIDAY`, `KACIT`, `LATEX`, `MARX`, `MINI`, `MODEST`, `NEW`, `NO-CLASS`, `PICO`, `RETRO`, `SAKURA`, `SAKURA-VADER`, `SEMANTIC`, `SIMPLE`, `STYLE-SANS`, `STYLE-SERIF`, `STYLIZE`, `SUPERSTYLIN`, `TACIT`, `VANILLA`, `WATER`, `WATER-DARK`, `WRIT`) berbasis `cleanrmd` / `simov/markdown-viewer`.
  - Pola regex deteksi berkas: `\.(?:markdown|mdown|mkdn|md|mkd|mdwn|mdtxt|mdtext|text)(?:#.*|\?.*)?$` untuk Markdown dan `\.(?:html|htm|shtml|xhtml|xht|dhtml)(?:#.*|\?.*)?$` untuk HTML.
  - Dukungan format standar `markdown-it`: tabel GFM, daftar tugas interaktif (`- [ ]`, `- [x]`), coret teks (`~~teks~~`), sorot teks (`==teks==`), subskrip (`~teks~`), superskrip (`^teks^`), sisip teks (`++teks++`), catatan kaki (`[^1]`), blok kode terlindungi, diagram Mermaid, dan formula matematika LaTeX.
  - Sinkronisasi gulir dua arah (Two-Way SyncScroll) antara editor Scintilla dan viewport pratinjau.

### 8. Paket Peralatan Pengembang (Developer Tools)

- **Utilitas Bawaan untuk Produktivitas Harian**:
  - Menambahkan `DeveloperToolsEngine` pada menu Peralatan: JSON Prettify & Minify, URL Encode & Decode, Unicode Escape & Unescape, dan generator UUID v4.
  - Dialog interaktif `TimestampConverterDialog` untuk konversi timestamp epoch Unix (detik dan milidetik) ke format UTC dan ISO Lokal dengan tombol salin cepat.

### 9. Penanda Navigasi Bilah Gulir & Pengalih Header/Source (`F4` / `Alt+O`)

- **Navigasi Cepat Antara Berkas Kode Sumber Terkait**:
  - Mengimplementasikan `CorrespondingFileSwitcher` untuk peralihan dua arah instan antara header C/C++ (`.h`, `.hpp`, `.hxx`) dan berkas implementasi (`.c`, `.cpp`, `.cxx`), serta berkas kode dan tes TypeScript/JavaScript (`.test.ts`, `.spec.ts`).
  - Menambahkan `ScrollbarNavigationMarkers` yang menampilkan indikator visual pada track bilah gulir untuk bookmark dan penanda hasil pencarian.

### 10. Parser & Penegakan Otomatis `.editorconfig`

- **Standardisasi Format Kode Tim Secara Otomatis**:
  - Mengimplementasikan `EditorConfigParser` yang mendukung aturan `indent_style` (spasi/tab), `indent_size`, `tab_width`, `end_of_line` (lf/crlf/cr), dan penelusuran direktori induk `root = true`.
  - Menerapkan konfigurasi tab dan indentasi secara otomatis saat berkas proyek dibuka.

### 11. Dock Riwayat Clipboard & Info Karakter Unicode

- **Pelacakan Clipboard dan Analisis Karakter Mendalam**:
  - Panel dock `ClipboardHistoryDock` mencatat 20 entri riwayat salinan terakhir dengan penyaringan pencarian dan penyisipan satu klik ke dalam editor.
  - Panel dock `CharacterInfoDock` menyajikan pratinjau glyph Unicode real-time, kode `U+XXXX`, nama karakter, kategori, representasi heksadesimal UTF-8/16/32, dan entitas HTML.

### 12. Pengalih Cepat Fuzzy (`Cmd+P` / `Ctrl+P`) & Dock Minimap Dokumen

- **Navigasi Bergaya Spotlight & Tinjauan Dokumen**:
  - Dialog frameless bergaya Spotlight `QuickSwitcherDialog` dengan pencocokan subsekuens fuzzy pada tab terbuka dan daftar berkas riwayat.
  - `DocumentMinimapDock` menyediakan minimap dokumen yang ringan dengan pelacakan viewport aktif dan batas perlindungan memori untuk berkas raksasa (>50 MB).
  - Integrasi menu berkas terkini pada dock native macOS via `MacServicesHandler::updateDockRecentMenu`.

### 13. Integrasi Rangkaian Paket Ikon Toolbar Aksara (Total 28 Paket) & Ergonomi Menu

- **Kustomisasi Visual Multi-Resolusi**:
  - Mengintegrasikan 10 paket ikon toolbar Aksara kurasi (340 file SVG dan PNG vektor) mencakup tema macOS Big Sur, Windows XP/Vista/10/11, Ubuntu Yaru, Arch Linux, Elementary OS, dan gaya modern (total 28 paket bawaan).
  - Relokasi menu Themes: Toolbar Icon Size ditempatkan di atas Toolbar Icon Packs, memindahkan Kustomisasi Toolbar ke menu Tools, dan menghapus pintasan custom pack dari menubar.
  - Terdaftar dalam `toolbar_icons.qrc` dan `ToolbarIconManager.cpp` dengan normalisasi alias dan cakupan unit test komprehensif.

### 14. Penuntasan Analisis Statis SonarLint & Clang-Tidy (0 Masalah)

- **Pembersihan Menyeluruh Kualitas Kode**:
  - Menuntaskan seluruh 25 peringatan analisis statis SonarLint/SonarQube dan clang-tidy pada direktori `src/` (`misc-const-correctness`, `misc-override-with-different-visibility`, `bugprone-implicit-widening-of-multiplication-result`, `readability-use-concise-preprocessor-directives`, `readability-braces-around-statements`, `readability-else-after-return`, `performance-unnecessary-value-param`).
  - Terverifikasi bersih dengan 0 diagnostik eror pada `clangd --check`, `cppcheck`, dan lulus 100% unit test.

### 15. Invarian Bilah Alat Mode Gelap Permanen di Semua Tema & Mode

- **Kontras Visual Sempurna & Tampilan Ikon Bersih**:
  - Bilah alat utama (`#mainToolBar`, `QToolBar`) dikunci secara permanen pada latar belakang gelap (`#21252b`) di seluruh tema warna, mode terang/gelap, dan setelan tampilan OS.
  - Mencegah ikon pastel dan kromatik ditampilkan pada latar belakang putih, serta menghapus token Qt Style Sheet tidak valid (`:not()` dan `!important`) guna menjamin 100% parsing QSS yang bersih tanpa tembus pandang background Cocoa native.

### 16. Integrasi Editor Default Tipe Media IANA Kanonikal (100+ Format)

- **Integrasi Desktop Menyeluruh**:
  - Skrip `SparkNotePad-Default-Editor.sh` dan skrip Linux diperluas sesuai registri resmi IANA Media Types.
  - Menghubungkan secara mulus 100+ ekstensi berkas kode sumber, format data konfigurasi, bahasa pemrograman sistem, dan dokumen web dengan SparkNotePad.

### 17. Standar Paket DMG Ringan Permanen (30MB–34MB, Batas Maksimal < 35MB)

- **Standar Pengemasan macOS Bersih Tanpa Bloatware**:
  - Bundel DMG rilis macOS melalui `scripts/package-dmg.sh` dijaga ketat pada rentang 30MB–34MB (di bawah batas absolut 35MB).
  - Mempertahankan tata letak presisi `.DS_Store` (`dmg-ds-store`), aset grafis latar belakang Retina `@2x` (`.background/dmg-background@2x.png`), dan ikon volume kustom (`.VolumeIcon.icns`).

---

## 📦 Berkas Biner & Aset Rilis Resmi

| Nama Aset                                     | Platform Target / Deskripsi                                          | Arsitektur    | Format                    |
| :-------------------------------------------- | :------------------------------------------------------------------- | :------------ | :------------------------ |
| **`SparkNotePad-v1.4.5-x64.dmg`**             | Rilis Kanonikal macOS (macOS 11.0+, Qt 6.5 LTS)                      | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-arm64.dmg`**           | Rilis Kanonikal macOS (macOS 11.0+, Qt 6.5 LTS)                      | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.2-x64.dmg`**       | Rilis macOS Catalina Legacy (macOS 10.15+, Qt 6.2 LTS)               | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.2-arm64.dmg`**     | Rilis Legacy macOS (macOS 11.0+, Qt 6.2 LTS)                         | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.5-x64.dmg`**       | Rilis Standar macOS (macOS 11.0+, Qt 6.5 LTS)                        | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.5-arm64.dmg`**     | Rilis Standar macOS (macOS 11.0+, Qt 6.5 LTS)                        | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.8-x64.dmg`**       | Rilis macOS (macOS 12.0+, Qt 6.8 LTS)                                | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.8-arm64.dmg`**     | Rilis macOS (macOS 12.0+, Qt 6.8 LTS)                                | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.10-x64.dmg`**      | Rilis macOS (macOS 13.0+, Qt 6.10)                                   | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.10-arm64.dmg`**    | Rilis macOS (macOS 14.0+, Qt 6.10)                                   | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.11-x64.dmg`**      | Rilis macOS (macOS 13.0+, Qt 6.11)                                   | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.11-arm64.dmg`**    | Rilis macOS (macOS 14.0+, Qt 6.11)                                   | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.12-x64.dmg`**      | Rilis macOS Mutakhir (macOS 13.0+, Qt 6.12)                          | Intel x86_64  | Apple Disk Image (`.dmg`) |
| **`SparkNotePad-v1.4.5-qt6.12-arm64.dmg`**    | Rilis macOS Mutakhir (macOS 14.0+, Qt 6.12)                          | Apple Silicon | Apple Disk Image (`.dmg`) |
| **`sparknotepad_1.4.5_amd64.deb`**            | Linux Debian / Ubuntu Kanonikal (`glibc 2.35+`, Qt 6.5 LTS)          | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.2_amd64.deb`**      | Linux Ubuntu 20.04 LTS Legacy (`glibc 2.31+`, Qt 6.2 LTS)            | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.5_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.5 LTS)                    | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.8_amd64.deb`**      | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.8 LTS)                    | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.8_arm64.deb`**      | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.8 LTS)              | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.10_amd64.deb`**     | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.10)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.10_arm64.deb`**     | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.10)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.11_amd64.deb`**     | Linux Debian / Ubuntu (`glibc 2.35+`, Qt 6.11)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.11_arm64.deb`**     | Linux Debian / Ubuntu ARM64 (`glibc 2.35+`, Qt 6.11)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.12_amd64.deb`**     | Linux Debian / Ubuntu (`glibc 2.39+`, Qt 6.12)                       | x86_64        | Paket Debian (`.deb`)     |
| **`sparknotepad_1.4.5-qt6.12_arm64.deb`**     | Linux Debian / Ubuntu ARM64 (`glibc 2.39+`, Qt 6.12)                 | aarch64       | Paket Debian (`.deb`)     |
| **`sparknotepad-1.4.5-1.x86_64.rpm`**         | Linux Fedora / RHEL / openSUSE Kanonikal (`glibc 2.35+`, Qt 6.5 LTS) | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.2-1.x86_64.rpm`**   | Linux CentOS / RHEL Legacy (`glibc 2.31+`, Qt 6.2 LTS)               | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.5-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.5 LTS)                      | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.8-1.x86_64.rpm`**   | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.8 LTS)                      | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.8-1.aarch64.rpm`**  | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.8 LTS)                | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.10-1.x86_64.rpm`**  | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.10)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.10-1.aarch64.rpm`** | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.10)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.11-1.x86_64.rpm`**  | Linux Fedora / RHEL (`glibc 2.35+`, Qt 6.11)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.11-1.aarch64.rpm`** | Linux Fedora / RHEL ARM64 (`glibc 2.35+`, Qt 6.11)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.12-1.x86_64.rpm`**  | Linux Fedora / RHEL (`glibc 2.39+`, Qt 6.12)                         | x86_64        | Paket RPM (`.rpm`)        |
| **`sparknotepad-1.4.5-qt6.12-1.aarch64.rpm`** | Linux Fedora / RHEL ARM64 (`glibc 2.39+`, Qt 6.12)                   | aarch64       | Paket RPM (`.rpm`)        |
| **`SparkNotePad-Default-Editor.sh`**          | Skrip Pembantu Asosiasi Berkas POSIX macOS & Linux                   | Universal     | Skrip Shell Bash          |
| **`SparkNotePad-Default-Editor-Linux.sh`**    | Skrip Mesin Editor Default Linux 5 Lapisan                           | Linux         | Skrip Shell Bash          |

---

## 📜 Riwayat Perubahan Lengkap

- <https://github.com/alsyundawy/SparkNotePad/compare/v1.4.0...v1.4.5>
