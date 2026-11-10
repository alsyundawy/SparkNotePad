# SparkNotePad v1.4.6 — 30 Paket Ikon Toolbar Baru (Total 69 Koleksi), Ergonomi Menu Konteks, Opsi Preferensi Umum, Perombakan Syntax Highlighting WCAG 2.1 & DMG Rilis 30MB–40MB

**Versi Rilis:** `v1.4.6`<br/>
**Tanggal Rilis:** 2026-11-10<br/>
**Pemelihara:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Mesin Editor Inti:** Scintilla 5.6.9 & Lexilla 5.5.5 (Vendored Resmi)<br/>
**Inti Lua:** Sandboxed Lua 5.5.1 (Sumber Resmi)<br/>
**Runtime Scripting:** Python 3.13+ Engine Bridge<br/>
**Standar Bahasa:** ISO C++20 / Qt 6.5–6.12<br/>
**Platform Target:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu, Debian, Fedora, RHEL/Rocky, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x

---

## ⚡ Ringkasan Eksekutif

SparkNotePad **v1.4.6** menghadirkan peningkatan besar pada kustomisasi visual bilah alat, kenyamanan menu konteks editor, opsi preferensi umum, perombakan pewarnaan sintaksis berstandar WCAG 2.1, serta standardisasi pengemasan macOS DMG 30MB–40MB tanpa bloatware dari versi 1.4.5:

1. **Ekspansi 41 Paket Ikon Toolbar Baru (Total 69 Koleksi Bawaan)**:
   - Menambah koleksi dari 28 paket di v1.4.5 menjadi 69 paket bawaan multi-resolusi lengkap (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
     - 11 Paket Ikon Tematik: `royal_blue`, `notepad_pro`, `pastel`, `fluent_modern`, `macos_bigsur_modern`, `dark_neon_pro`, `macos_glass`, `modern_flat_colorful`, `glossy_3d_skeuomorphic`, `retro_8bit_pixel`, `sketchy_doodle`.
     - 30 Paket Ikon Modern: `01-macos-big-sur`, `02-windows-11-fluent`, `03-arc-browser-glow`, `04-nordic-frost-minimal`, `05-dracula-cyber-neon`, `06-catppuccin-macchiato`, `07-tokyo-night-storm`, `08-synthwave-80s`, `09-monokai-pro-vivid`, `10-solarized-accent`, `11-retro-commodore-64`, `12-retro-win95-classic`, `13-matrix-phosphor-green`, `14-gameboy-nostalgia`, `15-pixel-dungeon-rpg`, `16-claymorphism-3d`, `17-skeuomorphic-leather-brass`, `18-neu-brutalism-bold`, `19-isometric-voxel-3d`, `20-origami-paper-fold`, `21-cyberpunk-2077-yellow`, `22-lofi-aesthetic-grain`, `23-vaporwave-pastel-sunset`, `24-astronomy-deep-space`, `25-botanical-nature-green`, `26-blueprint-schematic`, `27-chalkboard-handdrawn`, `28-gouache-art-paint`, `29-stained-glass-mosaic`, `30-swiss-minimal-3d`.
   - Standar Notepad++ Classic: bebas dari bayangan lantai, halo abu-abu, dan artefak timbul, mengambang sempurna di atas toolbar gelap permanen (`#21252b`) dengan margin nafas 18% untuk layar Retina.

2. **Penyempurnaan Ergonomi Menu Konteks**:
   - Menambahkan menu **Tempel & Sesuaikan Gaya (Paste & Match Style)** (`Cmd+Alt+Shift+V` / `Ctrl+Shift+V`) untuk menempel teks bersih tanpa format dan menyesuaikan indentasi.
   - Menambahkan menu **Cari di Google (Search on Google)** (`Cmd+Alt+G` / `Ctrl+Alt+G`) langsung dari teks yang diblok atau kata di bawah kursor.
   - Menyederhanakan konversi huruf menjadi **Huruf Besar (UPPERCASE)** dan **Huruf Kecil (lowercase)** secara langsung tanpa submenu bertingkat.
   - Menghapus opsi Run dan Bash encode/decode dari menu klik kanan editor agar lebih ringkas dan fokus.

3. **Opsi Baru pada Preferensi Umum**:
   - Pengaturan aktif/nonaktif Deteksi Encoding Otomatis (`enableEncodingAutoDetection`).
   - Pengaturan aktif/nonaktif Konfirmasi Simpan Semua (`enableSaveAllConfirmation`).

4. **Perombakan Mesin Syntax Highlighting & Tema (WCAG 2.1)**:
   - Perhitungan luminansi relatif dan rasio kontras otomatis dengan jaminan kontras minimal 4.0:1 pada teks utama di seluruh 70+ tema editor.
   - Pewarnaan semantik khusus keluarga bahasa (C/C++, Python, Web/HTML/JS/CSS, Markdown H1-H6, SQL, Shell, YAML, dll.).
   - Lencana Emas Menarik (`#FFD700`, tebal) untuk penanda komentar penting (TODO, FIXME, BUG, HACK, NOTE).
   - Warna amber hangat untuk urutan karakter escape (`\n`, `\t`, `\xHH`).
   - Kotak indikator penanda kemunculan kata kunci tersinkronisasi dinamis dengan tema aktif.

5. **Standardisasi Pengemasan macOS DMG Ringan (30MB–40MB)**:
   - Mempertahankan integritas binary universal resmi Qt tanpa lipo -thin berlebih, mempertahankan plugin format gambar standar (`libqjpeg`, `libqgif`, `libqwebp`).
   - Menghasilkan DMG rilis 38MB–39MB dengan tata letak visual kustom `.DS_Store`, background retina, dan checksum CRC32 valid.

6. **Kepatuhan Analisis Statis & Kualitas Kode 14 Pilar**:
   - `cppcheck`: 171/171 berkas pada `src/*.cpp` dan `src/*/*.cpp` lulus dengan 0 kesalahan dan 0 peringatan.
   - `clangd --check` dan `clang-tidy` lulus bersih tanpa diagnostik compiler.
   - Tingkat kelulusan tes unit 100% (`SparkNotePadUnitTests`).
   - 100% paritas terjemahan dwibahasa (Bahasa Inggris dan Bahasa Indonesia).
   - Tingkat kelulusan tes unit 100% (`SparkNotePadUnitTests`).
