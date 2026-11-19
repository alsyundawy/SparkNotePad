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
#include <QSet>
#include <QTextDocument>
#include <algorithm>
#include <array>
#include <mutex>
#include <numeric>
#include <vector>

namespace {

std::vector<QString> s_codeSnippets;
std::vector<int> s_lineAnchors;
std::mutex s_enhancerMutex;

const QSet<QString> &validHtmlTags() {
    static const QSet<QString> s_tags = {
        QStringLiteral("a"), QStringLiteral("abbr"), QStringLiteral("address"), QStringLiteral("area"),
        QStringLiteral("article"), QStringLiteral("aside"), QStringLiteral("audio"), QStringLiteral("b"),
        QStringLiteral("base"), QStringLiteral("bdi"), QStringLiteral("bdo"), QStringLiteral("blockquote"),
        QStringLiteral("body"), QStringLiteral("br"), QStringLiteral("button"), QStringLiteral("canvas"),
        QStringLiteral("caption"), QStringLiteral("cite"), QStringLiteral("code"), QStringLiteral("col"),
        QStringLiteral("colgroup"), QStringLiteral("data"), QStringLiteral("datalist"), QStringLiteral("dd"),
        QStringLiteral("del"), QStringLiteral("details"), QStringLiteral("dfn"), QStringLiteral("dialog"),
        QStringLiteral("div"), QStringLiteral("dl"), QStringLiteral("dt"), QStringLiteral("em"),
        QStringLiteral("embed"), QStringLiteral("fieldset"), QStringLiteral("figcaption"), QStringLiteral("figure"),
        QStringLiteral("footer"), QStringLiteral("form"), QStringLiteral("h1"), QStringLiteral("h2"),
        QStringLiteral("h3"), QStringLiteral("h4"), QStringLiteral("h5"), QStringLiteral("h6"),
        QStringLiteral("head"), QStringLiteral("header"), QStringLiteral("hgroup"), QStringLiteral("hr"),
        QStringLiteral("html"), QStringLiteral("i"), QStringLiteral("iframe"), QStringLiteral("img"),
        QStringLiteral("input"), QStringLiteral("ins"), QStringLiteral("kbd"), QStringLiteral("label"),
        QStringLiteral("legend"), QStringLiteral("li"), QStringLiteral("link"), QStringLiteral("main"),
        QStringLiteral("map"), QStringLiteral("mark"), QStringLiteral("menu"), QStringLiteral("meta"),
        QStringLiteral("meter"), QStringLiteral("nav"), QStringLiteral("noscript"), QStringLiteral("object"),
        QStringLiteral("ol"), QStringLiteral("optgroup"), QStringLiteral("option"), QStringLiteral("output"),
        QStringLiteral("p"), QStringLiteral("picture"), QStringLiteral("pre"), QStringLiteral("progress"),
        QStringLiteral("q"), QStringLiteral("rp"), QStringLiteral("rt"), QStringLiteral("ruby"),
        QStringLiteral("s"), QStringLiteral("samp"), QStringLiteral("script"), QStringLiteral("section"),
        QStringLiteral("select"), QStringLiteral("slot"), QStringLiteral("small"), QStringLiteral("source"),
        QStringLiteral("span"), QStringLiteral("strong"), QStringLiteral("style"), QStringLiteral("sub"),
        QStringLiteral("summary"), QStringLiteral("sup"), QStringLiteral("svg"), QStringLiteral("table"),
        QStringLiteral("tbody"), QStringLiteral("td"), QStringLiteral("template"), QStringLiteral("textarea"),
        QStringLiteral("tfoot"), QStringLiteral("th"), QStringLiteral("thead"), QStringLiteral("time"),
        QStringLiteral("title"), QStringLiteral("tr"), QStringLiteral("track"), QStringLiteral("u"),
        QStringLiteral("ul"), QStringLiteral("var"), QStringLiteral("video"), QStringLiteral("wbr")
    };
    return s_tags;
}

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
    const QString alertClass = QString::fromLatin1(c.title).toLower();
    const QString replacement = QStringLiteral(
                                    "<div class=\"markdown-alert markdown-alert-%1\" style=\"margin: 12px 0; padding: 10px 14px; border-left: 4px solid %2; "
                                    "background-color: %3; border-radius: 4px;\">"
                                    "<div class=\"markdown-alert-title\" style=\"font-weight: 600; color: %2; margin-bottom: 4px;\">%4 %5</div>"
                                    "<div>%6</div></div>")
                                    .arg(alertClass, color, bg, QString::fromUtf8(c.icon),
                                         QCoreApplication::translate("MarkdownPreviewEnhancer", c.title), inner);

    result.replace(bqStart, (bqEnd + 13) - bqStart, replacement);
    idx = bqStart + replacement.length();
}

QString cleanQtHtml(const QString &rawHtml) {
    // 1. Extract body content
    QString content = rawHtml;
    const qsizetype bodyStart = content.indexOf(QStringLiteral("<body"), 0, Qt::CaseInsensitive);
    if (bodyStart != -1) {
        const qsizetype openClose = content.indexOf(QLatin1Char('>'), bodyStart);
        if (openClose != -1) {
            const qsizetype bodyEnd = content.lastIndexOf(QStringLiteral("</body>"), -1, Qt::CaseInsensitive);
            if (bodyEnd != -1 && bodyEnd > openClose) {
                content = content.mid(openClose + 1, bodyEnd - (openClose + 1)).trimmed();
            } else {
                content = content.mid(openClose + 1).trimmed();
            }
        }
    }

    // 2. Remove -qt-block-indent and text-indent
    static const QRegularExpression qtIndentRegex(QStringLiteral(R"(-qt-block-indent:\s*\d+;?)"));
    content.remove(qtIndentRegex);
    static const QRegularExpression textIndentRegex(QStringLiteral(R"(text-indent:\s*\d+px;?)"));
    content.remove(textIndentRegex);

    // 3. Remove default margin resets that wipe out CSS margins on paragraphs
    static const QRegularExpression zeroMarginRegex(
        QStringLiteral(R"(margin-(?:top|bottom|left|right):\s*0px;?)"));
    content.remove(zeroMarginRegex);

    // 4. Clean empty style attributes
    static const QRegularExpression emptyStyleRegex(QStringLiteral(R"(\s*style="\s*")"));
    content.remove(emptyStyleRegex);

    // 5. Replace table tags with styled markdown-table class preserving border attributes
    static const QRegularExpression tableRegex(QStringLiteral(R"(<table\b[^>]*>)"), QRegularExpression::CaseInsensitiveOption);
    content.replace(tableRegex, QStringLiteral(R"(<table class="markdown-table" border="1" cellpadding="6" cellspacing="0" style="border-collapse: collapse; width: 100%; margin: 16px 0;">)"));

    return content;
}

QString sanitizeHtmlInsideBlock(QString block) {
    static const QRegularExpression brRegex(QStringLiteral(R"(<br\s*/?>)"), QRegularExpression::CaseInsensitiveOption);
    block.replace(brRegex, QStringLiteral("<br />"));
    block.replace(QStringLiteral("&nbsp;"), QStringLiteral("&#160;"));
    return block;
}

}  // namespace

QString MarkdownPreviewEnhancer::sanitizeForPreview(const QString &text) {
    QString s = text;
    s.replace(QLatin1Char('&'), QStringLiteral("&amp;"));
    s.replace(QLatin1Char('<'), QStringLiteral("&lt;"));
    s.replace(QLatin1Char('>'), QStringLiteral("&gt;"));
    return s;
}

QString MarkdownPreviewEnhancer::sanitizeCodeForTerminal(const QString &rawCode, const QString &lang) {
    QString code = rawCode;

    // 1. Strip all carriage returns to ensure pure Unix LF line endings
    code.remove(QLatin1Char('\r'));

    // 2. Normalize non-breaking spaces and narrow NBSP to standard ASCII space (0x20)
    code.replace(QChar(0x00A0), QLatin1Char(' '));
    code.replace(QChar(0x202F), QLatin1Char(' '));

    // 3. Strip zero-width spaces, joiners, and byte order marks that corrupt bash commands
    code.remove(QChar(0x200B));
    code.remove(QChar(0x200C));
    code.remove(QChar(0x200D));
    code.remove(QChar(0xFEFF));

    // 4. Normalize quotes and dashes for shell scripts (bash, sh, zsh, shell)
    const QString l = lang.trimmed().toLower();
    if (l == QStringLiteral("bash") || l == QStringLiteral("sh") ||
        l == QStringLiteral("zsh") || l == QStringLiteral("shell") || l.isEmpty()) {
        code.replace(QChar(0x201C), QLatin1Char('"'));
        code.replace(QChar(0x201D), QLatin1Char('"'));
        code.replace(QChar(0x2018), QLatin1Char('\''));
        code.replace(QChar(0x2019), QLatin1Char('\''));
        code.replace(QChar(0x0060), QLatin1Char('`'));
        code.replace(QChar(0x00B4), QLatin1Char('\''));

        code.replace(QChar(0x2013), QLatin1Char('-'));
        code.replace(QChar(0x2014), QLatin1Char('-'));
        code.replace(QChar(0x2212), QLatin1Char('-'));
    }

    return code;
}

QString MarkdownPreviewEnhancer::codeSnippet(int index) {
    const std::lock_guard<std::mutex> lock(s_enhancerMutex);
    if (index >= 0 && static_cast<std::size_t>(index) < s_codeSnippets.size()) {
        return s_codeSnippets[static_cast<std::size_t>(index)];
    }
    return QString();
}

int MarkdownPreviewEnhancer::codeSnippetCount() {
    const std::lock_guard<std::mutex> lock(s_enhancerMutex);
    return static_cast<int>(s_codeSnippets.size());
}

void MarkdownPreviewEnhancer::clearCodeSnippets() {
    const std::lock_guard<std::mutex> lock(s_enhancerMutex);
    s_codeSnippets.clear();
    s_lineAnchors.clear();
}

std::vector<int> MarkdownPreviewEnhancer::lastRenderedLineAnchors() {
    const std::lock_guard<std::mutex> lock(s_enhancerMutex);
    return s_lineAnchors;
}

int MarkdownPreviewEnhancer::findClosestAnchorLine(int docLine) {
    const std::lock_guard<std::mutex> lock(s_enhancerMutex);
    if (s_lineAnchors.empty()) {
        return -1;
    }
    auto it = std::upper_bound(s_lineAnchors.begin(), s_lineAnchors.end(), docLine);
    if (it == s_lineAnchors.begin()) {
        return s_lineAnchors.front();
    }
    --it;
    return *it;
}

void MarkdownPreviewEnhancer::findAnchorBracket(int line, int &prevAnchor, int &nextAnchor) {
    const std::lock_guard<std::mutex> lock(s_enhancerMutex);
    if (s_lineAnchors.empty()) {
        prevAnchor = -1;
        nextAnchor = -1;
        return;
    }
    auto it = std::upper_bound(s_lineAnchors.begin(), s_lineAnchors.end(), line);
    if (it == s_lineAnchors.begin()) {
        prevAnchor = s_lineAnchors.front();
        nextAnchor = (s_lineAnchors.size() > 1) ? s_lineAnchors[1] : s_lineAnchors.front();
        return;
    }
    nextAnchor = (it != s_lineAnchors.end()) ? *it : s_lineAnchors.back();
    --it;
    prevAnchor = *it;
}

QString MarkdownPreviewEnhancer::sanitizePseudoHtmlTags(const QString &text) {
    QString result = text;

    // 1. Normalize unclosed <br> to self-closing <br />
    static const QRegularExpression unclosedBrRegex(QStringLiteral(R"(<br\s*(?<!/)>)"),
                                                    QRegularExpression::CaseInsensitiveOption);
    result.replace(unclosedBrRegex, QStringLiteral("<br />"));

    // 2. Normalize unclosed <hr> to self-closing <hr />
    static const QRegularExpression unclosedHrRegex(QStringLiteral(R"(<hr\s*(?<!/)>)"),
                                                    QRegularExpression::CaseInsensitiveOption);
    result.replace(unclosedHrRegex, QStringLiteral("<hr />"));

    // 3. Escape pseudo-HTML tags that are not standard HTML5 elements (e.g. <user>, <domain>, <PPID>)
    static const QRegularExpression tagRegex(QStringLiteral(R"(<(/)?([a-zA-Z0-9_\-]+)([^>]*)>)"));
    const auto &validTags = validHtmlTags();

    auto matchIt = tagRegex.globalMatch(result);
    qsizetype offset = 0;

    while (matchIt.hasNext()) {
        const auto match = matchIt.next();
        const QString tagName = match.captured(2).toLower();
        if (!validTags.contains(tagName)) {
            const QString slash = match.captured(1);
            const QString attrs = match.captured(3);
            const QString replacement = QStringLiteral("&lt;%1%2%3&gt;").arg(slash, match.captured(2), attrs);

            const qsizetype start = match.capturedStart() + offset;
            const qsizetype length = match.capturedLength();
            result.replace(start, length, replacement);
            offset += replacement.length() - length;
        }
    }

    return result;
}

bool MarkdownPreviewEnhancer::isMarkdownPath(const QString &path) {
    static const QRegularExpression mdRegex(
        QStringLiteral(R"(\.(?:markdown|mdown|mkdn|md|mkd|mdwn|mdtxt|mdtext|text)(?:#.*|\?.*)?$)"),
        QRegularExpression::CaseInsensitiveOption);
    return mdRegex.match(path).hasMatch();
}

bool MarkdownPreviewEnhancer::isMarkdownOrHtmlPath(const QString &path) {
    return isMarkdownPath(path);
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
    static const QRegularExpression mermaidRegex(
        QStringLiteral(R"((?:^|\n)[ \t]*```mermaid[ \t]*\r?\n([\s\S]*?)\r?\n[ \t]*```(?=[ \t]*(?:\r?\n|$)))"),
        QRegularExpression::CaseInsensitiveOption);

    QString result = content;
    auto matchIterator = mermaidRegex.globalMatch(content);
    qsizetype offset = 0;

    while (matchIterator.hasNext()) {
        const QRegularExpressionMatch match = matchIterator.next();
        const QString rawDiagram = match.captured(1).trimmed();
        const QString cleanDiagram = sanitizeCodeForTerminal(rawDiagram, QStringLiteral("mermaid"));

        int snippetId = -1;
        {
            const std::lock_guard<std::mutex> lock(s_enhancerMutex);
            snippetId = static_cast<int>(s_codeSnippets.size());
            s_codeSnippets.push_back(cleanDiagram);
        }

        const QString escapedSource = sanitizeForPreview(cleanDiagram);

        const QString replacement = QStringLiteral(
                                        "<div class=\"code-block-container mermaid-diagram\">\n"
                                        "<div class=\"code-block-header mermaid-header\" style=\"user-select: none; -webkit-user-select: none;\">"
                                        "<span style=\"user-select: none;\">📊 Mermaid Diagram</span>"
                                        "<a href=\"copy-code:%1\" class=\"code-copy-btn\" style=\"float: right; text-decoration: none; font-size: 11px; font-weight: 600; cursor: pointer; padding: 1px 7px; border-radius: 4px;\" title=\"Copy clean diagram source\">📋 Copy</a>"
                                        "</div>\n"
                                        "<pre class=\"mermaid-code\" style=\"margin: 0; padding: 12px 14px; background: transparent; border: none; font-family: 'JetBrains Mono', 'SFMono-Regular', Consolas, Menlo, monospace; font-size: 12px; line-height: 1.45; white-space: pre !important;\">%2</pre>\n"
                                        "</div>\n")
                                        .arg(QString::number(snippetId), escapedSource);

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
    auto footnotesHtml = std::accumulate(definitions.begin(), definitions.end(),
        QStringLiteral("\n\n<hr class=\"footnotes-sep\">\n<section class=\"footnotes\">\n<ol class=\"footnotes-list\">\n"),
        [](QString acc, const FootnoteDef &def) {
            acc += QStringLiteral(
                       "<li id=\"fn_%1\" class=\"footnote-item\"><p>%2 <a href=\"#fnref_%1\" "
                       "class=\"footnote-backref\">↩</a></p></li>\n")
                       .arg(def.id, sanitizeForPreview(def.text));
            return acc;
        });
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

    // 6. Subscript: ~sub~ (single ~ not preceded or followed by slash/tilde; content alphanumeric)
    static const QRegularExpression subRegex(QStringLiteral(R"((?<![~/\\])~(?!/)(\w+?)~(?!~))"));
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

QString MarkdownPreviewEnhancer::renderMarkdownToHtml(const QString &rawMd, bool isDark) {
    if (rawMd.isEmpty()) {
        return QString();
    }

    {
        const std::lock_guard<std::mutex> lock(s_enhancerMutex);
        s_codeSnippets.clear();
        s_lineAnchors.clear();
    }

    // Precompute newline offsets for fast O(log N) source line lookup
    std::vector<qsizetype> lineOffsets;
    lineOffsets.reserve((rawMd.length() / 40) + 1);
    lineOffsets.push_back(0);
    for (qsizetype i = 0; i < rawMd.length(); ++i) {
        if (rawMd.at(i) == QLatin1Char('\n')) {
            lineOffsets.push_back(i + 1);
        }
    }

    const auto getLineNum = [&lineOffsets](qsizetype charPos) -> int {
        const auto it = std::upper_bound(lineOffsets.begin(), lineOffsets.end(), charPos);
        return static_cast<int>(std::distance(lineOffsets.begin(), it));
    };

    std::vector<int> localAnchors;
    localAnchors.push_back(1);

    // 0. Sanitize pseudo-tags & normalize <br> / <hr> elements
    QString working = sanitizePseudoHtmlTags(rawMd);

    // Clean redundant copy labels placed above code blocks in external markdown documents
    static const QRegularExpression copyLabelRegex(
        QStringLiteral(R"((?:\*\*|__)?\[\s*📋\s*(?:Salin\s+(?:Blok\s+Kode|Perintah)\s*\/\s*Copy\s+Code\s+Block)\s*\](?:\*\*|__)?\s*)"),
        QRegularExpression::CaseInsensitiveOption);
    working.remove(copyLabelRegex);

    // 1. Mask HTML comments
    static const QRegularExpression commentRegex(QStringLiteral(R"(<!--[\s\S]*?-->)"));
    std::vector<QString> comments;
    auto commentIt = commentRegex.globalMatch(working);
    qsizetype offset = 0;
    while (commentIt.hasNext()) {
        const auto m = commentIt.next();
        comments.push_back(m.captured(0));
        const QString token = QStringLiteral("SPARKCOMMENT%1XYZ").arg(comments.size() - 1);
        working.replace(m.capturedStart() + offset, m.capturedLength(), token);
        offset += token.length() - m.capturedLength();
    }

    // 2. Mask Mermaid blocks to prevent MD4C parser entity double-escaping
    static const QRegularExpression mermaidRegex(
        QStringLiteral(R"((?:^|\n)[ \t]*```mermaid[ \t]*\r?\n([\s\S]*?)\r?\n[ \t]*```(?=[ \t]*(?:\r?\n|$)))"),
        QRegularExpression::CaseInsensitiveOption);
    std::vector<QString> mermaidBlocks;
    auto mermaidIt = mermaidRegex.globalMatch(working);
    offset = 0;
    while (mermaidIt.hasNext()) {
        const auto m = mermaidIt.next();
        const int startLine = getLineNum(m.capturedStart());
        localAnchors.push_back(startLine);

        const QString rawDiagram = m.captured(1).trimmed();
        const QString cleanDiagram = sanitizeCodeForTerminal(rawDiagram, QStringLiteral("mermaid"));

        int snippetId = -1;
        {
            const std::lock_guard<std::mutex> lock(s_enhancerMutex);
            snippetId = static_cast<int>(s_codeSnippets.size());
            s_codeSnippets.push_back(cleanDiagram);
        }

        const QString diagramSource = sanitizeForPreview(cleanDiagram);
        const QString blockHtml = QStringLiteral(
                                      "<a name=\"line_%1\" id=\"line_%1\"></a>\n"
                                      "<div class=\"code-block-container mermaid-diagram\" id=\"diagram_%1\" style=\"margin: 16px 0; background-color: #0d1117; border: 1px solid #30363d; border-radius: 8px; overflow: hidden;\">\n"
                                      "<table width=\"100%\" class=\"code-block-header mermaid-header\" cellpadding=\"0\" cellspacing=\"0\" style=\"width: 100%; border: none; margin: 0; padding: 6px 12px; background-color: #161b22; border-bottom: 1px solid #30363d;\">\n"
                                      "<tr>\n"
                                      "<td align=\"left\" style=\"border: none; padding: 0; vertical-align: middle;\">\n"
                                      "<span class=\"code-block-pill\" style=\"display: inline-block; background-color: #21262d; color: #f0f6fc; font-family: 'JetBrains Mono', 'SFMono-Regular', Consolas, Menlo, monospace; font-size: 11.5px; font-weight: 600; padding: 2px 10px; border-radius: 12px; border: 1px solid #30363d;\">📊 Mermaid Diagram</span>\n"
                                      "</td>\n"
                                      "<td align=\"right\" style=\"border: none; padding: 0; text-align: right; vertical-align: middle;\">\n"
                                      "<a href=\"copy-code:%2\" class=\"code-copy-btn\" style=\"text-decoration: none; font-size: 11px; font-weight: 600; color: #c9d1d9; cursor: pointer; padding: 2px 8px; border: 1px solid #30363d; border-radius: 6px; background-color: #21262d;\" title=\"Copy clean diagram source\">📋 Copy</a>\n"
                                      "</td>\n"
                                      "</tr>\n"
                                      "</table>\n"
                                      "<pre class=\"mermaid-code\" style=\"margin: 0; padding: 14px 16px; background-color: #0d1117; border: none; font-family: 'JetBrains Mono', 'SFMono-Regular', Consolas, Menlo, monospace; font-size: 12px; line-height: 1.5; color: #e6edf3; white-space: pre !important; tab-size: 4;\">%3</pre>\n"
                                      "</div>")
                                      .arg(QString::number(startLine), QString::number(snippetId), diagramSource);
        mermaidBlocks.push_back(blockHtml);
        const QString token = QStringLiteral("\n\nSPARKMERMAIDBLOCK%1XYZ\n\n").arg(mermaidBlocks.size() - 1);
        working.replace(m.capturedStart() + offset, m.capturedLength(), token);
        offset += token.length() - m.capturedLength();
    }

    // 3. Mask Block Math ($$ ... $$)
    static const QRegularExpression blockMathRegex(QStringLiteral(R"(\$\$\s*\n?([\s\S]*?)\n?\s*\$\$)"));
    std::vector<QString> mathBlocks;
    auto mathIt = blockMathRegex.globalMatch(working);
    offset = 0;
    while (mathIt.hasNext()) {
        const auto m = mathIt.next();
        const int startLine = getLineNum(m.capturedStart());
        localAnchors.push_back(startLine);

        const QString mathSource = sanitizeForPreview(m.captured(1).trimmed());
        const QString blockHtml = QStringLiteral("<a name=\"line_%1\" id=\"line_%1\"></a>\n<div class=\"math-block\"><span class=\"math-tex\">%2</span></div>")
                                      .arg(QString::number(startLine), mathSource);
        mathBlocks.push_back(blockHtml);
        const QString token = QStringLiteral("\n\nSPARKMATHBLOCK%1XYZ\n\n").arg(mathBlocks.size() - 1);
        working.replace(m.capturedStart() + offset, m.capturedLength(), token);
        offset += token.length() - m.capturedLength();
    }

    // 4. Mask Fenced Code Blocks (``` or ~~~) into unified pre containers
    static const QRegularExpression fencedCodeRegex(
        QStringLiteral(R"((?:^|\n)[ \t]*(```|~~~)[ \t]*([^\r\n]*)\r?\n([\s\S]*?)\r?\n[ \t]*\1(?=[ \t]*(?:\r?\n|$)))"));

    const auto getLangInfo = [](const QString &lang) -> std::pair<QString, QString> {
        const QString l = lang.trimmed().toLower();
        if (l.isEmpty()) {
            return {QString(), QString()};
        }
        if (l == QStringLiteral("bash") || l == QStringLiteral("sh") || l == QStringLiteral("shell") || l == QStringLiteral("zsh")) {
            return {QStringLiteral("💻"), l};
        }
        if (l == QStringLiteral("python") || l == QStringLiteral("py")) {
            return {QStringLiteral("🐍"), QStringLiteral("python")};
        }
        if (l == QStringLiteral("cpp") || l == QStringLiteral("c++") || l == QStringLiteral("c") || l == QStringLiteral("h") || l == QStringLiteral("hpp") || l == QStringLiteral("cc")) {
            return {QStringLiteral("⚙️"), l};
        }
        if (l == QStringLiteral("html") || l == QStringLiteral("xml") || l == QStringLiteral("xhtml")) {
            return {QStringLiteral("🌐"), l};
        }
        if (l == QStringLiteral("json")) {
            return {QStringLiteral("📋"), QStringLiteral("json")};
        }
        if (l == QStringLiteral("yaml") || l == QStringLiteral("yml")) {
            return {QStringLiteral("📄"), QStringLiteral("yaml")};
        }
        if (l == QStringLiteral("sql")) {
            return {QStringLiteral("🗄️"), QStringLiteral("sql")};
        }
        if (l == QStringLiteral("docker") || l == QStringLiteral("dockerfile")) {
            return {QStringLiteral("🐳"), QStringLiteral("docker")};
        }
        if (l == QStringLiteral("nginx")) {
            return {QStringLiteral("🚀"), QStringLiteral("nginx")};
        }
        if (l == QStringLiteral("ini") || l == QStringLiteral("conf") || l == QStringLiteral("cfg")) {
            return {QStringLiteral("⚙️"), l};
        }
        if (l == QStringLiteral("markdown") || l == QStringLiteral("md")) {
            return {QStringLiteral("📝"), QStringLiteral("markdown")};
        }
        if (l == QStringLiteral("js") || l == QStringLiteral("javascript")) {
            return {QStringLiteral("⚡"), QStringLiteral("javascript")};
        }
        if (l == QStringLiteral("ts") || l == QStringLiteral("typescript")) {
            return {QStringLiteral("⚡"), QStringLiteral("typescript")};
        }
        if (l == QStringLiteral("css") || l == QStringLiteral("scss") || l == QStringLiteral("sass") || l == QStringLiteral("less")) {
            return {QStringLiteral("🎨"), l};
        }
        if (l == QStringLiteral("java") || l == QStringLiteral("kotlin") || l == QStringLiteral("swift") || l == QStringLiteral("rust") || l == QStringLiteral("go")) {
            return {QStringLiteral("💻"), l};
        }
        if (l == QStringLiteral("text") || l == QStringLiteral("txt")) {
            return {QStringLiteral("📄"), QStringLiteral("text")};
        }
        return {QStringLiteral("💻"), l};
    };

    std::vector<QString> codeBlocks;
    auto codeIt = fencedCodeRegex.globalMatch(working);
    offset = 0;
    while (codeIt.hasNext()) {
        const auto m = codeIt.next();
        const int startLine = getLineNum(m.capturedStart());
        localAnchors.push_back(startLine);

        const QString info = m.captured(2).trimmed();
        const QString rawCode = m.captured(3);

        QString lang;
        QString title;

        if (!info.isEmpty()) {
            static const QRegularExpression attrRegex(
                QStringLiteral(R"((?:filename|title|file)\s*=\s*["']?([^"'\s]+)["']?)"),
                QRegularExpression::CaseInsensitiveOption);
            const auto attrMatch = attrRegex.match(info);
            if (attrMatch.hasMatch()) {
                title = attrMatch.captured(1).trimmed();
            }

            static const QRegularExpression langTokenRegex(QStringLiteral(R"(^([a-zA-Z0-9_\-\+#]+)(?:[:\s]+(.*))?$)"));
            const auto langMatch = langTokenRegex.match(info);
            if (langMatch.hasMatch()) {
                lang = langMatch.captured(1).trimmed();
                if (title.isEmpty()) {
                    QString rest = langMatch.captured(2).trimmed();
                    if (!rest.isEmpty()) {
                        if ((rest.startsWith(QLatin1Char('"')) && rest.endsWith(QLatin1Char('"'))) ||
                            (rest.startsWith(QLatin1Char('\'')) && rest.endsWith(QLatin1Char('\'')))) {
                            rest = rest.mid(1, rest.length() - 2).trimmed();
                        }
                        title = rest;
                    }
                }
            } else {
                lang = info;
            }
        }

        if (title.isEmpty()) {
            static const QRegularExpression headerCommentRegex(
                QStringLiteral(R"((?:#|\/\/|\/\*)\s*(?:Nama|Script\s*Name|File\s*Name|File)\s*:\s*([a-zA-Z0-9_\-\.\/]+))"),
                QRegularExpression::CaseInsensitiveOption);
            const auto headerMatch = headerCommentRegex.match(rawCode.left(1024));
            if (headerMatch.hasMatch()) {
                title = headerMatch.captured(1).trimmed();
            }
        }

        if (title.isEmpty()) {
            const qsizetype codeStart = m.capturedStart();
            const qsizetype checkWindowStart = std::max(static_cast<qsizetype>(0), codeStart - 250);
            const QString precedingText = working.mid(checkWindowStart, codeStart - checkWindowStart);
            static const QRegularExpression precFileRegex(
                QStringLiteral(R"(\*\*(?:Source\s+Code\s+dari\s+file|File|Skrip)\s+[`*_]*([a-zA-Z0-9_\-\.\/]+)[`*_]*\s*:\s*\*\*)"),
                QRegularExpression::CaseInsensitiveOption);
            const auto precMatch = precFileRegex.match(precedingText);
            if (precMatch.hasMatch()) {
                title = precMatch.captured(1).trimmed();
            }
        }

        if (title.isEmpty()) {
            const auto [icon, displayLang] = getLangInfo(lang);
            title = !displayLang.isEmpty() ? (!icon.isEmpty() ? QStringLiteral("%1 %2").arg(icon, displayLang) : displayLang)
                                           : (lang.isEmpty() ? QStringLiteral("code") : lang.toLower());
        }

        // Sanitize code for pristine terminal execution (Unix LF, ASCII spaces, safe quotes)
        const QString cleanCode = sanitizeCodeForTerminal(rawCode, lang);

        int snippetId = -1;
        {
            const std::lock_guard<std::mutex> lock(s_enhancerMutex);
            snippetId = static_cast<int>(s_codeSnippets.size());
            s_codeSnippets.push_back(cleanCode);
        }

        const QString escapedCode = sanitizeForPreview(cleanCode);

        const QString blockHtml = QStringLiteral(
            "<a name=\"line_%1\" id=\"line_%1\"></a>\n"
            "<div class=\"code-block-container\" id=\"line_box_%1\" style=\"margin: 16px 0; background-color: #0d1117; border: 1px solid #30363d; border-radius: 8px; overflow: hidden;\">\n"
            "<table width=\"100%\" class=\"code-block-header\" cellpadding=\"0\" cellspacing=\"0\" style=\"width: 100%; border: none; margin: 0; padding: 6px 12px; background-color: #161b22; border-bottom: 1px solid #30363d;\">\n"
            "<tr>\n"
            "<td align=\"left\" style=\"border: none; padding: 0; vertical-align: middle;\">\n"
            "<span class=\"code-block-pill\" style=\"display: inline-block; background-color: #21262d; color: #f0f6fc; font-family: 'JetBrains Mono', 'SFMono-Regular', Consolas, Menlo, monospace; font-size: 11.5px; font-weight: 600; padding: 2px 10px; border-radius: 12px; border: 1px solid #30363d;\">%2</span>\n"
            "</td>\n"
            "<td align=\"right\" style=\"border: none; padding: 0; text-align: right; vertical-align: middle;\">\n"
            "<a href=\"copy-code:%3\" class=\"code-copy-btn\" style=\"text-decoration: none; font-size: 11px; font-weight: 600; color: #c9d1d9; cursor: pointer; padding: 2px 8px; border: 1px solid #30363d; border-radius: 6px; background-color: #21262d;\" title=\"Copy clean terminal code\">📋 Copy</a>\n"
            "</td>\n"
            "</tr>\n"
            "</table>\n"
            "<pre class=\"code-block-content\" style=\"margin: 0; padding: 14px 16px; background-color: #0d1117; border: none; font-family: 'JetBrains Mono', 'SFMono-Regular', Consolas, Menlo, monospace; font-size: 12px; line-height: 1.5; color: #e6edf3; white-space: pre !important; tab-size: 4;\">%4</pre>\n"
            "</div>")
            .arg(QString::number(startLine), title.toHtmlEscaped(), QString::number(snippetId), escapedCode);

        codeBlocks.push_back(blockHtml);
        const QString token = QStringLiteral("\n\nSPARKCODEBLOCK%1XYZ\n\n").arg(codeBlocks.size() - 1);

        const qsizetype start = m.capturedStart() + offset;
        const qsizetype length = m.capturedLength();
        working.replace(start, length, token);
        offset += token.length() - length;
    }

    // 5. Mask HTML blocks
    static const QRegularExpression htmlBlockRegex(
        QStringLiteral(
            R"((?:^|\n)(<(p|div|h[1-6]|table|center|section|article|header|footer|figure|details|summary)[^>]*>[\s\S]*?<\/\2>))"),
        QRegularExpression::CaseInsensitiveOption);

    std::vector<QString> htmlBlocks;
    auto blockIt = htmlBlockRegex.globalMatch(working);
    offset = 0;
    while (blockIt.hasNext()) {
        const auto m = blockIt.next();
        const QString rawBlock = sanitizeHtmlInsideBlock(m.captured(1).trimmed());
        htmlBlocks.push_back(rawBlock);
        const QString token = QStringLiteral("\n\nSPARKHTMLBLOCK%1XYZ\n\n").arg(htmlBlocks.size() - 1);

        const qsizetype start = m.capturedStart(1) + offset;
        const qsizetype length = m.capturedLength(1);
        working.replace(start, length, token);
        offset += token.length() - length;
    }

    // Pre-record all masked intervals in rawMd (code blocks, mermaid, comments, math, html blocks)
    // so headings and tables inside bash comments/code blocks are never misidentified
    std::vector<std::pair<qsizetype, qsizetype>> maskedIntervals;
    auto maskIt = fencedCodeRegex.globalMatch(rawMd);
    while (maskIt.hasNext()) {
        const auto m = maskIt.next();
        maskedIntervals.emplace_back(m.capturedStart(), m.capturedEnd());
    }
    auto mIt = mermaidRegex.globalMatch(rawMd);
    while (mIt.hasNext()) {
        const auto m = mIt.next();
        maskedIntervals.emplace_back(m.capturedStart(), m.capturedEnd());
    }
    auto cIt = commentRegex.globalMatch(rawMd);
    while (cIt.hasNext()) {
        const auto m = cIt.next();
        maskedIntervals.emplace_back(m.capturedStart(), m.capturedEnd());
    }
    auto mathMaskIt = blockMathRegex.globalMatch(rawMd);
    while (mathMaskIt.hasNext()) {
        const auto m = mathMaskIt.next();
        maskedIntervals.emplace_back(m.capturedStart(), m.capturedEnd());
    }
    auto htmlMaskIt = htmlBlockRegex.globalMatch(rawMd);
    while (htmlMaskIt.hasNext()) {
        const auto m = htmlMaskIt.next();
        maskedIntervals.emplace_back(m.capturedStart(), m.capturedEnd());
    }

    const auto isInMaskedInterval = [&maskedIntervals](qsizetype pos) -> bool {
        return std::ranges::any_of(maskedIntervals, [pos](const auto &interval) {
            return pos >= interval.first && pos <= interval.second;
        });
    };

    // Scan source line numbers for real headings and tables (excluding code blocks and comments)
    std::vector<int> headingLines;
    static const QRegularExpression headingLineRegex(QStringLiteral(R"((?:^|\n)#{1,6}\s+[^\n]+)"));
    auto headingIt = headingLineRegex.globalMatch(rawMd);
    while (headingIt.hasNext()) {
        const auto m = headingIt.next();
        const qsizetype pos = m.capturedStart();
        if (!isInMaskedInterval(pos)) {
            const int hLine = getLineNum(pos);
            headingLines.push_back(hLine);
            localAnchors.push_back(hLine);
        }
    }

    std::vector<int> tableLines;
    static const QRegularExpression tableLineRegex(QStringLiteral(R"((?:^|\n)\|[^\n]+\|\r?\n\|[-:\s|]+\|)"));
    auto tableIt = tableLineRegex.globalMatch(rawMd);
    while (tableIt.hasNext()) {
        const auto m = tableIt.next();
        const qsizetype pos = m.capturedStart();
        if (!isInMaskedInterval(pos)) {
            const int tLine = getLineNum(pos);
            tableLines.push_back(tLine);
            localAnchors.push_back(tLine);
        }
    }

    // 7. Process markdown extensions
    QString enriched = enhanceMarkdown(working, isDark);

    // Normalize any unclosed <input> tags to self-closing <input ... />
    // to prevent MD4C/QTextDocument parser element nesting and font shrinkage
    static const QRegularExpression unclosedInputRegex(
        QStringLiteral(R"(<input\b([^>]*?)(?<!/)>)"), QRegularExpression::CaseInsensitiveOption);
    enriched.replace(unclosedInputRegex, QStringLiteral("<input\\1 />"));

    // 8. Convert via QTextDocument (native MD4C parser)
    QTextDocument doc;
    doc.setMarkdown(enriched);
    const QString rawDocHtml = doc.toHtml();

    // 9. Clean Qt artifacts to ensure semantic HTML conforming to GitHub Primer
    QString html = cleanQtHtml(rawDocHtml);

    // 10. Restore Code blocks
    for (std::size_t i = 0; i < codeBlocks.size(); ++i) {
        const QString token = QStringLiteral("SPARKCODEBLOCK%1XYZ").arg(i);
        const QRegularExpression tokenP(QStringLiteral(R"(<p[^>]*>\s*)") + token + QStringLiteral(R"(\s*<\/p>)"));
        const auto match = tokenP.match(html);
        if (match.hasMatch()) {
            html.replace(match.capturedStart(), match.capturedLength(), codeBlocks[i]);
        } else {
            html.replace(token, codeBlocks[i]);
        }
    }

    // 11. Restore HTML blocks
    for (std::size_t i = 0; i < htmlBlocks.size(); ++i) {
        const QString token = QStringLiteral("SPARKHTMLBLOCK%1XYZ").arg(i);
        const QRegularExpression tokenP(QStringLiteral(R"(<p[^>]*>\s*)") + token + QStringLiteral(R"(\s*<\/p>)"));
        const auto match = tokenP.match(html);
        if (match.hasMatch()) {
            html.replace(match.capturedStart(), match.capturedLength(), htmlBlocks[i]);
        } else {
            html.replace(token, htmlBlocks[i]);
        }
    }

    // 12. Restore Math blocks
    for (std::size_t i = 0; i < mathBlocks.size(); ++i) {
        const QString token = QStringLiteral("SPARKMATHBLOCK%1XYZ").arg(i);
        const QRegularExpression tokenP(QStringLiteral(R"(<p[^>]*>\s*)") + token + QStringLiteral(R"(\s*<\/p>)"));
        const auto match = tokenP.match(html);
        if (match.hasMatch()) {
            html.replace(match.capturedStart(), match.capturedLength(), mathBlocks[i]);
        } else {
            html.replace(token, mathBlocks[i]);
        }
    }

    // 13. Restore Mermaid blocks
    for (std::size_t i = 0; i < mermaidBlocks.size(); ++i) {
        const QString token = QStringLiteral("SPARKMERMAIDBLOCK%1XYZ").arg(i);
        const QRegularExpression tokenP(QStringLiteral(R"(<p[^>]*>\s*)") + token + QStringLiteral(R"(\s*<\/p>)"));
        const auto match = tokenP.match(html);
        if (match.hasMatch()) {
            html.replace(match.capturedStart(), match.capturedLength(), mermaidBlocks[i]);
        } else {
            html.replace(token, mermaidBlocks[i]);
        }
    }

    // 14. Strip masked comments
    for (std::size_t i = 0; i < comments.size(); ++i) {
        const QString token = QStringLiteral("SPARKCOMMENT%1XYZ").arg(i);
        const QRegularExpression tokenP(QStringLiteral(R"(<p[^>]*>\s*)") + token + QStringLiteral(R"(\s*<\/p>)"));
        const auto match = tokenP.match(html);
        if (match.hasMatch()) {
            html.replace(match.capturedStart(), match.capturedLength(), QString());
        } else {
            html.replace(token, QString());
        }
    }

    // 15. Post-process headings with IDs and callouts
    const QString htmlWithIds = processHeadingsWithIds(html);
    const QString calloutsEnhanced = enhanceCallouts(htmlWithIds, isDark);

    // 16. Attach line anchors to tables and headings in HTML post-processing (iterating backwards to preserve offsets)
    QString finalHtml = calloutsEnhanced;

    std::vector<QRegularExpressionMatch> tableMatches;
    static const QRegularExpression tableTagRegex(QStringLiteral(R"(<table\b[^>]*>)"), QRegularExpression::CaseInsensitiveOption);
    auto tblMatchIt = tableTagRegex.globalMatch(finalHtml);
    while (tblMatchIt.hasNext()) {
        tableMatches.push_back(tblMatchIt.next());
    }
    for (qsizetype i = static_cast<qsizetype>(tableMatches.size()) - 1; i >= 0; --i) {
        const int tLine = (static_cast<std::size_t>(i) < tableLines.size()) ? tableLines[static_cast<std::size_t>(i)] : 1;
        const QString anchorTag = QStringLiteral("<a name=\"line_%1\" id=\"line_%1\"></a>\n").arg(tLine);
        finalHtml.insert(tableMatches[static_cast<std::size_t>(i)].capturedStart(), anchorTag);
    }

    std::vector<QRegularExpressionMatch> headingMatches;
    static const QRegularExpression hTagRegex(QStringLiteral(R"(<h([1-6])([^>]*)>)"), QRegularExpression::CaseInsensitiveOption);
    auto hMatchIt = hTagRegex.globalMatch(finalHtml);
    while (hMatchIt.hasNext()) {
        headingMatches.push_back(hMatchIt.next());
    }
    for (qsizetype i = static_cast<qsizetype>(headingMatches.size()) - 1; i >= 0; --i) {
        const int hLine = (static_cast<std::size_t>(i) < headingLines.size()) ? headingLines[static_cast<std::size_t>(i)] : 1;
        const QString anchorTag = QStringLiteral("<a name=\"line_%1\" id=\"line_%1\"></a>\n").arg(hLine);
        finalHtml.insert(headingMatches[static_cast<std::size_t>(i)].capturedStart(), anchorTag);
    }

    // Finalize synchronized scroll line anchors
    {
        const std::lock_guard<std::mutex> lock(s_enhancerMutex);
        std::sort(localAnchors.begin(), localAnchors.end());
        localAnchors.erase(std::unique(localAnchors.begin(), localAnchors.end()), localAnchors.end());
        s_lineAnchors = std::move(localAnchors);
    }

    // 17. Wrap in .markdown-body container for GitHub Markdown Preview Style compliance
    return QStringLiteral("<div class=\"markdown-body\">\n%1\n</div>").arg(finalHtml);
}
