#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 mark occurrences & Notepad3 match counting logic.
 * Sources: zufuliu/Notepad4 (src/Edit.cpp) & rizonesoft/Notepad3
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef MARKOCCURRENCES_H
#define MARKOCCURRENCES_H

#include <QObject>
#include <QString>
#include <QColor>

class ScintillaNext;

class MarkOccurrences : public QObject {
    Q_OBJECT

public:
    explicit MarkOccurrences(QObject *parent = nullptr);
    ~MarkOccurrences() override = default;

    // Marks all occurrences in the active editor.
    // If text is selected, uses the selection; otherwise uses the word under cursor.
    int markAll(ScintillaNext *editor, bool matchCase = false, bool wholeWord = false, bool useRegex = false);

    // Marks occurrences of specific text query.
    int markText(ScintillaNext *editor, const QString &text, bool matchCase = false, bool wholeWord = false,
                 bool useRegex = false);

    // Clears all marks made by this indicator in the editor.
    void clearMarks(ScintillaNext *editor);

    // Returns indicator index for the given editor.
    int indicatorForEditor(ScintillaNext *editor) const;

    // Last search information
    int lastMatchCount() const { return m_lastMatchCount; }
    const QString &lastQuery() const { return m_lastQuery; }

signals:
    void occurrencesMarked(const QString &query, int count);
    void marksCleared();

private:
    void setupIndicator(ScintillaNext *editor, int indicator);

    int m_lastMatchCount{0};
    QString m_lastQuery;
    QColor m_indicatorColor{255, 153, 0};  // Amber / Gold highlight
};

#endif  // MARKOCCURRENCES_H
