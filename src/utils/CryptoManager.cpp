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

#include "CryptoManager.h"

#include <QFile>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QtEndian>
#include <vector>
#include <cstring>
#include <algorithm>

// Portable secure memory zeroing: prevents compiler optimization from eliding the memset
// (equivalent to explicit_bzero on Linux, SecureZeroMemory on Windows).
// Uses volatile pointer indirection which compilers must not eliminate.
static void secureZeroMemory(void *ptr, size_t len) noexcept {
    volatile uint8_t *p = static_cast<volatile uint8_t *>(ptr);
    for (size_t i = 0; i < len; ++i) {
        p[i] = 0;
    }
}

#if defined(Q_OS_MACOS)
#include <CommonCrypto/CommonCryptor.h>
#include <CommonCrypto/CommonKeyDerivation.h>
#include <CommonCrypto/CommonHMAC.h>
#endif

// --- Portable Standard AES-256 implementation (FIPS-197) ---
namespace PortableAes {

static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9,
    0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f,
    0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07,
    0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
    0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58,
    0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3,
    0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec, 0x5f,
    0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac,
    0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a,
    0xae, 0x08, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70,
    0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
    0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42,
    0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

static const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39,
    0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2,
    0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76,
    0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc,
    0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d,
    0x84, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c,
    0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41, 0x4f,
    0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
    0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62,
    0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd,
    0x5a, 0xf4, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 0x60,
    0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d,
    0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6,
    0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

static const uint8_t rcon[15] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
                                 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d};

static inline uint8_t xtime(uint8_t x) {
    return (x << 1) ^ (((x >> 7) & 1) * 0x1b);
}

static inline uint8_t multiply(uint8_t x, uint8_t y) {
    return (((y & 1) * x) ^ ((y >> 1 & 1) * xtime(x)) ^ ((y >> 2 & 1) * xtime(xtime(x))) ^
            ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^ ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

static void keyExpansion256(const uint8_t *key, uint8_t *roundKeys) {
    std::memcpy(roundKeys, key, 32);
    uint8_t temp[4];
    int bytesGenerated = 32;
    int rconIteration = 1;

    while (bytesGenerated < 240) {
        for (int i = 0; i < 4; ++i)
            temp[i] = roundKeys[bytesGenerated - 4 + i];

        if (bytesGenerated % 32 == 0) {
            uint8_t t = temp[0];
            temp[0] = sbox[temp[1]] ^ rcon[rconIteration++];
            temp[1] = sbox[temp[2]];
            temp[2] = sbox[temp[3]];
            temp[3] = sbox[t];
        } else if (bytesGenerated % 32 == 16) {
            for (int i = 0; i < 4; ++i)
                temp[i] = sbox[temp[i]];
        }

        for (int i = 0; i < 4; ++i) {
            roundKeys[bytesGenerated] = roundKeys[bytesGenerated - 32] ^ temp[i];
            bytesGenerated++;
        }
    }
    secureZeroMemory(temp, sizeof(temp));
}

static void encryptBlock(const uint8_t *in, uint8_t *out, const uint8_t *roundKeys) {
    uint8_t state[16];
    std::memcpy(state, in, 16);

    for (int i = 0; i < 16; ++i)
        state[i] ^= roundKeys[i];

    for (int round = 1; round < 14; ++round) {
        for (int i = 0; i < 16; ++i)
            state[i] = sbox[state[i]];

        uint8_t temp = state[1];
        state[1] = state[5];
        state[5] = state[9];
        state[9] = state[13];
        state[13] = temp;
        temp = state[2];
        state[2] = state[10];
        state[10] = temp;
        temp = state[6];
        state[6] = state[14];
        state[14] = temp;
        temp = state[3];
        state[3] = state[15];
        state[15] = state[11];
        state[11] = state[7];
        state[7] = temp;

        for (int i = 0; i < 4; ++i) {
            int col = i * 4;
            uint8_t a = state[col], b = state[col + 1], c = state[col + 2], d = state[col + 3];
            uint8_t e = a ^ b ^ c ^ d;
            state[col] ^= e ^ xtime(a ^ b);
            state[col + 1] ^= e ^ xtime(b ^ c);
            state[col + 2] ^= e ^ xtime(c ^ d);
            state[col + 3] ^= e ^ xtime(d ^ a);
        }

        const uint8_t *rk = roundKeys + round * 16;
        for (int i = 0; i < 16; ++i)
            state[i] ^= rk[i];
    }

    for (int i = 0; i < 16; ++i)
        state[i] = sbox[state[i]];
    uint8_t temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;
    temp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = temp;

    const uint8_t *rk = roundKeys + 14 * 16;
    for (int i = 0; i < 16; ++i)
        out[i] = state[i] ^ rk[i];
}

static void decryptBlock(const uint8_t *in, uint8_t *out, const uint8_t *roundKeys) {
    uint8_t state[16];
    std::memcpy(state, in, 16);

    const uint8_t *rk = roundKeys + 14 * 16;
    for (int i = 0; i < 16; ++i)
        state[i] ^= rk[i];

    for (int round = 13; round > 0; --round) {
        uint8_t temp = state[13];
        state[13] = state[9];
        state[9] = state[5];
        state[5] = state[1];
        state[1] = temp;
        temp = state[2];
        state[2] = state[10];
        state[10] = temp;
        temp = state[6];
        state[6] = state[14];
        state[14] = temp;
        temp = state[7];
        state[7] = state[11];
        state[11] = state[15];
        state[15] = state[3];
        state[3] = temp;

        for (int i = 0; i < 16; ++i)
            state[i] = rsbox[state[i]];

        rk = roundKeys + round * 16;
        for (int i = 0; i < 16; ++i)
            state[i] ^= rk[i];

        for (int i = 0; i < 4; ++i) {
            int col = i * 4;
            uint8_t a = state[col], b = state[col + 1], c = state[col + 2], d = state[col + 3];
            state[col] = multiply(a, 0x0e) ^ multiply(b, 0x0b) ^ multiply(c, 0x0d) ^ multiply(d, 0x09);
            state[col + 1] = multiply(a, 0x09) ^ multiply(b, 0x0e) ^ multiply(c, 0x0b) ^ multiply(d, 0x0d);
            state[col + 2] = multiply(a, 0x0d) ^ multiply(b, 0x09) ^ multiply(c, 0x0e) ^ multiply(d, 0x0b);
            state[col + 3] = multiply(a, 0x0b) ^ multiply(b, 0x0d) ^ multiply(c, 0x09) ^ multiply(d, 0x0e);
        }
    }

    uint8_t temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;
    temp = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = state[3];
    state[3] = temp;

    for (int i = 0; i < 16; ++i)
        state[i] = rsbox[state[i]];

    rk = roundKeys;
    for (int i = 0; i < 16; ++i)
        out[i] = state[i] ^ rk[i];
}

}  // namespace PortableAes

// --- CryptoManager Implementation ---

bool CryptoManager::isEncryptedData(const QByteArray &data) {
    return data.size() >= static_cast<int>(MAGIC_HEADER_LEN) &&
           (data.startsWith(MAGIC_HEADER) || data.startsWith(LEGACY_MAGIC_HEADER));
}

bool CryptoManager::isEncryptedFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    const QByteArray header = file.read(MAGIC_HEADER_LEN);
    file.close();
    return isEncryptedData(header);
}

bool CryptoManager::constantTimeCompare(const uint8_t *a, const uint8_t *b, size_t length) {
    uint8_t diff = 0;
    for (size_t i = 0; i < length; ++i) {
        diff |= (a[i] ^ b[i]);
    }
    return diff == 0;
}

QByteArray CryptoManager::hmacSha256(const QByteArray &data, const uint8_t *key, size_t keyLen) {
    return QMessageAuthenticationCode::hash(
        data, QByteArray(reinterpret_cast<const char *>(key), static_cast<int>(keyLen)), QCryptographicHash::Sha256);
}

bool CryptoManager::deriveKeys(const QString &passphrase, const uint8_t *salt, size_t saltLen, uint32_t iterations,
                               uint8_t *aesKey, uint8_t *hmacKey) {
    if (passphrase.isEmpty() || !salt || saltLen == 0 || iterations == 0 || !aesKey || !hmacKey) {
        return false;
    }

    const QByteArray passBytes = passphrase.toUtf8();
    uint8_t derived[64];

#if defined(Q_OS_MACOS)
    int res = CCKeyDerivationPBKDF(kCCPBKDF2, passBytes.constData(), passBytes.size(), salt, saltLen,
                                   kCCPRFHmacAlgSHA256, iterations, derived, sizeof(derived));
    if (res != kCCSuccess) {
        return false;
    }
#else
    // Standard RFC 8018 PBKDF2-HMAC-SHA256 using QMessageAuthenticationCode
    uint32_t blockCount = (sizeof(derived) + 31) / 32;
    std::vector<uint8_t> u(32);
    std::vector<uint8_t> uPrev(32);
    QByteArray saltBlock(reinterpret_cast<const char *>(salt), static_cast<int>(saltLen));
    saltBlock.resize(static_cast<int>(saltLen + 4));

    size_t offset = 0;
    for (uint32_t block = 1; block <= blockCount; ++block) {
        saltBlock[static_cast<int>(saltLen + 0)] = static_cast<char>((block >> 24) & 0xff);
        saltBlock[static_cast<int>(saltLen + 1)] = static_cast<char>((block >> 16) & 0xff);
        saltBlock[static_cast<int>(saltLen + 2)] = static_cast<char>((block >> 8) & 0xff);
        saltBlock[static_cast<int>(saltLen + 3)] = static_cast<char>(block & 0xff);

        QByteArray h = QMessageAuthenticationCode::hash(saltBlock, passBytes, QCryptographicHash::Sha256);
        std::memcpy(uPrev.data(), h.constData(), 32);
        std::memcpy(u.data(), h.constData(), 32);

        for (uint32_t iter = 1; iter < iterations; ++iter) {
            h = QMessageAuthenticationCode::hash(QByteArray(reinterpret_cast<const char *>(uPrev.data()), 32),
                                                 passBytes, QCryptographicHash::Sha256);
            std::memcpy(uPrev.data(), h.constData(), 32);
            for (size_t k = 0; k < 32; ++k) {
                u[k] ^= uPrev[k];
            }
        }

        size_t toCopy = std::min(static_cast<size_t>(32), sizeof(derived) - offset);
        std::memcpy(derived + offset, u.data(), toCopy);
        offset += toCopy;
    }
    secureZeroMemory(u.data(), u.size());
    secureZeroMemory(uPrev.data(), uPrev.size());
#endif

    std::memcpy(aesKey, derived, 32);
    std::memcpy(hmacKey, derived + 32, 32);
    secureZeroMemory(derived, sizeof(derived));  // CWE-316: erase key material from stack
    return true;
}

QByteArray CryptoManager::aes256CbcEncrypt(const QByteArray &plaintext, const uint8_t *key, const uint8_t *iv) {
    // Apply PKCS#7 padding
    int padLen = 16 - (plaintext.size() % 16);
    QByteArray padded = plaintext;
    padded.append(padLen, static_cast<char>(padLen));

#if defined(Q_OS_MACOS)
    QByteArray ciphertext;
    ciphertext.resize(padded.size());
    size_t bytesMoved = 0;
    CCCryptorStatus status = CCCrypt(kCCEncrypt, kCCAlgorithmAES,
                                     0,  // No hardware padding, we padded manually via PKCS#7
                                     key, kCCKeySizeAES256, iv, padded.constData(), padded.size(), ciphertext.data(),
                                     ciphertext.size(), &bytesMoved);
    if (status == kCCSuccess) {
        ciphertext.resize(static_cast<int>(bytesMoved));
        return ciphertext;
    }
    return QByteArray();
#else
    // Fallback or portable AES-256-CBC
    uint8_t roundKeys[240];
    PortableAes::keyExpansion256(key, roundKeys);

    QByteArray ciphertext;
    ciphertext.resize(padded.size());

    uint8_t currentIv[16];
    std::memcpy(currentIv, iv, 16);

    const int numBlocks = padded.size() / 16;
    for (int b = 0; b < numBlocks; ++b) {
        uint8_t block[16];
        const uint8_t *src = reinterpret_cast<const uint8_t *>(padded.constData() + b * 16);
        for (int i = 0; i < 16; ++i) {
            block[i] = src[i] ^ currentIv[i];
        }
        uint8_t *dst = reinterpret_cast<uint8_t *>(ciphertext.data() + b * 16);
        PortableAes::encryptBlock(block, dst, roundKeys);
        std::memcpy(currentIv, dst, 16);
        secureZeroMemory(block, sizeof(block));
    }

    secureZeroMemory(currentIv, sizeof(currentIv));
    secureZeroMemory(roundKeys, sizeof(roundKeys));

    return ciphertext;
#endif
}

QByteArray CryptoManager::aes256CbcDecrypt(const QByteArray &ciphertext, const uint8_t *key, const uint8_t *iv,
                                           bool *ok) {
    if (ok)
        *ok = false;
    if (ciphertext.isEmpty() || ciphertext.size() % 16 != 0) {
        return QByteArray();
    }

    QByteArray decrypted;
    decrypted.resize(ciphertext.size());

#if defined(Q_OS_MACOS)
    size_t bytesMoved = 0;
    CCCryptorStatus status = CCCrypt(kCCDecrypt, kCCAlgorithmAES,
                                     0,  // No hardware unpadding, unpad manually to avoid padding oracle leakage
                                     key, kCCKeySizeAES256, iv, ciphertext.constData(), ciphertext.size(),
                                     decrypted.data(), decrypted.size(), &bytesMoved);
    if (status != kCCSuccess) {
        return QByteArray();
    }
#else
    uint8_t roundKeys[240];
    PortableAes::keyExpansion256(key, roundKeys);

    uint8_t currentIv[16];
    std::memcpy(currentIv, iv, 16);

    const int numBlocks = ciphertext.size() / 16;
    for (int b = 0; b < numBlocks; ++b) {
        const uint8_t *src = reinterpret_cast<const uint8_t *>(ciphertext.constData() + b * 16);
        uint8_t rawBlock[16];
        PortableAes::decryptBlock(src, rawBlock, roundKeys);
        uint8_t *dst = reinterpret_cast<uint8_t *>(decrypted.data() + b * 16);
        for (int i = 0; i < 16; ++i) {
            dst[i] = rawBlock[i] ^ currentIv[i];
        }
        std::memcpy(currentIv, src, 16);
        secureZeroMemory(rawBlock, sizeof(rawBlock));
    }

    secureZeroMemory(currentIv, sizeof(currentIv));
    secureZeroMemory(roundKeys, sizeof(roundKeys));
#endif

    // Validate PKCS#7 padding in constant time
    if (decrypted.isEmpty()) {
        return QByteArray();
    }

    const uint8_t *dataPtr = reinterpret_cast<const uint8_t *>(decrypted.constData());
    const int totalLen = decrypted.size();
    uint8_t padVal = dataPtr[totalLen - 1];

    if (padVal == 0 || padVal > 16 || padVal > totalLen) {
        return QByteArray();
    }

    uint8_t padDiff = 0;
    for (int i = 0; i < padVal; ++i) {
        padDiff |= (dataPtr[totalLen - 1 - i] ^ padVal);
    }

    if (padDiff != 0) {
        return QByteArray();
    }

    decrypted.chop(padVal);
    if (ok)
        *ok = true;
    return decrypted;
}

CryptoManager::CryptoResult CryptoManager::encrypt(const QByteArray &plaintext, const QString &passphrase) {
    CryptoResult res;
    if (passphrase.isEmpty()) {
        res.errorMessage = QStringLiteral("Passphrase cannot be empty.");
        return res;
    }

    // 1. Generate 16 bytes Salt and 16 bytes IV
    uint8_t salt[SALT_LEN];
    uint8_t iv[IV_LEN];
    QRandomGenerator::system()->fillRange(reinterpret_cast<quint32 *>(salt), SALT_LEN / sizeof(quint32));
    QRandomGenerator::system()->fillRange(reinterpret_cast<quint32 *>(iv), IV_LEN / sizeof(quint32));

    // 2. Derive 32 bytes AES Key + 32 bytes HMAC Key
    uint8_t aesKey[32];
    uint8_t hmacKey[32];
    if (!deriveKeys(passphrase, salt, SALT_LEN, DEFAULT_PBKDF2_ITERATIONS, aesKey, hmacKey)) {
        res.errorMessage = QStringLiteral("Failed to derive encryption keys.");
        return res;
    }

    // CWE-316: RAII scope guard guarantees keys are zeroed on ANY return path
    struct KeyScopeGuard {
        uint8_t *k1;
        size_t s1;
        uint8_t *k2;
        size_t s2;
        ~KeyScopeGuard() {
            if (k1 && s1 > 0)
                secureZeroMemory(k1, s1);
            if (k2 && s2 > 0)
                secureZeroMemory(k2, s2);
        }
    } keyGuard{aesKey, sizeof(aesKey), hmacKey, sizeof(hmacKey)};

    // 3. Encrypt AES-256-CBC
    const QByteArray cipher = aes256CbcEncrypt(plaintext, aesKey, iv);
    if (cipher.isEmpty() && !plaintext.isEmpty()) {
        res.errorMessage = QStringLiteral("AES encryption failed.");
        return res;
    }

    // 4. Assemble authenticated container:
    // Header (8) + Iterations (4) + Salt (16) + IV (16) + Ciphertext
    QByteArray authenticatedPayload;
    authenticatedPayload.append(MAGIC_HEADER, static_cast<int>(MAGIC_HEADER_LEN));

    quint32 iterBigEndian = qToBigEndian<quint32>(DEFAULT_PBKDF2_ITERATIONS);
    authenticatedPayload.append(reinterpret_cast<const char *>(&iterBigEndian), sizeof(iterBigEndian));
    authenticatedPayload.append(reinterpret_cast<const char *>(salt), static_cast<int>(SALT_LEN));
    authenticatedPayload.append(reinterpret_cast<const char *>(iv), static_cast<int>(IV_LEN));
    authenticatedPayload.append(cipher);

    // 5. Calculate HMAC-SHA256 over entire authenticatedPayload
    const QByteArray mac = hmacSha256(authenticatedPayload, hmacKey, sizeof(hmacKey));

    // 6. Final format: Magic (8) + Iter (4) + Salt (16) + IV (16) + MAC (32) + Ciphertext
    QByteArray container;
    container.append(MAGIC_HEADER, static_cast<int>(MAGIC_HEADER_LEN));
    container.append(reinterpret_cast<const char *>(&iterBigEndian), sizeof(iterBigEndian));
    container.append(reinterpret_cast<const char *>(salt), static_cast<int>(SALT_LEN));
    container.append(reinterpret_cast<const char *>(iv), static_cast<int>(IV_LEN));
    container.append(mac);
    container.append(cipher);

    res.success = true;
    res.data = container;

    // CWE-316: Securely zero all key material before return to prevent stack residue
    secureZeroMemory(aesKey, sizeof(aesKey));
    secureZeroMemory(hmacKey, sizeof(hmacKey));
    return res;
}

CryptoManager::CryptoResult CryptoManager::decrypt(const QByteArray &ciphertext, const QString &passphrase) {
    CryptoResult res;
    if (passphrase.isEmpty()) {
        res.errorMessage = QStringLiteral("Passphrase cannot be empty.");
        return res;
    }

    if (!isEncryptedData(ciphertext) || ciphertext.size() < static_cast<int>(HEADER_META_LEN)) {
        res.errorMessage = QStringLiteral("Invalid file format: not a recognized SparkNotePad encrypted file.");
        return res;
    }

    const char *ptr = ciphertext.constData();
    const char *fileMagic = ptr;
    ptr += MAGIC_HEADER_LEN;

    // Parse iterations
    quint32 iterRaw = *reinterpret_cast<const quint32 *>(ptr);
    quint32 iterations = qFromBigEndian<quint32>(iterRaw);
    ptr += sizeof(quint32);

    if (iterations < 1000 || iterations > 5000000) {
        res.errorMessage = QStringLiteral("Invalid KDF iteration count in container.");
        return res;
    }

    // Parse salt
    const uint8_t *salt = reinterpret_cast<const uint8_t *>(ptr);
    ptr += SALT_LEN;

    // Parse IV
    const uint8_t *iv = reinterpret_cast<const uint8_t *>(ptr);
    ptr += IV_LEN;

    // Parse stored MAC
    const uint8_t *storedMac = reinterpret_cast<const uint8_t *>(ptr);

    // Remaining is ciphertext
    int cipherOffset = static_cast<int>(HEADER_META_LEN);
    const QByteArray actualCipher = ciphertext.mid(cipherOffset);

    // 1. Derive Keys
    uint8_t aesKey[32];
    uint8_t hmacKey[32];
    if (!deriveKeys(passphrase, salt, SALT_LEN, iterations, aesKey, hmacKey)) {
        res.errorMessage = QStringLiteral("Key derivation failed.");
        return res;
    }

    // CWE-316: RAII scope guard guarantees keys are zeroed on ANY return path (tamper, padding error, etc.)
    struct KeyScopeGuard {
        uint8_t *k1;
        size_t s1;
        uint8_t *k2;
        size_t s2;
        ~KeyScopeGuard() {
            if (k1 && s1 > 0)
                secureZeroMemory(k1, s1);
            if (k2 && s2 > 0)
                secureZeroMemory(k2, s2);
        }
    } keyGuard{aesKey, sizeof(aesKey), hmacKey, sizeof(hmacKey)};

    // 2. Reconstruct authenticated payload and verify HMAC
    QByteArray authenticatedPayload;
    authenticatedPayload.append(fileMagic, static_cast<int>(MAGIC_HEADER_LEN));
    authenticatedPayload.append(reinterpret_cast<const char *>(&iterRaw), sizeof(iterRaw));
    authenticatedPayload.append(reinterpret_cast<const char *>(salt), static_cast<int>(SALT_LEN));
    authenticatedPayload.append(reinterpret_cast<const char *>(iv), static_cast<int>(IV_LEN));
    authenticatedPayload.append(actualCipher);

    const QByteArray calculatedMac = hmacSha256(authenticatedPayload, hmacKey, sizeof(hmacKey));

    if (!constantTimeCompare(reinterpret_cast<const uint8_t *>(calculatedMac.constData()), storedMac, MAC_LEN)) {
        res.errorMessage = QStringLiteral("Incorrect passphrase or file has been corrupted/tampered with.");
        return res;
    }

    // 3. Decrypt ciphertext
    bool decryptOk = false;
    const QByteArray plaintext = aes256CbcDecrypt(actualCipher, aesKey, iv, &decryptOk);
    if (!decryptOk) {
        res.errorMessage = QStringLiteral("Decryption failed: corrupted data padding.");
        return res;
    }

    // CWE-316: Securely zero all key material before return to prevent stack residue
    secureZeroMemory(aesKey, sizeof(aesKey));
    secureZeroMemory(hmacKey, sizeof(hmacKey));

    res.success = true;
    res.data = plaintext;
    return res;
}

CryptoManager::StrengthResult CryptoManager::evaluatePasswordStrength(const QString &passphrase) {
    StrengthResult res;
    const int len = passphrase.length();
    if (len == 0) {
        res.strength = PasswordStrength::VeryWeak;
        res.score = 0;
        res.label = QStringLiteral("Empty");
        return res;
    }

    int score = 0;

    // Length score (up to 35 pts)
    if (len >= 16)
        score += 35;
    else if (len >= 12)
        score += 25;
    else if (len >= 8)
        score += 15;
    else
        score += 5;

    // Character varieties
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSymbol = false;
    for (const QChar &ch : passphrase) {
        if (ch.isLower())
            hasLower = true;
        else if (ch.isUpper())
            hasUpper = true;
        else if (ch.isDigit())
            hasDigit = true;
        else
            hasSymbol = true;
    }

    int varietyCount = 0;
    if (hasLower) {
        score += 10;
        varietyCount++;
    }
    if (hasUpper) {
        score += 15;
        varietyCount++;
    }
    if (hasDigit) {
        score += 15;
        varietyCount++;
    }
    if (hasSymbol) {
        score += 20;
        varietyCount++;
    }

    // Variety combo bonus
    if (varietyCount >= 4)
        score += 10;
    else if (varietyCount >= 3)
        score += 5;

    // Penalties for common patterns
    const QString lower = passphrase.toLower();
    if (lower.contains(QStringLiteral("password")) || lower.contains(QStringLiteral("123456")) ||
        lower.contains(QStringLiteral("qwerty")) || lower.contains(QStringLiteral("admin"))) {
        score -= 25;
    }

    score = std::clamp(score, 0, 100);
    res.score = score;

    if (score < 25) {
        res.strength = PasswordStrength::VeryWeak;
        res.label = QStringLiteral("Very Weak");
    } else if (score < 50) {
        res.strength = PasswordStrength::Weak;
        res.label = QStringLiteral("Weak");
    } else if (score < 70) {
        res.strength = PasswordStrength::Fair;
        res.label = QStringLiteral("Fair");
    } else if (score < 85) {
        res.strength = PasswordStrength::Good;
        res.label = QStringLiteral("Good");
    } else {
        res.strength = PasswordStrength::Strong;
        res.label = QStringLiteral("Strong");
    }

    return res;
}
