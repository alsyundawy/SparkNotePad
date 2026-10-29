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

SparkNotePad v1.3.7 menyatukan utilitas pengembang tingkat lanjut ke dalam menu utama **Tools**:

- **Submenu Hash Kriptografis**: Dukungan algoritma MD5, CRC32, XXHASH, SHA-1, SHA-256, dan SHA-512 dengan aksi Generate (seluruh berkas), Generate Dari Berkas, Generate Dari Teks Terpilih, serta Verifikasi Checksum.
- **Validator Hash & Otentisitas**: Peluncur langsung verifikasi streaming hash berbasis buffer 64 KB.
- **Submenu MIME Tools**: Encode/Decode Base64, URL Percent Encode/Decode, dan Quoted-Printable Encode.
- **Submenu Konverter Format**: Konversi dua arah ASCII <-> Heksadesimal dan Desimal <-> Heksadesimal.
- **Enkripsi Dokumen AES-256**: Enkripsi dokumen berkekuatan militer dengan PBKDF2 (100.000 putaran) kini terorganisasi rapi di menu Tools.

---

Lanjutkan ke bab berikutnya: [[Antarmuka dan Koleksi Tema|03-Antarmuka-dan-Tema]].
