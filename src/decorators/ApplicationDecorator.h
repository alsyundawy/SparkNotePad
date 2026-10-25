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

#ifndef APPLICATIONDECORATOR_H
#define APPLICATIONDECORATOR_H

#include <QObject>

#include "SparkNotePadApplication.h"

class ApplicationDecorator : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY stateChanged)

public:
    explicit ApplicationDecorator(SparkNotePadApplication *app) : QObject(app), app(app) {}

    bool isEnabled() const { return enabled; }

public slots:
    void setEnabled(bool b);

signals:
    void stateChanged(bool b);

protected:
    SparkNotePadApplication *app;
    bool enabled = false;
};

#endif  // APPLICATIONDECORATOR_H
