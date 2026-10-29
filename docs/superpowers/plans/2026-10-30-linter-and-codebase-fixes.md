# Implementation Plan: Comprehensive Linter, SonarQube, Clang-Tidy Fixes & 13-Pillar Audit

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Fix all SonarQube, clangd, cppcheck, and IDE linter issues across `src/dialogs/PreferencesDialog.cpp`, `src/dialogs/ShortcutCustomizerDialog.cpp`, `src/features/ReleaseManager.cpp`, `src/utils/ExternalToolManager.cpp`, `src/utils/MimeTools.cpp/.h`, and `src/dialogs/MainWindow.cpp`, execute comprehensive 13-pillar code review, verify zero warnings under `trunk check`, ensure clean compilation, all unit tests pass, and update documentation.

**Architecture:** Pure ISO C++20 standard compliance with Qt 6.12. Memory management adhering to RAII (`std::unique_ptr`, `std::make_unique`, Qt object tree). Modern STL algorithms (`std::ranges::find_if`, `std::array`). Templated utility abstractions (`MimeTools::processByLine<Func>`). Elimination of cognitive complexity and nesting anti-patterns through functional decomposition.

**Tech Stack:** C++20, Qt 6.12 (Core, Widgets, Network), Scintilla 5.6.7, Clangd / Clang-Tidy, Cppcheck, SonarLint / SonarQube, Trunk, CMake, Ninja.

**Spec:** Direct user requirements and `@current_problems` issue block.

## Global Constraints

- Repository: `SparkNotePad` on branch `main`.
- Pure ISO C++20 standard compliance.
- No regression or removal of existing well-functioning features.
- Memory safety: zero manual `new`/`delete` when RAII or Qt parent-child hierarchy applies.
- Concurrency limit: `-j 4` for builds to preserve system resources on macOS.
- 100% clean linter checks (`trunk check --no-fix`) and 0 compiler warnings/errors.

## Review Focus

1. Pointer-to-const parameters and local variables: Ensure getters and non-modifying traversals do not permit accidental state mutation.
2. Nesting statement depth: Never exceed 3 levels of nested control flow (`if`/`for`/`while`/`switch`).
3. Lambda captures: Explicitly capture only required scope variables, avoiding wildcard captures (`[=]`, `[&]`) and unused `this`.
4. Integer precision: Use native `qsizetype` for Qt container indices and explicit casts for foreign integer types (e.g. Scintilla `sptr_t`).
5. Memory ownership: Use smart pointers for unparented heap allocations (`std::unique_ptr<Ui::PreferencesDialog>`).

---

### Task 1: Fix MimeTools Template & Integer Precision

**Files:**

- Modify: `src/utils/MimeTools.h:40-50`
- Modify: `src/utils/MimeTools.cpp:15-45, 115-145`
- Test: `tests/test_mime_tools.h`

- [ ] **Step 1.1: Convert `processByLine` to a template helper in `src/utils/MimeTools.h`**
      Move `processByLine` implementation to header as a template function taking `Func &&func`. Remove the `std::function` overload in `src/utils/MimeTools.cpp`.
- [ ] **Step 1.2: Fix `qsizetype` integer precision in `quotedPrintableDecode`**
      Change `int i = 0; const int size = src.size();` to `qsizetype i = 0; const qsizetype size = src.size();`.
- [ ] **Step 1.3: Run unit tests**
      Execute `./build/tests/SparkNotePadTests` to confirm all MIME tool tests pass.

---

### Task 2: Fix ExternalToolManager Shebang & Init-Statements

**Files:**

- Modify: `src/utils/ExternalToolManager.cpp:275-320`

- [ ] **Step 2.1: Use init-statement for `s_extMap` lookup and `lineLen`**
      Use `if (const auto it = s_extMap.constFind(ext); it != s_extMap.constEnd())` and `if (const auto lineLen = static_cast<int>(editor->lineLength(0)); lineLen <= 2)`.
- [ ] **Step 2.2: Replace C-style array with `std::array` in `shebangInterpreters`**
      Use `static constexpr std::array<std::pair<const char *, const char *>, 8> shebangInterpreters = ...`.

---

### Task 3: Fix ReleaseManager Init-Statements

**Files:**

- Modify: `src/features/ReleaseManager.cpp:95-102, 134-140`

- [ ] **Step 3.1: Declare `cachedEtag` inside if-init statement**
      Use `if (const QString cachedEtag = settings.value(QStringLiteral("ReleaseManager/LatestETag")).toString(); !cachedEtag.isEmpty())`.
- [ ] **Step 3.2: Declare `etag` inside if-init statement**
      Use `if (const QByteArray etag = reply->rawHeader("ETag"); !etag.isEmpty())`.

---

### Task 4: Fix PreferencesDialog Memory Management, Lambda Captures & Nested Conditionals

**Files:**

- Modify: `src/dialogs/PreferencesDialog.h`
- Modify: `src/dialogs/PreferencesDialog.cpp`

- [ ] **Step 4.1: Modernize `Ui::PreferencesDialog` memory management**
      Change `Ui::PreferencesDialog *ui` in header to `std::unique_ptr<Ui::PreferencesDialog> ui;` and initialize with `std::make_unique<Ui::PreferencesDialog>()` in constructor; default the destructor.
- [ ] **Step 4.2: Explicitly capture lambda scope variables and remove unused `this`**
      Replace wildcard captures `[=, this]` with explicit captures `[this]` or `[settings = this->settings]` across lines 72, 95, 107, 109, 127, 128, 163, 169, 174, 183.
- [ ] **Step 4.3: Eliminate variable shadowing and redundant types**
      Rename shadowed `index` parameters to `idx`. Replace explicit types with `auto` on lines 158, 164, 250, 251, 290, 309.
- [ ] **Step 4.4: Replace `std::bind` with `std::invoke`**
      In `MapSettingToCheckBox` and `MapSettingToGroupBox`, replace `std::bind(getter, settings)()` with `std::invoke(getter, settings)`.
- [ ] **Step 4.5: Complete TODO in `populateTranslationComboBox`**
      Sort available translations alphabetically while keeping system default at the top.
- [ ] **Step 4.6: Extract nested ternary conditionals into independent statements**
      In `updateThemeColors()`, extract default colors based on `dark` before resolving palette overrides.

---

### Task 5: Refactor ShortcutCustomizerDialog Complexity & Const Correctness

**Files:**

- Modify: `src/dialogs/ShortcutCustomizerDialog.h`
- Modify: `src/dialogs/ShortcutCustomizerDialog.cpp`

- [ ] **Step 5.1: Decompose `collectActions` into `registerAction` and `traverseMenu`**
      Extract helper methods to reduce Cognitive Complexity below 15, eliminate nested statements > 3, and ensure `const QMenu *` and `const QAction *` pointer-to-const usage.
- [ ] **Step 5.2: Extract nested ternary operators in `applyThemeStyles`**
      Separate dark/light defaults into independent statements.
- [ ] **Step 5.3: Add `const` qualifiers to member functions and variables**
      Declare `onFilterTextChanged(const QString &text) const` and `saveShortcuts() const`. Use `const QTreeWidgetItem *item` and `const auto *app`.

---

### Task 6: Verification, Formatting, Build & Test Suite

**Files:**

- Modify: `README.md`
- Modify: `CHANGELOG.md`

- [ ] **Step 6.1: Run `trunk fmt` and `trunk check --no-fix`**
      Verify 0 formatting deviations and 0 linter issues.
- [ ] **Step 6.2: Build project with `cmake --build build -j 4`**
      Verify 0 compiler warnings and 0 compilation errors.
- [ ] **Step 6.3: Run unit test suite `./build/tests/SparkNotePadTests`**
      Verify all tests pass with 100% green status.
- [ ] **Step 6.4: Update README, documentation, and CHANGELOG for version 1.3.7**
