# Welcome to the SparkNotePad Documentation Wiki

<p align="center">
  <b><a href="#-english-documentation">English</a></b> &nbsp;|&nbsp; <b><a href="#-dokumentasi-bahasa-indonesia">Bahasa Indonesia</a></b>
</p>

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad/releases/latest"><img src="https://img.shields.io/badge/Release-v1.3.7-2ea44f?style=for-the-badge&logo=github&logoColor=white" alt="Latest Release v1.3.7"></a>
  <a href="https://github.com/alsyundawy/SparkNotePad"><img src="https://img.shields.io/badge/Platform-macOS%20%7C%20Linux%20%7C%20FreeBSD-000000?style=for-the-badge&logo=apple&logoColor=white" alt="Platforms"></a>
  <a href="https://isocpp.org"><img src="https://img.shields.io/badge/Standard-ISO%20C%2B%2B20-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++20"></a>
  <a href="https://qt.io"><img src="https://img.shields.io/badge/Framework-Qt%206.5–6.12-41CD52?style=for-the-badge&logo=qt&logoColor=white" alt="Qt 6"></a>
  <a href="https://www.scintilla.org"><img src="https://img.shields.io/badge/Engine-Scintilla%205.6.7%20%2B%20Lexilla%205.5.4-8957e5?style=for-the-badge" alt="Scintilla"></a>
</p>

---

## 🇬🇧 English Documentation

**SparkNotePad** is a fast, lightweight native text and source code editor engineered with **Qt 6** and the **Scintilla / Lexilla** editing engine. Built for power users, developers, sysadmins, and writers across **macOS**, **Linux**, and **FreeBSD**, SparkNotePad delivers sub-millisecond line rendering, military-grade document encryption, and zero-telemetry privacy.

### 📚 Table of Contents

1. **[[01. Getting Started & Installation|01-Getting-Started]]**: Package downloads, macOS DMG installation, Ubuntu/Debian `.deb`, Fedora/RHEL `.rpm`, FreeBSD port, and default editor configuration.
2. **[[02. Feature Overview|02-Feature-Overview]]**: Scintilla canvas, Omnibar Command Palette (`Cmd+P`), side-by-side visual diff (`FileDiffDialog`), project workspace tree dock, and multi-selection.
3. **[[03. User Interface & Themes|03-User-Interface-and-Themes]]**: 75 curated developer themes across 25 families meeting WCAG 2.1 AA/AAA contrast, docking panels, and monoline vector SVG icons.
4. **[[04. Military-Grade Document Encryption|04-Military-Grade-Encryption]]**: Authenticated AES-256-CBC encryption with PBKDF2 100k iterations and HMAC-SHA256 integrity verification.
5. **[[05. Code Transformation & Forensics|05-Code-Transformation-Forensics]]**: XML/HTML and SQL formatters, JSON/CSS/JS minifiers, JWT token inspection, and Shannon Entropy document statistics.
6. **[[06. Keyboard Shortcuts Quick Reference|06-Keyboard-Shortcuts]]**: Comprehensive cross-platform hotkeys for macOS, Linux, and Windows.
7. **[[07. Platform Support Matrix|07-Platform-Support-Matrix]]**: Operating system compatibility matrix spanning macOS 11–26 (Apple Silicon & Intel), Ubuntu, Debian, Fedora, openSUSE, and FreeBSD.
8. **[[08. Building from Source|08-Building-from-Source]]**: Compiling SparkNotePad natively using CMake, Ninja, and Qt 6.

---

## 🇮🇩 Dokumentasi Bahasa Indonesia

**SparkNotePad** adalah editor teks dan kode sumber native yang ringan, gegas, dan tangguh, dibangun menggunakan pustaka **Qt 6** dan mesin penyunting **Scintilla / Lexilla**. Dirancang untuk pengembang perangkat lunak, administrator sistem, dan penulis di lingkungan **macOS**, **Linux**, dan **FreeBSD** dengan perenderan teks instan, enkripsi dokumen standar militer, serta privasi mutlak tanpa telemetri.

### 📖 Daftar Isi

1. **[[01. Memulai & Panduan Pemasangan|01-Memulai-SparkNotePad]]**: Unduhan paket biner resmi, DMG macOS, paket Debian/Ubuntu `.deb`, Fedora/RHEL `.rpm`, FreeBSD, serta skrip editor default.
2. **[[02. Ikhtisar Fitur Lengkap|02-Ikhtisar-Fitur]]**: Mesin Scintilla, Omnibar Command Palette (`Cmd+P`), komparasi file berdampingan (`FileDiffDialog`), docking proyek, dan multi-kursor.
3. **[[03. Antarmuka Pengguna & Tema|03-Antarmuka-dan-Tema]]**: 75 tema sintaks dalam 25 rumpun berstandar aksesibilitas WCAG 2.1 AA/AAA, panel modular ADS, dan ikon vektor SVG.
4. **[[04. Enkripsi Dokumen Standar Militer AES-256|04-Enkripsi-Dokumen-AES-256]]**: Enkripsi terotentikasi AES-256-CBC dengan derivasi kunci PBKDF2 100.000 iterasi dan verifikasi integritas HMAC-SHA256.
5. **[[05. Transformasi Kode & Forensik Dokumen|05-Transformasi-Kode-Forensik]]**: Formatter XML/HTML/SQL, pemadat kode, inspeksi token JWT, dan analisis Shannon Entropy.
6. **[[06. Daftar Lengkap Pintasan Keyboard|06-Daftar-Pintasan-Keyboard]]**: Panduan tombol pintas lintas sistem untuk macOS, Linux, dan Windows.
7. **[[07. Matriks Dukungan Platform Sistem Operasi|07-Matriks-Dukungan-Platform]]**: Kompatibilitas platform mencakup macOS 11–26 (Apple Silicon & Intel), Ubuntu, Debian, Fedora, openSUSE, dan FreeBSD.
8. **[[08. Panduan Kompilasi dari Kode Sumber|08-Kompilasi-dari-Sumber]]**: Langkah-langkah build native menggunakan CMake, Ninja, dan Qt 6.

---

### 🛡️ Core Principles & Invariants

- **Zero Bloat, Zero Webview**: Pure C++20 and native Qt 6 widgets running under 35 MB idle RAM.
- **Air-Gap Privacy**: SparkNotePad never connects to remote servers in the background. Network access is strictly air-gapped with TLS 1.2+ security policies.
- **Defensive Engineering**: Sandboxed Lua runtime, ReDoS regex protections, configuration tampering guards, and 14-pillar verified code quality.
