# SparkNotePad v1.3.7: Clangd Check, Multi-Linter Audit, i18n/Doc/Wiki Sync & Clean Release Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Execute full clangd, cppcheck, and clang-tidy static checks across all `src/*.cpp` and `src/*/*.cpp`, synchronize i18n translation catalogs and `.qm` binaries, update doc and wiki files with recent Tools suite features, prune older versions from README and DOCNOTE to feature strictly latest v1.3.7, verify zero linter defects and successful compilation, commit, push to branch `main`, and re-release tag `v1.3.7`.

**Architecture:** Maintain strict adherence to ISO C++20 and Qt 6.12 framework standards. Ensure compilation database synchronization (`build/compile_commands.json` & root symlink). Run non-intrusive static code quality verification under resource-bounded CPU/RAM constraints.

**Tech Stack:** C++20, Qt 6.12, Scintilla 5.6.7, Lexilla 5.5.4, CMake 3.21+, Ninja, LLVM/Clangd 20+, Cppcheck 2.22, Trunk CLI 1.25.0, Qt Linguist (`lupdate`, `lrelease`), Git.

**Spec:** User instruction to focus on branch `main`, run `clangd --check`, `cppcheck`, `clang-tidy`, resolve any true errors immediately, update `i18n`, `doc/`, `wiki/`, prune older versions from README/DOCNOTE (keeping only latest v1.3.7), verify build and linters, push, and re-release.

---

## Global Constraints

- **Git Branch Target:** Must strictly execute on branch `main`.
- **Identity & Repo Path:** Absolute path is `/Users/alsyundawy/Downloads/GitHub/SparkNotePad`. Do not confuse with any other repo.
- **Resource Discipline:** Limit concurrent build and linter jobs to `-j 4` or lower to prevent excessive RAM/CPU spikes on the macOS host.
- **Documentation Hygiene:** README and DOCNOTE must feature ONLY the latest version (v1.3.7) without obsolete release history tables. Full history remains in `CHANGELOG.md`.

---

## Review Focus

1. **Clangd AST Diagnostic Verification:** Distinguish between actual C++ compiler/syntax diagnostics (`[clang-diagnostic-error]`) and internal code-action refactoring tweaks (`tweak: SwapBinaryOperands ==> FAIL` on macro expansions). Ensure zero AST diagnostic errors across all translation units.
2. **Translation Completeness:** Ensure `update_translations` extracts all UI strings from `MainWindow.cpp` (including the new Tools menu) and compiles clean `.qm` binaries.
3. **Wiki & Doc Synchronization:** Align `doc/RELEASE_NOTES_v1.3.7.md`, `doc/FEATURE_INVENTORY.md`, and `wiki/` with all new Tools features and ergonomics.
4. **Markdown Formatting:** Autoformat modified docs via `trunk fmt` to avoid CI formatting errors.

---

## Tasks & Implementation Plan Checklist

### Task 1: Verify Clangd, Cppcheck, and Multi-Linter Health across `src/`

**Files:**

- Audit: `src/*.cpp` and `src/*/*.cpp`
- Verify: `src/main.cpp`, `src/ScintillaNext.cpp`, `src/dialogs/MainWindow.cpp`, `src/features/ThemeManager.cpp`, `src/SparkNotePadApplication.cpp`

- [x] **Step 1: Run clangd --check on core translation units**
      Verify that AST parsing completes with 0 compiler errors across source files.
- [x] **Step 2: Run Cppcheck on src/ with Qt library profile**
      Run: `cppcheck --language=c++ --library=qt --enable=warning,style,performance,portability --suppress=unusedStructMember --inline-suppr --quiet src/`
      Expected: 0 errors, exit code 0.
- [x] **Step 3: Run Trunk check across modified workspace files**
      Run: `trunk check --no-fix`
      Expected: `✔ No issues`, exit code 0.

---

### Task 2: Synchronize i18n Translation Catalogs & Binaries

**Files:**

- Modify: `i18n/SparkNotePad_en.ts`
- Modify: `i18n/SparkNotePad_id.ts`
- Generate: `i18n/SparkNotePad_en.qm`
- Generate: `i18n/SparkNotePad_id.qm`

- [x] **Step 1: Extract latest UI strings via lupdate**
      Run: `cmake --build build --target update_translations`
      Expected: Extracts 55 new strings from `MainWindow.cpp` into `.ts` catalogs.
- [x] **Step 2: Compile binary message files via lrelease**
      Run: `lrelease i18n/*.ts`
      Expected: Generates updated `.qm` files for both English and Indonesian.

---

### Task 3: Update Documentation (`doc/` & `wiki/`)

**Files:**

- Modify: `doc/RELEASE_NOTES_v1.3.7.md`
- Modify: `doc/FEATURE_INVENTORY.md`
- Modify: `wiki/02-Feature-Overview.md`
- Modify: `wiki/02-Ikhtisar-Fitur.md`

- [ ] **Step 1: Update doc/RELEASE_NOTES_v1.3.7.md and FEATURE_INVENTORY.md**
      Add full technical breakdown of the Notepad++ Tools menu (Hash, MIME Tools, Converter, AES-256 Encryption in Tools), Quick Open repositioning, and compact menubar font.
- [ ] **Step 2: Update wiki/02-Feature-Overview.md & 02-Ikhtisar-Fitur.md**
      Add dedicated section on the Tools menu suite in both English and Indonesian.

---

### Task 4: Prune Older Versions from README & Rebuild DOCNOTE (v1.3.7 Only)

**Files:**

- Modify: `README.md`
- Modify: `README-id.md`
- Modify: `DOCNOTE.md`
- Verify: `CHANGELOG.md`

- [ ] **Step 1: Prune older releases from README.md & README-id.md**
      Remove older version subheadings (`v1.3.6`, `v1.3.5`, `v1.3.0`) from the Changelog section. Feature exclusively `v1.3.7` with its comprehensive capabilities.
- [ ] **Step 2: Rebuild DOCNOTE.md to feature exclusively v1.3.7**
      Remove obsolete release note sections (v1.3.6 down to v1.0.0), keeping DOCNOTE.md focused exclusively as the authoritative v1.3.7 Release DocNote.
- [ ] **Step 3: Run trunk fmt on all modified documentation files**
      Format Markdown files via `trunk fmt` to ensure clean prettier and markdownlint compliance.

---

### Task 5: Build, Test & Release Execution on Branch `main`

**Files:**

- Target Branch: `main`
- Tag: `v1.3.7`

- [ ] **Step 1: Re-verify full test suite**
      Run: `bash scripts/run-unit-tests.sh && bash scripts/run-security-audit.sh`
      Expected: All 44 native tests PASS, security audit PASS.
- [ ] **Step 2: Commit all updates to branch `main`**
      Run: `git commit -m "docs(release): prune older versions, sync i18n, doc, and wiki for v1.3.7"`
- [ ] **Step 3: Push branch `main` and force-update tag `v1.3.7`**
      Run: `git push origin main && git tag -fa v1.3.7 -m "Release v1.3.7" && git push origin v1.3.7 --force`
      Expected: Triggers GitHub Actions release workflow.
