# Enkripsi Dokumen Tingkat Militer (AES-256-CBC)

SparkNotePad dilengkapi modul kriptografi bawaan yang dirancang untuk mengamankan data sensitif, kunci API, catatan rahasia, maupun kode sumber berharga langsung di dalam aplikasi tanpa memerlukan alat enkripsi pihak ketiga eksternal.

---

## 1. Arsitektur Kriptografi Bebas Kebocoran Data (Zero-Leak)

Modul keamanan SparkNotePad dibangun di atas standar kriptografi industri terbuka:

- **Algoritma Cipher:** AES-256-CBC (_Advanced Encryption Standard_ dengan panjang kunci 256-bit dalam mode _Cipher Block Chaining_).
- **Penurunan Kunci (Key Derivation):** PBKDF2 (_Password-Based Key Derivation Function 2_) menggunakan algoritma digest HMAC-SHA256 dengan **100.000 iterasi putaran hashing** serta garam acak kriptografis (_salt_) sepanjang 32 byte.
- **Integritas & Autentikasi Pesan (MAC):** HMAC-SHA256 sepanjang 32 byte untuk mendeteksi setiap upaya manipulasi atau kerusakan berkas biner secara dini sebelum proses dekripsi dijalankan.
- **Pembersihan Memori Aman (Memory Sanitization):** Seluruh variabel memori RAM yang menampung kata sandi dan kunci kriptografi dibersihkan seketika menggunakan fungsi penihilan memori eksplisit (`explicit_bzero` / pengisian nol) sesaat setelah enkripsi atau dekripsi selesai.

---

## 2. Struktur Format Kontainer Berkas `SPKENC01`

Berkas terenkripsi disimpan dalam format kontainer biner berorientasi byte yang terstruktur rapi:

```text
+-------------------+--------------------+--------------------+--------------------+--------------------+
| Magic Identifier  | Salt Kriptografis  | Initialization Vec | Tag HMAC-SHA256    | Data Ciphertext    |
| (8 Byte)          | (32 Byte)          | (IV - 16 Byte)     | (32 Byte)          | (Panjang Variabel) |
| "SPKENC01"        | Acak per-berkas    | Acak per-berkas    | Verifikasi Berkas  | AES-256-CBC PKCS#7 |
+-------------------+--------------------+--------------------+--------------------+--------------------+
```

### Keunggulan Format `SPKENC01`

1. **Pencegahan Serangan Replay & Rainbow Table:** Setiap kali dokumen disimpan ulang, _salt_ dan _IV_ baru dibuat secara independen dari penyedia entropi sistem operasi (`/dev/urandom` atau `SecRandomCopyBytes`).
2. **Validasi Cepat Sebelum Dekripsi:** Tag HMAC-SHA256 diverifikasi terlebih dahulu. Jika kata sandi yang dimasukkan salah atau data berkas korup, proses dekripsi dihentikan sebelum manipulasi plaintext dijalankan, mencegah eksploitasi serangan _padding oracle_.

---

## 3. Cara Mengenkripsi Dokumen

1. Buka dokumen yang ingin Anda amankan di SparkNotePad.
2. Tekan pintasan keyboard `Cmd+Alt+E` (macOS) atau `Ctrl+Alt+E` (Linux/Windows), atau pilih menu **Berkas (File)** -> **Enkripsi Dokumen (Encrypt Document)**.
3. Masukkan kata sandi pilihan Anda pada kotak dialog yang muncul, lalu ulangi pada kolom konfirmasi kata sandi.
4. Klik **Enkripsikan**. Teks pada editor akan diubah secara instan menjadi format aman, dan status enkripsi akan ditandai dengan ikon gembok pada tab dokumen.

---

## 4. Cara Mendekripsi Dokumen

1. Buka berkas berformat `SPKENC01` di SparkNotePad.
2. Aplikasi akan secara otomatis mendeteksi tanda pengenal biner terenkripsi dan menampilkan dialog permintaan kata sandi (atau tekan `Cmd+Alt+X` / `Ctrl+Alt+X`).
3. Masukkan kata sandi Anda dan klik **Dekripsikan**. Dokumen akan dibuka kembali dalam format teks asli (plaintext).

---

## 5. Inspektur Integritas Hash Dokumen (Hash Integrity Inspector)

Untuk memvalidasi integritas data atau mencocokkan checksum berkas unduhan, SparkNotePad menyertakan dialog inspektur hash multi-algoritma:

- **Algoritma yang Didukung:** MD5, SHA-1, SHA-224, SHA-256, SHA-384, SHA-512, Keccak-256, dan Blake2b.
- **Akses Cepat:** Buka menu **Alat (Tools)** -> **Hitung Hash Dokumen (Calculate Document Hashes)**.
- **Pencocokan Nilai Hash:** Masukkan nilai hash dari rilis resmi pada kolom verifikasi; dialog akan menampilkan centang hijau otomatis jika nilai cocok, atau tanda peringatan merah jika terdapat perbedaan karakter.

Lanjutkan ke bab berikutnya: [[Transformasi Kode dan Analisis Forensik|05-Transformasi-Kode-Forensik]].
