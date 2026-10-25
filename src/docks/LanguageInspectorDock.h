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
 * Notepad Next is free software: you can redistribute it and/or modify
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

#ifndef LANGUAGEINSPECTORDOCK_H
#define LANGUAGEINSPECTORDOCK_H

#include <QDockWidget>

#include "ScintillaTypes.h"

class MainWindow;
class ScintillaNext;

namespace Ui {
class LanguageInspectorDock;
}

class LanguageInspectorDock : public QDockWidget {
    Q_OBJECT

public:
    explicit LanguageInspectorDock(MainWindow *parent);
    ~LanguageInspectorDock();

private slots:
    void connectToEditor(ScintillaNext *editor);
    void updatePositionInfo(Scintilla::Update updated);
    void updateLexerInfo(ScintillaNext *editor);

private:
    Ui::LanguageInspectorDock *ui;

    QMetaObject::Connection editorConnection;
    QMetaObject::Connection documentConnection;

    void disconnectFromEditor();
    void updateLanguageName(ScintillaNext *editor);
    void updatePropertyInfo(ScintillaNext *editor);
    void updateKeywordInfo(ScintillaNext *editor);
    void updateStyleInfo(ScintillaNext *editor);
};

#endif  // LANGUAGEINSPECTORDOCK_H
