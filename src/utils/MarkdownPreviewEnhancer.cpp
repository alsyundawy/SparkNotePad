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

#include "MarkdownPreviewEnhancer.h"
#include <QRegularExpression>
#include <vector>

QString MarkdownPreviewEnhancer::sanitizeForPreview(const QString &text) {
    return text.toHtmlEscaped();
}

bool MarkdownPreviewEnhancer::isMarkdownPath(const QString &path) {
    static const QRegularExpression mdRegex(
        QStringLiteral(R"(\.(?:markdown|mdown|mkdn|md|mkd|mdwn|mdtxt|mdtext|text)(?:#.*|\?.*)?$)"),
        QRegularExpression::CaseInsensitiveOption);
    return mdRegex.match(path).hasMatch();
}

bool MarkdownPreviewEnhancer::isHtmlPath(const QString &path) {
    static const QRegularExpression htmlRegex(
        QStringLiteral(R"(\.(?:html|htm|shtml|xhtml|xht|dhtml)(?:#.*|\?.*)?$)"),
        QRegularExpression::CaseInsensitiveOption);
    return htmlRegex.match(path).hasMatch();
}

bool MarkdownPreviewEnhancer::isMarkdownOrHtmlPath(const QString &path) {
    return isMarkdownPath(path) || isHtmlPath(path);
}

QString MarkdownPreviewEnhancer::mathAndMermaidCss(bool isDark) {
    if (isDark) {
        return QStringLiteral(
            ".mermaid-diagram {\n"
            "    background-color: #161b22;\n"
            "    border: 1px solid #30363d;\n"
            "    border-radius: 6px;\n"
            "    margin: 16px 0;\n"
            "    padding: 12px;\n"
            "}\n"
            ".mermaid-header {\n"
            "    font-weight: 600;\n"
            "    color: #58a6ff;\n"
            "    font-size: 12px;\n"
            "    margin-bottom: 8px;\n"
            "    letter-spacing: 0.5px;\n"
            "}\n"
            ".mermaid-code {\n"
            "    background: transparent;\n"
            "    color: #e6edf3;\n"
            "    font-family: 'JetBrains Mono', SFMono-Regular, Consolas, Menlo, monospace;\n"
            "    font-size: 13px;\n"
            "    margin: 0;\n"
            "    padding: 0;\n"
            "}\n"
            ".math-block {\n"
            "    background-color: #161b22;\n"
            "    border: 1px solid #30363d;\n"
            "    border-radius: 6px;\n"
            "    padding: 12px 16px;\n"
            "    margin: 16px 0;\n"
            "    text-align: center;\n"
            "    font-family: KaTeX_Math, 'Cambria Math', STIXGeneral, serif;\n"
            "    font-size: 15px;\n"
            "    color: #79c0ff;\n"
            "    overflow-x: auto;\n"
            "}\n"
            ".math-inline {\n"
            "    background-color: rgba(110, 118, 129, 0.2);\n"
            "    padding: 2px 6px;\n"
            "    border-radius: 4px;\n"
            "    font-family: KaTeX_Math, 'Cambria Math', STIXGeneral, serif;\n"
            "    font-size: 13px;\n"
            "    color: #79c0ff;\n"
            "}\n");
    }

    return QStringLiteral(
        ".mermaid-diagram {\n"
        "    background-color: #f6f8fa;\n"
        "    border: 1px solid #d0d7de;\n"
        "    border-radius: 6px;\n"
        "    margin: 16px 0;\n"
        "    padding: 12px;\n"
        "}\n"
        ".mermaid-header {\n"
        "    font-weight: 600;\n"
        "    color: #0969da;\n"
        "    font-size: 12px;\n"
        "    margin-bottom: 8px;\n"
        "    letter-spacing: 0.5px;\n"
        "}\n"
        ".mermaid-code {\n"
        "    background: transparent;\n"
        "    color: #1f2328;\n"
        "    font-family: 'JetBrains Mono', SFMono-Regular, Consolas, Menlo, monospace;\n"
        "    font-size: 13px;\n"
        "    margin: 0;\n"
        "    padding: 0;\n"
        "}\n"
        ".math-block {\n"
        "    background-color: #f6f8fa;\n"
        "    border: 1px solid #d0d7de;\n"
        "    border-radius: 6px;\n"
        "    padding: 12px 16px;\n"
        "    margin: 16px 0;\n"
        "    text-align: center;\n"
        "    font-family: KaTeX_Math, 'Cambria Math', STIXGeneral, serif;\n"
        "    font-size: 15px;\n"
        "    color: #0550ae;\n"
        "    overflow-x: auto;\n"
        "}\n"
        ".math-inline {\n"
        "    background-color: rgba(175, 184, 193, 0.2);\n"
        "    padding: 2px 6px;\n"
        "    border-radius: 4px;\n"
        "    font-family: KaTeX_Math, 'Cambria Math', STIXGeneral, serif;\n"
        "    font-size: 13px;\n"
        "    color: #0550ae;\n"
        "}\n");
}

QString MarkdownPreviewEnhancer::processMermaidBlocks(const QString &content, bool isDark) {
    (void)isDark;
    static const QRegularExpression mermaidRegex(QStringLiteral("```mermaid\\s*\\n([\\s\\S]*?)\\n```"),
                                                 QRegularExpression::CaseInsensitiveOption);

    QString result = content;
    auto matchIterator = mermaidRegex.globalMatch(content);
    qsizetype offset = 0;

    while (matchIterator.hasNext()) {
        const QRegularExpressionMatch match = matchIterator.next();
        const QString diagramSource = match.captured(1).trimmed();
        const QString escapedSource = sanitizeForPreview(diagramSource);

        const QString replacement = QStringLiteral(
                                        "<div class=\"mermaid-diagram\">\n"
                                        "<div class=\"mermaid-header\">📊 Mermaid Diagram</div>\n"
                                        "<pre class=\"mermaid-code\">%1</pre>\n"
                                        "</div>\n")
                                        .arg(escapedSource);

        const qsizetype start = match.capturedStart() + offset;
        const qsizetype length = match.capturedLength();
        result.replace(start, length, replacement);
        offset += replacement.length() - length;
    }

    return result;
}

QString MarkdownPreviewEnhancer::processMathExpressions(const QString &content) {
    QString result = content;

    // 1. Process block math: $$ ... $$
    static const QRegularExpression blockMathRegex(QStringLiteral(R"(\$\$\s*\n?([\s\S]*?)\n?\s*\$\$)"));

    auto blockIt = blockMathRegex.globalMatch(result);
    qsizetype offset = 0;
    while (blockIt.hasNext()) {
        const QRegularExpressionMatch match = blockIt.next();
        const QString mathSource = match.captured(1).trimmed();
        const QString escaped = sanitizeForPreview(mathSource);

        const QString replacement =
            QStringLiteral("<div class=\"math-block\"><span class=\"math-tex\">%1</span></div>").arg(escaped);

        const qsizetype start = match.capturedStart() + offset;
        const qsizetype length = match.capturedLength();
        result.replace(start, length, replacement);
        offset += replacement.length() - length;
    }

    // 2. Process inline math: $...$ (avoid escaped \$ and currency like $50 or $100)
    static const QRegularExpression inlineMathRegex(
        QStringLiteral(R"((?<![\\a-zA-Z0-9])\$(?![0-9\s\$])([^\$\n]+?)(?<![\\\s])\$(?![a-zA-Z0-9]))"));

    auto inlineIt = inlineMathRegex.globalMatch(result);
    offset = 0;
    while (inlineIt.hasNext()) {
        const QRegularExpressionMatch match = inlineIt.next();
        const QString mathSource = match.captured(1).trimmed();

        bool isNumber = false;
        (void)mathSource.toDouble(&isNumber);
        if (!isNumber) {
            const QString escaped = sanitizeForPreview(mathSource);
            const QString replacement = QStringLiteral("<span class=\"math-inline\">%1</span>").arg(escaped);

            const qsizetype start = match.capturedStart() + offset;
            const qsizetype length = match.capturedLength();
            result.replace(start, length, replacement);
            offset += replacement.length() - length;
        }
    }

    return result;
}

QString MarkdownPreviewEnhancer::processTaskLists(const QString &content) {
    static const QRegularExpression taskRegex(QStringLiteral(R"((?:^|\n)([ \t]*[-*+]\s+)\[([ xX])\]\s+)"));

    QString result = content;
    auto it = taskRegex.globalMatch(content);
    qsizetype offset = 0;

    while (it.hasNext()) {
        const QRegularExpressionMatch match = it.next();
        const QString prefix = match.captured(1);
        const QString mark = match.captured(2).trimmed().toLower();
        const bool isChecked = (mark == QStringLiteral("x"));

        const QString checkbox = isChecked ? QStringLiteral("<input type=\"checkbox\" checked disabled> ")
                                           : QStringLiteral("<input type=\"checkbox\" disabled> ");
        QString replacement = prefix + checkbox;
        if (match.captured(0).startsWith(QLatin1Char('\n'))) {
            replacement.prepend(QLatin1Char('\n'));
        }

        const qsizetype start = match.capturedStart() + offset;
        const qsizetype length = match.capturedLength();
        result.replace(start, length, replacement);
        offset += replacement.length() - length;
    }

    return result;
}

QString MarkdownPreviewEnhancer::processFootnotes(const QString &content) {
    struct FootnoteDef {
        QString id;
        QString text;
    };
    std::vector<FootnoteDef> definitions;

    static const QRegularExpression fnDefRegex(
        QStringLiteral(R"((?:^|\n)\[\^([^\]]+)\]:\s*([^\n]+(?:\n(?!\n|\[\^|\S).*)*))"));

    QString body = content;
    auto matchIt = fnDefRegex.globalMatch(body);
    qsizetype offset = 0;

    while (matchIt.hasNext()) {
        const QRegularExpressionMatch match = matchIt.next();
        FootnoteDef def;
        def.id = match.captured(1).trimmed();
        def.text = match.captured(2).trimmed();
        definitions.push_back(def);

        const qsizetype start = match.capturedStart() + offset;
        const qsizetype length = match.capturedLength();
        body.replace(start, length, QStringLiteral("\n"));
        offset += 1 - length;
    }

    if (definitions.empty()) {
        return body;
    }

    // Replace reference links: [^id]
    static const QRegularExpression fnRefRegex(QStringLiteral(R"(\[\^([^\]]+)\])"));
    matchIt = fnRefRegex.globalMatch(body);
    offset = 0;
    while (matchIt.hasNext()) {
        const QRegularExpressionMatch match = matchIt.next();
        const QString id = match.captured(1).trimmed();
        const QString replacement =
            QStringLiteral("<sup class=\"footnote-ref\"><a href=\"#fn_%1\" id=\"fnref_%1\">[%1]</a></sup>").arg(id);

        const qsizetype start = match.capturedStart() + offset;
        const qsizetype length = match.capturedLength();
        body.replace(start, length, replacement);
        offset += replacement.length() - length;
    }

    // Append footnotes section
    auto footnotesHtml = QStringLiteral("\n\n<hr class=\"footnotes-sep\">\n<section class=\"footnotes\">\n<ol class=\"footnotes-list\">\n");
    for (const auto &def : definitions) {
        footnotesHtml +=
            QStringLiteral("<li id=\"fn_%1\" class=\"footnote-item\"><p>%2 <a href=\"#fnref_%1\" class=\"footnote-backref\">↩</a></p></li>\n")
                .arg(def.id, sanitizeForPreview(def.text));
    }
    footnotesHtml += QStringLiteral("</ol>\n</section>\n");

    return body + footnotesHtml;
}

QString MarkdownPreviewEnhancer::processExtensions(const QString &content) {
    // 1. Protect code fences and inline code with placeholders
    static const QRegularExpression codeRegex(QStringLiteral(R"((```[\s\S]*?```|`[^`\n]+`))"));
    std::vector<QString> codeSnippets;

    QString masked = content;
    auto codeIt = codeRegex.globalMatch(content);
    qsizetype offset = 0;

    while (codeIt.hasNext()) {
        const QRegularExpressionMatch match = codeIt.next();
        codeSnippets.push_back(match.captured(1));
        const QString token = QStringLiteral("\x1DCODE_%1\x1D").arg(codeSnippets.size() - 1);

        const qsizetype start = match.capturedStart() + offset;
        const qsizetype length = match.capturedLength();
        masked.replace(start, length, token);
        offset += token.length() - length;
    }

    // 2. Strikethrough: ~~del~~
    static const QRegularExpression strikeRegex(QStringLiteral(R"(~~(?=\S)(.+?)(?<=\S)~~)"));
    masked.replace(strikeRegex, QStringLiteral("<del>\\1</del>"));

    // 3. Mark / Highlight: ==mark==
    static const QRegularExpression markRegex(QStringLiteral(R"(==(?=\S)(.+?)(?<=\S)==)"));
    masked.replace(markRegex, QStringLiteral("<mark>\\1</mark>"));

    // 4. Insert: ++ins++
    static const QRegularExpression insRegex(QStringLiteral(R"(\+\+(?=\S)(.+?)(?<=\S)\+\+)"));
    masked.replace(insRegex, QStringLiteral("<ins>\\1</ins>"));

    // 5. Subscript: ~sub~ (single ~ not preceded or followed by ~)
    static const QRegularExpression subRegex(QStringLiteral(R"((?<!~)~(?!~|\s)([^~\s\n]+?)(?<!\s)~(?!~))"));
    masked.replace(subRegex, QStringLiteral("<sub>\\1</sub>"));

    // 6. Superscript: ^sup^ (single ^ not preceded or followed by ^)
    static const QRegularExpression supRegex(QStringLiteral(R"((?<!\^)\^(?!\^|\s)([^\^\s\n]+?)(?<!\s)\^(?!\^))"));
    masked.replace(supRegex, QStringLiteral("<sup>\\1</sup>"));

    // 7. Restore protected code blocks
    for (std::size_t i = 0; i < codeSnippets.size(); ++i) {
        const QString token = QStringLiteral("\x1DCODE_%1\x1D").arg(i);
        masked.replace(token, codeSnippets[i]);
    }

    return masked;
}

QString MarkdownPreviewEnhancer::enhanceMarkdown(const QString &rawMd, bool isDark) {
    QString processed = processMermaidBlocks(rawMd, isDark);
    processed = processMathExpressions(processed);
    processed = processTaskLists(processed);
    processed = processExtensions(processed);
    processed = processFootnotes(processed);
    return processed;
}
