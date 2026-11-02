# Desain Antarmuka, Ergonomi & Koleksi 75 Tema

SparkNotePad menghadirkan estetika visual modern yang dipadukan dengan standar aksesibilitas internasional. Antarmuka aplikasi dirancang untuk memberikan kenyamanan mata maksimal bagi para pengembang perangkat lunak, analis data, maupun penulis teknis.

---

## 1. Koleksi 75 Tema Pengembang (25 Rumpun Tema)

SparkNotePad menyediakan **75 tema bawaan** yang dikelompokkan ke dalam **25 rumpun desain terpopuler**:

| Rumpun Desain        | Varian Tema yang Tersedia                | Karakteristik Visual                                                 |
| :------------------- | :--------------------------------------- | :------------------------------------------------------------------- |
| **Dracula**          | Standard Dark, Pro, Alucard, Soft        | Kontras tinggi dengan aksen ungu dan merah muda neon                 |
| **Nord**             | Polar Night, Frost, Aurora, Snow Storm   | Palet warna dingin Skandinavia bernuansa biru pastel                 |
| **Solarized**        | Dark, Light, Flat, High-Contrast         | Teori warna ilmiah Ethan Schoonover untuk mengurangi kelelahan mata  |
| **Catppuccin**       | Mocha, Macchiato, Frappé, Latte          | Desain pastel menenangkan dengan diferensiasi sintaksis jelas        |
| **Gruvbox**          | Dark Hard/Medium/Soft, Light Hard/Medium | Nuansa retro hangat berbasis palet cokelat tanah dan krem            |
| **One Dark / Light** | Atom One Dark, One Pro, Light Vivid      | Tata warna seimbang berbasis ekosistem Atom dan VS Code              |
| **Monokai**          | Classic, Pro, Octagon, Ristretto         | Tampilan legendaris Sublime Text dengan warna oranye dan hijau cerah |
| **Tokyo Night**      | Night, Storm, Day, Light                 | Estetika visual cyberpunk modern khas kota Tokyo malam hari          |
| **GitHub**           | Dark Dimmed, Dark Default, Light         | Tampilan resmi selaras dengan antarmuka web GitHub                   |

Penggantian tema dapat dilakukan kapan saja melalui menu **Pengaturan (Preferences)** -> **Tema (Themes)** atau menekan pintasan cepat `Cmd+Alt+T` / `Ctrl+Alt+T`.

---

## 2. Kepatuhan Standar Aksesibilitas WCAG 2.1 AA / AAA

Setiap skema warna diuji secara ketat agar memenuhi standar aksesibilitas internasional:

- **Rasio Kontras Teks (Minimal 4.5:1):** Seluruh elemen teks kode, nomor baris, dan penanda kesalahan memiliki rasio kontras terhadap latar belakang di atas standar minimum WCAG 2.1 Tingkat AA.
- **Rasio Kontras Elemen Kontrol UI (Minimal 3.0:1):** Tombol, tab dokumen, scrollbar, dan garis pemisah panel didesain jelas terlihat tanpa menimbulkan kebingungan bagi pengguna dengan gangguan penglihatan warna (_color vision deficiency_).
- **Mode Kontras Tinggi (High-Contrast Mode):** Tersedia opsi kontras tinggi dengan rasio melampaui 7:1 (Tingkat AAA).

---

## 3. Ikon Vektor Monoline SVG Murni (24x24)

Seluruh ikon pada bilah alat (toolbar) dan panel samping digambar ulang menggunakan format vektor **SVG monoline 24x24**:

- **Bebas Artefak Pikselasi:** Ikon tetap tajam secara absolut pada layar High-DPI Retina (faktor skala 200%, 300%, maupun 400%).
- **Pewarnaan Dinamis Sesuai Tema:** Warna garis ikon secara otomatis menyesuaikan dengan palet tema yang aktif, menghindari masalah ikon gelap yang tidak terbaca pada tema berlatar belakang hitam pekat.

---

## 4. Sistem Penataan Panel Lanjutan (Qt-Advanced-Docking-System)

SparkNotePad mengintegrasikan pustaka docking modern (ADS):

- **Pemisahan Fleksibel (Splitting):** Bagi panel proyek, keluaran pencarian, atau pemantauan log secara vertikal maupun horizontal.
- **Pelepasan Menjadi Jendela Mengambang (Floating Windows):** Tarik judul tab panel untuk menjadikannya jendela mandiri yang dapat ditempatkan di layar kedua.
- **Pengelompokan Bertab (Tabbed Grouping):** Satukan beberapa panel utilitas ke dalam satu grup tab hemat ruang.

---

## 5. Rangkaian Paket Ikon Toolbar (10 Paket Vektor & Bitmap), Tombol Plus (+) Tab dan Kustomisasi Tombol

SparkNotePad v1.3.8 menghadirkan arsitektur personalisasi toolbar lengkap terinspirasi oleh Notepad++:

### Paket Ikon Toolbar Bawaan

- **Modern Ribbon (Standar)** (`modernRibbon`): Vektor SVG monoline 20x20 High-DPI dengan kontur halus (Standar).
- **Microsoft Fluent UI (Win 11)** (`fluentUi`): Ikonografi vektor dan bitmap Microsoft Fluent modern.
- **Windows 11 (Fluent 2)** (`windows11`): Microsoft Fluent Design System 2 beresolusi tinggi Directory Opus.
- **Windows 7 (Aero Glass)** (`windows7`): Rangkaian Windows 7/8 Aero Glass Azure otentik dengan gradien kaca sian tembus pandang dan bingkai krom.
- **Windows XP (Luna Classic)** (`windowsXp`): Nostalgia skeuomorfik 3D Windows XP Luna otentik (master 64×64 PNG).
- **Fugue Classic (Silk Suite)** (`fugue`): Ikon klasik Yusuke Kamiyamane Fugue Silk 2 dengan skalabilitas retina @2x.
- **Notepad++ Retro** (`notepadPlusPlus`): Rekreasi vektor High-DPI ikonografi klasik Notepad++.
- **NotepadNext Klasik (Silk)** (`notepadNext`): Ikon bitmap Silk 16x16 autentik dari NotepadNext.
- **Monochrome Outline (Minimalis)** (`monochrome`): Wireframe monoline 1.8px bersih dirancang untuk tema gelap dan terang.
- **Vibrant DuoTone (Cyberpunk)** (`vibrantDuotone`): Kontras dua warna sian elektrik luminansi tinggi & magenta bercahaya.

### Tombol Plus (+) pada Samping Deretan Tab

Setiap title bar area dock kini dilengkapi tombol interaktif **Plus (+)** tepat di samping kanan deretan tab editor. Mengklik tombol ini seketika membuka tab dokumen baru ("New 1", "New 2", dst.).

### Sinkronisasi Ikon Menubar & Menu Klik Kanan Dinamis

Penggantian paket ikon toolbar kini secara otomatis menyinkronkan seluruh ikon aksi pada Toolbar Utama, Menu Bar utama, submenu, menu konteks klik kanan tab, dan menu klik kanan editor secara instan melalui `ToolbarIconManager::applyToWindow()` dan `applyToMenu()`. Ukuran default ikon toolbar adalah 24×24 dan ikon menu adalah 16×16 (dengan aset retina 32×32).

### Skala Ukuran Ikon Toolbar Dinamis

Ubah ukuran ikon secara instan dari `Tema > Paket Ikon Toolbar > Ukuran Ikon Toolbar`:

- **16 × 16** (Kompak / Compact)
- **20 × 20** (Sedang / Medium)
- **24 × 24** (Standar / Disarankan)
- **32 × 32** (Besar / DPI Tinggi)
- **48 × 48** (Ekstra Besar / Extra Large)
- **64 × 64** (Ultra High-DPI / Resolusi Tinggi)

### Dialog Kustomisasi Tombol Toolbar (`CustomizeToolbarDialog`)

- Tambah, hapus, dan atur ulang tombol toolbar dengan dialog interaktif dual-list.
- Dukungan drag-and-drop dengan `InternalMove`.
- Sisipkan garis pemisah kustom.
- Filter pencarian cepat untuk menemukan tindakan.
- Tombol Atur Ulang ke Standar untuk memulihkan susunan 33 tombol bawaan.

### Ergonomi Menu Klik Kanan Editor

- **Relokasi Convert Case**: Diposisikan tepat di bawah `Pilih Semua` (Select All) untuk alur kerja transformasi teks yang intuitif.
- **Tipografi & Ikon Kompak**: Font diperkecil 1–2px (11px) dan ikon disesuaikan ke ukuran 14×14px dengan padding 3px yang ramping.
- **Pemulihan Gaya Otomatis**: Gaya menu bar utama dipulihkan secara otomatis saat menu klik kanan ditutup via `aboutToHide`.

---

## 6. Navigasi Multi-Monitor dan Tata Letak Jendela

Bagi pengguna yang bekerja dengan dua monitor atau lebih, SparkNotePad menyediakan serangkaian tombol pintas khusus untuk manajemen posisi jendela:

| Tindakan                      | macOS Shortcut        | Linux / Windows Shortcut | Deskripsi                                                 |
| :---------------------------- | :-------------------- | :----------------------- | :-------------------------------------------------------- |
| **Pindahkan ke Layar Kanan**  | `Cmd+Alt+Panah Kanan` | `Ctrl+Alt+Panah Kanan`   | Memindahkan jendela ke monitor sebelah kanan              |
| **Pindahkan ke Layar Kiri**   | `Cmd+Alt+Panah Kiri`  | `Ctrl+Alt+Panah Kiri`    | Memindahkan jendela ke monitor sebelah kiri               |
| **Pusatkan Jendela di Layar** | `Cmd+Alt+C`           | `Ctrl+Alt+C`             | Menengahkan jendela di area layar aktif                   |
| **Diagnostik Tampilan Layar** | `Cmd+Alt+D`           | `Ctrl+Alt+D`             | Menampilkan informasi DPI, refresh rate, dan profil warna |

Lanjutkan ke bab berikutnya: [[Enkripsi Dokumen Standar Militer AES-256|04-Enkripsi-Dokumen-AES-256]].
