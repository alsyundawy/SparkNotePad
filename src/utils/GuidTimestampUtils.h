#pragma once
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

#ifndef GUIDTIMESTAMPUTILS_H
#define GUIDTIMESTAMPUTILS_H

#include <cstdint>
#include <QString>

#ifndef UNIT_TESTING
class ScintillaNext;
#endif

class GuidTimestampUtils {
public:
    enum class GuidFormat : std::uint8_t {
        GuidHyphenatedLower,  // xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx (RFC 4122)
        GuidHyphenatedUpper,  // XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
        GuidWithBracesUpper,  // Format: uppercase GUID enclosed in braces
        GuidWithBracesLower,  // Format: lowercase GUID enclosed in braces
        GuidCompactUpper,     // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX (32 hex uppercase)
        GuidCompactLower      // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx (32 hex lowercase)
    };

    enum class TimestampFormat : std::uint8_t {
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

    static QString generateGuid(GuidFormat format = GuidFormat::GuidHyphenatedLower);
    static QString generateTimestamp(TimestampFormat format = TimestampFormat::DateTimeShort);

#ifndef UNIT_TESTING
    static Result insertGuid(ScintillaNext *editor, GuidFormat format = GuidFormat::GuidHyphenatedLower);
    static Result insertTimestamp(ScintillaNext *editor, TimestampFormat format = TimestampFormat::DateTimeShort);
    static Result insertText(ScintillaNext *editor, const QString &text);
#endif
};

#endif  // GUIDTIMESTAMPUTILS_H
