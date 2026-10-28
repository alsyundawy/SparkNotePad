# 04. Military-Grade Document Encryption

SparkNotePad features built-in, authenticated cryptographic protection for sensitive files, source code credentials, and proprietary text documents.

---

## 1. Cryptographic Architecture & Specification

SparkNotePad uses an authenticated **Encrypt-then-MAC (EtM)** architecture:

| Cryptographic Primitive           | Implementation Detail                                            | Purpose                                                       |
| :-------------------------------- | :--------------------------------------------------------------- | :------------------------------------------------------------ |
| **Cipher Algorithm**              | AES-256-CBC                                                      | Symmetric payload encryption                                  |
| **Key Derivation Function (KDF)** | PBKDF2 with HMAC-SHA256                                          | Derives 256-bit encryption key and 256-bit authentication key |
| **KDF Iteration Count**           | **100,000 iterations**                                           | Defends against brute-force and GPU dictionary attacks        |
| **Salt Generation**               | 128-bit Cryptographically Secure Random Salt                     | Prevents rainbow table attacks                                |
| **Initialization Vector (IV)**    | 128-bit Random IV per file/stream                                | Ensures identical plaintext yields unique ciphertext          |
| **Integrity & Authenticity**      | HMAC-SHA256                                                      | Validates document integrity and detects tampering            |
| **Magic Header Containers**       | `SPKENC01` (SparkNotePad format) & `NSPENC01` (legacy container) | Automatic format identification and version negotiation       |

---

## 2. File Format Container (`SPKENC01`)

Encrypted files are saved with standard binary encapsulation:

```text
+-----------------------+-------------------------+-----------------------+
| Magic Header (8 bytes)| Salt (16 bytes)         | IV (16 bytes)         |
| "SPKENC01"            | Cryptographic Salt     | Random Initial Vector |
+-----------------------+-------------------------+-----------------------+
| Ciphertext Payload (Variable length, PKCS#7 padded)                     |
+-------------------------------------------------+-----------------------+
| HMAC-SHA256 Authentication Tag (32 bytes)                               |
+-------------------------------------------------+-----------------------+
```

When decrypting:

1. SparkNotePad validates the magic header (`SPKENC01` or `NSPENC01`).
2. The user's passphrase is run through PBKDF2 with the extracted salt.
3. The HMAC-SHA256 tag is computed and compared in constant time. If invalid or corrupted, decryption aborts immediately before feeding data to the cipher.
4. Plaintext is decrypted and restored to the Scintilla editor canvas.

---

## 3. How to Encrypt and Decrypt Documents

### Encrypt Active Document

1. Press `Cmd+Alt+E` (macOS) or `Ctrl+Alt+E` (Linux), or navigate to `Tools > Encrypt Active Document...`.
2. Enter your secure passphrase and confirm it.
3. SparkNotePad transforms the active buffer into encrypted `SPKENC01` format.
4. Save the file (commonly with a `.spkenc` or `.enc` extension).

### Decrypt Active Document

1. Open the encrypted file in SparkNotePad.
2. SparkNotePad automatically detects the `SPKENC01` header and opens the decryption prompt (or press `Cmd+Alt+X` / `Ctrl+Alt+X`).
3. Enter the passphrase.
4. The decrypted plaintext is rendered instantly in the editor canvas.

---

## 4. Real-Time Cryptographic Hash Integrity Inspector

SparkNotePad v1.3.6+ includes a native hash calculator:

- **Launch**: Navigate to `Tools > Hash Integrity Inspector...`.
- Computes **SHA-256**, **SHA-512**, and **SHA-3** hashes across the active document or selected text.
- Supports side-by-side comparison with expected verification hashes from software publishers.

---

Next Chapter: **[[05. Code Transformation & Forensics|05-Code-Transformation-Forensics]]**
