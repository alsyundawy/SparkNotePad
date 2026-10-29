# SparkNotePad — Project Provenance, Architectural Heritage & Evolution

<!-- markdownlint-disable-file MD013 MD022 MD026 MD032 MD033 MD041 -->

**Document Version:** 1.3.7  
**Publication Date:** October 2026  
**Lead Maintainer:** Harry Dertin Sutisna Alsyundawy ([@alsyundawy](https://github.com/alsyundawy))  
**Organization:** ALSYUNDAWY IT SOLUTION ([https://alsyundawy.com](https://alsyundawy.com))  
**Repository:** [https://github.com/alsyundawy/SparkNotePad](https://github.com/alsyundawy/SparkNotePad)

---

## English

### 1. The Genesis & Technical Baseline

The historical and architectural foundation of this repository traces back to the initial source code baseline imported at commit [`4eabd12beec7e0f05017a49e85198dd58362c3f9`](https://github.com/alsyundawy/SparkNotePad/commit/4eabd12beec7e0f05017a49e85198dd58362c3f9).

That foundational baseline imported the cross-platform Qt scaffolding and Scintilla integration concepts originally pioneered by **Justin Dailey ([@dail8859](https://github.com/dail8859))** and the open-source contributors of **NotepadNext**. In its initial experimental development phases, the codebase was maintained and developed under the preliminary project moniker _NoteSciPad_.

### 2. The Evolution into SparkNotePad (v1.3.0+)

Commencing with **version 1.3.0**, the project underwent a formal architectural, conceptual, and aesthetic rebranding, adopting its permanent identity: **SparkNotePad**.

This milestone represented far more than a cosmetic name change; it marked the transition from an experimental build into a fully autonomous, production-hardened developer text editor tailored for modern Linux, FreeBSD, and macOS operating environments. With this transition, the project established its own release cycle, packaging pipelines, multi-Qt matrix, security policies, and power-user tooling.

### 3. Deep Gratitude to Original Contributors

We express our deepest, most sincere gratitude and highest respect to:

- **Justin Dailey ([@dail8859](https://github.com/dail8859))** and the open-source contributors of **NotepadNext**, whose elegant Qt GUI scaffolding, Scintilla integration paradigms, and cross-platform multi-tabbed editor architecture provided the indispensable bedrock upon which this journey commenced.
- **Neil Hodgson & the Scintilla/Lexilla Community**, whose world-class editing engine continues to deliver unbeatable sub-millisecond line-rendering performance.

### 4. Why an Independent Standalone Project Rather Than a Persistent Git Fork?

A deliberate, strategic decision was made to establish SparkNotePad as an independent project rather than maintaining it as a continuous git fork of NotepadNext. The primary architectural, philosophical, and operational rationales are:

1. **Fundamental Architectural Modernization (ISO C++20 & Strict Qt 6)**:
   - Upstream NotepadNext conscientiously maintains backwards compatibility with older compiler toolchains (C++14/C++17) and legacy Qt 5 environments.
   - In contrast, SparkNotePad made an aggressive architectural leap to **ISO C++20** (`std::ranges`, `std::string_view`, concepts, structured bindings) and strictly targets **Qt 6.5 through 6.12 LTS**. Upstream maintainers would understandably reject massive breaking refactors that drop legacy Qt 5 and older compiler support.

2. **Proactive Security Hardening & Zero-Trust Sandbox Isolation**:
   - SparkNotePad integrates stringent defensive security mechanisms designed for high-assurance developer workstations:
     - **Lua Sandbox Hardening (CVE-2026-42214)**: Full stripping of dangerous OS, file system, and debug execution primitives with automated adversarial unit test suites.
     - **ReDoS Mitigation (CVE-2026-4744)**: Pre-execution AST checks detecting catastrophic exponential backtracking in regular expressions.
     - **Config & Session Integrity Guard (CVE-2026-48778 / CVE-2026-48800)**: Protection against shell command injection and multi-user session state tampering.
     - **Sandboxed Markdown Preview (CVE-2026-20841)**: Strict URI scheme whitelisting blocking unauthorized protocol execution.
     - **Cryptographic Document Security**: Native AES-256 document encryption (PBKDF2 100,000 rounds + HMAC-SHA256) and streaming hash verification (SHA-256/512/SHA-3).
   - These rigorous defensive constraints exceed the lightweight scope of traditional text editors and require dedicated architectural oversight.

3. **Substantial Feature Set & Product Vision Divergence**:
   - SparkNotePad is engineered with a specialized power-user feature set that diverges significantly from NotepadNext's minimalist ethos:
     - **Omnibar Command Palette & Quick Open (`Cmd+P` / `Ctrl+P`)**: Keyboard-centric fuzzy search across tabs, recent files, and editor commands inspired by KDE Kate.
     - **Side-by-Side Dual-Editor Visual Diff**: In-memory Myers / LCS comparison engine with synchronized scrolling and visual line diff indicators.
     - **Notepad++ Parity Tools Suite**: 16-action MIME Tools (Base64 pad/unpad/unix/line/strict, Quoted-Printable, URL encoding variants), and Code Prettifier/Forensics (XML, HTML, SQL, JWT decoding, Shannon entropy analysis).
     - **Runtime Extensibility**: Embedded Python 3.13+ scripting engine, sandboxed JavaScript evaluator, and real-time log stream monitoring (`tail -f`).

4. **Dedicated Multi-Platform Distribution Pipeline**:
   - SparkNotePad maintains bespoke native build pipelines for **FreeBSD 13–15 (KDE Plasma 6 & XFCE)**, multi-distro Linux packages (Ubuntu 22.04–26.04 glibc 2.35 baseline, Debian, Fedora, openSUSE DEB/RPM/Flatpak/AppImage), and Universal 2 macOS DMG packaging across five separate Qt version streams.

5. **Clarity of Identity & Upstream Consideration**:
   - Submitting thousands of non-contiguous feature commits and breaking structural overhauls as upstream pull requests would place an undue burden on upstream maintainers.
   - Operating as an independent project provides absolute clarity for bug reports, release lifecycles, and package registries, allowing both projects to pursue their distinct ideals without compromise.

### 5. GPLv3 Compliance & Open Source Legal Analysis

| Legal & Ethical Dimension | GPLv3 Legal Requirement                        | SparkNotePad Status            | Compliance Verdict |
| :------------------------ | :--------------------------------------------- | :----------------------------- | :----------------: |
| **Source Availability**   | Full source code must be freely accessible     | 100% public on GitHub          |   **COMPLIANT**    |
| **Copyleft Continuity**   | Derivatives must remain under GPL-3.0-or-later | Full GPL-3.0 license preserved |   **COMPLIANT**    |
| **Original Copyrights**   | Original author notices must be retained       | Justin Dailey copyright intact |   **COMPLIANT**    |
| **Modification Notices**  | Must prominently state that code was modified  | Fully documented in docs & git |   **COMPLIANT**    |
| **UI Legal Notices**      | Interactive GUI must display license notices   | Implemented in AboutDialog     |   **COMPLIANT**    |
| **Repository Model**      | No requirement to use platform-specific fork   | Standalone repository is valid |   **COMPLIANT**    |
| **Project Rebranding**    | Allowed and ethically protects trademarks      | Rebranded to SparkNotePad      |   **COMPLIANT**    |

---

## Bahasa Indonesia

### 1. Awal Mula & Basis Kode Fondasional

Fondasi teknis dan historis repositori ini berakar dari impor _source code_ awal yang tercatat pada commit [`4eabd12beec7e0f05017a49e85198dd58362c3f9`](https://github.com/alsyundawy/SparkNotePad/commit/4eabd12beec7e0f05017a49e85198dd58362c3f9).

Impor fondasional tersebut menjadi landasan rancangan awal yang diturunkan dari arsitektur Scintilla dan Qt lintas platform yang dirintis oleh **Justin Dailey ([@dail8859](https://github.com/dail8859))** bersama para kontributor _open-source_ **NotepadNext**. Pada fase eksperimental awal, basis kode ini dirawat dan dikembangkan di bawah nama kerja sementara _NoteSciPad_.

### 2. Transformasi Menjadi SparkNotePad (v1.3.0+)

Mulai rilis **versi 1.3.0**, proyek ini secara resmi mengalami _rebranding_ arsitektur dan konseptual dengan menetapkan identitas permanennya: **SparkNotePad**.

Transformasi ini bukan sekadar pergantian nama, melainkan lompatan paradigma menuju editor teks desktop mandiri berstandar industri dengan pengerasan keamanan modern, performa tinggi, dan fitur komprehensif bagi pengguna Linux, FreeBSD, dan macOS. Dengan fase ini, proyek menetapkan siklus rilis mandiri, alur pengemasan (_packaging_), matriks multi-Qt, kebijakan keamanan defensif, serta fitur-fitur _power-user_.

### 3. Rasa Terima Kasih Mendalam kepada Kontributor Asli

Kami menyampaikan apresiasi dan rasa terima kasih yang sebesar-besarnya dengan penuh rasa hormat kepada:

- **Justin Dailey ([@dail8859](https://github.com/dail8859))** dan seluruh kontributor **NotepadNext**, yang telah membangun kerangka GUI Qt yang elegan, integrasi engine Scintilla, serta konsep editor multi-tab lintas platform yang menjadi pijakan awal proyek ini.
- **Neil Hodgson & Komunitas Scintilla/Lexilla**, yang terus menyediakan engine penyuntingan teks kelas dunia dengan kecepatan perenderan sub-milidetik yang tak tertandingi.

### 4. Alasan Memilih Proyek Mandiri Dibandingkan Mempertahankan Fork Git

Keputusan untuk membangun SparkNotePad sebagai proyek mandiri (_independent project_) dan bukan sekadar _fork_ berkelanjutan dari NotepadNext diambil secara sadar dan matang atas dasar pertimbangan arsitektur, filosofi produk, dan kepraktisan:

1. **Modernisasi Arsitektur Mendasar (ISO C++20 & Qt 6 Murni)**:
   - NotepadNext secara bijak mempertahankan kompatibilitas mundur dengan toolchain lawas (C++14/C++17) dan lingkungan Qt 5.
   - Sebaliknya, SparkNotePad melakukan lompatan arsitektural radikal ke standar modern **ISO C++20** (`std::ranges`, `std::string_view`, concepts, structured bindings) dan secara tegas hanya menargetkan **Qt 6.5 hingga 6.12 LTS**. Perubahan masif yang memutus kompatibilitas Qt 5 tentu tidak mungkin digabungkan (_merge_) ke proyek hulu tanpa merusak komitmen kompatibilitas mereka.

2. **Pengerasan Keamanan Tingkat Tinggi (Zero-Trust Sandbox Isolation)**:
   - SparkNotePad mengintegrasikan mekanisme pertahanan siber yang ketat untuk lingkungan pengembang profesional:
     - **Isolasi Sandbox Lua (CVE-2026-42214)**: Penonaktifan total terhadap primitif eksekusi sistem operasi, manipulasi berkas, dan modul debug yang berbahaya, diverifikasi melalui unit test otomatis.
     - **Mitigasi ReDoS (CVE-2026-4744)**: Pendeteksian dini terhadap regular expression berulang bertingkat (_catastrophic backtracking_) sebelum dieksekusi oleh Scintilla.
     - **Penjaga Integritas Konfigurasi & Sesi (CVE-2026-48778 / CVE-2026-48800)**: Perlindungan terhadap injeksi perintah shell eksternal dan manipulasi sesi multi-pengguna.
     - **Pratinjau Markdown Terisolasi (CVE-2026-20841)**: Pembatasan skema URI ketat yang memblokir eksekusi protokol berbahaya.
     - **Keamanan Kriptografi Dokumen**: Enkripsi dokumen native AES-256 (PBKDF2 100.000 iterasi + HMAC-SHA256) serta verifikasi hash data _streaming_ (SHA-256/512/SHA-3).
   - Pendekatan defensif ini berada di luar cakupan editor teks hulu yang berfokus minimalis.

3. **Perbedaan Visi Produk & Fitur Power-User yang Jauh Meluas**:
   - SparkNotePad menghadirkan perkakas pengembang tingkat lanjut yang tidak dirancang untuk editor teks sederhana:
     - **Palet Perintah Omnibar & Quick Open (`Cmd+P` / `Ctrl+P`)**: Navigasi modal terinspirasi dari KDE Kate untuk pencarian tab, berkas terkini, dan eksekusi perintah editor secara cepat.
     - **Mesin Visual Diff Berdampingan**: Perbandingan berkas berbasis algoritma Myers / LCS langsung di memori dengan sinkronisasi gulir dan penanda baris visual.
     - **Paket Alat MIME Lengkap Setara Notepad++**: 16 aksi MIME Tools (Base64 pad/unpad/unix/line/strict, Quoted-Printable, varian enkode URL), pemformat XML/HTML/SQL, serta dekoder JWT dan analisis entropi Shannon.
     - **Ekstensibilitas Runtime Modern**: Integrasi bridge scripting Python 3.13+, runner JavaScript terisolasi, dan pemantauan aliran log seketika (`tail -f`).

4. **Manajemen Rilis & Distribusi Lintas Platform Khusus**:
   - SparkNotePad memiliki alur _packaging_ mandiri untuk **FreeBSD 13–15 (KDE Plasma 6 & XFCE)**, distribusi Linux berbasis glibc 2.35 (Ubuntu 22.04–26.04, Debian, Fedora, openSUSE dalam format DEB/RPM/Flatpak/AppImage), serta Universal 2 DMG macOS di 5 cabang rilis Qt berbeda.

5. **Kejelasan Identitas & Menghormati Proyek Hulu**:
   - Mengajukan ratusan _pull request_ dengan perubahan arsitektural drastis akan membebani _maintainer_ NotepadNext.
   - Dengan berdiri sebagai entitas proyek mandiri, pengguna mendapatkan kejelasan kanal pelaporan _issue_, siklus rilis yang independen, dan kedua proyek dapat berkembang optimal sesuai visi masing-masing tanpa kompromi atau kebingungan identitas.

### 5. Analisis Kepatuhan Lisensi GNU GPLv3

| Dimensi Hukum & Etika        | Persyaratan Hukum GPLv3                     | Status SparkNotePad               |  Kesimpulan  |
| :--------------------------- | :------------------------------------------ | :-------------------------------- | :----------: |
| **Ketersediaan Source Code** | Kode sumber wajib dapat diakses publik      | 100% publik di GitHub             | **MEMATUHI** |
| **Kontinuitas Copyleft**     | Modifikasi wajib tetap berlisensi GPL-3.0   | Lisensi GPL-3.0 tetap utuh        | **MEMATUHI** |
| **Hak Cipta Asli**           | Nama pembuat awal tidak boleh dihapus       | Hak cipta Justin Dailey utuh      | **MEMATUHI** |
| **Catatan Modifikasi**       | Wajib mencatat bahwa kode telah diubah      | Terdokumentasi di docs & git      | **MEMATUHI** |
| **Pemberitahuan UI**         | GUI interaktif wajib memuat lisensi         | Tersedia di AboutDialog           | **MEMATUHI** |
| **Model Repositori**         | Tidak wajib menggunakan fitur fork platform | Repositori mandiri sepenuhnya sah | **MEMATUHI** |
| **Rebranding Proyek**        | Diperbolehkan dan melindungi merek dagang   | Sah berganti ke SparkNotePad      | **MEMATUHI** |
