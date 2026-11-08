#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QFile>
#include <QObject>
#include <QTemporaryDir>
#include <QTest>
#include "features/DocumentExportManager.h"

class TestDocumentExportManager : public QObject {
    Q_OBJECT

private slots:
    void testMarkdownToHtmlConversion() const {
        const QString md = QStringLiteral(
            "# SparkNotePad 1.4.5\n\n**Pure Qt 6** native export pipeline.\n\n- Feature 1\n- Feature 2\n");
        DocumentExportManager::ExportOptions opt;
        opt.includeCss = false;

        const QString html = DocumentExportManager::markdownToHtml(md, opt);
        QVERIFY(!html.isEmpty());
        QVERIFY(html.contains(QStringLiteral("SparkNotePad 1.4.5")));
        QVERIFY(html.contains(QStringLiteral("Pure Qt 6")));
    }

    void testHtmlStylingAndCssEmbedding() const {
        const QString md = QStringLiteral("## Section Header\nSome regular paragraph text.");
        DocumentExportManager::ExportOptions optLight;
        optLight.includeCss = true;
        optLight.darkTheme = false;
        optLight.title = QStringLiteral("Test Light");

        const QString lightHtml = DocumentExportManager::markdownToHtml(md, optLight);
        QVERIFY(lightHtml.contains(QStringLiteral("<title>Test Light</title>")));
        QVERIFY(lightHtml.contains(QStringLiteral("<style>")));
        QVERIFY(lightHtml.contains(QStringLiteral("#24292e")) || lightHtml.contains(QStringLiteral("#000000")));

        DocumentExportManager::ExportOptions optDark;
        optDark.includeCss = true;
        optDark.darkTheme = true;
        optDark.title = QStringLiteral("Test Dark");

        const QString darkHtml = DocumentExportManager::markdownToHtml(md, optDark);
        QVERIFY(darkHtml.contains(QStringLiteral("<title>Test Dark</title>")));
        QVERIFY(darkHtml.contains(QStringLiteral("#0d1117")) || darkHtml.contains(QStringLiteral("#1e1e1e")));
    }

    void testExportMarkdownToHtmlFile() const {
        QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());

        const QString outputPath = tempDir.filePath(QStringLiteral("export_test.html"));
        const QString md = QStringLiteral("# Test Export File\nDocument exported successfully.");

        DocumentExportManager::ExportOptions opt;
        opt.title = QStringLiteral("File Export Test");
        const bool ok = DocumentExportManager::exportMarkdownToHtml(md, outputPath, opt);
        QVERIFY(ok);

        QFile f(outputPath);
        QVERIFY(f.exists());
        QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
        const QString content = QString::fromUtf8(f.readAll());
        QVERIFY(content.contains(QStringLiteral("Test Export File")));
        QVERIFY(content.contains(QStringLiteral("<html")));
    }

    void testExportToPdfFile() const {
        QTemporaryDir tempDir;
        QVERIFY(tempDir.isValid());

        const QString outputPath = tempDir.filePath(QStringLiteral("export_test.pdf"));
        const QString md =
            QStringLiteral("# PDF Test Document\n\nTesting pure Qt 6 PDF printing.\n\nParagraph text line.");

        DocumentExportManager::ExportOptions opt;
        opt.title = QStringLiteral("PDF Test");
        opt.pageSize = QPageSize::A4;
        opt.orientation = QPageLayout::Orientation::Portrait;

        const bool ok = DocumentExportManager::exportToPdf(md, true, outputPath, opt);
        QVERIFY(ok);

        QFile f(outputPath);
        QVERIFY(f.exists());
        QVERIFY(f.size() > 0);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const QByteArray header = f.read(5);
        QCOMPARE(header, QByteArray("%PDF-"));
    }

    void testPandocDetection() const {
        // Just verify API call runs deterministically without crashing
        const bool available = DocumentExportManager::isPandocAvailable();
        (void)available;
        QVERIFY(true);
    }
};
