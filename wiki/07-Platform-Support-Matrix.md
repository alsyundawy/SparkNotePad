# Platform & Operating System Support Matrix

SparkNotePad is engineered as a cross-platform text and code editor built on **modern C++20 and Qt 6 (6.5–6.12)**. This document provides the official platform support matrix, binary baselines, desktop environment compatibility, and deployment strategies across macOS, Linux, and FreeBSD.

---

## 1. Operating System Compatibility Matrix

| Operating System         | Version / Release                       | Architecture                   | Native Support Level        | Recommended Packaging         |
| :----------------------- | :-------------------------------------- | :----------------------------- | :-------------------------- | :---------------------------- |
| **macOS**                | 11 (Big Sur) to 26+                     | Universal (`arm64` + `x86_64`) | Tier 1 (Official Release)   | Signed `.dmg` / Homebrew Cask |
| **Ubuntu Linux**         | 22.04 LTS (Jammy Jellyfish)             | `x86_64` / `aarch64`           | Tier 1 (Canonical Baseline) | Native `.deb` (glibc 2.35)    |
| **Ubuntu Linux**         | 24.04 LTS (Noble) to 26.04 LTS          | `x86_64` / `aarch64`           | Tier 1 (Official Release)   | Native `.deb`                 |
| **Debian GNU/Linux**     | 12 (Bookworm) to 14 (Forky)             | `amd64` / `arm64`              | Tier 1 (Official Release)   | Native `.deb`                 |
| **Fedora Linux**         | 40, 41, 42, 43, 44                      | `x86_64` / `aarch64`           | Tier 1 (Official Release)   | Native `.rpm`                 |
| **RHEL / Rocky / Alma**  | 9, 10, 11                               | `x86_64` / `aarch64`           | Tier 1 (Workstation)        | Native `.rpm` (via EPEL)      |
| **openSUSE**             | Tumbleweed / Leap 15.6+                 | `x86_64`                       | Tier 2 (Supported)          | Native `.rpm` / Flatpak       |
| **Arch Linux / Manjaro** | Rolling                                 | `x86_64` / `aarch64`           | Tier 2 (Community PKGBUILD) | AUR / Native CMake Build      |
| **FreeBSD**              | 13.x, 14.x, 15-CURRENT                  | `amd64` / `aarch64`            | Tier 2 (Official Port)      | Native FreeBSD Port / `pkg`   |
| **Legacy Linux**         | glibc < 2.35 (CentOS 7/8, Ubuntu 20.04) | `x86_64`                       | Tier 3 (Containerized)      | AppImage / Flatpak            |

---

## 2. Binary Baseline & Portability: The glibc 2.35 Anchor

SparkNotePad compiles its canonical Linux release binaries against **Ubuntu 22.04 LTS (glibc 2.35)**. This provides two significant architectural benefits:

1. **Broad Forward Compatibility:** Linux dynamically linked binaries are backwards-incompatible with older glibc versions, but forward-compatible with newer ones. Compiling on glibc 2.35 guarantees that the same `.deb` and `.rpm` binaries run on Ubuntu 22.04, Ubuntu 24.04, Ubuntu 26.04, Debian 12/13/14, Fedora 40+, and RHEL/Alma 9+.
2. **Deterministic SONAME Dependencies:** Packaging scripts (`PackagingLinux.cmake`) enforce strict SONAME requirements (`libc.so.6(GLIBC_2.35)(64bit)` and `libxcb-cursor.so.0()(64bit)`), preventing unexpected runtime crashes due to missing symbol sets.

---

## 3. Desktop Environment Integration

SparkNotePad integrates natively with the primary window managers and desktop environments across platforms:

### macOS Integration

- **Cocoa Services Menu:** Registers system-wide text transformation actions (e.g., "Open in SparkNotePad", "Encrypt with SparkNotePad").
- **AppKit Dark Mode:** Automatically synchronizes with system theme preference changes (`NSAppearance`).
- **High-DPI Retina Support:** Sharp vector icon rendering at `@2x` and `@3x` pixel ratios with subpixel antialiasing.
- **Dock Progress & Badges:** Displays active background operations on the macOS Dock icon.

### Linux Desktop Environments (X11 & Wayland)

- **KDE Plasma 5 & Plasma 6:** Direct integration with `kwriteconfig6` and sycoca cache for seamless default editor configuration.
- **GNOME Shell (Wayland/X11):** GIO/GVFS MIME database integration with client-side window decoration support.
- **XFCE & MATE:** Standard FreeDesktop desktop entries with Thunar and Caja context menu support.
- **Wayland Display Server:** Fully functional on Wayland native sessions without requiring XWayland bridging.

---

## 4. Hardware Requirements

| Resource               | Minimum                                      | Recommended                                   |
| :--------------------- | :------------------------------------------- | :-------------------------------------------- |
| **Processor**          | 64-bit x86_64 or Apple Silicon (M1/M2/M3/M4) | Multi-core 64-bit modern processor            |
| **Memory (RAM)**       | 256 MB free RAM                              | 1 GB+ free RAM (for multi-megabyte log files) |
| **Disk Space**         | 80 MB available storage                      | 200 MB (including local theme caches)         |
| **Display Resolution** | 1024x768 minimum resolution                  | 1920x1080 or High-DPI 4K/Retina display       |

---

## 5. Security & Binary Hardening Standards

Every official SparkNotePad binary is compiled with modern defensive compiler flags:

- **Stack Protection:** `-fstack-protector-strong`
- **Source Hardening:** `-D_FORTIFY_SOURCE=2` (or `=3` on GCC 14+)
- **Position Independent Executable:** `-fPIE` and `-pie`
- **Full Relocation Read-Only:** `-Wl,-z,relro,-z,now`
- **No Executable Stack:** `-Wl,-z,noexecstack`
- **macOS Hardened Runtime:** Codesigned with entitlements preventing arbitrary dyld injection.
