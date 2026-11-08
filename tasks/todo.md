# SparkNotePad v1.4.5 Actionable Task List

- [x] Task 1: Fix SonarLint `cpp:S6004` in `src/dialogs/MainWindow.cpp` (init-statement for `target` in if statements at lines 3874, 3885).
- [x] Task 2: Fix SonarLint issues in `src/docks/MarkdownSafePreviewDock.cpp`:
  - [x] `cpp:S6004`: Init-statement for `savedThemeName` (line 69) and `snippet` (line 265).
  - [x] `cpp:S6177`: Use `using enum MarkdownSafePreviewDock::PreviewFormat;` (line 558).
  - [x] `cpp:S5350`: Pointer-to-const for `scrollBar` (line 818).
  - [x] `cpp:S3358`: Extract nested ternary operators into independent statements (lines 397-402).
- [x] Task 3: Fix SonarLint `cpp:S1448` in `src/docks/MarkdownSafePreviewDock.h` (method count rule).
- [x] Task 4: Fix SonarLint `cpp:S6004` in `src/features/ThemeManager.cpp` (init-statement for `savedPalette`).
- [x] Task 5: Verify build completion via `cmake --build build -j2` without errors.
- [x] Task 6: Run test suite via `ctest --test-dir build --output-on-failure`.
- [x] Task 7: Build local DMG via `scripts/package-dmg.sh` and verify < 35MB size + background presentation.
- [x] Task 8: Update README.md, README-id.md, doc/, wiki/, and CHANGELOG.md for v1.4.5 release.
- [x] Task 9: Clean atomic commit and branch synchronization (`main`, `1.4.5`, `v1.4.5`).
