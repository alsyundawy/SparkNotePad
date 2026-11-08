# SparkNotePad v1.4.5 — Tumpukan Tab Tertutup, Tag Warna Tab, Pemulihan Crash, Pencarian Lanjutan, Ekspor PDF, Pratinjau Mermaid/LaTeX, Paket Ikon Aksara (28 Koleksi) & Penguatan 14 Pilar

**Versi Rilis:** `v1.4.5`<br/>
**Tanggal Rilis:** 2026-11-08<br/>
**Pengelola Utama:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Mesin Penyunting Inti:** Scintilla 5.6.9 (Vendored Resmi) & Lexilla 5.5.5 (Vendored Resmi)<br/>
**Runtime Lua Tersemat:** Sandboxed Lua 5.5.1 (Sumber Resmi)<br/>
**Runtime Skrip:** Python 3.13+ Engine Bridge<br/>
**Standar:** ISO C++20 / Qt 6.5–6.12<br/>
**Target Platform:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu 22.04–26.04, Debian 12–14, Fedora 40–44, RHEL/Rocky 9–11, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x (KDE Plasma 6 & XFCE)

---

## ⚡ Ringkasan Eksekutif

SparkNotePad **v1.4.5** merupakan rilis fitur dan stabilitas besar yang menghadirkan 14 pilar arsitektur fundamental yang dirancang khusus untuk para pengembang profesional, administrator sistem, dan pengguna tingkat lanjut:

1. **Tumpukan Tab Tertutup & Buka Kembali Tab (`Ctrl+Shift+T` / `Cmd+Shift+T`)**:
   - `ClosedTabsManager` mengelola antrean FIFO 50 tab yang baru saja ditutup, mempertahankan posisi kursor, rasio gulir, encoding teks, mode EOL, sintaksis bahasa, dan tag warna tab.
   - Aksi pemulihan instan tersedia pada menu konteks tab dan menu Berkas.
2. **Pemberian Tag & Pengelompokan Warna Tab**:
   - `TabColorManager` menyediakan 8 preset warna berstandar aksesibilitas tinggi (Merah, Oranye, Kuning, Hijau, Sian, Biru, Ungu, Magenta) dengan status persisten melalui `ApplicationSettings`.
3. **Manajer Autosave & Pemulihan Crash (Interval 10 Detik, Izin Ketat POSIX 0600)**:
   - `AutosaveRecoveryManager` menyimpan cadangan latar belakang setiap 10 detik untuk dokumen yang dimodifikasi, diamankan dengan izin POSIX `0600` dan checksum integritas SHA-256. Dialog interaktif `CrashRecoveryDialog` memungkinkan pemulihan selektif saat aplikasi dijalankan kembali.
4. **Mesin Karakter Escape Pencarian Lanjutan (Extended Search)**:
   - `ExtendedSearchParser` memproses `\xHH` (byte heksadesimal), `\dDDD` (byte desimal), `\0` (byte NUL), `\b` (backspace), `\f` (form-feed), dan `\v` (tab vertikal) pada dialog Cari/Ganti dan Bilah Pencarian Cepat.
5. **Audit 100+ Lexer & Ekspansi Bahasa Modern (Total 129 Bahasa)**:
   - Menambahkan lexer native baru: Crystal (`.cr`), Cangjie (`.cj`), LLVM IR (`.ll`), dan JSON5 (`.json5`).
   - Memodernisasi kata kunci bahasa: C23 (`_BitInt`, `constexpr`, `nullptr`), Rust 2024 (`gen`, `async gen`), Swift 6 (`consuming`, `borrowing`, `sending`), dan TypeScript 5.8.
6. **Alur Ekspor Native Markdown / HTML / PDF**:
   - Implementasi `DocumentExportManager` menggunakan `QPrinter` / `Qt6::PrintSupport` native dan fallback Pandoc.
   - Dialog interaktif `ExportOptionsDialog` mendukung ukuran kertas kustom (A4, Letter, Legal), orientasi, margin, dan CSS tersemat.
7. **Peningkatan Pratinjau Markdown & HTML (30 Tema, Format Kompiler markdown-it, Diagram Mermaid & Rumus LaTeX)**:
   - Integrasi 30 tema CSS classless (`GITHUB`, `GITHUB-DARK`, `ALMOND`, `AWSM`, `AXIST`, `BAMBOO`, `BULLFRAME`, `HOLIDAY`, `KACIT`, `LATEX`, `MARX`, `MINI`, `MODEST`, `NEW`, `NO-CLASS`, `PICO`, `RETRO`, `SAKURA`, `SAKURA-VADER`, `SEMANTIC`, `SIMPLE`, `STYLE-SANS`, `STYLE-SERIF`, `STYLIZE`, `SUPERSTYLIN`, `TACIT`, `VANILLA`, `WATER`, `WATER-DARK`, `WRIT`) berbasis `cleanrmd` / `simov/markdown-viewer`.
   - Regex deteksi berkas komprehensif: `\.(?:markdown|mdown|mkdn|md|mkd|mdwn|mdtxt|mdtext|text)(?:#.*|\?.*)?$` untuk Markdown dan `\.(?:html|htm|shtml|xhtml|xht|dhtml)(?:#.*|\?.*)?$` untuk HTML.
   - Dukungan format standar `markdown-it`: tabel GFM, daftar tugas interaktif (`- [ ]`, `- [x]`), coret teks (`~~teks~~`), sorot teks (`==teks==`), subskrip (`~teks~`), superskrip (`^teks^`), sisip teks (`++teks++`), catatan kaki (`[^1]`), blok kode terlindungi, diagram Mermaid, dan formula matematika LaTeX.
   - Two-Way SyncScroll yang menyelaraskan posisi gulir editor Scintilla dengan viewport pratinjau.
8. **Paket Peralatan Pengembang (Developer Tools)**:
   - `DeveloperToolsEngine` pada menu Peralatan: JSON Prettify & Minify, URL Encode & Decode, Unicode Escape & Unescape, serta generator UUID v4.
   - Dialog interaktif `TimestampConverterDialog` untuk konversi epoch Unix ke format UTC dan ISO Lokal.
9. **Penanda Navigasi Bilah Gulir & Pengalih Header/Source (`F4` / `Alt+O`)**:
   - `CorrespondingFileSwitcher` mendukung peralihan cepat dua arah antara berkas header C/C++ (`.h`, `.hpp`, `.hxx`) dan sumber (`.c`, `.cpp`, `.cxx`), serta berkas kode dan tes TypeScript/JavaScript (`.test.ts`, `.spec.ts`).
   - `ScrollbarNavigationMarkers` menampilkan bookmark dan sorotan hasil pencarian pada track bilah gulir.
10. **Parser & Penegakan Otomatis `.editorconfig`**:
    - `EditorConfigParser` mendukung aturan `indent_style`, `indent_size`, `tab_width`, `end_of_line`, dan penelusuran hierarki `root = true` secara otomatis saat berkas dibuka.
11. **Dock Riwayat Clipboard & Info Karakter Unicode**:
    - Dock `ClipboardHistoryDock` mencatat 20 riwayat salinan terakhir, pencarian cepat, dan penyisipan satu klik ke editor.
    - Dock `CharacterInfoDock` menyajikan pratinjau glyph Unicode real-time, kode `U+XXXX`, nama karakter, kategori, nilai heksadesimal UTF-8/16/32, dan entitas HTML.
12. **Pengalih Cepat Fuzzy (`Cmd+P` / `Ctrl+P`) & Dock Minimap Dokumen**:
    - Dialog frameless bergaya Spotlight `QuickSwitcherDialog` dengan pencocokan subsekuens fuzzy pada tab terbuka dan berkas terkini.
    - `DocumentMinimapDock` menyediakan tinjauan dokumen ringan dengan pelacakan viewport dan batas aman berkas besar (>50 MB).
    - Integrasi berkas terkini native macOS Dock via `MacServicesHandler::updateDockRecentMenu`.
13. **Integrasi Rangkaian Paket Ikon Toolbar Aksara (Total 28 Paket) & Ergonomi Menu**:
    - Penataan 10 paket ikon toolbar Aksara kurasi (340 file SVG dan PNG vektor) mencakup macOS Big Sur, Windows XP/Vista/10/11, Ubuntu Yaru, Arch Linux, Elementary OS, dan gaya modern (total 28 paket bawaan).
    - Relokasi menu Themes: Toolbar Icon Size ditempatkan di atas Toolbar Icon Packs, memindahkan menu Kustomisasi Toolbar ke menu Tools, dan menghapus pintasan folder custom pack dari menubar.
    - Terdaftar pada `toolbar_icons.qrc` dan `ToolbarIconManager.cpp` lengkap dengan normalisasi alias ID dan cakupan unit test.
14. **Penuntasan Analisis Statis SonarLint & Clang-Tidy (0 Masalah)**:
    - Menuntaskan seluruh 25 peringatan analisis statis SonarLint/SonarQube dan clang-tidy pada direktori `src/` (`misc-const-correctness`, `misc-override-with-different-visibility`, `bugprone-implicit-widening-of-multiplication-result`, `readability-use-concise-preprocessor-directives`, `readability-braces-around-statements`, `readability-else-after-return`, `performance-unnecessary-value-param`).
    - Terverifikasi 0 diagnostik eror pada `clangd --check`, `cppcheck`, dan lulus 100% unit test.
15. **Invarian Bilah Alat Mode Gelap Permanen di Semua Tema & Mode**:
    - Bilah alat utama (`#mainToolBar`, `QToolBar`) dikunci secara permanen pada latar belakang gelap (`#21252b !important`) di seluruh tema warna, mode terang/gelap, dan setelan tampilan OS.
    - Mencegah ikon pastel dan kromatik ditampilkan pada latar belakang putih, diperkuat dengan `setAutoFillBackground(true)` pada `#mainToolBar` untuk mencegah tembus pandang background Cocoa native atau jendela terang.
16. **Integrasi Editor Default Tipe Media IANA Kanonikal (100+ Format)**:
    - Skrip `SparkNotePad-Default-Editor.sh` dan skrip Linux diperluas sesuai registri resmi IANA Media Types.
    - Menghubungkan secara mulus 100+ ekstensi berkas kode sumber, format data konfigurasi, bahasa pemrograman sistem, dan dokumen web dengan SparkNotePad.
17. **Standar Paket DMG Ringan Permanen (30MB–34MB, Batas Maksimal < 35MB)**:
    - Bundel DMG rilis macOS melalui `scripts/package-dmg.sh` dijaga ketat pada rentang 30MB–34MB (di bawah batas absolut 35MB).
    - Mempertahankan tata letak presisi `.DS_Store` (`dmg-ds-store`), aset grafis latar belakang Retina `@2x` (`.background/dmg-background@2x.png`), dan ikon volume kustom (`.VolumeIcon.icns`).

---

## 📦 Unduhan & Verifikasi Integritas

- `SparkNotePad-v1.4.5-x64.dmg` (macOS Intel x86_64, 33 MB, terverifikasi via `hdiutil`)
- `SparkNotePad-v1.4.5-arm64.dmg` (macOS Apple Silicon ARM64)
- `sparknotepad_1.4.5_amd64.deb` (Debian / Ubuntu x86_64)
- `sparknotepad_1.4.5_arm64.deb` (Debian / Ubuntu ARM64)
- `sparknotepad-1.4.5-1.x86_64.rpm` (Fedora / RHEL x86_64)
- `sparknotepad-1.4.5-1.aarch64.rpm` (Fedora / RHEL ARM64)
