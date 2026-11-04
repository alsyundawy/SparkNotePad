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

SparkNotePad v1.3.8 provides a floating spotlight modal:

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

SparkNotePad v1.3.8 organizes power developer utilities under a top-level **Tools** menu:

- **Cryptographic Hash Submenu**: MD5, CRC32, XXHASH, SHA-1, SHA-256, SHA-512 with Generate (entire file), Generate From File, Generate From Selection, and Verify actions.
- **Cryptographic Hash & Authenticity Validator**: Direct launcher for streaming 64 KB chunk hash validation.
- **MIME Tools Submenu**: Base64 Encode/Decode, URL Percent Encode/Decode, and Quoted-Printable Encode.
- **Converter Submenu**: Bidirectional ASCII <-> HEX and Decimal <-> HEX conversion.
- **Relocated AES-256 Encryption**: High-assurance document encryption with PBKDF2 (100,000 rounds) organized cleanly under Tools.

---

## 7. Tab Bar Plus (+) Button & In-Tab Previews (v1.3.8)

- **Tab Bar Plus Button**: An integrated `+` button positioned at the end of the tab strip allows spawning new editor documents with one click.
- **In-Tab Markdown & HTML Preview**: Right-clicking `.md` or `.html` tabs provides an in-tab preview pane rendered with GitHub styling, keeping workflows clean without opening external windows. For HTML files, a "Preview in Default Browser" action is also provided.
- **File Commander Explorer**: Upgraded explorer dock featuring two-pane ergonomics, quick bookmark paths, breadcrumb navigation, and metrics bars.
- **Hex/Binary View Toggle**: Switch instantly between normal text and hex memory dumps via `Tools > Toggle Hex / Binary View` (positioned directly beneath Compare, `Ctrl+Alt+H` / `Cmd+Alt+H`).

---

## 8. Universal High-DPI, Retina & Multi-Monitor Scaling (v1.3.8)

- **LCD-Optimized Font Rendering**: Enabled `SC_EFF_QUALITY_LCD_OPTIMIZED` and double-buffered drawing (`setBufferedDraw(true)`) for razor-sharp text on Apple Silicon Retina MacBooks, Intel Retina, 4K/5K/6K external monitors, and standard 1366×768 / 1080p displays.
- **Scale Factor Adaptation**: Dynamically recalculates font metric geometry and canvas repaint upon display scale changes (`DevicePixelRatioChange` and `ScreenChangeInternal`).

---

## 9. Complete Lexilla 5.5.4 Lexer Suite (125 Highlighters)

SparkNotePad includes the full complement of 125 Lexilla lexers, covering modern and legacy languages including AsciiDoc, BibTeX, Bicep, Clojure, Elixir, GDScript, GLSL, HLSL, Mojo, OCaml, Raku, Solidity, SystemVerilog, WebAssembly, and Zig.

---

## 10. Dedicated Preferences Tabs & Desktop Menu Ergonomics (v1.3.8)

- **Themes & Toolbar Tab (Tab 3)**: Relocates visual theme configuration, window opacity slider, tail -f reload interval, and multi-DPI display placement guard alongside the Toolbar Icon Packs manager (23 packs, responsive description card layout, size selector 16x16 to 64x64, live icon preview, custom folder loader, and rescan).
- **Custom Toolbar Config Tab (Tab 4)**: Embedded visual toolbar customizer widget (`CustomizeToolbarWidget`) directly inside Preferences with search filtering, action adding/removing/reordering, separator insertion, and live hot-reload.
- **Desktop Standard Menu Hierarchy**: Toggle Hex/Binary view below Compare, Restart Application in macOS Application menu directly above Quit (`QAction::ApplicationSpecificRole`), redundant Character Palette deduplicated, and clean View menu without duplicate Side Panels submenu.

---

Next Chapter: **[[03. User Interface & Themes|03-User-Interface-and-Themes]]**
