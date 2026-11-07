# Catatan Rilis SparkNotePad — v1.3.9

**Versi Rilis:** `v1.3.9`<br/>
**Tanggal Rilis:** 07 November 2026<br/>
**Pengelola Utama:** Harry Dertin Sutisna Alsyundawy ([@alsyundawy](https://github.com/alsyundawy))<br/>
**Organisasi:** ALSYUNDAWY IT SOLUTION ([https://alsyundawy.com](https://alsyundawy.com))<br/>
**Repositori:** [https://github.com/alsyundawy/SparkNotePad](https://github.com/alsyundawy/SparkNotePad)<br/>
**Lisensi:** GNU General Public License v3.0 (`GPL-3.0-or-later`)

---

## 🌟 Yang Baru di v1.3.9

SparkNotePad v1.3.9 merupakan rilis stabilitas dan peningkatan engine inti fundamental yang menghadirkan pembaruan upstream resmi ke **Scintilla 5.6.9** dan **Lexilla 5.5.5**, mengatasi kasus khusus (_edge cases_) berbahaya pada pencarian dan penggantian ekspresi reguler, memperkenalkan pengaturan bahasa default yang dapat dikonfigurasi pengguna untuk tab/dokumen baru, memperkaya penyorotan sintaksis front-matter Markdown, serta lolos audit kualitas kode dan analisis statis komprehensif 14 pilar.

---

### 1. Integrasi Resmi Engine Inti Scintilla 5.6.9 & Lexilla 5.5.5

- **Engine Upstream Scintilla 5.6.9**:
  - Memulihkan perenderan warna penuh untuk gambar ikon XPM.
  - Akselerasi pencarian posisi awal baris dengan melompati komputasi tata letak baris yang berulang.
  - Peningkatan kapasitas cache lexeme dari 30 menjadi 40 byte untuk penanganan identifier panjang dan simbol query SQL/JSON.
  - Pembaruan pemetaan kategori karakter dan konversi kapitalisasi penuh standar **Unicode 17**.
  - Mengatasi crash undo riwayat perubahan yang disebabkan optimasi bulk deletion pada Scintilla 5.3.8.
- **Pustaka Lexer Upstream Lexilla 5.5.5**:
  - Peningkatan keamanan memori lintas lexer: mengatasi celah _out-of-bounds write_ (CWE-787) pada DMIS, pembatas here-doc Perl, Raku, TCMD, dan TeX.
  - Mengatasi celah _out-of-bounds read_ (CWE-125) pada lexer Ruby dan X12.
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

### 4. Pembaruan Dialog About & Dinamisasi Metadata

- **Dinamisasi Versi Tag Rilis**: Pada `src/dialogs/AboutDialog.cpp`, badge dan tautan Release Tag kini diikat secara dinamis ke konstanta `APP_VERSION`, menjamin tautan rilis selalu tepat mengarah ke `v1.3.9` tanpa teks tag usang.
- **Pembaruan Label Mesin Inti**: Menyelaraskan teks mesin penyunting menjadi Scintilla 5.6.9 & Lexilla 5.5.5 pada tab About, tab Pengelola (Maintainer), dan kartu Diagnostik.
- **Salin Info Sistem**: Menyelaraskan fungsi salin informasi sistem (clipboard system info) ke Scintilla 5.6.9 dan Lexilla 5.5.5.
- **Atribusi Repositori Leluhur**: Mengakui secara eksplisit `NotepadNext-MacOS` (`@alsyundawy`) sebagai repositori port fondasional macOS tempat SparkNotePad berakar dan berkembang.

---

### 5. Kompatibilitas Penuh Qt 6.2 LTS (macOS Catalina & Ubuntu 20.04 LTS)

- **API Zona Waktu Universal**: Mengganti `QTimeZone::UTC` dengan `QTimeZone::utc()` pada `src/utils/TokenForensicUtils.cpp`, memulihkan kompatibilitas penuh lintas Qt 6.2+.
- **Kompatibilitas String**: Mengganti `QLatin1StringView` menjadi `QLatin1String` pada `src/dialogs/MainWindow.cpp` dan `src/widgets/TabsQuickActionsBar.cpp`.
- **Pembersihan Peringatan Compiler**: Menghapus specifier `inline` redundan pada `src/widgets/TabsQuickActionsBar.h`.
- **Fallback Penguraian Warna**: Mengimplementasikan fungsi pembantu `parseColor` pada `src/features/ThemeManager.cpp` sebagai fallback aman untuk `QColor::fromString` saat dikompilasi pada Qt < 6.4.
- **Pemagaran Kriptografi**: Memagari API kriptografi Qt 6.4+ pada `src/utils/HashIntegrityManager.cpp` menggunakan `QT_VERSION_CHECK(6, 4, 0)`.

---

### 6. Matriks CI Multi-Arsitektur & Publikasi Rilis Otomatis

- **Paket Native Linux ARM64**: Menambahkan packaging biner native Linux ARM64 (`aarch64`) untuk distribusi Ubuntu, Debian, dan RPM pada matriks Qt 6.8, 6.10, 6.11, dan 6.12.
- **Pemulihan Runner Legacy**: Mengaktifkan kembali kompilasi Qt 6.2 LTS pada runner macOS Catalina dan Ubuntu 20.04.
- **Akselerasi Distribusi**: Mengutamakan job x64 dan menerbitkan aset rilis seketika saat job kompilasi selesai.
- **Standar Rilis Kanonikal**: Mematok target build rilis dan artefak default ke Qt 6.5 LTS.
- **Cakupan Distribusi**: Memperluas katalog distribusi biner hingga 50+ paket.

---

### 7. Penyempurnaan Packaging macOS DMG & Perampingan Framework

- **Tata Letak Grafis Resmi**: Menghilangkan fallback unstyled `--skip-jenkins` di CI sehingga layout DMG selalu bergaya resmi (`dmg-background.png`, ikon volume kustom, tautan Applications, koordinat jendela 680x440).
- **Pembersihan Framework Bloatware**: Memangkas framework Qt yang tidak terpakai (`QtPdf`, `QtQml`, `QtQuick`, `QtVirtualKeyboard`), menghemat ~15 MB.
- **Verifikasi DMG Lokal**: Pembuatan berkas `build/SparkNotePad-v1.3.9-x64.dmg` (33 MB) berhasil dibuat dan lolos verifikasi integritas checksum `hdiutil`.

---

### 8. Paritas Terjemahan Dwibahasa 100% (i18n)

- Penyelarasan penuh katalog terjemahan Bahasa Indonesia (`SparkNotePad_id.ts`) dan Bahasa Inggris (`SparkNotePad_en.ts`).
- Kompilasi melalui `lrelease`: tepat 1.850 entri terjemahan selesai, 0 belum selesai.

---

### 9. Rangkaian Kualitas Kode & Verifikasi 14 Pilar

SparkNotePad v1.3.9 mematuhi standar ketat 14 Pilar Rekayasa Perangkat Lunak:

- Terverifikasi bersih dengan 0 peringatan pada `clangd --check`, `cppcheck` (152 berkas `src/`), `clang-tidy`, dan SonarLint.
- Dikompilasi secara ketat menggunakan generator default CMake `Unix Makefiles` (Ninja dilarang permanen per `AGENTS.md`).
- Tingkat kelulusan pengujian 100% pada seluruh unit test di `SparkNotePadTests` (51/51 lolos).
- Paritas 100% modul Python 3.14 dan 3.15 (73 modul identik).
