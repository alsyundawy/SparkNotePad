# SparkNotePad User Guide

Welcome to **SparkNotePad** — an independent, fast, lightweight text and source code editor engineered for macOS and Linux, powered by native Qt 6 and vendored Scintilla and Lexilla editing engines.

---

## Table of Contents

1. [Overview & Philosophy](#1-overview--philosophy)
2. [Installation & Setup](#2-installation--setup)
3. [Setting SparkNotePad as Default Editor](#3-setting-sparknotepad-as-default-editor)
4. [User Interface Overview](#4-user-interface-overview)
5. [Text & Code Editing Features](#5-text--code-editing-features)
6. [Syntax Highlighting & Language Support](#6-syntax-highlighting--language-support)
7. [Military-Grade File & Text Encryption](#7-military-grade-file--text-encryption)
8. [Real-Time Log Tail Monitoring](#8-real-time-log-tail-monitoring)
9. [JavaScript & Math Expression Evaluator](#9-javascript--math-expression-evaluator)
10. [Developer & Text Utilities](#10-developer--text-utilities)
11. [Dynamic Theme Engine (Dark & Light Modes)](#11-dynamic-theme-engine-dark--light-modes)
12. [Multi-Monitor & Retina High-DPI Adaptation](#12-multi-monitor--retina-high-dpi-adaptation)
13. [macOS Native Integration & Services](#13-macos-native-integration--services)
14. [Accessibility & VoiceOver Support](#14-accessibility--voiceover-support)
15. [Keyboard Shortcuts Quick Reference](#15-keyboard-shortcuts-quick-reference)
16. [Troubleshooting & FAQ](#16-troubleshooting--faq)

---

## 1. Overview & Philosophy

SparkNotePad is designed for developers, sysadmins, writers, and power users who value:

- **Instant startup & low memory footprint**: Boots in milliseconds without heavy web runtimes or background daemon bloat.
- **Battle-tested Scintilla engine**: The gold standard in code editing, syntax styling, fold margins, and multi-cursor selection.
- **Strict User Privacy & Security**: Zero telemetry, zero analytics, zero external network phoning home. Built-in AES-256-CBC encryption.
- **First-class macOS & Linux UX**: Full macOS menu bar, Proxy Icon drag-and-drop, Services menu, Retina crispness, VoiceOver speech, and Linux `.deb` / `.rpm` package integration.

---

## 2. Installation & Setup

### 2.1 macOS

- **System Requirements:** macOS 11.0 (Big Sur) up to macOS 15.0+ (Sequoia) on Apple Silicon (M1/M2/M3/M4) or Intel x86_64.
- **Installation:**
  1. Download `SparkNotePad-v<version>-arm64.dmg` (or `SparkNotePad-v<version>-x64.dmg`).
  2. Open the DMG image and drag `SparkNotePad.app` into your `/Applications` directory.
  3. The application is codesigned with Apple Hardened Runtime.

### 2.2 Linux (Debian / Ubuntu / Linux Mint)

- Install the `.deb` package:

  ```bash
  sudo apt install ./sparknotepad_<version>_amd64.deb
  ```

### 2.3 Linux (Fedora / RHEL / openSUSE)

- Install the `.rpm` package:

  ```bash
  sudo rpm -ivh sparknotepad-<version>-1.x86_64.rpm
  # or using dnf:
  sudo dnf install ./sparknotepad-<version>-1.x86_64.rpm
  ```

---

## 3. Setting SparkNotePad as Default Editor

SparkNotePad includes an automated utility script (`scripts/SparkNotePad-Default-Editor.sh`):

```bash
# Check current system file associations:
./scripts/SparkNotePad-Default-Editor.sh --status

# Dry-run inspection without changing configuration:
./scripts/SparkNotePad-Default-Editor.sh --dry-run

# Set SparkNotePad as default editor for 470+ source code and text extensions:
./scripts/SparkNotePad-Default-Editor.sh

# Restore previous associations if desired:
./scripts/SparkNotePad-Default-Editor.sh --restore
```

On macOS, this configures LaunchServices using `duti` (or native LS fallback) across 38 Uniform Type Identifiers (UTIs) including `public.plain-text`, `public.source-code`, and markdown. On Linux, it configures `xdg-mime` and `update-alternatives`.

---

## 4. User Interface Overview

```text
┌────────────────────────────────────────────────────────────────────────┐
│ SparkNotePad — [Window Title: filename.cpp] (Proxy Icon Drag & Dirty Dot)│
├────────────────────────────────────────────────────────────────────────┤
│ Menu: File  Edit  Search  View  Tools  Run  Window  Help               │
├────────────────────────────────────────────────────────────────────────┤
│ Toolbar: [New] [Open] [Save] [Cut] [Copy] [Paste] [Undo] [Find] [Tail] │
├───────────────┬────────────────────────────────────────────────────────┤
│ File Browser  │ [tab1.cpp] [tab2.py *] [tab3.json]                     │
│ ───────────── │ 1 │ #include <iostream>                                │
│ > src/        │ 2 │                                                    │
│   > features/ │ 3 │ int main() {                                       │
│   CMakeLists  │ 4 │     std::cout << "Hello SparkNotePad!" << std::endl; │
│   README.md   │ 5 │     return 0;                                      │
│               │ 6 │ }                                                  │
├───────────────┴────────────────────────────────────────────────────────┤
│ Status Bar: UTF-8 | Unix (LF) | Ln: 4, Col: 12 | Sel: 0 | 100% | INS  │
└────────────────────────────────────────────────────────────────────────┘
```

- **Tabs Bar:** Reorderable document tabs with modified indicator dots (`*`), middle-click tab closure, and quick actions toolbar.
- **Proxy Icon (macOS):** Click and drag the document icon in the title bar directly to Finder, Mail, Terminal, or Slack.
- **Dock Widgets (View menu):**
  - **File Browser (`Ctrl+Alt+O`):** Integrated file tree navigation.
  - **Folder as Workspace:** Project-level file management.
  - **Language Inspector (`Ctrl+Shift+I`):** Real-time Scintilla lexer tokens, styles, and state inspector.
  - **Function List:** Outline of functions, classes, and symbols in the active file.
  - **Search Results (`Ctrl+Shift+F`):** Multi-file grep search hits.
  - **Lua Console & Debug Log:** Real-time diagnostics.

---

## 5. Text & Code Editing Features

- **Multi-Selection & Multiple Carets:** Hold `Ctrl` (or `Cmd` on macOS) and click anywhere in the text to add multiple carets and edit multiple lines simultaneously.
- **Column / Rectangular Selection:** Hold `Alt` and drag the mouse cursor to select rectangular columns of text. Press `Alt+C` to open the Column Editor.
- **Auto-Close Pairs:** Automatically inserts closing parentheses `()`, brackets `[]`, braces `{}`, single quotes `''`, and double quotes `""`.
- **Focused View Mode (`F11`):** Distraction-free editing canvas concealing surrounding toolbars and status docks.
- **Color Preview Call-Tips:** Hovering over CSS/HTML hex codes (e.g. `#388bfd`, `rgb(56, 139, 253)`) displays an inline popup color swatch.
- **Indent Guides & Whitespace:** Toggle visible indentation guides, tabs, spaces, and newline markers from the `View` menu.

---

## 6. Syntax Highlighting & Language Support

SparkNotePad supports over 100 programming languages, configuration formats, and markup types out of the box via vendored Lexilla:

- **Web & Scripting:** JavaScript, TypeScript, Python, Ruby, PHP, Lua, Perl, HTML5, CSS3, SCSS, JSON, YAML, TOML, XML.
- **Compiled Languages:** C, C++, Rust, Go, Swift, Java, C#, Kotlin, D, Zig, Fortran, Pascal.
- **Shell & Systems:** Bash, Zsh, PowerShell, Batch, CMake, Makefile, Dockerfile, NSIS.
- **Scientific & Data:** SQL, R, MATLAB/Octave, Julia, Markdown, LaTeX, Diff/Patch.

To override or manually select a language, select `Language > <Language Name>` from the top menu bar or use the Language Inspector dock (`Ctrl+Shift+I`).

---

## 7. Military-Grade File & Text Encryption

SparkNotePad provides built-in cryptographic protection to safeguard sensitive files, credentials, and confidential notes:

- **Algorithm:** AES-256-CBC with PKCS#7 padding.
- **Key Derivation:** PBKDF2 with SHA-256 and **100,000 iterations**, utilizing a cryptographically secure 128-bit random salt.
- **Integrity Validation:** Encrypted envelopes are encoded with a binary container header (`SPKENC01`, with dual-format backwards compatibility for legacy `NSPENC01`). Decryption validates PKCS#7 padding integrity and HMAC-SHA256 authenticated envelope verification, immediately alerting the user if a password or ciphertext is corrupted.
- **Usage:**
  - **Encrypt Document / Selection:** Select `Tools > Encryption > Encrypt Document...` (`Ctrl+Alt+E`). Enter a secure passphrase.
  - **Decrypt Document / Selection:** Select `Tools > Encryption > Decrypt Document...` (`Ctrl+Alt+X`). Enter your passphrase to restore plain text.
  - **macOS Services Integration:** Highlight text in any application (e.g., Safari or Mail), right-click and choose `Services > SparkNotePad: Encrypt Selection`.

---

## 8. Real-Time Log Tail Monitoring

Sysadmins and backend developers can monitor live log files without spawning a terminal:

- **Launch:** Open any active log file (e.g. `/var/log/system.log` or server application logs) and select `Tools > Monitoring & Tail Mode...` (`Ctrl+Alt+M`).
- **Features:**
  - **Follow Tail:** Automatically scrolls to the bottom as new lines arrive.
  - **Live Filter:** Highlight or filter incoming entries using regex or case-insensitive substrings (e.g., `ERROR`, `EXCEPTION`).
  - **Pause / Resume:** Temporarily freeze the stream to inspect an error trace, then resume streaming without losing position.
  - **Configurable Polling Interval:** Set update frequencies from 200ms to 5000ms.

---

## 9. JavaScript & Math Expression Evaluator

Transform SparkNotePad into an interactive scratchpad:

- **Launch:** Press `Ctrl+Alt+J` or select `Tools > Evaluate JavaScript / Math...`.
- **Expression Evaluation:** Calculate inline mathematical expressions:

  ```js
  Math.sqrt(144) * Math.PI + Math.pow(2, 10); // = 1061.699...
  ```

- **Text & JSON Transformation:** Format, parse, or clean up data:

  ```js
  JSON.stringify(JSON.parse(editorText), null, 4);
  ```

- **Security Sandboxing:** Executed inside an isolated `JSGlobalContextRef` engine (JavaScriptCore on macOS) with zero filesystem or network privileges.

---

## 10. Developer & Text Utilities

Located under the `Tools` and `Edit` menus:

- **Base64 Encoding & Decoding:** Encode or decode selected text (`Ctrl+Alt+B`).
- **Number Radix Conversion:** Convert integers seamlessly between Binary (`0b`), Octal (`0o`), Decimal, and Hexadecimal (`0x`).
- **UUID / GUID Generation:** Generate RFC-4122 v4 GUIDs directly into your document (`Ctrl+Alt+G`).
- **ISO-8859 Timestamps:** Insert current UTC or local ISO-8601 timestamps.
- **Case Conversion:** Convert selection to UPPERCASE, lowercase, Title Case, camelCase, snake_case, or kebab-case.
- **Code Minification & Pretty Printing:** Strip whitespace or format JSON/CSS.
- **Clipboard History Monitor:** Access recent clipboard snippets via `Edit > Clipboard History...` (`Ctrl+Alt+V`).
- **Window Transparency:** Adjust window opacity from 20% to 100% or click-through mode via `View > Window Transparency...`.

---

## 11. Dynamic Theme Engine (Dark & Light Modes)

SparkNotePad features a customized, native theme engine:

- **System Default (Auto):** Synchronizes with the OS appearance (`QStyleHints::colorSchemeChanged`) in real-time.
- **Forced Dark Mode:** Deep, contrast-rich palettes (`#1e1e1e` canvas, `#25282c` tabs) reducing eye strain during night coding.
- **Forced Light Mode:** Crisp, clean styling with high-contrast text and line borders.
- **Instant Toggle:** Press `Ctrl+Alt+T` (or `Cmd+Alt+T` on macOS) to switch between Dark and Light mode instantly.

---

## 12. Multi-Monitor & Retina High-DPI Adaptation

Optimized for mixed-display setups (Retina display + external standard/4K monitors):

- **PassThrough High-DPI Policy:** Prevents fuzzy text or glyph distortion during fractional scaling (125%, 150%, 175%).
- **Move to Next Screen (`Ctrl+Alt+Right` / `Cmd+Alt+Right`):** Moves the active window smoothly to the next connected display.
- **Move to Previous Screen (`Ctrl+Alt+Left` / `Cmd+Alt+Left`):** Returns window to previous display.
- **Center Window on Screen (`Ctrl+Alt+C` / `Cmd+Alt+C`):** Centers window on the active display.
- **Off-Screen Recovery Guard:** Automatically recovers and repositions windows that become orphaned when external monitors are unplugged.
- **Display Diagnostics Dialog (`Ctrl+Alt+D` / `Cmd+Alt+D`):** Inspect monitor resolutions, DPR, refresh rates, and color depths.

---

## 13. macOS Native Integration & Services

- **Native macOS Menu Bar:** Full macOS integration including the native `Window` menu (`Minimize`, `Zoom`, `Bring All to Front`).
- **Proxy Icon Dragging:** Drag the icon in the title bar directly into other applications.
- **Window Dirty Indicator:** macOS standard dirty dot inside the red close button when documents contain unsaved edits.
- **macOS Services Menu:**
  - `SparkNotePad: Open Selected File or Path`
  - `SparkNotePad: New Document with Selection`
  - `SparkNotePad: Encrypt Selection`
  - `SparkNotePad: Decrypt Selection`
- **Emoji & Symbols:** Press `Ctrl+Cmd+Space` to trigger the native macOS Character Palette.

---

## 14. Accessibility & VoiceOver Support

SparkNotePad is built with universal accessibility compliance (WCAG 2.1 AA):

- **VoiceOver Spoken Alerts:** Native speech notifications announce document tab switches, line counts, find results, and editor focus.
- **Focus Editor Shortcut:** Press `Ctrl+Alt+E` (or `Escape` in dialogs) to instantly return keyboard focus to the code canvas.
- **Searchable Shortcuts & Accessibility Guide:** Press `Shift+F1` to open the interactive keyboard reference dialog.
- **High-Contrast Themes:** Built-in high-contrast syntax schemes for visually impaired users.

---

## 15. Keyboard Shortcuts Quick Reference

| Action                        | macOS Shortcut                  | Linux / Windows Shortcut        |
| ----------------------------- | ------------------------------- | ------------------------------- |
| **Quick Open (Omnibar)**      | `Cmd+P` / `Ctrl+Alt+O`          | `Ctrl+P` / `Ctrl+Alt+O`         |
| **Command Palette Launcher**  | `Cmd+Shift+P` / `Ctrl+Alt+I`    | `Ctrl+Shift+P` / `Ctrl+Alt+I`   |
| **New Document**              | `Cmd+N`                         | `Ctrl+N`                        |
| **Open Document**             | `Cmd+O`                         | `Ctrl+O`                        |
| **Save Document**             | `Cmd+S`                         | `Ctrl+S`                        |
| **Save As**                   | `Cmd+Shift+S`                   | `Ctrl+Shift+S`                  |
| **Close Tab**                 | `Cmd+W`                         | `Ctrl+W`                        |
| **Print Document**            | `Cmd+Alt+Shift+P`               | `Ctrl+Alt+Shift+P`              |
| **Find Text**                 | `Cmd+F`                         | `Ctrl+F`                        |
| **Replace Text**              | `Cmd+H`                         | `Ctrl+H`                        |
| **Find in Files**             | `Cmd+Shift+F`                   | `Ctrl+Shift+F`                  |
| **Go to Line**                | `Cmd+G`                         | `Ctrl+G`                        |
| **Next Tab / Prev Tab**       | `Ctrl+Tab` / `Ctrl+Shift+Tab`   | `Ctrl+Tab` / `Ctrl+Shift+Tab`   |
| **Focus Editor**              | `Ctrl+Alt+E`                    | `Ctrl+Alt+E`                    |
| **Visual Diff Navigation**    | `F7` (Next) / `Shift+F7` (Prev) | `F7` (Next) / `Shift+F7` (Prev) |
| **Markdown Safe Preview**     | `Cmd+Shift+M`                   | `Ctrl+Shift+M`                  |
| **Toggle Dark/Light Theme**   | `Cmd+Alt+T`                     | `Ctrl+Alt+T`                    |
| **Move to Next Screen**       | `Cmd+Alt+Right`                 | `Ctrl+Alt+Right`                |
| **Move to Previous Screen**   | `Cmd+Alt+Left`                  | `Ctrl+Alt+Left`                 |
| **Center Window on Screen**   | `Cmd+Alt+C`                     | `Ctrl+Alt+C`                    |
| **Display Diagnostics**       | `Cmd+Alt+D`                     | `Ctrl+Alt+D`                    |
| **Encrypt Document**          | `Cmd+Alt+E`                     | `Ctrl+Alt+E`                    |
| **Decrypt Document**          | `Cmd+Alt+X`                     | `Ctrl+Alt+X`                    |
| **Evaluate JS / Math**        | `Cmd+Alt+J`                     | `Ctrl+Alt+J`                    |
| **Log Tail Monitoring**       | `Cmd+Alt+M`                     | `Ctrl+Alt+M`                    |
| **Clipboard History**         | `Cmd+Alt+V`                     | `Ctrl+Alt+V`                    |
| **Accessibility & Shortcuts** | `Shift+F1`                      | `Shift+F1` / `F1`               |
| **Restart Application**       | `Cmd+Alt+R`                     | `Ctrl+Alt+R`                    |
| **User Guide**                | `F1`                            | `Ctrl+F1`                       |

---

## 16. Troubleshooting & FAQ

**Q: Why does macOS say "SparkNotePad cannot be opened because developer cannot be verified"?**
A: SparkNotePad is codesigned with Apple Hardened Runtime. If installing an unnotarized development build, right-click `SparkNotePad.app` in Finder, select **Open**, and click **Open Anyway**.

**Q: Can I run multiple instances of SparkNotePad?**
A: By default, opening files or running SparkNotePad communicates with the primary instance via single-application IPC. To launch an independent window, pass the `--multi-instance` flag from the command line.

**Q: Where are my settings and custom configurations stored?**

- **macOS:** `~/Library/Preferences/SparkNotePad/SparkNotePad.ini`
- **Linux:** `~/.config/SparkNotePad/SparkNotePad.ini`
