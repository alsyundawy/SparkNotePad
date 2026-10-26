# Native Build & Desktop Integration Guide — FreeBSD 14.x & 15.x (KDE Plasma 6 & XFCE)

## 1. Overview & Architecture

SparkNotePad adalah editor teks dan source code native yang sangat cepat, ringan, dan kaya fitur, dibangun menggunakan standar **C++20**, GUI **Qt 6**, serta engine rendering teks **Scintilla 5.6.7** dan syntax lexer **Lexilla 5.5.4**.

Di FreeBSD, SparkNotePad berjalan sebagai aplikasi ELF native murni tanpa lapisan kompatibilitas Linux (tanpa Linuxulator) dan tanpa Wine.

### Karakteristik Utama Portabilitas FreeBSD

* **In-Tree Vendored Engine**: Scintilla, Lexilla, Lua, dan uchardet di-vendor langsung di dalam repositori (`thirdparty/`), memastikan konsistensi build dan kompatibilitas 100% dengan toolchain Clang di FreeBSD.
* **Native Qt 6 Integration**: Memanfaatkan port resmi `devel/qt6-base` dan `devel/qt6-5compat` yang dipelihara oleh tim `kde@FreeBSD.org`.
* **Zero Linux-Kernel Coupling**: Menggunakan POSIX APIs standar, threads native (`libthr`), dan protokol display X11/Wayland melalui Qt6 GUI sub-framework.

---

## 2. Prasyarat Sistem & Desktop Environment

Sebelum mengompilasi SparkNotePad, pastikan lingkungan grafis FreeBSD Anda telah terkonfigurasi dengan benar menggunakan **KDE Plasma 6** (rekomendasi utama) atau **XFCE** (alternatif ringan).

### 2.1 Akselerasi Grafis GPU (DRM Kernel Module)

Akselerasi grafis perangkat keras sangat penting untuk rendering sub-millisecond Qt6 dan OpenGL.

Pasang paket driver grafis sesuai GPU fisik mesin Anda:

```bash
sudo pkg install -y drm-kmod
```

Tambahkan driver yang sesuai ke `/etc/rc.conf`:

* **Intel HD/UHD/Iris Graphics**:

  ```bash
  sudo sysrc kld_list+="i915kms"
  ```

* **AMD Radeon / Radeon RX**:

  ```bash
  sudo sysrc kld_list+="amdgpu"
  ```

* **NVIDIA GeForce**:

  ```bash
  sudo pkg install -y nvidia-driver
  sudo sysrc kld_list+="nvidia-modeset"
  ```

Tambahkan akun user Anda ke grup `video` dan `wheel`:

```bash
sudo pw groupmod video -m $(whoami)
sudo pw groupmod wheel -m $(whoami)
```

### 2.2 Konfigurasi Filesystem Kompatibilitas (`/etc/fstab`)

Aplikasi berbasis Qt6 dan D-Bus membutuhkan `/proc` dan `/dev/fd` ter-mount pada sistem. Tambahkan ke `/etc/fstab`:

```bash
echo 'proc  /proc   procfs  rw  0 0' | sudo tee -a /etc/fstab
echo 'fdesc /dev/fd fdescfs rw  0 0' | sudo tee -a /etc/fstab
sudo mount /proc 2>/dev/null || true
sudo mount /dev/fd 2>/dev/null || true
```

### 2.3 Pilihan Desktop 1: KDE Plasma 6 (Direkomendasikan)

KDE Plasma 6 memberikan integrasi tema native paling harmonis dengan aplikasi Qt6.

```bash
# 1. Pasang paket meta KDE6, display manager SDDM, dan Xorg
sudo pkg install -y kde sddm xorg

# 2. Aktifkan D-Bus dan SDDM pada startup
sudo sysrc dbus_enable=YES
sudo sysrc sddm_enable=YES

# 3. Optimasi socket buffer untuk D-Bus/KDE di /etc/sysctl.conf
echo "net.local.stream.recvspace=65536" | sudo tee -a /etc/sysctl.conf
echo "net.local.stream.sendspace=65536" | sudo tee -a /etc/sysctl.conf
```

> **Tips Sesi Login:** Pada layar login SDDM, pilih sesi **Plasma (X11)** untuk stabilitas driver display maksimal pada rilis FreeBSD saat ini.

### 2.4 Pilihan Desktop 2: XFCE (Alternatif Ringan)

Untuk mesin dengan konsumsi memori minimal:

```bash
# 1. Pasang paket XFCE, LightDM greeter, dan Xorg
sudo pkg install -y xfce xfce4-goodies lightdm lightdm-gtk-greeter xorg

# 2. Aktifkan D-Bus dan LightDM pada startup
sudo sysrc dbus_enable=YES
sudo sysrc lightdm_enable=YES
```

---

## 3. Instalasi Dependensi Build

FreeBSD menyediakan Clang dalam sistem dasar (`/usr/bin/cc` dan `/usr/bin/c++`). Pasang CMake, Ninja, pustaka Qt 6, dan utility desktop yang diperlukan melalui `pkg`:

```bash
sudo pkg update
sudo pkg install -y \
    git cmake ninja pkgconf \
    qt6 qt6-base qt6-5compat qt6-tools \
    python3 uchardet \
    libxkbcommon libxcb \
    desktop-file-utils
```

### Ringkasan Fungsi Paket

| Paket FreeBSD | Deskripsi & Fungsi |
| :--- | :--- |
| `qt6` | Metapackage Qt 6 (umbrella package) memastikan seluruh modul dan plugin runtime lengkap |
| `qt6-base` | Pustaka inti: `QtCore`, `QtGui`, `QtWidgets`, `QtNetwork`, `QtPrintSupport`, `QtTest` |
| `qt6-5compat` | Modul `Core5Compat` untuk API utilitas teks dan encoding |
| `qt6-tools` | Utility developer Qt6 (`lupdate`, `lrelease`, `qmake`) |
| `cmake` + `ninja` | Sistem build modern dengan generator paralel berkecepatan tinggi |
| `pkgconf` | Pencarian flag link dan dependensi pustaka C/C++ |
| `uchardet` | Library deteksi charset/encoding dokumen |
| `libxkbcommon` + `libxcb` | Integrasi keyboard layout dan X11 event handler |
| `desktop-file-utils` | Utilitas `update-desktop-database` untuk registrasi menu aplikasi XDG |

---

## 4. Clone Repository & Konfigurasi CMake

Clone source code resmi dari GitHub:

```bash
git clone https://github.com/alsyundawy/SparkNotePad.git
cd SparkNotePad
```

Konfigurasikan CMake dengan path prefix standar FreeBSD (`/usr/local`):

```bash
# 1. Pastikan CMake mencari package di /usr/local
export CMAKE_PREFIX_PATH=/usr/local

# 2. Jalankan konfigurasi dengan generator Ninja
cmake -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=/usr/local \
    -DCMAKE_INSTALL_PREFIX=/usr/local
```

> **Catatan Teknis Prefix:** Di FreeBSD, semua software non-base terpasang di bawah `/usr/local/` (bukan `/usr/`). Menetapkan `-DCMAKE_PREFIX_PATH=/usr/local` dan `-DCMAKE_INSTALL_PREFIX=/usr/local` sangat penting agar binary, file desktop, dan icon terpasang di path standar FreeBSD.

---

## 5. Proses Kompilasi Paralel

> **PENTING: JANGAN LEWATKAN LANGKAH INI!**  
> Anda **wajib** menjalankan proses kompilasi sebelum melakukan instalasi. Jika Anda langsung menjalankan `cmake --install`, proses akan gagal karena binary belum dibangun.

Jalankan build menggunakan seluruh core CPU yang tersedia di FreeBSD:

```bash
cmake --build build -j$(sysctl -n hw.ncpu)
```

Proses kompilasi akan membangun:

1. Pustaka vendor lokal: Scintilla, Lexilla, Lua, dan uchardet
2. Pustaka UI internal: Qt-Advanced-Docking-System (ADS), SingleApplication, QSimpleUpdater
3. Seluruh modul dialog, editor, styling, dan executable utama `SparkNotePad`

### 5.1 Uji Coba Binary Langsung (Lokal)

Setelah selesai mengompilasi, Anda dapat langsung menguji binary sebelum instalasi sistem:

```bash
./build/src/SparkNotePad
```

SparkNotePad akan langsung terbuka di layar desktop FreeBSD Anda.

---

## 6. Instalasi ke Sistem & Integrasi Desktop

Setelah kompilasi sukses, pasang aplikasi ke sistem FreeBSD Anda:

```bash
sudo cmake --install build
```

Perintah di atas akan menempatkan file sesuai standar XDG FreeBSD:

* Executable: `/usr/local/bin/SparkNotePad`
* Desktop Entry: `/usr/local/share/applications/SparkNotePad.desktop`
* Aplikasi Icon: `/usr/local/share/icons/hicolor/scalable/apps/SparkNotePad.svg`
* Mimetype Icon: `/usr/local/share/icons/hicolor/scalable/mimetypes/SparkNotePad.svg`

### 6.1 Refresh Cache Desktop & Icon

Jalankan perintah berikut agar SparkNotePad langsung muncul di menu aplikasi KDE / XFCE tanpa perlu logout/reboot:

```bash
sudo update-desktop-database /usr/local/share/applications
sudo gtk-update-icon-cache -f /usr/local/share/icons/hicolor 2>/dev/null || true
```

---

## 7. Verifikasi & Panduan Troubleshooting

### 7.1 Verifikasi Linker Shared Library

Pastikan semua dependensi dinamis Qt6 dan FreeBSD POSIX threads terhubung dengan benar:

```bash
which SparkNotePad
ldd $(which SparkNotePad) | grep -E 'Qt6|libthr|xcb'
```

Contoh output yang valid:

```text
libQt6Widgets.so.6 => /usr/local/lib/qt6/libQt6Widgets.so.6
libQt6Gui.so.6 => /usr/local/lib/qt6/libQt6Gui.so.6
libQt6Core.so.6 => /usr/local/lib/qt6/libQt6Core.so.6
libQt6Core5Compat.so.6 => /usr/local/lib/qt6/libQt6Core5Compat.so.6
libthr.so.3 => /lib/libthr.so.3
libxcb.so.1 => /usr/local/lib/libxcb.so.1
```

### 7.2 Masalah Umum & Solusi

#### Masalah 1: `file INSTALL cannot find ".../libqtadvanceddocking-qt6_static.a"`

* **Penyebab**: Perintah `cmake --install build` dijalankan sebelum menjalankan `cmake --build build`, ATAU subproject ADS mencoba menginstal arsip statis internal ke sistem.
* **Solusi**:
  1. Pastikan Anda menjalankan `cmake --build build -j$(sysctl -n hw.ncpu)`.
  2. Pastikan file `thirdparty/CMakeLists.txt` menggunakan opsi `EXCLUDE_FROM_ALL YES` pada `CPMAddPackage` untuk mengisolasi instalasi vendor.

#### Masalah 2: Sesi Wayland pada KDE Plasma 6 Menampilkan Layar Hitam / Glitch

* **Penyebab**: Driver GPU tertentu di FreeBSD masih memiliki kendala KMS pada sesi Wayland native.
* **Solusi**: Di layar login SDDM, ubah sesi dari *Plasma (Wayland)* menjadi **Plasma (X11)**.

#### Masalah 3: JavaScript Evaluator Tool Tidak Berjalan

* **Penyebab**: SparkNotePad menyertakan bridge evaluator JavaScript yang menggunakan runtime eksternal di lingkungan Unix.
* **Solusi**: Pasang engine JavaScript favorit Anda di FreeBSD:

  ```bash
  sudo pkg install -y node
  # atau
  sudo pkg install -y quickjs
  ```

#### Masalah 4: Font Interface / Monospace Belum Terpasang

* **Penyebab**: Font modern seperti JetBrains Mono atau Inter belum terpasang di sistem dasar FreeBSD.
* **Solusi**: Pasang font developer pilihan:

  ```bash
  sudo pkg install -y jetbrains-mono inter-fonts dejavu
  fc-cache -f -v
  ```

---

## 8. Ringkasan Perintah Cepat (Cheat Sheet)

Salin dan jalankan urutan perintah berikut pada terminal FreeBSD Anda:

```bash
# 1. Update & pasang dependensi build
sudo pkg update
sudo pkg install -y git cmake ninja pkgconf qt6 qt6-base qt6-5compat qt6-tools python3 uchardet libxkbcommon libxcb desktop-file-utils

# 2. Clone repositori
git clone https://github.com/alsyundawy/SparkNotePad.git
cd SparkNotePad

# 3. Konfigurasi CMake
export CMAKE_PREFIX_PATH=/usr/local
cmake -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=/usr/local \
    -DCMAKE_INSTALL_PREFIX=/usr/local

# 4. Kompilasi (LANGKAH WAJIB)
cmake --build build -j$(sysctl -n hw.ncpu)

# 5. Instalasi ke sistem & registrasi menu desktop
sudo cmake --install build
sudo update-desktop-database /usr/local/share/applications
sudo gtk-update-icon-cache -f /usr/local/share/icons/hicolor 2>/dev/null || true

# 6. Jalankan SparkNotePad
SparkNotePad &
```
