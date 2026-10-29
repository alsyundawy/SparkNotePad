/*
 * This file is part of SparkNotePad.
 *
 * Full MIME Tools suite inspired by Notepad++ MIME Tools plugin.
 * Reference: Notepad++ mimeTools plugin by Don HO & contributors.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "MimeTools.h"
#include <QObject>
#include <QUrl>
#include <QStringList>

QString MimeTools::processByLine(const QString &input, QString (*func)(const QString &)) {
    if (input.isEmpty())
        return {};

    const bool hasCrLf = input.contains(QLatin1String("\r\n"));
    const QString eol = hasCrLf ? QStringLiteral("\r\n") : QStringLiteral("\n");

    const QStringList lines = input.split(QChar('\n'));
    QStringList processed;
    processed.reserve(lines.size());

    for (const QString &line : lines) {
        QString cleanLine = line;
        if (cleanLine.endsWith(QChar('\r'))) {
            cleanLine.chop(1);
        }
        processed.append(func(cleanLine));
    }

    return processed.join(eol);
}

QString MimeTools::base64Encode(const QString &input) {
    return QString::fromLatin1(input.toUtf8().toBase64(QByteArray::OmitTrailingEquals));
}

QString MimeTools::base64EncodeWithPadding(const QString &input) {
    return QString::fromLatin1(input.toUtf8().toBase64(QByteArray::KeepTrailingEquals));
}

QString MimeTools::base64EncodeUnixEol(const QString &input, int lineLength) {
    const QString b64 = base64Encode(input);
    if (lineLength <= 0 || b64.length() <= lineLength)
        return b64;

    QString result;
    result.reserve(b64.length() + (b64.length() / lineLength) + 1);
    for (qsizetype i = 0; i < b64.length(); i += lineLength) {
        if (i > 0)
            result.append(QLatin1Char('\n'));
        result.append(b64.mid(i, lineLength));
    }
    return result;
}

QString MimeTools::base64EncodeByLine(const QString &input) {
    return processByLine(input, &MimeTools::base64Encode);
}

QString MimeTools::base64Decode(const QString &input) {
    return QString::fromUtf8(QByteArray::fromBase64(input.toUtf8()));
}

QString MimeTools::base64DecodeStrict(const QString &input, bool *ok) {
    const auto result =
        QByteArray::fromBase64Encoding(input.toUtf8().trimmed(), QByteArray::AbortOnBase64DecodingErrors);
    if (result) {
        if (ok)
            *ok = true;
        return QString::fromUtf8(*result);
    }
    if (ok)
        *ok = false;
    return QObject::tr("[Error: Invalid Base64 Data or Padding]");
}

QString MimeTools::base64DecodeByLine(const QString &input) {
    return processByLine(input, &MimeTools::base64Decode);
}

QString MimeTools::quotedPrintableEncode(const QString &input) {
    const QByteArray src = input.toUtf8();
    QByteArray out;
    out.reserve(src.size() * 3);

    int col = 0;
    for (int i = 0; i < src.size(); ++i) {
        const auto c = static_cast<unsigned char>(src.at(i));
        if (col >= 73) {
            out.append("=\r\n");
            col = 0;
        }

        if ((c >= 33 && c <= 126 && c != '=') || c == '\t' ||
            (c == ' ' && i + 1 < src.size() && src.at(i + 1) != '\n' && src.at(i + 1) != '\r')) {
            out.append(static_cast<char>(c));
            col++;
        } else {
            out.append('=');
            out.append(QByteArray::number(c, 16).toUpper().rightJustified(2, '0'));
            col += 3;
        }
    }
    return QString::fromLatin1(out);
}

QString MimeTools::quotedPrintableDecode(const QString &input) {
    const QByteArray src = input.toLatin1();
    QByteArray out;
    out.reserve(src.size());

    for (int i = 0; i < src.size(); ++i) {
        const char ch = src.at(i);
        if (ch == '=') {
            if (i + 2 < src.size() && src.at(i + 1) == '\r' && src.at(i + 2) == '\n') {
                i += 2;  // Soft line break CRLF
                continue;
            }
            if (i + 1 < src.size() && src.at(i + 1) == '\n') {
                i += 1;  // Soft line break LF
                continue;
            }
            if (i + 2 < src.size()) {
                bool okHex = false;
                const int val = src.mid(i + 1, 2).toInt(&okHex, 16);
                if (okHex) {
                    out.append(static_cast<char>(val));
                    i += 2;
                    continue;
                }
            }
        }
        out.append(ch);
    }
    return QString::fromUtf8(out);
}

QString MimeTools::urlEncodeRfc1738(const QString &input) {
    const QByteArray utf8 = input.toUtf8();
    QByteArray out;
    out.reserve(utf8.size() * 3);

    for (const char c : utf8) {
        const auto uc = static_cast<unsigned char>(c);
        if ((uc >= 'a' && uc <= 'z') || (uc >= 'A' && uc <= 'Z') || (uc >= '0' && uc <= '9') || uc == '$' ||
            uc == '-' || uc == '_' || uc == '.' || uc == '+' || uc == '!' || uc == '*' || uc == '\'' || uc == '(' ||
            uc == ')' || uc == ',') {
            out.append(c);
        } else if (uc == ' ') {
            out.append('+');
        } else {
            out.append('%');
            out.append(QByteArray::number(uc, 16).toUpper().rightJustified(2, '0'));
        }
    }
    return QString::fromLatin1(out);
}

QString MimeTools::urlEncodeRfc1738ByLine(const QString &input) {
    return processByLine(input, &MimeTools::urlEncodeRfc1738);
}

QString MimeTools::urlEncodeExtended(const QString &input) {
    return QString::fromLatin1(QUrl::toPercentEncoding(input));
}

QString MimeTools::urlEncodeExtendedByLine(const QString &input) {
    return processByLine(input, &MimeTools::urlEncodeExtended);
}

QString MimeTools::urlEncodeFull(const QString &input) {
    const QByteArray utf8 = input.toUtf8();
    QByteArray out;
    out.reserve(utf8.size() * 3);

    for (const char c : utf8) {
        const auto uc = static_cast<unsigned char>(c);
        out.append('%');
        out.append(QByteArray::number(uc, 16).toUpper().rightJustified(2, '0'));
    }
    return QString::fromLatin1(out);
}

QString MimeTools::urlEncodeFullByLine(const QString &input) {
    return processByLine(input, &MimeTools::urlEncodeFull);
}

QString MimeTools::urlDecode(const QString &input) {
    // RFC 1738 replaces '+' with space before percent-decoding
    QString mod = input;
    mod.replace(QLatin1Char('+'), QLatin1Char(' '));
    return QUrl::fromPercentEncoding(mod.toUtf8());
}
