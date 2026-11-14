# Catatan Rilis SparkNotePad v1.4.7 — Peningkatan Engine Inti (Scintilla 5.6.9, Lexilla 5.5.5, Patch Lua 5.5.1), Transfer Sistem (.sparkbundle), Inspektur Berkas POSIX, Pratinjau Markdown Asinkron, Deteksi Enkoding 3-Titik & Penguatan Keamanan CVE 2025–2026

**Versi Rilis:** `v1.4.7`<br/>
**Tanggal Rilis:** 2026-11-14<br/>
**Pengembang Utama:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Engine Inti:** Scintilla 5.6.9 (Vendored Resmi) & Lexilla 5.5.5 (Vendored Resmi)<br/>
**Runtime Lua:** Sandboxed Lua 5.5.1 (Sumber Resmi Ter-patch 1-4)<br/>
**Bridge Scripting:** Python 3.13+ Engine Bridge<br/>
**Standar:** ISO C++20 / Qt 6.5–6.12<br/>
**Target Platform:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu 22.04–26.04, Debian 12–14, Fedora 40–44, RHEL/Rocky 9–11, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x (KDE Plasma 6 & XFCE)

---

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="assets/sparknotepad-desktop-banner.jpg" width="100%" alt="SparkNotePad Desktop Suite — Editor Teks &amp; Kode Sumber Native Scintilla &amp; Qt untuk Linux &amp; macOS" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="logo.png" width="128" height="128" alt="Ikon Aplikasi Native macOS &amp; Linux SparkNotePad" />
  </a>
</p>

<h1 align="center">SparkNotePad</h1>

<p align="center">
  <b>SparkNotePad — Editor teks dan kode sumber native berbasis Scintilla &amp; Qt yang ringan dan cepat untuk Linux, FreeBSD, dan macOS.</b><br>
  <i>Dibangun dengan Qt 6 dan engine inti Scintilla 5.6.9 / Lexilla 5.5.5 native untuk performa eksekusi maksimal dan efisiensi memori tinggi</i>
</p>

---

> Pengembang Utama & Pemelihara:<br>
> **[`HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)`](https://github.com/alsyundawy)** — [`ALSYUNDAWY IT SOLUTION`](https://alsyundawy.com)<br>
>
> 🍏 **[`Rilis Terkini (v1.4.7)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
> 📖 **[`Catatan Rilis Teknis (DOCNOTE.md)`](DOCNOTE.md)** &nbsp;|&nbsp;
> 📜 **[`Catatan Perubahan (CHANGELOG.md)`](CHANGELOG.md)** &nbsp;|&nbsp;
> 🌐 **[`Matriks Dukungan Platform`](doc/PLATFORM_SUPPORT.md)** &nbsp;|&nbsp;
> 🔱 **[`Panduan Build FreeBSD (KDE6 & XFCE)`](doc/BUILD_FREEBSD_KDE_XFCE.md)** &nbsp;|&nbsp;
> 📋 **[`Analisis & Tasklist FreeBSD`](doc/FREEBSD_BUILD_ANALYSIS_AND_TASKLIST.md)** &nbsp;|&nbsp;
> 🐛 **[`Pelacak Masalah (Issue Tracker)`](https://github.com/alsyundawy/SparkNotePad/issues)** &nbsp;|&nbsp;
> 💖 **[`Dukungan via PayPal`](https://www.paypal.me/alsyundawy)**

---

## ⚡ Ringkasan Eksekutif

SparkNotePad **v1.4.7** menghadirkan pembaruan substansial pada engine inti pengeditan teks, mobilitas konfigurasi ruang kerja lintas platform, inspeksi izin berkas POSIX dan dialek skrip, keandalan deteksi pengodean karakter dokumen besar, dialog navigasi baru, serta penguatan keamanan menyeluruh terhadap kerentanan CVE 2025–2026:

1. **Peningkatan Scintilla 5.6.9 & Lexilla 5.5.5**: Integrasi tabel karakter Unicode 17, peningkatan tembolok leksem menjadi 40 bita, akselerasi tata letak awal baris, perbaikan crash undo riwayat perubahan, pemulihan warna penanda/pelengkapan otomatis XPM, dan 12 perbaikan keamanan memori upstream Lexilla.
2. **Peningkatan Menyeluruh Pratinjau Markdown & Engine Gambar Jaringan (`MarkdownBrowser`)**: Pratinjau Markdown berakurasi tinggi dengan pengunduhan gambar remote `http`/`https` asinkron tanpa memblokir GUI, rasterisasi vektor SVG beresolusi tinggi untuk layar Retina/4K, proteksi blok HTML mentah agar tidak terpotong parser Qt, dan resolusi aset relatif dinamis.
3. **Transfer Sistem & Bundel Ruang Kerja Portabel (`SparkTransferManager`)**: Ekspor dan impor konfigurasi pengguna (preferensi, tema QSS, makro, sesi, dan skrip) via arsip `.sparkbundle` dengan proteksi Zip-Slip dan verifikasi HMAC dalam memori.
4. **Inspektur Skrip POSIX & Utilitas Touch (`PosixFileInspector`)**: Deteksi dialek shebang real-time, pemformat izin oktal dan simbolik, lencana izin bilah status dengan satu-klik beralih izin eksekusi (`chmod +x`), serta utilitas penyesuaian waktu berkas (touch).
5. **Deteksi Enkoding 3-Titik & Proteksi Simpan Berkas Besar Atomik**: Pengambilan sampel 3-titik (kepala 16KB, tengah 16KB, ekor 16KB) untuk keandalan deteksi set karakter dokumen besar, dukungan BOM UTF-32 LE/BE, dan penyimpanan atomik via berkas sementara privat (`0600`).
6. **Dialog Navigasi Multi-Mode & Pratinjau Ganti Nama Aman**: Navigasi presisi berdasarkan Baris:Kolom, Ofset Karakter, atau Ofset Bita, serta penggantian nama aman pada pohon folder dengan pratinjau uji coba (dry-run).
7. **Penguatan Keamanan Komprehensif CVE 2025–2026**: Kekebalan penuh dan verifikasi otomatis terhadap celah keamanan CVE-2026-42214, CVE-2026-48778, CVE-2026-52885, CVE-2026-52886, CVE-2026-57233, CVE-2026-54758, dan CVE-2026-77605.

---

## 🛡️ Matriks Mitigasi Keamanan CVE 2025–2026

| Kerentanan                         | Cakupan Masalah                                        | Mitigasi SparkNotePad v1.4.7                                                                 |
| :--------------------------------- | :----------------------------------------------------- | :------------------------------------------------------------------------------------------- |
| **CVE-2026-42214**                 | Injeksi kode Lua via ekstensi berkas di NotepadNext    | Parameter dilewatkan melalui tumpukan C Lua (`lua_pushlstring`); pustaka OS/IO dinonaktifkan |
| **CVE-2026-48778 / 48800 / 52885** | RCE / TOCTOU manipulasi konfigurasi di Notepad++       | Validasi HMAC-SHA256 dalam memori; penulisan berkas atomik izin 0600                         |
| **CVE-2026-52886 / 57233**         | Penelusuran jalur / Zip-Slip overwrite di Notepad++    | Kanonikalisasi jalur via `weakly_canonical` / `canonicalFilePath`; penolakan traversal ketat |
| **CVE-2026-54758 / 85279 / 86054** | Buffer overflow berbasis tumpukan di Notepad++         | Nol buffer tumpukan berukuran tetap; seluruh string menggunakan `QString` dan `std::string`  |
| **CVE-2026-77605 / 25926 / 38972** | Pembajakan jalur CWD / library eksternal               | Resolusi jalur absolut kanonikal untuk utilitas sistem eksternal                             |
| **CVE-2025-14575 / 2026-20841**    | Pencarian sertifikat CWD OpenSSL Qt & URI RCE Markdown | Minimum TLS 1.2+; pembatasan skema URI pratinjau ketat                                       |

---

## 📦 Matriks Unduhan & Paket Rilis

| Platform                    | Arsitektur Target    | Format | Tautan Unduhan                                                                                                                         |
| :-------------------------- | :------------------- | :----: | :------------------------------------------------------------------------------------------------------------------------------------- |
| **macOS Intel (Kanonikal)** | x86_64               | `.dmg` | [SparkNotePad-v1.4.7-x64.dmg](https://github.com/alsyundawy/SparkNotePad/releases/download/v1.4.7/SparkNotePad-v1.4.7-x64.dmg)         |
| **macOS Apple Silicon**     | arm64 (M1–M4)        | `.dmg` | [SparkNotePad-v1.4.7-arm64.dmg](https://github.com/alsyundawy/SparkNotePad/releases/download/v1.4.7/SparkNotePad-v1.4.7-arm64.dmg)     |
| **Linux Ubuntu/Debian**     | x86_64 (glibc 2.35+) | `.deb` | [sparknotepad_1.4.7_amd64.deb](https://github.com/alsyundawy/SparkNotePad/releases/download/v1.4.7/sparknotepad_1.4.7_amd64.deb)       |
| **Linux Fedora/RHEL**       | x86_64 (glibc 2.35+) | `.rpm` | [sparknotepad-1.4.7-1.x86_64.rpm](https://github.com/alsyundawy/SparkNotePad/releases/download/v1.4.7/sparknotepad-1.4.7-1.x86_64.rpm) |
