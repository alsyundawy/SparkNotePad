/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4, UrlThreshold) and Notepad3 URL features.
 * Sources: zufuliu/notepad4 & rizonesoft/Notepad3
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <QDesktopServices>
#include <QTimer>
#include <QUrl>

#include "URLFinder.h"
#include "UrlUtils.h"

URLFinder::URLFinder(ScintillaNext *editor) : EditorDecorator(editor), timer(new QTimer(this)), m_urlThresholdMB(5) {
    // Setup the indicator
    indicator = editor->allocateIndicator("url_finder");

    editor->indicSetStyle(indicator, INDIC_PLAIN);
    editor->indicSetFore(indicator, 0xFF0000);

    editor->indicSetHoverStyle(indicator, INDIC_DOTS);
    editor->indicSetHoverFore(indicator, 0xFF0000);

    // Resizing the window could reveal more text
    connect(editor, &ScintillaNext::resized, timer, qOverload<>(&QTimer::start));

    timer->setInterval(200);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &URLFinder::findURLs);

    connect(this, &EditorDecorator::stateChanged, this, [=, this](bool b) {
        if (b) {
            connect(editor, &ScintillaNext::resized, timer, qOverload<>(&QTimer::start));
            connect(editor, &ScintillaNext::reloaded, timer, qOverload<>(&QTimer::start));
            findURLs();
        } else {
            disconnect(editor, &ScintillaNext::resized, timer, qOverload<>(&QTimer::start));
            clearURLs();
        }
    });
}

void URLFinder::setUrlThreshold(int thresholdMB) {
    if (m_urlThresholdMB != thresholdMB) {
        m_urlThresholdMB = thresholdMB;
        findURLs();
    }
}

void URLFinder::findURLs() {
    clearURLs();

    if (!isEnabled()) {
        return;
    }

    if (UrlUtils::exceedsUrlThreshold(editor, m_urlThresholdMB)) {
        return;
    }

    int currentLine = editor->docLineFromVisible(editor->firstVisibleLine());
    int linesLeftToProcess = editor->linesOnScreen();
    const int flags = SCFIND_REGEXP;
    const QByteArray reg = QByteArray(UrlUtils::urlRegexPattern());

    while (linesLeftToProcess >= 0 && currentLine < editor->lineCount()) {
        // Should only happen if the line is hidden
        if (!editor->lineVisible(currentLine)) {
            currentLine++;
            continue;
        }

        const int startPos = editor->positionFromLine(currentLine);
        const int endPos = editor->lineEndPosition(currentLine);

        Sci_TextToFind ttf{{startPos, (Sci_PositionCR)endPos}, reg.constData(), {-1, -1}};
        while (editor->send(SCI_FINDTEXT, flags, (sptr_t)&ttf) != -1) {
            const int startUrl = ttf.chrgText.cpMin;
            int endUrl = ttf.chrgText.cpMax;

            if (startUrl > 0) {
                const int prevChar = static_cast<int>(editor->charAt(startUrl - 1));
                const int nextChar = static_cast<int>(editor->charAt(endUrl - 1));

                if ((prevChar == '(' && nextChar == ')') || (prevChar == '[' && nextChar == ']') ||
                    (prevChar == '<' && nextChar == '>') || (prevChar == '"' && nextChar == '"')) {
                    endUrl--;
                }
            }

            editor->indicatorFillRange(startUrl, endUrl - startUrl);
            ttf.chrg.cpMin = endUrl;
        }

        // If a line is wrapped, skip however many lines it takes up on the screen
        linesLeftToProcess -= editor->wrapCount(currentLine);

        // If the current line is a fold header and the fold is not expanded, skip
        if (((editor->foldLevel(currentLine) & SC_FOLDLEVELHEADERFLAG) == SC_FOLDLEVELHEADERFLAG) &&
            (!editor->foldExpanded(currentLine))) {
            currentLine = editor->lastChild(currentLine, -1) + 1;
        } else {
            currentLine++;
        }
    }
}

void URLFinder::clearURLs() {
    editor->setIndicatorCurrent(indicator);
    editor->indicatorClearRange(0, editor->length());
}

void URLFinder::notify(const Scintilla::NotificationData *pscn) {
    // Reprocess any time the content was updated or the editor was vertically scrolled
    if ((pscn->nmhdr.code == Scintilla::Notification::UpdateUI)) {
        if (FlagSet(pscn->updated, Scintilla::Update::VScroll)) {
            timer->start();
        }
    } else if (pscn->nmhdr.code == Scintilla::Notification::Modified) {
        if (FlagSet(pscn->modificationType, Scintilla::ModificationFlags::InsertText) ||
            FlagSet(pscn->modificationType, Scintilla::ModificationFlags::DeleteText)) {
            timer->start();
        }
    } else if (pscn->nmhdr.code == Scintilla::Notification::Zoom) {
        timer->start();
    } else if (pscn->nmhdr.code == Scintilla::Notification::IndicatorClick &&
               FlagSet(pscn->modifiers, Scintilla::KeyMod::Ctrl)) {
        const int indicators = editor->indicatorAllOnFor(pscn->position);

        if (indicators & (1 << indicator)) {
            const int indicatorStart = editor->indicatorStart(indicator, pscn->position);
            const int indicatorEnd = editor->indicatorEnd(indicator, pscn->position);

            QUrl url(editor->get_text_range(indicatorStart, indicatorEnd));

            if (url.isValid()) {
                qInfo("URL hotspot click: \"%s\"", editor->get_text_range(indicatorStart, indicatorEnd).constData());
                QDesktopServices::openUrl(url);
            } else {
                qWarning("Invalid url \"%s\"", qUtf8Printable(url.errorString()));
            }
        }
    }
}

bool URLFinder::isURL(int position) const {
    const int indicators = editor->indicatorAllOnFor(position);
    return indicators & (1 << indicator);
}

void URLFinder::openURLAt(int position) const {
    if (!isURL(position))
        return;

    const int indicatorStart = editor->indicatorStart(indicator, position);
    const int indicatorEnd = editor->indicatorEnd(indicator, position);

    QUrl url(editor->get_text_range(indicatorStart, indicatorEnd));

    if (url.isValid()) {
        qInfo("Open URL: \"%s\"", editor->get_text_range(indicatorStart, indicatorEnd).constData());
        QDesktopServices::openUrl(url);
    } else {
        qWarning("Invalid url \"%s\"", qUtf8Printable(url.errorString()));
    }
}

void URLFinder::copyURLToClipboard(int position) const {
    const int indicatorStart = editor->indicatorStart(indicator, position);
    const int indicatorEnd = editor->indicatorEnd(indicator, position);

    QUrl url(editor->get_text_range(indicatorStart, indicatorEnd));

    if (url.isValid()) {
        qInfo("Copy URL to clipboard: \"%s\"", editor->get_text_range(indicatorStart, indicatorEnd).constData());
        editor->copyRange(indicatorStart, indicatorEnd);
    } else {
        qWarning("Invalid url \"%s\"", qUtf8Printable(url.errorString()));
    }
}
