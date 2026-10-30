/*
 * This file is part of SparkNotePad.
 *
 * Enhanced PCRE2 regular expression search & replace engine with capture group
 * substitution (\1..\9, $1..$9, \0, &), dot matches newline, and backward search.
 * Adapted from Scintilla and Notepad++ PCRE2 regex engine concepts.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "QRegexSearch.h"

#include <QtGlobal>
#include <QRegularExpression>

using namespace Scintilla;

#ifdef SCI_OWNREGEX
RegexSearchBase *Scintilla::Internal::CreateRegexSearch(CharClassify *charClassTable) {
    Q_UNUSED(charClassTable);

    qInfo(Q_FUNC_INFO);

    return new QRegexSearch();
}
#endif

QRegexSearch::QRegexSearch() {}

Sci::Position QRegexSearch::FindText(Document *doc, Sci::Position minPos, Sci::Position maxPos, const char *s,
                                     bool caseSensitive, bool word, bool wordStart, Scintilla::FindOption flags,
                                     Sci::Position *length) {
    Q_UNUSED(caseSensitive);
    Q_UNUSED(word);
    Q_UNUSED(wordStart);

    if (minPos == maxPos)
        return -1;

    const bool forward = (minPos < maxPos);
    Sci::Position startPos = forward ? minPos : maxPos;
    Sci::Position endPos = forward ? maxPos : minPos;

    startPos = doc->MovePositionOutsideChar(startPos, 1, false);
    endPos = doc->MovePositionOutsideChar(endPos, -1, false);

    if (startPos >= endPos)
        return -1;

    auto options = QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption;

    if (!FlagSet(flags, FindOption::MatchCase))
        options |= QRegularExpression::CaseInsensitiveOption;

    if (FlagSet(flags, FindOption::Cxx11RegEx))
        options |= QRegularExpression::DotMatchesEverythingOption;

    QRegularExpression re(QString::fromUtf8(s), options);
    if (!re.isValid())
        return -1;

    const Sci::Position rangeLength = endPos - startPos;
    const QString utf8 = QString::fromUtf8(doc->RangePointer(startPos, rangeLength), rangeLength);

    if (forward) {
        QRegularExpressionMatch m =
            re.match(utf8, 0, QRegularExpression::NormalMatch, QRegularExpression::NoMatchOption);
        if (!m.hasMatch())
            return -1;

        match = std::move(m);
        const Sci::Position positionStart = doc->GetRelativePositionUTF16(startPos, match.capturedStart(0));
        const Sci::Position positionEnd = doc->GetRelativePositionUTF16(positionStart, match.capturedLength(0));
        *length = positionEnd - positionStart;
        return positionStart;
    } else {
        // Backwards search: iterate all matches in range and select the last one
        auto it = re.globalMatch(utf8);
        QRegularExpressionMatch lastMatch;
        while (it.hasNext()) {
            lastMatch = it.next();
        }

        if (!lastMatch.hasMatch())
            return -1;

        match = std::move(lastMatch);
        const Sci::Position positionStart = doc->GetRelativePositionUTF16(startPos, match.capturedStart(0));
        const Sci::Position positionEnd = doc->GetRelativePositionUTF16(positionStart, match.capturedLength(0));
        *length = positionEnd - positionStart;
        return positionStart;
    }
}

const char *QRegexSearch::SubstituteByPosition(Document *doc, const char *text, Sci::Position *length) {
    Q_UNUSED(doc);

    if (!match.isValid() || !match.hasMatch()) {
        return text;
    }

    // Full PCRE2 & Notepad++ replacement syntax:
    // \1..\9 or $1..$9 for captured groups
    // \0, $0, & for full match
    // \t (tab), \n (newline), \r (CR), \\ (literal backslash), \$ (literal dollar)
    QString result;
    const int inLen = static_cast<int>(*length);
    int i = 0;
    while (i < inLen) {
        const char ch = text[i];
        if ((ch == '\\' || ch == '$') && i + 1 < inLen) {
            const char nextCh = text[i + 1];
            if (nextCh >= '0' && nextCh <= '9') {
                const int groupNum = nextCh - '0';
                result += match.captured(groupNum);
                i += 2;
                continue;
            } else if (nextCh == '&') {
                result += match.captured(0);
                i += 2;
                continue;
            } else if (ch == '\\') {
                if (nextCh == 'n') {
                    result += QLatin1Char('\n');
                    i += 2;
                    continue;
                } else if (nextCh == 'r') {
                    result += QLatin1Char('\r');
                    i += 2;
                    continue;
                } else if (nextCh == 't') {
                    result += QLatin1Char('\t');
                    i += 2;
                    continue;
                } else if (nextCh == '\\') {
                    result += QLatin1Char('\\');
                    i += 2;
                    continue;
                } else if (nextCh == '$') {
                    result += QLatin1Char('$');
                    i += 2;
                    continue;
                }
            } else if (ch == '$' && nextCh == '$') {
                result += QLatin1Char('$');
                i += 2;
                continue;
            }
        }
        result += QLatin1Char(ch);
        i++;
    }

    if (substituted) {
        delete substituted;
    }

    substituted = new QByteArray(result.toUtf8());
    *length = substituted->length();
    return substituted->data();
}
