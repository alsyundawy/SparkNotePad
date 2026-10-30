#pragma once
/*
 *
 * This file is part of SparkNotepad.
 *
 * Copyright 2018-2026 Justin Dailey
 * Copyright 2025-2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotepad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Notepad Next is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Notepad Next.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <cstdint>
#include <QDir>
#include <QSettings>

class ScintillaNext;
class MainWindow;
class SparkNotePadApplication;

class SessionManager {
public:
    enum class SessionFileType : std::uint8_t {
        None = 0,
        SavedFile = 1,
        UnsavedFile = 2,
        TempFile = 4,
    };
    using enum SessionFileType;
    Q_DECLARE_FLAGS(SessionFileTypes, SessionFileType)

    explicit SessionManager(SparkNotePadApplication *app, SessionFileTypes types = SessionFileTypes());

    void setSessionFileTypes(SessionFileTypes types);

    static void clear();

    void saveSession(MainWindow *window);
    void loadSession(MainWindow *window);

    bool willFileGetStoredInSession(const ScintillaNext *editor) const;

private:
    static QDir sessionDirectory();

    static void saveIntoSessionDirectory(const ScintillaNext *editor, const QString &sessionFileName);

    static SessionFileType determineType(const ScintillaNext *editor);

    static void clearSettings();
    static void clearDirectory();

    void storeFileDetails(const ScintillaNext *editor, QSettings &settings) const;
    ScintillaNext *loadFileDetails(const QSettings &settings);

    void storeUnsavedFileDetails(const ScintillaNext *editor, QSettings &settings) const;
    ScintillaNext *loadUnsavedFileDetails(const QSettings &settings);

    void storeTempFile(const ScintillaNext *editor, QSettings &settings) const;
    ScintillaNext *loadTempFile(const QSettings &settings);

    void storeEditorViewDetails(const ScintillaNext *editor, QSettings &settings) const;
    void loadEditorViewDetails(ScintillaNext *editor, const QSettings &settings) const;

    SparkNotePadApplication *app;
    SessionFileTypes fileTypes;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SessionManager::SessionFileTypes)

#endif  // SESSIONMANAGER_H
