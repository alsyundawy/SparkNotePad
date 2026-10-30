#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Implements AES-256 authenticated encryption (AES-256-CBC + HMAC-SHA256
 * Encrypt-then-MAC with PBKDF2-HMAC-SHA256 key derivation) following
 * NIST SP 800-38A, NIST SP 800-132, and RFC 8018 standards.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <QByteArray>
#include <QString>
#include <cstdint>

class CryptoManager {
public:
    static constexpr const char *MAGIC_HEADER = "SPKENC01";
    // cppcheck-suppress unusedStructMember
    static constexpr const char *LEGACY_MAGIC_HEADER = "NSPENC01";
    static constexpr size_t MAGIC_HEADER_LEN = 8;
    static constexpr size_t SALT_LEN = 16;
    static constexpr size_t IV_LEN = 16;
    static constexpr size_t MAC_LEN = 32;                                                          // HMAC-SHA256
    static constexpr size_t HEADER_META_LEN = MAGIC_HEADER_LEN + 4 + SALT_LEN + IV_LEN + MAC_LEN;  // 76 bytes
    static constexpr uint32_t DEFAULT_PBKDF2_ITERATIONS = 100000;

    struct CryptoResult {
        bool success = false;
        QByteArray data;
        QString errorMessage;
    };

    enum class PasswordStrength { VeryWeak, Weak, Fair, Good, Strong };

    struct StrengthResult {
        PasswordStrength strength = PasswordStrength::Weak;
        int score = 0;  // 0..100
        QString label;
    };

    // Quick identification
    static bool isEncryptedData(const QByteArray &data);
    static bool isEncryptedFile(const QString &filePath);

    // High-level operations
    static CryptoResult encrypt(const QByteArray &plaintext, const QString &passphrase);
    static CryptoResult decrypt(const QByteArray &ciphertext, const QString &passphrase);

    // Password strength evaluation
    static StrengthResult evaluatePasswordStrength(const QString &passphrase);

    // Constant-time memory comparison for timing attack mitigation
    static bool constantTimeCompare(const uint8_t *a, const uint8_t *b, size_t length);

    // Key derivation: derives 64 bytes (32 bytes AES key + 32 bytes HMAC key)
    static bool deriveKeys(const QString &passphrase, const uint8_t *salt, size_t saltLen, uint32_t iterations,
                           uint8_t *aesKey, uint8_t *hmacKey);

    // Low-level AES-256-CBC with PKCS#7 padding
    static QByteArray aes256CbcEncrypt(const QByteArray &plaintext, const uint8_t *key, const uint8_t *iv);
    static QByteArray aes256CbcDecrypt(const QByteArray &ciphertext, const uint8_t *key, const uint8_t *iv,
                                       bool *ok = nullptr);

    // HMAC-SHA256
    static QByteArray hmacSha256(const QByteArray &data, const uint8_t *key, size_t keyLen);
};
