#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad++ Run menu and variable macro expansion concept
 * ($(FULL_CURRENT_PATH), $(CURRENT_DIRECTORY), $(FILE_NAME), etc.)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef EXTERNALTOOLMANAGER_H
#define EXTERNALTOOLMANAGER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QObject>

class ScintillaNext;

struct ExternalTool {
    QString id;
    QString name;
    QString command;
    QString workingDirectory;
    QString shortcut;
    bool runInTerminal = true;
    bool saveBeforeRun = true;
};

class ExternalToolManager : public QObject {
    Q_OBJECT

public:
    // Variable substitution
    static QString expandVariables(const QString &cmdTemplate, ScintillaNext *editor);

    // Command execution
    static bool executeCommand(const QString &commandLine, const QString &workingDir, bool inTerminal,
                               QString *errorOut = nullptr);
    static bool executeTool(const ExternalTool &tool, ScintillaNext *editor, QString *errorOut = nullptr);

    // Common system actions
    static void openTerminal(const QString &dirPath, const QString &cmdToRun = QString());
    static void revealInFileManager(const QString &filePath);
    static void openInBrowser(const QString &pathOrUrl);
    static void webSearch(const QString &query, const QString &service = QStringLiteral("google"));

    // Quick runner based on file type / interpreter
    static bool runCurrentFileWithInterpreter(ScintillaNext *editor, QString *errorOut = nullptr);

    // Persistence via QSettings
    static QList<ExternalTool> loadCustomTools();
    static void saveCustomTools(const QList<ExternalTool> &tools);
    static void addCustomTool(const ExternalTool &tool);
    static void deleteCustomTool(const QString &id);

    static QStringList loadCommandHistory();
    static void addCommandToHistory(const QString &cmd);

    // Helper
    static QString getSelectedOrCurrentWord(ScintillaNext *editor);
};

#endif  // EXTERNALTOOLMANAGER_H
