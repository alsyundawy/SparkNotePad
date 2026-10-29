# Implementation Plan: Full MIME Tools, Run Help Links, Custom Shortcuts, API Limit Fix & 13-Pillar Audit

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Implement the complete 16-action Notepad++ MIME Tools suite, integrate Developer Run Help entries (PHP, Rust, Python, Wikipedia), create an interactive Shortcut Customizer dialog in Settings and Preferences, eliminate GitHub API rate limits in ReleaseManager via unmetered fallback/ETag caching, update AboutDialog shortcuts, and execute 13-pillar static verification on branch `main`.

**Architecture:** Pure C++20 / Qt 6 modular architecture. MimeTools operations encapsulated in `src/utils/MimeTools.h/.cpp` with comprehensive unit tests. Shortcut customization managed via `ShortcutCustomizerDialog` persisting to `QSettings`. Rate-limit mitigation in `ReleaseManager` combining ETag caching and unmetered GitHub web redirect fallback (`Location` header).

**Tech Stack:** C++20, Qt 6.12 (Widgets, Network, Core5Compat), Scintilla 5.6.7, QKeySequence, QSettings, CMake, Ninja.

**Spec:** Direct user requirements and attached Notepad++ MIME Tools screenshot specification.

## Global Constraints

- Repository: `SparkNotePad` on branch `main`.
- Pure ISO C++20 standard compliance.
- No regression or removal of existing well-functioning features.
- Memory safety: Qt parent-child hierarchy and `QPointer` lifetime guards.
- No rate-limit failures on software update check.
- 100% clean trunk check and 0 compiler errors.

## Review Focus

1. Text buffer encoding: Multi-byte UTF-8 preservation in line-by-line Base64 and URL encoding.
2. Shortcut conflict detection: Prevent clobbering essential editor bindings without user confirmation.
3. Network safety: Offline mode compliance and TLS 1.2+ security invariants in update checking.
4. Memory lifecycle: Proper widget cleanup and non-blocking dialogs.
5. Invariant preservation: Invariants check script `verify-engine-invariants.sh` must remain 100% green.

---

### Task 1: Complete 16-Action Notepad++ MIME Tools Suite

**Files:**

- Create: `src/utils/MimeTools.h`
- Create: `src/utils/MimeTools.cpp`
- Modify: `src/CMakeLists.txt`
- Modify: `src/dialogs/MainWindow.cpp:1050-1125`
- Test: `tests/TestMimeTools.cpp`

- [ ] **Step 1.1: Create `src/utils/MimeTools.h` and `src/utils/MimeTools.cpp`**
      Implement the 16 MIME operations matching the user's screenshot:
  1. `Base64 Encode` (Standard RFC 4648)
  2. `Base64 Encode with padding` (Standard with `=` padding)
  3. `Base64 Encode with Unix EOL` (Wrap at 76 chars with `\n`)
  4. `Base64 Encode by line` (Encode each line independently)
  5. `Base64 Decode` (Standard RFC 4648 decode)
  6. `Base64 Decode strict` (Strict validation mode)
  7. `Base64 Decode by line` (Decode each line independently)
  8. `Quoted-printable Encode` (RFC 2045)
  9. `Quoted-printable Decode` (Decode `=XX` and soft breaks)
  10. `URL Encode (RFC1738)` (Alphanumeric and `$-_.+!*'(),` safe; space as `+`)
  11. `URL Encode (RFC1738) by line`
  12. `URL Encode (Extended)` (RFC 3986 percent-encoding)
  13. `URL Encode (Extended) by line`
  14. `URL Encode (Full)` (Encode all characters to `%XX`)
  15. `URL Encode (Full) by line`
  16. `URL Decode` (Decode both `%XX` and `+`)
- [ ] **Step 1.2: Add unit tests in `tests/TestMimeTools.cpp`**
      Add unit tests validating RFC test vectors for all 16 transformations.
- [ ] **Step 1.3: Update `MainWindow::setupToolsMenu()` in `src/dialogs/MainWindow.cpp`**
      Populate `menuMimeTools` with all 16 actions organized with separators matching the exact visual layout from the attached image.

---

### Task 2: Navigation Run Menu Help Actions & Shortcuts

**Files:**

- Modify: `src/dialogs/MainWindow.h`
- Modify: `src/dialogs/MainWindow.cpp:4054-4133`

- [ ] **Step 2.1: Add slot declarations and actions for documentation lookup**
  - "Get PHP Help" (`Alt+F1` / Mac `Opt+F1`) -> `https://www.php.net/manual-lookup.php?pattern=` + selection (or `https://www.php.net/`).
  - "Rust Documentation" (`Alt+F2` / Mac `Opt+F2`) -> `https://rust-lang.org/learn/`.
  - "Python Documentation" (`Alt+F3` / Mac `Opt+F3`) -> `https://www.w3schools.com/python/`.
  - "Wikipedia Search" (`Alt+Shift+F1` / `Ctrl+Alt+W`) -> `https://en.wikipedia.org/wiki/Special:Search?search=` + selection.
- [ ] **Step 2.2: Wire actions into `MainWindow::setupRunMenu()`**
      Insert actions under a dedicated "Developer Help & Web Reference" section in the Run menu with appropriate icons and platform-aware shortcut mappings.

---

### Task 3: Interactive Custom Shortcut Mapper & Settings Integration

**Files:**

- Create: `src/dialogs/ShortcutCustomizerDialog.h`
- Create: `src/dialogs/ShortcutCustomizerDialog.cpp`
- Modify: `src/dialogs/MainWindow.h`
- Modify: `src/dialogs/MainWindow.cpp`
- Modify: `src/dialogs/PreferencesDialog.h`
- Modify: `src/dialogs/PreferencesDialog.cpp`
- Modify: `src/dialogs/PreferencesDialog.ui`
- Modify: `src/dialogs/AboutDialog.cpp:489-516`

- [ ] **Step 3.1: Create `ShortcutCustomizerDialog`**
  - Implement a dialog with a filterable `QTableWidget` / `QTreeWidget` listing all MainWindow actions categorized by Menu.
  - Provide a `QKeySequenceEdit` control for key capture.
  - Implement conflict detection (warning when a key sequence is already assigned).
  - Persist custom shortcuts to `QSettings("SparkNotePad", "Shortcuts")` and provide a "Reset to Defaults" action.
- [ ] **Step 3.2: Hook shortcut dialog into Settings Menu and Preferences Dialog**
  - Add `actionCustomizeShortcuts` ("Customize Shortcuts...") to `ui->menuSettings`.
  - Add "Shortcuts & Key Bindings..." button in `PreferencesDialog`.
  - Implement `applyCustomShortcuts()` on application initialization.
- [ ] **Step 3.3: Update shortcut documentation in `AboutDialog::createShortcutsTab()`**
      Update the Shortcuts table in the About dialog to list all active shortcuts including the new Run and Tools shortcuts.

---

### Task 4: Fix GitHub API Rate Limiting in `ReleaseManager`

**Files:**

- Modify: `src/features/ReleaseManager.h`
- Modify: `src/features/ReleaseManager.cpp:80-142`

- [ ] **Step 4.1: Implement ETag and `If-None-Match` in `ReleaseManager`**
      Store the ETag from GitHub responses in `QSettings` and attach `If-None-Match` to avoid consuming rate limits when no new release exists.
- [ ] **Step 4.2: Implement unmetered web redirect fallback on HTTP 403**
      If `api.github.com` returns 403 (Rate Limit Exceeded), perform a HEAD/GET request to `https://github.com/alsyundawy/SparkNotePad/releases/latest` without redirect following to extract the redirect `Location` header (`.../releases/tag/vX.Y.Z`), resolving the newest release version without consuming API tokens.

---

### Task 5: 13-Pillar Code Quality Review & Linter Verification

**Files:**

- Audit: All `src/*.cpp` and `src/*/*.cpp`
- Verify: `clangd --check`, `cppcheck`, `trunk check --no-fix`

- [ ] **Step 5.1: Run `clangd --check` and `cppcheck`**
      Verify 0 true compiler or AST diagnostic errors across all source files.
- [ ] **Step 5.2: Execute `trunk check --no-fix`**
      Ensure formatting and linters pass cleanly.
- [ ] **Step 5.3: Run test suites**
      Execute `scripts/verify-engine-invariants.sh`, `scripts/run-unit-tests.sh`, and `scripts/run-security-audit.sh`.

---

### Task 6: Documentation, Changelog & Local DMG Verification

**Files:**

- Modify: `README.md`
- Modify: `README-id.md`
- Modify: `DOCNOTE.md`
- Modify: `CHANGELOG.md`
- Output: `build/SparkNotePad-v1.3.7-x64.dmg`

- [ ] **Step 6.1: Update `README.md`, `README-id.md`, `DOCNOTE.md`, `CHANGELOG.md`**
      Document the complete 16-action MIME Tools, Developer Run Help, Shortcut Customizer, and API limit resolution.
- [ ] **Step 6.2: Rebuild local DMG and verify signature and size**
