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

#include "DefaultDirectoryManager.h"
#include "ApplicationSettings.h"

DefaultDirectoryManager::DefaultDirectoryManager(MainWindow *window, ApplicationSettings *settings)
    : QObject{window}, window(window), settings(settings) {
    connect(window, &MainWindow::editorActivated, this, &DefaultDirectoryManager::editorActivated);
    connect(window, &MainWindow::fileDialogAccepted, this, &DefaultDirectoryManager::fileDialogAccepted);
}

QString DefaultDirectoryManager::getDefaultDirectory() const {
    using enum ApplicationSettings::DefaultDirectoryBehaviorEnum;

    if (const auto e = settings->defaultDirectoryBehavior(); e == FollowCurrentDocument) {
        const ScintillaNext *editor = window->currentEditor();
        return editor->isFile() ? editor->getPath() : settings->defaultDirectory();
    } else if (e == RememberLastUsed || e == HardCoded) {
        return settings->defaultDirectory();
    }

    return QString();
}

void DefaultDirectoryManager::editorActivated(const ScintillaNext *editor) {
    using enum ApplicationSettings::DefaultDirectoryBehaviorEnum;
    if (settings->defaultDirectoryBehavior() == FollowCurrentDocument && editor && editor->isFile()) {
        settings->setDefaultDirectory(editor->getPath());
    }
}

void DefaultDirectoryManager::fileDialogAccepted(const QString &filePath) {
    using enum ApplicationSettings::DefaultDirectoryBehaviorEnum;
    if (settings->defaultDirectoryBehavior() == RememberLastUsed && !filePath.isEmpty()) {
        settings->setDefaultDirectory(QFileInfo(filePath).absolutePath());
    }
}
