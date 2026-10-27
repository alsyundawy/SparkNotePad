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

#ifndef MYERSDIFFENGINE_H
#define MYERSDIFFENGINE_H

#include <QString>
#include <QStringList>
#include <QVector>

class MyersDiffEngine {
public:
    enum class DiffOp { Equal, Insert, Delete, Modify };

    struct DiffLine {
        DiffOp op{DiffOp::Equal};
        int lineA{-1};
        int lineB{-1};
        QString textA;
        QString textB;
    };

    struct DiffResult {
        QVector<DiffLine> lines;
        int additions{0};
        int deletions{0};
        int modifications{0};
        bool hasDifferences{false};
    };

    static DiffResult compare(const QString &textA, const QString &textB);

private:
    static QVector<DiffLine> computeLcs(const QStringList &linesA, const QStringList &linesB);
};

#endif  // MYERSDIFFENGINE_H
