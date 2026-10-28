# Kompilasi dari Kode Sumber (Building from Source)

SparkNotePad menggunakan sistem pembangunan **CMake** (versi 3.20 atau yang lebih baru) serta kompilator standar **C++20** modern yang dipadukan dengan **Ninja** untuk proses kompilasi paralel yang cepat dan deterministik.

Panduan ini menyediakan instruksi langkah demi langkah untuk membangun SparkNotePad pada **macOS**, **Ubuntu / Debian**, **Fedora / RHEL**, dan **FreeBSD**.

---

## 1. Prasyarat & Dukungan Kompilator

SparkNotePad memerlukan komponen berikut:

- **Kompilator Standar C++20:** Apple Clang 14+, GCC 12+, atau LLVM Clang 15+.
- **CMake:** Versi 3.20 ke atas.
- **Generator Ninja:** Sangat disarankan untuk kompilasi multi-thread yang optimal.
- **Kerangka Kerja Qt 6:** Versi Qt 6.5.0 atau yang lebih baru (`qtbase`, `qt5compat`, `qttools`).

Mesin inti penyuntingan (Scintilla 5.6.7, Lexilla 5.5.4, Lua 5.5.1, uchardet) telah tersemat langsung di dalam folder `thirdparty/` dan tidak membutuhkan instalasi manual terpisah. Pustaka utilitas pendukung (seperti Qt-Advanced-Docking-System, SingleApplication, dan EditorConfig Core) akan diunduh secara otomatis via CPM (_CMake Package Manager_) pada proses konfigurasi awal.

---

## 2. Kompilasi di macOS (Apple Silicon & Intel)

### Langkah 1: Pasang Dependensi melalui Homebrew

```bash
brew update
brew install cmake ninja pkg-config qt@6
```

### Langkah 2: Klon Repositori

```bash
git clone https://github.com/alsyundawy/SparkNotePad.git
cd SparkNotePad
```

### Langkah 3: Konfigurasi CMake

Arahkan CMake ke direktori instalasi Qt 6 Homebrew:

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="$(brew --prefix qt@6)"
```

### Langkah 4: Bangun Berkas Aplikasi

```bash
cmake --build build --parallel "$(sysctl -n hw.ncpu)"
```

Setelah kompilasi selesai, bundel aplikasi macOS siap dijalankan:

```bash
open ./build/src/SparkNotePad.app
```

---

## 3. Kompilasi di Ubuntu & Debian Linux

### Langkah 1: Pasang Paket Dependensi

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

### Langkah 2: Konfigurasi & Kompilasi

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel "$(nproc)"
```

### Langkah 3: Jalankan Program

```bash
./build/src/SparkNotePad
```

---

## 4. Kompilasi di Fedora, RHEL & AlmaLinux

### Langkah 1: Pasang Paket via DNF

```bash
sudo dnf install -y \
    gcc-c++ cmake ninja-build \
    qt6-qtbase-devel qt6-qt5compat-devel qt6-qttools-devel \
    mesa-libGL-devel libxkbcommon-devel fontconfig-devel freetype-devel \
    rpm-build dpkg-dev
```

### Langkah 2: Konfigurasi & Kompilasi

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel "$(nproc)"
```

---

## 5. Kompilasi di FreeBSD (13.x–15.x)

### Langkah 1: Pasang Dependensi via `pkg`

```bash
sudo pkg update
sudo pkg install -y \
    cmake ninja pkgconf \
    qt6-base qt6-5compat qt6-tools \
    libxkbcommon fontconfig freetype2
```

### Langkah 2: Konfigurasi & Kompilasi

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build -j "$(sysctl -n hw.ncpu)"
```

---

## 6. Membuat Paket Pemasang `.deb` dan `.rpm`

SparkNotePad dilengkapi skrip pembuatan paket CPack otomatis:

```bash
# Membuat paket Debian (.deb)
cmake --build build --target deb

# Membuat paket RedHat (.rpm)
cmake --build build --target rpm
```

Berkas paket hasil kompilasi akan diletakkan di dalam folder `build/`:

- `build/sparknotepad_1.3.7_amd64.deb`
- `build/sparknotepad-1.3.7-1.x86_64.rpm`

Pasang dengan resolusi dependensi otomatis:

```bash
# Ubuntu / Debian
sudo apt install ./build/sparknotepad_1.3.7_amd64.deb

# Fedora / RHEL
sudo dnf install ./build/sparknotepad-1.3.7-1.x86_64.rpm
```
