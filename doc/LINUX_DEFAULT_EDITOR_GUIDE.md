# SparkNotePad Linux Default Editor Engineering Guide

**Document Version:** 1.0.0  
**Target Script:** [`SparkNotePad-Default-Editor-Linux.sh`](../SparkNotePad-Default-Editor-Linux.sh) & [`SparkNotePad-Default-Editor.sh`](../SparkNotePad-Default-Editor.sh)  
**Maintainer:** [Harry Dertin Sutisna Alsyundawy (@alsyundawy)](https://github.com/alsyundawy) — [ALSYUNDAWY IT SOLUTION](https://alsyundawy.com)  
**Standard Compliance:** FreeDesktop.org XDG MIME Applications v1.0.1, Debian Policy Manual (Alternatives), FHS 3.0, POSIX.1-2017

---

## 1. Executive Summary & Mission

In the fragmented ecosystem of modern Linux distributions, establishing an application as the **system-wide default text and code editor** requires far more than setting a single configuration file. Different distributions, packaging formats (`.deb`, `.rpm`), desktop environments (GNOME, KDE Plasma, XFCE, Cinnamon, MATE, LXQt, Budgie, COSMIC, Deepin, Pantheon), window managers (Sway, Hyprland, i3), and terminal workflows utilize divergent registries, priority hierarchies, and environment variables.

`SparkNotePad-Default-Editor-Linux.sh` is an industrial-strength, defensive Bash automation engine designed to establish **SparkNotePad** as the default editor with 100% determinism across **all Debian-based and RPM-based distributions**, as well as independent Linux systems.

---

## 2. Comprehensive Distribution & Package Matrix

The automation engine features dynamic OS detection via `/etc/os-release`, `/etc/debian_version`, `/etc/redhat-release`, and toolchain inspection:

| Distribution Family          | Representative Distributions                                                                                                                                                                                                                                                                                                           | Package Format |         Core Package Managers          |                 Default Alternatives Engine                  |
| :--------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :------------: | :------------------------------------: | :----------------------------------------------------------: |
| **Debian / Ubuntu Family**   | Debian (11 Bullseye, 12 Bookworm, 13 Trixie, Sid)<br>Ubuntu (20.04, 22.04 LTS, 24.04 LTS, 26.04)<br>Linux Mint (Cinnamon, MATE, XFCE)<br>Pop!_OS, Elementary OS, Zorin OS<br>Kali Linux, Parrot OS, MX Linux, AntiX<br>Devuan, PureOS, Raspberry Pi OS<br>Bodhi Linux, Peppermint OS, SparkyLinux<br>Deepin Linux, Tuxedo OS, KDE neon |     `.deb`     | `apt`<br>`apt-get`<br>`nala`<br>`dpkg` |    `update-alternatives` (`/var/lib/dpkg/alternatives/`)     |
| **Red Hat / Fedora Family**  | Fedora (38, 39, 40, 41, 42)<br>Red Hat Enterprise Linux (RHEL 8, 9, 10)<br>CentOS Stream (8, 9, 10)<br>Rocky Linux (8, 9), AlmaLinux (8, 9)<br>Oracle Linux (8, 9), Amazon Linux 2023<br>EuroLinux                                                                                                                                     |     `.rpm`     | `dnf`<br>`dnf5`<br>`rpm`<br>`microdnf` |          `alternatives` (`/var/lib/alternatives/`)           |
| **SUSE Family**              | openSUSE Tumbleweed (Rolling)<br>openSUSE Leap (15.5, 15.6, 16.0)<br>SUSE Linux Enterprise Server/Desktop (SLES/SLED 15)                                                                                                                                                                                                               |     `.rpm`     |           `zypper`<br>`rpm`            | `update-alternatives`<br>`alternatives`<br>`libalternatives` |
| **Mandriva / Mageia Family** | Mageia (8, 9), OpenMandriva Lx, ROSA Linux, PCLinuxOS                                                                                                                                                                                                                                                                                  |     `.rpm`     |       `urpmi`<br>`dnf`<br>`rpm`        |            `alternatives` / `update-alternatives`            |
| **Independent / Rolling**    | Arch Linux, Manjaro, EndeavourOS, Alpine, Void, Solus, Gentoo, NixOS                                                                                                                                                                                                                                                                   |     Custom     |  `pacman`<br>`apk`<br>`xbps`<br>`nix`  |            Universal FreeDesktop XDG / Shell ENV             |

---

## 3. Desktop Environments (DE) & Window Managers (WM)

Desktop environments parse default applications through varying combinations of FreeDesktop XDG specifications, GIO/GVFS daemons, KService sycoca caches, and configuration registries:

### A. Full Desktop Environments

1. **GNOME (Shell 40, 42, 43, 44, 45, 46, 47)**:
   - Handled via `gio mime <mimetype> <desktop_file>` and `~/.config/mimeapps.list` / `gnome-mimeapps.list`.
   - File Manager: Nautilus (GNOME Files).
2. **KDE Plasma (Plasma 5 & Plasma 6)**:
   - Managed via `kwriteconfig6` / `kwriteconfig5` modifying `~/.config/mimeapps.list` under `[Default Applications]`.
   - State refreshed via `kbuildsycoca6 --noincremental` or `kbuildsycoca5`.
   - File Manager: Dolphin.
3. **XFCE (4.16, 4.18)**:
   - Controlled via `xfce4-mime-settings` and FreeDesktop `mimeapps.list`.
   - File Manager: Thunar (`thunar -q` refresh).
4. **Cinnamon (Linux Mint)**:
   - Reads `~/.config/mimeapps.list` and `~/.config/cinnamon-mimeapps.list`.
   - GSettings schema: `org.cinnamon.desktop.default-applications`.
   - File Manager: Nemo.
5. **MATE**:
   - Reads `~/.config/mimeapps.list` and `~/.config/mate-mimeapps.list`.
   - GSettings schema: `org.mate.desktop.default-applications`.
   - File Manager: Caja.
6. **LXQt & LXDE**:
   - Configured via `lxqt-config-file-associations` and `~/.config/mimeapps.list`.
   - File Manager: PCManFM-Qt / PCManFM.
7. **Budgie (Solus & Ubuntu Budgie)**:
   - FreeDesktop XDG + GIO/GVFS integration.
8. **COSMIC (Pop!\_OS 24.04+ Rust Desktop)**:
   - Wayland-native Smithay compositor strictly honoring `~/.config/mimeapps.list`.
   - File Manager: COSMIC Files.
9. **Pantheon (Elementary OS)**:
   - FreeDesktop XDG + GIO integration.
   - File Manager: Pantheon Files (`io.elementary.files`).
10. **Deepin DDE**:
    - DDE Daemon integration with XDG `mimeapps.list`.
11. **Enlightenment (E25) / Moksha**:
    - EFL-based desktop integration via standard XDG MIME.

### B. Tiling Window Managers & Wayland Compositors

- **Wayland**: Sway, Hyprland, Wayfire, River.
- **X11**: i3, bspwm, AwesomeWM, dwm, Qtile, xmonad.
- **Behavior**: Tiling WMs rely entirely on FreeDesktop standard commands (`xdg-open`, `xdg-mime`), user `mimeapps.list`, and terminal environment variables (`$VISUAL`, `$EDITOR`).

---

## 4. The 5-Layer Integration Architecture

`SparkNotePad-Default-Editor-Linux.sh` executes a defense-in-depth, 5-layer registration workflow ensuring complete system coverage:

```text
 ┌────────────────────────────────────────────────────────────────────────┐
 │              SPARKNOTEPAD LINUX 5-LAYER INTEGRATION ENGINE             │
 └───────────────────────────────────┬────────────────────────────────────┘
                                     │
         ┌───────────────────────────┴───────────────────────────┐
         ▼                                                       ▼
 ┌───────────────────────────────┐               ┌───────────────────────────────┐
 │ Layer 1: FreeDesktop XDG MIME │               │ Layer 2: DE Native Tool APIs  │
 │ • xdg-mime default <mime>     │               │ • gio mime <mime>             │
 │ • ~/.config/mimeapps.list     │               │ • kwriteconfig6 / kwriteconfig│
 │ • ~/.config/*-mimeapps.list   │               │ • kbuildsycoca6 cache reload  │
 │ • update-desktop-database     │               │ • XFCE / Thunar refresh       │
 └───────────────┬───────────────┘               └───────────────┬───────────────┘
                 │                                               │
                 └───────────────────────┬───────────────────────┘
                                         ▼
 ┌────────────────────────────────────────────────────────────────────────┐
 │ Layer 3: System Terminal Alternatives                                  │
 │ • Debian/Ubuntu: update-alternatives --install /usr/bin/editor editor  │
 │ • Fedora/RHEL/openSUSE: alternatives --install /usr/bin/editor editor  │
 │ • Priority 100 / Fallback gnome-text-editor mapping                   │
 └───────────────────────────────────────┬────────────────────────────────┘
                                         ▼
 ┌────────────────────────────────────────────────────────────────────────┐
 │ Layer 4: Shell Environment Variables                                   │
 │ • User scope: Idempotent exports in ~/.bashrc, ~/.profile, ~/.zshrc    │
 │ • System scope: /etc/profile.d/sparknotepad-editor.sh (root)           │
 │ • Variables: VISUAL="SparkNotePad", EDITOR="SparkNotePad", SUDO_EDITOR │
 └───────────────────────────────────────┬────────────────────────────────┘
                                         ▼
 ┌────────────────────────────────────────────────────────────────────────┐
 │ Layer 5: Binary & Desktop Entry Resolution                             │
 │ • Auto-discovers native /usr/bin, /usr/local/bin, or Flatpak binaries  │
 │ • Self-healing scaffolding: generates valid ~/.local/share/            │
 │   applications/SparkNotePad.desktop if uninstalled                     │
 └────────────────────────────────────────────────────────────────────────┘
```

---

## 5. Comprehensive MIME Types Catalog (80+ Types)

The script associates SparkNotePad with over 80 MIME classifications:

| Category                     | Associated MIME Types                                                                                                                                                                                                            | Typical Extensions                                                     |
| :--------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :--------------------------------------------------------------------- |
| **Plain Text & Notes**       | `text/plain`, `text/plain;charset=utf-8`, `application/x-zerosize`, `text/x-log`                                                                                                                                                 | `.txt`, `.log`, `.text`, `.nfo`, `.1st`, extensionless                 |
| **Markdown & Documentation** | `text/markdown`, `text/x-markdown`, `text/x-rst`, `text/x-asciidoc`, `text/x-tex`, `text/x-latex`                                                                                                                                | `.md`, `.markdown`, `.rst`, `.adoc`, `.tex`                            |
| **Web & Stylesheets**        | `text/html`, `application/xhtml+xml`, `text/css`, `text/less`, `text/x-scss`, `text/x-sass`                                                                                                                                      | `.html`, `.htm`, `.xhtml`, `.css`, `.scss`, `.less`                    |
| **JavaScript & TypeScript**  | `text/javascript`, `application/javascript`, `application/x-javascript`, `text/typescript`, `application/typescript`                                                                                                             | `.js`, `.mjs`, `.cjs`, `.ts`, `.mts`, `.jsx`, `.tsx`                   |
| **Serialization & Data**     | `application/json`, `application/ld+json`, `application/x-yaml`, `text/x-yaml`, `text/yaml`, `application/toml`, `text/x-toml`, `application/xml`, `text/xml`, `text/csv`, `text/tab-separated-values`                           | `.json`, `.yaml`, `.yml`, `.toml`, `.xml`, `.csv`, `.tsv`              |
| **Systems Languages**        | `text/x-c`, `text/x-c++`, `text/x-csrc`, `text/x-chdr`, `text/x-c++src`, `text/x-c++hdr`, `text/x-objcsrc`, `text/x-csharp`, `text/x-rust`, `text/rust`, `text/x-go`, `text/x-golang`, `text/x-zig`, `text/x-nim`, `text/x-dsrc` | `.c`, `.h`, `.cpp`, `.hpp`, `.cs`, `.rs`, `.go`, `.zig`, `.nim`        |
| **Scripting Languages**      | `text/x-python`, `text/x-python3`, `text/x-script.python`, `text/x-lua`, `text/x-ruby`, `application/x-ruby`, `text/x-perl`, `application/x-perl`, `text/x-php`, `application/x-php`, `text/x-tcl`, `text/x-r`                   | `.py`, `.pyw`, `.lua`, `.rb`, `.pl`, `.php`, `.tcl`, `.r`              |
| **Shell & DevOps**           | `text/x-shellscript`, `application/x-shellscript`, `text/x-bash`, `text/x-zsh`, `text/x-fish`, `text/x-cmake`, `text/x-makefile`, `text/x-dockerfile`, `text/x-diff`, `text/x-patch`                                             | `.sh`, `.bash`, `.zsh`, `.fish`, `CMakeLists.txt`, `Makefile`, `.diff` |
| **Configs & Databases**      | `text/x-config`, `text/x-ini`, `application/x-wine-extension-ini`, `text/x-sql`, `application/sql`                                                                                                                               | `.conf`, `.ini`, `.cfg`, `.sql`, `.env`                                |

---

## 6. Command-Line Reference & Usage

### Basic Usage

```bash
# Non-interactive execution (auto-confirm and apply immediately)
./SparkNotePad-Default-Editor-Linux.sh -f

# Run comprehensive diagnostic audit
./SparkNotePad-Default-Editor-Linux.sh --status

# Preview all configuration steps without modifying system (dry run)
./SparkNotePad-Default-Editor-Linux.sh -d

# Apply system-wide configuration (requires root)
sudo ./SparkNotePad-Default-Editor-Linux.sh --system

# Revert associations back to distribution native default editor
./SparkNotePad-Default-Editor-Linux.sh --restore
```

### Command-Line Arguments Table

| Option     | Long Option             | Description                                                                   |
| :--------- | :---------------------- | :---------------------------------------------------------------------------- |
| `-y`, `-f` | `--yes`, `--force`      | Non-interactive execution; skips interactive prompts.                         |
| `-d`       | `--dry-run`             | Simulates all actions and displays what would change.                         |
| `-s`       | `--status`, `--check`   | Runs a diagnostic audit of current XDG, DE, alternatives, and ENV states.     |
|            | `--system`              | Applies configurations system-wide (requires root privileges).                |
|            | `--user`                | Applies configurations to the current user (default).                         |
|            | `--restore`, `--revert` | Reverts associations to distro defaults (e.g. `gnome-text-editor`, `kwrite`). |
|            | `--bin-path <path>`     | Explicitly sets path to `SparkNotePad` executable binary.                     |
|            | `--desktop-path <path>` | Explicitly sets path to `SparkNotePad.desktop` desktop entry file.            |
| `-h`       | `--help`                | Displays help message and command syntax.                                     |
| `-v`       | `--version`             | Displays version and maintainer metadata.                                     |

---

## 7. Quality & Verification Standards

`SparkNotePad-Default-Editor-Linux.sh` complies strictly with:

- **ShellCheck**: 100% clean check (`shellcheck SparkNotePad-Default-Editor-Linux.sh` exits with 0 warnings).
- **shfmt / Trunk**: Strictly formatted with standard 2-space indentation and POSIX portable shell patterns.
- **Safety Invariants**:
  - `set -Eeuo pipefail` traps all unbound variables and pipeline failures.
  - Safe atomic file writes using `mktemp` prevent partial or corrupted configuration files.
  - Zero unquoted variable expansions prevent word-splitting and path-injection risks.
