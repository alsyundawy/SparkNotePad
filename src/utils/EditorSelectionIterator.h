#pragma once
/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef EDITORSELECTIONITERATOR_H
#define EDITORSELECTIONITERATOR_H

#include <QString>
#include <functional>

#if !defined(UNIT_TESTING)
#include "ScintillaNext.h"
#endif

class EditorSelectionIterator {
public:
#if !defined(UNIT_TESTING)
    /**
     * @brief Transforms each active selection in the editor using the given transform function.
     * If no selection is active, transforms the entire document or the current line based on fallbackToLine.
     * Wraps the entire operation in a single atomic undo action.
     *
     * @param editor Pointer to active ScintillaNext editor instance.
     * @param transform Lambda or function object accepting (const QString &in) -> QString out.
     * @param fallbackToLine If true and no selection exists, operates on the current line.
     * @return true if any text was transformed; false otherwise.
     */
    template <typename Func>
    static bool transformSelections(ScintillaNext *editor, Func transform, bool fallbackToLine = false) {
        if (!editor) {
            return false;
        }

        const sptr_t selCount = editor->selections();
        bool hasAnySelection = false;

        for (sptr_t i = 0; i < selCount; ++i) {
            if (editor->selectionNStart(i) != editor->selectionNEnd(i)) {
                hasAnySelection = true;
                break;
            }
        }

        editor->beginUndoAction();

        if (hasAnySelection) {
            // Process selections in reverse order to preserve position offsets
            for (sptr_t i = selCount - 1; i >= 0; --i) {
                const sptr_t start = editor->selectionNStart(i);
                const sptr_t end = editor->selectionNEnd(i);
                if (start == end) {
                    continue;
                }

                const sptr_t minPos = qMin(start, end);
                const sptr_t maxPos = qMax(start, end);
                const QString input = QString::fromUtf8(editor->get_text_range(minPos, maxPos));
                const QString output = transform(input);

                if (output != input) {
                    editor->setTargetRange(minPos, maxPos);
                    const QByteArray utf8 = output.toUtf8();
                    editor->replaceTarget(utf8.length(), utf8.constData());
                    editor->setSelectionNStart(i, minPos);
                    editor->setSelectionNEnd(i, minPos + utf8.length());
                }
            }
        } else if (fallbackToLine) {
            const sptr_t curPos = editor->currentPos();
            const sptr_t line = editor->lineFromPosition(curPos);
            const sptr_t start = editor->positionFromLine(line);
            const sptr_t end = editor->lineEndPosition(line);

            if (start < end) {
                const QString input = QString::fromUtf8(editor->get_text_range(start, end));
                const QString output = transform(input);

                if (output != input) {
                    editor->setTargetRange(start, end);
                    const QByteArray utf8 = output.toUtf8();
                    editor->replaceTarget(utf8.length(), utf8.constData());
                }
            }
        }

        editor->endUndoAction();
        return true;
    }
#endif
};

#endif  // EDITORSELECTIONITERATOR_H
