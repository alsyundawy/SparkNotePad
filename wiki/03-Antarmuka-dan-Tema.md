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

Penggantian tema dan konfigurasi bilah alat dapat dilakukan kapan saja melalui menu **Pengaturan (Preferences)** pada tab **Themes & Toolbar** (atau pintasan cepat `Cmd+Alt+T` / `Ctrl+Alt+T`), serta penyesuaian tombol bilah alat melalui tab **Custom Toolbar Config** (atau menu `Settings > Customize Toolbar...`).

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

## 5. Rangkaian Paket Ikon Toolbar (69 Paket Ikon Bawaan), Tombol Plus (+) Tab dan Kustomisasi Tombol

SparkNotePad v1.4.6 menghadirkan arsitektur personalisasi toolbar lengkap terinspirasi oleh Notepad++ dengan 69 paket ikon bawaan, dukungan High-DPI (2x Retina), dan adaptasi luminans cerdas untuk Tema Gelap:

### Paket Ikon Toolbar Bawaan (69 Paket)

1. **Modern Ribbon (Standar)** (`modernRibbon`): Vektor SVG monoline 20x20 High-DPI dengan kontur halus (Standar).
2. **Fugue Classic (Silk Suite)** (`fugue`): Ikon klasik Yusuke Kamiyamane Fugue 3.5.6 otentik dengan skalabilitas multi-resolusi dan retina @2x.
3. **Windows 7 (Aero Glass Vector)** (`aero_win7`): Ikonografi vektor Windows 7 Aero Glass.
4. **Windows 7 (Classic Aero Desktop)** (`windows7`): Rangkaian Windows 7/8 Aero Glass Azure otentik dengan gradien kaca sian tembus pandang dan bingkai krom.
5. **Microsoft Fluent Design Regular** (`fluent_regular`): Rangkaian vektor reguler Microsoft Fluent Design.
6. **Microsoft Fluent Design Filled** (`fluent_filled`): Rangkaian vektor tebal Microsoft Fluent Design.
7. **Windows 11 (Fluent 2)** (`windows11`): Microsoft Fluent Design System 2 beresolusi tinggi Directory Opus.
8. **Notepad++ Klasik** (`notepadPlusPlus`): Rekreasi vektor High-DPI ikonografi klasik Notepad++.
9. **Glossy Orange** (`glossy_orange`): Ikon vektor oranye mengkilap skeuomorfik yang cerah.
10. **Windows XP (Luna Classic)** (`windowsXp`): Nostalgia skeuomorfik 3D Windows XP Luna otentik (master 64×64 PNG).
11. **SparkNotepad Klasik (Silk)** (`notepadNext`): Ikon bitmap Silk 16x16 autentik dari SparkNotepad.
12. **Vibrant DuoTone (Cyberpunk)** (`vibrantDuotone`): Kontras dua warna sian elektrik luminansi tinggi & magenta bercahaya.
13. **Monochrome Outline (Minimalis)** (`monochrome`): Wireframe monoline 1.8px bersih dirancang untuk tema gelap dan terang.
14. **Elegant Themes** (`eleganticons`): Ikonografi klasik otentik 48x48 karya Nicholas Roach di semua skala resolusi.
15. **Seven Classic** (`seven`): Rangkaian ikonografi desktop berkilau klasik Windows 7 / Seven di 6 skala multi-DPI (16x16, 20x20, 24x24, 32x32, 48x48, 64x64).
16. **Mignon Icon Theme** (`mignon`): Ikon vektor modern dan elegan karya Igor F. Moraes.
17. **Palette Icon Theme** (`palette`): Tema ikon vektor penuh warna karya mjkim0727.
18. **Diagonal Icon Theme** (`diagonal`): Gaya vektor diagonal dinamis karya Tamás Csábi.
19. **Aksara Icon Pack Rounded** (`aksara-icon-pack-rounded`): Geometri membulat modern yang lembut dan elegan.
20. **Aksara macOS Big Sur** (`aksara-macos-bigsur`): Estetika squircle modern Apple macOS Big Sur & Monterey.
21. **Aksara Windows XP** (`aksara-npp-xp`): Nostalgia ikonografi klasik Windows XP Luna.
22. **Aksara Windows Vista** (`aksara-npp-vista`): Ikonografi glossy transparan Windows Vista.
23. **Aksara Windows 10** (`aksara-npp-win10`): Ikonografi vektor monokromatik datar Windows 10.
24. **Aksara Windows 11** (`aksara-npp-win11`): Ikonografi membulat Fluent 2 modern Windows 11.
25. **Aksara Vista Aero** (`aksara-vista-aero`): Estetika kaca Aero Glass otentik Windows Vista.
26. **Aksara Ubuntu Yaru** (`aksara-ubuntu-yaru`): Ikonografi tema resmi Ubuntu Yaru Desktop.
27. **Aksara Arch Linux** (`aksara-arch-linux`): Ikonografi minimalis biru sian khas Arch Linux.
28. **Aksara Elementary OS** (`aksara-elementary-os`): Ikonografi humanis rapi khas Elementary OS.
29. **Royal Blue** (`royal_blue`): Ikonografi desktop kaca skeuomorfik biru royal aero glossy.
30. **Notepad Pro** (`notepad_pro`): Ikonografi transparan aero bergaya Vista/7 modern dan elegan.
31. **Pastel Modern** (`pastel`): Palet warna pastel minimalis yang hangat dan ramah.
32. **Fluent Modern** (`fluent_modern`): Ikonografi berlapis 3D Microsoft Windows 11 Fluent.
33. **macOS Big Sur Modern** (`macos_bigsur_modern`): Estetika squircle macOS elegan dengan aksen cerah.
34. **Dark Neon Pro** (`dark_neon_pro`): Neon cyberpunk kontras tinggi yang menyala di palet gelap.
35. **macOS Glass** (`macos_glass`): Estetika akrilik aero kaca buram tembus pandang.
36. **Modern Flat & Colorful** (`modern_flat_colorful`): Gaya vektor datar bersih dengan warna cerah kontras tinggi.
37. **Glossy 3D Skeuomorphic** (`glossy_3d_skeuomorphic`): Estetika 3D lempung mengkilap yang taktil dan bervolume.
38. **Retro 8-Bit Pixel Art** (`retro_8bit_pixel`): Ikonografi game arkade retro berpiksel presisi.
39. **Sketchy Hand-Drawn Doodle** (`sketchy_doodle`): Ikonografi doodle artistik goresan tangan yang ekspresif.
40. **macOS Big Sur (2020)** (`01-macos-big-sur`): Squircle putih/translusen 3D lembut dengan elemen skeuomorfik cerah dan bayangan halus.
41. **Windows 11 Fluent (2021)** (`02-windows-11-fluent`): Squircle akrilik gelap dengan lampu latar biru/sian glossy dan refleksi tajam.
42. **Fluent Glass (2024)** (`03-fluent-glass`): Squircle kaca buram dengan transparansi kuat, gradien pastel, dan tepian refraktif.
43. **Dark Neon Pro (2020)** (`04-dark-neon-pro`): Latar obsidian gelap dengan kontur neon ungu, pink, dan sian yang menyala intens.
44. **Modern Colorful (2020)** (`05-modern-colorful`): Squircle 3D lembut cerah dengan warna jenuh yang terinspirasi oleh OS mobile modern.
45. **Metallic Pro (2023)** (`06-metallic-pro`): Squircle logam sikat dan krom dengan sudut bevel 3D perak/abu-abu mesin presisi.
46. **Material Design 3 (2023)** (`07-material-design-3`): Palet warna dinamis dengan bentuk 3D membulat lembut dan elevasi tonal.
47. **Neumorphism (2019)** (`08-neumorphism`): UI lembut dengan permukaan plastik terekstrusi dan bayangan ganda terang/gelap.
48. **Claymorphism (2022)** (`09-claymorphism`): Lempung 3D matte dengan geometri bervolume empuk dan kehangatan taktil yang ramah.
49. **Aurora Gradient (2021)** (`10-aurora-gradient`): Gradien jala bersinar dengan perpaduan 3D halus warna sian, ungu, dan zamrud.
50. **Bento Grid (2023)** (`11-bento-grid`): Warna bersih modular dengan lapisan kartu glossy dan bingkai mikro yang presisi.
51. **Cyber-Y2K (2024)** (`12-cyber-y2k`): Logam cair perak dengan refleksi spekular neon pink dan sian yang energetik.
52. **Glassmorphism Light (2021)** (`13-glassmorphism-light`): Kaca akrilik putih semi-transparan ultra-bersih dengan refraksi prisma warna.
53. **Windows XP (2001)** (`14-windows-xp`): Ikon skeuomorfik 3D otentik Windows XP Luna dengan warna primer cerah dan sudut bevel klasik.
54. **Flat 2.0 Squircle (2022)** (`15-flat-20-squircle`): Estetika vektor datar modern dalam kartu pembatas squircle lembut.
55. **Techno Blueprint (2023)** (`16-techno-blueprint`): Wireframe cetak biru CAD teknik dalam warna sian teknis pada garis kisi grid.
56. **Pastel Dream (2022)** (`17-pastel-dream`): Palet pastel mimpi yang menenangkan dengan bentuk 3D marshmallow empuk.
57. **Vaporwave (2020)** (`18-vaporwave`): Estetika retro 80an/90an vaporwave dengan kisi wireframe dan matahari terbenam magenta.
58. **Neo-Brutalism (2023)** (`19-neo-brutalism`): Garis batas hitam 2px kontras tegas, warna primer berani, dan bayangan jatuh pekat.
59. **Monochrome Steel (2022)** (`20-monochrome-steel`): Baja sikat industri dan abu-abu batu untuk minimalisme yang fokus.
60. **Deep Space (2023)** (`21-deep-space`): Gradien nebula kosmik dengan aksen bintang sian dan violet yang bercahaya.
61. **Liquid Glass (2024)** (`22-liquid-glass`): Bentuk kaca organik cair dengan pantulan spekular dan refraksi dinamis.
62. **Retro Mac OS Classic (1991)** (`23-retro-mac-os-classic`): Ikon nostalgia dither hitam-putih 1-bit & 8-bit System 7 / Platinum klasik.
63. **Holographic (2024)** (`24-holographic`): Refleksi foil warna-warni berpendar dengan pantulan spekular spektrum pelangi.
64. **Earthy Organic (2023)** (`25-earthy-organic`): Nada zaitun botani alami, terakota hangat, dan oker ramah lingkungan.
65. **AI Generative (2024)** (`26-ai-generative`): Ikonografi pendaran saraf kecerdasan buatan dengan gradien kuantum ungu dan sian.
66. **High-Tech Carbon (2023)** (`27-high-tech-carbon`): Tekstur anyaman serat karbon berteknologi tinggi dengan aksen kuning balap.
67. **Synthwave Neon (2021)** (`28-synthwave-neon`): Garis kisi synthwave outrun era 80an dalam warna hot pink dan biru kobalt elektrik.
68. **Fluent Acrylic (2023)** (`29-fluent-acrylic`): Gaya desktop Mika/Akrilik Windows 11 Sun Valley yang translusen elegan.
69. **Swiss Minimal 3D (2024)** (`30-swiss-minimal-3d`): Gaya tipografi internasional Swiss yang diterjemahkan ke dalam bentuk geometris 3D berdisiplin tinggi.

### Kejelasan High-DPI & Layar Retina

Semua paket ikon dilengkapi pixmap multi-resolusi (16x16, 20x20, 24x24, 32x32, 48x48, 64x64) serta rendering otomatis `@2x` Retina DPR (`setDevicePixelRatio(2.0)`). Paket vektor SVG dirender secara native pada koordinat fisik layar tanpa buram atau artefak interpolasi.

### Adaptasi Luminans Tema Gelap (Dark Mode Tone Mapping)

Pada tema gelap, paket ikon outline (seperti `monochrome`) secara otomatis menginversi piksel gelap menjadi silver-white luminous, sedangkan paket berwarna ditingkatkan kecerahannya dengan pemetaan HSL tone mapping tanpa merusak corak warna dan saturasi aslinya.

### Invarian Bilah Alat Mode Gelap Permanen

SparkNotePad menegakkan invarian arsitektur di mana bilah alat utama (`#mainToolBar`, `QToolBar`) secara permanen dirender dalam latar belakang gelap (`#21252b !important`) di seluruh tema (terang, gelap, otomatis) maupun preferensi tampilan OS:

- **Kontras Visual Konsisten**: Paket ikon toolbar pastel dan berwarna mempertahankan visibilitas tajam dan kontras tinggi tanpa risiko memudar pada kanvas putih.
- **Pencegahan Tembus Pandang Jendela Native**: Penggunaan eksplisit `setAutoFillBackground(true)` dan stylesheet QSS prioritas tinggi mencegah latar belakang native Cocoa atau jendela induk menembus bilah alat.
- **Pemisah Gelap Halus**: Batas bawah 1px (`#181a1f`) memisahkan bilah alat secara elegan dari header panel dock dan area kerja Scintilla.

### Tombol Plus (+) pada Samping Deretan Tab

Setiap title bar area dock kini dilengkapi tombol interaktif **Plus (+)** tepat di samping kanan deretan tab editor. Mengklik tombol ini seketika membuka tab dokumen baru ("New 1", "New 2", dst.).

### Sinkronisasi Ikon Menubar & Menu Klik Kanan Dinamis

Penggantian paket ikon toolbar kini secara otomatis menyinkronkan seluruh ikon aksi pada Toolbar Utama, Menu Bar utama, submenu, menu konteks klik kanan tab, dan menu klik kanan editor secara instan melalui `ToolbarIconManager::applyToWindow()` dan `applyToMenu()`. Ukuran default ikon toolbar adalah 24×24 dan ikon menu adalah 16×16 (dengan aset retina 32×32).

### Skala Ukuran Ikon Toolbar Dinamis & Ergonomi Menu

Ubah ukuran ikon secara instan dari menu `Themes > Toolbar Icon Size` (terletak di atas `Toolbar Icon Packs`) atau menu klik kanan toolbar. Kustomisasi tombol toolbar dapat diakses melalui `Tools > Customize Toolbar Buttons...`:

- **16 × 16** (Kompak / Compact)
- **20 × 20** (Sedang / Medium)
- **24 × 24** (Standar / Disarankan)
- **32 × 32** (Besar / DPI Tinggi)
- **48 × 48** (Ekstra Besar / Extra Large)
- **64 × 64** (Ultra High-DPI / Resolusi Tinggi)

### Tab Preferensi Khusus untuk Tema dan Bilah Alat

- **Tab Themes & Toolbar (Tab 3)**:
  - Menggabungkan pengaturan tema aktif, penggeser opasitas jendela, interval muat ulang `tail -f`, dan proteksi penempatan multi-display.
  - Manajer Paket Ikon Toolbar lengkap yang mencakup 69 paket ikon, pemilih paket aktif dengan kartu deskripsi yang responsif dan elegan, pengatur ukuran ikon multi-DPI (16x16 hingga 64x64), bilah pratinjau sampel ikon langsung, pemuat folder paket kustom, dan tombol pemindaian ulang instan.
- **Tab Custom Toolbar Config (Tab 4)**:
  - Mengintegrasikan editor penyesuai bilah alat visual (`CustomizeToolbarWidget`) langsung ke dalam kotak dialog Preferensi.
  - Tambah, hapus, dan atur ulang urutan tombol bilah alat dengan daftar ganda interaktif lengkap dengan filter pencarian instan.
  - Sisipkan garis pemisah kustom dan pulihkan konfigurasi bawaan 33 tombol dengan pembaruan langsung (hot-reload).

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

---

## 7. Pratinjau Langsung Markdown dengan 30 Tema Bawaan

SparkNotePad menyediakan dock pratinjau langsung yang terisolasi dan aman (`Ctrl+Shift+M` / `Cmd+Shift+M`) serta tab pratinjau khusus (`Ctrl+Shift+P` / `Cmd+Shift+P`):

- **30 Tema Classless Kurasi**:
  - `GITHUB`, `GITHUB-DARK`, `ALMOND`, `AWSM`, `AXIST`, `BAMBOO`, `BULLFRAME`, `HOLIDAY`, `KACIT`, `LATEX`, `MARX`, `MINI`, `MODEST`, `NEW`, `NO-CLASS`, `PICO`, `RETRO`, `SAKURA`, `SAKURA-VADER`, `SEMANTIC`, `SIMPLE`, `STYLE-SANS`, `STYLE-SERIF`, `STYLIZE`, `SUPERSTYLIN`, `TACIT`, `VANILLA`, `WATER`, `WATER-DARK`, `WRIT`.
- **Deteksi Format Otomatis Komprehensif**:
  - Markdown: `\.(?:markdown|mdown|mkdn|md|mkd|mdwn|mdtxt|mdtext|text)(?:#.*|\?.*)?$`
  - HTML: `\.(?:html|htm|shtml|xhtml|xht|dhtml)(?:#.*|\?.*)?$`
- **Dukungan Format Lengkap markdown-it**:
  - Tabel GFM, daftar tugas (`- [ ]`, `- [x]`), teks coret (`~~teks~~`), teks sorot (`==teks==`), subskrip (`~teks~`), superskrip (`^teks^`), teks sisipan (`++teks++`), catatan kaki (`[^1]`), diagram Mermaid, dan formula matematika LaTeX (`$$...$$` dan `$rumus$`).
- **Sinkronisasi Scroll Dua Arah**:
  - Sinkronisasi posisi bilah gulir (scrollbar) real-time antara editor teks Scintilla dan tampilan pratinjau.

---

Lanjutkan ke bab berikutnya: [[Enkripsi Dokumen Standar Militer AES-256|04-Enkripsi-Dokumen-AES-256]].
