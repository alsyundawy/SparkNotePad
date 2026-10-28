# 01. Getting Started & Installation

Welcome to the installation guide for **SparkNotePad v1.3.7**. SparkNotePad provides native standalone binaries across macOS, Linux, and FreeBSD without web runtime overhead.

---

## 1. System Requirements

| Operating System             | Supported Versions                      | Minimum Architecture                       | Memory & Storage       |
| :--------------------------- | :-------------------------------------- | :----------------------------------------- | :--------------------- |
| **macOS**                    | macOS 11.0 (Big Sur) – macOS 26.0+      | Apple Silicon (`arm64`) & Intel (`x86_64`) | 100 MB RAM, 60 MB Disk |
| **Linux (Ubuntu/Debian)**    | Ubuntu 22.04–26.04, Debian 12–14        | `x86_64` (glibc ≥ 2.35)                    | 100 MB RAM, 50 MB Disk |
| **Linux (Fedora/RHEL/SUSE)** | Fedora 40–44, RHEL/Rocky 9–11, openSUSE | `x86_64`                                   | 100 MB RAM, 50 MB Disk |
| **FreeBSD**                  | FreeBSD 13.x, 14.x, 15.x                | `amd64` (KDE Plasma 6 & XFCE)              | 100 MB RAM, 50 MB Disk |

---

## 2. Installation on macOS

### Method A: Standalone DMG Disk Image

1. Download the appropriate `.dmg` package from [GitHub Releases](https://github.com/alsyundawy/SparkNotePad/releases/latest):
   - **Apple Silicon (M1/M2/M3/M4):** `SparkNotePad-v1.3.7-arm64.dmg`
   - **Intel x86_64:** `SparkNotePad-v1.3.7-x64.dmg`
2. Double-click the downloaded `.dmg` file to mount it.
3. Drag **SparkNotePad.app** into your `/Applications` directory.
4. Eject the DMG disk image.

> [!NOTE]
> SparkNotePad is hardened with Apple Hardened Runtime. If macOS Gatekeeper presents a prompt on first launch, right-click `SparkNotePad.app` in Finder, select **Open**, and click **Open Anyway**.

---

## 3. Installation on Linux

### Ubuntu 22.04 LTS+ & Debian 12+ (`.deb`)

Download `sparknotepad_1.3.7_amd64.deb` and install it using `apt` with a leading `./` so all system Qt 6 dependencies are automatically resolved:

```bash
# Recommended high-level installation (auto-resolves dependencies):
sudo apt update
sudo apt install ./sparknotepad_1.3.7_amd64.deb
```

To remove the package:

```bash
sudo apt remove sparknotepad
```

### Fedora, RHEL 9+, Rocky Linux & openSUSE (`.rpm`)

Download `sparknotepad-1.3.7-1.x86_64.rpm` and install it using `dnf`:

```bash
sudo dnf install ./sparknotepad-1.3.7-1.x86_64.rpm
```

---

## 4. Native Installation on FreeBSD (KDE Plasma 6 & XFCE)

On FreeBSD 14.x and 15.x, SparkNotePad runs as a native ELF binary without compatibility layers:

```bash
# 1. Install prerequisites via pkg
sudo pkg install -y git cmake ninja pkgconf qt6 qt6-base qt6-5compat qt6-tools python3 uchardet libxkbcommon libxcb desktop-file-utils

# 2. Clone repository & compile
git clone https://github.com/alsyundawy/SparkNotePad.git
cd SparkNotePad
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/local -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build -j$(sysctl -n hw.ncpu)

# 3. Install globally
sudo cmake --install build
```

---

## 5. Setting SparkNotePad as Default Editor

SparkNotePad includes automated helper scripts for system file associations:

### macOS

Run the universal file association helper:

```bash
chmod +x SparkNotePad-Default-Editor.sh
./SparkNotePad-Default-Editor.sh
```

### Linux (GNOME, KDE Plasma, XFCE)

Configure default MIME type bindings across all text, code, script, and configuration formats:

```bash
chmod +x SparkNotePad-Default-Editor-Linux.sh
./SparkNotePad-Default-Editor-Linux.sh
```

---

Next Chapter: **[[02. Feature Overview|02-Feature-Overview]]**
