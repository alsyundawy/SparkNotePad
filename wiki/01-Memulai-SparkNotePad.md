# Panduan Memulai SparkNotePad

Selamat datang di dokumentasi resmi **SparkNotePad**! Panduan ini dirancang untuk membantu Anda mengunduh, memasang, dan mengonfigurasi SparkNotePad pada sistem operasi **macOS**, **Linux**, dan **FreeBSD** secara cepat dan efisien.

---

## 1. Persyaratan Sistem Minimum

Sebelum melakukan instalasi, pastikan sistem komputer Anda memenuhi spesifikasi berikut:

- **Sistem Operasi:**
  - macOS 11.0 (Big Sur) hingga macOS 26+ (Apple Silicon M1–M4 & Intel 64-bit)
  - Distribusi Linux modern 64-bit dengan glibc 2.35+ (Ubuntu 22.04+, Debian 12+, Fedora 40+, Arch Linux)
  - FreeBSD 13.x, 14.x, atau 15-CURRENT (arsitektur `amd64` / `aarch64`)
- **Memori (RAM):** Minimal 256 MB (Direkomendasikan 1 GB+ untuk pengolahan file log berukuran besar)
- **Ruang Disk:** 80 MB untuk file aplikasi biner

---

## 2. Instalasi di macOS

### Menggunakan Paket Biner Resmi (.dmg)

1. Unduh berkas disk image terbaru: `SparkNotePad-v1.3.7-macOS.dmg` dari halaman [GitHub Releases](https://github.com/alsyundawy/SparkNotePad/releases).
2. Klik dua kali pada berkas `.dmg` untuk membukanya.
3. Seret ikon **SparkNotePad** ke dalam folder **Applications** Anda.
4. Buka SparkNotePad melalui Spotlight (`Cmd + Spasi`) atau Launchpad.

> [!NOTE]
> SparkNotePad dikompilasi menggunakan Apple Hardened Runtime. Jika sistem macOS memunculkan pemberitahuan verifikasi pengembang pada build prarilis (prerelease), klik kanan `SparkNotePad.app`, pilih **Buka (Open)**, lalu konfirmasi **Buka**.

---

## 3. Instalasi di Linux

### Ubuntu & Debian (.deb)

SparkNotePad menyediakan paket native `.deb` yang kompatibel dengan Ubuntu 22.04 LTS, Ubuntu 24.04 LTS, Debian 12 (Bookworm), dan Debian 13 (Trixie):

```bash
# Unduh berkas .deb terbaru dari rilis resmi
wget https://github.com/alsyundawy/SparkNotePad/releases/download/v1.3.7/sparknotepad_1.3.7_amd64.deb

# Pasang dengan resolusi dependensi otomatis menggunakan APT
sudo apt update
sudo apt install ./sparknotepad_1.3.7_amd64.deb
```

> [!TIP]
> Perintah `sudo apt install ./file.deb` lebih direkomendasikan dibandingkan `dpkg -i`, karena APT secara otomatis mengunduh pustaka runtime Qt6 dan dependensi terkait dari repositori resmi.

### Fedora, RHEL & AlmaLinux (.rpm)

Untuk distribusi berbasis RPM (Fedora 40+, AlmaLinux 9+, Rocky Linux 9+):

```bash
# Unduh paket .rpm resmi
wget https://github.com/alsyundawy/SparkNotePad/releases/download/v1.3.7/sparknotepad-1.3.7-1.x86_64.rpm

# Pasang menggunakan manajer paket DNF
sudo dnf install ./sparknotepad-1.3.7-1.x86_64.rpm
```

### Eksekusi Portabel: AppImage

Bagi pengguna distribusi Linux lain (seperti openSUSE, Solus, Void) atau sistem tanpa akses root administrator:

```bash
# Berikan izin eksekusi pada berkas AppImage
chmod +x SparkNotePad-1.3.7-x86_64.AppImage

# Jalankan langsung
./SparkNotePad-1.3.7-x86_64.AppImage
```

---

## 4. Instalasi di FreeBSD

Pengguna FreeBSD dapat memasang SparkNotePad langsung melalui sistem ports atau repositori biner `pkg`:

```bash
sudo pkg update
sudo pkg install sparknotepad
```

---

## 5. Menjadikan SparkNotePad Editor Bawaan Sistem (Default Editor)

SparkNotePad menyertakan utilitas konfigurasi otomatis yang mendukung macOS, GNOME, KDE Plasma 5/6, XFCE, dan FreeDesktop MIME:

- **Melalui Menu Aplikasi:** Buka SparkNotePad, pilih menu **Alat (Tools)** -> **Jadikan Editor Bawaan (Set as Default Editor)**.
- **Melalui Terminal (Linux):** Jalankan skrip integrasi:

  ```bash
  /usr/share/sparknotepad/scripts/SparkNotePad-Default-Editor-Linux.sh
  ```

Skrip ini akan mendaftarkan asosiasi jenis berkas teks (`text/plain`, `text/x-csrc`, `application/json`, `text/markdown`, dll.) secara otomatis ke dalam konfigurasi `mimeapps.list` serta cache sistem KDE Plasma.

---

## 6. Lokasi Berkas Konfigurasi

Semua pengaturan preferensi disimpan dalam berkas format standar `.ini`:

- **macOS:** `~/Library/Preferences/SparkNotePad/SparkNotePad.ini`
- **Linux & FreeBSD:** `~/.config/SparkNotePad/SparkNotePad.ini`

Lanjutkan ke bab berikutnya: [[Ikhtisar Fitur (Feature Overview)|02-Ikhtisar-Fitur]].
