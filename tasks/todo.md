# SparkNotePad Actionable Task List

- **Current Version:** `v1.5.0` (Completed & Packaged)
- **Previous Release:** `v1.4.7`
- **Master Plan Document:** [v1.5.0_tasklist.md](v1.5.0_tasklist.md)
- **Detailed Architectural Plan:** [SPARKNOTEPAD_1.5.0_IMPLEMENTATION_AND_TASK_LIST_PLAN.md](../doc/SPARKNOTEPAD_1.5.0_IMPLEMENTATION_AND_TASK_LIST_PLAN.md)
- **Status:** COMPLETED & VERIFIED (Ready for Release v1.5.0)

---

## Release v1.5.0 Actionable Tasks (Completed & Verified)

- [x] **Milestone 1: Scintilla 5.6.9 & Lexilla 5.5.5 Unicode 17 Upgrade**
  - [x] Task 1.1: Audit & Integrasi Kompatibilitas Scintilla 5.6.9 (XPM, bypass layout, 40B lexeme cache, Unicode 17).
  - [x] Task 1.2: Sinkronisasi Lexilla 5.5.5 & Mitigasi Crash Preprocessor C++ (Issue #388).
  - [x] Task 1.3: Sinkronisasi Tabel Karakter Unicode 17 Standar Internasional.
  - [x] Task 1.4: Eliminasi Regresi Pemetaan Warna Gambar XPM Scintilla.
  - [x] Task 1.5: Mitigasi Loop Tak Terbatas Parser Berkas BBEdit (`CVE-2026-82605`).
  - [x] Task 1.6: Pengujian Kompatibilitas Biner & Regresi Scintilla/Lexilla (`tests/test_scintilla_lexilla_upgrade.h`).

- [x] **Milestone 2: Lua 5.5.1 Scripting Engine & Sandboxing Integration**
  - [x] Task 2.1: Peningkatan Mesin Lua ke Versi 5.5.1 resmi.
  - [x] Task 2.2: Isolasi Total Lingkungan Sandbox Lua (`CVE-2026-42214` - strip `os`, `io`, `package`, `debug`).
  - [x] Task 2.3: API Binding Scintilla untuk Ekstensi Lua berkecepatan tinggi.
  - [x] Task 2.4: Dock Konsol Interaktif Lua dengan Penanganan Kesalahan Runtime (`src/docks/LuaConsoleDock.cpp/.h`).
  - [x] Task 2.5: Pengujian Unit VM Lua 5.5.1 & Uji Penetrasi Sandbox (`tests/test_lua_sandbox.h`).

- [x] **Milestone 3: Proteksi Total 69 Toolbar Icon Pack & Jaminan Zero-Regression**
  - [x] Task 3.1: Audit Kunci Integritas Ke-69 Direktori Icon Pack yang Sudah Ada (LOCKED: Zero Regression).
  - [x] Task 3.2: Refaktorisasi Non-Breaking `ToolbarIconManager` untuk Dynamic Lazy-Loading & Retina Scaling.
  - [x] Task 3.3: Dialog Kustomisasi Toolbar Interaktif (`CustomizeToolbarDialog`).
  - [x] Task 3.4: Sanitasi Entitas Vektor SVG Terhadap Kerentanan Qt (`CVE-2025-14576`).
  - [x] Task 3.5: Fallback Graceful Non-Intrusif untuk Aksi Menu Baru tanpa Mengganggu Icon Pack Aktif.
  - [x] Task 3.6: Suite Pengujian Unit Integritas Icon Pack (`tests/test_toolbar_icon_manager.h`).

- [x] **Milestone 4: Proteksi 25 Theme Families & Mandat Permanen Dark Toolbar**
  - [x] Task 4.1: Audit Kunci Integritas 25 Keluarga Tema / 75 Varian Tema Aktif (LOCKED: Zero Modification).
  - [x] Task 4.2: Penegakan Permanen Dark Mode Toolbar (`#mainToolBar` berlatar `#21252b` di SEMUA tema & mode).
  - [x] Task 4.3: BGR Endianness Safety & Scintilla Buffer Color Conversion (`scColor`).
  - [x] Task 4.4: Dialog Pemilih Tema Cepat (`ThemeSwitcherDialog`, `Ctrl+Shift+T`).
  - [x] Task 4.5: Pengujian Unit Kepatuhan Kontras WCAG 2.1 & Integritas Tema (`tests/test_theme_manager.h`).

- [x] **Milestone 5: Live Tail Monitoring Engine (`tail -f`) & File Utilities**
  - [x] Task 5.1: Mesin Pemantauan Berkas Live Tail (`TailMonitoringManager.cpp/.h`, `Ctrl+Alt+Y`).
  - [x] Task 5.2: Logika Pengguliran Cerdas (Smart Auto-Scroll vs User Selection).
  - [x] Task 5.3: Dialog Opsi Live Tail (`TailOptionsDialog.cpp/.h`).
  - [x] Task 5.4: Aksi Perbarui Waktu Berkas (`File -> Touch / Update Timestamp` via `utime`).
  - [x] Task 5.5: Supresi Dialog Inconsistent Line Endings Saat Background Reload.
  - [x] Task 5.6: Suite Pengujian Live Tail Monitoring (`tests/test_tail_monitoring.h`).

- [x] **Milestone 6: Military-Grade Cryptography (`AES-256-CBC` + `HMAC-SHA256`)**
  - [x] Task 6.1: Implementasi Kontainer Kriptografis `.sparkenc` (Header, Salt, IV, PBKDF2 100k, HMAC).
  - [x] Task 6.2: Dialog Enkripsi & Dekripsi Berkas Interaktif (`EncryptDialog`, `DecryptDialog`).
  - [x] Task 6.3: Pembersihan Memori Pasca-Kriptografi (`Secure Zeroization`) via `OPENSSL_cleanse`.
  - [x] Task 6.4: Indikator Status Bar Status Enkripsi & Proteksi Autosave Berkas Terenkripsi.
  - [x] Task 6.5: Suite Pengujian Kriptografis AES-256 (`tests/test_crypto_manager.h`).

- [x] **Milestone 7: Developer Tools Engine & GitHub Markdown Preview Suite**
  - [x] Task 7.1: Mesin HTML Formatter & Beautifier (`DeveloperToolsEngine::formatHtml`).
  - [x] Task 7.2: Mesin HTML Minifier (`DeveloperToolsEngine::minifyHtml`).
  - [x] Task 7.3: Mesin Enkripsi & Obfuscation HTML (JS Escape, Base64, Password-Protected AES-GCM).
  - [x] Task 7.4: Mesin Dekripsi & De-obfuscation HTML (`DeveloperToolsEngine::decryptHtml`).
  - [x] Task 7.5: Mesin CSS Formatter & Beautifier (`DeveloperToolsEngine::formatCss`).
  - [x] Task 7.6: Mesin CSS Minifier (`DeveloperToolsEngine::minifyCss`).
  - [x] Task 7.7: Mesin JavaScript Formatter & Beautifier (`DeveloperToolsEngine::formatJs`).
  - [x] Task 7.8: Mesin JavaScript Minifier (`DeveloperToolsEngine::minifyJs`).
  - [x] Task 7.9: Mesin XML Formatter & Beautifier (`DeveloperToolsEngine::formatXml`).
  - [x] Task 7.10: Mesin XML Minifier (`DeveloperToolsEngine::minifyXml`).
  - [x] Task 7.11: Inspektur & Decoder Token JWT (`DeveloperToolsEngine::decodeJwt`).
  - [x] Task 7.12: RFC 2045 Quoted-Printable Encoder & Decoder (`MimeToolsEngine`).
  - [x] Task 7.13: Base64 Line-by-Line Encoder & Decoder (`MimeToolsEngine`).
  - [x] Task 7.14: HTML Entity Encoder & Decoder (`MimeToolsEngine`).
  - [x] Task 7.15: Konverter Teks ASCII <-> Hex String (`ConverterToolsEngine`).
  - [x] Task 7.16: Konverter Multi-Radix / Basis Bilangan (Bin, Oct, Dec, Hex) (`ConverterToolsEngine`).
  - [x] Task 7.17: Suite Konversi Kasus Huruf Lanjutan (Title Case, Invert Case, Sentence Case).
  - [x] Task 7.18: Evaluator Kekuatan Password & Entropi Kriptografis (`CryptoToolsEngine`).
  - [x] Task 7.19: Dialog Workbench Developer Tools Terpadu (`DeveloperToolsDialog` - Tabbed Hub).
  - [x] Task 7.20: Evaluator JavaScript Terisolasi (`JsEvaluator.cpp/.h`, `Ctrl+Shift+J`).
  - [x] Task 7.21: Suite Pengujian Regular Expression Interaktif (`RegexTesterDialog.cpp/.h`).
  - [x] Task 7.22: Evaluator Matematika Sebaris Dokumen (`InlineMathEvaluator.cpp/.h`).
  - [x] Task 7.23: Color Preview Call-Tips pada Hover Nilai Warna (`ColorPreviewCallTip.cpp/.h`).
  - [x] Task 7.24: Dialog Penampil Perbedaan Teks Visual (`DiffViewerDialog.cpp/.h`).
  - [x] Task 7.25: Suite Pengujian Komprehensif Developer Tools Engine (`tests/test_developer_tools_engine.h`).
  - [x] Task 7.26: Studi & Integrasi Desain Sistem GitHub Primer (`vscode-github-markdown-preview-style` by `mjbvz`).
  - [x] Task 7.27: Rekayasa Ulang Pipeline Render Markdown (`MarkdownPreviewEnhancer` dengan pembersih tag mandiri & token masking).
  - [x] Task 7.28: Penerapan Styling Tema GitHub Lengkap (Light, Dark, Dark Dimmed & High Contrast).
  - [x] Task 7.29: Dukungan Penuh Fitur Lanjutan GFM (GitHub Alerts, Code Highlighting, Tables, Tasklists, Math, Mermaid).
  - [x] Task 7.30: Arsitektur Native Qt Markdown Engine dengan GitHub Primer Styling (`MarkdownPreviewEnhancer` & `MarkdownThemeManager`).
  - [x] Task 7.31: Eliminasi Menyeluruh Fungsi & Tombol "Open in Default Web Browser" di Seluruh Aplikasi.
  - [x] Task 7.32: Pengujian Unit & Regresi Visual Markdown Preview Engine (`tests/test_markdown_preview_engine.h`).

- [x] **Milestone 8: Remote File Browser Dock (`SFTP` / `SCP` / `FTPS`)**
  - [x] Task 8.1: Implementasi `RemoteFileBrowserDock.cpp/.h` (Arsitektur SparkleSFTP).
  - [x] Task 8.2: Mesin Caching Lokal Terisolasi dengan Izin POSIX `0600`.
  - [x] Task 8.3: Unggah Otomatis Atomik saat Berkas Disimpan.
  - [x] Task 8.4: Fortifikasi Pipeline Transfer Melawan Zip-Slip & Path Traversal (`CVE-2026-57233`).
  - [x] Task 8.5: Penyimpanan Kredensial Aman via OS Keychain.

- [x] **Milestone 9: Window Menu, View Toggles & Ergonomic Menu Bar Relocation**
  - [x] Task 9.1: Pembaruan Lengkap Menu Window dari NotepadMac (`Minimize`, `Zoom`, `Center`, `Move Screens`, `Bring to Front`).
  - [x] Task 9.2: Navigasi Tab Dokumen Cepat (`Next Tab`, `Previous Tab`, `Focus Editor`).
  - [x] Task 9.3: Daftar Dokumen Dinamis Menu Window dengan Indikator Aktif (`✓`) dan Dirty Marker (`*`).
  - [x] Task 9.4: Porting Aksi Toggle Tampilan dari NotepadMac (Line Numbers, Folding, White Space & EOL, Indent Guides, Caret Line, Column Marker, Word Wrap, Read-Only).
  - [x] Task 9.5: Kontrol Pembesaran Tampilan / Zooming Standar macOS (`Zoom In`, `Zoom Out`, `Reset Zoom`).
  - [x] Task 9.6: Integrasi macOS Finder Quick Actions & Services (dari `macOS-NewFile-QuickActions/`).
  - [x] Task 9.7: Mesin Migrasi Pengaturan Otomatis NotepadNext (`SettingsMigrationManager`).
  - [x] Task 9.8: Integrasi & Distribusi `SparkNotePad-Default-Editor.sh` (macOS Takeover Engine).
  - [x] Task 9.9: Integrasi & Distribusi `SparkNotePad-Default-Editor-Linux.sh` (5-Layer Integration).
  - [x] Task 9.10: Suite Pengujian Unit Menu Window & Toggle View (`tests/test_window_view_menus.h`).
  - [x] Task 9.11: Relokasi Menu Encoding ke Submenu Format (`Format -> File Encoding` & `Line Endings`).
  - [x] Task 9.12: Relokasi Menu Run ke Submenu Tools (`Tools -> Run & External Execution` - browser eksternal dihapus).
  - [x] Task 9.13: Standardisasi Sistem Shortcut & Resolusi Konflik Keyboard Global (`ShortcutCustomizerDialog`).

- [x] **Milestone 10: 2025–2026 CVE/GHSA Threat Mitigation & Hardening**
  - [x] Task 10.1: Mitigasi Supply-Chain & Update Metadata Hijacking (`CVE-2025-15556`).
  - [x] Task 10.2: Mitigasi Binary Planting & Unsafe Search Path (`CVE-2025-49144` & `CWE-426`).
  - [x] Task 10.3: Proteksi Integritas Direktori Plugin (`CVE-2025-56383`).
  - [x] Task 10.4: Sanitasi Format String Status Bar & Logging (`CVE-2026-3008` & `CVE-2026-6539`).
  - [x] Task 10.5: Mitigasi Arbitrary Config Code Execution (`CVE-2026-48800`, `48778`, `48770`, `52884`).
  - [x] Task 10.6: Mitigasi Markdown URI Handler Remote Code Execution (`CVE-2026-20841`).
  - [x] Task 10.7: Mitigasi Kerentanan Parser XML & SVG Qt 6 (`CVE-2026-105925` & `CVE-2026-8168`).
  - [x] Task 10.8: Suite Pengujian Keamanan Siber v1.5.0 (`tests/test_v150_security.h`).

- [x] **Milestone 11: Static Analysis, Linter & 14-Pillar Quality Gate**
  - [x] Task 11.1: Audit Linter Menyeluruh Direktori Periferal Proyek (`scripts/*`, `doc/*`, `tools/*`, `wiki/*`, `tests/*`).
  - [x] Task 11.2: Audit AST `clangd --check` pada Seluruh `src/*.cpp` dan `src/*/*.cpp` dengan Auto-Remediasi Instan.
  - [x] Task 11.3: Analisis Statis Mendalam `cppcheck --enable=all --error-exitcode=1` dengan Auto-Remediasi Instan.
  - [x] Task 11.4: Gerbang Kualitas SonarLint IDE & Scan Sonar-Scanner (0 Blocker, 0 Critical, 0 Major).
  - [x] Task 11.5: Audit Modern C++20 `clang-tidy -p build` (Google C++ Style & Safety Checks).
  - [x] Task 11.6: Investigasi Web Tools Final & Pemindaian Kerentanan Dependensi Mutakhir.
  - [x] Task 11.7: Protokol Eksekusi Review Kode 14 Pilar Berbasis Checklist untuk Seluruh Berkas Repositori.

- [x] **Milestone 12: CMake Build, DMG Packaging, Credits, i18n & Docs/Wiki**
  - [x] Task 12.1: Konfigurasi CMake Standar (Unix Makefiles - Strictly No Ninja, `-j2`).
  - [x] Task 12.2: Eksekusi Kompilasi C++20 & Biner Stripping (`strip -x`).
  - [x] Task 12.3: Eksekusi Test Suite Terotomasi (`ctest` - 100% lulus).
  - [x] Task 12.4: Packaging DMG macOS via `scripts/package-dmg.sh` (Plafon Ketat 30MB–40MB, `.DS_Store` preset).
  - [x] Task 12.5: Pembaruan Kredit Pengembang, Copyright & Atribusi Upstream Komprehensif.
  - [x] Task 12.6: Sinkronisasi Penuh i18n Bahasa Bilingual (`i18n/SparkNotePad_{id,en}.ts` & `.qm`).
  - [x] Task 12.7: Pembaruan Menyeluruh `README.md`, `README-id.md`, User Guide & Wiki Proyek.
  - [x] Task 12.8: Verifikasi Akhir Gate Rilis & Pre-Flight Checklist.
  - [x] Task 12.9: Atomic Git Commit Format Konvensional.
