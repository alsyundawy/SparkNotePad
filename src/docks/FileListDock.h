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

#ifndef FILELISTDOCK_H
#define FILELISTDOCK_H

#include "ScintillaNext.h"

#include <QDockWidget>
#include <QListWidgetItem>
#include <memory>

namespace Ui {
class FileList;
}

class MainWindow;

class FileListDock : public QDockWidget {
    Q_OBJECT

public:
    explicit FileListDock(MainWindow *parent);
    ~FileListDock() override;

private slots:
    void refreshList();
    void addEditor(ScintillaNext *editor);
    void removeEditor(const ScintillaNext *editor);

    void selectCurrentEditor();

    void itemClicked(const QListWidgetItem *item) const;

    void editorSavePointChanged(bool dirty);
    void editorRenamed();
    void onVisibilityChanged(bool visible);

private:
    QListWidgetItem *lookupItemByEditor(const ScintillaNext *editor) const;

    std::unique_ptr<Ui::FileList> ui;
    MainWindow *window;
};

#endif  // FILELISTDOCK_H
