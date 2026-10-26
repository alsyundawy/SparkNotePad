# Analisis Mendalam & Task List: Kompilasi SparkNotePad di FreeBSD 14.x / 15.1 (KDE Plasma 6 & XFCE)

## 1. Ringkasan Eksekutif & Kronologi Insiden

Dokumen ini menyajikan analisis forensik lengkap terhadap kegagalan instalasi SparkNotePad pada FreeBSD 15.1 (`alsyundawy@alsyundawy-freebsd`), perbandingannya dengan dokumen rujukan `/Users/alsyundawy/Downloads/SparkNotePad_FreeBSD_Tutorial.pdf` (v1.3.5, Oktober 2026), serta task list teknis komprehensif agar kompilasi dan integrasi desktop berjalan 100% sukses.

### Kronologi Perintah yang Dijalankan Pengguna

```bash
alsyundawy@alsyundawy-freebsd:~ $ git clone https://github.com/alsyundawy/SparkNotePad.git
alsyundawy@alsyundawy-freebsd:~ $ cd SparkNotePad
alsyundawy@alsyundawy-freebsd:~/SparkNotePad $ export CMAKE_PREFIX_PATH=/usr/local
alsyundawy@alsyundawy-freebsd:~/SparkNotePad $ cmake -B build -G Ninja \
> -DCMAKE_BUILD_TYPE=Release \
> -DCMAKE_PREFIX_PATH=/usr/local \
> -DCMAKE_INSTALL_PREFIX=/usr/local
... [CMake Berhasil Men-generate Build Files dalam 317.5 detik] ...
-- Build files have been written to: /home/alsyundawy/SparkNotePad/build
alsyundawy@alsyundawy-freebsd:~/SparkNotePad $ sudo cmake --install build
Password:
-- Install configuration: "Release"
-- Installing: /usr/local/include/qtadvanceddocking-qt6/FloatingWidgetTitleBar.h
...
CMake Error at build/_deps/ads-build/src/cmake_install.cmake:78 (file):
  file INSTALL cannot find
  "/home/alsyundawy/SparkNotePad/build/x64/lib/libqtadvanceddocking-qt6_static.a":
  No such file or directory.
```

---

## 2. Bedah Forensik & Identifikasi Root Cause

### 2.1 Root Cause #1 (Penyebab Fatal) — Step Omission (Kompilasi Dilewati)

* **Temuan**:
  Pada dokumen tutorial resmi `SparkNotePad_FreeBSD_Tutorial.pdf` (Halaman 4, Bagian 5 & 6):
  * Bagian 4: `cmake -B build -G Ninja ...` (Konfigurasi CMake)
  * Bagian 5: `cmake --build build -j$(sysctl -n hw.ncpu)` (Proses Kompilasi Source Code)
  * Bagian 6: `sudo cmake --install build` (Proses Instalasi Binary & Asset)
* **Analisis Mekanisme CMake**:
  Perintah `cmake --install <dir>` (sejak CMake 3.15+) **tidak otomatis memicu build tool (Ninja/Make)**. Perintah tersebut hanya menjalankan script `cmake_install.cmake` yang mengasumsikan seluruh target binary (`SparkNotePad`, file `.a`, `.so`) sudah selesai dikompilasi.
  Karena pengguna melompat langsung dari Bagian 4 (`cmake -B build ...`) ke Bagian 6 (`sudo cmake --install build`), **proses kompilasi C++ tidak pernah terjadi**.
  Direktori `build/x64/lib/` masih kosong, sehingga ketika script instalasi mencoba menyalin file `libqtadvanceddocking-qt6_static.a`, CMake melempar error `file INSTALL cannot find: No such file or directory`.

### 2.2 Root Cause #2 — Kebocoran Aturan Instalasi Subproject Vendor (ADS)

* **Temuan**:
  Pustaka `Qt-Advanced-Docking-System` (ADS v5.1.1) diambil melalui CPM di `thirdparty/CMakeLists.txt`.
  Secara default, CMake memasukkan semua target `install()` milik subproject ke dalam instalasi proyek utama jika tidak diisolasi.
  Akibatnya, saat `cmake --install` dijalankan:
  * Header ADS dipaksa diinstal ke `/usr/local/include/qtadvanceddocking-qt6/`
  * Lisensi ADS diinstal ke `/usr/local/share/ads/license/`
  * Arsip statis `libqtadvanceddocking-qt6_static.a` dipaksa diinstal ke `/usr/local/lib/`
* **Dampak**:
  SparkNotePad menggunakan ADS sebagai pustaka internal statis (*in-tree static dependency*). Mengotori `/usr/local/include` dan `/usr/local/lib` dengan komponen internal vendor adalah pelanggaran tata kelola instalasi Unix yang bersih.
* **Solusi Arsitektur**:
  Menambahkan `EXCLUDE_FROM_ALL YES` pada seluruh blok `CPMAddPackage` di `thirdparty/CMakeLists.txt`. Hal ini mengecualikan target subproject dari target instalasi global tanpa memutus link dependency executable `SparkNotePad`.

### 2.3 Warning Forensik — GuiPrivate Target Dependency

* **Temuan**:
  Pada Qt 6.10+ / Qt 6.11 (FreeBSD ports `devel/qt6-base`), modul private (`Qt6::GuiPrivate`) wajib dideklarasikan secara eksplisit dalam daftar komponen `find_package(Qt6 ...)` jika ada target yang me-link ke sana (ADS menggunakan `<qpa/qplatformnativeinterface.h>` untuk kustomisasi judul window X11).
  Karena root `CMakeLists.txt` belum menyertakan `GuiPrivate` atau belum mematikan peringatan internal Qt, Qt melempar peringatan:

  ```text
  The GuiPrivate target is mentioned as a dependency for qtadvanceddocking-qt6, but not declared.
  ```

* **Solusi Arsitektur**:
  Menambahkan `set(QT_NO_PRIVATE_MODULE_WARNING ON)` dan `find_package(Qt6 ... OPTIONAL_COMPONENTS GuiPrivate)` pada root `CMakeLists.txt`.

### 2.4 Warning Forensik — Deteksi Packaging Linux di Sistem FreeBSD

* **Temuan**:
  Log menampilkan: `-- Configuring Linux packaging (.deb, .rpm, AppImage)`.
  Penyebabnya adalah evaluasi `if(UNIX AND NOT APPLE)` pada `cmake/Packaging.cmake`. Pada FreeBSD, `UNIX` bernilai `TRUE` dan `APPLE` bernilai `FALSE`.
* **Solusi Arsitektur**:
  Menambahkan percabangan `if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")` agar sistem FreeBSD menampilkan status konfigurasi FreeBSD desktop dan menonaktifkan target generator `.deb`, `.rpm`, serta `AppImage` yang tidak relevan di lingkungan BSD.

---

## 3. Actionable Task List: Menuju Kompilasi & Instalasi 100% Berhasil

Gunakan checklist berikut saat mengeksekusi kompilasi di mesin FreeBSD Anda:

### Tahap 1 — Konfigurasi Desktop & Akselerasi Grafis (FreeBSD 14/15)

* [ ] **1.1 Pasang Driver Akselerasi GPU (DRM Kernel Module)**

  ```bash
  sudo pkg install -y drm-kmod
  # Untuk Intel GPU:
  sudo sysrc kld_list+="i915kms"
  # Untuk AMD GPU:
  sudo sysrc kld_list+="amdgpu"
  ```

* [ ] **1.2 Konfigurasi Izin User Group**

  ```bash
  sudo pw groupmod video -m $(whoami)
  sudo pw groupmod wheel -m $(whoami)
  ```

* [ ] **1.3 Konfigurasi Filesystem Kompatibilitas di `/etc/fstab`**

  Pastikan `procfs` dan `fdescfs` terpasang (wajib untuk lingkungan desktop Qt6, Chromium, dan D-Bus):

  ```bash
  echo 'proc  /proc   procfs  rw  0 0' | sudo tee -a /etc/fstab
  echo 'fdesc /dev/fd fdescfs rw  0 0' | sudo tee -a /etc/fstab
  sudo mount /proc 2>/dev/null || true
  sudo mount /dev/fd 2>/dev/null || true
  ```

* [ ] **1.4 Pastikan Layanan Display Manager Aktif**

  Untuk **KDE Plasma 6**:

  ```bash
  sudo pkg install -y kde sddm xorg
  sudo sysrc dbus_enable=YES
  sudo sysrc sddm_enable=YES
  ```

  Untuk **XFCE**:

  ```bash
  sudo pkg install -y xfce xfce4-goodies lightdm lightdm-gtk-greeter xorg
  sudo sysrc dbus_enable=YES
  sudo sysrc lightdm_enable=YES
  ```

### Tahap 2 — Instalasi Dependensi Build (Ports / Packages)

* [ ] **2.1 Instal Toolchain C++20, Qt6, dan Utility X11/Desktop**

  ```bash
  sudo pkg install -y git cmake ninja pkgconf \
      qt6-base qt6-5compat qt6-tools \
      uchardet python3 libxkbcommon libxcb desktop-file-utils
  ```

### Tahap 3 — Pembaruan Source Code & CMake Fixes

* [ ] **3.1 Update Source Code Repository**

  ```bash
  cd ~/SparkNotePad
  git pull origin main
  ```

* [ ] **3.2 Verifikasi `thirdparty/CMakeLists.txt`**

  Pastikan `EXCLUDE_FROM_ALL YES` aktif untuk `ads`, `QSimpleUpdater`, `SingleApplication`, dan `editorconfig_core_qt` agar target instalasi subproject tidak mengotori direktori `/usr/local`.

* [ ] **3.3 Verifikasi `CMakeLists.txt`**

  Pastikan `QT_NO_PRIVATE_MODULE_WARNING ON` dan `OPTIONAL_COMPONENTS GuiPrivate` terkonfigurasi.

### Tahap 4 — Konfigurasi CMake

* [ ] **4.1 Bersihkan Direktori Build Lama**

  ```bash
  rm -rf build
  ```

* [ ] **4.2 Set Environment Prefix & Jalankan CMake**

  ```bash
  export CMAKE_PREFIX_PATH=/usr/local
  cmake -B build -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH=/usr/local \
      -DCMAKE_INSTALL_PREFIX=/usr/local
  ```

  *Kriteria Sukses:* Konfigurasi selesai tanpa error, menampilkan `-- Build files have been written to: .../build`.

### Tahap 5 — Kompilasi Paralel (LANGKAH KRUSIAL YANG WAJIB DIJALANKAN)

* [ ] **5.1 Kompilasi Menggunakan Seluruh Core CPU**

  ```bash
  cmake --build build -j$(sysctl -n hw.ncpu)
  ```

  *Kriteria Sukses:* Ninja selesai mengompilasi seluruh target library statis (Scintilla, Lexilla, Lua, uchardet, ADS) dan target binary executable `build/src/SparkNotePad`.

* [ ] **5.2 Uji Coba Eksekusi Binary Lokal**

  ```bash
  ./build/src/SparkNotePad
  ```

  *Kriteria Sukses:* Jendela GUI SparkNotePad terbuka secara native di sesi desktop KDE Plasma 6 atau XFCE Anda.

### Tahap 6 — Instalasi Sistem & Registrasi Desktop

* [ ] **6.1 Eksekusi Instalasi Bersih ke `/usr/local`**

  ```bash
  sudo cmake --install build
  ```

  File yang diinstal hanya:

  * `/usr/local/bin/SparkNotePad`
  * `/usr/local/share/applications/SparkNotePad.desktop`
  * `/usr/local/share/icons/hicolor/scalable/apps/SparkNotePad.svg`
  * `/usr/local/share/icons/hicolor/scalable/mimetypes/SparkNotePad.svg`

* [ ] **6.2 Refresh Cache Aplikasi & Icon XDG Desktop**

  ```bash
  sudo update-desktop-database /usr/local/share/applications
  sudo gtk-update-icon-cache -f /usr/local/share/icons/hicolor 2>/dev/null || true
  ```

### Tahap 7 — Verifikasi Runtime & QA Testing

* [ ] **7.1 Verifikasi Linker Shared Library**

  ```bash
  ldd $(which SparkNotePad) | grep -E 'Qt6|xcb|thr'
  ```

  *Pastikan `libQt6Core.so`, `libQt6Gui.so`, `libQt6Widgets.so`, dan `libthr.so` ter-link dengan benar.*

* [ ] **7.2 Verifikasi Integrasi Desktop Menu**

  Buka menu aplikasi (KDE Application Launcher / XFCE Whisker Menu) -> cari **SparkNotePad** -> klik untuk membuka.

* [ ] **7.3 Jalankan Unit Test (Opsional)**

  ```bash
  ctest --test-dir build --output-on-failure
  ```

---

## 4. Kesimpulan Perbandingan

| Parameter Evaluasi | Eksekusi Awal Pengguna (Error) | Prosedur Perbaikan Terverifikasi |
| :--- | :--- | :--- |
| **Langkah Kompilasi** | Dilewati (langsung `cmake --install`) | `cmake --build build -j$(sysctl -n hw.ncpu)` dijalankan penuh |
| **Status File ADS** | Missing (`libqtadvanceddocking-qt6_static.a`) | Terkompilasi in-tree & diisolasi dengan `EXCLUDE_FROM_ALL YES` |
| **Isolasi Pustaka Sistem** | Mencoba menginstal header/arsip vendor | Bersih: Hanya `SparkNotePad` & asset desktop diinstal ke sistem |
| **Peringatan Qt6 GuiPrivate** | Muncul peringatan unhandled target | Diselesaikan via `OPTIONAL_COMPONENTS GuiPrivate` |
| **Hasil Akhir** | `CMake Error at cmake_install.cmake` | **Kompilasi Sukses 100%, Binary Terpasang di `/usr/local/bin`** |
