# Catatan Rilis SparkNotePad v1.5.0 — Rangkaian Fitur Lengkap Developer Tools (JSON/XML/HTML/CSS/JS, Enkripsi, JWT, Radiks), Restrukturisasi Menu, Pratinjau Markdown GitHub Primer & Peningkatan Tampilan

**Versi Rilis:** `v1.5.0`<br/>
**Tanggal Rilis:** 2026-11-19<br/>
**Pengembang & Pengelola:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Engine Penyunting Inti:** Scintilla 5.6.9 (Vendored Official) & Lexilla 5.5.5 (Vendored Official)<br/>
**Engine Lua Tertanam:** Sandboxed Lua 5.5.1 (Sumber Resmi Ter-patch 1-4)<br/>
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
  <b>SparkNotePad — editor teks dan kode sumber native yang ringan dan cepat untuk Linux, FreeBSD, dan macOS.</b><br>
  <i>Dibangun dengan Qt 6 dan engine Scintilla 5.6.9 / Lexilla 5.5.5 native untuk performa maksimal dan efisiensi memori tinggi</i>
</p>

---

> Lead Developer & Maintainer:<br>
> **[`HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)`](https://github.com/alsyundawy)** — [`ALSYUNDAWY IT SOLUTION`](https://alsyundawy.com)<br>
>
> 🍏 **[`Rilis Terbaru (v1.5.0)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
> 📖 **[`DocNotes Rilis (DOCNOTE.md)`](DOCNOTE.md)** &nbsp;|&nbsp;
> 📜 **[`Riwayat Perubahan (CHANGELOG.md)`](CHANGELOG.md)** &nbsp;|&nbsp;
> 🌐 **[`Matriks Dukungan Platform`](doc/PLATFORM_SUPPORT.md)** &nbsp;|&nbsp;
> 🐛 **[`Pelacak Masalah (Issue Tracker)`](https://github.com/alsyundawy/SparkNotePad/issues)** &nbsp;|&nbsp;
> 💖 **[`Dukungan via PayPal`](https://www.paypal.me/alsyundawy)**

---

## ⚡ Ringkasan Eksekutif

SparkNotePad **v1.5.0** menghadirkan pembaruan fitur yang masif dan modernisasi ergonomis di seluruh aplikasi:

1. **Rangkaian Lengkap Developer Tools (`DeveloperToolsEngine` & `DeveloperToolsDialog`)**:
   - Dialog interaktif multi-tab Developer Tools Hub (`Ctrl+Alt+T` / `Ctrl+Cmd+T`) dengan 5 tab fungsional.
   - **Hash Tools**: 11 algoritma hash kriptografis (MD5, SHA-1, SHA-224, SHA-256, SHA-384, SHA-512, SHA3-224, SHA3-256, SHA3-384, SHA3-512, Keccak-256), hashing integritas berkas, serta perbandingan checksum berdampingan dengan indikator kecocokan visual.
   - **MIME Tools**: Enkode/dekode Base64 baris demi baris, Quoted-Printable (RFC 2045), URL encode/decode, dan HTML entities encode/decode.
   - **Converter Tools**: Konverter basis Radiks (Biner, Oktal, Desimal, Heksadesimal), konverter ASCII <-> Heksadesimal dengan pemisah kustom, serta konversi format huruf (Title Case, Invert Case, Sentence Case).
   - **Encryption Tools**: Proteksi Enkripsi Dokumen HTML dengan 3 mode operasional:
     1. Injeksi unescape JavaScript.
     2. Enkode Base64 `atob()`.
     3. Enkripsi standar militer Password AES-256-GCM dengan harness runtime dekripsi CryptoJS tertanam.
     - Pengukur kekuatan kata sandi dan entropi ala zxcvbn secara real-time.
   - **Developer Tools**: Pemformat (pretty-print) dan pemadat (minify) untuk JSON, XML, HTML, CSS, dan JavaScript; Inspektur Token RFC 7519 JWT dengan ekstraksi header/payload serta validasi timestamp kedaluwarsa; pembuat UUID v4; dan konverter dua arah Unix Epoch / ISO-8601.

2. **Pratinjau Langsung Markdown Aman GitHub Primer Sempurna (`mjbvz/vscode-github-markdown-preview-style`)**:
   - Rekayasa ulang engine perender pratinjau yang sepenuhnya selaras dengan token visual GitHub Markdown Preview.
   - Pembersihan seluruh artefak internal Qt rich text (`-qt-block-indent`, margin bawaan paragraf) demi menjaga tipografi GitHub native, garis batas, zebra striping tabel, dan kotak kode.
   - Dukungan native GitHub Alert Callouts (`[!NOTE]`, `[!TIP]`, `[!IMPORTANT]`, `[!WARNING]`, `[!CAUTION]`) dengan garis tepi dan lencana resmi.
   - Penghapusan total tombol dan fungsi "Open in Default Browser" di seluruh dock dan widget pratinjau demi keamanan maksimal dan operasi mandiri tanpa bloatware.

3. **Restrukturisasi Menu Tingkat Atas**:
   - Penambahan menu tingkat atas **Format** yang mengelompokkan konversi set karakter / enkoding (`ui->menuEncodings`) dan konversi pemisah baris (`ui->menuEOLConversion`).
   - Relokasi menu **Run & External Execution** dari bilah menu utama ke dalam menu `Tools -> Run & External Execution` (`Ctrl+Alt+R` / `F5`).
   - Penambahan submenu khusus **Developer Tools** di dalam `Tools` dengan aksi langsung untuk formatter, minifier, encoder, dan dialog Developer Tools Hub (`Ctrl+Alt+T`).

4. **Peningkatan Sakelar Tampilan (View & Navigation Toggles)**:
   - Penambahan sakelar di menu `View` untuk mengaktifkan/menonaktifkan Nomor Baris, Pelipatan Kode, Garis Panduan Indentasi, Sorotan Baris Kursor, Mode Hanya-Baca (Read-Only), serta Pembesaran (Zoom In / Out / Reset).

5. **Pembaruan Kredit, Pintasan & Terjemahan**:
   - Pembaruan tab Kredit & Pintasan pada dialog `AboutDialog` dengan hotkey dan atribusi baru.
   - Sinkronisasi katalog terjemahan `i18n` (`SparkNotePad_id.ts`, `SparkNotePad_en.ts`) dan kompilasi berkas biner `.qm`.
