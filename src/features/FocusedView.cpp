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

#include "FocusedView.h"
#include "ScintillaNext.h"
#include "BookMarkDecorator.h"

#include <QRegularExpression>
#include <QVector>
#include <QByteArray>
#include <algorithm>

#ifndef SC_ELEMENT_HIDDEN_LINE
#define SC_ELEMENT_HIDDEN_LINE 81
#endif

FocusedView::FocusedView(QObject *parent) : QObject(parent) {}

bool FocusedView::isFocused(const ScintillaNext *editor) const {
    if (!editor)
        return false;
    return m_editorOptions.contains(editor);
}

void FocusedView::ensureHiddenLineStyle(ScintillaNext *editor) {
    if (!editor)
        return;
    // Set subtle horizontal indicator line for hidden blocks: RGBA grey (0x808080 | alpha 0xC0)
    editor->setElementColour(SC_ELEMENT_HIDDEN_LINE, 0x808080 | (0xC0 << 24));
}

FocusedView::FilterOptions FocusedView::currentOptions(const ScintillaNext *editor) const {
    if (!editor || !isFocused(editor)) {
        return m_lastOptions;
    }
    return m_editorOptions.value(editor, m_lastOptions);
}

FocusedView::FilterResult FocusedView::toggleFocusedView(ScintillaNext *editor, bool *dialogNeeded) {
    if (dialogNeeded) {
        *dialogNeeded = false;
    }

    if (!editor) {
        return {false, 0, 0, 0, QString(), tr("No active editor")};
    }

    if (isFocused(editor)) {
        return clearFilter(editor);
    }

    // Check if there is selection or word at cursor
    int selectionStart = static_cast<int>(editor->selectionStart());
    int selectionEnd = static_cast<int>(editor->selectionEnd());
    QString query;

    if (selectionStart != selectionEnd) {
        if ((selectionEnd - selectionStart) <= 1024) {
            query = QString::fromUtf8(editor->get_text_range(selectionStart, selectionEnd)).trimmed();
        }
    } else {
        const int curPos = static_cast<int>(editor->currentPos());
        const int wStart = static_cast<int>(editor->wordStartPosition(curPos, true));
        const int wEnd = static_cast<int>(editor->wordEndPosition(curPos, true));
        if (wStart != wEnd) {
            query = QString::fromUtf8(editor->get_text_range(wStart, wEnd)).trimmed();
        }
    }

    if (query.isEmpty()) {
        if (dialogNeeded) {
            *dialogNeeded = true;
        }
        return {false,     static_cast<int>(editor->lineCount()),
                0,         static_cast<int>(editor->lineCount()),
                QString(), tr("Open filter dialog")};
    }

    FilterOptions opts;
    opts.query = query;
    opts.matchCase = false;
    opts.wholeWord = false;
    opts.useRegex = false;
    opts.invert = false;
    opts.bookmarksOnly = false;
    opts.contextLines = 0;

    return applyFilter(editor, opts);
}

namespace {
bool computeLineMatches(ScintillaNext *editor, const FocusedView::FilterOptions &options, int lineCount,
                        QVector<bool> &lineHasMatch, QString &errorMsg) {
    if (options.bookmarksOnly) {
        const auto *bm = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);
        if (bm == nullptr) {
            errorMsg = QObject::tr("Bookmark decorator unavailable");
            return false;
        }
        for (int line = 0; line < lineCount; ++line) {
            lineHasMatch[line] = bm->isBookmarkSet(line);
        }
        return true;
    }

    if (options.query.isEmpty()) {
        errorMsg = QObject::tr("No filter criteria provided");
        return false;
    }

    if (options.useRegex) {
        QRegularExpression::PatternOptions regexOptions = QRegularExpression::NoPatternOption;
        if (!options.matchCase) {
            regexOptions |= QRegularExpression::CaseInsensitiveOption;
        }
        const QString pattern = options.wholeWord ? QStringLiteral("\\b%1\\b").arg(options.query) : options.query;
        const QRegularExpression re(pattern, regexOptions);
        if (!re.isValid()) {
            errorMsg = QObject::tr("Invalid regular expression: %1").arg(re.errorString());
            return false;
        }
        for (int line = 0; line < lineCount; ++line) {
            const QString lineStr = QString::fromUtf8(editor->getLine(line));
            lineHasMatch[line] = lineStr.contains(re);
        }
        return true;
    }

    if (options.wholeWord) {
        const QString escaped = QRegularExpression::escape(options.query);
        const QRegularExpression re(
            QStringLiteral("\\b%1\\b").arg(escaped),
            options.matchCase ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
        for (int line = 0; line < lineCount; ++line) {
            const QString lineStr = QString::fromUtf8(editor->getLine(line));
            lineHasMatch[line] = lineStr.contains(re);
        }
        return true;
    }

    const Qt::CaseSensitivity cs = options.matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive;
    for (int line = 0; line < lineCount; ++line) {
        const QString lineStr = QString::fromUtf8(editor->getLine(line));
        lineHasMatch[line] = lineStr.contains(options.query, cs);
    }
    return true;
}

void applyContextLines(int lineCount, int contextLines, const QVector<bool> &lineHasMatch, QVector<bool> &visible) {
    if (contextLines <= 0) {
        return;
    }
    for (int line = 0; line < lineCount; ++line) {
        if (!lineHasMatch[line]) {
            continue;
        }
        const int start = std::max(0, line - contextLines);
        const int end = std::min(lineCount - 1, line + contextLines);
        for (int c = start; c <= end; ++c) {
            visible[c] = true;
        }
    }
}

void hideNonVisibleLines(ScintillaNext *editor, int lineCount, const QVector<bool> &visible) {
    editor->showLines(0, lineCount - 1);
    int rangeStart = -1;
    for (int line = 0; line < lineCount; ++line) {
        if (!visible[line]) {
            if (rangeStart == -1) {
                rangeStart = line;
            }
        } else if (rangeStart != -1) {
            editor->hideLines(rangeStart, line - 1);
            rangeStart = -1;
        }
    }
    if (rangeStart != -1) {
        editor->hideLines(rangeStart, lineCount - 1);
    }
}
}  // namespace

FocusedView::FilterResult FocusedView::applyFilter(ScintillaNext *editor, const FilterOptions &options) {
    if (!editor) {
        return {false, 0, 0, 0, QString(), tr("No active editor")};
    }

    const auto lineCount = static_cast<int>(editor->lineCount());
    if (lineCount <= 0) {
        return {false, 0, 0, 0, QString(), tr("Document is empty")};
    }

    m_lastOptions = options;
    QVector<bool> lineHasMatch(lineCount, false);
    QString errorMsg;

    if (!computeLineMatches(editor, options, lineCount, lineHasMatch, errorMsg)) {
        return {false, lineCount, 0, lineCount, options.query, errorMsg};
    }

    QVector<bool> visible = lineHasMatch;
    applyContextLines(lineCount, options.contextLines, lineHasMatch, visible);

    if (options.invert) {
        for (int line = 0; line < lineCount; ++line) {
            visible[line] = !visible[line];
        }
    }

    int matchedCount = 0;
    int visibleCount = 0;
    for (int line = 0; line < lineCount; ++line) {
        if (lineHasMatch[line]) {
            matchedCount++;
        }
        if (visible[line]) {
            visibleCount++;
        }
    }

    if (visibleCount == 0) {
        const QString msg = options.bookmarksOnly
                                ? tr("No bookmarked lines found. Filter not applied.")
                                : tr("No matching lines found for '%1'. Filter not applied.").arg(options.query);
        return {false, lineCount, 0, lineCount, options.query, msg};
    }

    ensureHiddenLineStyle(editor);
    hideNonVisibleLines(editor, lineCount, visible);

    // Ensure caret is on a visible line
    if (const auto curLine = static_cast<int>(editor->lineFromPosition(editor->currentPos()));
        !editor->lineVisible(curLine)) {
        if (const auto it = std::ranges::find(visible, true); it != visible.end()) {
            editor->gotoLine(static_cast<sptr_t>(std::distance(visible.begin(), it)));
        }
    }

    m_editorOptions[editor] = options;
    m_lastOptions = options;
    editor->QObject::setProperty("FocusedViewActive", true);

    FilterResult res;
    res.active = true;
    res.totalLines = lineCount;
    res.matchedLines = matchedCount;
    res.visibleLines = visibleCount;
    res.filterQuery = options.bookmarksOnly ? tr("[Bookmarks]") : options.query;
    res.message =
        tr("Focused View: %1 / %2 lines visible (%3 matched)").arg(visibleCount).arg(lineCount).arg(matchedCount);

    emit filterApplied(editor, res);
    return res;
}

FocusedView::FilterResult FocusedView::clearFilter(ScintillaNext *editor) {
    if (!editor) {
        return {false, 0, 0, 0, QString(), tr("No active editor")};
    }

    const auto lineCount = static_cast<int>(editor->lineCount());
    if (lineCount > 0) {
        editor->showLines(0, lineCount - 1);
    }

    m_editorOptions.remove(editor);
    editor->QObject::setProperty("FocusedViewActive", false);

    FilterResult res;
    res.active = false;
    res.totalLines = lineCount;
    res.matchedLines = lineCount;
    res.visibleLines = lineCount;
    res.message = tr("Focused View disabled: all %1 lines visible").arg(lineCount);

    emit filterCleared(editor);
    return res;
}

FocusedView::FilterResult FocusedView::filterBySelectionOrWord(ScintillaNext *editor, bool invert) {
    if (!editor)
        return {false, 0, 0, 0, QString(), tr("No active editor")};

    int selectionStart = static_cast<int>(editor->selectionStart());
    int selectionEnd = static_cast<int>(editor->selectionEnd());
    QString query;

    if (selectionStart != selectionEnd) {
        if ((selectionEnd - selectionStart) <= 1024) {
            query = QString::fromUtf8(editor->get_text_range(selectionStart, selectionEnd)).trimmed();
        }
    } else {
        const int curPos = static_cast<int>(editor->currentPos());
        const int wStart = static_cast<int>(editor->wordStartPosition(curPos, true));
        const int wEnd = static_cast<int>(editor->wordEndPosition(curPos, true));
        if (wStart != wEnd) {
            query = QString::fromUtf8(editor->get_text_range(wStart, wEnd)).trimmed();
        }
    }

    if (query.isEmpty()) {
        return {false,     static_cast<int>(editor->lineCount()), 0, static_cast<int>(editor->lineCount()),
                QString(), tr("No text selected or under cursor")};
    }

    FilterOptions opts;
    opts.query = query;
    opts.matchCase = false;
    opts.wholeWord = false;
    opts.useRegex = false;
    opts.invert = invert;
    opts.bookmarksOnly = false;
    opts.contextLines = 0;

    return applyFilter(editor, opts);
}

FocusedView::FilterResult FocusedView::filterByBookmarks(ScintillaNext *editor, bool invert) {
    if (!editor)
        return {false, 0, 0, 0, QString(), tr("No active editor")};

    FilterOptions opts;
    opts.query = QString();
    opts.matchCase = false;
    opts.wholeWord = false;
    opts.useRegex = false;
    opts.invert = invert;
    opts.bookmarksOnly = true;
    opts.contextLines = 0;

    return applyFilter(editor, opts);
}
