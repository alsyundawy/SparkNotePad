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

#include "FileTimeUtils.h"
#include "ScintillaNext.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QCoreApplication>

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#endif

QDateTime FileTimeUtils::getFileModificationTime(const QString &filePath) {
    if (filePath.isEmpty())
        return QDateTime();

    QFileInfo fi(filePath);
    if (!fi.exists())
        return QDateTime();

    fi.refresh();
    return fi.lastModified();
}

QDateTime FileTimeUtils::getFileAccessTime(const QString &filePath) {
    if (filePath.isEmpty())
        return QDateTime();

    QFileInfo fi(filePath);
    if (!fi.exists())
        return QDateTime();

    fi.refresh();
    return fi.lastRead();
}

bool FileTimeUtils::setFileTimes(const QString &filePath, const QDateTime &modificationTime,
                                 const QDateTime &accessTime) {
    if (filePath.isEmpty() || !modificationTime.isValid())
        return false;

    QFileInfo fi(filePath);
    if (!fi.exists())
        return false;

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    QByteArray localPath = QFile::encodeName(filePath);
    struct timespec ts[2];

    // ts[0] = access time
    QDateTime atime = accessTime.isValid() ? accessTime : fi.lastRead();
    if (!atime.isValid())
        atime = modificationTime;

    ts[0].tv_sec = atime.toSecsSinceEpoch();
    ts[0].tv_nsec = static_cast<long>((atime.toMSecsSinceEpoch() % 1000) * 1000000L);

    // ts[1] = modification time
    ts[1].tv_sec = modificationTime.toSecsSinceEpoch();
    ts[1].tv_nsec = static_cast<long>((modificationTime.toMSecsSinceEpoch() % 1000) * 1000000L);

    if (utimensat(AT_FDCWD, localPath.constData(), ts, 0) == 0) {
        return true;
    }

    // Fallback to utimes if utimensat is not supported on older kernels/APIs
    struct timeval tv[2];
    tv[0].tv_sec = ts[0].tv_sec;
    tv[0].tv_usec = static_cast<suseconds_t>(ts[0].tv_nsec / 1000L);
    tv[1].tv_sec = ts[1].tv_sec;
    tv[1].tv_usec = static_cast<suseconds_t>(ts[1].tv_nsec / 1000L);
    if (utimes(localPath.constData(), tv) == 0) {
        return true;
    }
#endif

    // Cross-platform fallback using Qt QFile::setFileTime
    QFile file(filePath);
    if (file.open(QFile::ReadWrite | QFile::ExistingOnly)) {
        bool ok = file.setFileTime(modificationTime, QFileDevice::FileModificationTime);
        if (accessTime.isValid()) {
            file.setFileTime(accessTime, QFileDevice::FileAccessTime);
        }
        file.close();
        return ok;
    }

    return false;
}

bool FileTimeUtils::setFileModificationTime(const QString &filePath, const QDateTime &time) {
    return setFileTimes(filePath, time, QDateTime());
}

FileTimeUtils::FileTimeResult FileTimeUtils::touchFile(const QString &filePath) {
    FileTimeResult result;
    if (filePath.isEmpty()) {
        result.success = false;
        result.errorMessage = QCoreApplication::translate("FileTimeUtils", "File path is empty.");
        return result;
    }

    QFileInfo fi(filePath);
    if (!fi.exists()) {
        result.success = false;
        result.errorMessage = QCoreApplication::translate("FileTimeUtils", "File does not exist: %1").arg(filePath);
        return result;
    }

    fi.refresh();
    result.originalTime = fi.lastModified();
    const QDateTime now = QDateTime::currentDateTime();

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    QByteArray localPath = QFile::encodeName(filePath);
    // Passing NULL to utimensat sets both times to current system time
    if (utimensat(AT_FDCWD, localPath.constData(), nullptr, 0) == 0) {
        fi.refresh();
        result.success = true;
        result.newTime = fi.lastModified();
        result.message = QCoreApplication::translate("FileTimeUtils", "File timestamp successfully updated to %1.")
                             .arg(result.newTime.toString(Qt::ISODate));
        return result;
    }

    if (utimes(localPath.constData(), nullptr) == 0) {
        fi.refresh();
        result.success = true;
        result.newTime = fi.lastModified();
        result.message = QCoreApplication::translate("FileTimeUtils", "File timestamp successfully updated to %1.")
                             .arg(result.newTime.toString(Qt::ISODate));
        return result;
    }
#endif

    // Fallback using Qt QFile::setFileTime
    QFile file(filePath);
    if (file.open(QFile::ReadWrite | QFile::ExistingOnly)) {
        bool okMod = file.setFileTime(now, QFileDevice::FileModificationTime);
        file.setFileTime(now, QFileDevice::FileAccessTime);
        file.close();
        if (okMod) {
            fi.refresh();
            result.success = true;
            result.newTime = fi.lastModified();
            result.message = QCoreApplication::translate("FileTimeUtils", "File timestamp successfully updated to %1.")
                                 .arg(result.newTime.toString(Qt::ISODate));
            return result;
        }
    }

    result.success = false;
    result.errorMessage = QCoreApplication::translate(
        "FileTimeUtils", "Failed to update file timestamp (permission denied or I/O error).");
    return result;
}

FileTimeUtils::FileTimeResult FileTimeUtils::touchEditorFile(ScintillaNext *editor) {
    FileTimeResult res;
    if (!editor) {
        res.errorMessage = QCoreApplication::translate("FileTimeUtils", "No active editor.");
        return res;
    }

    if (!editor->isFile()) {
        res.errorMessage = QCoreApplication::translate("FileTimeUtils", "Document is not saved as a file on disk.");
        return res;
    }

    const QString path = editor->getFilePath();
    res = touchFile(path);
    if (res.success) {
        // Synchronize editor internal modifiedTime so it recognizes the updated timestamp
        editor->updateTimestamp();
    }
    return res;
}

FileTimeUtils::FileTimeResult FileTimeUtils::saveWithOriginalTime(ScintillaNext *editor) {
    FileTimeResult res;
    if (!editor) {
        res.errorMessage = QCoreApplication::translate("FileTimeUtils", "No active editor.");
        return res;
    }

    if (!editor->isFile()) {
        res.errorMessage = QCoreApplication::translate("FileTimeUtils", "Document is not saved as a file on disk.");
        return res;
    }

    const QString path = editor->getFilePath();
    QFileInfo fi(path);
    if (!fi.exists()) {
        res.errorMessage = QCoreApplication::translate("FileTimeUtils", "File does not exist on disk: %1").arg(path);
        return res;
    }

    fi.refresh();
    const QDateTime originalTime = fi.lastModified();
    const QDateTime originalAccessTime = fi.lastRead();

    // Perform save with preserveTimestamp flag enabled
    QFileDevice::FileError err = editor->save(/*preserveTimestamp=*/true);
    if (err != QFileDevice::NoError) {
        res.errorMessage = QCoreApplication::translate("FileTimeUtils", "Failed to save file: error code %1")
                               .arg(static_cast<int>(err));
        return res;
    }

    res.success = true;
    res.originalTime = originalTime;
    res.newTime = originalTime;
    res.message =
        QCoreApplication::translate("FileTimeUtils", "File \"%1\" saved with original timestamp preserved (%2)")
            .arg(editor->getName(), originalTime.toString(Qt::ISODate));
    return res;
}
