/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) and Notepad3 (rizonesoft/notepad3)
 * case conversion routines (Upper, Lower, Invert, Title, Sentence case)
 * with multi-selection support for SparkNotePad.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "CaseConverter.h"
#if !defined(UNIT_TESTING)
#include "ScintillaNext.h"
#endif
#if !defined(UNIT_TESTING)
#include "UndoAction.h"
#endif

#include <QCoreApplication>
#include <vector>
#include <algorithm>

QString CaseConverter::toUpper(const QString &text) {
    return text.toUpper();
}

QString CaseConverter::toLower(const QString &text) {
    return text.toLower();
}

QString CaseConverter::toInvertCase(const QString &text) {
    QString result;
    result.reserve(text.size());
    for (const QChar &ch : text) {
        if (ch.isUpper()) {
            result.append(ch.toLower());
        } else if (ch.isLower()) {
            result.append(ch.toUpper());
        } else {
            result.append(ch);
        }
    }
    return result;
}

QString CaseConverter::toTitleCase(const QString &text) {
    QString result;
    result.reserve(text.size());
    bool inWord = false;

    for (int i = 0; i < text.size(); ++i) {
        const QChar ch = text.at(i);
        if (ch.isLetterOrNumber()) {
            if (!inWord) {
                result.append(ch.toUpper());
                inWord = true;
            } else {
                result.append(ch.toLower());
            }
        } else {
            // Check for apostrophe inside a word (e.g. don't, it's, O'Connor)
            if (ch == '\'' && inWord && (i + 1 < text.size()) && text.at(i + 1).isLetter()) {
                result.append(ch);
                // inWord remains true
            } else {
                result.append(ch);
                inWord = false;
            }
        }
    }
    return result;
}

QString CaseConverter::toSentenceCase(const QString &text) {
    QString result;
    result.reserve(text.size());
    bool startOfSentence = true;

    for (int i = 0; i < text.size(); ++i) {
        const QChar ch = text.at(i);
        if (ch.isLetter()) {
            if (startOfSentence) {
                result.append(ch.toUpper());
                startOfSentence = false;
            } else {
                result.append(ch.toLower());
            }
        } else {
            result.append(ch);
            if (ch == '.' || ch == '!' || ch == '?' || ch == '\r' || ch == '\n') {
                startOfSentence = true;
            }
        }
    }
    return result;
}

QString CaseConverter::convertText(const QString &text, CaseType caseType) {
    switch (caseType) {
        case CaseType::UpperCase:
            return toUpper(text);
        case CaseType::LowerCase:
            return toLower(text);
        case CaseType::InvertCase:
            return toInvertCase(text);
        case CaseType::TitleCase:
            return toTitleCase(text);
        case CaseType::SentenceCase:
            return toSentenceCase(text);
    }
    return text;
}

#if !defined(UNIT_TESTING)
CaseConverter::ConvertResult CaseConverter::convertEditor(ScintillaNext *editor, CaseType caseType) {
    ConvertResult res;
    if (!editor) {
        res.errorMessage = QCoreApplication::translate("CaseConverter", "No active editor.");
        return res;
    }

    if (editor->readOnly()) {
        res.errorMessage = QCoreApplication::translate("CaseConverter", "Document is read-only.");
        return res;
    }

    struct SelectionRange {
        int index = 0;
        sptr_t start = 0;
        sptr_t end = 0;
        bool caretAtEnd = false;
    };

    std::vector<SelectionRange> ranges;
    const sptr_t numSelections = editor->selections();

    for (sptr_t i = 0; i < numSelections; ++i) {
        sptr_t start = editor->selectionNStart(i);
        sptr_t end = editor->selectionNEnd(i);
        sptr_t caret = editor->selectionNCaret(i);
        sptr_t anchor = editor->selectionNAnchor(i);
        if (end > start) {
            ranges.push_back({static_cast<int>(i), start, end, caret >= anchor});
        }
    }

    bool wasWordExpanded = false;
    if (ranges.empty()) {
        // No explicit selection; expand to word under caret
        sptr_t pos = editor->currentPos();
        sptr_t wStart = editor->wordStartPosition(pos, true);
        sptr_t wEnd = editor->wordEndPosition(pos, true);
        if (wEnd > wStart) {
            ranges.push_back({0, wStart, wEnd, true});
            wasWordExpanded = true;
        }
    }

    if (ranges.empty()) {
        res.errorMessage = QCoreApplication::translate("CaseConverter", "No text selected or under cursor.");
        return res;
    }

    // Process ranges sorted in descending order by start position
    // so any byte length changes don't invalidate preceding ranges
    std::sort(ranges.begin(), ranges.end(),
              [](const SelectionRange &a, const SelectionRange &b) { return a.start > b.start; });

    struct UpdatedSel {
        int index = 0;
        sptr_t newStart = 0;
        sptr_t newEnd = 0;
        bool caretAtEnd = false;
    };
    std::vector<UpdatedSel> updated;
    updated.reserve(ranges.size());

    const UndoAction ua(editor);

    for (const auto &r : ranges) {
        sptr_t len = r.end - r.start;
        QByteArray originalBytes(len + 1, '\0');
        editor->getRange(r.start, r.end, originalBytes.data());
        originalBytes.resize(len);

        QString originalStr = QString::fromUtf8(originalBytes);
        QString convertedStr = convertText(originalStr, caseType);

        if (convertedStr != originalStr) {
            QByteArray newBytes = convertedStr.toUtf8();
            editor->setTargetRange(r.start, r.end);
            editor->replaceTarget(newBytes.length(), newBytes.constData());

            sptr_t newEnd = r.start + static_cast<sptr_t>(newBytes.length());
            updated.push_back({r.index, r.start, newEnd, r.caretAtEnd});
            res.charactersChanged += static_cast<int>(originalStr.length());
        } else {
            updated.push_back({r.index, r.start, r.end, r.caretAtEnd});
        }
        res.selectionsConverted++;
    }

    // Restore selections
    if (!updated.empty()) {
        if (wasWordExpanded || updated.size() == 1) {
            const auto &s = updated[0];
            if (s.caretAtEnd) {
                editor->setSelection(s.newEnd, s.newStart);
            } else {
                editor->setSelection(s.newStart, s.newEnd);
            }
        } else {
            // Restore multiple selections
            for (const auto &s : updated) {
                if (s.caretAtEnd) {
                    editor->setSelectionNCaret(s.index, s.newEnd);
                    editor->setSelectionNAnchor(s.index, s.newStart);
                } else {
                    editor->setSelectionNCaret(s.index, s.newStart);
                    editor->setSelectionNAnchor(s.index, s.newEnd);
                }
            }
        }
    }

    res.success = true;
    QString caseName;
    switch (caseType) {
        case CaseType::UpperCase:
            caseName = QCoreApplication::translate("CaseConverter", "UPPER CASE");
            break;
        case CaseType::LowerCase:
            caseName = QCoreApplication::translate("CaseConverter", "lower case");
            break;
        case CaseType::InvertCase:
            caseName = QCoreApplication::translate("CaseConverter", "iNVERT cASE");
            break;
        case CaseType::TitleCase:
            caseName = QCoreApplication::translate("CaseConverter", "Title Case");
            break;
        case CaseType::SentenceCase:
            caseName = QCoreApplication::translate("CaseConverter", "Sentence case");
            break;
    }

    res.message = QCoreApplication::translate("CaseConverter", "Converted %1 selection(s) to %2.")
                      .arg(res.selectionsConverted)
                      .arg(caseName);
    return res;
}

CaseConverter::ConvertResult CaseConverter::upperCase(ScintillaNext *editor) {
    return convertEditor(editor, CaseType::UpperCase);
}

CaseConverter::ConvertResult CaseConverter::lowerCase(ScintillaNext *editor) {
    return convertEditor(editor, CaseType::LowerCase);
}

CaseConverter::ConvertResult CaseConverter::invertCase(ScintillaNext *editor) {
    return convertEditor(editor, CaseType::InvertCase);
}

CaseConverter::ConvertResult CaseConverter::titleCase(ScintillaNext *editor) {
    return convertEditor(editor, CaseType::TitleCase);
}

CaseConverter::ConvertResult CaseConverter::sentenceCase(ScintillaNext *editor) {
    return convertEditor(editor, CaseType::SentenceCase);
}

#endif
