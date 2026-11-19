# SparkNotePad v1.5.0 — Full Developer Tools Suite (JSON/XML/HTML/CSS/JS, Encryption, JWT, Radix), Re-Architected Menus, GitHub Primer Markdown Preview & View Enhancements

**Release Version:** `v1.5.0`<br/>
**Release Date:** 2026-11-19<br/>
**Maintainer:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Core Editing Engine:** Scintilla 5.6.9 (Vendored Official) & Lexilla 5.5.5 (Vendored Official)<br/>
**Embedded Lua Core:** Sandboxed Lua 5.5.1 (Official Patched Source 1-4)<br/>
**Scripting Runtime:** Python 3.13+ Engine Bridge<br/>
**Standard:** ISO C++20 / Qt 6.5–6.12<br/>
**Target Platforms:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu 22.04–26.04, Debian 12–14, Fedora 40–44, RHEL/Rocky 9–11, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x (KDE Plasma 6 & XFCE)

---

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="assets/sparknotepad-desktop-banner.jpg" width="100%" alt="SparkNotePad Desktop Suite — Native Scintilla &amp; Qt Text and Source Code Editor for Linux &amp; macOS" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/alsyundawy/SparkNotePad">
    <img src="logo.png" width="128" height="128" alt="SparkNotePad Native macOS &amp; Linux Application Icon" />
  </a>
</p>

<h1 align="center">SparkNotePad</h1>

<p align="center">
  <b>SparkNotePad — Lightweight, blazing-fast native Scintilla &amp; Qt text and source code editor for Linux, FreeBSD, and macOS.</b><br>
  <i>Engineered with Qt 6 and native Scintilla 5.6.9 / Lexilla 5.5.5 core for maximum execution performance and minimal memory footprint</i>
</p>

---

> Lead Developer & Maintainer:<br>
> **[`HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)`](https://github.com/alsyundawy)** — [`ALSYUNDAWY IT SOLUTION`](https://alsyundawy.com)<br>
>
> 🍏 **[`Latest Releases (v1.5.0)`](https://github.com/alsyundawy/SparkNotePad/releases/latest)** &nbsp;|&nbsp;
> 📖 **[`Release DocNotes (DOCNOTE.md)`](DOCNOTE.md)** &nbsp;|&nbsp;
> 📜 **[`Changelog (CHANGELOG.md)`](CHANGELOG.md)** &nbsp;|&nbsp;
> 🌐 **[`Platform Support Matrix`](doc/PLATFORM_SUPPORT.md)** &nbsp;|&nbsp;
> 🐛 **[`Issue Tracker`](https://github.com/alsyundawy/SparkNotePad/issues)** &nbsp;|&nbsp;
> 💖 **[`Support via PayPal`](https://www.paypal.me/alsyundawy)**

---

## ⚡ Executive Summary

SparkNotePad **v1.5.0** introduces a massive feature expansion and ergonomic modernization across the entire application:

1. **Comprehensive Developer Tools Suite (`DeveloperToolsEngine` & `DeveloperToolsDialog`)**:
   - Multi-tab interactive Developer Tools Hub (`Ctrl+Alt+T` / `Ctrl+Cmd+T`) with 5 dedicated functional tabs.
   - **Hash Tools**: 11 cryptographic hash algorithms (MD5, SHA-1, SHA-224, SHA-256, SHA-384, SHA-512, SHA3-224, SHA3-256, SHA3-384, SHA3-512, Keccak-256), file integrity hashing with progress, and side-by-side verification comparison.
   - **MIME Tools**: Base64 line-by-line encode/decode, Quoted-Printable (RFC 2045) encode/decode, URL percent-encoding/decoding, and HTML entity encode/decode.
   - **Converter Tools**: Radix base converter (bases 2, 8, 10, 16), ASCII <-> Hexadecimal converter with configurable delimiters, and advanced Case Conversion (Title Case, Invert Case, Sentence Case).
   - **Encryption Tools**: HTML Document Protection with 3 distinct operational modes:
     1. JavaScript unescape string injection.
     2. Base64 `atob()` encoding.
     3. Military-grade Password AES-256-GCM encryption with embedded CryptoJS runtime decryption harness.
     - Real-time zxcvbn password strength and entropy evaluation meter.
   - **Developer Tools**: Format and minify for JSON, XML, HTML, CSS, and JavaScript; RFC 7519 JWT Token Inspector with decoded header, payload claims, and expiration dates; UUID v4 generator; and Unix Epoch / ISO-8601 bidirectional converter.

2. **Perfect GitHub Primer Markdown Safe Live Preview (`mjbvz/vscode-github-markdown-preview-style`)**:
   - Re-engineered preview HTML generator strictly conforming to GitHub Markdown CSS specifications.
   - Eliminated Qt rich text formatting artifacts (`-qt-block-indent`, zero margin resets) to ensure GitHub typography, border colors, table zebra striping, and code block styling display accurately.
   - Native GitHub Alert Callout rendering (`[!NOTE]`, `[!TIP]`, `[!IMPORTANT]`, `[!WARNING]`, `[!CAUTION]`) with official icons, accent borders, and background tints.
   - Completely removed "Open in Default Browser" buttons and functions across all preview docks and widgets for maximum security and zero-bloat standalone operation.

3. **Re-Architected Top-Level Menu Structure**:
   - Created top-level **Format** menu grouping character set encodings (`ui->menuEncodings`) and newline conversions (`ui->menuEOLConversion`).
   - Relocated **Run & External Execution** from top-level menubar into `Tools -> Run & External Execution` (`Ctrl+Alt+R` / `F5`).
   - Added dedicated **Developer Tools** submenu inside `Tools` with direct actions for formatters, minifiers, encoders, and the Developer Tools Hub (`Ctrl+Alt+T`).

4. **Enhanced View & Navigation Toggles**:
   - Added toggle actions in `View` menu for Line Numbers, Code Folding, Indent Guides, Caret Line Highlight, Read-Only mode, and Zoom In / Out / Reset.

5. **Updated Credits, Shortcuts & Translations**:
   - Full updates to `AboutDialog` Credits & Shortcuts tabs with the new hotkeys and attributions.
   - Synchronized `i18n` translation catalogs (`SparkNotePad_id.ts`, `SparkNotePad_en.ts`) and compiled binary `.qm` packages.
