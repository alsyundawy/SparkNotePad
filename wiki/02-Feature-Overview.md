# 02. Feature Overview

SparkNotePad combines the heritage of the Scintilla text editing engine with modern developer ergonomics inspired by KDE Kate, Pluma (MATE), and Notepad--.

---

## 1. Scintilla 5.6.7 Core Editing Canvas

At the heart of SparkNotePad is **Scintilla 5.6.7** paired with **Lexilla 5.5.4**, delivering:

- **Sub-Millisecond Line Rendering**: Zero lag on files containing millions of characters.
- **Multi-Selection & Rectangular Column Editing**: Hold `Alt` (or `Option` on macOS) while dragging to select rectangular text blocks, or hold `Ctrl` (`Cmd` on macOS) while clicking to place multiple independent carets.
- **Multi-Cursor Status Bar Indicator**: The bottom status bar dynamically displays `[N Cursors]` whenever multi-selection is active.
- **Code Folding**: Hierarchical fold margins supporting indent-based, brace-based, and keyword-based folding.
- **100+ Built-In Language Lexers**: Highlighting for C, C++, C#, Python, Java, JavaScript, TypeScript, PHP, Rust, Go, Zig, SQL, HTML, XML, YAML, JSON, Markdown, Bash, Lua, and more.

---

## 2. Omnibar Command Palette & Quick Open

SparkNotePad v1.3.7 introduces a floating spotlight modal:

### Quick Open Mode (`Cmd+P` / `Ctrl+P` or Kate `Ctrl+Alt+O`)

- **Tab Switching & Search**: Type keywords to instantly find and switch between any open document buffer or recently opened file.
- **Fuzzy Matching**: Matches fuzzy characters across full paths and file names.
- **Go-To-Line Jumping**: Type `:line` directly in the search bar (e.g., `:150`) to jump directly to line 150 of the active document.

### Command Runner Mode (`Cmd+Shift+P` / `Ctrl+Shift+P` or Kate `Ctrl+Alt+I`)

- Prefix any search with `>` (or launch via `Cmd+Shift+P`) to search and execute any editor action, menu command, transformer, or view toggle without touching the mouse.

---

## 3. Native Side-by-Side Visual Diff & File Compare

Inspired by `cxasm/notepad--` and Kate `DiffWidget`, SparkNotePad includes a native comparison engine:

- **Launch**: Access via `File > Compare / Visual Diff...`.
- **Dual Synchronized Panes**: Two Scintilla editor canvases scroll synchronously in vertical lockstep.
- **Pure C++20 Myers Difference Engine**: Computes shortest edit scripts (LCS) in memory without external diff dependencies.
- **Real-Time Line Highlighting**:
  - 🟢 **Green**: Inserted lines
  - 🔴 **Red**: Deleted lines
  - 🟡 **Yellow**: Modified lines
- **Rapid Keyboard Navigation**:
  - `F7`: Jump to Next Difference
  - `Shift+F7`: Jump to Previous Difference

---

## 4. Upgraded Project & Workspace Tree Dock

Manage codebase files with an integrated file system dock:

- **Live Bottom Filter Bar**: Type glob patterns or substrings in `filterEdit` to filter project tree items in real time.
- **Toolbar Actions**: One-click **Refresh** and **Collapse All** buttons.
- **Right-Click Context Menu**:
  - `Open`: Open file in the editor canvas.
  - `Open with System Application`: Launch file in default OS handler.
  - `Show in File Manager / Finder`: Reveal file location on disk.
  - `Copy Full Path` / `Copy Relative Path`: Copy paths to clipboard.
- **`.sparkproject` JSON Support**: Automatically detects and loads workspace configurations.

---

## 5. Additional Power Utilities

- **Real-Time Log Tail Monitoring (`Cmd+Alt+M` / `Ctrl+Alt+M`)**: Automatically reloads and tails log files as new lines are appended (`tail -f`).
- **Focused View**: Filters canvas lines to isolate only occurrences matching search terms.
- **JavaScript & Math Expression Evaluator (`Cmd+Alt+J` / `Ctrl+Alt+J`)**: Evaluate JavaScript snippets, trigonometric functions, and hexadecimal/binary calculations instantly.
- **Mark All Occurrences**: Real-time indicator highlighting across all matching words with count feedback in the status bar.

---

## 6. Notepad++ Style Tools Menu Suite

SparkNotePad v1.3.7 organizes power developer utilities under a top-level **Tools** menu:

- **Cryptographic Hash Submenu**: MD5, CRC32, XXHASH, SHA-1, SHA-256, SHA-512 with Generate (entire file), Generate From File, Generate From Selection, and Verify actions.
- **Cryptographic Hash & Authenticity Validator**: Direct launcher for streaming 64 KB chunk hash validation.
- **MIME Tools Submenu**: Base64 Encode/Decode, URL Percent Encode/Decode, and Quoted-Printable Encode.
- **Converter Submenu**: Bidirectional ASCII <-> HEX and Decimal <-> HEX conversion.
- **Relocated AES-256 Encryption**: High-assurance document encryption with PBKDF2 (100,000 rounds) organized cleanly under Tools.

---

Next Chapter: **[[03. User Interface & Themes|03-User-Interface-and-Themes]]**
