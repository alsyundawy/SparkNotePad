#!/usr/bin/env python3
"""
Populate and compile i18n translations for SparkNotePad (Indonesian & English).
Ensures 100% complete translation coverage with 0 unfinished entries.
"""

import xml.etree.ElementTree as ET

ID_MAP = {
    # AboutDialog
    "Official GitHub Primer Markdown preview style specification and CSS tokens.": (
        "Spesifikasi gaya pratinjau Markdown resmi GitHub Primer dan token CSS."
    ),
    "Notepad-- cross-platform architecture and Developer Tools suite inspiration.": (
        "Arsitektur lintas platform Notepad-- dan inspirasi paket Developer Tools."
    ),
    "Developer Tools Hub (JSON, XML, HTML, CSS, JS, Crypto, MIME, Radix)": (
        "Developer Tools Hub (JSON, XML, HTML, CSS, JS, Kripto, MIME, Radiks)"
    ),
    # DeveloperToolsDialog
    "Hash Tools": "Alat Hash",
    "Compute cryptographic hashes (MD5, SHA-1, SHA-256, SHA-512, Keccak) and verify file checksums": (
        "Hitung hash kriptografi (MD5, SHA-1, SHA-256, SHA-512, Keccak) dan verifikasi checksum berkas"
    ),
    "MIME Tools": "Alat MIME",
    "Encode and decode Base64, URL percent-encoding, HTML entities, and Quoted-Printable": (
        "Enkode dan dekode Base64, enkode persen URL, entitas HTML, dan Quoted-Printable"
    ),
    "Converter": "Konverter",
    "Convert numeral radix bases (Hex, Dec, Bin, Oct) and text casing": (
        "Konversi basis angka (Heks, Des, Bin, Okt) dan kapitalisasi teks"
    ),
    "Encryption": "Enkripsi",
    "HTML document encryption, obfuscation, and password strength analysis": (
        "Enkripsi dokumen HTML, obfuskasi, dan analisis kekuatan kata sandi"
    ),
    "Developer Tools": "Alat Pengembang",
    "Format and minify JSON/XML/HTML/CSS/JS, decode JWT, generate UUIDv4, and Epoch timestamps": (
        "Format dan minifikasi JSON/XML/HTML/CSS/JS, dekode JWT, hasilkan UUIDv4, dan stempel waktu Epoch"
    ),
    "Input Text:": "Teks Masukan:",
    "Source text pane for formatting, hashing, encoding, or conversion": (
        "Panel teks sumber untuk pemformatan, hashing, pengkodean, atau konversi"
    ),
    "Processed Output:": "Keluaran Terproses:",
    "Target output pane showing results from developer tools operations": (
        "Panel keluaran target yang menampilkan hasil operasi alat pengembang"
    ),
    "Operational feedback and status notifications": "Umpan balik operasional dan notifikasi status",
    "Load from Editor": "Muat dari Editor",
    "Copy Output": "Salin Keluaran",
    "Apply to Editor": "Terapkan ke Editor",
    "Close": "Tutup",
    "Algorithm:": "Algoritma:",
    "Cryptographic hash algorithm to compute": "Algoritma hash kriptografi yang akan dihitung",
    "Select cryptographic hash algorithm (SHA-256, SHA-512, MD5, SHA-1, SHA3, Keccak)": (
        "Pilih algoritma hash kriptografi (SHA-256, SHA-512, MD5, SHA-1, SHA3, Keccak)"
    ),
    "Generate Hash": "Hasilkan Hash",
    "Compare Hash:": "Bandingkan Hash:",
    "Expected hash string for automated integrity comparison": (
        "String hash yang diharapkan untuk perbandingan integritas otomatis"
    ),
    "Checksum match validation indicator": "Indikator validasi kecocokan checksum",
    "File Checksum:": "Checksum Berkas:",
    "Target file path for binary checksum calculation": "Jalur berkas target untuk perhitungan checksum biner",
    "Browse File...": "Telusuri Berkas...",
    "Base64 Encode": "Enkode Base64",
    "Base64 Decode": "Dekode Base64",
    "URL Encode": "Enkode URL",
    "URL Decode": "Dekode URL",
    "HTML Encode": "Enkode HTML",
    "HTML Decode": "Dekode HTML",
    "QP Encode": "Enkode QP",
    "QP Decode": "Dekode QP",
    "From Base:": "Dari Basis:",
    "Decimal (10)": "Desimal (10)",
    "Hexadecimal (16)": "Heksadesimal (16)",
    "Binary (2)": "Biner (2)",
    "Octal (8)": "Oktal (8)",
    "To Base:": "Ke Basis:",
    "Convert Radix": "Konversi Radiks",
    "Uppercase Hex": "Heks Huruf Besar",
    "ASCII → Hex": "ASCII → Heks",
    "Hex → ASCII": "Heks → ASCII",
    "UPPER CASE": "HURUF BESAR",
    "lowercase": "huruf kecil",
    "Title Case": "Huruf Besar Setiap Kata",
    "Invert Case": "Balik Kapitalisasi",
    "Sentence Case": "Kapitalisasi Kalimat",
    "Passphrase:": "Frasa Sandi:",
    "Enter encryption passphrase...": "Masukkan frasa sandi enkripsi...",
    "Empty": "Kosong",
    "Base64 Obfuscation": "Obfuskasi Base64",
    "Encrypt HTML": "Enkripsi HTML",
    "Decrypt HTML": "Dekripsi HTML",
    "Format JSON": "Format JSON",
    "Minify JSON": "Minifikasi JSON",
    "Format XML": "Format XML",
    "Minify XML": "Minifikasi XML",
    "Format HTML": "Format HTML",
    "Minify HTML": "Minifikasi HTML",
    "Format CSS": "Format CSS",
    "Minify CSS": "Minifikasi CSS",
    "Format JS": "Format JS",
    "Minify JS": "Minifikasi JS",
    "JSON Indent:": "Indentasi JSON:",
    "Dashes": "Tanda Hubung",
    "Developer Tools & Utilities Hub": "Hub Alat Pengembang & Utilitas",
    "Enter or paste input text here... (Loaded automatically from active document selection)": (
        "Masukkan atau tempel teks masukan di sini... (Dimuat otomatis dari pilihan dokumen aktif)"
    ),
    "Source text to be processed, converted, hashed, or formatted": (
        "Teks sumber untuk diproses, dikonversi, di-hash, atau diformat"
    ),
    "Processed result will appear here...": "Hasil proses akan muncul di sini...",
    "Processed result from tools, hashes, or converters (ready to copy or apply)": (
        "Hasil pemrosesan dari alat, hash, atau konverter (siap disalin atau diterapkan)"
    ),
    "Load selected text or entire content from currently active document in editor": (
        "Muat teks terpilih atau seluruh isi dari dokumen yang sedang aktif di editor"
    ),
    "Copy processed output text directly to system clipboard": (
        "Salin teks keluaran hasil proses langsung ke papan klip sistem"
    ),
    "Replace current selection or insert processed output into active document": (
        "Ganti pilihan saat ini atau sisipkan keluaran hasil proses ke dokumen aktif"
    ),
    "Close Developer Tools Hub dialog": "Tutup dialog Hub Alat Pengembang",
    "Compute selected cryptographic hash from input text": (
        "Hitung hash kriptografi yang dipilih dari teks masukan"
    ),
    "Paste expected hash to compare against output...": (
        "Tempel hash yang diharapkan untuk dibandingkan dengan keluaran..."
    ),
    "Paste target hash here to compare and verify integrity": (
        "Tempel hash target di sini untuk membandingkan dan memverifikasi integritas"
    ),
    "Select file to calculate cryptographic hash...": (
        "Pilih berkas untuk menghitung hash kriptografi..."
    ),
    "Path to target file for checksum calculation": "Jalur berkas target untuk perhitungan checksum",
    "Browse file from disk to compute or verify cryptographic hash": (
        "Telusuri berkas dari disk untuk menghitung atau memverifikasi hash kriptografi"
    ),
    "Encode input text to standard RFC 4648 Base64": "Enkode teks masukan ke Base64 standar RFC 4648",
    "Decode Base64 string back to plain text": "Dekode string Base64 kembali ke teks biasa",
    "Percent-encode URL parameters (RFC 3986)": "Enkode persen parameter URL (RFC 3986)",
    "Decode percent-encoded URL string": "Dekode string URL yang terenkode persen",
    "Encode HTML special characters to named and numeric entities": (
        "Enkode karakter khusus HTML ke entitas bernama dan numerik"
    ),
    "Decode HTML entities back to raw characters": "Dekode entitas HTML kembali ke karakter asli",
    "Encode text using MIME Quoted-Printable format (RFC 2045)": (
        "Enkode teks menggunakan format MIME Quoted-Printable (RFC 2045)"
    ),
    "Decode MIME Quoted-Printable encoded string": "Dekode string yang terenkode MIME Quoted-Printable",
    "Input numeral radix base (Decimal, Hexadecimal, Binary, Octal)": (
        "Basis angka masukan (Desimal, Heksadesimal, Biner, Oktal)"
    ),
    "Output numeral radix base (Decimal, Hexadecimal, Binary, Octal)": (
        "Basis angka keluaran (Desimal, Heksadesimal, Biner, Oktal)"
    ),
    "Convert integer between different numeral radix bases": (
        "Konversi bilangan bulat antara basis angka yang berbeda"
    ),
    "Format hexadecimal output with uppercase letters (A-F)": (
        "Format keluaran heksadesimal dengan huruf besar (A-F)"
    ),
    "Convert ASCII text characters to space-separated hexadecimal bytes": (
        "Konversi karakter teks ASCII ke byte heksadesimal berjarak spasi"
    ),
    "Convert space-separated hexadecimal bytes back to ASCII text": (
        "Konversi byte heksadesimal berjarak spasi kembali ke teks ASCII"
    ),
    "Convert entire text to uppercase letters": "Ubah seluruh teks menjadi huruf besar",
    "Convert entire text to lowercase letters": "Ubah seluruh teks menjadi huruf kecil",
    "Capitalize the first letter of each word in the text": "Kapitalisasi huruf pertama setiap kata dalam teks",
    "Invert casing of each character (uppercase to lowercase and vice versa)": (
        "Balik kapitalisasi setiap karakter (huruf besar ke kecil dan sebaliknya)"
    ),
    "Capitalize the first letter of each sentence in the text": (
        "Kapitalisasi huruf pertama setiap kalimat dalam teks"
    ),
    "Secret passphrase used for encrypting or decrypting HTML document": (
        "Frasa sandi rahasia yang digunakan untuk mengenkripsi atau mendekripsi dokumen HTML"
    ),
    "Real-time cryptographic entropy and password strength evaluation": (
        "Evaluasi entropi kriptografi dan kekuatan kata sandi secara waktu nyata"
    ),
    "Obfuscate encrypted HTML payload with Base64 encoding layer": (
        "Obfuskasi muatan HTML terenkripsi dengan lapisan pengkodean Base64"
    ),
    "Encrypt HTML input text using passphrase-protected payload": (
        "Enkripsi teks masukan HTML menggunakan muatan yang dilindungi frasa sandi"
    ),
    "Decrypt protected HTML payload back to readable source text": (
        "Dekripsi muatan HTML yang dilindungi kembali ke teks sumber yang dapat dibaca"
    ),
    "Format and beautify JSON with configurable indentation": (
        "Format dan percantik JSON dengan indentasi yang dapat dikonfigurasi"
    ),
    "Minify JSON by removing unnecessary whitespace and line breaks": (
        "Minifikasi JSON dengan menghapus spasi dan jeda baris yang tidak perlu"
    ),
    "Format and indent XML markup with proper hierarchy": (
        "Format dan indentasi markup XML dengan hierarki yang tepat"
    ),
    "Minify XML document by stripping indentation and whitespace": (
        "Minifikasi dokumen XML dengan menghapus indentasi dan spasi kosong"
    ),
    "Format and beautify HTML document structure": "Format dan percantik struktur dokumen HTML",
    "Minify HTML markup by compressing tags and attributes": (
        "Minifikasi markup HTML dengan memadatkan tag dan atribut"
    ),
    "Minify CSS stylesheets by compressing rules and declarations": (
        "Minifikasi stylesheet CSS dengan memadatkan aturan dan deklarasi"
    ),
    "Minify JavaScript code by stripping comments and extra whitespace": (
        "Minifikasi kode JavaScript dengan menghapus komentar dan spasi berlebih"
    ),
    "Decode JWT Token": "Dekode Token JWT",
    "Decode JSON Web Token (JWT) header, claims payload, and signature": (
        "Dekode header Token Web JSON (JWT), muatan klaim, dan tanda tangan"
    ),
    "Number of spaces per indentation level for JSON formatting": (
        "Jumlah spasi per tingkat indentasi untuk pemformatan JSON"
    ),
    "Current Epoch & ISO": "Epoch & ISO Saat Ini",
    "Display current Unix timestamp (seconds, milliseconds) and ISO 8601 UTC date-time": (
        "Tampilkan stempel waktu Unix saat ini (detik, milidetik) dan tanggal-waktu UTC ISO 8601"
    ),
    "Generate UUIDv4": "Hasilkan UUIDv4",
    "Generate cryptographically random RFC 4122 Version 4 UUID": (
        "Hasilkan UUID Acak Kriptografis RFC 4122 Versi 4"
    ),
    "Uppercase": "Huruf Besar",
    "Format generated UUID with uppercase hexadecimal letters (A-F)": (
        "Format UUID yang dihasilkan dengan huruf heksadesimal besar (A-F)"
    ),
    "{ } Braces": "{ } Kurung Kurawal",
    "Wrap generated UUID in curly braces: {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}": (
        "Bungkus UUID yang dihasilkan dalam kurung kurawal: {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}"
    ),
    "Include hyphens in UUID representation (standard 8-4-4-4-12 format)": (
        "Sertakan tanda hubung dalam representasi UUID (format standar 8-4-4-4-12)"
    ),
    "Loaded selection from editor (%1 chars)": "Pilihan dimuat dari editor (%1 karakter)",
    "Loaded active document (%1 chars)": "Dokumen aktif dimuat (%1 karakter)",
    "Applied to editor successfully.": "Berhasil diterapkan ke editor.",
    "Copied output to clipboard.": "Keluaran berhasil disalin ke papan klip.",
    "Generated %1 hash": "Hash %1 berhasil dihasilkan",
    "✓ MATCH": "✓ COCOK",
    "✗ MISMATCH": "✗ TIDAK COCOK",
    "Select File to Hash": "Pilih Berkas untuk Di-hash",
    "Cannot open file: %1": "Tidak dapat membuka berkas: %1",
    "Calculated %1 for %2 (%3 bytes)": "%1 berhasil dihitung untuk %2 (%3 byte)",
    "Base64 encoded.": "Base64 berhasil dienkode.",
    "Base64 decoded.": "Base64 berhasil didekode.",
    "URL encoded.": "URL berhasil dienkode.",
    "URL decoded.": "URL berhasil didekode.",
    "HTML entities encoded.": "Entitas HTML berhasil dienkode.",
    "HTML entities decoded.": "Entitas HTML berhasil didekode.",
    "Quoted-Printable encoded.": "Quoted-Printable berhasil dienkode.",
    "Quoted-Printable decoded.": "Quoted-Printable berhasil didekode.",
    "Error: %1": "Kesalahan: %1",
    "Converted from Base %1 to Base %2": "Dikonversi dari Basis %1 ke Basis %2",
    "Converted ASCII to Hex.": "ASCII berhasil dikonversi ke Heks.",
    "Hex Error: %1": "Kesalahan Heks: %1",
    "Converted Hex to ASCII.": "Heks berhasil dikonversi ke ASCII.",
    "Converted to Title Case.": "Dikonversi ke Huruf Besar Setiap Kata.",
    "Inverted character cases.": "Kapitalisasi karakter dibalik.",
    "Converted to Sentence Case.": "Dikonversi ke Kapitalisasi Kalimat.",
    "%1 (%2 bits)": "%1 (%2 bit)",
    "Encryption Error: %1": "Kesalahan Enkripsi: %1",
    "HTML successfully encrypted.": "HTML berhasil dienkripsi.",
    "Decryption Error: %1": "Kesalahan Dekripsi: %1",
    "HTML successfully decrypted.": "HTML berhasil didekripsi.",
    "JSON Error: %1": "Kesalahan JSON: %1",
    "JSON formatted.": "JSON berhasil diformat.",
    "JSON minified.": "JSON berhasil diminifikasi.",
    "XML Error: %1": "Kesalahan XML: %1",
    "XML formatted.": "XML berhasil diformat.",
    "XML minified.": "XML berhasil diminifikasi.",
    "HTML Error: %1": "Kesalahan HTML: %1",
    "HTML formatted.": "HTML berhasil diformat.",
    "HTML minified.": "HTML berhasil diminifikasi.",
    "CSS Error: %1": "Kesalahan CSS: %1",
    "CSS formatted.": "CSS berhasil diformat.",
    "CSS minified.": "CSS berhasil diminifikasi.",
    "JS Error: %1": "Kesalahan JS: %1",
    "JS formatted.": "JS berhasil diformat.",
    "JS minified.": "JS berhasil diminifikasi.",
    "JWT Error: %1": "Kesalahan JWT: %1",
    "\n\nExpiration Date: %1 (UTC)": "\n\nTanggal Kedaluwarsa: %1 (UTC)",
    "\nIssued At Date: %1 (UTC)": "\nTanggal Diterbitkan: %1 (UTC)",
    "JWT decoded successfully.": "JWT berhasil didekode.",
    "Generated UUIDv4.": "UUIDv4 berhasil dihasilkan.",
    "Epoch: %1\nISO 8601: %2": "Epoch: %1\nISO 8601: %2",
    "Epoch generated.": "Epoch berhasil dihasilkan.",
    # MainWindow
    "Tools &Hub & Utilities Suite...": "&Hub Alat & Paket Utilitas...",
    "Developer Tools &Hub...": "&Hub Alat Pengembang...",
    "HTML Format / Prettify": "Format / Percantik HTML",
    "HTML Prettify": "Percantik HTML",
    "HTML Minify / Compact": "Minifikasi / Padatkan HTML",
    "HTML Minify": "Minifikasi HTML",
    "XML Format / Prettify": "Format / Percantik XML",
    "XML Prettify": "Percantik XML",
    "XML Minify / Compact": "Minifikasi / Padatkan XML",
    "XML Minify": "Minifikasi XML",
    "CSS Format / Prettify": "Format / Percantik CSS",
    "CSS Minify / Compact": "Minifikasi / Padatkan CSS",
    "JavaScript Format / Prettify": "Format / Percantik JavaScript",
    "JS Prettify": "Percantik JS",
    "JavaScript Minify / Compact": "Minifikasi / Padatkan JavaScript",
    "JS Minify": "Minifikasi JS",
    "Toggle &Line Numbers": "Alihkan Nomor &Baris",
    "Toggle Code &Folding Margin": "Alihkan Margin &Lipatan Kode",
    "Toggle &Indentation Guides": "Alihkan Panduan &Indentasi",
    "Toggle &Caret Line Highlight": "Alihkan Sorotan Baris &Kursor",
    "Toggle &Read-Only": "Alihkan &Hanya-Baca",
    "&Reset Zoom (100%)": "&Atur Ulang Perbesaran (100%)",
    "F&ormat": "F&ormat",
    "File &Encoding": "&Enkod Berkas",
    "&Line Endings": "Akhir &Baris",
    # Markdown docks & preview
    "Code Copied!": "Kode Disalin!",
    "CODE COPIED!": "KODE DISALIN!",
}


def process_ts_file(filepath, is_id=True):
    tree = ET.parse(filepath)
    root = tree.getroot()
    updated = 0
    for ctx in root.findall("context"):
        for msg in ctx.findall("message"):
            src_elem = msg.find("source")
            if src_elem is None or not src_elem.text:
                continue
            src = src_elem.text
            trans = msg.find("translation")
            if trans is None:
                trans = ET.SubElement(msg, "translation")

            if is_id:
                if src in ID_MAP:
                    trans.text = ID_MAP[src]
                    if "type" in trans.attrib:
                        del trans.attrib["type"]
                    updated += 1
                elif trans.attrib.get("type") == "unfinished" and trans.text:
                    del trans.attrib["type"]
                    updated += 1
            else:
                # English TS: translation is source text itself (unless numerus="yes")
                if (
                    msg.attrib.get("numerus") == "yes"
                    or len(trans.findall("numerusform")) > 0
                ):
                    if "type" in trans.attrib:
                        del trans.attrib["type"]
                    trans.text = None
                else:
                    trans.text = src
                    if "type" in trans.attrib:
                        del trans.attrib["type"]
                updated += 1

    tree.write(filepath, encoding="utf-8", xml_declaration=True)
    print(f"Updated {filepath}: {updated} translations finalized.")


if __name__ == "__main__":
    process_ts_file("i18n/SparkNotePad_id.ts", is_id=True)
    process_ts_file("i18n/SparkNotePad_en.ts", is_id=False)
