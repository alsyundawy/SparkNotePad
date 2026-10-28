# Keyboard Shortcuts Quick Reference

SparkNotePad is engineered for keyboard-centric developer productivity. Every major workflow—from file switching and command palette invocation to multi-monitor window redistribution and cryptographic operations—can be executed without lifting your hands from the keyboard.

---

## 1. Global Navigation & Command Discovery

SparkNotePad features a dual-mode quick palette architecture inspired by modern IDEs:

| Action                            | macOS Shortcut               | Linux / Windows Shortcut      | Function / Scope                                                |
| :-------------------------------- | :--------------------------- | :---------------------------- | :-------------------------------------------------------------- |
| **Quick Open (Omnibar)**          | `Cmd+P` / `Ctrl+Alt+O`       | `Ctrl+P` / `Ctrl+Alt+O`       | Real-time fuzzy document search across open project tree        |
| **Command Palette Launcher**      | `Cmd+Shift+P` / `Ctrl+Alt+I` | `Ctrl+Shift+P` / `Ctrl+Alt+I` | Access 100+ menu actions, tools, converters, and formatters     |
| **Focus Editor Canvas**           | `Ctrl+Alt+E`                 | `Ctrl+Alt+E`                  | Instantly return keyboard focus to editor from docks or dialogs |
| **Keyboard & Accessibility Help** | `Shift+F1`                   | `Shift+F1` / `F1`             | Open interactive searchable shortcut reference dialog           |
| **Online Documentation / Wiki**   | `F1`                         | `Ctrl+F1`                     | Open local User Guide or official GitHub Wiki                   |

---

## 2. File & Document Management

| Action                  | macOS Shortcut    | Linux / Windows Shortcut | Description                                             |
| :---------------------- | :---------------- | :----------------------- | :------------------------------------------------------ |
| **New Document**        | `Cmd+N`           | `Ctrl+N`                 | Create new untitled document in a new tab               |
| **Open Document**       | `Cmd+O`           | `Ctrl+O`                 | Open file chooser dialog                                |
| **Save Document**       | `Cmd+S`           | `Ctrl+S`                 | Atomic save document to disk                            |
| **Save As**             | `Cmd+Shift+S`     | `Ctrl+Shift+S`           | Save copy with new path / encoding / permissions        |
| **Save All Documents**  | `Cmd+Alt+S`       | `Ctrl+Alt+S`             | Save all modified tabs at once                          |
| **Close Current Tab**   | `Cmd+W`           | `Ctrl+W`                 | Close active tab with dirty buffer warning              |
| **Close All Tabs**      | `Cmd+Shift+W`     | `Ctrl+Shift+W`           | Close all tabs prompting for modified files             |
| **Next Tab**            | `Ctrl+Tab`        | `Ctrl+Tab`               | Cycle forward through document tabs                     |
| **Previous Tab**        | `Ctrl+Shift+Tab`  | `Ctrl+Shift+Tab`         | Cycle backward through document tabs                    |
| **Print Document**      | `Cmd+Alt+Shift+P` | `Ctrl+Alt+Shift+P`       | Print document with custom margins and syntax rendering |
| **Restart Application** | `Cmd+Alt+R`       | `Ctrl+Alt+R`             | Restart application safely preserving session state     |

---

## 3. Editing & Selection

| Action                         | macOS Shortcut          | Linux / Windows Shortcut  | Description                                              |
| :----------------------------- | :---------------------- | :------------------------ | :------------------------------------------------------- |
| **Undo**                       | `Cmd+Z`                 | `Ctrl+Z`                  | Revert previous edit                                     |
| **Redo**                       | `Cmd+Shift+Z` / `Cmd+Y` | `Ctrl+Y` / `Ctrl+Shift+Z` | Replay reverted edit                                     |
| **Cut**                        | `Cmd+X`                 | `Ctrl+X`                  | Cut selection to system clipboard                        |
| **Copy**                       | `Cmd+C`                 | `Ctrl+C`                  | Copy selection to system clipboard                       |
| **Paste**                      | `Cmd+V`                 | `Ctrl+V`                  | Paste clipboard content at caret                         |
| **Select All**                 | `Cmd+A`                 | `Ctrl+A`                  | Select entire document buffer                            |
| **Duplicate Line / Selection** | `Cmd+D`                 | `Ctrl+D`                  | Duplicate current line or active selection               |
| **Delete Line**                | `Cmd+Shift+K`           | `Ctrl+Shift+K`            | Delete current line without copying to clipboard         |
| **Toggle Line Comment**        | `Cmd+/`                 | `Ctrl+/`                  | Comment or uncomment selected lines according to lexer   |
| **Toggle Block Comment**       | `Cmd+Alt+/`             | `Ctrl+Shift+/`            | Wrap selection in language block comment (`/* */`, etc.) |
| **Join Lines**                 | `Cmd+J`                 | `Ctrl+J`                  | Join selected lines into a single line                   |
| **Indent Selection**           | `Tab`                   | `Tab`                     | Indent block by configured tab width / spaces            |
| **Unindent Selection**         | `Shift+Tab`             | `Shift+Tab`               | Unindent block by configured tab width / spaces          |

---

## 4. Search, Navigation & Comparison

| Action                         | macOS Shortcut             | Linux / Windows Shortcut | Description                                            |
| :----------------------------- | :------------------------- | :----------------------- | :----------------------------------------------------- |
| **Find in Text**               | `Cmd+F`                    | `Ctrl+F`                 | Open in-editor search bar with regex/case support      |
| **Replace in Text**            | `Cmd+H`                    | `Ctrl+H`                 | Open in-editor replace bar                             |
| **Find Next**                  | `F3` / `Cmd+G`             | `F3`                     | Jump to next search occurrence                         |
| **Find Previous**              | `Shift+F3` / `Cmd+Shift+G` | `Shift+F3`               | Jump to previous search occurrence                     |
| **Find in Files**              | `Cmd+Shift+F`              | `Ctrl+Shift+F`           | Global multi-threaded project search dock              |
| **Go to Line / Column**        | `Cmd+L`                    | `Ctrl+G`                 | Jump directly to line and character offset             |
| **Next Visual Diff Chunk**     | `F7`                       | `F7`                     | Jump to next diff change chunk in File Diff dialog     |
| **Previous Visual Diff Chunk** | `Shift+F7`                 | `Shift+F7`               | Jump to previous diff change chunk in File Diff dialog |
| **Toggle Bookmark**            | `Cmd+F2`                   | `Ctrl+F2`                | Set or clear line bookmark indicator                   |
| **Next Bookmark**              | `F2`                       | `F2`                     | Move caret to next bookmark                            |
| **Previous Bookmark**          | `Shift+F2`                 | `Shift+F2`               | Move caret to previous bookmark                        |

---

## 5. Security & Developer Utilities

| Action                              | macOS Shortcut | Linux / Windows Shortcut | Description                                           |
| :---------------------------------- | :------------- | :----------------------- | :---------------------------------------------------- |
| **Encrypt Document**                | `Cmd+Alt+E`    | `Ctrl+Alt+E`             | Encrypt document using AES-256-CBC and PBKDF2         |
| **Decrypt Document**                | `Cmd+Alt+X`    | `Ctrl+Alt+X`             | Decrypt encrypted document buffer                     |
| **Evaluate JavaScript / Math**      | `Cmd+Alt+J`    | `Ctrl+Alt+J`             | Evaluate active line or selection in QJSEngine        |
| **Log Tail Monitoring**             | `Cmd+Alt+M`    | `Ctrl+Alt+M`             | Toggle real-time log file tailing and auto-scroll     |
| **Clipboard History**               | `Cmd+Alt+V`    | `Ctrl+Alt+V`             | Open multi-item clipboard history viewer              |
| **Format Document (Prettier/JSON)** | `Cmd+Shift+I`  | `Ctrl+Shift+I`           | Auto-format current file buffer                       |
| **Safe Markdown Preview**           | `Cmd+Shift+M`  | `Ctrl+Shift+M`           | Toggle sandboxed side-by-side Markdown HTML preview   |
| **Toggle Dark / Light Theme**       | `Cmd+Alt+T`    | `Ctrl+Alt+T`             | Cycle between configured Dark and Light theme presets |

---

## 6. Multi-Monitor & Window Management

| Action                         | macOS Shortcut  | Linux / Windows Shortcut | Description                                                    |
| :----------------------------- | :-------------- | :----------------------- | :------------------------------------------------------------- |
| **Move Window to Next Screen** | `Cmd+Alt+Right` | `Ctrl+Alt+Right`         | Shift SparkNotePad window to adjacent monitor on the right     |
| **Move Window to Prev Screen** | `Cmd+Alt+Left`  | `Ctrl+Alt+Left`          | Shift SparkNotePad window to adjacent monitor on the left      |
| **Center Window on Display**   | `Cmd+Alt+C`     | `Ctrl+Alt+C`             | Center application window on the current active display        |
| **Display Diagnostics**        | `Cmd+Alt+D`     | `Ctrl+Alt+D`             | Display DPI, screen geometry, scale factor, and color profiles |
| **Toggle Fullscreen Mode**     | `Cmd+Ctrl+F`    | `F11`                    | Toggle borderless fullscreen editor mode                       |
| **Toggle Project Tree Dock**   | `Cmd+Alt+B`     | `Ctrl+Alt+B`             | Show / hide the collapsible file system project dock           |

---

> [!TIP]
> You can view and search this full reference anytime inside the application by pressing `Shift+F1` or selecting **Help -> Shortcuts & Accessibility Guide**.
