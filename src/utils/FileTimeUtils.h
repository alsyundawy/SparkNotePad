#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) and Notepad3 (rizonesoft/notepad3)
 * file timestamp management (Touch & Preserve Time) features.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef FILETIMEUTILS_H
#define FILETIMEUTILS_H

#include <QDateTime>
#include <QString>

class ScintillaNext;

class FileTimeUtils {
public:
    struct FileTimeResult {
        bool success = false;
        QDateTime originalTime;
        QDateTime newTime;
        QString message;
        QString errorMessage;
    };

    // Low-level filesystem operations
    static QDateTime getFileModificationTime(const QString &filePath);
    static QDateTime getFileAccessTime(const QString &filePath);
    static bool setFileModificationTime(const QString &filePath, const QDateTime &time);
    static bool setFileTimes(const QString &filePath, const QDateTime &modificationTime,
                             const QDateTime &accessTime = QDateTime());

    // Touch file operation (sets mtime & atime to current time on disk without altering contents)
    static FileTimeResult touchFile(const QString &filePath);

    // High-level ScintillaNext editor operations
    static FileTimeResult touchEditorFile(ScintillaNext *editor);
    static FileTimeResult saveWithOriginalTime(ScintillaNext *editor);
};

#endif  // FILETIMEUTILS_H
