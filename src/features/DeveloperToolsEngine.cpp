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

#include "DeveloperToolsEngine.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QTimeZone>
#include <QUrl>
#include <QUuid>

QString DeveloperToolsEngine::formatJson(const QString &json, int indentSpaces, QString *errorMsg) {
    (void)indentSpaces;
    QJsonParseError parseErr;
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError) {
        if (errorMsg) {
            *errorMsg = QStringLiteral("JSON Error at offset %1: %2").arg(parseErr.offset).arg(parseErr.errorString());
        }
        return QString();
    }
    if (errorMsg) {
        errorMsg->clear();
    }
    return QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
}

QString DeveloperToolsEngine::minifyJson(const QString &json, QString *errorMsg) {
    QJsonParseError parseErr;
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError) {
        if (errorMsg) {
            *errorMsg = QStringLiteral("JSON Error at offset %1: %2").arg(parseErr.offset).arg(parseErr.errorString());
        }
        return QString();
    }
    if (errorMsg) {
        errorMsg->clear();
    }
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

QString DeveloperToolsEngine::urlEncode(const QString &text) {
    return QString::fromUtf8(QUrl::toPercentEncoding(text));
}

QString DeveloperToolsEngine::urlDecode(const QString &text) {
    return QUrl::fromPercentEncoding(text.toUtf8());
}

QString DeveloperToolsEngine::unicodeEscape(const QString &text) {
    QString result;
    result.reserve(text.length() * 2);

    for (qsizetype i = 0; i < text.length(); ++i) {
        const QChar ch = text.at(i);
        if (ch.isHighSurrogate() && i + 1 < text.length() && text.at(i + 1).isLowSurrogate()) {
            const QChar low = text.at(++i);
            result += QStringLiteral("\\u%1\\u%2")
                          .arg(static_cast<ushort>(ch.unicode()), 4, 16, QLatin1Char('0'))
                          .arg(static_cast<ushort>(low.unicode()), 4, 16, QLatin1Char('0'));
        } else if (ch.unicode() >= 0x20 && ch.unicode() <= 0x7E && ch.unicode() != '\\') {
            result += ch;
        } else {
            result += QStringLiteral("\\u%1").arg(static_cast<ushort>(ch.unicode()), 4, 16, QLatin1Char('0'));
        }
    }

    return result;
}

QString DeveloperToolsEngine::unicodeUnescape(const QString &text) {
    QString result;
    result.reserve(text.length());

    for (qsizetype i = 0; i < text.length(); ++i) {
        if (text.at(i) == QLatin1Char('\\') && i + 1 < text.length()) {
            const QChar next = text.at(i + 1);
            if (next == QLatin1Char('u') && i + 5 < text.length()) {
                bool ok = false;
                const ushort code = text.mid(i + 2, 4).toUShort(&ok, 16);
                if (ok) {
                    i += 5;
                    const QChar ch(code);
                    if (ch.isHighSurrogate() && i + 6 < text.length() && text.mid(i + 1, 2) == QStringLiteral("\\u")) {
                        bool okLow = false;
                        const ushort lowCode = text.mid(i + 3, 4).toUShort(&okLow, 16);
                        if (okLow) {
                            i += 6;
                            result += ch;
                            result += QChar(lowCode);
                            continue;
                        }
                    }
                    result += ch;
                    continue;
                }
            } else if (next == QLatin1Char('\\')) {
                result += QLatin1Char('\\');
                ++i;
                continue;
            } else if (next == QLatin1Char('n')) {
                result += QLatin1Char('\n');
                ++i;
                continue;
            } else if (next == QLatin1Char('r')) {
                result += QLatin1Char('\r');
                ++i;
                continue;
            } else if (next == QLatin1Char('t')) {
                result += QLatin1Char('\t');
                ++i;
                continue;
            }
        }
        result += text.at(i);
    }

    return result;
}

QString DeveloperToolsEngine::generateUuidV4(bool uppercase, bool withBraces, bool withDashes) {
    const QUuid uuid = QUuid::createUuid();
    QString str = uuid.toString(withBraces ? QUuid::WithBraces : QUuid::WithoutBraces);
    if (!withDashes) {
        str.remove(QLatin1Char('-'));
    }
    return uppercase ? str.toUpper() : str.toLower();
}

QString DeveloperToolsEngine::epochToIso(qint64 epochSeconds) {
    const QDateTime dt = QDateTime::fromSecsSinceEpoch(epochSeconds, QTimeZone::utc());
    return dt.toString(Qt::ISODate);
}

qint64 DeveloperToolsEngine::isoToEpoch(const QString &isoString, bool *ok) {
    QDateTime dt = QDateTime::fromString(isoString, Qt::ISODate);
    if (!dt.isValid()) {
        dt = QDateTime::fromString(isoString, Qt::ISODateWithMs);
    }
    if (ok) {
        *ok = dt.isValid();
    }
    return dt.isValid() ? dt.toSecsSinceEpoch() : 0;
}
