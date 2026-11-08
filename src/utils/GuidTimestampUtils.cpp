/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) and Notepad3 (rizonesoft/notepad3)
 * GUID and timestamp insertion features.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "GuidTimestampUtils.h"
#ifndef UNIT_TESTING
#include "ScintillaNext.h"
#endif

#include <QUuid>
#include <QDateTime>
#include <QLocale>

QString GuidTimestampUtils::generateGuid(GuidFormat format) {
    const QUuid uuid = QUuid::createUuid();
    QString result;

    switch (format) {
        case GuidFormat::GuidHyphenatedLower:
            result = uuid.toString(QUuid::WithoutBraces).toLower();
            break;
        case GuidFormat::GuidHyphenatedUpper:
            result = uuid.toString(QUuid::WithoutBraces).toUpper();
            break;
        case GuidFormat::GuidWithBracesUpper:
            result = uuid.toString(QUuid::WithBraces).toUpper();
            break;
        case GuidFormat::GuidWithBracesLower:
            result = uuid.toString(QUuid::WithBraces).toLower();
            break;
        case GuidFormat::GuidCompactUpper:
            result = uuid.toString(QUuid::Id128).toUpper();
            break;
        case GuidFormat::GuidCompactLower:
            result = uuid.toString(QUuid::Id128).toLower();
            break;
    }
    return result;
}

QString GuidTimestampUtils::generateTimestamp(TimestampFormat format) {
    const QDateTime now = QDateTime::currentDateTime();

    switch (format) {
        case TimestampFormat::DateTimeShort:
            return now.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        case TimestampFormat::DateTimeLong:
            return QLocale::system().toString(now, QLocale::LongFormat);  // flawfinder: ignore
        case TimestampFormat::Iso8601Local:
            return now.toString(Qt::ISODate);
        case TimestampFormat::Iso8601Utc:
            return QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
        case TimestampFormat::DateOnly:
            return now.toString(QStringLiteral("yyyy-MM-dd"));
        case TimestampFormat::TimeOnly:
            return now.toString(QStringLiteral("HH:mm:ss"));
        case TimestampFormat::UnixTimestampSecs:
            return QString::number(now.toSecsSinceEpoch());
        case TimestampFormat::UnixTimestampMillis:
            return QString::number(now.toMSecsSinceEpoch());
    }

    return now.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
}

#ifndef UNIT_TESTING
GuidTimestampUtils::Result GuidTimestampUtils::insertText(ScintillaNext *editor, const QString &text) {
    if (!editor) {
        return {false, QString(), QStringLiteral("No active editor")};
    }

    if (editor->readOnly()) {
        return {false, QString(), QStringLiteral("Document is read-only")};
    }

    const auto startPos = static_cast<int>(editor->selectionStart());
    const auto endPos = static_cast<int>(editor->selectionEnd());

    editor->beginUndoAction();
    if (startPos != endPos) {
        editor->setTargetRange(startPos, endPos);
    } else {
        const auto curPos = static_cast<int>(editor->currentPos());
        editor->setTargetRange(curPos, curPos);
    }

    const QByteArray utf8 = text.toUtf8();
    editor->replaceTarget(utf8.length(), utf8.constData());
    const auto targetPos = static_cast<int>((startPos != endPos ? startPos : editor->currentPos()) + utf8.length());
    editor->setSelection(targetPos, targetPos);
    editor->endUndoAction();

    return {true, text, QString()};
}

GuidTimestampUtils::Result GuidTimestampUtils::insertGuid(ScintillaNext *editor, GuidFormat format) {
    const QString guid = generateGuid(format);
    return insertText(editor, guid);
}

GuidTimestampUtils::Result GuidTimestampUtils::insertTimestamp(ScintillaNext *editor, TimestampFormat format) {
    const QString ts = generateTimestamp(format);
    return insertText(editor, ts);
}

#endif
