# SparkNotePad v1.4.6 — 30 New Modern Toolbar Icon Packs (69 Total), Context Menu Ergonomics, General Preferences, WCAG 2.1 Contrast Safety & Standard 30MB–40MB DMG Packaging

**Release Version:** `v1.4.6`<br/>
**Release Date:** 2026-11-10<br/>
**Maintainer:** [HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)<br/>
**Core Editing Engine:** Scintilla 5.6.9 (Vendored Official) & Lexilla 5.5.5 (Vendored Official)<br/>
**Embedded Lua Core:** Sandboxed Lua 5.5.1 (Official Source)<br/>
**Scripting Runtime:** Python 3.13+ Engine Bridge<br/>
**Standard:** ISO C++20 / Qt 6.5–6.12<br/>
**Target Platforms:** macOS 11.0–26.0 (Universal: Apple Silicon ARM64 & Intel x86_64) | Linux (Ubuntu 22.04–26.04, Debian 12–14, Fedora 40–44, RHEL/Rocky 9–11, openSUSE, Slackware) | FreeBSD 13.x / 14.x / 15.x (KDE Plasma 6 & XFCE)

---

## ⚡ Executive Summary

SparkNotePad **v1.4.6** delivers major enhancements across visual customization, editor context menu ergonomics, general preferences options, WCAG 2.1 contrast mathematical safety for syntax highlighting, and canonical 30MB–40MB macOS DMG packaging:

1. **30 New Modern Toolbar Icon Packs (69 Built-in Suites Total)**:
   - Added 30 high-resolution multi-scale icon packs (16x16, 20x20, 24x24, 32x32, 48x48, 64x64):
     `01-macos-big-sur`, `02-windows-11-fluent`, `03-arc-browser-glow`, `04-nordic-frost-minimal`, `05-dracula-cyber-neon`, `06-catppuccin-macchiato`, `07-tokyo-night-storm`, `08-synthwave-80s`, `09-monokai-pro-vivid`, `10-solarized-accent`, `11-retro-commodore-64`, `12-retro-win95-classic`, `13-matrix-phosphor-green`, `14-gameboy-nostalgia`, `15-pixel-dungeon-rpg`, `16-claymorphism-3d`, `17-skeuomorphic-leather-brass`, `18-neu-brutalism-bold`, `19-isometric-voxel-3d`, `20-origami-paper-fold`, `21-cyberpunk-2077-yellow`, `22-lofi-aesthetic-grain`, `23-vaporwave-pastel-sunset`, `24-astronomy-deep-space`, `25-botanical-nature-green`, `26-blueprint-schematic`, `27-chalkboard-handdrawn`, `28-gouache-art-paint`, `29-stained-glass-mosaic`, `30-swiss-minimal-3d`.
   - Guaranteed pure isolated icon graphics floating cleanly on the permanent dark toolbar (`#21252b`) with 18% breathing margins preventing edge clipping.

2. **Editor Context Menu Ergonomics**:
   - Added **Paste & Match Style** (`pasteAndMatchStyle`) to strip formatting and match current editor indentation.
   - Added **Search on Google** (`searchOnGoogle`) to directly query selected text or word under cursor in default browser.
   - Streamlined case conversion to direct **UPPERCASE** and **lowercase** actions without multi-tier submenus.
   - Removed cluttered Run and Bash encode/decode actions from right-click context menu.

3. **General Preferences Additions**:
   - Added option to Enable / Disable Character Encoding Auto-detection (`enableEncodingAutoDetection`).
   - Added option to Enable / Disable Save All Confirmation dialog (`enableSaveAllConfirmation`).

4. **WCAG 2.1 Contrast Safety & Syntax Highlighting Overhaul**:
   - Integrated WCAG 2.1 relative luminance and contrast calculations guaranteeing minimum 4.0:1 contrast ratio across all 70+ themes.
   - Semantic styling tailored to language families (C/C++, Python, Web/HTML/JS/CSS, Markdown progressive H1-H6, SQL, Shell, YAML, etc.).
   - Gold Badges (`#FFD700`, bold) for TODO, FIXME, BUG, HACK, NOTE.
   - Warm golden amber for string escape sequences (`\n`, `\t`, `\xHH`).
   - Theme-synchronized rounded border box indicator for SmartHighlighter.

5. **Canonical 30MB–40MB macOS DMG Packaging**:
   - Calibrated release DMG packaging to 38MB–39MB while retaining universal Qt libraries and standard image format plugins (`libqjpeg`, `libqgif`, `libqwebp`).
   - Automated verification gate with valid CRC32 checksum, custom retina background, layout preset `.DS_Store` (680x440), and runtime launch test.

6. **Static Analysis & 14-Pillar Code Quality Gates**:
   - `cppcheck`: 171/171 files checked across `src/*.cpp` and `src/*/*.cpp` with 0 errors and 0 warnings.
   - `clangd --check` and `clang-tidy` passing cleanly with zero compiler diagnostics.
   - 100% unit test pass rate (`SparkNotePadUnitTests`).
