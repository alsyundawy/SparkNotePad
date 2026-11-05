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

### Switching Themes & Toolbar Configurations

- Press `Cmd+Alt+T` (macOS) or `Ctrl+Alt+T` (Linux) to toggle between Dark and Light mode variants instantly.
- Select themes and configure toolbar icon packs via `Preferences > Themes & Toolbar` tab.
- Customize toolbar buttons and action order via `Preferences > Custom Toolbar Config` tab or `Settings > Customize Toolbar...`.

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

## 4. Toolbar Icon Packs Suite (40 Built-In Packs), Plus (+) Tab Button & Visual Customizer

SparkNotePad v1.3.8 features a comprehensive toolbar theming and customization architecture with 40 curated icon packs, full high-DPI (2x Retina) multi-resolution support, and dynamic Dark Mode luminance adaptation:

### Built-in Toolbar Icon Packs (40 Packs)

1. **Modern Ribbon (Default)** (`modernRibbon`): High-DPI 20x20 monoline vector SVG suite with refined contours (Default).
2. **Fugue Classic (Silk Suite)** (`fugue`): Authentic pixel-perfect Fugue 3.5.6 iconography by Yusuke Kamiyamane across all 6 resolutions.
3. **Windows Vista (Aero Glass)** (`aero_vista`): Windows Vista Aero vector iconography.
4. **Windows 7 (Aero Glass Vector)** (`aero_win7`): Windows 7 Aero Glass vector iconography.
5. **Windows 7 (Classic Aero Desktop)** (`windows7`): Authentic Windows 7/8 Aero Glass desktop suite rebuilt with translucent cyan gradients and polished chrome borders.
6. **Microsoft Fluent Design Regular** (`fluent_regular`): Microsoft Fluent Design regular vector suite.
7. **Microsoft Fluent Design Filled** (`fluent_filled`): Microsoft Fluent Design filled bold vector suite.
8. **Windows 11 (Fluent 2)** (`windows11`): Microsoft Fluent Design System 2 with rounded geometry from Directory Opus 32px iconography.
9. **Windows 10 (Modern Flat)** (`windows10_flat`): Windows 10 modern flat vector iconography suite.
10. **Arch Linux (Rolling Cyan)** (`arch_blue`): Arch Linux rolling cyan minimalist vector suite.
11. **Notepad3 (Warm Amber)** (`notepad3_warm`): Warm amber and brown accents inspired by the Notepad3 editor.
12. **Notepad++ (Modern Green)** (`notepadpp_green`): Refreshed modern green Notepad++ style icons.
13. **Notepad++ Classic** (`notepadPlusPlus`): Authentic high-DPI vector recreations of classic Notepad++ iconography.
14. **Monoline Minimal** (`monoline_minimal`): Ultra-clean minimalist outline wireframe icons with automatic dark-mode luminance inversion.
15. **Glossy Orange** (`glossy_orange`): Vibrant skeuomorphic orange vector icons.
16. **Pixel Design (Fugue-like)** (`pixel_fugue_like`): Nostalgic pixel-art iconography reminiscent of desktop classics.
17. **Material Design (Sharp Vector)** (`material_sharp`): Google Material Design iconography with sharp geometric vector lines.
18. **Windows XP (Luna Classic)** (`windowsXp`): Authentic Windows XP Luna 3D skeuomorphic nostalgia rebuilt from official master icons (master 64×64 PNG supporting 16×16 to 64×64).
19. **NotepadNext Classic (Silk)** (`notepadNext`): Authentic 16x16 Silk bitmap icons from NotepadNext.
20. **Vibrant DuoTone (Cyberpunk)** (`vibrantDuotone`): High-luminance electric cyan & radiant magenta dual-tone contrast.
21. **Monochrome Outline (Minimalist)** (`monochrome`): Clean 1.8px monoline wireframes engineered for dark and light themes.
22. **Elegant Themes** (`eleganticons`): Authentic classic 48x48 iconography by Nicholas Roach across all resolution scales.
23. **Seven Classic** (`seven`): Classic Windows 7 / Seven glossy desktop iconography suite across 6 resolutions (16x16, 20x20, 24x24, 32x32, 48x48, 64x64).
24. **Mignon Icon Theme** (`mignon`): Modern, elegant vector icons by Igor F. Moraes.
25. **Palette Icon Theme** (`palette`): Colorful vector icon theme by mjkim0727.
26. **Diagonal Icon Theme** (`diagonal`): Dynamic diagonal vector styling by Tamás Csábi.
27. **Argon Icon Theme** (`argon`): Modern geometric vector iconography by Stuart Hayhurst.
28. **Reversal Icon Theme** (`reversal`): Clean rounded vector icons by yeyushengfan258.
29. **BigSur Icon Theme** (`bigsur`): Apple Big Sur squircle vector iconography by yeyushengfan258.
30. **Mkos Big Sur** (`mkos_bigsur`): Detailed macOS Big Sur icon theme by zayronxio.
31. **We10X Icon Theme** (`we10x`): Windows 10X modern colorful vector icons by yeyushengfan258.
32. **MacTahoe Icon Theme** (`mactahoe`): macOS Tahoe styled vector iconography by Vince Liuice.
33. **UOS Full Distro Icons** (`uos`): Deepin/UOS full distro vector icons by zayronxio.
34. **Deepin Icons 2022** (`deepin`): Deepin OS desktop vector icons by zayronxio.
35. **WhiteSur Icon Theme** (`whitesur`): Flagship WhiteSur vector icon theme by Vince Liuice.
36. **Elementary KDE Icons** (`elementary_kde`): Elementary OS aesthetic adapted for KDE by zayronxio.
37. **McMojave Circle** (`mcmojave_circle`): Circular macOS Mojave vector icons by Vince Liuice.
38. **Colloid Icon Theme** (`colloid`): Clean modern vector iconography by Vince Liuice.
39. **Fluent KDE** (`fluent_kde`): Microsoft Fluent theme for KDE by zayronxio.
40. **Win11OS KDE** (`win11os_kde`): Windows 11 styled vector icons by yeyushengfan258.

### High-DPI & Retina Clarity

All icon packs feature multi-resolution pixmaps (16x16, 20x20, 24x24, 32x32, 48x48, 64x64) and automatic `@2x` Retina DPR rendering (`setDevicePixelRatio(2.0)`). Vector SVG packs render dynamically on physical display coordinates without blur or interpolation artifacts.

### Dark Mode Luminance Tone Mapping

Under dark themes, wireframe outline packs (such as `monoline_minimal`) automatically invert dark pixels to luminous silver-white, while chromatic packs (`material_sharp`, `pixel_fugue_like`, `notepadpp_green`, `notepad3_warm`) undergo HSL tone mapping to boost lightness while preserving original hue and saturation.

### Plus (+) Button on Tab Bar

Every dock area title bar includes an interactive **Plus (+)** button positioned directly next to the open editor tabs. Clicking this button immediately creates a new editor tab ("New 1", "New 2", etc.).

### Dynamic Menubar & Right-Click Context Menu Synchronization

Changing toolbar icon packs instantaneously propagates active iconography across all top-level menubars, submenus, tab context menus, and the editor right-click context menu via `ToolbarIconManager::applyToWindow()` and `applyToMenu()`. Default toolbar icons render at 24×24 and menu icons at 16×16 (with 32×32 retina assets).

### Dynamic Toolbar Icon Sizing

Switch icon size seamlessly from `Themes > Toolbar Icon Packs > Toolbar Icon Size` or toolbar right-click:

- **16 × 16** (Compact)
- **20 × 20** (Medium)
- **24 × 24** (Default / Recommended)
- **32 × 32** (Large / High-DPI)
- **48 × 48** (Extra Large)
- **64 × 64** (Ultra High-DPI)

### Dedicated Preferences Tabs for Themes and Toolbar

- **Themes & Toolbar Tab (Tab 3)**:
  - Consolidates active theme selection, window opacity slider, tail -f reload interval, and multi-display safety guards.
  - Comprehensive Toolbar Icon Packs manager featuring 40 distinct packs, active icon pack selector with responsive description card styling, multi-DPI icon size selector (16x16 to 64x64), live icon sample preview strip, custom icon packs folder launcher, and live rescan.
- **Custom Toolbar Config Tab (Tab 4)**:
  - Embedded visual toolbar customizer (`CustomizeToolbarWidget`) directly inside Preferences.
  - Add, remove, and reorder toolbar buttons using intuitive dual list views with search filtering.
  - Insert separators and reset toolbar to default configuration with instant hot-reload.

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
