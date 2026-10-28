# SparkNotePad — Feature Inventory & Implementation Status

- **Date:** 2026-10-28
- **Release Version:** SparkNotePad v1.3.7
- **Editor Engine:** Scintilla 5.6.7 + Lexilla 5.5.4 (vendored in `thirdparty/`)
- **Standard:** C++20 / Qt 6.5–6.12

---

## 1. Core Feature Inventory

### 1.1 Core Editor & Scintilla Integration

- **ScintillaNext Wrapper**: C++ Qt wrapper encapsulating Scintilla's native document model, styles, margins, indicators, and commands.
- **Lexilla Lexers**: Static linking to Lexilla lexers covering 100+ languages (C/C++, Python, Java, JS, TypeScript, HTML/XML, SQL, PHP, Bash, Rust, Go, Zig, TOML, Dart, etc.).
- **Lua-Based Language Definitions**: Extensible language definition system in `src/languages/*.lua` configuring keyword lists, comment characters, and style mapping.
- **Multi-Tab & Session Management**:
  - `SessionManager`: Automatic session saving, dirty file state preservation, and tab layout restore across restarts.
  - `TabsQuickActionsBar`: Quick tab switching, tab closure, and editor list menu.
- **Docking System**: Qt Advanced Docking System (`qtadvanceddocking-qt6`) enabling draggable, dockable, floating panels.
- **Encoding & EOL Support**:
  - Encoding auto-detection via `uchardet`.
  - UTF-8, UTF-16LE, UTF-16BE, Windows-1252, ISO-8859, and common legacy code pages.
  - Line ending conversions (CRLF, LF, CR).

### 1.2 Search & Navigation

- **QuickFindWidget**: In-buffer search bar with forward/backward find and incremental search.
- **Find / Replace Dialog**: Extended search with case sensitivity, whole word, wrap around, and regular expressions (`SCI_OWNREGEX`). Strictly enforces 1 search input box on Find and Mark tabs.
- **Goto Line / Position**: Modal dialog for direct buffer navigation.
- **Find in Files**: Multi-threaded directory search across project files.
- **Mark All Occurrences**: Real-time Scintilla indicator marking with match count display in status bar.

### 1.3 Text Editing & Transformation Utilities

- **Sorting**: `ScintillaSorter` supporting lexicographical, numeric, case-sensitive, and reverse sorting.
- **Indentation & Whitespace**: Tab-to-space, space-to-tab, trim trailing whitespace, indent guides, and visible whitespace toggle.
- **Case Conversion**: Uppercase, Lowercase, Title Case, Sentence Case, and Invert Case (`bALIK hURUF`).
- **Base64 Encode / Decode**: Instant in-place selection transformation.
- **Number Conversion**: Decimal ↔ Hexadecimal ↔ Octal ↔ Binary with standard notation prefixes.
- **Insert GUID & Timestamps**: Multiple UUID/GUID formats and configurable ISO/RFC timestamps.
- **Pretty & Compress**: In-place formatter and minifier for JSON, CSS, JavaScript, XML, HTML, and SQL.
- **Focused View**: Distraction-free editing filtering canvas to display only matching lines.
- **Show Character Info**: Real-time Unicode code point, category, and byte sequence inspector.
- **File Time Utilities**: Touch file timestamp and save with timestamp preservation.

### 1.4 Cryptography & Log Streaming

- **AES-256 Document Encryption**: Authenticated encryption (PBKDF2 100k + HMAC-SHA256) supporting `SPKENC01` and legacy `NSPENC01` format containers.
- **Real-Time Log Tail Monitoring**: Zero-modal `tail -f` log streaming mode with automatic buffer rolling and status indicators.
- **JavaScript & Math Expression Evaluator**: Sandboxed JavaScriptCore expression evaluator.

### 1.5 Defensive Security Hardening (v1.3.6)

- **Lua Sandbox Hardening**: Comprehensive nullification of dangerous OS, I/O, debug, and package loader primitives in `LuaState` (CVE-2026-42214).
- **ConfigIntegrityGuard**: Guarding session configurations with `0600` permissions and blocking external tool shell interpreter injection (CVE-2026-48778, CVE-2026-48800).
- **RegexSafetyEngine**: Detecting catastrophic nested quantifiers before regex evaluation (CVE-2026-4744).
- **MarkdownSafePreviewDock**: Isolated preview with safe URI scheme whitelisting (CVE-2026-20841).
- **Real-Time Cryptographic Hash Integrity**: Multi-hash calculator (SHA-256, SHA-512, SHA-3) via `HashIntegrityDialog`.
- **Vector Toolbar Suite**: 24x24 monoline vector SVG toolbar icons replacing raster bitmaps.

### 1.6 High-Velocity Ergonomics, Diff & Forensics (v1.3.7)

- **Omnibar Command Palette & Quick Open**: Global modal spotlight bound to `Cmd+P` / `Ctrl+P` (Kate `Ctrl+Alt+O`) and `Cmd+Shift+P` / `Ctrl+Shift+P` (Kate `Ctrl+Alt+I`) for tab switching, `:line` jumping, and executing editor actions.
- **Native Side-by-Side Visual Diff & File Compare**: Pure C++20 Myers/LCS diff algorithm in `FileDiffDialog` with synchronized vertical scrolling and `F7`/`Shift+F7` navigation.
- **Upgraded Project & Workspace Tree Dock**: Live bottom filter bar (`filterEdit`), toolbar controls (Refresh, Collapse All), and context menu.
- **Swiss-Army Code Transformer**: XML / HTML Prettify and Minify, SQL formatting, and JWT token inspection.
- **Document Statistics & Forensics**: Real-time character/word/line counts, reading time, and Shannon Entropy computation in `DocumentStatisticsDialog`.
- **Multi-Cursor Status Indicator**: Active cursor count displayed in the status bar (`[N Cursors]`).

### 1.7 Platform Polish (macOS & Linux)

- **macOS Native Integration**: Apple HIG-compliant `.app` bundle, Proxy Icon drag-and-drop, window dirty state dot, Services menu provider, VoiceOver spoken feedback, and full Retina icon sets (`SparkNotePad.icns`).
- **Application Lifecycle**: Native Restart (`Ctrl+Alt+R` / `Cmd+Option+R`) and explicit Exit action.
- **Packaging**: macOS DMG builder (`macdeployqt`) and Linux native DEB / RPM packaging (`CPack`).

---

## 2. Capability Implementation Matrix

| Capability                                  | Status in v1.3.7 | Engine / Implementation                                           |
| :------------------------------------------ | :--------------: | :---------------------------------------------------------------- |
| **Omnibar Command Palette & Quick Open**    |   ✅ Complete    | `CommandPaletteDialog` (`Cmd+P`, `Cmd+Shift+P`)                   |
| **Side-by-Side Visual Diff**                |   ✅ Complete    | `FileDiffDialog` + pure C++20 `MyersDiffEngine`                   |
| **Project Tree Live Filter Bar**            |   ✅ Complete    | `FolderAsWorkspaceDock` filter bar & context menu                 |
| **Document Statistics & Shannon Entropy**   |   ✅ Complete    | `DocumentStatisticsDialog` & `TokenForensicUtils`                 |
| **XML / HTML & SQL Prettify / Minify**      |   ✅ Complete    | `CodeCompressPretty` streaming formatter                          |
| **JWT Token Inspector & Validator**         |   ✅ Complete    | `TokenForensicUtils::decodeJwt()`                                 |
| **Multi-Cursor Active Counter**             |   ✅ Complete    | `EditorInfoStatusBar` (`[N Cursors]`)                             |
| **Mark All Occurrences + Match Count**      |   ✅ Complete    | Scintilla indicator marking + real-time status bar counter        |
| **Enhanced Auto-completion & Auto-close**   |   ✅ Complete    | Auto-close pairs `()`, `[]`, `{}`, `""`, `''`, context completion |
| **File Browser Dock**                       |   ✅ Complete    | Integrated file tree explorer dock widget                         |
| **Base64 Encode / Decode**                  |   ✅ Complete    | Direct in-place selection encode/decode                           |
| **Number Conversion**                       |   ✅ Complete    | Dec ↔ Hex ↔ Oct ↔ Bin conversion                                  |
| **Insert GUID & Timestamps**                |   ✅ Complete    | Configurable GUID formats and timestamp insertion                 |
| **Full-Screen Mode**                        |   ✅ Complete    | Native macOS + Qt full-screen toggle                              |
| **Color Preview CallTip**                   |   ✅ Complete    | Hex/RGB hover swatch preview via Scintilla CallTip                |
| **Pretty & Compress (JSON/CSS/JS/XML/SQL)** |   ✅ Complete    | In-place formatters without external dependencies                 |
| **Focused View**                            |   ✅ Complete    | Filter editor buffer to display only matching lines               |
| **Show Character Info**                     |   ✅ Complete    | Unicode code point, category, and byte sequence dump              |
| **File Time Utilities**                     |   ✅ Complete    | Touch file time & save preserving original timestamp              |
| **AES-256 File & Text Encryption**          |   ✅ Complete    | `CryptoManager` with PBKDF2 100k + HMAC-SHA256                    |
| **Log Tail Monitoring (`tail -f`)**         |   ✅ Complete    | Non-modal real-time log monitoring engine                         |
| **JavaScript Expression Evaluator**         |   ✅ Complete    | Sandboxed JS expression and math evaluation                       |
| **Application Restart & Explicit Exit**     |   ✅ Complete    | `MainWindow::restartApplication()` and clean exit                 |
| **Complete Indonesian Localization**        |   ✅ Complete    | 1,474 translations finished, EYD V & PASTI compliant              |
