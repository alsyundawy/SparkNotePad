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
#include <QCoreApplication>
#include <QRegularExpression>
#include <array>
#include <vector>

namespace {

struct CalloutDef {
    const char *tag;
    const char *icon;
    const char *title;
    const char *colorLight;
    const char *colorDark;
    const char *bgLight;
    const char *bgDark;
};

constexpr std::array<CalloutDef, 5> s_callouts = {{
    {"[!NOTE]", "ℹ️", "Note", "#0969da", "#2f81f7", "rgba(9, 105, 218, 0.08)", "rgba(47, 129, 247, 0.12)"},
    {"[!TIP]", "💡", "Tip", "#1a7f37", "#3fb950", "rgba(26, 127, 55, 0.08)", "rgba(63, 185, 80, 0.12)"},
    {"[!IMPORTANT]", "🟣", "Important", "#8250df", "#a371f7", "rgba(130, 80, 223, 0.08)", "rgba(163, 113, 247, 0.12)"},
    {"[!WARNING]", "⚠️", "Warning", "#9a6700", "#d29922", "rgba(154, 103, 0, 0.08)", "rgba(210, 153, 34, 0.12)"},
    {"[!CAUTION]", "🛑", "Caution", "#cf222e", "#f85149", "rgba(207, 34, 46, 0.08)", "rgba(248, 81, 73, 0.12)"},
}};

QString cleanCalloutInner(const QString &raw) {
    QString inner = raw.trimmed();
    bool stripped = true;
    while (stripped) {
        stripped = false;
        if (inner.startsWith(QStringLiteral("<br />"), Qt::CaseInsensitive)) {
            inner = inner.mid(6).trimmed();
            stripped = true;
        } else if (inner.startsWith(QStringLiteral("<br>"), Qt::CaseInsensitive)) {
            inner = inner.mid(4).trimmed();
            stripped = true;
        } else if (inner.startsWith(QStringLiteral("<br/>"), Qt::CaseInsensitive)) {
            inner = inner.mid(5).trimmed();
            stripped = true;
        } else if (inner.startsWith(QStringLiteral("<p>"), Qt::CaseInsensitive)) {
            inner = inner.mid(3).trimmed();
            stripped = true;
        }
    }
    if (inner.endsWith(QStringLiteral("</p>"), Qt::CaseInsensitive)) {
        inner = inner.left(inner.length() - 4).trimmed();
    }
    return inner;
}

void replaceSingleCallout(QString &result, const CalloutDef &c, qsizetype &idx, bool isDark) {
    const QString marker = QString::fromUtf8(c.tag);
    const qsizetype bqStart = result.lastIndexOf(QStringLiteral("<blockquote"), idx);
    if (bqStart == -1 || bqStart < idx - 100) {
        idx += marker.length();
        return;
    }

    const qsizetype bqEnd = result.indexOf(QStringLiteral("</blockquote>"), idx);
    if (bqEnd == -1) {
        idx += marker.length();
        return;
    }

    const qsizetype contentStart = idx + marker.length();
    const QString inner = cleanCalloutInner(result.mid(contentStart, bqEnd - contentStart));
    const QString color = isDark ? QString::fromLatin1(c.colorDark) : QString::fromLatin1(c.colorLight);
    const QString bg = isDark ? QString::fromLatin1(c.bgDark) : QString::fromLatin1(c.bgLight);
    const QString replacement = QStringLiteral(
                                    "<div style=\"margin: 12px 0; padding: 10px 14px; border-left: 4px solid %1; "
                                    "background-color: %2; border-radius: 4px;\">"
                                    "<div style=\"font-weight: 600; color: %1; margin-bottom: 4px;\">%3 %4</div>"
                                    "<div>%5</div></div>")
                                    .arg(color, bg, QString::fromUtf8(c.icon),
                                         QCoreApplication::translate("MarkdownPreviewEnhancer", c.title), inner);

    result.replace(bqStart, (bqEnd + 13) - bqStart, replacement);
    idx = bqStart + replacement.length();
}

}  // namespace

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
    static const QRegularExpression htmlRegex(QStringLiteral(R"(\.(?:html|htm|shtml|xhtml|xht|dhtml)(?:#.*|\?.*)?$)"),
                                              QRegularExpression::CaseInsensitiveOption);
    return htmlRegex.match(path).hasMatch();
}

bool MarkdownPreviewEnhancer::isMarkdownOrHtmlPath(const QString &path) {
    return isMarkdownPath(path) || isHtmlPath(path);
}

QString MarkdownPreviewEnhancer::slugifyHeading(const QString &text) {
    QString slug = text.toLower();
    static const QRegularExpression htmlTagRegex(QStringLiteral("<[^>]*>"));
    slug.remove(htmlTagRegex);
    static const QRegularExpression nonAlphaNum(QStringLiteral("[^a-z0-9]+"));
    slug.replace(nonAlphaNum, QStringLiteral("-"));
    while (slug.startsWith(QLatin1Char('-'))) {
        slug.remove(0, 1);
    }
    while (slug.endsWith(QLatin1Char('-'))) {
        slug.chop(1);
    }
    if (slug.isEmpty()) {
        slug = QStringLiteral("heading");
    }
    return slug;
}

QString MarkdownPreviewEnhancer::processHeadingsWithIds(const QString &html) {
    static const QRegularExpression headingRegex(QStringLiteral(R"(<h([1-6])([^>]*)>(.*?)</h\1>)"),
                                                 QRegularExpression::CaseInsensitiveOption);

    QString result = html;
    auto matchIt = headingRegex.globalMatch(html);
    qsizetype offset = 0;

    while (matchIt.hasNext()) {
        const QRegularExpressionMatch match = matchIt.next();
        const QString level = match.captured(1);
        const QString attrs = match.captured(2);
        const QString innerText = match.captured(3);

        if (!attrs.contains(QStringLiteral("id="), Qt::CaseInsensitive)) {
            const QString slug = slugifyHeading(innerText);
            const QString replacement = QStringLiteral("<h%1%2 id=\"%3\">%4</h%1>").arg(level, attrs, slug, innerText);

            const qsizetype start = match.capturedStart() + offset;
            const qsizetype length = match.capturedLength();
            result.replace(start, length, replacement);
            offset += replacement.length() - length;
        }
    }
    return result;
}

QString MarkdownPreviewEnhancer::enhanceCallouts(const QString &html, bool isDark) {
    QString result = html;
    for (const auto &c : s_callouts) {
        const QString marker = QString::fromUtf8(c.tag);
        qsizetype idx = 0;
        while ((idx = result.indexOf(marker, idx, Qt::CaseInsensitive)) != -1) {
            replaceSingleCallout(result, c, idx, isDark);
        }
    }
    return result;
}

QString MarkdownPreviewEnhancer::processTableOfContents(const QString &content) {
    if (!content.contains(QStringLiteral("[TOC]"), Qt::CaseInsensitive) &&
        !content.contains(QStringLiteral("{:toc}"), Qt::CaseInsensitive)) {
        return content;
    }

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

    static const QRegularExpression headingLineRegex(QStringLiteral(R"((?:^|\n)(#{1,6})\s+([^\n]+))"));
    auto headingIt = headingLineRegex.globalMatch(masked);

    QString tocHtml = QStringLiteral(
        "<div class=\"table-of-contents\">\n"
        "<div class=\"toc-title\">📖 Table of Contents</div>\n"
        "<ul class=\"toc-list\">\n");

    int count = 0;
    while (headingIt.hasNext()) {
        const QRegularExpressionMatch match = headingIt.next();
        const int level = static_cast<int>(match.captured(1).length());
        QString title = match.captured(2).trimmed();
        for (std::size_t i = 0; i < codeSnippets.size(); ++i) {
            const QString token = QStringLiteral("\x1DCODE_%1\x1D").arg(i);
            title.replace(token, codeSnippets[i]);
        }
        const QString slug = slugifyHeading(title);
        const int indentSpaces = (level - 1) * 2;
        const QString indent(indentSpaces, QLatin1Char(' '));
        tocHtml += QStringLiteral("%1  <li class=\"toc-level-%2\"><a href=\"#%3\">%4</a></li>\n")
                       .arg(indent)
                       .arg(level)
                       .arg(slug, sanitizeForPreview(title));
        ++count;
    }
    tocHtml += QStringLiteral("</ul>\n</div>\n\n");

    for (std::size_t i = 0; i < codeSnippets.size(); ++i) {
        const QString token = QStringLiteral("\x1DCODE_%1\x1D").arg(i);
        masked.replace(token, codeSnippets[i]);
    }

    if (count == 0) {
        return content;
    }

    static const QRegularExpression tocTagRegex(QStringLiteral(R"(\[(?:TOC)\]|\{:toc\})"),
                                                QRegularExpression::CaseInsensitiveOption);
    masked.replace(tocTagRegex, tocHtml);
    return masked;
}

QString MarkdownPreviewEnhancer::processEmoji(const QString &content) {
    if (!content.contains(QLatin1Char(':'))) {
        return content;
    }

    struct EmojiMapping {
        const char *code;
        const char *unicode;
    };

    static constexpr std::array<EmojiMapping, 32> s_emojis = {{
        {":rocket:", "🚀"},
        {":sparkles:", "✨"},
        {":white_check_mark:", "✅"},
        {":check:", "✔️"},
        {":heavy_check_mark:", "✔️"},
        {":x:", "❌"},
        {":warning:", "⚠️"},
        {":bulb:", "💡"},
        {":memo:", "📝"},
        {":pencil:", "✏️"},
        {":clipboard:", "📋"},
        {":fire:", "🔥"},
        {":star:", "⭐"},
        {":bug:", "🐛"},
        {":zap:", "⚡"},
        {":link:", "🔗"},
        {":tada:", "🎉"},
        {":smile:", "😄"},
        {":thumbsup:", "👍"},
        {":+1:", "👍"},
        {":thumbsdown:", "👎"},
        {":-1:", "👎"},
        {":heart:", "❤️"},
        {":gear:", "⚙️"},
        {":lock:", "🔒"},
        {":unlock:", "🔓"},
        {":package:", "📦"},
        {":book:", "📖"},
        {":eyes:", "👀"},
        {":100:", "💯"},
        {":art:", "🎨"},
        {":key:", "🔑"},
    }};

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

    for (const auto &e : s_emojis) {
        masked.replace(QString::fromLatin1(e.code), QString::fromUtf8(e.unicode));
    }

    for (std::size_t i = 0; i < codeSnippets.size(); ++i) {
        const QString token = QStringLiteral("\x1DCODE_%1\x1D").arg(i);
        masked.replace(token, codeSnippets[i]);
    }

    return masked;
}

QString MarkdownPreviewEnhancer::mathAndMermaidCss(bool isDark) {
    const QString extraCss = isDark ? QStringLiteral(
                                          ".table-of-contents {\n"
                                          "    background-color: #161b22;\n"
                                          "    border: 1px solid #30363d;\n"
                                          "    border-radius: 6px;\n"
                                          "    padding: 12px 16px;\n"
                                          "    margin: 16px 0;\n"
                                          "}\n"
                                          ".toc-title {\n"
                                          "    font-weight: 600;\n"
                                          "    color: #58a6ff;\n"
                                          "    font-size: 13px;\n"
                                          "    margin-bottom: 8px;\n"
                                          "}\n"
                                          ".toc-list {\n"
                                          "    list-style: none;\n"
                                          "    padding-left: 0;\n"
                                          "    margin: 0;\n"
                                          "}\n"
                                          ".toc-list li {\n"
                                          "    margin: 4px 0;\n"
                                          "    font-size: 13px;\n"
                                          "}\n"
                                          ".toc-level-2 { padding-left: 16px; }\n"
                                          ".toc-level-3 { padding-left: 32px; }\n"
                                          ".toc-level-4 { padding-left: 48px; }\n")
                                    : QStringLiteral(
                                          ".table-of-contents {\n"
                                          "    background-color: #f6f8fa;\n"
                                          "    border: 1px solid #d0d7de;\n"
                                          "    border-radius: 6px;\n"
                                          "    padding: 12px 16px;\n"
                                          "    margin: 16px 0;\n"
                                          "}\n"
                                          ".toc-title {\n"
                                          "    font-weight: 600;\n"
                                          "    color: #0969da;\n"
                                          "    font-size: 13px;\n"
                                          "    margin-bottom: 8px;\n"
                                          "}\n"
                                          ".toc-list {\n"
                                          "    list-style: none;\n"
                                          "    padding-left: 0;\n"
                                          "    margin: 0;\n"
                                          "}\n"
                                          ".toc-list li {\n"
                                          "    margin: 4px 0;\n"
                                          "    font-size: 13px;\n"
                                          "}\n"
                                          ".toc-level-2 { padding-left: 16px; }\n"
                                          ".toc-level-3 { padding-left: 32px; }\n"
                                          ".toc-level-4 { padding-left: 48px; }\n");

    if (isDark) {
        return extraCss + QStringLiteral(
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

    return extraCss + QStringLiteral(
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
    auto footnotesHtml = QStringLiteral(
        "\n\n<hr class=\"footnotes-sep\">\n<section class=\"footnotes\">\n<ol class=\"footnotes-list\">\n");
    for (const auto &def : definitions) {
        footnotesHtml += QStringLiteral(
                             "<li id=\"fn_%1\" class=\"footnote-item\"><p>%2 <a href=\"#fnref_%1\" "
                             "class=\"footnote-backref\">↩</a></p></li>\n")
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

    // 2. Keyboard shortcut chips: [[Ctrl]] -> <kbd>Ctrl</kbd>
    static const QRegularExpression kbdRegex(QStringLiteral(R"(\[\[([^\[\]\n]+)\]\])"));
    masked.replace(kbdRegex, QStringLiteral("<kbd>\\1</kbd>"));

    // 3. Strikethrough: ~~del~~
    static const QRegularExpression strikeRegex(QStringLiteral(R"(~~(?=\S)(.+?)(?<=\S)~~)"));
    masked.replace(strikeRegex, QStringLiteral("<del>\\1</del>"));

    // 4. Mark / Highlight: ==mark==
    static const QRegularExpression markRegex(QStringLiteral(R"(==(?=\S)(.+?)(?<=\S)==)"));
    masked.replace(markRegex, QStringLiteral("<mark>\\1</mark>"));

    // 5. Insert: ++ins++
    static const QRegularExpression insRegex(QStringLiteral(R"(\+\+(?=\S)(.+?)(?<=\S)\+\+)"));
    masked.replace(insRegex, QStringLiteral("<ins>\\1</ins>"));

    // 6. Subscript: ~sub~ (single ~ not preceded or followed by ~)
    static const QRegularExpression subRegex(QStringLiteral(R"((?<!~)~(?!~|\s)([^~\s\n]+?)(?<!\s)~(?!~))"));
    masked.replace(subRegex, QStringLiteral("<sub>\\1</sub>"));

    // 7. Superscript: ^sup^ (single ^ not preceded or followed by ^)
    static const QRegularExpression supRegex(QStringLiteral(R"((?<!\^)\^(?!\^|\s)([^\^\s\n]+?)(?<!\s)\^(?!\^))"));
    masked.replace(supRegex, QStringLiteral("<sup>\\1</sup>"));

    // 8. Restore protected code blocks
    for (std::size_t i = 0; i < codeSnippets.size(); ++i) {
        const QString token = QStringLiteral("\x1DCODE_%1\x1D").arg(i);
        masked.replace(token, codeSnippets[i]);
    }

    return masked;
}

QString MarkdownPreviewEnhancer::enhanceMarkdown(const QString &rawMd, bool isDark) {
    if (rawMd.isEmpty()) {
        return rawMd;
    }
    // Fast path: if no markdown special syntax tokens exist, skip heavy regex pipeline
    if (!rawMd.contains(QLatin1Char('`')) && !rawMd.contains(QLatin1Char('$')) && !rawMd.contains(QLatin1Char('[')) &&
        !rawMd.contains(QLatin1Char('^')) && !rawMd.contains(QLatin1Char('~')) && !rawMd.contains(QLatin1Char('=')) &&
        !rawMd.contains(QLatin1Char('+')) && !rawMd.contains(QLatin1Char(':')) && !rawMd.contains(QLatin1Char('{'))) {
        return rawMd;
    }

    QString processed = processTableOfContents(rawMd);
    processed = processMermaidBlocks(processed, isDark);
    processed = processMathExpressions(processed);
    processed = processTaskLists(processed);
    processed = processExtensions(processed);
    processed = processFootnotes(processed);
    processed = processEmoji(processed);
    return processed;
}
