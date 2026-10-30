#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4, UrlThreshold) and Notepad3 URL features.
 * Sources: zufuliu/notepad4 & rizonesoft/Notepad3
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef URLUTILS_H
#define URLUTILS_H

#include <QString>
#include <QUrl>

class ScintillaNext;

class UrlUtils {
public:
    // Checks if the document size exceeds the given UrlThreshold in MB.
    // If thresholdMB is 0, returns true (indicating threshold exceeded / disabled).
    static bool exceedsUrlThreshold(ScintillaNext *editor, int thresholdMB);

    // Extracts URL at given position if present
    static QString getUrlAtPosition(ScintillaNext *editor, int position);

    // Opens URL under cursor in system browser
    static bool openUrlAtPosition(ScintillaNext *editor, int position);

    // Copies URL under cursor to clipboard
    static bool copyUrlAtPosition(ScintillaNext *editor, int position);

    // Standard regex pattern supporting http, https, ftp, ftps, file, and mailto
    static const char *urlRegexPattern();
};

#endif  // URLUTILS_H
