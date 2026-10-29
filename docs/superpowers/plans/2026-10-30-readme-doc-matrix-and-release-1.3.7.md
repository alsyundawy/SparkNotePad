# Implementation Plan: Documentation Build Matrix Expansion, 13-Pillar Code Audit & v1.3.7 Release

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Expand the build guide links in `README.md` and `README-id.md` to comprehensively cover macOS and Linux build guides alongside FreeBSD, perform a 13-pillar code review audit across all translation units, ensure zero warnings under `trunk check`, verify clean build and tests, update `CHANGELOG.md` and `DOCNOTE.md`, and push & re-release tag `v1.3.7` on branch `main`.

**Architecture:** Pure documentation and code hygiene synchronization. Consistent bi-directional Markdown cross-referencing for macOS (`MACOS_CODESIGN_AND_NOTARIZATION.md`, `DEVELOPER_GUIDE.md`), Linux (`BUILD_UBUNTU_DEBIAN.md`, `LINUX_DEB_RPM_PACKAGING.md`), and FreeBSD (`BUILD_FREEBSD_KDE_XFCE.md`, `FREEBSD_BUILD_ANALYSIS_AND_TASKLIST.md`).

**Tech Stack:** C++20, Qt 6.12, CMake, Ninja, Trunk, Git.

**Spec:** Direct user prompt requirements.

## Global Constraints

- Repository: `SparkNotePad` on branch `main`.
- Keep compilation concurrency bounded (`-j 4`) to prevent MacBook resource exhaustion.
- Zero warnings/errors across all configured linters (`trunk check --no-fix`).
- All 51 unit tests must pass.
- Release tag `v1.3.7` force-updated and pushed to `origin`.

## Review Focus

1. Consistent Markdown link syntax: Use standard relative file paths (`doc/FILE.md`).
2. Dual language parity: Keep `README.md` (English) and `README-id.md` (Indonesian) synchronized.
3. Accurate platform guides: Ensure referenced files actually exist in `doc/`.
4. Clean code verification: Ensure 0 SonarLint / clangd / cppcheck diagnostics remain.
5. Atomic Git commits: Stage all verified files cleanly without stray files.

---

### Task 1: Update Build Guides in README.md and README-id.md

**Files:**

- Modify: `README.md:75-90, 230-285`
- Modify: `README-id.md:75-90, 230-285`

- [ ] **Step 1.1: Add Linux and macOS build guides to README.md maintainer header**
      Add links for `BUILD_UBUNTU_DEBIAN.md`, `LINUX_DEB_RPM_PACKAGING.md`, `MACOS_CODESIGN_AND_NOTARIZATION.md`, and `DEVELOPER_GUIDE.md` alongside `BUILD_FREEBSD_KDE_XFCE.md`.
- [ ] **Step 1.2: Add macOS Native Build & Packaging subsection to README.md**
      Include `cmake --build build --target dmg` command snippet and link to macOS codesigning guide.
- [ ] **Step 1.3: Synchronize README-id.md with translated build guide links and sections**
- [ ] **Step 1.4: Format Markdown with `trunk fmt`**

---

### Task 2: 13-Pillar Code Audit & Static Analysis Verification

**Files:**

- Review: `src/` (all translation units)

- [ ] **Step 2.1: Verify 13 pillars (Bug, Syntax, Runtime, Logic, Memory, Dead Code, Duplicate Code, Circular Dependency, Performance, Security, Maintainability, Scalability, Readability, IDE Linter)**
- [ ] **Step 2.2: Run `trunk check --no-fix` across all modified files**
      Verify 0 linter issues.

---

### Task 3: Build Verification & Unit Test Suite

**Files:**

- Test: `./build/tests/SparkNotePadTests`

- [ ] **Step 3.1: Build project with `cmake --build build -j 4`**
- [ ] **Step 3.2: Run `./build/tests/SparkNotePadTests`**
      Confirm all 51 test cases pass.

---

### Task 4: Update Documentation, CHANGELOG, Push & Re-Release

**Files:**

- Modify: `CHANGELOG.md`
- Modify: `DOCNOTE.md`

- [ ] **Step 4.1: Update `CHANGELOG.md` and `DOCNOTE.md`**
- [ ] **Step 4.2: Stage and commit all changes to branch `main`**
- [ ] **Step 4.3: Push `main` to `origin`**
- [ ] **Step 4.4: Force-update and push tag `v1.3.7` to `origin`**
