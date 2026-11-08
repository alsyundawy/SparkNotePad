#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <QString>
#include <QStringList>

class ScintillaNext;

class EditorConfigParser {
public:
    enum class IndentStyle { Unset, Tab, Space };

    enum class EndOfLine { Unset, Lf, Crlf, Cr };

    struct Settings {
        IndentStyle indentStyle = IndentStyle::Unset;
        int indentSize = -1;
        int tabWidth = -1;
        EndOfLine endOfLine = EndOfLine::Unset;
        bool trimTrailingWhitespace = false;
        bool insertFinalNewline = false;
        int maxLineLength = -1;
    };

    static Settings resolveSettings(const QString &filePath);

    static bool matchGlob(const QString &pattern, const QString &fileName);

    static QStringList expandBraces(const QString &pattern);

    static void applyToEditor(ScintillaNext *editor, const Settings &settings);
    static void applyToEditor(ScintillaNext *editor, const QString &filePath);
};
