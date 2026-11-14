# SparkNotePad — Feature Inventory & Implementation Status

- **Date:** 2026-11-14
- **Release Version:** SparkNotePad v1.4.7
- **Editor Engine:** Scintilla 5.6.9 + Lexilla 5.5.5 (129 lexers complete)
- **Standard:** C++20 / Qt 6.5–6.12

---

## 1. Core Feature Inventory

### 1.1 Core Editor & Scintilla Integration

- **ScintillaNext Wrapper**: C++ Qt wrapper encapsulating Scintilla's native document model, styles, margins, indicators, and commands.
- **Lexilla Lexers**: Static linking to Lexilla lexers covering 125 languages (100% Lexilla parity including GDScript, AsciiDoc, BibTeX, F#, Eiffel, Modula, PO, Crontab, Conf, Hex, Raku, Stata, Clojure, Elixir, Groovy, Haxe, OCaml, Racket, GLSL, HLSL, SystemVerilog, Troff, POV-Ray).
- **Lua-Based Language Definitions**: Extensible language definition system in `src/languages/*.lua` configuring keyword lists, comment characters, and style mapping.
- **Multi-Tab & Session Management**:
  - `SessionManager`: Automatic session saving, dirty file state preservation, and tab layout restore across restarts.
  - `DockedEditorTitleBar`: Interactive Plus (+) button at the end of tab row for instant new document creation.
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

### 1.8 Toolbar Personalization, Icon Suites, In-Tab Preview & Lexilla Parity (v1.3.8)

- **38 Built-In Toolbar Icon Packs**: Modern Ribbon (Default Vector), Microsoft Fluent UI (Win 11), Windows 11 (Fluent 2), Windows 7 (Aero Glass), Windows XP (Luna Classic), Fugue Classic (Silk Suite - Authentic 3.5.6 by Yusuke Kamiyamane), Elegant Themes (Nicholas Roach), Notepad++ Classic (Nostalgia), NotepadNext Classic (Silk), Monochrome Outline (Minimalist), Vibrant DuoTone (Cyberpunk), Material Design (Sharp Vector), Pixel Design (Fugue-like), Monoline Minimal, Notepad++ (Modern Green), Notepad3 (Warm Amber), Seven Classic, Mignon, Palette, Diagonal, plus 16 Aksara Icon Packs (Bold, Duotone, Outline, Rounded, Sharp, macOS Big Sur, Windows XP/Vista/10/11, Vista Aero, Ubuntu Yaru, GNOME 51, KDE 6, Arch Linux, Elementary OS).
- **Multi-DPI Retina Scaling (16–64px) & Dynamic Dark Mode Tone Mapping**: Native scalable SVG vectors without raster blur, multi-resolution PNGs with `@2x` Retina DPR (`setDevicePixelRatio(2.0)`), and intelligent HSL tone mapping for bright, vivid icons under both dark and light modes.
- **Dynamic Menubar & Context Menu Synchronization**: Real-time whole-window icon propagation (`ToolbarIconManager::applyToWindow()` and `applyToMenu()`).
- **Interactive Tab Bar Plus (+) Button**: Direct document creation via title bar button next to editor tabs.
- **In-Tab Modern HTML & Markdown Preview (`PreviewTabWidget`)**: Embedded in-tab preview in central dock area (`ads::CenterDockWidgetArea`) with zoom controls, refresh, and system default browser preview.
- **Modern File Commander Browser**: Bookmarks bar (Home, Documents, Downloads, Desktop, Workspace), bottom storage space and item metrics bar, and dark/light mode parity.
- **Reversible Hex / Binary View**: Live toggle (`Ctrl+Alt+H`) displaying 16-byte hex dump and ASCII representation with zero-loss reversible buffer restoration.
- **100% Lexilla Parity (125 Languages)**: Complete adoption of all Lexilla lexers in `src/languages/`.
- **Localized Plaintext Documentation System**: Help menu Docnote and Changelog automatically load localized English or Indonesian documents matching the editor language.

### 1.9 Scintilla 5.6.9, Lexilla 5.5.5, Default Language, Qt 6.2 LTS & Multi-Architecture Distribution (v1.3.9)

- **Official Scintilla 5.6.9 & Lexilla 5.5.5 Upstream Integration**: 160 files updated, 3,206 insertions directly from upstream `scintilla.org` with full Unicode 17 mapping, full-color XPM rendering, accelerated line-start queries, and 40-byte lexeme caching.
- **Lexilla Memory Safety Hardening**: Patched buffer overflows (CWE-787) and out-of-bounds reads (CWE-125) across DMIS, Perl, Raku, Ruby, TCMD, TeX, Visual Prolog, and X12.
- **Markdown Front-Matter Syntax Styling**: Native styling for YAML (`---`), TOML (`+++`), and JSON (`{}`) front-matter metadata blocks in Markdown documents (`SCE_MARKDOWN_FRONT_MARK`, `SCE_MARKDOWN_FRONT`, `SCE_MARKDOWN_FRONT_KEY`).
- **Sandboxed Embedded Lua 5.5.1 Runtime**: Official Lua 5.5.1 runtime with unsafe OS/IO APIs stripped for high-assurance script isolation.
- **Default Language Preference for New Documents**: Configurable default programming language for untitled documents in Preferences > Editor.
- **Infinite-Loop Regex Replace-All Mitigation (CWE-835 / DoS)**: Enforced forward cursor progression preventing application hangs on zero-length regex matches (`^`, `$`, `\b`, lookarounds).
- **Search Dialog Direction Invariant**: Automatically unchecks and disables backwards direction search in Regular Expression mode.
- **Dynamic About Dialog Metadata**: Release Tag dynamically parameterized with `APP_VERSION`, engine versions synchronized to Scintilla 5.6.9 / Lexilla 5.5.5.
- **Full Qt 6.2 LTS Backward Compatibility**: Universal support for macOS Catalina 10.15 and Ubuntu 20.04 LTS via `QTimeZone::utc()`, `QLatin1String`, `parseColor`, and version-guarded cryptographic checks.
- **Multi-Architecture Linux ARM64 & x64 Release Matrix (50+ Packages)**: Automated native packaging for Linux ARM64 (`aarch64`) and x64 across DEB and RPM distributions, immediate per-job publishing, and lean DMG packaging with bloatware pruned.

### 1.10 30 Modern Icon Packs (69 Total), Context Menu Ergonomics, General Preferences & WCAG 2.1 Syntax Highlighting (v1.4.6)

- **30 New Modern Toolbar Icon Packs (69 Built-in Suites Total)**: Added 30 modern multi-resolution icon packs (16x16–64x64): `01-macos-big-sur`, `02-windows-11-fluent`, `03-fluent-glass`, `04-dark-neon-pro`, `05-modern-colorful`, `06-metallic-pro`, `07-material-design-3`, `08-neumorphism`, `09-claymorphism`, `10-aurora-gradient`, `11-bento-grid`, `12-cyber-y2k`, `13-glassmorphism-light`, `14-windows-xp`, `15-flat-20-squircle`, `16-techno-blueprint`, `17-pastel-dream`, `18-vaporwave`, `19-neo-brutalism`, `20-monochrome-steel`, `21-deep-space`, `22-liquid-glass`, `23-retro-mac-os-classic`, `24-holographic`, `25-earthy-organic`, `26-ai-generative`, `27-high-tech-carbon`, `28-synthwave-neon`, `29-fluent-acrylic`, `30-swiss-minimal-3d`.
- **Editor Context Menu Ergonomics**: Added **Paste & Match Style** (`Cmd+Alt+Shift+V` / `Ctrl+Shift+V`), **Search on Google** (`Cmd+Alt+G` / `Ctrl+Alt+G`), and direct **UPPERCASE** / **lowercase** conversion actions.
- **General Preferences Additions**: Toggle checkboxes for Character Encoding Auto-detection (`enableEncodingAutoDetection`) and Save All Confirmation dialog (`enableSaveAllConfirmation`).
- **WCAG 2.1 Contrast Safety & Syntax Highlighting Overhaul**: Mathematical luminance calculation guaranteeing 4.0:1 contrast ratio across all 70+ themes, High-visibility Gold Badges (`#FFD700`) for TODO/FIXME markers, and theme-synchronized indicator box.
- **Canonical 30MB–40MB Lightweight macOS DMG Standard**: Calibrated release DMG packaging strictly between 30MB and 40MB (38MB–39MB) preserving Qt frameworks and image plugins.

---

## 2. Capability Implementation Matrix

| Capability                                  | Status in v1.4.6 | Engine / Implementation                                               |
| :------------------------------------------ | :--------------: | :-------------------------------------------------------------------- |
| **Scintilla 5.6.9 Official Engine**         |   ✅ Complete    | Core text rendering canvas, fold margins, Unicode 17                  |
| **Lexilla 5.5.5 Official Lexer Library**    |   ✅ Complete    | Complete 125 language lexers with memory safety hardening             |
| **Markdown Front-Matter Syntax Styling**    |   ✅ Complete    | YAML (`---`), TOML (`+++`), JSON (`{}`) front-matter styling          |
| **Sandboxed Lua 5.5.1 Runtime**             |   ✅ Complete    | Official Lua 5.5.1 with unsafe OS/IO system APIs disabled             |
| **Default Language for New Documents**      |   ✅ Complete    | `ApplicationSettings::DefaultLanguage` in Preferences > Editor        |
| **Infinite-Loop Regex Fix (CWE-835)**       |   ✅ Complete    | `Finder::replaceAll()` with enforced forward cursor advancement       |
| **Dynamic About Dialog Metadata**           |   ✅ Complete    | `AboutDialog` dynamic `APP_VERSION` binding & Scintilla 5.6.9         |
| **Qt 6.2 LTS Universal Compatibility**      |   ✅ Complete    | macOS Catalina 10.15 & Ubuntu 20.04 support                           |
| **Multi-Arch Linux ARM64 & x64 Matrix**     |   ✅ Complete    | 50+ binary release packages across DEB, RPM, and DMG                  |
| **Lean macOS DMG Packaging (38–39 MB)**     |   ✅ Complete    | Strictly within 30MB–40MB ceiling with pre-baked `.DS_Store` layout   |
| **Toolbar Icon Packs Suite (69 Packs)**     |   ✅ Complete    | `ToolbarIconManager` (69 modern and classic multi-DPI icon packs)     |
| **Dynamic Menu & Context Menu Icon Sync**   |   ✅ Complete    | `ToolbarIconManager::applyToWindow()` & `applyToMenu()`               |
| **Paste & Match Style & Google Search**     |   ✅ Complete    | Editor context menu ergonomics with direct UPPER/lowercase actions    |
| **General Preferences Additions**           |   ✅ Complete    | Encoding auto-detection and save all confirmation toggle checks       |
| **WCAG 2.1 Contrast Syntax Highlighting**   |   ✅ Complete    | 4.0:1 contrast calculation, Gold Badges `#FFD700`, indicator boxes    |
| **Tab Bar Plus (+) Button**                 |   ✅ Complete    | `DockedEditorTitleBar` (`m_plusButton`)                               |
| **In-Tab HTML & Markdown Preview**          |   ✅ Complete    | `PreviewTabWidget` + `ads::CenterDockWidgetArea`                      |
| **Preview in Default Web Browser**          |   ✅ Complete    | `MainWindow::openInDefaultBrowser()` via `QDesktopServices`           |
| **Modern File Commander Browser**           |   ✅ Complete    | `FileBrowserDock` with bookmarks bar & storage metrics status bar     |
| **Reversible Hex / Binary View**            |   ✅ Complete    | `MainWindow::toggleHexBinaryView()` (`Ctrl+Alt+H`)                    |
| **100% Lexilla Lexers Parity (125 Lexers)** |   ✅ Complete    | `src/languages/*.lua` synced into `scripts/init.lua`                  |
| **Multi-DPI Icon Sizing (16x16–64x64)**     |   ✅ Complete    | `ToolbarIconManager::setIconSize()` (16, 20, 24, 32, 48, 64)          |
| **Omnibar Command Palette & Quick Open**    |   ✅ Complete    | `CommandPaletteDialog` (`Cmd+P`, `Cmd+Shift+P`)                       |
| **Side-by-Side Visual Diff**                |   ✅ Complete    | `FileDiffDialog` + pure C++20 `MyersDiffEngine`                       |
| **Project Tree Live Filter Bar**            |   ✅ Complete    | `FolderAsWorkspaceDock` filter bar & context menu                     |
| **Document Statistics & Shannon Entropy**   |   ✅ Complete    | `DocumentStatisticsDialog` & `TokenForensicUtils`                     |
| **XML / HTML & SQL Prettify / Minify**      |   ✅ Complete    | `CodeCompressPretty` streaming formatter                              |
| **JWT Token Inspector & Validator**         |   ✅ Complete    | `TokenForensicUtils::decodeJwt()`                                     |
| **Multi-Cursor Active Counter**             |   ✅ Complete    | `EditorInfoStatusBar` (`[N Cursors]`)                                 |
| **Mark All Occurrences + Match Count**      |   ✅ Complete    | Scintilla indicator marking + real-time status bar counter            |
| **Enhanced Auto-completion & Auto-close**   |   ✅ Complete    | Auto-close pairs `()`, `[]`, `{}`, `""`, `''`, context completion     |
| **File Browser Dock**                       |   ✅ Complete    | Integrated file tree explorer dock widget                             |
| **Base64 Encode / Decode**                  |   ✅ Complete    | Direct in-place selection encode/decode                               |
| **Number Conversion**                       |   ✅ Complete    | Dec ↔ Hex ↔ Oct ↔ Bin conversion                                      |
| **Insert GUID & Timestamps**                |   ✅ Complete    | Configurable GUID formats and timestamp insertion                     |
| **Full-Screen Mode**                        |   ✅ Complete    | Native macOS + Qt full-screen toggle                                  |
| **Color Preview CallTip**                   |   ✅ Complete    | Hex/RGB hover swatch preview via Scintilla CallTip                    |
| **Pretty & Compress (JSON/CSS/JS/XML/SQL)** |   ✅ Complete    | In-place formatters without external dependencies                     |
| **Focused View**                            |   ✅ Complete    | Filter editor buffer to display only matching lines                   |
| **Show Character Info**                     |   ✅ Complete    | Unicode code point, category, and byte sequence dump                  |
| **File Time Utilities**                     |   ✅ Complete    | Touch file time & save preserving original timestamp                  |
| **AES-256 File & Text Encryption**          |   ✅ Complete    | `CryptoManager` with PBKDF2 100k + HMAC-SHA256                        |
| **Log Tail Monitoring (`tail -f`)**         |   ✅ Complete    | Non-modal real-time log monitoring engine                             |
| **JavaScript Expression Evaluator**         |   ✅ Complete    | Sandboxed JS expression and math evaluation                           |
| **Application Restart & Explicit Exit**     |   ✅ Complete    | `MainWindow::restartApplication()` and clean exit                     |
| **Preferences Themes & Toolbar Tab**        |   ✅ Complete    | `PreferencesDialog` (Themes & Toolbar Icon Packs with card layout)    |
| **Preferences Custom Toolbar Config Tab**   |   ✅ Complete    | `PreferencesDialog` (Embedded visual toolbar customizer widget)       |
| **Standard Desktop Editor Menu Hierarchy**  |   ✅ Complete    | `MainWindow` (Hex/Binary below Compare, macOS app menu Restart)       |
| **System Transfer (.sparkbundle)**          |   ✅ Complete    | `SparkTransferManager` & `TransferBundleDialog` with Zip-Slip defense |
| **POSIX Shell Inspector & Touch Utility**   |   ✅ Complete    | `PosixFileInspector`, shebang dialect, octal/symbolic, `chmod +x`     |
| **Stateless 3-Point Encoding Detector**     |   ✅ Complete    | `EncodingDetector` (header, middle, tail 16KB), UTF-32 LE/BE BOM      |
| **Atomic File Save & Sibling Tmp Guard**    |   ✅ Complete    | `ScintillaNext::writeToDisk` with atomic rename and 0600 permissions  |
| **Multi-Mode Goto Dialog (Line/Char/Byte)** |   ✅ Complete    | `GotoDialog` with live preview and target pos calculation             |
| **Project Tree Safe Rename Preview**        |   ✅ Complete    | `FolderAsWorkspaceDock` safe rename with traversal & collision checks |
| **2025–2026 CVE Security Hardening**        |   ✅ Complete    | Immunity against CVE-2026-42214, 48778, 52885, 52886, 57233, etc.     |
| **Complete Indonesian Localization**        |   ✅ Complete    | 2,247 translations finished (100% complete, 0 unfinished, EYD V)      |
