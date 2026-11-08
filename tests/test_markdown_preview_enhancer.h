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

class TestMarkdownPreviewEnhancer : public QObject {
    Q_OBJECT

private slots:
    void testProcessMermaidBlocks() const {
        const QString input = QStringLiteral("Intro\n```mermaid\ngraph TD\n    A-->B\n```\nOutro");
        const QString result = MarkdownPreviewEnhancer::processMermaidBlocks(input, false);

        QVERIFY(result.contains(QStringLiteral("class=\"mermaid-diagram\"")) ||
                result.contains(QStringLiteral("mermaid")));
        QVERIFY(result.contains(QStringLiteral("graph TD")));
        QVERIFY(result.contains(QStringLiteral("A--&gt;B")) || result.contains(QStringLiteral("A-->B")));
    }

    void testProcessBlockMath() const {
        const QString input = QStringLiteral("Formula:\n$$\nE = mc^2\n$$\nDone");
        const QString result = MarkdownPreviewEnhancer::processMathExpressions(input);

        QVERIFY(result.contains(QStringLiteral("class=\"math-block\"")));
        QVERIFY(result.contains(QStringLiteral("E = mc^2")));
    }

    void testProcessInlineMath() const {
        const QString input = QStringLiteral("The equation is $f(x) = x^2$ in math.");
        const QString result = MarkdownPreviewEnhancer::processMathExpressions(input);

        QVERIFY(result.contains(QStringLiteral("class=\"math-inline\"")));
        QVERIFY(result.contains(QStringLiteral("f(x) = x^2")));
    }

    void testPreserveCurrencyAndEscapedDollars() const {
        const QString input = QStringLiteral("The price is $50 and another is $100. Escaped \\$variable.");
        const QString result = MarkdownPreviewEnhancer::processMathExpressions(input);

        // Price should not be turned into inline math
        QVERIFY(!result.contains(QStringLiteral("class=\"math-inline\"")));
        QVERIFY(result.contains(QStringLiteral("$50")));
    }

    void testXssSanitization() const {
        const QString malicious =
            QStringLiteral("```mermaid\n<script>alert('xss')</script>\n```\n$$<img src=x onerror=alert(1)>$$");
        const QString enhanced = MarkdownPreviewEnhancer::enhanceMarkdown(malicious, false);

        QVERIFY(!enhanced.contains(QStringLiteral("<script>")));
        QVERIFY(!enhanced.contains(QStringLiteral("<img src=x")));
        QVERIFY(enhanced.contains(QStringLiteral("&lt;script&gt;")) || enhanced.contains(QStringLiteral("&lt;img")));
    }
};
