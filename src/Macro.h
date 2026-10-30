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

#ifndef MACRO_H
#define MACRO_H

#include "MacroStep.h"

class Macro {
public:
    Macro();

    ~Macro();

    void addMacroStep(Scintilla::Message message, Scintilla::uptr_t wParam, Scintilla::sptr_t lParam);
    void addMacroStep(const MacroStep &step);

    qsizetype size() const { return steps.size(); }

    void replay(ScintillaNext *editor, int n = 1) const;
    void replayTillEndOfFile(ScintillaNext *editor) const;

    const QString &getName() const;
    void setName(const QString &value);

    QVector<MacroStep> &getSteps() { return steps; }

    friend QDataStream &operator<<(QDataStream &stream, const Macro &macro) {
        return stream << macro.name << macro.steps;
    }
    friend QDataStream &operator>>(QDataStream &stream, Macro &macro) { return stream >> macro.name >> macro.steps; }

private:
    QVector<MacroStep> steps;
    QString name;
};
Q_DECLARE_METATYPE(Macro)
Q_DECLARE_METATYPE(Macro *)

#endif  // MACRO_H
