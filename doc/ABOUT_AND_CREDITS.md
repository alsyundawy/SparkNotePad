# SparkNotePad Credits, Attribution, Tools & Non-Affiliation Policy

## 1. Important Legal Notice

> **SparkNotePad is an independent open-source project. It is not affiliated with, endorsed by, or associated with Notepad++, NotepadNext, Notepad4, or Notepad3.**

---

## 2. Upstream Credits & Attribution (v1.0.0 – v1.3.5)

SparkNotePad stands on the shoulders of giants. We express our deepest gratitude to the following projects, maintainers, communities, and tools:

### A. AI Engineering, Code Review & Security Auditing

| Project / Tool       | Organization    | Role & Contribution                                                                                                   |
| :------------------- | :-------------- | :-------------------------------------------------------------------------------------------------------------------- |
| **Antigravity IDE**  | Google DeepMind | Primary AI Agentic Pair Programming IDE driving architecture, refactoring, terminal workflows, and quality assurance. |
| **Gemini 3.8 Flash** | Google DeepMind | Continuous AI Code Review, 13-Pillar Security Auditing, Static Analysis, and Linter Compliance Engine.                |

### B. Upstream Projects & Ancestral Editors

| Project                  | Author / Maintainer         | Role & Contribution                                                                            |
| :----------------------- | :-------------------------- | :--------------------------------------------------------------------------------------------- |
| **NotepadNext**          | Justin Dailey (`@dail8859`) | Cross-platform Qt application framework, docking architecture, and Scintilla integration base. |
| **Notepad4**             | Zufu Liu (`@zufuliu`)       | Architectural enhancements, Scintilla integration ideas, syntax lexers, and utility patterns.  |
| **Notepad3**             | Rizonesoft & Florian Balmer | Text transformation algorithms, Scintilla power-user patterns, and UI polish.                  |
| **Notepad--**            | cxasm (`@cxasm`)            | Cross-platform text comparison, encoding detection, and editor concepts.                       |
| **Notepad++**            | Don Ho (`@donho`)           | The legendary Windows text editor that inspired the modern notepad generation.                 |
| **SubnetCalc for macOS** | Harry D.S. Alsyundawy       | UI design inspiration, modern theme architecture, and release documentation standards.         |

### C. Core Editing Engines & Lexers

| Component             | Author / Maintainer                                                | Role & Contribution                                                                            |
| :-------------------- | :----------------------------------------------------------------- | :--------------------------------------------------------------------------------------------- |
| **Scintilla 5.6.7**   | Neil Hodgson & Contributors                                        | Core text editing canvas, high-performance rendering, fold margins, and multi-caret selection. |
| **Lexilla 5.5.4**     | Neil Hodgson & Contributors                                        | Syntax styling and highlighting lexers for over 100 programming languages.                     |
| **Lua 5.5.1 / 5.4.x** | Roberto Ierusalimschy, Waldemar Celes, Luiz Henrique de Figueiredo | Embedded scripting engine and automation runtime.                                              |

### D. Third-Party Libraries & Frameworks

| Library                                    | Author / Organization      | Role & Contribution                                                                        |
| :----------------------------------------- | :------------------------- | :----------------------------------------------------------------------------------------- |
| **The Qt Project (Qt 6.5–6.12)**           | The Qt Company & Community | Modern cross-platform C++ GUI framework (QtCore, QtGui, QtWidgets, QtPrintSupport, QtSvg). |
| **Qt Advanced Docking System (ADS 5.1.1)** | Manuel Freiholz            | Professional dock widget management, movable tabs, and floating tool windows.              |
| **QSimpleUpdater**                         | Alex Spataru               | Seamless multi-platform software update checking.                                          |
| **SingleApplication (3.5.6)**              | Itay Grudev                | High-performance IPC and single-instance process management.                               |
| **EditorConfig Core Qt**                   | Hong Xu                    | Universal code style and `.editorconfig` specification enforcement.                        |
| **uchardet**                               | Freedesktop / Mozilla      | Fast and universal character encoding auto-detection engine.                               |
| **OpenSSL Project**                        | OpenSSL Software Services  | Industry-standard cryptography, TLS/SSL, and secure hash algorithms.                       |

### E. Visual Artwork & Typography

| Asset / Designer                        | Author                | Role & Contribution                                                                          |
| :-------------------------------------- | :-------------------- | :------------------------------------------------------------------------------------------- |
| **Lon McGregor (NotepadPlusPlusIcons)** | Lon McGregor          | Open-source chameleon and editor artwork series, adapted for SparkNotePad official branding. |
| **SparkNotePad Monoline Vector Suite**  | Harry D.S. Alsyundawy | Retina UI vector icons customized for 75 themes, menus, and toolbars.                        |

---

## 3. Dedicated AboutDialog Architecture

SparkNotePad features a native, tabbed `AboutDialog` (`src/dialogs/AboutDialog.h` & `.cpp`) with 100% unified, seamless theme backgrounds and high-DPI Retina optimization:

1. **About Tab (`&About`):** Versioning, maintainer info, official links, and explicit non-affiliation declaration on a unified card palette.
2. **Maintainer Tab (`&Maintainer`):** Maintainer contact channels, GitHub profile, official website, and sponsorship/donation methods.
3. **Credits Tab (`&Credits`):** Comprehensive acknowledgments for AI tools (Antigravity IDE & Gemini 3.8 Flash), upstream projects, engines, libraries, and artwork.
4. **Diagnostics Tab (`&Diagnostics`):** Real-time host OS, kernel version, CPU architecture, Qt version, DPR display scale, engine versions, and one-click clipboard copy.
5. **Shortcuts Tab (`&Shortcuts`):** Comprehensive table of key productivity keyboard shortcuts.
6. **License Tab (`&License`):** GNU General Public License v3.0 (GPLv3) terms and conditions.
