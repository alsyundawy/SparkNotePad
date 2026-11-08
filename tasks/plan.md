# SparkNotePad v1.4.5 Master Execution & Verification Plan

## 1. Overview & Operational Principles

- **Target Repository**: Strictly `SparkNotePad` (`io.github.alsyundawy.SparkNotePad`).
- **Generator Enforcement**: Standard Unix Makefiles with `-j2` jobs allocation (`cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j2`). Permanent prohibition of Ninja (`-G Ninja`).
- **14-Pillar Quality Gate**:
  1. Bug Review
  2. Syntax Review
  3. Runtime Review
  4. Logic Review
  5. Memory Review
  6. Dead Code Review
  7. Duplicate Code Review
  8. Circular Dependency Review
  9. Performance Bottleneck Review
  10. Security Vulnerability Review
  11. Maintainability Review
  12. Scalability Review
  13. Readability Review
  14. Architectural Review

---

## 2. Execution Phases

### Phase 1: Linter & SonarLint Remediation

- [x] Fix `MainWindow.cpp` lines 3874 & 3885: Use init-statement in `if` (`cpp:S6004`).
- [x] Fix `MarkdownSafePreviewDock.cpp` line 69: Use init-statement for `savedThemeName` (`cpp:S6004`).
- [x] Fix `MarkdownSafePreviewDock.cpp` line 265: Use init-statement for `snippet` (`cpp:S6004`).
- [x] Fix `MarkdownSafePreviewDock.cpp` line 558: Use `using enum` for `PreviewFormat` (`cpp:S6177`).
- [x] Fix `MarkdownSafePreviewDock.cpp` line 818: Make `scrollBar` pointer-to-const (`cpp:S5350`).
- [x] Fix `MarkdownSafePreviewDock.cpp` lines 397-402: Extract nested ternary operators into independent statements (`cpp:S3358`).
- [x] Fix `ThemeManager.cpp`: Use init-statement for `savedPalette` (`cpp:S6004`).
- [x] Fix `MarkdownSafePreviewDock.h`: Method count governance / NOSONAR justification (`cpp:S1448`).

### Phase 2: Core Platform & Ecosystem Alignment

- [x] Integrate complete IANA Media Types registry into `SparkNotePad-Default-Editor.sh` and `SparkNotePad-Default-Editor-Linux.sh`.
- [x] Synchronize root scripts with `scripts/` directory copies.
- [x] Synchronize FreeDesktop desktop launcher MIME types in `deploy/linux/SparkNotePad.desktop`.
- [x] Guarantee permanent dark mode toolbar in all themes and modes via `setAutoFillBackground(true)` and `#mainToolBar` QSS rules.

### Phase 3: Build & Test Verification

- [x] Complete clean CMake build (`cmake --build build -j2`).
- [x] Execute test suite via `ctest --test-dir build --output-on-failure`.
- [x] Package local release DMG via `scripts/package-dmg.sh`.
- [x] Assert strict DMG size ceiling: 30MB–34MB (hard ceiling < 35MB).
- [x] Assert DMG background preservation (`backgroundImageAlias`, 680x440 layout, icons).

### Phase 4: Documentation & Release Synchronization

- [ ] Update `README.md` and `README-id.md`.
- [ ] Update `wiki/` and `doc/` guides.
- [ ] Rebuild `DOCNOTE.md` and `CHANGELOG.md` for v1.4.5.
- [ ] Synchronize git branches `main`, `1.4.5`, and tag `v1.4.5`.
