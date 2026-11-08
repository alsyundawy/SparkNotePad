# SparkNotePad v1.4.5 — Tumpukan Tab Tertutup, Tag Warna Tab, Pemulihan Crash, Pencarian Lanjutan, Ekspor PDF, Pratinjau Mermaid/LaTeX, Paket Ikon Aksara (38 Koleksi) & Penguatan 14 Pilar

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
7. **Peningkatan Pratinjau Markdown (Diagram Mermaid & Rumus Matematika LaTeX)**:
   - `MarkdownPreviewEnhancer` merender flowchart Mermaid, diagram urutan, diagram kelas, dan state machine via kontainer web terisolasi.
   - Dukungan penuh rumus matematika LaTeX (`$$...$$` dan `$...$`) dengan proteksi simbol mata uang dollar.
   - Fitur Two-Way SyncScroll yang menyelaraskan posisi gulir editor Scintilla dengan viewport pratinjau Markdown.
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
13. **Integrasi Rangkaian Paket Ikon Toolbar Aksara (Total 38 Paket)**:
    - Penambahan 16 paket ikon toolbar Aksara baru (544 file SVG dan PNG vektor) mencakup macOS Big Sur, Windows XP/Vista/10/11, GNOME 51, KDE 6, Ubuntu Yaru, Arch Linux, Elementary OS, dan gaya modern.
    - Terdaftar pada `toolbar_icons.qrc` dan `ToolbarIconManager.cpp` lengkap dengan normalisasi alias ID dan cakupan unit test.
14. **Penuntasan Analisis Statis SonarLint & Clang-Tidy (0 Masalah)**:
    - Menuntaskan seluruh 25 peringatan analisis statis SonarLint/SonarQube dan clang-tidy pada direktori `src/` (`misc-const-correctness`, `misc-override-with-different-visibility`, `bugprone-implicit-widening-of-multiplication-result`, `readability-use-concise-preprocessor-directives`, `readability-braces-around-statements`, `readability-else-after-return`, `performance-unnecessary-value-param`).
    - Terverifikasi 0 diagnostik eror pada `clangd --check`, `cppcheck`, dan lulus 100% unit test.

---

## 📦 Unduhan & Verifikasi Integritas

- `SparkNotePad-v1.4.5-x64.dmg` (macOS Intel x86_64, 33 MB, terverifikasi via `hdiutil`)
- `SparkNotePad-v1.4.5-arm64.dmg` (macOS Apple Silicon ARM64)
- `sparknotepad_1.4.5_amd64.deb` (Debian / Ubuntu x86_64)
- `sparknotepad_1.4.5_arm64.deb` (Debian / Ubuntu ARM64)
- `sparknotepad-1.4.5-1.x86_64.rpm` (Fedora / RHEL x86_64)
- `sparknotepad-1.4.5-1.aarch64.rpm` (Fedora / RHEL ARM64)
