# SparkNotePad Workspace Instructions for AI Agents

## Mandatory Build Rules

1. **PERMANENT PROHIBITION OF NINJA**:
   - DO NOT use `ninja`, `-G Ninja`, or `cmake-ninja` under any circumstances.
   - Ninja causes severe CPU thermal spikes and laptop overheating.
   - All builds must use standard CMake with default generators (Unix Makefiles).

2. **STANDARD CMAKE USAGE**:
   - Configuration: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
   - Build: `cmake --build build` (or controlled jobs `cmake --build build -j2`)
   - Test: `ctest --test-dir build --output-on-failure` or `./build/tests/SparkNotePadTests`

3. **PERMANENT LIGHTWEIGHT DMG PACKAGING STANDARD (STRICTLY ~30MB–40MB CEILING)**:
   - **Zero Bloatware Mandate**: Under NO circumstances may unused Qt frameworks (`QtPdf.framework`, `QtQml.framework`, `QtQuick.framework`, `QtVirtualKeyboard.framework`, `QtVirtualKeyboardQml.framework`, `libqtvirtualkeyboardplugin.dylib`, extra redundant dylibs) be bundled into macOS release DMGs.
   - **Binary Stripping**: Always strip local binary symbols from `SparkNotePad.app/Contents/MacOS/SparkNotePad` using `strip -x`.
   - **Strict Size Range**: The packaged DMG must strictly remain between 30MB and 40MB (hard ceiling: 40MB / 41,943,040 bytes, floor: 30MB / 31,457,280 bytes). Any DMG build exceeding 40MB or falling below 30MB is considered defective or over-pruned and will fail build verification.

4. **PERMANENT DMG BRANDING & BACKGROUND PRESERVATION**:
   - **Canonical Packager**: Always package macOS DMGs using `scripts/package-dmg.sh` via native `hdiutil` with the pre-baked layout preset (`deploy/macos/dmg-ds-store`, 10,244 bytes).
   - **Prohibition of Flaky AppleScript Tools**: Never invoke unverified `create-dmg` AppleScripts that strip `backgroundImageAlias` or fail on modern macOS.
   - **Required Presentation Assets**: Every DMG bundle MUST preserve:
     - `.background/dmg-background.png` and `.background/dmg-background@2x.png`
     - Pre-baked `.DS_Store` containing `backgroundImageAlias`, 680x440 window bounds, and icon coordinates
     - `Applications -> /Applications` drag-and-drop symlink
     - `.VolumeIcon.icns` with custom volume attributes
   - **Automated Verification Gate**: `scripts/package-dmg.sh` automatically mounts the output DMG, asserts that `backgroundImageAlias` is present and valid, checks file size <= 40MB (and >= 30MB), and validates CRC32 checksum via `hdiutil verify`.

5. **STRICT GIT DISCIPLINE & ZERO-TRASH COMMITS**:
   - Do not create superfluous, noisy, or scattered git commits ("no nyampah git commits").
   - Every commit must be atomic, purposeful, and follow Conventional Commits formatting (`feat(...)`, `fix(...)`, `docs(...)`).

6. **PERMANENT DARK MODE TOOLBAR IN ALL THEMES AND MODES**:
   - The primary toolbar background (`#mainToolBar`, `QToolBar`) must ALWAYS remain dark mode (`#21252b` / dark palette) across ALL modes (light, dark, auto) and themes.
   - Pastel toolbar icons must never be rendered on a light or white background.
   - Any toolbar styling must use `setAutoFillBackground(true)` and high-specificity QSS with `!important` to prevent native Cocoa or light window background bleed-through.

## Agent Identity & Protocol

- In this workspace, the agent responds to the maintainer without excuses, focusing purely on zero-bloat, precision execution, and immediate problem resolution.
