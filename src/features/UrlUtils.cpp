/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4, UrlThreshold) and Notepad3 URL features.
 * Sources: zufuliu/notepad4 & rizonesoft/Notepad3
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "UrlUtils.h"
#include "ScintillaNext.h"
#include "URLFinder.h"

#include <QDesktopServices>
#include <QApplication>
#include <QClipboard>

const char *UrlUtils::urlRegexPattern() {
    return R"(\b(?:https?|ftps?|file)://[-a-zA-Z0-9@:%._\+~#=]{1,256}(?:\.[a-zA-Z0-9()]{1,6})?\b(?:[-a-zA-Z0-9()@:%_\+.~#?&\/=]*)|mailto:[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})";
}

bool UrlUtils::exceedsUrlThreshold(const ScintillaNext *editor, int thresholdMB) {
    if (!editor)
        return true;
    if (thresholdMB <= 0)
        return true;  // 0 disables URL highlighting

    const qint64 thresholdBytes = static_cast<qint64>(thresholdMB) * 1024 * 1024;
    return editor->length() > thresholdBytes;
}

QString UrlUtils::getUrlAtPosition(ScintillaNext *editor, int position) {
    if (editor == nullptr) {
        return QString();
    }
    if (const auto *uf = editor->findChild<URLFinder *>(QString(), Qt::FindDirectChildrenOnly);
        uf == nullptr || !uf->isEnabled() || !uf->isURL(position)) {
        return QString();
    }

    const int indicator = editor->allocateIndicator("url_finder");
    const auto start = static_cast<int>(editor->indicatorStart(indicator, position));
    if (const auto end = static_cast<int>(editor->indicatorEnd(indicator, position)); start >= 0 && end > start) {
        return QString::fromUtf8(editor->get_text_range(start, end));
    }
    return QString();
}

bool UrlUtils::openUrlAtPosition(const ScintillaNext *editor, int position) {
    if (editor == nullptr) {
        return false;
    }
    if (const auto *uf = editor->findChild<URLFinder *>(QString(), Qt::FindDirectChildrenOnly);
        uf != nullptr && uf->isEnabled() && uf->isURL(position)) {
        uf->openURLAt(position);
        return true;
    }
    return false;
}

bool UrlUtils::copyUrlAtPosition(const ScintillaNext *editor, int position) {
    if (editor == nullptr) {
        return false;
    }
    if (const auto *uf = editor->findChild<URLFinder *>(QString(), Qt::FindDirectChildrenOnly);
        uf != nullptr && uf->isEnabled() && uf->isURL(position)) {
        uf->copyURLToClipboard(position);
        return true;
    }
    return false;
}
