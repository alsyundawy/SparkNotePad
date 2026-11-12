#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QObject>
#include <QTest>
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

    void testHtmlPathRegex() const {
        const QStringList validHtml = {QStringLiteral("index.html"),         QStringLiteral("page.htm"),
                                       QStringLiteral("server.shtml"),       QStringLiteral("doc.xhtml"),
                                       QStringLiteral("test.xht"),           QStringLiteral("dynamic.dhtml"),
                                       QStringLiteral("site.html#header"),   QStringLiteral("view.htm?param=1"),
                                       QStringLiteral("/var/www/INDEX.HTML")};

        for (const QString &path : validHtml) {
            QVERIFY2(MarkdownPreviewEnhancer::isHtmlPath(path),
                     qPrintable(QStringLiteral("Should match html: %1").arg(path)));
        }

        const QStringList invalidHtml = {QStringLiteral("doc.md"), QStringLiteral("main.cpp"),
                                         QStringLiteral("data.json")};

        for (const QString &path : invalidHtml) {
            QVERIFY2(!MarkdownPreviewEnhancer::isHtmlPath(path),
                     qPrintable(QStringLiteral("Should NOT match html: %1").arg(path)));
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
};
