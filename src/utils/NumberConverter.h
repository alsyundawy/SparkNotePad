#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 number conversion logic.
 * Source: zufuliu/Notepad4 (src/Edit.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef NUMBERCONVERTER_H
#define NUMBERCONVERTER_H

#include <QString>

#if !defined(UNIT_TESTING)
class ScintillaNext;
#endif

class NumberConverter {
public:
    enum TargetBase { ToHex, ToDec, ToBin, ToOct };

    struct Result {
        bool success{false};
        QString original;
        QString converted;
        QString errorMessage;
    };

    // Converts number at selection or cursor in active editor to the target base.
#if !defined(UNIT_TESTING)
    static Result convert(ScintillaNext *editor, TargetBase targetBase, bool uppercaseHex = true,
                          bool addPrefix = true);
#endif

    // Pure parsing helper: detects input base from string (handles 0x, 0b, 0o, etc.)
    static Result convertString(const QString &input, TargetBase targetBase, bool uppercaseHex = true,
                                bool addPrefix = true);
};

#endif  // NUMBERCONVERTER_H
