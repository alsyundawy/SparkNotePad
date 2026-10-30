#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad3 Focused View & line filtering architecture.
 * Sources: rizonesoft/Notepad3 (src/Edit.c, FocusedView) & Scintilla line visibility APIs
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef FOCUSEDVIEW_H
#define FOCUSEDVIEW_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QSet>

class ScintillaNext;

class FocusedView : public QObject {
    Q_OBJECT

public:
    struct FilterOptions {
        QString query;
        bool matchCase{false};
        bool wholeWord{false};
        bool useRegex{false};
        bool invert{false};         // If true, hide matching lines (keep non-matching)
        bool bookmarksOnly{false};  // If true, filter based on bookmarked lines
        int contextLines{0};        // Number of context lines before/after match
    };

    struct FilterResult {
        bool active{false};
        int totalLines{0};
        int matchedLines{0};
        int visibleLines{0};
        QString filterQuery;
        QString message;
    };

    explicit FocusedView(QObject *parent = nullptr);
    ~FocusedView() override = default;

    // Checks whether Focused View is currently active on the given editor
    bool isFocused(const ScintillaNext *editor) const;

    // Toggles Focused View:
    // If active -> restores all lines (turns off)
    // If inactive -> filters by selection or word under caret; sets dialogNeeded to true if both empty
    FilterResult toggleFocusedView(ScintillaNext *editor, bool *dialogNeeded = nullptr);

    // Applies full filter options to the editor
    FilterResult applyFilter(ScintillaNext *editor, const FilterOptions &options);

    // Clears any active line filter on the editor, restoring all lines
    FilterResult clearFilter(ScintillaNext *editor);

    // Convenience filter methods
    FilterResult filterBySelectionOrWord(ScintillaNext *editor, bool invert = false);
    FilterResult filterByBookmarks(ScintillaNext *editor, bool invert = false);

    // Returns current or last-used filter options for the editor
    FilterOptions currentOptions(const ScintillaNext *editor) const;

signals:
    void filterApplied(ScintillaNext *editor, const FilterResult &result);
    void filterCleared(ScintillaNext *editor);

private:
    static void ensureHiddenLineStyle(ScintillaNext *editor);

    QMap<const ScintillaNext *, FilterOptions> m_editorOptions;
    FilterOptions m_lastOptions;
};

#endif  // FOCUSEDVIEW_H
