# Building from Source

SparkNotePad uses the **CMake** build system (version 3.20 or newer) and modern **C++20** compilers with **Ninja** for fast, deterministic parallel compilation.

This guide provides step-by-step instructions for building SparkNotePad on **macOS**, **Ubuntu / Debian**, **Fedora / RHEL**, and **FreeBSD**.

---

## 1. Prerequisites & Compiler Support

SparkNotePad requires:

- **C++20 Standard Compiler:** Apple Clang 14+, GCC 12+, or LLVM Clang 15+.
- **CMake:** Version 3.20 or higher.
- **Ninja Build:** Recommended generator for fast multi-threaded builds.
- **Qt 6 Framework:** Qt 6.5.0 or newer (`qtbase`, `qt5compat`, `qttools`).

Vendored core engines (Scintilla 5.6.7, Lexilla 5.5.4, Lua 5.5.1, uchardet) are embedded in `thirdparty/` and require no manual installation. Secondary Qt helper libraries (Qt-Advanced-Docking-System, SingleApplication, EditorConfig Core) are retrieved automatically via CPM (CMake Package Manager) during initial CMake configuration.

---

## 2. Building on macOS (Apple Silicon & Intel)

### Step 1: Install Build Dependencies via Homebrew

```bash
brew update
brew install cmake ninja pkg-config qt@6
```

### Step 2: Clone the Repository

```bash
git clone https://github.com/alsyundawy/SparkNotePad.git
cd SparkNotePad
```

### Step 3: Configure CMake

Point CMake to the Homebrew Qt 6 installation:

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="$(brew --prefix qt@6)"
```

### Step 4: Compile the Application Bundle

```bash
cmake --build build --parallel "$(sysctl -n hw.ncpu)"
```

Upon completion, the native macOS bundle is located at:

```bash
open ./build/src/SparkNotePad.app
```

---

## 3. Building on Ubuntu & Debian Linux

### Step 1: Install Packages

**Ubuntu 24.04 / 26.04 & Debian 13+:**

```bash
sudo apt update
sudo apt install -y \
    build-essential cmake ninja-build pkgconf \
    qt6-base-dev qt6-base-private-dev qt6-base-dev-tools \
    qt6-5compat-dev qt6-tools-dev qt6-tools-dev-tools linguist-qt6 \
    libgl1-mesa-dev libxkbcommon-dev libxkbcommon-x11-0 \
    libfontconfig1-dev libfreetype6-dev libxcb1-dev libxcb-cursor-dev \
    dpkg-dev rpm
```

**Ubuntu 22.04 LTS & Debian 12:**

```bash
sudo apt update
sudo apt install -y \
    build-essential g++-12 cmake ninja-build pkg-config \
    qt6-base-dev qt6-base-dev-tools \
    qt6-5compat-dev qt6-tools-dev qt6-tools-dev-tools linguist-qt6 \
    libgl1-mesa-dev libxkbcommon-dev libxkbcommon-x11-0 \
    libfontconfig1-dev libfreetype6-dev libxcb1-dev libxcb-cursor-dev \
    dpkg-dev rpm
```

### Step 2: Configure & Compile

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel "$(nproc)"
```

### Step 3: Execute Binary

```bash
./build/src/SparkNotePad
```

---

## 4. Building on Fedora, RHEL & AlmaLinux

### Step 1: Install Build Dependencies via DNF

```bash
sudo dnf install -y \
    gcc-c++ cmake ninja-build \
    qt6-qtbase-devel qt6-qt5compat-devel qt6-qttools-devel \
    mesa-libGL-devel libxkbcommon-devel fontconfig-devel freetype-devel \
    rpm-build dpkg-dev
```

### Step 2: Configure & Compile

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel "$(nproc)"
```

---

## 5. Building on FreeBSD (13.x–15.x)

### Step 1: Install Dependencies via `pkg`

```bash
sudo pkg update
sudo pkg install -y \
    cmake ninja pkgconf \
    qt6-base qt6-5compat qt6-tools \
    libxkbcommon fontconfig freetype2
```

### Step 2: Configure & Compile

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build -j "$(sysctl -n hw.ncpu)"
```

---

## 6. Generating Installable `.deb` and `.rpm` Packages

SparkNotePad includes automated CPack packaging definitions:

```bash
# Build Debian package (.deb)
cmake --build build --target deb

# Build RedHat package (.rpm)
cmake --build build --target rpm
```

The compiled distribution packages will be created inside `build/`:

- `build/sparknotepad_1.3.7_amd64.deb`
- `build/sparknotepad-1.3.7-1.x86_64.rpm`

Install with automatic dependency resolution:

```bash
# Ubuntu / Debian
sudo apt install ./build/sparknotepad_1.3.7_amd64.deb

# Fedora / RHEL
sudo dnf install ./build/sparknotepad-1.3.7-1.x86_64.rpm
```
