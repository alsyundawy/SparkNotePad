#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <QString>
#include <QCryptographicHash>
#include <functional>

class HashIntegrityManager {
public:
    HashIntegrityManager() = default;

    /**
     * @brief Computes cryptographic hash for a file using chunk-by-chunk streaming
     * to ensure constant memory footprint regardless of file size.
     */
    static QString computeFileHash(const QString &filePath,
                                   QCryptographicHash::Algorithm algorithm = QCryptographicHash::Sha256,
                                   bool *ok = nullptr,
                                   const std::function<void(qint64, qint64)> &progressCallback = nullptr);

    /**
     * @brief Computes cryptographic hash for an in-memory text buffer.
     */
    static QString computeTextHash(const QString &text,
                                   QCryptographicHash::Algorithm algorithm = QCryptographicHash::Sha256);

    /**
     * @brief Performs case-insensitive, whitespace-trimmed comparison between
     * computed hash and an expected checksum string.
     */
    static bool verifyHashMatch(const QString &computedHash, const QString &expectedHash);
};
