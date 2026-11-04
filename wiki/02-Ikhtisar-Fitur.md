# Ikhtisar Fitur SparkNotePad

SparkNotePad dirancang untuk memadukan kecepatan eksekusi editor teks native C++ dengan fleksibilitas dan kenyamanan lingkungan pengembangan modern. Berikut adalah fitur-fitur unggulan yang tersemat di dalamnya.

---

## 1. Mesin Penyunting Tingkat Tinggi: Scintilla 5.6.7 & Lexilla 5.5.4

Jantung penyuntingan SparkNotePad ditenagai oleh mesin Scintilla 5.6.7 dan pemroses sintaksis Lexilla 5.5.4 terbaru:

- **Dukungan Sintaksis Luas:** Pewarnaan sintaksis untuk lebih dari 80 bahasa pemrograman, format konfigurasi, dan markup (C/C++, Rust, Python, Go, TypeScript, JSON, YAML, Markdown, SQL, Shell, HTML/CSS, Lua, dan lainnya).
- **Deteksi Encoding Cerdas:** Mengintegrasikan pustaka `uchardet` untuk mendeteksi secara instan set karakter UTF-8, UTF-16, ISO-8859, Windows-1252, hingga Shift-JIS tanpa risiko teks rusak (_garbled text_).
- **Indikator Garis & Lipatan Kode (Code Folding):** Visualisasi blok fungsi, struktur percabangan, dan penanda indentasi yang memudahkan penjelajahan berkas kode yang panjang.

---

## 2. Navigasi Cepat Dual-Mode: Omnibar & Command Palette

Navigasi dokumen dan eksekusi perintah dapat dilakukan secara instan lewat keyboard:

### Quick Open (Omnibar Pencarian Berkas)

- **Pintasan:** `Cmd+P` (macOS) / `Ctrl+P` (Linux/Windows)
- **Fungsi:** Menampilkan bilah pencarian mengambang (_floating omnibar_) dengan algoritma pencocokan samar (_fuzzy matching_). Anda dapat langsung membuka berkas dalam struktur proyek hanya dengan mengetikkan beberapa karakter dari nama berkas tersebut.

### Command Palette (Peluncur Perintah)

- **Pintasan:** `Cmd+Shift+P` (macOS) / `Ctrl+Shift+P` (Linux/Windows)
- **Fungsi:** Mengakses lebih dari 100 aksi aplikasi, utilitas konversi, format kode, serta konfigurasi tema tanpa harus menjelajahi bilah menu secara manual.

---

## 3. Perbandingan Berkas Visual (Side-by-Side Visual Diff)

SparkNotePad menyertakan dialog visual perbandingan berkas bawaan (`FileDiffDialog`):

- **Penyelarasan Gulir Otomatis:** Kedua panel teks (kiri dan kanan) terkunci secara tersinkronisasi saat digulirkan.
- **Pewarnaan Selisih Baris:**
  - Hijau: Baris yang ditambahkan.
  - Merah: Baris yang dihapus.
  - Kuning/Oranye: Baris yang dimodifikasi.
- **Navigasi Pintas:** Tekan `F7` untuk melompat ke blok perbedaan berikutnya, atau `Shift+F7` untuk kembali ke blok sebelumnya.
- **Deteksi Otomatis:** Membandingkan dokumen yang sedang aktif dengan berkas lain di disk atau riwayat simpanan sebelumnya.

---

## 4. Panel Proyek (Project Tree Dock) dengan Filter Instan

- **Penyematan Fleksibel (ADS):** Panel struktur berkas proyek dapat digeser, disematkan di sisi kiri/kanan/bawah, atau dilepas sebagai jendela mengambang terpisah.
- **Bilah Penyaring Real-Time:** Dilengkapi bilah input filter pencarian dengan dukungan wildcard (`*.cpp`, `*.h`) dan pencarian teks parsial secara instan.
- **Pengawasan Sistem Berkas (File System Watcher):** Struktur direktori secara otomatis diperbarui saat ada penambahan atau penghapusan berkas di luar aplikasi.

---

## 5. Pemantauan Berkas Log Real-Time (Log Tail Monitoring)

- **Pintasan:** `Cmd+Alt+M` (macOS) / `Ctrl+Alt+M` (Linux/Windows)
- **Fungsi:** Memantau berkas log sistem atau keluaran server secara langsung (_live tailing_) menggunakan antarmuka peristiwa sistem berkas native (`FSEvents` di macOS dan `inotify` di Linux).
- **Fitur Gulir Otomatis:** Otomatis menggulirkan kursor ke baris paling akhir saat data log baru ditulis oleh proses lain, tanpa mengunci atau memblokir penyuntingan berkas.

---

## 7. Rangkaian Menu Tools Gaya Notepad++

SparkNotePad v1.3.8 menyatukan utilitas pengembang tingkat lanjut ke dalam menu utama **Tools**:

- **Submenu Hash Kriptografis**: Dukungan algoritma MD5, CRC32, XXHASH, SHA-1, SHA-256, dan SHA-512 dengan aksi Generate (seluruh berkas), Generate Dari Berkas, Generate Dari Teks Terpilih, serta Verifikasi Checksum.
- **Validator Hash & Otentisitas**: Peluncur langsung verifikasi streaming hash berbasis buffer 64 KB.
- **Submenu MIME Tools**: Encode/Decode Base64, URL Percent Encode/Decode, dan Quoted-Printable Encode.
- **Submenu Konverter Format**: Konversi dua arah ASCII <-> Heksadesimal dan Desimal <-> Heksadesimal.
- **Enkripsi Dokumen AES-256**: Enkripsi dokumen berkekuatan militer dengan PBKDF2 (100.000 putaran) kini terorganisasi rapi di menu Tools.

---

## 8. Tombol Plus (+) pada Tab & Pratinjau dalam Tab (v1.3.8)

- **Tombol Plus Bilah Tab**: Tombol `+` terintegrasi di samping bilah tab untuk membuka dokumen baru secara instan dengan satu klik.
- **Pratinjau Markdown & HTML dalam Tab**: Klik kanan tab `.md` atau `.html` menyediakan opsi pratinjau langsung di dalam tab editor dengan gaya resmi GitHub tanpa harus membuka jendela luar. Khusus berkas HTML, tersedia juga opsi buka di peramban standar.
- **Penjelajah Berkas File Commander**: Panel explorer ditingkatkan dengan ergonomi komandan dua panel, rekam jejak breadcrumb, bookmark cepat, dan bilah status metrik.
- **Toggle Tampilan Hex / Binary**: Beralih seketika antara teks biasa dan tampilan dump heksadesimal melalui `Tools > Toggle Hex / Binary View` (diposisikan tepat di bawah Bandingkan / Compare, `Ctrl+Alt+H` / `Cmd+Alt+H`).

---

## 9. Skalabilitas Universal High-DPI, Retina & Multi-Monitor (v1.3.8)

- **Rendering Font Teroptimasi LCD**: Mengaktifkan `SC_EFF_QUALITY_LCD_OPTIMIZED` dan double-buffered drawing (`setBufferedDraw(true)`) demi ketajaman teks maksimal pada layar MacBook Retina (Apple Silicon & Intel), layar 4K/5K/6K eksternal, dan resolusi generik 1366×768 / 1080p.
- **Adaptasi Skala Otomatis**: Menghitung ulang metrik font dan geometri canvas seketika saat terjadi perubahan rasio piksel atau perpindahan layar (`DevicePixelRatioChange` dan `ScreenChangeInternal`).

---

## 10. Koleksi Lengkap 125 Lexer Lexilla 5.5.4

SparkNotePad mengadopsi 125 lexer Lexilla secara lengkap, mendukung bahasa modern dan klasik termasuk AsciiDoc, BibTeX, Bicep, Clojure, Elixir, GDScript, GLSL, HLSL, Mojo, OCaml, Raku, Solidity, SystemVerilog, WebAssembly, dan Zig.

---

## 11. Tab Khusus Preferensi & Ergonomi Menu Desktop (v1.3.8)

- **Tab Themes & Toolbar (Tab 3)**: Menggabungkan konfigurasi tema visual, penggeser transparansi jendela, interval `tail -f`, proteksi penempatan multi-layar, serta manajer 21 paket ikon toolbar dengan kartu deskripsi elegan, ukuran dinamis 16x16 hingga 64x64, pratinjau langsung, dan pemuat paket eksternal.
- **Tab Custom Toolbar Config (Tab 4)**: Editor visual bilah alat terintegrasi langsung dalam Preferensi (`CustomizeToolbarWidget`) lengkap dengan filter pencarian, tambah/hapus aksi, pengurutan tombol, pemisah kustom, dan hot-reload instan.
- **Hierarki Menu Standar Desktop**: Tampilan Hex/Biner di bawah Bandingkan, Mulai Ulang Aplikasi di menu aplikasi macOS tepat di atas Keluar (`QAction::ApplicationSpecificRole`), deduplikasi palet emoji, dan pembersihan menu Tampilan (View).

---

Lanjutkan ke bab berikutnya: [[Antarmuka dan Koleksi Tema|03-Antarmuka-dan-Tema]].
