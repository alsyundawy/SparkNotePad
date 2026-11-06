# Catatan Rilis SparkNotePad — v1.3.9

**Versi Rilis:** `v1.3.9`<br/>
**Tanggal Rilis:** 07 November 2026<br/>
**Pengelola Utama:** Harry Dertin Sutisna Alsyundawy ([@alsyundawy](https://github.com/alsyundawy))<br/>
**Organisasi:** ALSYUNDAWY IT SOLUTION ([https://alsyundawy.com](https://alsyundawy.com))<br/>
**Repositori:** [https://github.com/alsyundawy/SparkNotePad](https://github.com/alsyundawy/SparkNotePad)<br/>
**Lisensi:** GNU General Public License v3.0 (`GPL-3.0-or-later`)

---

## 🌟 Yang Baru di v1.3.9

SparkNotePad v1.3.9 merupakan rilis stabilitas dan peningkatan engine inti fundamental yang menghadirkan pembaruan upstream resmi ke **Scintilla 5.6.9** dan **Lexilla 5.5.5**, mengatasi kasus khusus (*edge cases*) berbahaya pada pencarian dan penggantian ekspresi reguler, memperkenalkan pengaturan bahasa default yang dapat dikonfigurasi pengguna untuk tab/dokumen baru, memperkaya penyorotan sintaksis front-matter Markdown, serta lolos audit kualitas kode dan analisis statis komprehensif 14 pilar.

---

### 1. Integrasi Resmi Engine Inti Scintilla 5.6.9 & Lexilla 5.5.5

- **Engine Upstream Scintilla 5.6.9**:
  - Memulihkan perenderan warna penuh untuk gambar ikon XPM.
  - Akselerasi pencarian posisi awal baris dengan melompati komputasi tata letak baris yang berulang.
  - Peningkatan kapasitas cache lexeme dari 30 menjadi 40 byte untuk penanganan identifier panjang dan simbol query SQL/JSON.
  - Pembaruan pemetaan kategori karakter dan konversi kapitalisasi penuh standar **Unicode 17**.
  - Mengatasi crash undo riwayat perubahan yang disebabkan optimasi bulk deletion pada Scintilla 5.3.8.
- **Pustaka Lexer Upstream Lexilla 5.5.5**:
  - Peningkatan keamanan memori lintas lexer: mengatasi celah *out-of-bounds write* (CWE-787) pada DMIS, pembatas here-doc Perl, Raku, TCMD, dan TeX.
  - Mengatasi celah *out-of-bounds read* (CWE-125) pada lexer Ruby dan X12.
  - Mengatasi crash pemanggilan makro preprocessor pada lexer C++.
  - Mengatasi crash rekursi bersarang dalam pada lexer Visual Prolog.
- **Antarmuka Qt yang Diregenerasi**:
  - Regenerasi berkas `ScintillaEdit.cpp` dan `ScintillaEdit.h` melalui skrip resmi `WidgetGen.py`.

---

### 2. Penyorotan Sintaksis Front-Matter Markdown

Pada berkas `src/languages/markdown.lua`, tiga ID style baru dari Lexilla 5.5.5 telah diintegrasikan secara penuh:
- **`SCE_MARKDOWN_FRONT_MARK` (ID 22)**: Gaya visual pembatas metadata (`---` pada YAML, `+++` pada TOML, `{}` pada JSON).
- **`SCE_MARKDOWN_FRONT` (ID 23)**: Gaya visual isi dan nilai properti front-matter.
- **`SCE_MARKDOWN_FRONT_KEY` (ID 24)**: Penyorotan tebal untuk kunci atribut metadata.

---

### 3. Porting Fitur & Stabilitas Terpilih dari NotepadNext

- **Perbaikan Infinite-Loop pada Replace All (CWE-835 / DoS)**:
  - Sumber: NotepadNext commit `ef75daf94613a9931fdd7ecdeb18393f45248129`.
  - Pada `src/Finder.cpp`, memperbaiki hang aplikasi saat mencocokkan regex dengan panjang nol (`^`, `$`, `\b`, lookaround).
  - Menegakkan pergerakan kursor maju melalui `editor->positionAfter()`.
- **Invarian Arah Dialog Cari & Ganti**:
  - Sumber: NotepadNext commit `adfd1280146b917560e1dd98c93e5bd3a31222f2`.
  - Pada `src/dialogs/FindReplaceDialog.cpp`, secara otomatis menonaktifkan dan menghapus centang pencarian arah mundur saat mode Regular Expression dipilih.
- **Pengaturan Bahasa Default untuk Dokumen Baru**:
  - Sumber: NotepadNext commit `be1e274cc3385900bf44c48cbba5c5c6a5299747`.
  - Menambahkan pengaturan `DefaultLanguage` pada Preferensi > Editor (`PreferencesDialog`), terisi otomatis dengan seluruh bahasa yang didukung.
  - Secara otomatis menerapkan bahasa pilihan pengguna saat membuat dokumen baru yang belum disimpan.

---

### 4. Rangkaian Kualitas Kode & Verifikasi 14 Pilar

SparkNotePad v1.3.9 mematuhi standar ketat 14 Pilar Rekayasa Perangkat Lunak:
- Terverifikasi bersih dengan 0 peringatan pada `clangd --check`, `cppcheck`, `clang-tidy`, dan SonarLint.
- Dikompilasi secara ketat menggunakan generator default CMake `Unix Makefiles` (Ninja dilarang permanen per `AGENTS.md`).
- Tingkat kelulusan pengujian 100% pada seluruh unit test di `SparkNotePadTests`.
