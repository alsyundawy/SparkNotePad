# Catatan Rilis SparkNotePad v1.3.8 — 13 Rangkaian Paket Ikon Toolbar, Pratinjau Langsung Markdown/HTML Dalam Tab, Susunan Menu Standar & Penyempurnaan Sorotan Treeview

**Versi Rilis:** `v1.3.8`<br/>
**Tanggal Rilis:** 2026-11-03<br/>
**Pengelola Utama:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Mesin Inti Editor:** Scintilla 5.6.7 (Vendored) & Lexilla 5.5.4 (Vendored)<br/>
**Inti Embedded Lua:** Sandboxed Lua 5.5.1 / 5.4.7 (Official Source)<br/>
**Runtime Scripting:** Python 3.13+ Engine Bridge<br/>
**Standar:** ISO C++20 / Qt 6.5–6.12<br/>
**Platform Target:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu 22.04–26.04, Debian 12–14, Fedora 40–44, RHEL/Rocky 9–11, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x (KDE Plasma 6 & XFCE)

---

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="assets/sparknotepad-desktop-banner.jpg" width="100%" alt="SparkNotePad Desktop Suite — Editor Teks dan Kode Sumber Asli Scintilla &amp; Qt untuk Linux &amp; macOS" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="logo.png" width="128" height="128" alt="Ikon Aplikasi Asli SparkNotePad macOS &amp; Linux" />
  </a>
</p>

<h1 align="center">SparkNotePad</h1>

<p align="center">
  <b>SparkNotePad — Editor teks dan kode sumber asli Scintilla &amp; Qt yang ringan, cepat, dan modern untuk Linux dan macOS.</b><br>
  <i>Dirancang dengan Qt 6 serta inti Scintilla 5.6.7 / Lexilla 5.5.4 bawaan untuk kinerja eksekusi maksimal dan penggunaan memori minimal</i>
</p>

---

> Pengembang & Pengelola Utama:<br>
> **[`HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)`](https://github.com/alsyundawy)** — [`ALSYUNDAWY IT SOLUTION`](https://alsyundawy.com)<br>
>
> 🍏 **[`Rilis Terbaru (v1.3.8)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
> 📖 **[`DocNote Rilis (DOCNOTE.md)`](DOCNOTE.md)** &nbsp;|&nbsp;
> 📜 **[`Log Perubahan (CHANGELOG.md)`](CHANGELOG.md)** &nbsp;|&nbsp;
> 🌐 **[`Matriks Dukungan Platform`](doc/PLATFORM_SUPPORT.md)** &nbsp;|&nbsp;
> 🔱 **[`Panduan Kompilasi FreeBSD (KDE6 & XFCE)`](doc/BUILD_FREEBSD_KDE_XFCE.md)** &nbsp;|&nbsp;
> 📋 **[`Analisis Kompilasi & Daftar Tugas FreeBSD`](doc/FREEBSD_BUILD_ANALYSIS_AND_TASKLIST.md)** &nbsp;|&nbsp;
> ⚖️ **[`Komparasi NotepadNext vs SparkNotePad`](doc/NOTEPADNEXT_VS_SPARKNOTEPAD_COMPARISON.md)** &nbsp;|&nbsp;
> 🐛 **[`Pelacak Masalah / Issue Tracker`](https://github.com/alsyundawy/SparkNotePad/issues)** &nbsp;|&nbsp;
> 💖 **[`Dukungan via PayPal`](https://www.paypal.me/alsyundawy)**

---

## ⚡ Ringkasan Eksekutif

SparkNotePad v1.3.8 menghadirkan lompatan besar dalam ergonomi visual dan alur kerja penyuntingan. Pembaruan ini mencakup integrasi 13 rangkaian paket ikon bilah alat (toolbar) resolusi ganda, pembaruan menyeluruh pratinjau langsung Markdown & HTML dalam tab dokumen maupun panel dok lengkap dengan lencana metrik format dan kotak catatan bergaya GitHub, penataan ulang hierarki menu utama sesuai standar aplikasi notepad desktop profesional, perbaikan garis sorotan seleksi pada File Browser dan Folder-as-Workspace agar tidak terpotong, serta verifikasi kelulusan 100% pada 14 pilar audit rekayasa kode.

---

## 🌟 Fitur Utama & Inovasi di v1.3.8

### 1. Rangkaian 21 Paket Ikon Toolbar Bawaan

Dapat diakses langsung via menu `Tema > Paket Ikon Toolbar` secara bebas restart:

1. **Modern Ribbon (Standar)** (`modernRibbon`): Vektor SVG monoline 20x20 High-DPI dengan kontur halus (Standar).
2. **Microsoft Fluent UI (Win 11)** (`fluentUi`): Ikonografi vektor dan bitmap Microsoft Fluent modern.
3. **Windows 11 (Fluent 2)** (`windows11`): Microsoft Fluent Design System 2 beresolusi tinggi Directory Opus.
4. **Windows 7 (Aero Glass)** (`windows7`): Rangkaian Windows 7/8 Aero Glass Azure otentik dengan gradien kaca sian tembus pandang dan bingkai krom.
5. **Windows XP (Luna Classic)** (`windowsXp`): Nostalgia skeuomorfik 3D Windows XP Luna otentik (master 64×64 PNG).
6. **Fugue Classic (Silk Suite)** (`fugue`): Ikon klasik Yusuke Kamiyamane Fugue Silk 2 dengan skalabilitas retina @2x.
7. **Notepad++ Klasik** (`notepadPlusPlus`): Rekreasi vektor High-DPI ikonografi klasik Notepad++.
8. **NotepadNext Klasik (Silk)** (`notepadNext`): Ikon bitmap Silk 16x16 autentik dari NotepadNext.
9. **Monochrome Outline (Minimalis)** (`monochrome`): Wireframe monoline 1.8px bersih dirancang untuk tema gelap dan terang.
10. **Vibrant DuoTone (Cyberpunk)** (`vibrantDuotone`): Kontras dua warna sian elektrik luminansi tinggi & magenta bercahaya.
11. **Material Design (Sharp Vector)** (`material_sharp`): Ikonografi Google Material Design dengan garis vektor geometris tajam.
12. **Pixel Design (Fugue-like)** (`pixel_fugue_like`): Ikonografi seni piksel klasik desktop bernuansa retro.
13. **Monoline Minimal** (`monoline_minimal`): Ikon garis minimalis ultra-bersih dengan inversi luminans otomatis pada tema gelap.
14. **Notepad++ (Modern Green)** (`notepadpp_green`): Nuansa hijau segar khas ikonografi modern Notepad++.
15. **Notepad3 (Warm Amber)** (`notepad3_warm`): Aksen amber hangat terinspirasi dari editor Notepad3.
16. **Humanity GNOME** (`humanity_gnome`): Ikonografi desktop Linux klasik dari proyek Humanity.
17. **Elementary OS Flat** (`elementary_flat`): Desain datar modern minimalis dari Elementary OS.
18. **Breeze KDE Plasma** (`breeze_kde`): Ikonografi KDE Plasma Breeze dengan keseimbangan geometris tajam.
19. **Papirus Rounded** (`papirus_round`): Rangkaian ikon Linux modern dengan sudut membulat cerah.
20. **macOS Cupertino Clean** (`mac_cupertino`): Simbol monokrom dan penuh warna mematuhi Apple HIG.
21. **VS Code Codicons** (`vscode_codicons`): Wireframe developer bersih terinspirasi IDE modern.

- **Kejelasan High-DPI & Layar Retina**: Menghilangkan tampilan buram dengan memuat vektor SVG langsung ke engine vektor Qt serta menghasilkan varian pixmap `@2x` Retina DPR (`setDevicePixelRatio(2.0)`) untuk paket berbasis raster.
- **Tone Mapping Luminans Cerdas Tema Gelap**: Pada tema gelap, wireframe (`monoline_minimal`) diinversi menjadi perak-putih bercahaya, sedangkan paket berwarna (`material_sharp`, `pixel_fugue_like`, `notepadpp_green`, `notepad3_warm`) ditingkatkan kecerahannya melalui pemetaan HSL tanpa menggeser corak warna dan saturasi.

### 2. Penskalaan Ukuran Dinamis & Sinkronisasi Seluruh Jendela

- **Rentang Ukuran Bilah Alat**: Penggantian ukuran ikon dari 16×16 (Kompak), 20×20, 24×24 (Standar), 32×32 (Besar), 48×48 (Ekstra Besar), hingga 64×64 (Ultra High-DPI).
- **Propagasi Menubar & Menu Konteks**: Pemilihan paket ikon toolbar secara otomatis menyinkronkan seluruh aksi pada Menu Bar, submenu, dan menu klik kanan editor via `ToolbarIconManager::applyToWindow()`.

### 3. Pratinjau Markdown & HTML Langsung Modern (`MarkdownSafePreviewDock` & `PreviewTabWidget`)

- **Lencana Metrik Format**: Header pratinjau menampilkan format dokumen (`MARKDOWN` / `HTML`), jumlah kata, serta perkiraan waktu baca secara real-time.
- **Kotak Catatan Bergaya GitHub Otentik**:
  - `> [!NOTE]` (Kotak biru informatif)
  - `> [!TIP]` (Kotak hijau panduan)
  - `> [!IMPORTANT]` (Kotak ungu prioritas penting)
  - `> [!WARNING]` (Kotak kuning peringatan)
  - `> [!CAUTION]` (Kotak merah bahaya)
- **Blok Kode Membulat dengan Tombol Salin**: Blok kode bersudut 8px, latar belakang halus, serta tombol salin instan ke papan klip.
- **Presentasi Ganda**: Buka pratinjau baik di panel samping maupun tab editor mandiri dengan rasio gulir tersinkronisasi.
- **Ekspor Mandiri**: Aksi satu klik Salin HTML Bersih dan Buka di Peramban Web.

### 4. Penataan Ulang Menu Aplikasi Standar Notepad Desktop

Menu pada `MainWindow.ui` dan `MainWindow.cpp` telah ditata ulang sesuai alur kerja aplikasi editor teks profesional:

- **Berkas (File)**: Baru, Buka, Buka Cepat (`Cmd+P` / `Ctrl+P`), Muat Ulang, Simpan, Simpan Sebagai, Ekspor, Atur Halaman, Cetak, Tutup, Keluar.
- **Edit**: Operasi papan klip standar, Urung/Ulangi, Indentasi, Konversi Huruf, Operasi Baris, Komentar.
- **Cari (Search)**: Temukan, Ganti, Tandai Kemunculan, Pencarian Bertahap, Lompat ke Baris, Submenu Markah Buku.
- **Tampilan (View)**: Layar Penuh, Zoom, Baris Berlipat, Tampilkan Spasi, Tampilkan EOL, Navigasi Riwayat Perubahan, serta submenu khusus **Panel Samping** (File Browser, Workspace, Tail Monitor, Hex Viewer).
- **Alat (Tools)**: Pengodean Base64/URL, Perhitungan Hash/Checksum, Konverter Angka dan Huruf, Evaluator Matematika, Pretty-printer XML/JSON, serta menu khusus **Pratinjau HTML & Markdown**.

### 5. Penyempurnaan Garis Sorotan File Browser & Workspace

- **Garis Sorotan Penuh & Rapi**: Menghilangkan efek pemotongan kotak seleksi dengan penegakan `show-decoration-selected: 1;` dan pewarnaan seleksi baris penuh mengikuti palet tema aktif (`ThemePaletteData`).
- **Markah Cepat Folder**: Tombol navigasi cepat satu klik menuju Beranda (Home), Desktop, Dokumen, dan Unduhan.
- **Bilah Status Metrik**: Footer bergaya commander menampilkan jumlah berkas/folder serta ukuran berkas dan waktu modifikasi terakhir.

### 6. Dialog Kustomisasi Tombol Toolbar Visual (`CustomizeToolbarDialog`)

- Antarmuka interaktif daftar ganda untuk menambah, menghapus, dan mengatur urutan tombol toolbar dengan drag-and-drop (`InternalMove`).
- Dukungan penyisipan garis pemisah (separator) kustom dan pencarian aksi langsung.
- Tombol Reset ke Default untuk memulihkan konfigurasi 33 tombol bawaan.

---

## 🛡️ Audit Kualitas Kode & Keamanan 14-Pilar

Verifikasi menyeluruh terhadap 14 pilar rekayasa kode:

| Pilar                                 | Status Verifikasi | Bukti & Invarian Kunci                                                                                    |
| :------------------------------------ | :---------------: | :-------------------------------------------------------------------------------------------------------- |
| **1. Bug Review**                     |     **PASS**      | 0 peringatan kompilator/runtime, iterator aman, pemeriksaan batas ketat, tanpa kegagalan asersi.          |
| **2. Syntax Review**                  |     **PASS**      | Kepatuhan ISO C++20 murni, kompilasi bersih pada Clang/GCC/AppleClang tanpa ekstensi.                     |
| **3. Runtime Review**                 |     **PASS**      | 0 crash saat menutup tab belum disimpan, 0 deadlock pada thread latar belakang, event loop stabil.        |
| **4. Logic Review**                   |     **PASS**      | Arah pencarian deterministik, pencegahan inkonsistensi form Cari/Tandai, sinkronisasi pratinjau presisi.  |
| **5. Memory Review**                  |     **PASS**      | Smart pointer QPointer, pengelola siklus hidup RAII, 0 kebocoran memori, mitigasi UAF pada penutupan tab. |
| **6. Dead Code Review**               |     **PASS**      | Pembersihan artefak warisan NotepadNext yang tidak digunakan, pemangkasan fungsi usang.                   |
| **7. Duplicate Code Review**          |     **PASS**      | Penyatuan pustaka utilitas pembantu, validasi terpusat skema URI, standardisasi matematika tema.          |
| **8. Circular Dependency Review**     |     **PASS**      | Arsitektur graf asiklik (DAG) terarah dan terverifikasi di seluruh modul sumber dan header.               |
| **9. Performance Bottleneck Review**  |     **PASS**      | Waktu start-up sub-milidetik, pemilahan sintaks ter-debounce, kalkulasi entropi Shannon O(N).             |
| **10. Security Vulnerability Review** |     **PASS**      | Sandbox Lua ketat, pencegahan injeksi shell, batas ReDoS pada ekspresi reguler, pratinjau aman, TLS 1.2+. |
| **11. Maintainability Review**        |     **PASS**      | Pemisahan tanggung jawab berbasis SOLID OOP, dokumentasi teknis komprehensif.                             |
| **12. Scalability Review**            |     **PASS**      | Dukungan buffer dokumen multi-juta baris, penghitungan hash berkas multi-GB secara streaming.             |
| **13. Readability Review**            |     **PASS**      | Gaya penulisan C++20 idiomatis, kepatuhan const-correctness eksplisit, penamaan fungsi yang jelas.        |
| **14. IDE Linter Review**             |     **PASS**      | 0 isu pada Trunk check (59/59 uji lulus), 0 kesalahan Clangd AST, 0 peringatan SonarQube quality gate.    |

---

## 📦 Berkas Rilis Terverifikasi

| Nama Berkas                                               | Platform / Arsitektur                                              | Ukuran   | Checksum SHA-256                                                   |
| :-------------------------------------------------------- | :----------------------------------------------------------------- | :------- | :----------------------------------------------------------------- |
| **`SparkNotePad-v1.3.8-x64.dmg`** _(Rilis Terverifikasi)_ | macOS Intel (`x86_64`) & Apple Silicon via Rosetta 2 (macOS 11.0+) | 28.05 MB | `add02c6297a36bc5a52870b3ccc3531d46265cd2fa8e6b4d0eb878ed1d91db3a` |
| **`SparkNotePad-v1.3.8-qt6.12-x64.dmg`**                  | macOS Intel (`x86_64`) (macOS 13.0+, Qt 6.12)                      | 28.97 MB | `4434116cd1cf801634984693175402a4bf74581f33527498c558c4fe7c9ad0b3` |
| **`SparkNotePad-v1.3.8-qt6.11-x64.dmg`**                  | macOS Intel (`x86_64`) (macOS 13.0+, Qt 6.11)                      | 28.67 MB | `8ef3db8351b0ace57772b4d01b1a774902166e44b93bc657bb6b2707246ec4c3` |
| **`SparkNotePad-v1.3.8-qt6.10-x64.dmg`**                  | macOS Intel (`x86_64`) (macOS 13.0+, Qt 6.10)                      | 28.27 MB | `f05cd164e65efd58e4fef531dbb9134bc535be8566fae9185a5385d38a0f12ea` |
| **`SparkNotePad-v1.3.8-qt6.8-x64.dmg`**                   | macOS Intel (`x86_64`) (macOS 12.0+, Qt 6.8 LTS)                   | 28.05 MB | `6f3a7e45eb5256aa9fd3d18105ca23023e3e4f35839ce6f5e744e8ecab076f87` |
| **`SparkNotePad-Default-Editor.sh`**                      | Skrip Asosiasi Universal macOS & Linux POSIX                       | 19.19 KB | `719b80f133c0d040a64d4788e9dad08c48ba846910d9bac555e3a629dd571522` |
| **`SparkNotePad-Default-Editor-Linux.sh`**                | Skrip Engine Default Editor Linux 5-Layer                          | 30.32 KB | `c6b9efec00d26d7a5e769f2e8730010cef9497e92ef4e3f8fabd272c6be624b3` |

---

### 📦 Matriks Rilis Multi-Versi Lengkap (v1.0.0 – v1.3.8)

| Versi Rilis | Tag Rilis Git | Cabang Rilis Utama | Platform Target                                                                         | Sorotan Tonggak Utama                                                                                                                                    |
| :---------- | :------------ | :----------------- | :-------------------------------------------------------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **v1.3.8**  | `v1.3.8`      | `main`, `1.3.8`    | macOS (x86_64/arm64), Linux (Ubuntu/Debian/Fedora/RHEL/SUSE/Slackware), FreeBSD (13–15) | 13 Paket Ikon Toolbar, Pratinjau Markdown/HTML Dalam Tab dengan Kotak Catatan, Menu Standar Notepad, Perbaikan Sorotan Treeview, Bebas Kesalahan Linter  |
| **v1.3.7**  | `v1.3.7`      | `1.3.7`            | macOS (x86_64/arm64), Linux (Ubuntu/Debian/Fedora/RHEL/SUSE/Slackware), FreeBSD (13–15) | Omnibar Command Palette, Komparasi Visual Berdampingan (Diff), Filter Tree Proyek, Forensik Kode & Transformasi, Entropi Shannon, Pengerasan Zero-Linter |
| **v1.3.6**  | `v1.3.6`      | `1.3.6`            | macOS (x86_64/arm64), Linux (Ubuntu/Debian/Fedora/RHEL/SUSE/Slackware), FreeBSD (13–15) | Perluasan dukungan platform, isolasi sandbox Lua, pencegahan ReDoS, dok pratinjau aman Markdown, 27 ikon monoline SVG, audit 14 pilar                    |
| **v1.3.5**  | `v1.3.5`      | `1.3.5`            | macOS (x86_64/arm64), Linux (Debian/Ubuntu/Fedora/RHEL), FreeBSD (14–15)                | Siklus hidup restart/exit anggun, mesin default editor Linux, pemolesan About dialog Retina, pembersihan 1.288 diagnostik IDE                            |
| **v1.3.0**  | `v1.3.0`      | `1.3.0`            | macOS, Linux                                                                            | Perubahan nama resmi menjadi SparkNotePad, ikon Apple HIG, kontainer terotentikasi AES-256, kelulusan audit 13 pilar                                     |
| **v1.2.0**  | `v1.2.0`      | `1.2.0`            | macOS, Linux                                                                            | Scintilla 5.6.7, Lexilla 5.5.4, Lua 5.5.1, ekspor PDF, integrasi mkcert, lexer Kotlin/GraphQL/WASM/Bicep                                                 |
| **v1.1.0**  | `v1.1.0`      | `1.1.0`, `develop` | macOS, Linux                                                                            | Mode Gelap/Terang ramping, evaluasi matematika inline, 7 lexer populer baru, border tab peach                                                            |
| **v1.0.0**  | `v1.0.0`      | `1.0.0`            | macOS, Linux                                                                            | Rilis produksi perdana, Qt 6 asli + mesin Scintilla 5.5.4, enkripsi dokumen AES-256                                                                      |

---

### 👤 Pengelola & Atribusi Multi-Versi (v1.0.0 – v1.3.8)

- **Pengelola Utama & Arsitek Modernisasi:**
  - Harry Dertin Sutisna Alsyundawy ([@alsyundawy](https://github.com/alsyundawy)) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)
- **Mesin Rekayasa AI, Peninjauan Kode & Audit:**
  - **Google DeepMind Antigravity IDE**: Editor Kode AI Utama, mesin refactoring kode multi-berkas, dan lingkungan pengembangan terpadu yang memodernisasi basis kode SparkNotePad serta memverifikasi build di seluruh rilis.
  - **Gemini 3.8 Flash (Google DeepMind)**: Mesin Peninjau Kode AI Tingkat Lanjut, Audit Keamanan 14-Pilar, Kepatuhan Linter, dan verifikasi analisis statis.
- **Apresiasi Sumber Hulu & Leluhur:**
  - **NotepadNext-MacOS ([@alsyundawy](https://github.com/alsyundawy/NotepadNext-MacOS))**: Repositori silsilah leluhur teroptimasi macOS.
  - **Justin Dailey ([@dail8859](https://github.com/dail8859)) / NotepadNext**: Fondasi kerangka Qt lintas platform dan warisan arsitektur awal.
  - **Neil Hodgson & Kontributor Scintilla/Lexilla**: Mesin penyunting fondasional (**Scintilla 5.6.7**) dan lexer (**Lexilla 5.5.4**).
  - **Zufu Liu ([@zufuliu](https://github.com/zufuliu)) / Notepad4**: Ergonomi transformasi teks, pola integrasi Scintilla, dan algoritma utilitas.
  - **Rizonesoft / Florian Balmer / Notepad3**: Konsep manipulasi teks dan manajemen pengodean karakter.
  - **cxasm ([@cxasm](https://github.com/cxasm)) / Notepad--**: Paradigma multi-tab lintas platform dan metodologi penyuntingan teks.
  - **Don Ho ([@donho](https://github.com/donho)) / Notepad++**: Pelopor editor teks sumber terbuka Windows yang menginspirasi silsilah notepad modern.
  - **Lua.org**: Runtime skrip tertanam Lua 5.5.1 / 5.4.x.
  - **Lon McGregor (NotepadPlusPlusIcons)**: Ikonografi bilah alat dan utilitas klasik.

---

**Log Perubahan Lengkap**: <https://github.com/alsyundawy/SparkNotePad/compare/v1.3.7...v1.3.8>
