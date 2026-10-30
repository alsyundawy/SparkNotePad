#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) and Notepad3 (rizonesoft/notepad3)
 * GUID and timestamp insertion features.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef GUIDTIMESTAMPUTILS_H
#define GUIDTIMESTAMPUTILS_H

#include <QString>

#if !defined(UNIT_TESTING)
class ScintillaNext;
#endif

class GuidTimestampUtils {
public:
    enum GuidFormat {
        GuidHyphenatedLower,  // xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx (RFC 4122)
        GuidHyphenatedUpper,  // XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
        GuidWithBracesUpper,  // {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} (Windows registry format)
        GuidWithBracesLower,  // {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}
        GuidCompactUpper,     // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX (32 hex uppercase)
        GuidCompactLower      // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx (32 hex lowercase)
    };

    enum TimestampFormat {
        DateTimeShort,       // yyyy-MM-dd HH:mm:ss
        DateTimeLong,        // Locale long date & time
        Iso8601Local,        // yyyy-MM-ddTHH:mm:ss±HH:mm
        Iso8601Utc,          // yyyy-MM-ddTHH:mm:ssZ
        DateOnly,            // yyyy-MM-dd
        TimeOnly,            // HH:mm:ss
        UnixTimestampSecs,   // 1792352700
        UnixTimestampMillis  // 1792352700000
    };

    struct Result {
        bool success{false};
        QString insertedText;
        QString errorMessage;
    };

    static QString generateGuid(GuidFormat format = GuidHyphenatedLower);
    static QString generateTimestamp(TimestampFormat format = DateTimeShort);

#if !defined(UNIT_TESTING)
    static Result insertGuid(ScintillaNext *editor, GuidFormat format = GuidHyphenatedLower);
    static Result insertTimestamp(ScintillaNext *editor, TimestampFormat format = DateTimeShort);
    static Result insertText(ScintillaNext *editor, const QString &text);
#endif
};

#endif  // GUIDTIMESTAMPUTILS_H
