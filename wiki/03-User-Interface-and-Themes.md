# 03. User Interface & Themes

SparkNotePad is crafted for visual comfort, accessibility, and high productivity during extended coding sessions.

---

## 1. 75 Developer Themes Across 25 Families

SparkNotePad features a curated library of **75 high-contrast syntax color themes** grouped into 25 distinct developer theme families:

| Theme Family         | Dark Mode Variants             | Light Mode Variants       |
| :------------------- | :----------------------------- | :------------------------ |
| **One Dark / Light** | Atom One Dark, One Dark Pro    | Atom One Light            |
| **Dracula**          | Dracula Official, Dracula Soft | Dracula Alucard           |
| **Nord**             | Nord Deep, Nord Snowstorm      | Nord Light                |
| **Catppuccin**       | Mocha, Macchiato, Frappé       | Latte                     |
| **Gruvbox**          | Gruvbox Dark Hard/Medium       | Gruvbox Light Hard/Medium |
| **Tokyo Night**      | Tokyo Night Storm, Night       | Tokyo Night Day           |
| **Solarized**        | Solarized Dark, Solarized Blue | Solarized Light           |
| **GitHub**           | GitHub Dark, GitHub Dimmed     | GitHub Light Default      |
| **Monokai**          | Monokai Pro, Monokai Classic   | Monokai Light             |
| **Rosé Pine**        | Rosé Pine Main, Rosé Pine Moon | Rosé Pine Dawn            |
| **SynthWave '84**    | SynthWave 84 Neon, Cyberpunk   | SynthWave Daylight        |
| **Palenight**        | Material Palenight, Deep Ocean | Material Light            |

### Switching Themes

- Press `Cmd+Alt+T` (macOS) or `Ctrl+Alt+T` (Linux) to toggle between Dark and Light mode variants instantly.
- Select specific themes via `Settings > Style Configurator...`.

---

## 2. WCAG 2.1 AA / AAA Accessibility Standards

Every theme's foreground and background colors are audited to ensure high contrast ratios:

- Standard body text meets minimum contrast ratio of **4.5:1** (WCAG AA).
- Large text, comments, and line numbers maintain legible contrast against canvas backdrops.
- High-contrast accessibility schemes are available for users with visual impairments.

---

## 3. Qt Advanced Docking System (ADS 5.1.1)

All side panels in SparkNotePad use the **Qt Advanced Docking System**:

- **Drag & Dock**: Click and drag panel header tabs to dock them to the Left, Right, Top, or Bottom.
- **Tabbed Stacking**: Drag one panel over another to stack them into tabbed notebooks.
- **Floating Windows**: Tear off any dock widget into an independent desktop window across multiple displays.
- **Auto-Hide Sidebars**: Pin panels to automatically slide out when hovered or clicked.

---

## 4. Toolbar Icon Packs Suite (15 Built-In Packs) & Button Customizer

SparkNotePad v1.3.8 introduces an extensive toolbar theming and customization architecture inspired by Notepad++:

### Built-in Toolbar Icon Packs

- **Modern Ribbon (Default)** (`modernRibbon`): High-DPI 20x20 monoline vector SVG suite with refined contours.
- **NotepadNext Classic (Silk)** (`notepadNext`): Authentic 16x16 Silk bitmap icons from upstream NotepadNext release, safely relocated to `icons/packs/notepadNext/`.
- **Notepad++ Retro** (`notepadPlusPlus`): Authentic high-DPI vector recreations of classic Notepad++ iconography.
- **android material 2026** (`androidMaterial`): Google Material You / M3 adaptive geometry and tonal depth.
- **Vista 2026** (`windowsVista`): Windows Vista Aero glass aesthetic with rich gloss and volumetric lighting.
- **XP 2026** (`windowsXp`): Authentic Windows XP Luna 3D skeuomorphic nostalgia rebuilt from official 1024×1024 High-Resolution master icons and Directory Opus 10 XP suite (master 64×64 PNG supporting 16×16 to 64×64).
- **Win10 2026** (`windows10`): Windows 10 Metro / MDL2 sharp wireframe monoline aesthetic.
- **Win11 2026** (`windows11`): Microsoft Fluent Design System 2 with rounded geometry and soft depth from Directory Opus 32px iconography.
- **Win 7 2026** (`windows7`): Authentic Windows 7/8 Aero Glass Azure suite rebuilt from UltimateDesktops Aero Glass Azure sprite sheet with translucent cyan glass gradients, polished chrome borders, and volumetric glass reflections (master 64×64 PNG).
- **Monterey 2026** (`macosMonterey`): macOS Monterey modern SF Symbols aesthetic with clean Apple styling.
- **NOTEPAD3 ICONPACK** (`notepad3`): Rizonesoft Notepad3 technical teal and sharp developer-focused vectors.
- **NOTEPAD4 ICONPACK** (`notepad4`): Zufuliu Notepad4 modern deep indigo flat vector iconography.
- **NOTEPAD2 ICONPACK** (`notepad2`): Florian Balmer Notepad2 classic retro 16-color nostalgic palette.
- **Monochrome Outline (Minimalist)** (`monochrome`): Clean 1.8px monoline wireframes engineered for dark and light themes.
- **Vibrant DuoTone (Neon/Cyber)** (`vibrantDuotone`): High-luminance electric cyan & radiant magenta dual-tone contrast.

### Dynamic Menubar & Right-Click Context Menu Synchronization

Changing toolbar icon packs instantaneously propagates active iconography across all top-level menubars, submenus, and the editor right-click context menu via `ToolbarIconManager::applyToWindow()`, with automatic fallback ensuring visual elegance.

### Dynamic Toolbar Icon Sizing

Switch icon size seamlessly from `Themes > Toolbar Icon Packs > Toolbar Icon Size` or toolbar right-click:

- **16 × 16** (Compact)
- **20 × 20** (Medium)
- **24 × 24** (Default / Recommended)
- **32 × 32** (Large / High-DPI)
- **48 × 48** (Extra Large)
- **64 × 64** (Ultra High-DPI)

### Visual Toolbar Customizer (`CustomizeToolbarDialog`)

- Add, remove, and reorder toolbar buttons using an intuitive dual-list dialog.
- Drag-and-drop reordering with `InternalMove` support.
- Insert custom separator divider lines.
- Instant search filter for fast action discovery.
- Reset to Defaults button restoring the standard 33-button layout.

### Editor Right-Click Context Menu Ergonomics

- **Convert Case Relocation**: Moved directly below `Select All` for intuitive text transformation.
- **Compact Aesthetics**: Scaled font down by 1–2px (11px) and icons to 14×14px with sleek 3px padding.
- **Non-Destructive Restoration**: Connected `aboutToHide` signal to cleanly preserve standard menu bar styling.

---

## 5. Multi-Monitor & Retina High-DPI Adaptation

SparkNotePad includes dedicated multi-monitor ergonomics:

- **Move to Next Screen**: `Cmd+Alt+Right` / `Ctrl+Alt+Right`
- **Move to Previous Screen**: `Cmd+Alt+Left` / `Ctrl+Alt+Left`
- **Center on Screen**: `Cmd+Alt+C` / `Ctrl+Alt+C`
- **Display Diagnostics Dialog**: `Cmd+Alt+D` / `Ctrl+Alt+D` (inspect DPI, DPR, refresh rate, and resolution).
- **Off-Screen Recovery Guard**: Prevents windows from being trapped off-screen when disconnecting external monitors.

---

Next Chapter: **[[04. Military-Grade Document Encryption|04-Military-Grade-Encryption]]**
