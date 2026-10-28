# Matriks Dukungan Platform & Sistem Operasi

SparkNotePad dibangun menggunakan standar bahasa **C++20 modern dan kerangka kerja Qt 6 (6.5–6.12)**. Dokumen ini merinci matriks dukungan platform resmi, dasar pustaka biner (_binary baseline_), kompatibilitas lingkungan desktop, serta standar keamanan biner pada macOS, Linux, dan FreeBSD.

---

## 1. Matriks Kompatibilitas Sistem Operasi

| Sistem Operasi             | Versi / Rilis                           | Arsitektur                     | Tingkat Dukungan           | Format Paket Rekomendasi         |
| :------------------------- | :-------------------------------------- | :----------------------------- | :------------------------- | :------------------------------- |
| **macOS**                  | 11 (Big Sur) hingga 26+                 | Universal (`arm64` + `x86_64`) | Tingkat 1 (Rilis Resmi)    | Berkas `.dmg` resmi / Homebrew   |
| **Ubuntu Linux**           | 22.04 LTS (Jammy Jellyfish)             | `x86_64` / `aarch64`           | Tingkat 1 (Acuan Standar)  | Paket native `.deb` (glibc 2.35) |
| **Ubuntu Linux**           | 24.04 LTS (Noble) s.d. 26.04 LTS        | `x86_64` / `aarch64`           | Tingkat 1 (Rilis Resmi)    | Paket native `.deb`              |
| **Debian GNU/Linux**       | 12 (Bookworm) s.d. 14 (Forky)           | `amd64` / `arm64`              | Tingkat 1 (Rilis Resmi)    | Paket native `.deb`              |
| **Fedora Linux**           | 40, 41, 42, 43, 44                      | `x86_64` / `aarch64`           | Tingkat 1 (Rilis Resmi)    | Paket native `.rpm`              |
| **RHEL / Rocky / Alma**    | 9, 10, 11                               | `x86_64` / `aarch64`           | Tingkat 1 (Stasiun Kerja)  | Paket native `.rpm` (via EPEL)   |
| **openSUSE**               | Tumbleweed / Leap 15.6+                 | `x86_64`                       | Tingkat 2 (Didukung Penuh) | Paket RPM / Flatpak              |
| **Arch Linux / Manjaro**   | Rilis Bergulir (_Rolling_)              | `x86_64` / `aarch64`           | Tingkat 2 (Komunitas AUR)  | Repositori AUR / Kompilasi CMake |
| **FreeBSD**                | 13.x, 14.x, 15-CURRENT                  | `amd64` / `aarch64`            | Tingkat 2 (Port Resmi)     | Port FreeBSD / `pkg`             |
| **Linux Warisan (Legacy)** | glibc < 2.35 (CentOS 7/8, Ubuntu 20.04) | `x86_64`                       | Tingkat 3 (Kontainer)      | AppImage / Flatpak               |

---

## 2. Standar Biner & Portabilitas: Acuan glibc 2.35

SparkNotePad mengompilasi paket biner resmi Linux di atas fondasi **Ubuntu 22.04 LTS (glibc 2.35)**. Pendekatan ini memberikan dua keunggulan teknis mendasar:

1. **Kompatibilitas Maju yang Luas (Forward Compatibility):** Program biner Linux yang ditautkan secara dinamis ke glibc bersifat kompatibel maju dengan versi glibc yang lebih baru, namun tidak dapat berjalan pada versi yang lebih lama. Membangun di atas glibc 2.35 memastikan paket biner yang sama dapat berjalan tanpa kendala pada Ubuntu 22.04, 24.04, 26.04, Debian 12, 13, 14, Fedora 40+, dan RHEL 9+.
2. **Ketergantungan Simbolik yang Pasti (SONAME):** Skrip pembuatan paket menyertakan penegakan pustaka sistem spesifik (`libc.so.6(GLIBC_2.35)(64bit)` dan `libxcb-cursor.so.0()(64bit)`), mencegah kesalahan eksekusi saat dijalankan pada sistem yang tidak sesuai.

---

## 3. Integrasi Lingkungan Desktop

SparkNotePad menyatu secara harmonis dengan antarmuka grafis sistem operasi:

### Integrasi macOS

- **Menu Layanan Cocoa (Services):** Terdaftar langsung pada menu klik-kanan sistem (misal: "Buka di SparkNotePad", "Enkripsi dengan SparkNotePad").
- **Sinkronisasi Tema Gelap/Terang:** Berganti tema visual secara otomatis mengikuti preferensi sistem macOS (`NSAppearance`).
- **Dukungan Layar Retina:** Rendering ikon vektor monoline tajam pada rasio piksel tinggi `@2x` dan `@3x`.

### Lingkungan Desktop Linux (X11 & Wayland)

- **KDE Plasma 5 & Plasma 6:** Terintegrasi langsung dengan `kwriteconfig6` dan basis data asosiasi berkas sycoca.
- **GNOME Shell:** Menyesuaikan dengan standar GNOME Client-Side Decoration dan basis data GIO/GVFS MIME.
- **XFCE & MATE:** Pendaftaran menu konteks pengelola berkas Thunar dan Caja melalui standar FreeDesktop.
- **Protokol Wayland:** Berjalan murni pada sesi native Wayland tanpa memerlukan lapisan perantara XWayland.

---

## 4. Persyaratan Perangkat Keras

| Sumber Daya          | Spesifikasi Minimum                      | Rekomendasi Penggunaan                          |
| :------------------- | :--------------------------------------- | :---------------------------------------------- |
| **Prosesor (CPU)**   | 64-bit x86_64 atau Apple Silicon (M1–M4) | Prosesor multi-core 64-bit modern               |
| **Memori (RAM)**     | 256 MB RAM kosong                        | 1 GB+ RAM (untuk berkas log puluhan ribu baris) |
| **Penyimpanan Disk** | 80 MB ruang penyimpanan                  | 200 MB (termasuk cache tema lokal)              |
| **Resolusi Layar**   | 1024x768 piksel                          | 1920x1080 atau layar High-DPI 4K/Retina         |

---

## 5. Penguatan Keamanan Biner (Compiler Hardening Flags)

Setiap biner resmi SparkNotePad dikompilasi dengan parameter keamanan tingkat lanjut:

- **Perlindungan Stack:** `-fstack-protector-strong`
- **Pengerasan Kode Sumber:** `-D_FORTIFY_SOURCE=2` (atau `=3` pada GCC 14+)
- **Executable Bebas Posisi:** `-fPIE` dan `-pie`
- **Tabel Relokasi Hanya-Baca Penuh:** `-Wl,-z,relro,-z,now`
- **Pencegahan Eksekusi Stack:** `-Wl,-z,noexecstack`
- **Apple Hardened Runtime:** Penandatanganan sertifikat dengan pembatasan injeksi kode dinamis.
