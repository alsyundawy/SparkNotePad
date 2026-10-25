/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad++ Run menu and variable macro expansion concept
 * ($(FULL_CURRENT_PATH), $(CURRENT_DIRECTORY), $(FILE_NAME), etc.)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ExternalToolManager.h"
#include "ScintillaNext.h"

#include <algorithm>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QSettings>
#include <QUrl>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QUuid>

QString ExternalToolManager::getSelectedOrCurrentWord(ScintillaNext *editor) {
    if (!editor)
        return {};

    int selStart = editor->selectionNStart(0);
    int selEnd = editor->selectionNEnd(0);

    if (selEnd > selStart) {
        const int len = selEnd - selStart;
        QByteArray bytes(len + 1, '\0');
        editor->getRange(selStart, selEnd, bytes.data());
        bytes.resize(len);
        return QString::fromUtf8(bytes);
    }

    // Fallback to word under caret
    int pos = editor->currentPos();
    int wStart = editor->wordStartPosition(pos, true);
    int wEnd = editor->wordEndPosition(pos, true);
    if (wEnd > wStart) {
        const int len = wEnd - wStart;
        QByteArray bytes(len + 1, '\0');
        editor->getRange(wStart, wEnd, bytes.data());
        bytes.resize(len);
        return QString::fromUtf8(bytes);
    }

    return {};
}

QString ExternalToolManager::expandVariables(const QString &cmdTemplate, ScintillaNext *editor) {
    QString result = cmdTemplate;
    if (result.isEmpty())
        return result;

    QString fullPath;
    QString dirPath;
    QString fileName;
    QString namePart;
    QString extPart;
    int line = 1;
    int col = 1;

    if (editor) {
        if (editor->isFile()) {
            QFileInfo fi = editor->getFileInfo();
            fullPath = fi.canonicalFilePath();
            if (fullPath.isEmpty())
                fullPath = fi.absoluteFilePath();
            dirPath = fi.dir().canonicalPath();
            if (dirPath.isEmpty())
                dirPath = fi.dir().absolutePath();
            fileName = fi.fileName();
            namePart = fi.completeBaseName();
            extPart = fi.suffix();
        }

        line = editor->lineFromPosition(editor->currentPos()) + 1;
        col = editor->column(editor->currentPos()) + 1;
    }

    const QString currentWord = getSelectedOrCurrentWord(editor);
    const QString appDir = QCoreApplication::applicationDirPath();

    result.replace(QStringLiteral("$(FULL_CURRENT_PATH)"), fullPath);
    result.replace(QStringLiteral("$(CURRENT_DIRECTORY)"), dirPath);
    result.replace(QStringLiteral("$(FILE_NAME)"), fileName);
    result.replace(QStringLiteral("$(NAME_PART)"), namePart);
    result.replace(QStringLiteral("$(EXT_PART)"), extPart);
    result.replace(QStringLiteral("$(CURRENT_WORD)"), currentWord);
    result.replace(QStringLiteral("$(CURRENT_LINE)"), QString::number(line));
    result.replace(QStringLiteral("$(CURRENT_COLUMN)"), QString::number(col));
    result.replace(QStringLiteral("$(APP_DIR)"), appDir);
    result.replace(QStringLiteral("$(NOTEPAD_DIR)"), appDir);

    return result;
}

bool ExternalToolManager::executeCommand(const QString &commandLine, const QString &workingDir, bool inTerminal,
                                         QString *errorOut) {
    const QString trimmed = commandLine.trimmed();
    if (trimmed.isEmpty()) {
        if (errorOut)
            *errorOut = tr("Command line is empty.");
        return false;
    }

    if (inTerminal) {
        openTerminal(workingDir, trimmed);
        return true;
    }

    // Direct background execution
#if defined(Q_OS_WIN)
    bool ok = QProcess::startDetached(QStringLiteral("cmd.exe"), {QStringLiteral("/c"), trimmed}, workingDir);
#else
    bool ok = QProcess::startDetached(QStringLiteral("/bin/bash"), {QStringLiteral("-c"), trimmed}, workingDir);
#endif

    if (!ok && errorOut) {
        *errorOut = tr("Failed to start process: %1").arg(trimmed);
    }
    return ok;
}

bool ExternalToolManager::executeTool(const ExternalTool &tool, ScintillaNext *editor, QString *errorOut) {
    if (tool.saveBeforeRun && editor && !editor->isSavedToDisk()) {
        editor->save();
    }

    const QString expandedCmd = expandVariables(tool.command, editor);
    QString expandedDir = expandVariables(tool.workingDirectory, editor);

    if (expandedDir.isEmpty() && editor && editor->isFile()) {
        expandedDir = editor->getFileInfo().dir().absolutePath();
    }

    return executeCommand(expandedCmd, expandedDir, tool.runInTerminal, errorOut);
}

void ExternalToolManager::openTerminal(const QString &dirPath, const QString &cmdToRun) {
    QString workDir = dirPath;
    if (workDir.isEmpty()) {
        workDir = QDir::currentPath();
    }

#if defined(Q_OS_MACOS) || defined(Q_OS_MAC)
    if (cmdToRun.isEmpty()) {
        QProcess::startDetached(QStringLiteral("open"), {QStringLiteral("-a"), QStringLiteral("Terminal"), workDir});
    } else {
        QString escapedCmd = cmdToRun;
        escapedCmd.replace(QLatin1Char('\\'), QStringLiteral("\\\\"));
        escapedCmd.replace(QLatin1Char('"'), QStringLiteral("\\\""));
        if (!workDir.isEmpty()) {
            escapedCmd = QStringLiteral("cd \"%1\" && %2").arg(workDir, escapedCmd);
        }
        QString appleScript = QStringLiteral("tell application \"Terminal\" to do script \"%1\"").arg(escapedCmd);
        QProcess::startDetached(QStringLiteral("osascript"),
                                {QStringLiteral("-e"), QStringLiteral("tell application \"Terminal\" to activate"),
                                 QStringLiteral("-e"), appleScript});
    }
#elif defined(Q_OS_WIN)
    QString finalCmd = cmdToRun;
    if (finalCmd.isEmpty()) {
        finalCmd = QStringLiteral("title SparkNotePad Terminal");
    }
    QProcess::startDetached(QStringLiteral("cmd.exe"), {QStringLiteral("/k"), finalCmd}, workDir);
#else
    // Linux / BSD
    QString term = qEnvironmentVariable("TERMINAL");
    if (term.isEmpty()) {
        const QStringList candidates = {QStringLiteral("x-terminal-emulator"),
                                        QStringLiteral("gnome-terminal"),
                                        QStringLiteral("konsole"),
                                        QStringLiteral("xfce4-terminal"),
                                        QStringLiteral("alacritty"),
                                        QStringLiteral("kitty"),
                                        QStringLiteral("xterm")};
        const auto it = std::find_if(candidates.cbegin(), candidates.cend(),
                                     [](const QString &c) { return !QStandardPaths::findExecutable(c).isEmpty(); });
        if (it != candidates.cend()) {
            term = *it;
        }
    }
    if (term.isEmpty()) {
        term = QStringLiteral("xterm");
    }

    if (cmdToRun.isEmpty()) {
        QProcess::startDetached(term, {}, workDir);
    } else {
        if (term.contains(QStringLiteral("gnome-terminal"))) {
            QProcess::startDetached(
                term, {QStringLiteral("--working-directory=") + workDir, QStringLiteral("--"), QStringLiteral("bash"),
                       QStringLiteral("-c"), cmdToRun + QStringLiteral("; exec bash")});
        } else if (term.contains(QStringLiteral("konsole"))) {
            QProcess::startDetached(term,
                                    {QStringLiteral("--workdir"), workDir, QStringLiteral("-e"), QStringLiteral("bash"),
                                     QStringLiteral("-c"), cmdToRun + QStringLiteral("; exec bash")});
        } else {
            QProcess::startDetached(term,
                                    {QStringLiteral("-e"), QStringLiteral("bash"), QStringLiteral("-c"),
                                     cmdToRun + QStringLiteral("; exec bash")},
                                    workDir);
        }
    }
#endif
}

void ExternalToolManager::revealInFileManager(const QString &filePath) {
    if (filePath.isEmpty())
        return;

#if defined(Q_OS_MACOS) || defined(Q_OS_MAC)
    QProcess::startDetached(QStringLiteral("open"), {QStringLiteral("-R"), filePath});
#elif defined(Q_OS_WIN)
    QProcess::startDetached(QStringLiteral("explorer.exe"),
                            {QStringLiteral("/select,"), QDir::toNativeSeparators(filePath)});
#else
    QFileInfo fi(filePath);
    QString dir = fi.isDir() ? filePath : fi.dir().absolutePath();
    QProcess::startDetached(QStringLiteral("xdg-open"), {dir});
#endif
}

void ExternalToolManager::openInBrowser(const QString &pathOrUrl) {
    if (pathOrUrl.isEmpty())
        return;

    if (pathOrUrl.startsWith(QStringLiteral("http://"), Qt::CaseInsensitive) ||
        pathOrUrl.startsWith(QStringLiteral("https://"), Qt::CaseInsensitive) ||
        pathOrUrl.startsWith(QStringLiteral("file://"), Qt::CaseInsensitive)) {
        QDesktopServices::openUrl(QUrl(pathOrUrl));
    } else {
        QDesktopServices::openUrl(QUrl::fromLocalFile(pathOrUrl));
    }
}

void ExternalToolManager::webSearch(const QString &query, const QString &service) {
    const QString trimmed = query.trimmed();
    if (trimmed.isEmpty())
        return;

    const QString encoded = QString::fromUtf8(QUrl::toPercentEncoding(trimmed));
    QString urlStr;

    if (service == QStringLiteral("wikipedia")) {
        urlStr = QStringLiteral("https://en.wikipedia.org/wiki/Special:Search?search=%1").arg(encoded);
    } else if (service == QStringLiteral("github")) {
        urlStr = QStringLiteral("https://github.com/search?q=%1").arg(encoded);
    } else if (service == QStringLiteral("stackoverflow")) {
        urlStr = QStringLiteral("https://stackoverflow.com/search?q=%1").arg(encoded);
    } else {
        // Default Google
        urlStr = QStringLiteral("https://www.google.com/search?q=%1").arg(encoded);
    }

    QDesktopServices::openUrl(QUrl(urlStr));
}

bool ExternalToolManager::runCurrentFileWithInterpreter(ScintillaNext *editor, QString *errorOut) {
    if (!editor) {
        if (errorOut)
            *errorOut = tr("No active editor.");
        return false;
    }

    if (!editor->isFile()) {
        if (errorOut)
            *errorOut = tr("Current file must be saved to disk before running.");
        return false;
    }

    if (!editor->isSavedToDisk()) {
        editor->save();
    }

    const QFileInfo fi = editor->getFileInfo();
    const QString filePath = fi.canonicalFilePath().isEmpty() ? fi.absoluteFilePath() : fi.canonicalFilePath();
    const QString ext = fi.suffix().toLower();
    const QString workDir = fi.dir().canonicalPath().isEmpty() ? fi.dir().absolutePath() : fi.dir().canonicalPath();

    QString cmd;
    if (ext == QStringLiteral("py")) {
        cmd = QStringLiteral("python3 \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("js") || ext == QStringLiteral("mjs") || ext == QStringLiteral("cjs")) {
        cmd = QStringLiteral("node \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("ts")) {
        cmd = QStringLiteral("npx ts-node \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("sh") || ext == QStringLiteral("bash")) {
        cmd = QStringLiteral("bash \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("zsh")) {
        cmd = QStringLiteral("zsh \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("rb")) {
        cmd = QStringLiteral("ruby \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("pl")) {
        cmd = QStringLiteral("perl \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("php")) {
        cmd = QStringLiteral("php \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("lua")) {
        cmd = QStringLiteral("lua \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("go")) {
        cmd = QStringLiteral("go run \"%1\"").arg(filePath);
    } else if (ext == QStringLiteral("html") || ext == QStringLiteral("htm")) {
        openInBrowser(filePath);
        return true;
    } else {
        // Check shebang on line 1
        int lineLen = editor->lineLength(0);
        if (lineLen > 2) {
            QByteArray firstLineBytes = editor->getLine(0);
            QString firstLine = QString::fromUtf8(firstLineBytes).trimmed();
            if (firstLine.startsWith(QStringLiteral("#!"))) {
                QString shebang = firstLine.mid(2).trimmed();
                if (shebang.contains(QStringLiteral("python")))
                    cmd = QStringLiteral("python3 \"%1\"").arg(filePath);
                else if (shebang.contains(QStringLiteral("node")))
                    cmd = QStringLiteral("node \"%1\"").arg(filePath);
                else if (shebang.contains(QStringLiteral("bash")))
                    cmd = QStringLiteral("bash \"%1\"").arg(filePath);
                else if (shebang.contains(QStringLiteral("sh")))
                    cmd = QStringLiteral("sh \"%1\"").arg(filePath);
                else if (shebang.contains(QStringLiteral("perl")))
                    cmd = QStringLiteral("perl \"%1\"").arg(filePath);
                else if (shebang.contains(QStringLiteral("ruby")))
                    cmd = QStringLiteral("ruby \"%1\"").arg(filePath);
                else if (shebang.contains(QStringLiteral("php")))
                    cmd = QStringLiteral("php \"%1\"").arg(filePath);
                else if (shebang.contains(QStringLiteral("lua")))
                    cmd = QStringLiteral("lua \"%1\"").arg(filePath);
            }
        }
    }

    if (cmd.isEmpty()) {
        // Fallback: execute file directly if executable, or open in terminal
        cmd = QStringLiteral("\"%1\"").arg(filePath);
    }

    openTerminal(workDir, cmd);
    return true;
}

QList<ExternalTool> ExternalToolManager::loadCustomTools() {
    QList<ExternalTool> tools;
    QSettings settings(QStringLiteral("SparkNotePad"), QStringLiteral("SparkNotePad"));
    int size = settings.beginReadArray(QStringLiteral("ExternalTools"));

    if (size == 0) {
        settings.endArray();
        // Return default preset tools
        ExternalTool t1;
        t1.id = QStringLiteral("preset-python");
        t1.name = QStringLiteral("Run with Python");
        t1.command = QStringLiteral("python3 \"$(FULL_CURRENT_PATH)\"");
        t1.runInTerminal = true;
        t1.saveBeforeRun = true;
        tools.append(t1);

        ExternalTool t2;
        t2.id = QStringLiteral("preset-node");
        t2.name = QStringLiteral("Run with Node.js");
        t2.command = QStringLiteral("node \"$(FULL_CURRENT_PATH)\"");
        t2.runInTerminal = true;
        t2.saveBeforeRun = true;
        tools.append(t2);

        ExternalTool t3;
        t3.id = QStringLiteral("preset-bash");
        t3.name = QStringLiteral("Run in Bash");
        t3.command = QStringLiteral("bash \"$(FULL_CURRENT_PATH)\"");
        t3.runInTerminal = true;
        t3.saveBeforeRun = true;
        tools.append(t3);

        ExternalTool t4;
        t4.id = QStringLiteral("preset-browser");
        t4.name = QStringLiteral("Preview in Browser");
        t4.command = QStringLiteral("$(FULL_CURRENT_PATH)");
        t4.runInTerminal = false;
        t4.saveBeforeRun = true;
        tools.append(t4);

        saveCustomTools(tools);
        return tools;
    }

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        ExternalTool t;
        t.id = settings.value(QStringLiteral("id")).toString();
        t.name = settings.value(QStringLiteral("name")).toString();
        t.command = settings.value(QStringLiteral("command")).toString();
        t.workingDirectory = settings.value(QStringLiteral("workingDirectory")).toString();
        t.shortcut = settings.value(QStringLiteral("shortcut")).toString();
        t.runInTerminal = settings.value(QStringLiteral("runInTerminal"), true).toBool();
        t.saveBeforeRun = settings.value(QStringLiteral("saveBeforeRun"), true).toBool();
        if (!t.name.isEmpty() && !t.command.isEmpty()) {
            tools.append(t);
        }
    }
    settings.endArray();
    return tools;
}

void ExternalToolManager::saveCustomTools(const QList<ExternalTool> &tools) {
    QSettings settings(QStringLiteral("SparkNotePad"), QStringLiteral("SparkNotePad"));
    settings.beginWriteArray(QStringLiteral("ExternalTools"));
    for (int i = 0; i < tools.size(); ++i) {
        settings.setArrayIndex(i);
        const auto &t = tools.at(i);
        settings.setValue(QStringLiteral("id"),
                          t.id.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : t.id);
        settings.setValue(QStringLiteral("name"), t.name);
        settings.setValue(QStringLiteral("command"), t.command);
        settings.setValue(QStringLiteral("workingDirectory"), t.workingDirectory);
        settings.setValue(QStringLiteral("shortcut"), t.shortcut);
        settings.setValue(QStringLiteral("runInTerminal"), t.runInTerminal);
        settings.setValue(QStringLiteral("saveBeforeRun"), t.saveBeforeRun);
    }
    settings.endArray();
}

void ExternalToolManager::addCustomTool(const ExternalTool &tool) {
    auto tools = loadCustomTools();
    ExternalTool t = tool;
    if (t.id.isEmpty()) {
        t.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    tools.append(t);
    saveCustomTools(tools);
}

void ExternalToolManager::deleteCustomTool(const QString &id) {
    auto tools = loadCustomTools();
    for (int i = 0; i < tools.size(); ++i) {
        if (tools.at(i).id == id) {
            tools.removeAt(i);
            break;
        }
    }
    saveCustomTools(tools);
}

QStringList ExternalToolManager::loadCommandHistory() {
    QSettings settings(QStringLiteral("SparkNotePad"), QStringLiteral("SparkNotePad"));
    return settings.value(QStringLiteral("RunDialog/History")).toStringList();
}

void ExternalToolManager::addCommandToHistory(const QString &cmd) {
    if (cmd.trimmed().isEmpty())
        return;

    QStringList history = loadCommandHistory();
    history.removeAll(cmd);
    history.prepend(cmd);
    while (history.size() > 25) {
        history.removeLast();
    }
    QSettings settings(QStringLiteral("SparkNotePad"), QStringLiteral("SparkNotePad"));
    settings.setValue(QStringLiteral("RunDialog/History"), history);
}
