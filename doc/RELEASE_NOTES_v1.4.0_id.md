# Catatan Rilis SparkNotePad v1.4.0 — Spasi Baris Ekstra, Tooltip Tab Diperkaya, Pencarian Fuzzy Aman, Eksekusi Batch Makro, MCP Server & Mitigasi CVE 2025–2026

**Versi Rilis:** `v1.4.0`<br/>
**Tanggal Rilis:** 2026-11-07<br/>
**Maintainer:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Engine Inti:** Scintilla 5.6.9 (Vendored Resmi) & Lexilla 5.5.5 (Vendored Resmi)<br/>
**Runtime Lua:** Sandboxed Lua 5.5.1 (Sumber Resmi)<br/>
**Jembatan Scripting:** Python 3.13+ Engine Bridge<br/>
**Standar:** ISO C++20 / Qt 6.5–6.12<br/>
**Target Platform:** macOS 11.0–26.0 (Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu, Debian, Fedora, RHEL, openSUSE) | FreeBSD 13–15

---

## ⚡ Ringkasan Eksekutif

SparkNotePad **v1.4.0** menghadirkan peningkatan produktivitas penting yang terinspirasi dari riset mendalam terhadap Nextpad++, Notepad--, dan editor modern lainnya, dipadukan dengan mitigasi kerentanan keamanan 2025–2026 dan konektivitas Model Context Protocol (MCP):

1. **Pengaturan Spasi Baris Ekstra (Ascent & Descent)**: Pengaturan ketinggian baris atas dan bawah (0–30 px) pada Preferences > Margins/Padding via API Scintilla `SCI_SETEXTRAASCENT` dan `SCI_SETEXTRADESCENT` dengan propagasi instan ke semua tab editor.
2. **Tooltip Tab Diperkaya dengan Inspeksi Metadata Dokumen**: Tampilan tooltip informatif di `DockedEditor` yang menyajikan path absolut, ukuran berkas di disk, jumlah baris, karakter, encoding teks, format baris baru (CRLF/LF), serta timestamp modifikasi berkas.
3. **Mesin Pencarian Subsekuens Fuzzy Tanpa Backtracking**: Algoritma regex linier di `RegexSafetyEngine` untuk pencarian fuzzy cepat tanpa risiko ReDoS atau pembekuan aplikasi.
4. **Eksekusi Makro Batch Lintas Dokumen Terbuka**: Opsi cakupan eksekusi di `MacroRunDialog` ("Current Document" atau "All Open Documents") dengan dialog proses transparan.
5. **Server MCP Loopback Lokal Berintegritas Tinggi**: Server Model Context Protocol loopback lokal `127.0.0.1` JSON-RPC dengan otentikasi Bearer Token kriptografis (`list_open_tabs`, `read_active_tab`).
6. **Pengerasan Keamanan CVE 2025–2026**:
   - **CVE-2026-20841**: Perlindungan skema URL/URI berbahaya dan pemblokiran eksekusi biner langsung di `NetworkSecurityPolicy`.
   - **CVE-2026-16762**: Pembatasan kedalaman nesting XML (maksimal 32 tingkat) dan batas ukuran 10 MB di `ConfigIntegrityGuard`.
   - **CVE-2026-48778 & CVE-2026-48800**: Pencegahan eskalasi hak istimewa dan penegakan izin berkas konfigurasi ketat 0600.
7. **Salin Info Diagnostik Debug Satu Klik**: Menu Bantuan > Debug Info menyalin seluruh detail konfigurasi runtime, versi Scintilla/Lexilla/Lua, dan platform OS ke clipboard.
8. **Standar Kualitas 14 Pilar**: 100% kelulusan unit test, 0 error linter, dan 0 pelanggaran SonarLint.

---

## 📦 Unduhan & Verifikasi

- `SparkNotePad-v1.4.0-x64.dmg` (macOS Intel x86_64, 33 MB, terverifikasi valid via `hdiutil`)
- `SparkNotePad-v1.4.0-arm64.dmg` (macOS Apple Silicon ARM64)
- `sparknotepad_1.4.0_amd64.deb` (Debian / Ubuntu x86_64)
- `sparknotepad_1.4.0_arm64.deb` (Debian / Ubuntu ARM64)
- `sparknotepad-1.4.0-1.x86_64.rpm` (Fedora / RHEL x86_64)
- `sparknotepad-1.4.0-1.aarch64.rpm` (Fedora / RHEL ARM64)
