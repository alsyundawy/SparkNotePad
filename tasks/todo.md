# SparkNotePad v1.4.0 Task List & Actionable Checkpoints

## Phase 1: Scintilla 5.6.9 Change History & Line Spacing Integration

- [ ] Task 1.1: Implement Change History indicators in `src/ScintillaNext.cpp` and `src/ScintillaNext.h` using Scintilla 5.6.9 native `SCI_SETCHANGEHISTORY`.
- [ ] Task 1.2: Add fine-grained line spacing (`SCI_SETEXTRAASCENT` and `SCI_SETEXTRADESCENT`) in `ScintillaNext` and `ApplicationSettings`.
- [ ] Task 1.3: Update `PreferencesDialog.ui` and `PreferencesDialog.cpp` to provide toggles and spinboxes for Change History and Line Spacing.

## Checkpoint 1: Engine Features Active

- [ ] Change history markers display accurately in margin without overlapping bookmarks or folding.
- [ ] Extra line ascent and descent correctly adjust text line height in the editor.
- [ ] Preferences settings persist across editor restarts.

---

## Phase 2: macOS Title-Bar Proxy Icon & Enriched Tab Tooltips

- [ ] Task 2.1: Implement `setWindowFilePath` in `MainWindow.cpp` for active document macOS title-bar proxy icon support.
- [ ] Task 2.2: Add rich HTML tooltips to tabs in `DockedEditor.cpp` (path, size, line count, character count, encoding, EOL, last modified timestamp).

## Checkpoint 2: Desktop Integration & Tab UX

- [ ] macOS proxy icon displays active file and allows dragging to Terminal and Finder.
- [ ] Tab tooltips show complete, clean diagnostic metadata on hover.

---

## Phase 3: Search Enhancements & Batch Automation

- [ ] Task 3.1: Implement safe fuzzy subsequence pattern generation in `RegexSafetyEngine` and add "Fuzzy Match" checkbox in `FindReplaceDialog` and `QuickFindWidget`.
- [ ] Task 3.2: Implement batch macro processing in `MacroRunDialog` ("Run Macro on All Open Documents / Folder") with background worker and cancellation.

## Checkpoint 3: Search & Automation Verified

- [ ] Fuzzy search finds scattered subsequence tokens without regex backtracking hangs (ReDoS safe).
- [ ] Batch macro executes across target files with progress reporting and abort support.

---

## Phase 4: Diagnostic Transparency & Local MCP Tool Server

- [ ] Task 4.1: Implement "Help → Copy Debug Information to Clipboard" in `MainWindow.cpp`.
- [ ] Task 4.2: Implement local loopback-only, token-authenticated, read-only MCP server in `src/features/LocalMcpServer.cpp`.

## Checkpoint 4: Diagnostics & AI Pairing Active

- [ ] Debug Info clipboard copy outputs complete, accurate environment and configuration details.
- [ ] Local MCP server accepts local loopback connections, authenticates tokens, and exposes read-only tab data safely.

---

## Phase 5: Security Hardening & 2025–2026 CVE Mitigations

- [ ] Task 5.1: Harden link clicking in `NetworkSecurityPolicy` against CVE-2026-20841 (strict URI scheme allowlisting).
- [ ] Task 5.2: Enforce XML and JSON recursion depth caps in `ConfigIntegrityGuard` against CVE-2026-16762.
- [ ] Task 5.3: Ensure `QProcess` invocations in `ExternalToolManager` avoid shell string concatenation (CVE-2026-25926 mitigation).

## Checkpoint 5: Security Invariants Enforced

- [ ] Malicious URI schemes (`file://*.exe`, `cmd:`, `powershell:`) are intercepted and blocked.
- [ ] Deeply nested XML/JSON files fail gracefully without CPU or memory exhaustion.
- [ ] External tool execution is safe against shell command injection.

---

## Phase 6: i18n Translation & UI Text Synchronization

- [ ] Task 6.1: Run `lupdate src/ -ts i18n/SparkNotePad_en.ts i18n/SparkNotePad_id.ts`.
- [ ] Task 6.2: Translate all newly introduced strings for English and Indonesian (ensure 0 unfinished strings).
- [ ] Task 6.3: Compile translation catalogs using `lrelease`.

## Checkpoint 6: 100% Translation Parity

- [ ] Both `SparkNotePad_en.ts` and `SparkNotePad_id.ts` pass `grep 'type="unfinished"'` with 0 matches.
- [ ] Binary `.qm` files compile and load cleanly.

---

## Phase 7: 14-Pillar Code Review & Static Analysis Verification

- [ ] Task 7.1: Run `cppcheck --enable=all` and `clangd --check` on all modified source files.
- [ ] Task 7.2: Restart SonarLint language server (`sonar-restart-ext.sh`) and resolve all IDE diagnostics.
- [ ] Task 7.3: Verify complete compliance against all 14 review pillars (Bug, Syntax, Runtime, Logic, Memory, Dead Code, Duplicate Code, Circular Dependency, Performance, Security, Maintainability, Scalability, Readability, Architecture).

## Checkpoint 7: Zero-Defect Code Quality

- [ ] Zero compiler errors, zero warnings.
- [ ] SonarLint reports 0 BLOCKER, CRITICAL, or MAJOR issues.
- [ ] 14-pillar review checklist fully satisfied with empirical citations.

---

## Phase 8: CMake Build & Release Synchronization

- [ ] Task 8.1: Update project version to `1.4.0` in `CMakeLists.txt`.
- [ ] Task 8.2: Build project using CMake Unix Makefiles (`cmake --build build -j2`, NO NINJA).
- [ ] Task 8.3: Run unit tests (`ctest --test-dir build --output-on-failure`).
- [ ] Task 8.4: Rebuild macOS DMG package (`cmake --build build --target dmg -j2`) and verify with `hdiutil`.
- [ ] Task 8.5: Synchronize all documentation (`README.md`, `README-id.md`, `doc/CHANGELOG.txt`, `doc/DOCNOTE.txt`, `wiki/`).

## Checkpoint 8: Release Gate Passed

- [ ] Clean build and test execution exit with code 0.
- [ ] Verified DMG package ready for distribution.
- [ ] All documentation updated and synchronized for v1.4.0.
