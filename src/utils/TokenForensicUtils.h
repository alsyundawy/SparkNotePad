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

#ifndef TOKENFORENSICUTILS_H
#define TOKENFORENSICUTILS_H

#include <cmath>
#include <QByteArray>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

class TokenForensicUtils {
public:
    struct JwtInfo {
        bool valid{false};
        QJsonObject header;
        QJsonObject payload;
        QString signature;
        QDateTime issuedAt;
        QDateTime expiration;
        bool isExpired{false};
        QString formattedOutput;
        QString errorMessage;
    };

    // Calculate Shannon entropy in bits per symbol (0.00 to 8.00)
    static double calculateShannonEntropy(const QString &text);
    static double calculateShannonEntropy(const QByteArray &data);

    // Detect and decode JWT
    static bool isLikelyJwt(const QString &token);
    static JwtInfo decodeJwt(const QString &token);

    // Document forensics statistics
    struct DocumentStats {
        qsizetype totalChars{0};
        qsizetype charsNoSpaces{0};
        qsizetype words{0};
        qsizetype lines{0};
        qsizetype nonEmptyLines{0};
        qsizetype bytesUtf8{0};
        double readingTimeMinutes{0.0};
        double speakingTimeMinutes{0.0};
        double shannonEntropy{0.0};
        bool isHighEntropy{false};
    };

    static DocumentStats analyzeText(const QString &text);
};

#endif  // TOKENFORENSICUTILS_H
