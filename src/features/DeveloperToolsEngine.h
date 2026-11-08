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

class DeveloperToolsEngine {
public:
    static QString formatJson(const QString &json, int indentSpaces = 4, QString *errorMsg = nullptr);

    static QString minifyJson(const QString &json, QString *errorMsg = nullptr);

    static QString urlEncode(const QString &text);

    static QString urlDecode(const QString &text);

    static QString unicodeEscape(const QString &text);

    static QString unicodeUnescape(const QString &text);

    static QString generateUuidV4(bool uppercase = false, bool withBraces = false, bool withDashes = true);

    static QString epochToIso(qint64 epochSeconds);

    static qint64 isoToEpoch(const QString &isoString, bool *ok = nullptr);
};
