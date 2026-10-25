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
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SparkNotePad is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SparkNotePad.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ApplicationSettings.h"
#include "SingleApplication"

#include <QCommandLineParser>
#include <QPointer>
#include <QTimer>

class MainWindow;
class LuaState;
class EditorManager;
class RecentFilesListManager;
class ScintillaNext;
class SessionManager;
class TranslationManager;

class SparkNotePadApplication : public SingleApplication {
    Q_OBJECT

public:
    SparkNotePadApplication(int &argc, char **argv);

    bool init();

    RecentFilesListManager *getRecentFilesListManager() const { return recentFilesListManager; }
    EditorManager *getEditorManager() const { return editorManager; }
    SessionManager *getSessionManager() const;
    TranslationManager *getTranslationManager() const { return translationManager; }

    LuaState *getLuaState() const { return luaState; }
    QString getFileDialogFilter() const;
    QString getFileDialogFilterForLanguage(const QString &language) const;
    ApplicationSettings *getSettings() const { return settings; }

    QStringList getLanguages() const;
    void setEditorLanguage(ScintillaNext *editor, const QString &languageName) const;
    QStringList getLanguageKeywords(const QString &languageName) const;

    QString detectLanguage(ScintillaNext *editor) const;
    QString detectLanguageFromExtension(const QString &extension) const;
    QString detectLanguageFromContents(ScintillaNext *editor) const;

    void sendInfoToPrimaryInstance();

    bool isRunningAsAdmin() const;

    // Releases the SingleApplication primary instance lock so a restarted process
    // can acquire it. Must be called before QProcess::startDetached() in restartApplication().
    // See: MainWindow::restartApplication() (v1.3.5)
    void releaseSingleInstanceLock();

    QStringList debugInfo() const;

protected:
    bool event(QEvent *event) override;

private slots:
    void saveSettings() const;
    void receiveInfoFromSecondaryInstance(quint32 instanceId, QByteArray message);
    void saveSession();

private:
    void openFiles(const QStringList &files);
    void loadSettings();

    EditorManager *editorManager = nullptr;
    RecentFilesListManager *recentFilesListManager = nullptr;
    ApplicationSettings *settings = nullptr;
    SessionManager *sessionManager = nullptr;
    TranslationManager *translationManager = nullptr;

    LuaState *luaState = nullptr;
    MainWindow *window = nullptr;

    // cppcheck-suppress unusedStructMember
    QPointer<QWidget> currentlyFocusedWidget;  // Weak pointer to focused QWidget

    MainWindow *createNewWindow();

    QCommandLineParser parser;
    QTimer autoSaveTimer;  // Autosave session timer
};
