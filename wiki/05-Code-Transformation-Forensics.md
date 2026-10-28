# 05. Code Transformation & Forensics

SparkNotePad v1.3.7 provides an extensive Swiss-Army toolkit for formatting, minifying, analyzing, and inspecting source code and security tokens.

---

## 1. Code Formatters & Minifiers (`CodeCompressPretty`)

Access via `Edit > Code Transformation` or the Omnibar (`Cmd+Shift+P` / `Ctrl+Shift+P`):

| Language       | Operations Supported | Implementation Details                                                                                         |
| :------------- | :------------------- | :------------------------------------------------------------------------------------------------------------- |
| **JSON**       | Prettify & Minify    | Full indentation tree reconstruction and whitespace stripping via Qt JSON engine                               |
| **XML / HTML** | Prettify & Minify    | Memory-safe streaming tokenization using `QXmlStreamReader` and `QXmlStreamWriter`                             |
| **SQL**        | Prettify Queries     | Uppercasing SQL reserved keywords (`SELECT`, `FROM`, `WHERE`, `JOIN`, `ORDER BY`, etc.) and clause indentation |
| **CSS**        | Prettify & Minify    | Rule block indentation, selector formatting, and comment cleanup                                               |
| **JavaScript** | Prettify & Minify    | JavaScript code beautification and minification                                                                |

---

## 2. Text Transformation & Conversion Utilities

- **Base64 Encode / Decode**: Instant selection transformation via `Edit > Transform > Base64 Encode` (or Decode).
- **Number Radix Conversion**: Convert highlighted numbers instantly between Decimal, Hexadecimal, Octal, and Binary.
- **Case Converters**:
  - `Uppercase` (`UPPERCASE`)
  - `Lowercase` (`lowercase`)
  - `Title Case` (`Title Case`)
  - `Sentence Case` (`Sentence case`)
  - `Invert Case` (`iNVERT cASE` / `bALIK hURUF`)
- **Insert GUID / UUID & Timestamps**: Insert RFC-compliant UUIDs and ISO 8601 timestamps directly at the caret position.
- **Sorting (`ScintillaSorter`)**: Case-sensitive, case-insensitive, numeric, and reverse line sorting with duplicate line removal.

---

## 3. JWT (JSON Web Token) Inspector & Validator

Analyze web tokens and OAuth bearer credentials with one click:

1. Select any JWT string in the active document (or highlight the entire token `header.payload.signature`).
2. Navigate to `Tools > Inspect JWT Token...` or launch via Omnibar `>Inspect JWT`.
3. SparkNotePad decodes and presents:
   - **Header**: Algorithm (`alg`), Type (`typ`), Key ID (`kid`).
   - **Payload**: Subject (`sub`), Issuer (`iss`), Audience (`aud`), Expiration (`exp`) converted to human-readable UTC timestamp.
   - **Validation Status**: Visual green/red badge indicating whether the token has expired.
   - **Signature**: Formatted cryptographic signature representation.

---

## 4. Document Statistics & Shannon Entropy Dialog

Inspired by Pluma (MATE) and forensics tooling:

- **Launch**: Navigate to `View > Document Statistics & Forensics...`.
- **Metrics Computed**:
  - Total Characters (with and without whitespace)
  - Word Count
  - Line Count
  - Estimated Reading Time (based on standard 200 wpm reading velocity)
- **Shannon Entropy ($\mathcal{H}$)**:
  $$\mathcal{H} = -\sum_{i=0}^{255} P(b_i) \log_2 P(b_i)$$
  - Standard English text / source code: **3.5 – 4.8 bits/byte**
  - Obfuscated scripts / packed base64: **5.5 – 6.5 bits/byte**
  - Encrypted payloads / high-entropy ciphertext: **> 7.5 bits/byte**
- **Copy Button**: One-click markdown summary copy to clipboard.

---

Next Chapter: **[[06. Keyboard Shortcuts Quick Reference|06-Keyboard-Shortcuts]]**
