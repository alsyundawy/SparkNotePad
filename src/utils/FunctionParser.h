#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Function and symbol parser for NoteSciPad Function List Dock.
 * Adapted from Notepad++ Function List concept and modern symbol extraction patterns.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef FUNCTIONPARSER_H
#define FUNCTIONPARSER_H

#include <QString>
#include <QVector>

class ScintillaNext;

struct FunctionSymbol {
    enum Type { Function, Method, Class, Struct, Heading, Property };

    QString name;
    QString signature;
    QString parentScope;
    int line = 1;      // 1-indexed line number
    int position = 0;  // Character / byte position in Scintilla
    Type type = Function;
};

class FunctionParser {
public:
    static QVector<FunctionSymbol> parse(ScintillaNext *editor);
    static QVector<FunctionSymbol> parseText(const QString &text, const QString &languageName);
};

#endif  // FUNCTIONPARSER_H
