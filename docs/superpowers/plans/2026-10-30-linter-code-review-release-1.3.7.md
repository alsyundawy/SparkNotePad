# SparkNotePad v1.3.7: 13-Pillar Code Review, Linter Zero-Defect & Release Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Eradicate all IDE diagnostics and SonarQube linter errors, conduct a rigorous 13-pillar production code review across the codebase, ensure all linters (trunk, cppcheck, clang-format, megalinter rules) pass with zero defects, verify build and tests, update documentation (README, DOCNOTE, CHANGELOG for v1.3.7), and push/release cleanly on branch `main`.

**Architecture:** Maintain strict adherence to Qt 6.12 / C++20 standards and clean architecture. Ensure compilation database synchronization (`build/compile_commands.json` & root symlink) so clangd and SonarLint resolve all system and project headers without falling back. Execute read-only static scanning and targeted atomic fixes under conservative CPU/RAM limits to protect the developer workstation.

**Tech Stack:** C++20, Qt 6 (Core, Gui, Widgets, Network, PrintSupport, Core5Compat), Scintilla 5.6.7, Lexilla 5.5.4, CMake 3.21+, LLVM/Clang 20+, cppcheck, Trunk CLI 1.25.0, SonarLint / SonarQube C++ rules, MegaLinter.

**Spec:** User specification to focus strictly on branch `main`, resolve all 1288 IDE diagnostics and 756 SonarQube errors, execute 13-pillar code review, pass all linters, optimize without regression or unrequested restructuring, update README/DOCNOTE/CHANGELOG, and push release v1.3.7.

---

## Global Constraints

- **Git Branch Target:** Must strictly execute on branch `main`.
- **Zero Hallucination & Zero Regressions:** Do not alter working application logic, break Scintilla integration, or drop existing functional components.
- **Resource Discipline:** Limit concurrent linter jobs to `-j 2` or logical core caps to prevent RAM/CPU spikes on the host macOS workstation.
- **Header & ABI Stability:** Preserve public signatures and Qt slot/signal connections in `MainWindow`, `ScintillaNext`, and `SparkNotePadApplication`.
- **Compilation Database:** Keep `build/compile_commands.json` (312 entries) in sync with root `compile_commands.json` for clangd and SonarLint.

---

## Review Focus

1. **Missing Compilation Database Path:** `clangd` and `sonarlint` point to `build/compile_commands.json`. If missing, 1288+ syntax/header errors appear in the IDE Problems panel.
2. **Qt 6 QByteArray vs QString Mismatch:** `ScintillaNext::getSelText()` returns `QByteArray`. Calling `.toUtf8()` on it or assigning without `QString::fromUtf8` causes compile/linter errors.
3. **Clang-format Compliance:** Any newly added code must be formatted via `.clang-format` through `trunk fmt` to prevent linter CI failures.
4. **Memory & Parent-Child Ownership:** Dynamically allocated `QMenu` and `QAction` instances must set `this` (MainWindow) as parent to prevent memory leaks.
5. **ReDoS / Buffer Safety:** Text encoding/decoding and hash generation on large buffers must handle empty inputs and extreme lengths safely without freezing the GUI event loop.

---

## Tasks & Implementation Plan Checklist

### Task 1: IDE Diagnostics & Compilation Database Synchronization (Clangd & SonarLint)

**Files:**

- Create/Sync: `build/compile_commands.json`
- Create/Sync: `compile_commands.json`
- Verify: `.vscode/settings.json`
- Verify: `.clangd`
- Verify: `sonar-project.properties`

**Interfaces:**

- Consumes: `scripts/generate_compilation_database.py`, CMake configuration, macOS SDK & Qt 6 framework paths.
- Produces: 312-entry unified compilation database recognized by both clangd language server and SonarLint/SonarQube C-family analyzer.

- [x] **Step 1: Verify compilation database generator script**
      Run: `python3 scripts/generate_compilation_database.py`
      Expected: Outputs `Generated compilation database with 312 entries at .../build/compile_commands.json`.

- [x] **Step 2: Verify compile_commands.json root symlink**
      Run: `ls -la compile_commands.json build/compile_commands.json`
      Expected: Both files exist and are valid JSON.

- [x] **Step 3: Verify IDE linter configuration mappings**
      Check that `.vscode/settings.json` (`clangd.arguments`, `sonarqube.pathToCompileCommands`, `sonarlint.pathToCompileCommands`), `.clangd` (`CompilationDatabase: "build"`), and `sonar-project.properties` all target `build/compile_commands.json`.

- [x] **Step 4: Verify clangd diagnostic suppressions for external vendored libraries**
      Ensure thirdparty/scintilla, .cpm-cache, and generated headers are excluded from standalone IDE error reporting so only first-party `src/` diagnostics are tracked.

---

### Task 2: 13-Pillar Production Code Review across `src/`

**Files:**

- Review: `src/dialogs/MainWindow.cpp`
- Review: `src/dialogs/MainWindow.h`
- Review: `src/features/ThemeManager.cpp`
- Review: `src/SparkNotePadApplication.cpp`
- Review: `src/ScintillaNext.cpp` & `src/ScintillaNext.h`
- Review: `src/utils/CryptoManager.h` & `src/utils/CryptoManager.cpp`

**Checklist across the 13 Engineering Pillars:**

- [x] **Pillar 1: Bug Review**
  - Verify null pointer guards on `currentEditor()` in all new Tools menu actions (Hash, MIME Tools, Converter, Encryption).
  - Verify bounds checking on hex conversion and string slicing.
- [x] **Pillar 2: Syntax Review**
  - Verify full compliance with ISO C++20 (`-std=c++20`).
  - Verify absence of forbidden macro expansions or stray pragmas.
- [x] **Pillar 3: Runtime Review**
  - Verify that hash calculation for large files or selections does not block UI indefinitely.
  - Verify status bar timeout (4000ms) for transient feedback messages.
- [x] **Pillar 4: Logic Review**
  - Verify that MD5, SHA-1, SHA-256, SHA-512 actions generate correct cryptographic hashes.
  - Verify CRC32 and XXHASH properly route to `HashIntegrityDialog`.
  - Verify Base64 / URL / Quoted-Printable encoding and decoding logic preserves UTF-8 characters.
  - Verify ASCII<->HEX and Decimal<->HEX converters handle empty and malformed inputs gracefully.
- [x] **Pillar 5: Memory Review**
  - Verify Qt parent-child hierarchy on all newly allocated `QMenu` and `QAction` objects (`this` ownership).
  - Zero memory leaks, zero dangling pointers, zero double frees.
- [x] **Pillar 6: Dead Code Review**
  - Verify removal of redundant or superseded menu actions in `menuFile`.
  - Ensure all declared slots and lambdas are actively connected.
- [x] **Pillar 7: Duplicate Code Review**
  - Verify common hash generation and clipboard dispatch share unified helper lambdas.
- [x] **Pillar 8: Circular Dependency Review**
  - Ensure header includes in `MainWindow.h` remain minimal with forward declarations where applicable.
- [x] **Pillar 9: Performance Bottleneck Review**
  - Use `QStringView` / `QLatin1String` where possible to avoid redundant string allocations.
  - Compact menubar styling via CSS without incurring Qt widget repaint loops.
- [x] **Pillar 10: Security Vulnerability Review (OWASP 2025 / CWE Top 25)**
  - No command execution or unquoted shell calls (CWE-78).
  - AES-256 and PBKDF2 iterations preserved at 100,000 rounds (CWE-326/916).
  - MIME and hex conversion cannot trigger buffer overruns (CWE-119).
- [x] **Pillar 11: Maintainability Review**
  - Clean descriptive action names, proper Qt `tr()` translation wrappers on all UI strings.
- [x] **Pillar 12: Scalability Review**
  - High-DPI 2x/4K rendering support preserved for all toolbar and menu icons.
- [x] **Pillar 13: Readability Review**
  - Consistent indentation, clear structural comments, adherence to SparkNotePad codebase conventions.
- [x] **Pillar 14: IDE Linter Review**
  - Zero diagnostics in clangd, zero warnings in cppcheck, zero rule breaches in SonarLint.

---

### Task 3: Multi-Linter Verification & Code Formatting

**Files:**

- Format: `src/dialogs/MainWindow.cpp`
- Check: All modified files via `trunk check`
- Validate: `.trunk/trunk.yaml`, `.mega-linter.yml`

- [x] **Step 1: Execute Trunk formatter on modified sources**
      Run: `trunk fmt src/dialogs/MainWindow.cpp`
      Expected: `✔ Formatted src/dialogs/MainWindow.cpp`, exit code 0.

- [x] **Step 2: Run Trunk linter checks**
      Run: `trunk check --no-fix`
      Expected: `Checked 1 modified file. ✔ No issues`, exit code 0.

- [x] **Step 3: Run Cppcheck with Qt library rules**
      Run: `cppcheck --language=c++ --library=qt --enable=warning,style,performance,portability --suppress=unusedStructMember --inline-suppr --quiet src/`
      Expected: Exit code 0, 0 defects.

- [x] **Step 4: Validate MegaLinter configuration consistency**
      Verify `.mega-linter.yml` correctly filters non-TypeScript `.ts` translation files and excludes thirdparty/build directories.

---

### Task 4: Compilation & Test Suite Verification

**Files:**

- Target: `SparkNotePad` binary
- Tests: `scripts/run-unit-tests.sh`
- Security Audit: `scripts/run-security-audit.sh`
- Invariant Guard: `scripts/verify-engine-invariants.sh`

- [x] **Step 1: Run CMake build**
      Run: `cmake --build build --target SparkNotePad -- -j$(sysctl -n hw.logicalcpu)`
      Expected: `[36/37] Linking CXX executable SparkNotePad`, exit code 0, 0 compiler errors.

- [x] **Step 2: Run standalone utility algorithm tests**
      Run: `bash scripts/run-unit-tests.sh`
      Expected: Base64, Radix/Hex, UUID, JSON, Math, and PBKDF2 tests all PASS.

- [x] **Step 3: Run static security audit**
      Run: `bash scripts/run-security-audit.sh`
      Expected: Zero dangerous process calls, JS sandboxing verified, XSS escaping verified, PBKDF2 100k verified.

- [x] **Step 4: Run engine invariants guard**
      Run: `bash scripts/verify-engine-invariants.sh`
      Expected: Scintilla, Lexilla, Lua 5.5+, PythonScriptEngine verified.

---

### Task 5: Documentation & Changelog Overhaul for Release v1.3.7

**Files:**

- Modify: `README.md`
- Modify: `README-id.md`
- Modify: `DOCNOTE.md`
- Modify: `CHANGELOG.md`

- [ ] **Step 1: Update README.md and README-id.md**
      Add dedicated section documenting the new Notepad++ style **Tools** menu:
  - Hash submenu (MD5, CRC32, XXHASH, SHA-1, SHA-256, SHA-512 with Generate, From File, From Selection, Verify)
  - MIME Tools submenu (Base64 Encode/Decode, URL Encode/Decode, Quoted-Printable Encode)
  - Converter submenu (ASCII<->HEX, Decimal<->HEX)
  - Relocated AES-256 Encryption & Cryptographic Hash Validator
  - Quick Open placement in File menu
  - Compact menubar typography (3px 6px padding, 12px font)

- [ ] **Step 2: Update DOCNOTE.md**
      Document the v1.3.7 release notes, compilation database eradication of 1288 IDE errors & 756 SonarQube errors, 13-pillar code review results, and build validation.

- [ ] **Step 3: Update CHANGELOG.md**
      Record the v1.3.7 features, enhancements, and linter fixes following Keep a Changelog standards.

---

### Task 6: Git Commit, Tagging, Push & GitHub Release Workflow

**Files:**

- Stage: `src/dialogs/MainWindow.cpp`, `README.md`, `README-id.md`, `DOCNOTE.md`, `CHANGELOG.md`
- Target Branch: `main`

- [ ] **Step 1: Check git diff and staging status**
      Run: `git diff --stat`
      Expected: Only clean, reviewed, and formatted files.

- [ ] **Step 2: Commit changes to branch `main`**
      Run: `git commit -m "docs(release): update README, DOCNOTE, and CHANGELOG for v1.3.7 Tools suite and linter hardening"`

- [ ] **Step 3: Push branch `main` to origin**
      Run: `git push origin main`
      Expected: Successful fast-forward push to GitHub `origin/main`.

- [ ] **Step 4: Verify or update tag `v1.3.7` and release triggers**
      Run: `git tag -fa v1.3.7 -m "Release v1.3.7 - Tools Menu, Forensics Suite, Linter Zero-Defect Hardening" && git push origin v1.3.7 --force`
