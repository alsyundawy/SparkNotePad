# SparkNotePad Developer Guide

Welcome to the **SparkNotePad Developer Guide**. This document serves as the authoritative architectural blueprint, development runbook, and contribution manual for SparkNotePad maintainers and developers.

---

## Table of Contents

1. [Core Architectural Invariants](#1-core-architectural-invariants)
2. [Project Layout & Directory Structure](#2-project-layout--directory-structure)
3. [Prerequisites & Development Setup](#3-prerequisites--development-setup)
4. [Building SparkNotePad](#4-building-sparknotepad)
5. [Step-by-Step: Adding a New Feature](#5-step-by-step-adding-a-new-feature)
6. [Scintilla & Lexilla Integration Model](#6-scintilla--lexilla-integration-model)
7. [Coding Standards & Conventions](#7-coding-standards--conventions)
8. [Static Verification & Testing Protocols](#8-static-verification--testing-protocols)
9. [macOS Hardened Runtime, Codesigning & Notarization](#9-macos-hardened-runtime-codesigning--notarization)
10. [Linux Packaging (.deb, .rpm, CPack)](#10-linux-packaging-deb-rpm-cpack)
11. [CI/CD Pipeline & Release Workflows](#11-cicd-pipeline--release-workflows)
12. [Git Workflow & Commit Conventions](#12-git-workflow--commit-conventions)

---

## 1. Core Architectural Invariants

Every modification to SparkNotePad must strictly respect these architectural invariants:

1. **Vendored Engine Invariant:**
   - Always compile against `thirdparty/scintilla` and `thirdparty/lexilla`.
   - Never replace the editor core with web engines, generic text widgets, or external dynamic libraries.
2. **Dedicated Class Architecture:**
   - Every substantive capability must be encapsulated in its own distinct class under `src/features/` or `src/utils/`.
   - Avoid dumping sprawling feature logic into `MainWindow.cpp`.
3. **Discoverability (Menu & Shortcut Guarantee):**
   - Every user-facing feature must be exposed via an intuitive top menu entry and an explicit, conflict-free keyboard shortcut.
4. **Permanent Application Branding:**
   - The application name is strictly **SparkNotePad**.
   - Bundle identifier: `io.github.alsyundawy.SparkNotePad`.
   - Organization name: `SparkNotePad`.

---

## 2. Project Layout & Directory Structure

```text
SparkNotePad/
├── CMakeLists.txt                # Root CMake project configuration
├── TASK_LIST.md                  # Master roadmap and tracking task list
├── WORKGROUND.md                 # Architectural guidelines & execution rules
├── README.md                     # Project overview and entry point
├── cmake/                        # Build & packaging helper modules
│   └── PackagingLinux.cmake      # CPack rules for .deb and .rpm
├── deploy/                       # Platform deployment artifacts
│   ├── macos/                    # Info.plist, entitlements, app icon (.icns)
│   ├── linux/                    # Desktop file, metainfo.xml, icons
│   └── windows/                  # Resource scripts, Windows icon
├── doc/                          # Technical specifications and ADRs
│   ├── DECISIONS.md              # Architectural Decision Records (ADR-001 - ADR-013)
│   ├── USER_GUIDE.md             # End-user reference manual
│   ├── DEVELOPER_GUIDE.md        # Contributor and maintainer guide
│   └── ...                       # Feature-specific technical briefs
├── scripts/                      # Maintainer automation scripts
│   ├── SparkNotePad-Default-Editor.sh # Default editor association utility
│   ├── codesign-and-notarize-macos.sh # macOS codesign + notarytool pipeline
│   └── package-deb-rpm.sh        # CPack multi-distribution packager
├── src/                          # Primary C++ / Qt6 source tree
│   ├── main.cpp                  # Entry point (High-DPI policy, single application)
│   ├── EditorManager.cpp/.h      # Document lifecycle & Scintilla container
│   ├── ScintillaNext.cpp/.h      # Qt wrapper around Scintilla engine
│   ├── features/                 # Modular feature engines
│   │   ├── ThemeManager.*        # Dynamic theme & palette coordinator
│   │   ├── ScreenDisplayManager.*# Multi-monitor & Retina adaptation
│   │   ├── AccessibilityManager.*# VoiceOver announcements & focus routing
│   │   ├── MacServicesHandler.*  # macOS NSServices & Cocoa bridge
│   │   ├── TailMonitoringManager.*# Live log tailing engine
│   │   ├── TransparentWindowManager.* # Opacity controller
│   │   └── PerformanceProfiler.* # CPU/memory benchmark engine
│   ├── dialogs/                  # Modal and non-modal Qt dialogs
│   │   ├── MainWindow.cpp/.h/.ui # Primary application window
│   │   ├── DisplayInfoDialog.*   # Multi-monitor diagnostics dialog
│   │   ├── AccessibilityHelpDialog.* # Keyboard shortcuts reference
│   │   ├── EncryptDialog.* / DecryptDialog.* # AES-256 dialogs
│   │   └── ...
│   ├── docks/                    # Advanced Docking System widgets (ADS)
│   │   ├── FileBrowserDock.*     # Local directory tree browser
│   │   ├── LanguageInspectorDock.* # Real-time Scintilla token analyzer
│   │   └── ...
│   └── utils/                    # Reusable pure utility algorithms
│       ├── CryptoManager.*       # AES-256-CBC, PBKDF2, PKCS7
│       ├── JsEvaluator.*         # Sandboxed JavaScriptCore executor
│       ├── Base64Utils.*         # RFC-4648 Base64 codec
│       └── ...
└── thirdparty/                   # Vendored core dependencies
    ├── scintilla/                # Scintilla code editing engine
    ├── lexilla/                  # Lexilla language lexers
    ├── ads/                      # Qt Advanced Docking System
    └── ...
```

---

## 3. Prerequisites & Development Setup

### 3.1 macOS

- macOS 12+ (Apple Silicon or Intel).
- Xcode Command Line Tools: `xcode-select --install`.
- Qt 6 (via Homebrew): `brew install qt@6 cmake`.
- Set environment path:

  ```bash
  export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"
  export CMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@6"
  ```

### 3.2 Linux (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install build-essential cmake \
    qt6-base-dev qt6-base-private-dev qt6-tools-dev \
    libuchardet-dev libgl1-mesa-dev
```

### 3.3 Linux (Fedora / RHEL)

```bash
sudo dnf install gcc-c++ cmake \
    qt6-qtbase-devel qt6-qttools-devel \
    uchardet-devel mesa-libGL-devel
```

---

## 4. Building SparkNotePad

From the repository root:

```bash
# 1. Configure the build directory
cmake -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$(brew --prefix qt@6 2>/dev/null || echo /usr/lib/qt6)"

# 2. Compile SparkNotePad
cmake --build build

# 3. Launch SparkNotePad locally
# macOS:
open build/src/SparkNotePad.app
# Linux:
./build/src/SparkNotePad
```

---

## 5. Step-by-Step: Adding a New Feature

Follow this checklist whenever you implement a new feature:

1. **Design Class Interface:**
   Create a dedicated header and implementation file under `src/features/` (or `src/utils/` for stateless utilities). Use RAII and modern C++17/20 idioms.
2. **Register in Build System:**
   Add your `.h` and `.cpp` files to `src/CMakeLists.txt` under the appropriate source grouping.
3. **Implement UI Dialog (if required):**
   Place dialogs in `src/dialogs/`. Ensure all controls have explicit object names and accessibility descriptions.
4. **Wire Menus & Shortcuts:**
   In `src/dialogs/MainWindow.cpp`, instantiate the action, assign a distinct shortcut using `QKeySequence`, and connect it to a dedicated slot.
5. **Static Verification:**
   Run the static balance verification script (`python3 scratch/verify_static.py`) to confirm zero unbalanced braces or parentheses.
6. **Documentation & ADR:**
   Create `doc/<FEATURE_NAME>.md` and append a formal Architectural Decision Record (ADR) to `doc/DECISIONS.md`.
7. **Task Tracking:**
   Update `TASK_LIST.md` to mark the task complete.

---

## 6. Scintilla & Lexilla Integration Model

SparkNotePad interfaces with Scintilla through `src/ScintillaNext.cpp` which subclasses `QsciScintillaBase` / `ScintillaEditBase`:

- **Sending Messages:** Use the Scintilla messaging interface (`send(SCI_..., wParam, lParam)`).
- **Styling & Lexers:** Lexers are loaded from `thirdparty/lexilla`. The `LanguageInspectorDock` provides real-time introspection into token indices and styling bytes.
- **DPI Scaling:** Scintilla rasterizes text using `devicePixelRatioF()`. When moving windows across monitors with different pixel ratios, `ScreenDisplayManager` notifies Scintilla editors to trigger an update cycle.

---

## 7. Coding Standards & Conventions

- **Formatting:** 4 spaces for indentation (no tabs in C++ files).
- **Naming Conventions:**
  - Classes: `PascalCase` (e.g., `ScreenDisplayManager`, `DisplayInfoDialog`).
  - Methods & Functions: `camelCase` (e.g., `ensureWindowOnScreen`, `isDarkActive`).
  - Member Variables: `m_camelCase` (e.g., `m_mainWindow`, `m_lastDpr`).
- **Memory Management:** Prefer stack allocation, `std::unique_ptr`, `std::shared_ptr`, or Qt parent-child ownership. Avoid raw `new` without parentage.
- **Safety & Error Handling:** Use defensive input validation. Never throw raw exceptions across Qt event loops.

---

## 8. Static Verification & Testing Protocols

We maintain high codebase reliability through automated verification scripts:

- **Brace & Parenthesis Balance Scanner:**
  Ensures every source file maintains balanced scope brackets, string literal delimiters, and comments.

  ```bash
  python3 scratch/verify_static.py
  ```

- **High-DPI & Multi-Monitor Check:**

  ```bash
  python3 scratch/verify_multimonitor.py
  ```

- **Cryptographic Sanity Check:**
  Verifies AES-256 roundtrip encryption, PKCS7 padding validity, and PBKDF2 key consistency.

---

## 9. macOS Hardened Runtime, Codesigning & Notarization

Production releases on macOS require deep signing and Apple notarization.
The automated script `scripts/codesign-and-notarize-macos.sh` performs:

1. Entitlements injection (`deploy/macos/SparkNotePad.entitlements`):
   - `com.apple.security.cs.allow-jit`
   - `com.apple.security.cs.allow-unsigned-executable-memory`
   - `com.apple.security.cs.disable-library-validation`
   - `com.apple.security.automation.apple-events`
2. Deep nested signing of embedded frameworks, plugins, and helper binaries.
3. Notarization submission via `xcrun notarytool submit --wait`.
4. Ticket stapling via `xcrun stapler staple SparkNotePad.app`.
5. Gatekeeper verification via `spctl -a -vvv -t install SparkNotePad.app`.

---

## 10. Linux Packaging (.deb, .rpm, CPack)

SparkNotePad integrates with CPack for Linux distributions:

```bash
# Build both Debian and RPM packages:
./scripts/package-deb-rpm.sh

# Generated packages:
# build/SparkNotePad-<version>-Linux.deb
# build/SparkNotePad-<version>-Linux.rpm
```

The desktop file (`deploy/linux/SparkNotePad.desktop`) registers MIME associations and Desktop Actions (New Window, New File).

---

## 11. CI/CD Pipeline & Release Workflows

GitHub Actions workflows are defined in `.github/workflows/`:

- **`build.yml`:** Multi-platform CI matrix building SparkNotePad on macOS (Apple Silicon + Intel) and Linux (Ubuntu + Fedora) on every pull request and push to `main`.
- **`release.yml`:** Automated tag-triggered release workflow that builds, signs, packages, and attaches `.dmg`, `.deb`, `.rpm`, and checksum assets to GitHub Releases.

---

## 12. Git Workflow & Commit Conventions

- Use conventional commit messages:
  - `feat: <description>` (new features)
  - `fix: <description>` (bug fixes)
  - `refactor: <description>` (code cleanup without functional changes)
  - `docs: <description>` (documentation updates)
  - `ci: <description>` (CI/CD and build scripts)
- Keep pull requests focused, verifiable, and backed by ADRs.
