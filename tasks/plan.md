# Implementation Plan: SparkNotePad v1.3.8 Comprehensive Quality Audit, i18n & Release

## Overview

Comprehensive quality audit, static analysis, translation completion, documentation rebuild, and release pipeline execution for SparkNotePad v1.3.8 across 5 skills (`planning-and-task-breakdown`, `production-code-review`, `ide-linter-error-fix`, `verification-before-completion`, `bash-pro`) and 14 review pillars:

1. Bug Review
2. Syntax Review
3. Runtime Review
4. Logic Review
5. Memory / Resource Review
6. Dead Code Review
7. Duplicate Code Review
8. Circular Dependency Review
9. Performance Bottlenecks Review
10. Security Vulnerability Review
11. Maintainability Review
12. Scalability Review
13. Readability Review
14. Architecture / Design / Completeness Review

## Architecture & Policy Decisions

- **Strict Repository Identity Separation**: This repository is `SparkNotePad`. Upstream/ancestral repositories such as `NotepadNext-MacOS`, `NotepadNext`, `Notepad4`, and `Notepad3` are strictly recognized as distinct external projects in attribution, credits, and documentation.
- **Permanent Prohibition of Ninja**: As mandated by `AGENTS.md`, CMake builds MUST use Unix Makefiles with controlled concurrency (`-j2`) to protect the host MacBook from CPU thermal throttling.
- **i18n Translation Completeness**: Both English (`i18n/SparkNotePad_en.ts`) and Indonesian (`i18n/SparkNotePad_id.ts`) must have 100% translation coverage with 0 unfinished entries (`type="unfinished"`).
- **Mandatory Static Analysis Suite**: Every single `.cpp` file under `src/` and subdirectories will be audited using `clangd --check`, `cppcheck`, `clang-tidy`, SonarLint (`sonar-restart-ext.sh`), and `trunk`. Any detected warnings or errors must be fixed atomically on the spot.
- **Release Lineage**: All release changes will be committed and tagged on branch `1.3.8`, and release files must be those produced by the official GitHub Actions runner.

## Task Breakdown

### Phase 1: i18n Translation Double-Check & Synchronization

- [x] Task 1.1: Run `lupdate` across `src/` to capture all strings (including new preview tabs, icon packs, docnote/changelog viewers).
- [x] Task 1.2: Translate all newly discovered and unfinished strings in `i18n/SparkNotePad_en.ts` and `i18n/SparkNotePad_id.ts`.
- [x] Task 1.3: Verify 0 unfinished entries via `grep 'type="unfinished"'` and compile `.qm` files via `lrelease`.

### Phase 2: Upstream Sources & About/Credits Section Update

- [x] Task 2.1: Audit git commit logs and history for all historical and upstream sources (`NotepadNext-MacOS`, `NotepadNext`, `Scintilla`, `Lexilla`, `uchardet`, `ADS`, `QSimpleUpdater`, `SingleApplication`, `EditorConfig`).
- [x] Task 2.2: Update `src/dialogs/AboutDialog.cpp` to include `NotepadNext-MacOS` repository link (`https://github.com/alsyundawy/NotepadNext-MacOS`), update credits header to `(v1.0.0 – v1.3.8)`, and ensure proper translations.
- [x] Task 2.3: Synchronize `doc/ABOUT_AND_CREDITS.md` and `README.md` with accurate attribution and clear non-affiliation statements.

### Phase 3: Static Analysis & Deep Compiler Checks

- [x] Task 3.1: Run `clangd --check` on all `.cpp` files in `src/*.cpp` and `src/*/*.cpp` using `compile_commands.json`.
- [x] Task 3.2: Run `cppcheck --enable=all` with suppressed noise to identify potential null dereferences, buffer overflows, or uninitialized variables.
- [x] Task 3.3: Run `clang-tidy` and SonarLint verification loop (restarting SonarLint Language Server via `sonar-restart-ext.sh`).
- [x] Task 3.4: Apply atomic remediation to any detected warnings, syntax flaws, or diagnostic errors immediately.

### Phase 4: 14-Pillar Comprehensive Code Review Checklist

- [x] Task 4.1: Perform structured audit across all 14 pillars on core subsystems (`src/`, `src/dialogs/`, `src/docks/`, `src/editor/`, `src/utils/`, `src/widgets/`).
- [x] Task 4.2: Generate a formal 14-pillar review checklist and findings report artifact.

### Phase 5: Meta-Linter & Build Verification

- [x] Task 5.1: Run `trunk check` and `lint-check.sh` on all modified files (`.cpp`, `.h`, `.ts`, `.md`, `.json`, `.lua`).
- [x] Task 5.2: Execute standard CMake configuration and build with Unix Makefiles: `cmake --build build -j2`.
- [x] Task 5.3: Run unit tests via `ctest --test-dir build --output-on-failure`.

### Phase 6: Documentation Rebuild, Version 1.3.8 Sync & Release

- [x] Task 6.1: Update `README.md`, `README-id.md`, `wiki/*.md`, and all `.md` files in `doc/` to reflect v1.3.8 features.
- [x] Task 6.2: Rebuild localized plaintext docnotes and changelogs (`DOCNOTE_en.txt`, `DOCNOTE_id.txt`, `CHANGELOG_en.txt`, `CHANGELOG_id.txt`).
- [x] Task 6.3: Ensure branch `1.3.8` is fully synchronized, committed with clean conventional commit messages, pushed to `origin/1.3.8`, and tagged for the GitHub Actions runner release.

### Phase 7: Distinct Toolbar Icon Packs Integration & Diagnostics Remediation

- [x] Task 7.1: Synchronize all 13 distinct toolbar icon packs (with resolutions `16x16`, `20x20`, `24x24`, `32x32`, `48x48`, `64x64`) from `/Users/alsyundawy/Downloads/SparkNotePad_Toolbar_IconPacks` into `src/icons/packs/`.
- [x] Task 7.2: Generate `src/toolbar_icons.qrc` containing all 2,496 multi-DPI icons and link it in `src/CMakeLists.txt` and `tests/CMakeLists.txt`.
- [x] Task 7.3: Enhance `ToolbarIconManager` to seamlessly support multi-resolution folder lookups, register all 13 new packs with descriptive names, and maintain fallback aliases.
- [x] Task 7.4: Remediate all IDE warnings and diagnostics in `@[current_problems]` (`ThemePaletteData.h`, `ManageToolsDialog.cpp`, `MainWindow.cpp`).
- [x] Task 7.5: Update and expand unit tests in `tests/test_toolbar_icon_manager.h` and verify all tests pass via `ctest`.
- [x] Task 7.6: Compile local test build and package DMG (`cmake --build build -j2`), verify with `hdiutil`, and commit & push.

## Risks and Mitigations

| Risk                                         | Impact | Mitigation                                                                           |
| :------------------------------------------- | :----- | :----------------------------------------------------------------------------------- |
| High CPU temperature / fan noise             | High   | Strict prohibition of Ninja; build with standard CMake Unix Makefiles and `-j2`.     |
| Unfinished translation tags breaking release | Med    | Automated verification via `grep -c 'type="unfinished"' i18n/*.ts` asserting 0.      |
| Clangd false positives on Qt MOC headers     | Med    | Pass `--compile-commands-dir=build` and check real diagnostic severity.              |
| Branch confusion between main and 1.3.8      | High   | Keep commit history linear; update 1.3.8 branch cleanly and verify git ref pointers. |
| Multi-DPI toolbar icon blurriness on Retina  | Med    | Include native 16x16 through 64x64 pixmaps in `QIcon` for pixel-perfect scaling.     |
