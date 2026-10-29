#pragma once
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

#ifndef MIMETOOLS_H
#define MIMETOOLS_H

#include <QString>
#include <QByteArray>

class MimeTools {
public:
    // Base64 Suite
    static QString base64Encode(const QString &input);
    static QString base64EncodeWithPadding(const QString &input);
    static QString base64EncodeUnixEol(const QString &input, int lineLength = 76);
    static QString base64EncodeByLine(const QString &input);
    static QString base64Decode(const QString &input);
    static QString base64DecodeStrict(const QString &input, bool *ok = nullptr);
    static QString base64DecodeByLine(const QString &input);

    // Quoted-Printable Suite
    static QString quotedPrintableEncode(const QString &input);
    static QString quotedPrintableDecode(const QString &input);

    // URL Encoding Suite
    static QString urlEncodeRfc1738(const QString &input);
    static QString urlEncodeRfc1738ByLine(const QString &input);
    static QString urlEncodeExtended(const QString &input);
    static QString urlEncodeExtendedByLine(const QString &input);
    static QString urlEncodeFull(const QString &input);
    static QString urlEncodeFullByLine(const QString &input);
    static QString urlDecode(const QString &input);

private:
    static QString processByLine(const QString &input, QString (*func)(const QString &));
};

#endif  // MIMETOOLS_H
