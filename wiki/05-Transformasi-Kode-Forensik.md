# Transformasi Kode, Utilitas Forensik & Evaluasi JavaScript

SparkNotePad dilengkapi beragam modul bawaan untuk memudahkan pengembang perangkat lunak, peneliti keamanan siber (_security researchers_), dan administrator sistem dalam melakukan pemeriksaan data serta manipulasi struktur teks tanpa bergantung pada situs web pihak ketiga yang berisiko membocorkan informasi privat.

---

## 1. Pemformat Kode Sumber Bawaan (Code Formatters & Minifiers)

Dapat diakses langsung melalui menu **Alat (Tools)** -> **Pemformat Kode (Formatters)**:

- **JSON Beautifier & Minifier:** Merapikan struktur JSON dengan indentasi yang dapat dikonfigurasi (2 spasi, 4 spasi, atau tab), serta mode minifikasi untuk memadatkan berkas konfigurasi ke dalam satu baris.
- **XML / HTML Formatter:** Meluruskan penutupan tag yang bertingkat secara otomatis.
- **CSS / SASS Formatter:** Mengatur blok selektor, deklarasi properti, dan kurung kurawal agar mudah dibaca.
- **SQL Query Beautifier:** Menyeragamkan penulisan klausa SQL utama (`SELECT`, `FROM`, `WHERE`, `JOIN`, `GROUP BY`, `ORDER BY`) ke dalam huruf kapital dengan indentasi logika yang rapi.
- **JavaScript & TypeScript Formatter:** Merapikan deklarasi fungsi, objek, dan pemanggilan pustaka secara instan.

---

## 2. Utilitas Enkoding, Dekoding & Konversi Bilangan (Radix Converters)

Bekerja dengan format pertukaran data menjadi sangat mudah lewat menu **Ubah (Edit)** -> **Konversi (Transformations)**:

- **Base64 Encode & Decode:** Mendukung format standar RFC 4648 maupun format URL-safe tanpa karakter padding (`=`).
- **URL / URI Percent-Encoding:** Mengubah karakter khusus menjadi representasi persen (`%20`, `%2F`, dll.) dan sebaliknya.
- **HTML Entity Encoder / Decoder:** Mengonversi entitas XML/HTML (`&lt;`, `&gt;`, `&quot;`, `&amp;`).
- **Konversi Basis Bilangan (Radix Converter):** Mengonversi angka atau rangkaian biner antara format Desimal, Heksadesimal (`0x...`), Biner (`0b...`), dan Oktal (`0o...`) secara simultan.

---

## 3. Inspektur & Validator Forensik Token JWT

Inspektur JSON Web Token (JWT) memungkinkan pemeriksaan langsung struktur payload otentikasi:

- **Dekode Tiga Segmen:** Memecah token JWT menjadi **Header** (algoritma dan tipe token), **Payload** (klaim pengguna), dan **Signature** (tanda tangan digital) dalam panel berwarna yang jelas.
- **Validasi Waktu Kedaluwarsa Otomatis:** Mengonversi cap waktu UNIX `exp` (_Expires At_), `iat` (_Issued At_), dan `nbf` (_Not Before_) ke format tanggal dan jam lokal pengguna. Jika token telah kedaluwarsa, penanda peringatan visual akan menyala.
- **Keamanan Absolut:** Seluruh proses evaluasi berlangsung 100% secara lokal pada memori komputer Anda; tidak ada payload atau kunci rahasia yang dikirimkan ke jaringan internet.

---

## 4. Statistik Dokumen & Analisis Entropi Shannon (Shannon Entropy)

Melalui dialog **Statistik Dokumen (Document Statistics)**:

- **Metrik Dasar:** Menghitung jumlah total karakter, karakter tanpa spasi, kata, baris fisik, baris logis, serta ukuran dalam byte.
- **Perhitungan Entropi Shannon (0.0 s.d. 8.0 bit/byte):**
  - Entropi rendah (1.0–3.5): Teks polos bahasa alami atau kode dengan banyak perulangan pola.
  - Entropi menengah (4.0–5.5): Kode sumber terstruktur rapi (C++, JSON, XML).
  - Entropi tinggi (7.0–8.0): Menandakan data terkompresi (gzip/zip), berkas terenkripsi kuat, atau skrip kode yang telah disamarkan (_obfuscated payload / malware shellcode_).

---

## 5. Evaluator JavaScript & Matematika Tersemat (QJSEngine)

- **Pintasan:** `Cmd+Alt+J` (macOS) / `Ctrl+Alt+J` (Linux/Windows)
- **Fungsi:** Mengevaluasi ekspresi matematika atau kode JavaScript singkat langsung pada baris teks yang sedang aktif atau teks yang disorot, lalu menampilkan hasilnya di samping kursor atau mengganti teks tersebut.
- **Contoh Penggunaan:**
  - Perhitungan matematika: `Math.sin(Math.PI / 4) * 100` -> `70.7106`
  - Manipulasi string: `"hello world".toUpperCase()` -> `HELLO WORLD`
  - Operasi tanggal: `new Date().toISOString()` -> `2026-10-28T12:00:00.000Z`
- **Keamanan Eksekusi:** Berjalan di dalam lingkungan terisolasi (_sandboxed_) QJSEngine tanpa akses ke sistem berkas atau perintah shell sistem, menjamin keamanan eksekusi dari bahaya injeksi skrip.

Lanjutkan ke bab berikutnya: [[Daftar Lengkap Pintasan Keyboard|06-Daftar-Pintasan-Keyboard]].
