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

class MarkdownPreviewEnhancer {
public:
    static QString enhanceMarkdown(const QString &rawMd, bool isDark);

    static QString renderMarkdownToHtml(const QString &rawMd, bool isDark);

    static QString enhanceCallouts(const QString &html, bool isDark);

    static QString processMermaidBlocks(const QString &content, bool isDark);

    static QString processMathExpressions(const QString &content);

    static QString processFootnotes(const QString &content);

    static QString processTaskLists(const QString &content);

    static QString processExtensions(const QString &content);

    static QString processTableOfContents(const QString &content);

    static QString processEmoji(const QString &content);

    static QString processHeadingsWithIds(const QString &html);

    static QString slugifyHeading(const QString &text);

    static QString sanitizeForPreview(const QString &text);

    static QString mathAndMermaidCss(bool isDark);

    static bool isMarkdownPath(const QString &path);

    static bool isHtmlPath(const QString &path);

    static bool isMarkdownOrHtmlPath(const QString &path);
};
