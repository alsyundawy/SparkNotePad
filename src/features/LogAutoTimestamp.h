#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Windows Notepad, Notepad++, and Notepad3 .LOG auto-timestamp specification.
 * Sources: rizonesoft/Notepad3 & classic Notepad .LOG behavior
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOGAUTOTIMESTAMP_H
#define LOGAUTOTIMESTAMP_H

#include <QString>

class ScintillaNext;

class LogAutoTimestamp {
public:
    // Returns true if document starts with .LOG header on line 0
    static bool hasLogHeader(ScintillaNext *editor);

    // Checks and appends .LOG timestamp if header is present.
    // If force is false, ensures it only runs once per file open.
    static bool checkAndApplyLogTimestamp(ScintillaNext *editor, bool force = false);

    // Manually appends timestamp log entry to end of document
    static bool appendLogTimestamp(ScintillaNext *editor);

    // Inserts .LOG header on line 0
    static bool insertLogHeader(ScintillaNext *editor);

    // Settings
    static bool isAutoLogEnabled();
    static void setAutoLogEnabled(bool enabled);

    // Timestamp formatting
    static QString currentLogTimestampString();
};

#endif  // LOGAUTOTIMESTAMP_H
