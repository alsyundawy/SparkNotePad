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

#ifndef EDITORDECORATOR_H
#define EDITORDECORATOR_H

#include <QObject>

#include "ScintillaNext.h"

class EditorDecorator : public QObject {  // NOSONAR(cpp:S5008)
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY stateChanged)

public:
    explicit EditorDecorator(ScintillaNext *editor) : QObject(editor), editor(editor) {}
    ~EditorDecorator() override = default;

    bool isEnabled() const { return enabled; }
    ScintillaNext *getEditor() const { return editor; }

public slots:
    virtual void setEnabled(bool b);
    virtual void notify(const Scintilla::NotificationData *pscn) = 0;

signals:
    void stateChanged(bool b);

protected:
    ScintillaNext *editor;  // NOSONAR(cpp:S878, cpp:S5008)
    bool enabled = false;   // NOSONAR(cpp:S878, cpp:S5008)
};

#endif  // EDITORDECORATOR_H
