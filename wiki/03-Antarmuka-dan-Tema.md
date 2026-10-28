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

## 5. Navigasi Multi-Monitor dan Tata Letak Jendela

Bagi pengguna yang bekerja dengan dua monitor atau lebih, SparkNotePad menyediakan serangkaian tombol pintas khusus untuk manajemen posisi jendela:

| Tindakan                      | macOS Shortcut        | Linux / Windows Shortcut | Deskripsi                                                 |
| :---------------------------- | :-------------------- | :----------------------- | :-------------------------------------------------------- |
| **Pindahkan ke Layar Kanan**  | `Cmd+Alt+Panah Kanan` | `Ctrl+Alt+Panah Kanan`   | Memindahkan jendela ke monitor sebelah kanan              |
| **Pindahkan ke Layar Kiri**   | `Cmd+Alt+Panah Kiri`  | `Ctrl+Alt+Panah Kiri`    | Memindahkan jendela ke monitor sebelah kiri               |
| **Pusatkan Jendela di Layar** | `Cmd+Alt+C`           | `Ctrl+Alt+C`             | Menengahkan jendela di area layar aktif                   |
| **Diagnostik Tampilan Layar** | `Cmd+Alt+D`           | `Ctrl+Alt+D`             | Menampilkan informasi DPI, refresh rate, dan profil warna |

Lanjutkan ke bab berikutnya: [[Enkripsi Dokumen Standar Militer AES-256|04-Enkripsi-Dokumen-AES-256]].
