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

#include "MarkdownSafePreviewDock.h"
#include "ScintillaNext.h"
#include "MainWindow.h"
#include <QComboBox>
#include <QDesktopServices>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QTemporaryFile>
#include <QTextDocument>
#include <QVBoxLayout>

MarkdownSafePreviewDock::MarkdownSafePreviewDock(MainWindow *parent)
    : QDockWidget(tr("HTML & Markdown Preview"), parent),
      browser(new QTextBrowser(this)),
      debounceTimer(new QTimer(this)),
      comboFormat(new QComboBox(this)),
      lblFormatStatus(new QLabel(this)),
      btnOpenBrowser(new QPushButton(this)),
      btnRefresh(new QPushButton(this)),
      btnZoomIn(new QPushButton(QStringLiteral("+"), this)),
      btnZoomOut(new QPushButton(QStringLiteral("−"), this)),
      btnResetZoom(new QPushButton(QStringLiteral("100%"), this)) {
    setObjectName(QStringLiteral("MarkdownSafePreviewDock"));

    // Restore persistent settings
    const QSettings settings;
    autoShowEnabled = settings.value(QStringLiteral("Gui/AutoShowMarkdownHtmlPreview"), false).toBool();
    const int savedFormat = settings.value(QStringLiteral("Gui/PreviewFormat"), 0).toInt();
    if (savedFormat >= 0 && savedFormat <= 2) {
        currentFormat = static_cast<PreviewFormat>(savedFormat);
    }

    setupUi();

    debounceTimer->setSingleShot(true);
    debounceTimer->setInterval(250);
    connect(debounceTimer, &QTimer::timeout, this, &MarkdownSafePreviewDock::renderPreview);
}

void MarkdownSafePreviewDock::setupUi() {
    auto *container = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    // Header Toolbar
    auto *topBar = new QWidget(container);
    auto *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(2, 2, 2, 2);
    topLayout->setSpacing(4);

    comboFormat->addItem(tr("Auto"), static_cast<int>(PreviewFormat::Auto));
    comboFormat->addItem(tr("Markdown"), static_cast<int>(PreviewFormat::Markdown));
    comboFormat->addItem(tr("HTML"), static_cast<int>(PreviewFormat::Html));
    comboFormat->setCurrentIndex(static_cast<int>(currentFormat));
    comboFormat->setToolTip(tr("Select preview rendering format (Auto detects Markdown or HTML)"));
    connect(comboFormat, &QComboBox::currentIndexChanged, this, &MarkdownSafePreviewDock::onFormatComboChanged);
    topLayout->addWidget(comboFormat);

    lblFormatStatus->setStyleSheet(QStringLiteral(
        "QLabel { font-size: 11px; padding: 2px 6px; border-radius: 3px; background-color: rgba(127,127,127,0.2); }"));
    lblFormatStatus->setText(tr("Auto"));
    topLayout->addWidget(lblFormatStatus);

    topLayout->addStretch(1);

    btnZoomOut->setToolTip(tr("Zoom Out"));
    btnZoomOut->setFixedWidth(24);
    connect(btnZoomOut, &QPushButton::clicked, this, &MarkdownSafePreviewDock::zoomOut);
    topLayout->addWidget(btnZoomOut);

    btnResetZoom->setToolTip(tr("Reset Zoom (100%)"));
    btnResetZoom->setFixedWidth(40);
    connect(btnResetZoom, &QPushButton::clicked, this, &MarkdownSafePreviewDock::resetZoom);
    topLayout->addWidget(btnResetZoom);

    btnZoomIn->setToolTip(tr("Zoom In"));
    btnZoomIn->setFixedWidth(24);
    connect(btnZoomIn, &QPushButton::clicked, this, &MarkdownSafePreviewDock::zoomIn);
    topLayout->addWidget(btnZoomIn);

    btnRefresh->setToolTip(tr("Refresh Preview"));
    btnRefresh->setText(QStringLiteral("↻"));
    btnRefresh->setFixedWidth(26);
    connect(btnRefresh, &QPushButton::clicked, this, &MarkdownSafePreviewDock::renderPreview);
    topLayout->addWidget(btnRefresh);

    btnOpenBrowser->setToolTip(tr("Open in Default Web Browser"));
    btnOpenBrowser->setText(QStringLiteral("🌐"));
    btnOpenBrowser->setFixedWidth(26);
    connect(btnOpenBrowser, &QPushButton::clicked, this, &MarkdownSafePreviewDock::openInBrowser);
    topLayout->addWidget(btnOpenBrowser);

    mainLayout->addWidget(topBar);

    // Hardened sandboxing: disable automatic link opening to intercept all navigation (CVE-2026-20841)
    browser->setOpenExternalLinks(false);
    browser->setOpenLinks(false);
    connect(browser, &QTextBrowser::anchorClicked, this, &MarkdownSafePreviewDock::handleAnchorClicked);

    // Apply GitHub Markdown styling
    applyGitHubTheme();

    mainLayout->addWidget(browser);
    setWidget(container);

    baseFontSize = browser->font().pointSize();
    if (baseFontSize <= 0) {
        baseFontSize = 13;
    }
}

void MarkdownSafePreviewDock::setActiveEditor(ScintillaNext *editor) {
    if (currentEditor == editor) {
        scheduleRender();
        return;
    }

    if (currentEditor) {
        disconnect(currentEditor, nullptr, this, nullptr);
    }

    currentEditor = editor;

    if (currentEditor) {
        connect(currentEditor, &ScintillaNext::notifyChange, this, &MarkdownSafePreviewDock::scheduleRender);
    }

    scheduleRender();
}

void MarkdownSafePreviewDock::scheduleRender() {
    if (isVisible()) {
        debounceTimer->start();
    }
}

MarkdownSafePreviewDock::PreviewFormat MarkdownSafePreviewDock::detectFormat(const ScintillaNext *editor) {
    if (!editor) {
        return PreviewFormat::Markdown;
    }

    const QString lang = editor->languageName.toLower();
    if (lang.contains(QStringLiteral("html")) || lang.contains(QStringLiteral("xml"))) {
        return PreviewFormat::Html;
    }
    if (lang.contains(QStringLiteral("markdown"))) {
        return PreviewFormat::Markdown;
    }

    const QString ext = editor->getFileInfo().suffix().toLower();
    if (ext == QStringLiteral("html") || ext == QStringLiteral("htm") || ext == QStringLiteral("xhtml") ||
        ext == QStringLiteral("shtml")) {
        return PreviewFormat::Html;
    }
    if (ext == QStringLiteral("md") || ext == QStringLiteral("markdown") || ext == QStringLiteral("mdown") ||
        ext == QStringLiteral("mkd") || ext == QStringLiteral("mkdn")) {
        return PreviewFormat::Markdown;
    }

    // Heuristic scan of first 400 bytes
    const QString snippet = QString::fromUtf8(editor->get_text_range(0, 400)).trimmed().toLower();
    if (snippet.startsWith(QStringLiteral("<!doctype html")) || snippet.startsWith(QStringLiteral("<html")) ||
        snippet.contains(QStringLiteral("<head>")) || snippet.contains(QStringLiteral("<body>")) ||
        snippet.contains(QStringLiteral("</div>")) || snippet.contains(QStringLiteral("</p>"))) {
        return PreviewFormat::Html;
    }

    return PreviewFormat::Markdown;
}

void MarkdownSafePreviewDock::changeEvent(QEvent *event) {
    QDockWidget::changeEvent(event);
    if (event->type() == QEvent::PaletteChange || event->type() == QEvent::ApplicationPaletteChange) {
        applyGitHubTheme();
        renderPreview();
    }
}

void MarkdownSafePreviewDock::applyGitHubTheme() {
    const bool isDark = palette().color(QPalette::Window).lightness() < 128;
    const QString bgColor = isDark ? QStringLiteral("#0d1117") : QStringLiteral("#ffffff");
    const QString fgColor = isDark ? QStringLiteral("#e6edf3") : QStringLiteral("#1f2328");
    const QString selColor =
        isDark ? QStringLiteral("rgba(56, 139, 253, 0.4)") : QStringLiteral("rgba(9, 105, 218, 0.25)");

    browser->setStyleSheet(
        QStringLiteral(
            "QTextBrowser { background-color: %1; color: %2; border: none; selection-background-color: %3; }")
            .arg(bgColor, fgColor, selColor));

    browser->document()->setDefaultStyleSheet(buildGitHubStyleSheet(isDark));
}

QString MarkdownSafePreviewDock::buildGitHubStyleSheet(bool isDark) const {
    const QString textColor = isDark ? QStringLiteral("#e6edf3") : QStringLiteral("#1f2328");
    const QString bgColor = isDark ? QStringLiteral("#0d1117") : QStringLiteral("#ffffff");
    const QString borderColor = isDark ? QStringLiteral("#30363d") : QStringLiteral("#d8dee4");
    const QString mutedColor = isDark ? QStringLiteral("#848d97") : QStringLiteral("#656d76");
    const QString codeBg =
        isDark ? QStringLiteral("rgba(110, 118, 129, 0.4)") : QStringLiteral("rgba(175, 184, 193, 0.2)");
    const QString blockBg = isDark ? QStringLiteral("#161b22") : QStringLiteral("#f6f8fa");
    const QString linkColor = isDark ? QStringLiteral("#2f81f7") : QStringLiteral("#0969da");

    return QStringLiteral(
               "body { "
               "  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Noto Sans', Helvetica, Arial, "
               "sans-serif; "
               "  font-size: %1px; line-height: 1.5; color: %2; background-color: %3; margin: 0; padding: 16px 20px; "
               "} "
               "h1, h2, h3, h4, h5, h6 { "
               "  font-weight: 600; line-height: 1.25; margin-top: 24px; margin-bottom: 16px; color: %2; "
               "} "
               "h1 { font-size: 2em; padding-bottom: 0.3em; border-bottom: 1px solid %4; } "
               "h2 { font-size: 1.5em; padding-bottom: 0.3em; border-bottom: 1px solid %4; } "
               "h3 { font-size: 1.25em; } "
               "h4 { font-size: 1em; } "
               "h5 { font-size: 0.875em; } "
               "h6 { font-size: 0.85em; color: %5; } "
               "p { margin-top: 0; margin-bottom: 16px; line-height: 1.5; } "
               "blockquote { "
               "  margin: 0 0 16px 0; padding: 0 1em; color: %5; border-left: 0.25em solid %4; "
               "} "
               "ul, ol { margin-top: 0; margin-bottom: 16px; padding-left: 2em; } "
               "li { margin-top: 0.25em; } "
               "code { "
               "  font-family: 'SFMono-Regular', Consolas, 'Liberation Mono', Menlo, monospace; "
               "  font-size: 85%; padding: 0.2em 0.4em; background-color: %6; border-radius: 6px; "
               "} "
               "pre { "
               "  margin-top: 0; margin-bottom: 16px; padding: 16px; line-height: 1.45; "
               "  background-color: %7; border-radius: 6px; "
               "  font-family: 'SFMono-Regular', Consolas, 'Liberation Mono', Menlo, monospace; font-size: 85%; "
               "} "
               "table { border-collapse: collapse; border-spacing: 0; margin-top: 0; margin-bottom: 16px; width: 100%; "
               "} "
               "th, td { padding: 6px 13px; border: 1px solid %4; } "
               "th { font-weight: 600; background-color: %7; } "
               "tr:nth-child(2n) { background-color: %7; } "
               "hr { height: 0.25em; padding: 0; margin: 24px 0; background-color: %4; border: 0; } "
               "a { color: %8; text-decoration: none; } "
               "kbd { "
               "  display: inline-block; padding: 3px 5px; font-size: 11px; line-height: 10px; color: %2; "
               "  background-color: %7; border: 1px solid %4; border-radius: 6px; "
               "} "
               "img { max-width: 100%; height: auto; }")
        .arg(QString::number(baseFontSize), textColor, bgColor, borderColor, mutedColor, codeBg, blockBg, linkColor);
}

QString MarkdownSafePreviewDock::generateFullHtmlDocument(const QString &bodyHtml, bool isDark) const {
    const QString css = buildGitHubStyleSheet(isDark);
    const QString title = currentEditor ? currentEditor->getFileInfo().fileName() : tr("SparkNotePad Preview");

    return QStringLiteral(
               "<!DOCTYPE html>\n"
               "<html>\n"
               "<head>\n"
               "<meta charset=\"utf-8\">\n"
               "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
               "<title>%1</title>\n"
               "<style>\n"
               "%2\n"
               "</style>\n"
               "</head>\n"
               "<body>\n"
               "%3\n"
               "</body>\n"
               "</html>\n")
        .arg(title.toHtmlEscaped(), css, bodyHtml);
}

void MarkdownSafePreviewDock::renderPreview() {
    if (!currentEditor) {
        browser->clear();
        if (lblFormatStatus) {
            lblFormatStatus->setText(tr("No Document"));
        }
        return;
    }

    applyGitHubTheme();

    const QString content = QString::fromUtf8(currentEditor->getText());
    const PreviewFormat effectiveFormat =
        (currentFormat == PreviewFormat::Auto) ? detectFormat(currentEditor) : currentFormat;

    updateFormatLabel(effectiveFormat);

    if (effectiveFormat == PreviewFormat::Html) {
        browser->setHtml(content);
    } else {
        browser->setMarkdown(content);
    }
}

void MarkdownSafePreviewDock::updateFormatLabel(PreviewFormat effectiveFormat) {
    if (!lblFormatStatus) {
        return;
    }

    if (currentFormat == PreviewFormat::Auto) {
        if (effectiveFormat == PreviewFormat::Html) {
            lblFormatStatus->setText(tr("Auto (HTML)"));
        } else {
            lblFormatStatus->setText(tr("Auto (Markdown)"));
        }
    } else if (currentFormat == PreviewFormat::Html) {
        lblFormatStatus->setText(tr("HTML"));
    } else {
        lblFormatStatus->setText(tr("Markdown"));
    }
}

void MarkdownSafePreviewDock::setPreviewFormat(PreviewFormat newFormat) {
    if (currentFormat == newFormat) {
        return;
    }
    currentFormat = newFormat;
    if (comboFormat && comboFormat->currentIndex() != static_cast<int>(newFormat)) {
        comboFormat->setCurrentIndex(static_cast<int>(newFormat));
    }
    QSettings settings;
    settings.setValue(QStringLiteral("Gui/PreviewFormat"), static_cast<int>(newFormat));
    emit formatChanged(newFormat);
    renderPreview();
}

void MarkdownSafePreviewDock::onFormatComboChanged(int index) {
    if (index >= 0 && index <= 2) {
        setPreviewFormat(static_cast<PreviewFormat>(index));
    }
}

void MarkdownSafePreviewDock::setAutoShowEnabled(bool enabled) {
    if (autoShowEnabled == enabled) {
        return;
    }
    autoShowEnabled = enabled;
    QSettings settings;
    settings.setValue(QStringLiteral("Gui/AutoShowMarkdownHtmlPreview"), enabled);
    emit autoShowChanged(enabled);
}

void MarkdownSafePreviewDock::openInBrowser() {
    if (!currentEditor) {
        return;
    }

    const PreviewFormat effectiveFormat =
        (currentFormat == PreviewFormat::Auto) ? detectFormat(currentEditor) : currentFormat;

    if (effectiveFormat == PreviewFormat::Html && currentEditor->isFile() && currentEditor->getFileInfo().exists()) {
        const QString path = currentEditor->getFileInfo().absoluteFilePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        return;
    }

    // Render full GitHub Markdown styled page
    const bool isDark = palette().color(QPalette::Window).lightness() < 128;
    const QString rawContent = QString::fromUtf8(currentEditor->getText());
    QString bodyHtml;
    if (effectiveFormat == PreviewFormat::Html) {
        bodyHtml = rawContent;
    } else {
        QTextDocument doc;
        doc.setMarkdown(rawContent);
        bodyHtml = doc.toHtml();
    }

    QTemporaryFile tempFile(QDir::tempPath() + QStringLiteral("/SparkNotePad_preview_XXXXXX.html"));
    tempFile.setAutoRemove(false);
    if (tempFile.open()) {
        const QString fullHtml = generateFullHtmlDocument(bodyHtml, isDark);
        tempFile.write(fullHtml.toUtf8());
        const QString tempPath = tempFile.fileName();
        tempFile.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(tempPath));
    }
}

void MarkdownSafePreviewDock::zoomIn() {
    browser->zoomIn(1);
}

void MarkdownSafePreviewDock::zoomOut() {
    browser->zoomOut(1);
}

void MarkdownSafePreviewDock::resetZoom() {
    QFont f = browser->font();
    f.setPointSize(baseFontSize);
    browser->setFont(f);
}

void MarkdownSafePreviewDock::handleAnchorClicked(const QUrl &url) {
    const QString scheme = url.scheme().toLower();

    // Whitelist: strictly allow only safe external protocols
    if (scheme == QStringLiteral("http") || scheme == QStringLiteral("https") || scheme == QStringLiteral("mailto")) {
        QDesktopServices::openUrl(url);
        return;
    }

    // Explicitly block and warn against local execution schemes (CVE-2026-20841)
    qWarning("Blocked unsafe Markdown/HTML preview link navigation: %s", qUtf8Printable(url.toString()));
    QMessageBox::warning(
        this, tr("Blocked Untrusted Link"),
        tr("Navigation to link '%1' with scheme '%2' is blocked for security.\n\n"
           "Only http, https, and mailto URLs are permitted in HTML & Markdown Preview (mitigating CVE-2026-20841).")
            .arg(url.toString(), scheme));
}
