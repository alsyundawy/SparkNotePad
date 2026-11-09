# SparkNotePad v1.4.6 — 11 Icon Pack Toolbar Baru (Total 39 Pack), Standar Bersih Pristine Notepad++ Classic, Optimasi Kompresi Lossless & DMG Rilis Ringan

**Versi Rilis:** `v1.4.6`<br/>
**Tanggal Rilis:** 2026-11-09<br/>
**Pemelihara:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Mesin Editor Inti:** Scintilla 5.6.9 & Lexilla 5.5.5 (Vendored Resmi)<br/>
**Inti Lua:** Sandboxed Lua 5.5.1 (Sumber Resmi)<br/>
**Runtime Scripting:** Python 3.13+ Engine Bridge<br/>
**Standar Bahasa:** ISO C++20 / Qt 6.5–6.12<br/>
**Platform Target:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu, Debian, Fedora, RHEL/Rocky, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x

---

## ⚡ Ringkasan Eksekutif

SparkNotePad **v1.4.6** memperluas kustomisasi visual dan standar kualitas packaging aplikasi dengan 11 paket icon toolbar baru, pembersihan total artefak bayangan dan embos sesuai standar emas Notepad++ (Classic), optimasi kompresi PNG lossless, dan penegakan batas ukuran DMG rilis yang ramping:

1. **11 Paket Icon Toolbar Baru (Total 39 Paket Bawaan)**:
   - **`royal_blue` (Royal Blue Aero Glossy)**: Desain kaca skeuomorphic desktop warna biru elegan.
   - **`notepad_pro` (Notepad Pro Vista 7 Style)**: Desain transparan aero modern era Windows Vista/7.
   - **`pastel` (Pastel Modern)**: Palet warna pastel minimalis yang hangat dan bersahabat.
   - **`fluent_modern` (Microsoft Fluent Modern)**: Desain 3D berlapis modern Windows 11 Fluent.
   - **`macos_bigsur_modern` (macOS Big Sur Style)**: Estetika squircle macOS dengan aksen warna cerah.
   - **`dark_neon_pro` (Dark Neon Style)**: Kontras tinggi cyberpunk neon bercahaya di latar gelap.
   - **`macos_glass` (macOS Glassmorphism)**: Estetika kaca buram transparan aero akrilik.
   - **`modern_flat_colorful` (Modern Flat & Colorful)**: Desain vektor datar dengan warna-warna kontras tegas.
   - **`glossy_3d_skeuomorphic` (Glossy 3D Skeuomorphic)**: Desain 3D clay bervolume taktil mengkilap.
   - **`retro_8bit_pixel` (Retro 8-Bit Pixel Art)**: Seni pixel arcade retro klasik.
   - **`sketchy_doodle` (Sketchy Hand-Drawn Doodle)**: Ilustrasi doodle artistik buatan tangan.

2. **Sanitasi Icon Bersih Pristine Menyamai Notepad++ (Classic)**:
   - Menghapus seluruh bayangan lantai, halo ambient occlusion, ubin latar abu-abu, dan strok/embos pada seluruh 11 paket icon baru.
   - Memastikan icon tampil murni mengambang secara bersih di atas toolbar gelap permanen (`#21252b`).
   - Menerapkan margin bernapas 18% di seluruh resolusi (16x16, 20x20, 24x24, 32x32, 48x48, 64x64) sehingga tidak ada icon yang terpotong di tepi.

3. **Optimasi Kompresi PNG Lossless & Penghematan Ruang**:
   - Menjalankan kuantisasi palet fast octree dengan preservasi penuh kanal transparansi alfa serta kompresi zlib level 9 pada 1.736 file aset PNG.
   - Memangkas ukuran disk icon sebesar 2,17 MB (penurunan 28,9%) tanpa penurunan kualitas visual sedikit pun.
   - Mengaktifkan opsi `AUTORCC_OPTIONS "--compress;9;--threshold;0"` pada `src/CMakeLists.txt` untuk kompresi waktu kompilasi resource Qt yang maksimal.

4. **Standar DMG Bebas Bloatware (~33MB, Batas Maksimum < 35MB)**:
   - Menegakkan batas ketat ukuran file DMG antara 30MB hingga 34MB (batas keras 35MB / 36.700.160 bytes).
   - Memangkas framework yang tidak digunakan (`QtDBus.framework`, `QtOpenGL.framework`, `QtPdf.framework`, `QtQml.framework`, `QtQuick.framework`, `QtVirtualKeyboard.framework`).
   - Menghasilkan DMG rilis resmi `SparkNotePad-v1.4.6-x64.dmg` berukuran tepat **35.004.366 bytes (~33,3 MB)** lengkap dengan aset presentasi: layout window `.DS_Store` 680x440, background kustom `.background/dmg-background.png`, icon volume kustom, dan checksum CRC32 valid.

5. **Kepatuhan Analisis Statis, MegaLinter & SonarLint**:
   - `clangd --check` lulus dengan 0 error.
   - `actionlint` lulus pada seluruh berkas alur kerja GitHub Actions tanpa peringatan.
   - `shellcheck` lulus pada seluruh skrip otomasi menggunakan GNU Bash 5.3+ strict mode.
   - Tingkat kelulusan pengujian unit 100% pada seluruh 8 test suite.
