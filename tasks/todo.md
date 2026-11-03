# SparkNotePad v1.3.8 Task List

## Phase 1: i18n Translation Double-Check & Synchronization

- [x] Task 1.1: Run `lupdate` across `src/` to capture all strings (including preview tabs, icon packs, docnote/changelog viewers).
- [x] Task 1.2: Translate all newly discovered and unfinished strings in `i18n/SparkNotePad_en.ts` and `i18n/SparkNotePad_id.ts`.
- [x] Task 1.3: Verify 0 unfinished entries via `grep 'type="unfinished"'` and compile `.qm` files via `lrelease`.

## Checkpoint 1: i18n Translation Parity

- [x] Both English and Indonesian translation files have 0 unfinished strings.
- [x] All `.qm` binary catalogs compiled without errors.

## Phase 2: Upstream Sources & About/Credits Section Update

- [x] Task 2.1: Audit git commit logs and history for all historical and upstream sources (`NotepadNext-MacOS`, `NotepadNext`, `Scintilla`, `Lexilla`, `uchardet`, `ADS`, `QSimpleUpdater`, `SingleApplication`, `EditorConfig`).
- [x] Task 2.2: Update `src/dialogs/AboutDialog.cpp` to include `NotepadNext-MacOS` repository link (`https://github.com/alsyundawy/NotepadNext-MacOS`), update credits header to `(v1.0.0 – v1.3.8)`, and ensure proper translations.
- [x] Task 2.3: Synchronize `doc/ABOUT_AND_CREDITS.md` and `README.md` with accurate attribution and clear non-affiliation statements.

## Checkpoint 2: Attribution & Credits Accuracy

- [x] `AboutDialog` and documentation explicitly reference `NotepadNext-MacOS` with link.
- [x] `SparkNotePad` repository identity is pristine and distinct.

## Phase 3: Static Analysis & Deep Compiler Checks

- [x] Task 3.1: Run `clangd --check` on all `.cpp` files in `src/*.cpp` and `src/*/*.cpp` using `compile_commands.json`.
- [x] Task 3.2: Run `cppcheck --enable=all` with suppressed noise to identify potential null dereferences, buffer overflows, or uninitialized variables.
- [x] Task 3.3: Run `clang-tidy` and SonarLint verification loop (restarting SonarLint Language Server via `sonar-restart-ext.sh`).
- [x] Task 3.4: Apply atomic remediation to any detected warnings, syntax flaws, or diagnostic errors immediately.

## Checkpoint 3: Static Analysis Zero Defect

- [x] `clangd --check` passes with zero critical diagnostic errors.
- [x] `cppcheck` reports 0 issues.
- [x] SonarLint and IDE diagnostics show 0 errors across touched files.

## Phase 4: 14-Pillar Comprehensive Code Review Checklist

- [x] Task 4.1: Perform structured audit across all 14 pillars on core subsystems (`src/`, `src/dialogs/`, `src/docks/`, `src/editor/`, `src/utils/`, `src/widgets/`).
- [x] Task 4.2: Generate a formal 14-pillar review checklist and findings report artifact.

## Checkpoint 4: 14-Pillar Audit Sign-off

- [x] All 14 pillars reviewed and verified against evidence.
- [x] Findings and remediations documented in formal artifact.

## Phase 5: Meta-Linter & Build Verification

- [x] Task 5.1: Run `trunk check` and `lint-check.sh` on all modified files (`.cpp`, `.h`, `.ts`, `.md`, `.json`, `.lua`).
- [x] Task 5.2: Execute standard CMake configuration and build with Unix Makefiles: `cmake --build build -j2`.
- [x] Task 5.3: Run unit tests via `ctest --test-dir build --output-on-failure`.

## Checkpoint 5: Build & Tests Green

- [x] CMake Unix Makefiles build succeeds with exit code 0.
- [x] All unit tests pass with 0 failures.
- [x] Meta-linters report 0 errors.

## Phase 6: Documentation Rebuild, Version 1.3.8 Sync & Release

- [x] Task 6.1: Update `README.md`, `README-id.md`, `wiki/*.md`, and all `.md` files in `doc/` to reflect v1.3.8 features.
- [x] Task 6.2: Rebuild localized plaintext docnotes and changelogs (`DOCNOTE_en.txt`, `DOCNOTE_id.txt`, `CHANGELOG_en.txt`, `CHANGELOG_id.txt`).
- [x] Task 6.3: Ensure branch `1.3.8` is fully synchronized, committed with clean conventional commit messages, pushed to `origin/1.3.8`, and tagged for the GitHub Actions runner release.

## Final Release Gate

- [x] Release branch `1.3.8` and tag `v1.3.8` pushed to origin.
- [x] GitHub Actions runner release workflow triggered and monitored.

## Phase 7: Distinct Toolbar Icon Packs Integration & Diagnostics Remediation

- [x] Task 7.1: Synchronize all 13 distinct toolbar icon packs (`16x16`, `20x20`, `24x24`, `32x32`, `48x48`, `64x64`) into `src/icons/packs/`.
- [x] Task 7.2: Generate `src/toolbar_icons.qrc` and register it in `src/CMakeLists.txt` and `tests/CMakeLists.txt`.
- [x] Task 7.3: Upgrade `ToolbarIconManager` with multi-resolution folder resolution, registration of all 13 packs, and backward-compatible aliasing.
- [x] Task 7.4: Remediate all IDE warnings in `@[current_problems]` (`ThemePaletteData.h`, `ManageToolsDialog.cpp`, `MainWindow.cpp`).
- [x] Task 7.5: Update and pass all unit tests in `tests/test_toolbar_icon_manager.h`.
- [x] Task 7.6: Compile local build and verify DMG package with `hdiutil`.

## Checkpoint 7: Icon Packs & Diagnostics Verification Gate

- [x] All 13 packs load crisp icons at all sizes (16, 20, 24, 32, 48, 64).
- [x] Zero compiler errors, zero linter issues, 100% tests passing.
- [x] DMG build verified and ready for testing.

## Phase 8: Markdown/HTML Preview Refinement, Menu Reorganization, Workspace Highlight Fix & Complete Diagnostics Clean

- [x] Task 8.1: Refine HTML & Markdown Live Preview in `MarkdownSafePreviewDock` & `PreviewTabWidget` with modern UI/UX, pill badges (format, word count, reading time), and enhanced GitHub/DevDocs CSS with callouts (`[!NOTE]`, `[!TIP]`, `[!WARNING]`, `[!IMPORTANT]`, `[!CAUTION]`), rounded code blocks, and export actions.
- [x] Task 8.2: Reorganize and optimize all menus in `MainWindow.ui` and `MainWindow.cpp` to match professional standard notepad layouts (File, Edit, Search, View, Encoding, Language, Tools, Macro, Help) with dedicated `Preview` and `Panels` sub-menus.
- [x] Task 8.3: Overhaul File Browser and Folder-as-Workspace docks (`FolderAsWorkspaceDock.cpp`, `FileBrowserDock.cpp`) to fix selection box cropping/clipping (`show-decoration-selected: 1`, `branch:selected`, full row focus, matching theme selection style) and modernize toolbar controls.
- [x] Task 8.4: Complete full static analysis and IDE diagnostic remediation across `src/` (`formatHexDump`, static member methods, brace omissions, const qualifiers, SonarLint rules) and restart SonarLint LS.
- [x] Task 8.5: Conduct 14-pillar code review audit and verify zero regression across unit tests (`./build/tests/SparkNotePadTests`).
- [x] Task 8.6: Rebuild local macOS DMG package (`cmake --build build --target dmg -j2` strictly with Unix Makefiles, no Ninja) and verify with `hdiutil verify`.
- [x] Task 8.7: Update documentation (`README.md`, `README-id.md`, `wiki/*.md`, `doc/`), commit, and push to branch `1.3.8`.

## Checkpoint 8: Final Feature Polish & Release Gate

- [x] HTML/Markdown preview provides modern, elegant live rendering with GitHub callouts and export options.
- [x] Standard notepad menu structure is organized, clean, and intuitive.
- [x] File Browser and Workspace docks have pixel-perfect, uncropped selection highlights matching the theme.
- [x] Zero linter warnings, zero compiler warnings, 100% tests passing.
- [x] Local DMG package rebuilt and validated with `hdiutil`.
