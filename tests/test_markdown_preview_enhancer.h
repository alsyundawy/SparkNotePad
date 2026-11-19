#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QObject>
#include <QTest>
#include <QTextBrowser>
#include <QFile>
#include "utils/MarkdownPreviewEnhancer.h"
#include "utils/MarkdownThemeManager.h"

class TestMarkdownPreviewEnhancer : public QObject {
    Q_OBJECT

private slots:
    void testProcessMermaidBlocks() const {
        const auto input = QStringLiteral("Intro\n```mermaid\ngraph TD\n    A-->B\n```\nOutro");
        const auto result = MarkdownPreviewEnhancer::processMermaidBlocks(input, false);

        QVERIFY(result.contains(QStringLiteral("class=\"mermaid-diagram\"")) ||
                result.contains(QStringLiteral("mermaid")));
        QVERIFY(result.contains(QStringLiteral("graph TD")));
        QVERIFY(result.contains(QStringLiteral("A--&gt;B")) || result.contains(QStringLiteral("A-->B")));
    }

    void testProcessBlockMath() const {
        const auto input = QStringLiteral("Formula:\n$$\nE = mc^2\n$$\nDone");
        const auto result = MarkdownPreviewEnhancer::processMathExpressions(input);

        QVERIFY(result.contains(QStringLiteral("class=\"math-block\"")));
        QVERIFY(result.contains(QStringLiteral("E = mc^2")));
    }

    void testProcessInlineMath() const {
        const auto input = QStringLiteral("The equation is $f(x) = x^2$ in math.");
        const auto result = MarkdownPreviewEnhancer::processMathExpressions(input);

        QVERIFY(result.contains(QStringLiteral("class=\"math-inline\"")));
        QVERIFY(result.contains(QStringLiteral("f(x) = x^2")));
    }

    void testPreserveCurrencyAndEscapedDollars() const {
        const auto input = QStringLiteral("The price is $50 and another is $100. Escaped \\$variable.");
        const auto result = MarkdownPreviewEnhancer::processMathExpressions(input);

        // Price should not be turned into inline math
        QVERIFY(!result.contains(QStringLiteral("class=\"math-inline\"")));
        QVERIFY(result.contains(QStringLiteral("$50")));
    }

    void testXssSanitization() const {
        const auto malicious =
            QStringLiteral("```mermaid\n<script>alert('xss')</script>\n```\n$$<img src=x onerror=alert(1)>$$");
        const auto enhanced = MarkdownPreviewEnhancer::enhanceMarkdown(malicious, false);

        QVERIFY(!enhanced.contains(QStringLiteral("<script>")));
        QVERIFY(!enhanced.contains(QStringLiteral("<img src=x")));
        QVERIFY(enhanced.contains(QStringLiteral("&lt;script&gt;")) || enhanced.contains(QStringLiteral("&lt;img")));
    }

    void testMarkdownPathRegex() const {
        const QStringList validMd = {QStringLiteral("README.markdown"),
                                     QStringLiteral("doc.mdown"),
                                     QStringLiteral("guide.mkdn"),
                                     QStringLiteral("notes.md"),
                                     QStringLiteral("spec.mkd"),
                                     QStringLiteral("draft.mdwn"),
                                     QStringLiteral("log.mdtxt"),
                                     QStringLiteral("memo.mdtext"),
                                     QStringLiteral("readme.text"),
                                     QStringLiteral("file.md#section-2"),
                                     QStringLiteral("file.markdown?raw=true"),
                                     QStringLiteral("/Users/test/PATH/UPPER.MD")};

        for (const QString &path : validMd) {
            QVERIFY2(MarkdownPreviewEnhancer::isMarkdownPath(path),
                     qPrintable(QStringLiteral("Should match markdown: %1").arg(path)));
        }

        const QStringList invalidMd = {QStringLiteral("script.cpp"), QStringLiteral("index.html"),
                                       QStringLiteral("image.png"), QStringLiteral("archive.tar.gz")};

        for (const QString &path : invalidMd) {
            QVERIFY2(!MarkdownPreviewEnhancer::isMarkdownPath(path),
                     qPrintable(QStringLiteral("Should NOT match markdown: %1").arg(path)));
        }
    }

    void testMarkdownOnlyPathRegex() const {
        const QStringList validMd = {QStringLiteral("README.md"), QStringLiteral("doc.markdown"),
                                     QStringLiteral("notes.mdown"), QStringLiteral("file.mkdn")};

        for (const QString &path : validMd) {
            QVERIFY2(MarkdownPreviewEnhancer::isMarkdownPath(path),
                     qPrintable(QStringLiteral("Should match markdown: %1").arg(path)));
        }

        const QStringList nonMarkdown = {QStringLiteral("index.html"), QStringLiteral("page.htm"),
                                         QStringLiteral("main.cpp"), QStringLiteral("data.json")};

        for (const QString &path : nonMarkdown) {
            QVERIFY2(!MarkdownPreviewEnhancer::isMarkdownPath(path),
                     qPrintable(QStringLiteral("Should NOT match markdown: %1").arg(path)));
        }
    }

    void testMarkdownItExtensions() const {
        const auto input =
            QStringLiteral("This is ~~deleted~~ and ==highlighted== and ++inserted++ and H~2~O and E=mc^2^.");
        const auto enhanced = MarkdownPreviewEnhancer::enhanceMarkdown(input, false);

        QVERIFY(enhanced.contains(QStringLiteral("<del>deleted</del>")));
        QVERIFY(enhanced.contains(QStringLiteral("<mark>highlighted</mark>")));
        QVERIFY(enhanced.contains(QStringLiteral("<ins>inserted</ins>")));
        QVERIFY(enhanced.contains(QStringLiteral("<sub>2</sub>")));
        QVERIFY(enhanced.contains(QStringLiteral("<sup>2</sup>")));
    }

    void testTaskLists() const {
        const auto input = QStringLiteral("- [ ] Unfinished task\n- [x] Completed task\n* [X] Another completed");
        const auto enhanced = MarkdownPreviewEnhancer::enhanceMarkdown(input, false);

        QVERIFY(enhanced.contains(QStringLiteral("<input type=\"checkbox\" disabled>")));
        QVERIFY(enhanced.contains(QStringLiteral("<input type=\"checkbox\" checked disabled>")));
    }

    void testFootnotes() const {
        const auto input = QStringLiteral("Here is a reference[^1] in text.\n\n[^1]: This is the footnote definition.");
        const auto enhanced = MarkdownPreviewEnhancer::enhanceMarkdown(input, false);

        QVERIFY(enhanced.contains(
            QStringLiteral("<sup class=\"footnote-ref\"><a href=\"#fn_1\" id=\"fnref_1\">[1]</a></sup>")));
        QVERIFY(enhanced.contains(QStringLiteral("class=\"footnotes\"")));
        QVERIFY(enhanced.contains(QStringLiteral("<li id=\"fn_1\"")));
        QVERIFY(enhanced.contains(QStringLiteral("This is the footnote definition.")));
    }

    void testCodeBlockProtection() const {
        const auto input = QStringLiteral("```\n~~not strikethrough~~ and ==not mark==\n```\n`$not math$`");
        const auto enhanced = MarkdownPreviewEnhancer::enhanceMarkdown(input, false);

        QVERIFY(!enhanced.contains(QStringLiteral("<del>")));
        QVERIFY(!enhanced.contains(QStringLiteral("<mark>")));
        QVERIFY(enhanced.contains(QStringLiteral("~~not strikethrough~~")));
        QVERIFY(enhanced.contains(QStringLiteral("==not mark==")));
    }

    void testThemeManagerCatalog() const {
        const QStringList themes = MarkdownThemeManager::themeNames();
        QCOMPARE(themes.size(), 30);
        QCOMPARE(themes[0], QStringLiteral("GITHUB"));
        QCOMPARE(themes[1], QStringLiteral("GITHUB-DARK"));
        QCOMPARE(themes[2], QStringLiteral("ALMOND"));
        QCOMPARE(themes[29], QStringLiteral("WRIT"));

        QCOMPARE(MarkdownThemeManager::themeName(MarkdownThemeManager::ThemeId::GitHub), QStringLiteral("GITHUB"));
        QCOMPARE(MarkdownThemeManager::themeName(MarkdownThemeManager::ThemeId::GitHubDark),
                 QStringLiteral("GITHUB-DARK"));
        QCOMPARE(MarkdownThemeManager::themeFromName(QStringLiteral("sakura-vader")),
                 MarkdownThemeManager::ThemeId::SakuraVader);
        QCOMPARE(MarkdownThemeManager::themeFromName(QStringLiteral("water-dark")),
                 MarkdownThemeManager::ThemeId::WaterDark);

        QVERIFY(MarkdownThemeManager::isThemeDark(MarkdownThemeManager::ThemeId::GitHubDark));
        QVERIFY(MarkdownThemeManager::isThemeDark(MarkdownThemeManager::ThemeId::Retro));
        QVERIFY(!MarkdownThemeManager::isThemeDark(MarkdownThemeManager::ThemeId::GitHub));

        const QString css = MarkdownThemeManager::generateThemeCss(MarkdownThemeManager::ThemeId::Pico, 14);
        QVERIFY(css.contains(QStringLiteral("14px")));
        QVERIFY(css.contains(QStringLiteral("body")));
        QVERIFY(css.contains(QStringLiteral("h1")));
    }

    void testProcessCallouts() const {
        const auto input = QStringLiteral(
            "<blockquote>\n[!NOTE]\nThis is a critical note\n</blockquote>\n"
            "<blockquote>\n[!WARNING]\nTake caution!\n</blockquote>");
        const auto result = MarkdownPreviewEnhancer::enhanceCallouts(input, false);

        QVERIFY(result.contains(QStringLiteral("Note")));
        QVERIFY(result.contains(QStringLiteral("Warning")));
        QVERIFY(result.contains(QStringLiteral("This is a critical note")));
        QVERIFY(result.contains(QStringLiteral("Take caution!")));
    }

    void testProcessTableOfContents() const {
        const auto input = QStringLiteral(
            "[TOC]\n\n# Introduction\nSome text\n## Getting Started\nMore text\n### Advanced Usage\nDetails");
        const auto result = MarkdownPreviewEnhancer::processTableOfContents(input);

        QVERIFY(result.contains(QStringLiteral("class=\"table-of-contents\"")));
        QVERIFY(result.contains(QStringLiteral("Table of Contents")));
        QVERIFY(result.contains(QStringLiteral("<a href=\"#introduction\">Introduction</a>")));
        QVERIFY(result.contains(QStringLiteral("<a href=\"#getting-started\">Getting Started</a>")));
        QVERIFY(result.contains(QStringLiteral("<a href=\"#advanced-usage\">Advanced Usage</a>")));
    }

    void testProcessHeadingsWithIds() const {
        const auto html = QStringLiteral("<h1>My Header</h1>\n<h2>Sub Section & More</h2>");
        const auto result = MarkdownPreviewEnhancer::processHeadingsWithIds(html);

        QVERIFY(result.contains(QStringLiteral("<h1 id=\"my-header\">My Header</h1>")));
        QVERIFY(result.contains(QStringLiteral("<h2 id=\"sub-section-more\">Sub Section & More</h2>")));
    }

    void testProcessEmoji() const {
        const auto input =
            QStringLiteral("Build with :rocket: and :sparkles: for :100: success! `:not_an_emoji:` in code");
        const auto result = MarkdownPreviewEnhancer::processEmoji(input);

        QVERIFY(result.contains(QString::fromUtf8("🚀")));
        QVERIFY(result.contains(QString::fromUtf8("✨")));
        QVERIFY(result.contains(QString::fromUtf8("💯")));
        QVERIFY(result.contains(QStringLiteral("`:not_an_emoji:`")));
    }

    void testProcessKeyboardTags() const {
        const auto input = QStringLiteral("Press [[Ctrl]] + [[Alt]] + [[Delete]] to lock.");
        const auto result = MarkdownPreviewEnhancer::processExtensions(input);

        QVERIFY(result.contains(QStringLiteral("<kbd>Ctrl</kbd>")));
        QVERIFY(result.contains(QStringLiteral("<kbd>Alt</kbd>")));
        QVERIFY(result.contains(QStringLiteral("<kbd>Delete</kbd>")));
    }

    void testSlugifyHeading() const {
        QCOMPARE(MarkdownPreviewEnhancer::slugifyHeading(QStringLiteral("Hello World!")),
                 QStringLiteral("hello-world"));
        QCOMPARE(MarkdownPreviewEnhancer::slugifyHeading(QStringLiteral("---Section 1.2.3---")),
                 QStringLiteral("section-1-2-3"));
        QCOMPARE(MarkdownPreviewEnhancer::slugifyHeading(QStringLiteral("")), QStringLiteral("heading"));
    }

    void testRenderMarkdownToHtmlPreservesHtmlBlocks() const {
        const auto input = QStringLiteral(
            "<p align=\"center\">\n"
            "  <a href=\"https://example.com\"><img src=\"banner.png\" width=\"100%\" alt=\"Banner\" /></a>\n"
            "</p>\n\n"
            "<h1 align=\"center\">Title</h1>\n\n"
            "<p align=\"center\"><b>Subtitle</b><br>details &nbsp; more</p>\n\n"
            "> [!NOTE]\n"
            "> Important note here\n\n"
            "| Col1 | Col2 |\n"
            "|:---|:---|\n"
            "| A | B |\n");

        const auto html = MarkdownPreviewEnhancer::renderMarkdownToHtml(input, false);
        QVERIFY(html.contains(QStringLiteral("align=\"center\"")));
        QVERIFY(html.contains(QStringLiteral("banner.png")));
        QVERIFY(html.contains(QStringLiteral("Title")));
        QVERIFY(html.contains(QStringLiteral("Subtitle")));
        QVERIFY(html.contains(QStringLiteral("<table")));
        QVERIFY(html.contains(QStringLiteral("Important note here")));
    }

    void testRenderMarkdownFencedCodeBlocks() const {
        const auto input = QStringLiteral(
            "# Script Execution Test\n\n"
            "```bash\n"
            "# 1. Setup Zimbra Host\n"
            "HOST=\"mail.example.co.id\"\n"
            "if [ -z \"$HOST\" ]; then\n"
            "    echo \"Host is empty!\"\n"
            "    exit 1\n"
            "fi\n"
            "# 2. Check service status\n"
            "su - zimbra -c 'zmcontrol status'\n"
            "```\n\n"
            "```html\n"
            "<div class=\"sample\"><p>Safe code</p></div>\n"
            "```\n");

        const auto html = MarkdownPreviewEnhancer::renderMarkdownToHtml(input, true);
        QVERIFY(html.contains(QStringLiteral("code-block-container")));
        QVERIFY(html.contains(QStringLiteral("code-block-header")));
        QVERIFY(html.contains(QStringLiteral("💻 bash")));
        QVERIFY(html.contains(QStringLiteral("🌐 html")));
        QVERIFY(html.contains(QStringLiteral("code-block-content")));
        QVERIFY(html.contains(QStringLiteral("zmcontrol status")));
        QVERIFY(html.contains(QStringLiteral("copy-code:")));
        QVERIFY(html.contains(QStringLiteral("📋 Copy")));
    }

    void testSanitizeCodeForTerminal() const {
        // Test CRLF removal, NBSP replacement, zero-width space removal, smart quote normalization
        const QString dirtyBash = QString::fromUtf8(
            "# Bash test\r\n"
            "HOST=\u201Cmail.example.co.id\u201D\r\n"
            "PORT=\u20188080\u2019\r\n"
            "FLAG=\u2013-help\r\n"
            "CMD=echo\u00A0\"Hello\u200BWorld\"\r\n");

        const QString clean = MarkdownPreviewEnhancer::sanitizeCodeForTerminal(dirtyBash, QStringLiteral("bash"));

        QVERIFY(!clean.contains(QLatin1Char('\r')));
        QVERIFY(!clean.contains(QChar(0x00A0)));
        QVERIFY(!clean.contains(QChar(0x200B)));
        QVERIFY(!clean.contains(QChar(0x201C)));
        QVERIFY(!clean.contains(QChar(0x201D)));
        QVERIFY(!clean.contains(QChar(0x2018)));
        QVERIFY(!clean.contains(QChar(0x2019)));
        QVERIFY(!clean.contains(QChar(0x2013)));

        QVERIFY(clean.contains(QStringLiteral("HOST=\"mail.example.co.id\"")));
        QVERIFY(clean.contains(QStringLiteral("PORT='8080'")));
        QVERIFY(clean.contains(QStringLiteral("FLAG=--help")));
        QVERIFY(clean.contains(QStringLiteral("CMD=echo \"HelloWorld\"")));

        // Test multiline IFS normalization to strict mode IFS=$'\n\t'
        const QString brokenIfs = QStringLiteral("set -Eeuo pipefail\nIFS=$'\n    '\n");
        const QString cleanIfs = MarkdownPreviewEnhancer::sanitizeCodeForTerminal(brokenIfs, QStringLiteral("bash"));
        QVERIFY(cleanIfs.contains(QStringLiteral("IFS=$'\\n\\t'")));

        // Test redundant copy label removal (including Indonesian & English variations)
        const QString docWithLabels = QStringLiteral(
            "**[📋 Salin Blok Kode / Copy Code Block]**\n\n"
            "```bash\n"
            "echo \"Test 1\"\n"
            "```\n\n"
            "**[📋 Salin Konfigurasi / Copy Configuration]**\n\n"
            "```text\n"
            "config_val=1\n"
            "```\n\n"
            "**[📋 Salin Skrip / Copy Script Code]**\n\n"
            "```sh\n"
            "echo \"Test 2\"\n"
            "```\n");
        const QString renderedLabels = MarkdownPreviewEnhancer::renderMarkdownToHtml(docWithLabels, false);
        QVERIFY(!renderedLabels.contains(QStringLiteral("Salin Blok Kode")));
        QVERIFY(!renderedLabels.contains(QStringLiteral("Salin Konfigurasi")));
        QVERIFY(!renderedLabels.contains(QStringLiteral("Salin Skrip")));
    }

    void testCopyCodeSnippetAndAnchors() const {
        const auto input = QStringLiteral(
            "# Header 1\n\n"
            "Some introductory text.\n\n"
            "```bash\n"
            "echo \"First script\"\n"
            "```\n\n"
            "## Header 2\n\n"
            "| Col A | Col B |\n"
            "|:---|:---|\n"
            "| Val 1 | Val 2 |\n\n"
            "```python\n"
            "print('Second script')\n"
            "```\n");

        const auto html = MarkdownPreviewEnhancer::renderMarkdownToHtml(input, true);

        // Verify code snippets registered
        QVERIFY(MarkdownPreviewEnhancer::codeSnippetCount() >= 2);
        QCOMPARE(MarkdownPreviewEnhancer::codeSnippet(0).trimmed(), QStringLiteral("echo \"First script\""));
        QCOMPARE(MarkdownPreviewEnhancer::codeSnippet(1).trimmed(), QStringLiteral("print('Second script')"));

        // Verify anchors are generated and indexed
        const auto anchors = MarkdownPreviewEnhancer::lastRenderedLineAnchors();
        QVERIFY(!anchors.empty());
        QVERIFY(std::is_sorted(anchors.begin(), anchors.end()));

        // Test anchor lookups
        const int closest1 = MarkdownPreviewEnhancer::findClosestAnchorLine(1);
        QVERIFY(closest1 >= 1);

        const int closestCode = MarkdownPreviewEnhancer::findClosestAnchorLine(6);
        QVERIFY(closestCode <= 6);

        // Test bracket anchor lookups
        int prev = -1;
        int next = -1;
        MarkdownPreviewEnhancer::findAnchorBracket(4, prev, next);
        QVERIFY(prev >= 1);
        QVERIFY(next >= prev);

        // Verify HTML contains line anchors and Image 1 rounded pill badge
        QVERIFY(html.contains(QStringLiteral("name=\"line_")));
        QVERIFY(html.contains(QStringLiteral("class=\"code-block-pill\"")));
    }

    void testZimbraProductionGuideRender() const {
        QFile file(QStringLiteral("/Users/alsyundawy/Downloads/Zimbra-10.1.21-Complete-Production-Guide-example.md"));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QSKIP("Zimbra example file not found on disk");
        }
        const QString content = QString::fromUtf8(file.readAll());
        file.close();

        const auto html = MarkdownPreviewEnhancer::renderMarkdownToHtml(content, false);
        QVERIFY(html.length() > 500000);

        // Verify beginning
        QVERIFY(html.contains(QStringLiteral("ZIMBRA COLLABORATION 10.1.21")));
        // Verify middle
        QVERIFY(html.contains(QStringLiteral("Panel Hosting Komersial")));
        // Verify end
        QVERIFY(html.contains(QStringLiteral("LEMBAR PENGESAHAN DOKUMEN PRODUKSI")));
        // Verify tables
        QVERIFY(html.contains(QStringLiteral("Kategori Parameter")));
        // Verify copy buttons on bash blocks
        QVERIFY(html.contains(QStringLiteral("copy-code:")));
        QVERIFY(html.contains(QStringLiteral("📋 Copy")));
        // Verify Mermaid block copy buttons and diagram headers
        QVERIFY(html.contains(QStringLiteral("📊 Mermaid Diagram")));
        QVERIFY(html.contains(QStringLiteral("class=\"mermaid-code\"")));

        // Verify line anchors
        const auto anchors = MarkdownPreviewEnhancer::lastRenderedLineAnchors();
        QVERIFY(anchors.size() > 200);

        // Verify closest anchor for line 464
        const int anchor464 = MarkdownPreviewEnhancer::findClosestAnchorLine(464);
        QVERIFY(anchor464 > 0 && anchor464 <= 464);

        // Verify closest anchor for line 5954
        const int anchor5954 = MarkdownPreviewEnhancer::findClosestAnchorLine(5954);
        QVERIFY(anchor5954 > 0 && anchor5954 <= 5954);
    }
};
