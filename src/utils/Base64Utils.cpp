/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 Base64 conversion utilities.
 * Source: zufuliu/Notepad4 (src/Edit.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "Base64Utils.h"
#if !defined(UNIT_TESTING)
#include "ScintillaNext.h"
#endif

#if !defined(UNIT_TESTING)
Base64Utils::Result Base64Utils::encode(ScintillaNext *editor) {
    if (!editor) {
        return {false, false, QStringLiteral("No active editor")};
    }

    if (editor->selectionEmpty()) {
        const int docLength = editor->length();
        if (docLength == 0) {
            return {false, true, QStringLiteral("Document is empty")};
        }

        const QByteArray raw = editor->get_text_range(0, docLength);
        const QByteArray encoded = raw.toBase64();

        editor->beginUndoAction();
        editor->setText(encoded.constData());
        editor->endUndoAction();

        return {true, true, QString()};
    }

    const QByteArray selText = editor->getSelText();
    const QByteArray encoded = selText.toBase64();

    editor->beginUndoAction();
    editor->replaceSel(encoded.constData());
    editor->endUndoAction();

    return {true, false, QString()};
}

Base64Utils::Result Base64Utils::decode(ScintillaNext *editor) {
    if (!editor) {
        return {false, false, QStringLiteral("No active editor")};
    }

    if (editor->selectionEmpty()) {
        const int docLength = editor->length();
        if (docLength == 0) {
            return {false, true, QStringLiteral("Document is empty")};
        }

        const QByteArray raw = editor->get_text_range(0, docLength).trimmed();
        auto result = QByteArray::fromBase64Encoding(raw, QByteArray::AbortOnBase64DecodingErrors);
        if (!result) {
            return {false, true, QStringLiteral("Invalid Base64 content or bad padding")};
        }

        editor->beginUndoAction();
        editor->setText(result.decoded.constData());
        editor->endUndoAction();

        return {true, true, QString()};
    }

    const QByteArray selText = editor->getSelText().trimmed();
    auto result = QByteArray::fromBase64Encoding(selText, QByteArray::AbortOnBase64DecodingErrors);
    if (!result) {
        return {false, false, QStringLiteral("Invalid Base64 selection or bad padding")};
    }

    editor->beginUndoAction();
    editor->replaceSel(result.decoded.constData());
    editor->endUndoAction();

    return {true, false, QString()};
}

#endif

QString Base64Utils::encode(const QString &plainText) {
    return QString::fromLatin1(plainText.toUtf8().toBase64());
}

QString Base64Utils::decode(const QString &base64Text, bool *ok) {
    auto result =
        QByteArray::fromBase64Encoding(base64Text.trimmed().toLatin1(), QByteArray::AbortOnBase64DecodingErrors);
    if (!result) {
        if (ok)
            *ok = false;
        return QString();
    }
    if (ok)
        *ok = true;
    return QString::fromUtf8(*result);
}

QString Base64Utils::encodeUrlSafe(const QString &plainText) {
    return QString::fromLatin1(
        plainText.toUtf8().toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
}

QString Base64Utils::decodeUrlSafe(const QString &base64Text, bool *ok) {
    auto result = QByteArray::fromBase64Encoding(base64Text.trimmed().toLatin1(),
                                                 QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
    if (!result) {
        if (ok)
            *ok = false;
        return QString();
    }
    if (ok)
        *ok = true;
    return QString::fromUtf8(*result);
}
