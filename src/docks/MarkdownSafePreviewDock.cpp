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
#include <QApplication>
#include <QClipboard>
#include <QComboBox>
#include <QDesktopServices>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollBar>
#include <QSettings>
#include <QTemporaryFile>
#include <QTextDocument>
#include <QVBoxLayout>
#include <algorithm>
#include <array>
#include "ScintillaNext.h"
#include "MainWindow.h"
#include "../dialogs/ExportOptionsDialog.h"
#include "../features/DocumentExportManager.h"
#include "../features/ThemeManager.h"
#include "../features/ThemePaletteData.h"
#include "../utils/MarkdownPreviewEnhancer.h"
#include "../utils/MarkdownThemeManager.h"

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

constexpr std::array<CalloutDef, 5> s_dockCallouts = {{
    {"[!NOTE]", "ℹ️", "Note", "#0969da", "#2f81f7", "rgba(9, 105, 218, 0.08)", "rgba(47, 129, 247, 0.12)"},
    {"[!TIP]", "💡", "Tip", "#1a7f37", "#3fb950", "rgba(26, 127, 55, 0.08)", "rgba(63, 185, 80, 0.12)"},
    {"[!IMPORTANT]", "🟣", "Important", "#8250df", "#a371f7", "rgba(130, 80, 223, 0.08)", "rgba(163, 113, 247, 0.12)"},
    {"[!WARNING]", "⚠️", "Warning", "#9a6700", "#d29922", "rgba(154, 103, 0, 0.08)", "rgba(210, 153, 34, 0.12)"},
    {"[!CAUTION]", "🛑", "Caution", "#cf222e", "#f85149", "rgba(207, 34, 46, 0.08)", "rgba(248, 81, 73, 0.12)"},
}};

QString cleanDockCalloutInner(const QString &raw) {
    QString inner = raw.trimmed();
    if (inner.startsWith(QStringLiteral("<br />"))) {
        return inner.mid(6).trimmed();
    }
    if (inner.startsWith(QStringLiteral("<br>"))) {
        return inner.mid(4).trimmed();
    }
    if (inner.startsWith(QStringLiteral("<br/>"))) {
        return inner.mid(5).trimmed();
    }
    return inner;
}

void replaceSingleDockCallout(QString &result, const CalloutDef &c, qsizetype &idx, bool isDark) {
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
    const QString inner = cleanDockCalloutInner(result.mid(contentStart, bqEnd - contentStart));
    const QString color = isDark ? QString::fromLatin1(c.colorDark) : QString::fromLatin1(c.colorLight);
    const QString bg = isDark ? QString::fromLatin1(c.bgDark) : QString::fromLatin1(c.bgLight);
    const QString replacement = QStringLiteral(
                                    "<div style=\"margin: 12px 0; padding: 10px 14px; border-left: 4px solid %1; "
                                    "background-color: %2; border-radius: 4px;\">"
                                    "<div style=\"font-weight: 600; color: %1; margin-bottom: 4px;\">%3 %4</div>"
                                    "<div>%5</div></div>")
                                    .arg(color, bg, QString::fromUtf8(c.icon),
                                         QCoreApplication::translate("MarkdownSafePreviewDock", c.title), inner);

    result.replace(bqStart, (bqEnd + 13) - bqStart, replacement);
    idx = bqStart + replacement.length();
}

QString enhanceCallouts(const QString &html, bool isDark) {
    QString result = html;
    for (const auto &c : s_dockCallouts) {
        const QString marker = QString::fromUtf8(c.tag);
        qsizetype idx = 0;
        while ((idx = result.indexOf(marker, idx, Qt::CaseInsensitive)) != -1) {
            replaceSingleDockCallout(result, c, idx, isDark);
        }
    }
    return result;
}

QString generateFullHtmlDocument(const QString &bodyHtml, const QString &title, MarkdownThemeManager::ThemeId theme,
                                 int fontSize) {
    const QString css = MarkdownThemeManager::generateThemeCss(theme, fontSize);
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

void updateFormatLabel(QLabel *lblFormatStatus, MarkdownSafePreviewDock::PreviewFormat currentFormat,
                       MarkdownSafePreviewDock::PreviewFormat effectiveFormat) {
    if (!lblFormatStatus) {
        return;
    }

    using enum MarkdownSafePreviewDock::PreviewFormat;
    if (currentFormat == Auto) {
        if (effectiveFormat == Html) {
            lblFormatStatus->setText(QCoreApplication::translate("MarkdownSafePreviewDock", "Auto (HTML)"));
        } else {
            lblFormatStatus->setText(QCoreApplication::translate("MarkdownSafePreviewDock", "Auto (Markdown)"));
        }
    } else if (currentFormat == Html) {
        lblFormatStatus->setText(QCoreApplication::translate("MarkdownSafePreviewDock", "HTML"));
    } else {
        lblFormatStatus->setText(QCoreApplication::translate("MarkdownSafePreviewDock", "Markdown"));
    }
}
}  // namespace

MarkdownSafePreviewDock::MarkdownSafePreviewDock(MainWindow *parent)
    : QDockWidget(tr("HTML & Markdown Live Preview"), parent),
      browser(new QTextBrowser(this)),
      debounceTimer(new QTimer(this)) {
    m_ui.comboFormat = new QComboBox(this);
    m_ui.comboTheme = new QComboBox(this);
    m_ui.lblFormatStatus = new QLabel(this);
    m_ui.lblStats = new QLabel(this);
    m_ui.btnOpenBrowser = new QPushButton(this);
    m_ui.btnCopyHtml = new QPushButton(this);
    m_ui.btnExportHtml = new QPushButton(this);
    m_ui.btnExportPdf = new QPushButton(this);
    m_ui.btnRefresh = new QPushButton(this);
    m_ui.btnZoomIn = new QPushButton(this);
    m_ui.btnZoomOut = new QPushButton(this);
    m_ui.btnResetZoom = new QPushButton(QStringLiteral("100%"), this);

    setObjectName(QStringLiteral("MarkdownSafePreviewDock"));
    browser->setObjectName(QStringLiteral("markdownPreviewBrowser"));

    // Restore persistent settings
    const QSettings settings;
    autoShowEnabled = settings.value(QStringLiteral("Gui/AutoShowMarkdownHtmlPreview"), false).toBool();
    if (const int savedFormat = settings.value(QStringLiteral("Gui/PreviewFormat"), 0).toInt();
        savedFormat >= 0 && savedFormat <= 2) {
        currentFormat = static_cast<PreviewFormat>(savedFormat);
    }
    if (const QString savedThemeName = settings.value(QStringLiteral("Gui/MarkdownPreviewTheme"), QString()).toString();
        !savedThemeName.isEmpty()) {
        currentTheme = MarkdownThemeManager::themeFromName(savedThemeName);
        m_userExplicitTheme = true;
    } else {
        const bool isDark = ThemeManager::instance()->isDarkActive();
        currentTheme = MarkdownThemeManager::defaultTheme(isDark);
    }

    setupUi();

    debounceTimer->setSingleShot(true);
    debounceTimer->setInterval(200);
    connect(debounceTimer, &QTimer::timeout, this, &MarkdownSafePreviewDock::renderPreview);

    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this, &MarkdownSafePreviewDock::applyGitHubTheme);
    connect(ThemeManager::instance(), &ThemeManager::paletteChanged, this, &MarkdownSafePreviewDock::applyGitHubTheme);
    applyGitHubTheme();
}

void MarkdownSafePreviewDock::setupUi() {
    auto *container = new QWidget(this);
    container->setObjectName(QStringLiteral("previewContainer"));
    auto *mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    // Header Toolbar
    auto *topBar = new QWidget(container);
    topBar->setObjectName(QStringLiteral("previewTopBar"));
    auto *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(6, 4, 6, 4);
    topLayout->setSpacing(5);

    m_ui.comboFormat->addItem(tr("Auto"), static_cast<int>(PreviewFormat::Auto));
    m_ui.comboFormat->addItem(tr("Markdown"), static_cast<int>(PreviewFormat::Markdown));
    m_ui.comboFormat->addItem(tr("HTML"), static_cast<int>(PreviewFormat::Html));
    m_ui.comboFormat->setCurrentIndex(static_cast<int>(currentFormat));
    m_ui.comboFormat->setToolTip(tr("Select preview rendering format (Auto detects Markdown or HTML)"));
    connect(m_ui.comboFormat, &QComboBox::currentIndexChanged, this, [this](int index) {
        if (index >= 0 && index <= 2) {
            setPreviewFormat(static_cast<PreviewFormat>(index));
        }
    });
    topLayout->addWidget(m_ui.comboFormat);

    const QStringList themeList = MarkdownThemeManager::themeNames();
    for (int i = 0; i < themeList.size(); ++i) {
        m_ui.comboTheme->addItem(themeList[i], i);
    }
    m_ui.comboTheme->setCurrentIndex(static_cast<int>(currentTheme));
    m_ui.comboTheme->setToolTip(tr("Select Markdown theme (30 cleanrmd / markdown-it themes)"));
    connect(m_ui.comboTheme, &QComboBox::currentIndexChanged, this, [this](int index) {
        if (index >= 0 && index < static_cast<int>(MarkdownThemeManager::themeNames().size())) {
            setPreviewTheme(static_cast<MarkdownThemeManager::ThemeId>(index));
        }
    });
    topLayout->addWidget(m_ui.comboTheme);

    m_ui.lblFormatStatus->setObjectName(QStringLiteral("lblFormatStatus"));
    m_ui.lblFormatStatus->setText(tr("Auto"));
    topLayout->addWidget(m_ui.lblFormatStatus);

    m_ui.lblStats->setObjectName(QStringLiteral("lblPreviewStats"));
    m_ui.lblStats->setText(tr("0 words • 0 min read"));
    topLayout->addWidget(m_ui.lblStats);

    topLayout->addStretch(1);

    m_ui.btnZoomOut->setIcon(QIcon(QStringLiteral(":/icons/zoomOut.svg")));
    m_ui.btnZoomOut->setIconSize(QSize(14, 14));
    m_ui.btnZoomOut->setToolTip(tr("Zoom Out"));
    m_ui.btnZoomOut->setFixedSize(28, 24);
    m_ui.btnZoomOut->setCursor(Qt::PointingHandCursor);
    connect(m_ui.btnZoomOut, &QPushButton::clicked, this, [this] { browser->zoomOut(1); });
    topLayout->addWidget(m_ui.btnZoomOut);

    m_ui.btnResetZoom->setText(QStringLiteral("100%"));
    m_ui.btnResetZoom->setToolTip(tr("Reset Zoom (100%)"));
    m_ui.btnResetZoom->setFixedHeight(24);
    m_ui.btnResetZoom->setMinimumWidth(48);
    m_ui.btnResetZoom->setCursor(Qt::PointingHandCursor);
    connect(m_ui.btnResetZoom, &QPushButton::clicked, this, [this] {
        QFont f = browser->font();
        f.setPointSize(baseFontSize);
        browser->setFont(f);
    });
    topLayout->addWidget(m_ui.btnResetZoom);

    m_ui.btnZoomIn->setIcon(QIcon(QStringLiteral(":/icons/zoomIn.svg")));
    m_ui.btnZoomIn->setIconSize(QSize(14, 14));
    m_ui.btnZoomIn->setToolTip(tr("Zoom In"));
    m_ui.btnZoomIn->setFixedSize(28, 24);
    m_ui.btnZoomIn->setCursor(Qt::PointingHandCursor);
    connect(m_ui.btnZoomIn, &QPushButton::clicked, this, [this] { browser->zoomIn(1); });
    topLayout->addWidget(m_ui.btnZoomIn);

    m_ui.btnRefresh->setIcon(QIcon(QStringLiteral(":/icons/arrow_refresh.svg")));
    m_ui.btnRefresh->setIconSize(QSize(14, 14));
    m_ui.btnRefresh->setToolTip(tr("Refresh Preview"));
    m_ui.btnRefresh->setFixedSize(28, 24);
    m_ui.btnRefresh->setCursor(Qt::PointingHandCursor);
    connect(m_ui.btnRefresh, &QPushButton::clicked, this, &MarkdownSafePreviewDock::renderPreview);
    topLayout->addWidget(m_ui.btnRefresh);

    m_ui.btnCopyHtml->setIcon(QIcon(QStringLiteral(":/icons/copy.svg")));
    m_ui.btnCopyHtml->setIconSize(QSize(14, 14));
    m_ui.btnCopyHtml->setToolTip(tr("Copy Formatted HTML to Clipboard"));
    m_ui.btnCopyHtml->setFixedSize(28, 24);
    m_ui.btnCopyHtml->setCursor(Qt::PointingHandCursor);
    connect(m_ui.btnCopyHtml, &QPushButton::clicked, this, &MarkdownSafePreviewDock::copyHtml);
    topLayout->addWidget(m_ui.btnCopyHtml);

    m_ui.btnExportHtml->setIcon(QIcon(QStringLiteral(":/icons/export.svg")));
    m_ui.btnExportHtml->setIconSize(QSize(14, 14));
    m_ui.btnExportHtml->setToolTip(tr("Export as Standalone HTML File..."));
    m_ui.btnExportHtml->setFixedSize(28, 24);
    m_ui.btnExportHtml->setCursor(Qt::PointingHandCursor);
    connect(m_ui.btnExportHtml, &QPushButton::clicked, this, &MarkdownSafePreviewDock::exportHtml);
    topLayout->addWidget(m_ui.btnExportHtml);

    m_ui.btnExportPdf->setText(QStringLiteral("PDF"));
    m_ui.btnExportPdf->setToolTip(tr("Export Document to PDF..."));
    m_ui.btnExportPdf->setFixedHeight(24);
    m_ui.btnExportPdf->setCursor(Qt::PointingHandCursor);
    connect(m_ui.btnExportPdf, &QPushButton::clicked, this, &MarkdownSafePreviewDock::exportPdf);
    topLayout->addWidget(m_ui.btnExportPdf);

    m_ui.btnOpenBrowser->setIcon(QIcon(QStringLiteral(":/icons/globe.svg")));
    m_ui.btnOpenBrowser->setIconSize(QSize(14, 14));
    m_ui.btnOpenBrowser->setText(QStringLiteral("Browser"));
    m_ui.btnOpenBrowser->setToolTip(tr("Open in Default Web Browser"));
    m_ui.btnOpenBrowser->setFixedHeight(24);
    m_ui.btnOpenBrowser->setCursor(Qt::PointingHandCursor);
    connect(m_ui.btnOpenBrowser, &QPushButton::clicked, this, &MarkdownSafePreviewDock::openInBrowser);
    topLayout->addWidget(m_ui.btnOpenBrowser);

    mainLayout->addWidget(topBar);

    // Hardened sandboxing: disable automatic link opening to intercept all navigation (CVE-2026-20841)
    browser->setOpenExternalLinks(false);
    browser->setOpenLinks(false);
    connect(browser, &QTextBrowser::anchorClicked, this, &MarkdownSafePreviewDock::handleAnchorClicked);
    connect(browser->verticalScrollBar(), &QScrollBar::valueChanged, this,
            &MarkdownSafePreviewDock::syncScrollFromBrowser);

    // Apply styling
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
        return;
    }

    if (currentEditor) {
        disconnect(currentEditor, &ScintillaNext::notifyChange, this, &MarkdownSafePreviewDock::scheduleRender);
        disconnect(currentEditor, &ScintillaNext::updateUi, this, &MarkdownSafePreviewDock::syncScrollFromEditor);
    }

    currentEditor = editor;

    if (currentEditor) {
        connect(currentEditor, &ScintillaNext::notifyChange, this, &MarkdownSafePreviewDock::scheduleRender);
        connect(currentEditor, &ScintillaNext::updateUi, this, &MarkdownSafePreviewDock::syncScrollFromEditor);
        renderPreview();
    } else {
        browser->clear();
    }
}

void MarkdownSafePreviewDock::scheduleRender() {
    if (isVisible() && currentEditor) {
        debounceTimer->start();
    }
}

MarkdownSafePreviewDock::PreviewFormat MarkdownSafePreviewDock::detectFormat(const ScintillaNext *editor) {
    using enum PreviewFormat;
    if (!editor) {
        return Markdown;
    }

    const QString path = editor->isFile() ? editor->getFileInfo().filePath() : editor->getName();
    if (MarkdownPreviewEnhancer::isHtmlPath(path)) {
        return Html;
    }
    if (MarkdownPreviewEnhancer::isMarkdownPath(path)) {
        return Markdown;
    }

    const QString lang = editor->languageName.toLower();
    if (lang.contains(QStringLiteral("html")) || lang.contains(QStringLiteral("xml"))) {
        return Html;
    }
    if (lang.contains(QStringLiteral("markdown"))) {
        return Markdown;
    }

    // Heuristic scan of first 400 bytes
    if (const QString snippet = QString::fromUtf8(editor->get_text_range(0, 400)).trimmed().toLower();
        snippet.startsWith(QStringLiteral("<!doctype html")) || snippet.startsWith(QStringLiteral("<html")) ||
        snippet.contains(QStringLiteral("<head>")) || snippet.contains(QStringLiteral("<body>")) ||
        snippet.contains(QStringLiteral("</div>")) || snippet.contains(QStringLiteral("</p>"))) {
        return Html;
    }

    return Markdown;
}

void MarkdownSafePreviewDock::changeEvent(QEvent *event) {
    if (event->type() == QEvent::PaletteChange) {
        applyGitHubTheme();
        renderPreview();
    }
    QDockWidget::changeEvent(event);
}

void MarkdownSafePreviewDock::applyGitHubTheme() {
    if (m_isUpdatingTheme) {
        return;
    }
    m_isUpdatingTheme = true;

    const bool isDark = ThemeManager::instance()->isDarkActive();
    if (!m_userExplicitTheme) {
        currentTheme = MarkdownThemeManager::defaultTheme(isDark);
        if (m_ui.comboTheme) {
            m_ui.comboTheme->blockSignals(true);
            m_ui.comboTheme->setCurrentIndex(static_cast<int>(currentTheme));
            m_ui.comboTheme->blockSignals(false);
        }
    }

    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    QString accent;
    QString dockBg;
    QString topBarBg;
    QString textPri;
    QString textSec;
    QString border;
    QString btnBg;

    if (pal) {
        accent = QString::fromLatin1(pal->blue);
        dockBg = QString::fromLatin1(pal->win);
        topBarBg = QString::fromLatin1(pal->card);
        textPri = QString::fromLatin1(pal->txtPri);
        textSec = QString::fromLatin1(pal->txtSec);
        border = QString::fromLatin1(pal->border);
        btnBg = QString::fromLatin1(pal->card);
    } else if (isDark) {
        accent = QStringLiteral("#58a6ff");
        dockBg = QStringLiteral("#161b22");
        topBarBg = QStringLiteral("#21262d");
        textPri = QStringLiteral("#e6edf3");
        textSec = QStringLiteral("#8b949e");
        border = QStringLiteral("#30363d");
        btnBg = QStringLiteral("#21262d");
    } else {
        accent = QStringLiteral("#0969da");
        dockBg = QStringLiteral("#f6f8fa");
        topBarBg = QStringLiteral("#ffffff");
        textPri = QStringLiteral("#1f2328");
        textSec = QStringLiteral("#57606a");
        border = QStringLiteral("#d0d7de");
        btnBg = QStringLiteral("#f6f8fa");
    }

    const QString badgeBg = isDark ? QStringLiteral("rgba(56, 139, 253, 0.15)") : QStringLiteral("rgba(9, 105, 218, 0.1)");

    const QColor themeBg = MarkdownThemeManager::themeBackgroundColor(currentTheme);
    const QColor themeFg = MarkdownThemeManager::themeTextColor(currentTheme);
    const QString selColor = isDark ? QStringLiteral("rgba(56, 139, 253, 0.4)") : QStringLiteral("rgba(9, 105, 218, 0.25)");

    const QString dockStyle = QStringLiteral(
                                  "QDockWidget {"
                                  "  background-color: %1;"
                                  "  color: %2;"
                                  "}"
                                  "QDockWidget::title {"
                                  "  background-color: %1;"
                                  "  color: %2;"
                                  "  padding: 6px 10px;"
                                  "  border-bottom: 2px solid %3;"
                                  "  font-weight: 600;"
                                  "  font-size: 11px;"
                                  "}"
                                  "QWidget#previewTopBar {"
                                  "  background-color: %4;"
                                  "  border: 1px solid %5;"
                                  "  border-radius: 6px;"
                                  "}"
                                  "QWidget#previewTopBar QComboBox {"
                                  "  background-color: %6;"
                                  "  color: %2;"
                                  "  border: 1px solid %5;"
                                  "  border-radius: 4px;"
                                  "  padding: 2px 6px;"
                                  "  font-size: 11px;"
                                  "}"
                                  "QWidget#previewTopBar QComboBox:hover {"
                                  "  border-color: %3;"
                                  "}"
                                  "QLabel#lblFormatStatus {"
                                  "  font-size: 10px;"
                                  "  font-weight: 600;"
                                  "  padding: 2px 8px;"
                                  "  border-radius: 10px;"
                                  "  background-color: %7;"
                                  "  color: %3;"
                                  "  border: 1px solid %3;"
                                  "}"
                                  "QLabel#lblPreviewStats {"
                                  "  font-size: 10px;"
                                  "  color: %8;"
                                  "  padding: 2px 6px;"
                                  "}"
                                  "QWidget#previewTopBar QPushButton {"
                                  "  background-color: %6;"
                                  "  color: %2;"
                                  "  border: 1px solid %5;"
                                  "  border-radius: 4px;"
                                  "  font-size: 11px;"
                                  "  padding: 2px 5px;"
                                  "  min-height: 20px;"
                                  "}"
                                  "QWidget#previewTopBar QPushButton:hover {"
                                  "  border-color: %3;"
                                  "  color: %3;"
                                  "}"
                                  "QWidget#previewTopBar QPushButton:pressed {"
                                  "  background-color: %3;"
                                  "  color: #ffffff;"
                                  "}")
                                  .arg(dockBg, textPri, accent, topBarBg, border, btnBg, badgeBg, textSec);
    setStyleSheet(dockStyle);

    browser->setStyleSheet(
        QStringLiteral(
            "#markdownPreviewBrowser {"
            "  background-color: %1;"
            "  color: %2;"
            "  border: none;"
            "  selection-background-color: %3;"
            "}")
            .arg(themeBg.name(), themeFg.name(), selColor));

    browser->document()->setDefaultStyleSheet(MarkdownThemeManager::generateThemeCss(currentTheme, baseFontSize));
    m_isUpdatingTheme = false;
}

void MarkdownSafePreviewDock::setPreviewTheme(MarkdownThemeManager::ThemeId newTheme) {
    currentTheme = newTheme;
    m_userExplicitTheme = true;
    if (m_ui.comboTheme) {
        m_ui.comboTheme->blockSignals(true);
        m_ui.comboTheme->setCurrentIndex(static_cast<int>(newTheme));
        m_ui.comboTheme->blockSignals(false);
    }
    QSettings settings;
    settings.setValue(QStringLiteral("Gui/MarkdownPreviewTheme"), MarkdownThemeManager::themeName(currentTheme));
    applyGitHubTheme();
    renderPreview();
}

void MarkdownSafePreviewDock::renderPreview() {
    using enum PreviewFormat;
    if (!currentEditor) {
        browser->clear();
        if (m_ui.lblFormatStatus) {
            m_ui.lblFormatStatus->setText(tr("No Document"));
        }
        if (m_ui.lblStats) {
            m_ui.lblStats->setText(tr("0 words • 0 min read"));
        }
        return;
    }

    const PreviewFormat effectiveFormat =
        (currentFormat == Auto) ? detectFormat(currentEditor) : currentFormat;
    updateFormatLabel(m_ui.lblFormatStatus, currentFormat, effectiveFormat);

    const QString text = QString::fromUtf8(currentEditor->getText());
    if (m_ui.lblStats) {
        const QStringList words = text.split(QRegularExpression(QStringLiteral("\\s+")), Qt::SkipEmptyParts);
        const qsizetype wordCount = words.size();
        const qsizetype minutes = std::max(static_cast<qsizetype>(1), (wordCount + 199) / 200);
        m_ui.lblStats->setText(tr("%1 words • %2 min read").arg(wordCount).arg(minutes));
    }

    const bool isDark = MarkdownThemeManager::isThemeDark(currentTheme);

    // Save scroll position ratio
    double scrollRatio = 0.0;
    const auto *scrollBar = browser->verticalScrollBar();
    if (scrollBar && scrollBar->maximum() > 0) {
        scrollRatio = static_cast<double>(scrollBar->value()) / static_cast<double>(scrollBar->maximum());
    }

    if (effectiveFormat == Html) {
        browser->setHtml(text);
    } else {
        const QString enrichedMd = MarkdownPreviewEnhancer::enhanceMarkdown(text, isDark);
        QTextDocument doc;
        doc.setMarkdown(enrichedMd);
        const QString enhanced = enhanceCallouts(doc.toHtml(), isDark);
        browser->setHtml(enhanced);
    }

    if (auto *const bar = browser->verticalScrollBar(); bar && bar->maximum() > 0 && scrollRatio > 0.0) {
        bar->setValue(static_cast<int>(scrollRatio * bar->maximum()));
    }
}

void MarkdownSafePreviewDock::setPreviewFormat(PreviewFormat newFormat) {
    if (currentFormat == newFormat) {
        return;
    }
    currentFormat = newFormat;
    if (m_ui.comboFormat && m_ui.comboFormat->currentIndex() != static_cast<int>(newFormat)) {
        m_ui.comboFormat->setCurrentIndex(static_cast<int>(newFormat));
    }
    QSettings settings;
    settings.setValue(QStringLiteral("Gui/PreviewFormat"), static_cast<int>(newFormat));
    emit formatChanged(newFormat);
    renderPreview();
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

void MarkdownSafePreviewDock::copyHtml() {
    using enum PreviewFormat;
    if (!currentEditor) {
        return;
    }
    const QString rawContent = QString::fromUtf8(currentEditor->getText());
    const PreviewFormat effectiveFormat =
        (currentFormat == Auto) ? detectFormat(currentEditor) : currentFormat;
    QString bodyHtml;
    if (effectiveFormat == Html) {
        bodyHtml = rawContent;
    } else {
        QTextDocument doc;
        doc.setMarkdown(rawContent);
        bodyHtml = doc.toHtml();
    }
    const QString fileName = currentEditor->getFileInfo().fileName();
    const QString title = fileName.isEmpty() ? tr("SparkNotePad Preview") : fileName;
    const QString fullHtml = generateFullHtmlDocument(bodyHtml, title, currentTheme, baseFontSize);
    QApplication::clipboard()->setText(fullHtml);
    if (m_ui.lblFormatStatus) {
        const QString orig = m_ui.lblFormatStatus->text();
        m_ui.lblFormatStatus->setText(tr("HTML Copied!"));
        QTimer::singleShot(2000, this, [this, orig] {
            if (m_ui.lblFormatStatus) {
                m_ui.lblFormatStatus->setText(orig);
            }
        });
    }
}

void MarkdownSafePreviewDock::exportHtml() {
    using enum PreviewFormat;
    if (!currentEditor) {
        return;
    }
    QString defaultName = currentEditor->getName();
    if (defaultName.isEmpty()) {
        defaultName = tr("Untitled");
    }
    if (!defaultName.endsWith(QStringLiteral(".html"), Qt::CaseInsensitive)) {
        if (const auto dot = defaultName.lastIndexOf('.'); dot > 0) {
            defaultName = defaultName.left(dot);
        }
        defaultName += QStringLiteral(".html");
    }

    ExportOptionsDialog dlg(ExportOptionsDialog::TargetFormat::Html, defaultName, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    const QString savePath = QFileDialog::getSaveFileName(this, tr("Export HTML Document"), defaultName,
                                                          tr("HTML Documents (*.html *.htm);;All Files (*)"));
    if (savePath.isEmpty()) {
        return;
    }
    const QString rawContent = QString::fromUtf8(currentEditor->getText());
    const PreviewFormat effectiveFormat =
        (currentFormat == Auto) ? detectFormat(currentEditor) : currentFormat;

    if (effectiveFormat == Html) {
        QFile file(savePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            file.write(rawContent.toUtf8());
            file.close();
        }
    } else {
        DocumentExportManager::exportMarkdownToHtml(rawContent, savePath, dlg.options());
    }
}

void MarkdownSafePreviewDock::exportPdf() {
    using enum PreviewFormat;
    if (!currentEditor) {
        return;
    }
    QString defaultName = currentEditor->getName();
    if (defaultName.isEmpty()) {
        defaultName = tr("Untitled");
    }
    if (!defaultName.endsWith(QStringLiteral(".pdf"), Qt::CaseInsensitive)) {
        if (const auto dot = defaultName.lastIndexOf('.'); dot > 0) {
            defaultName = defaultName.left(dot);
        }
        defaultName += QStringLiteral(".pdf");
    }

    ExportOptionsDialog dlg(ExportOptionsDialog::TargetFormat::Pdf, defaultName, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    const QString savePath = QFileDialog::getSaveFileName(this, tr("Export PDF Document"), defaultName,
                                                          tr("PDF Documents (*.pdf);;All Files (*)"));
    if (savePath.isEmpty()) {
        return;
    }

    const QString rawContent = QString::fromUtf8(currentEditor->getText());
    const PreviewFormat effectiveFormat =
        (currentFormat == Auto) ? detectFormat(currentEditor) : currentFormat;
    const bool isMarkdown = (effectiveFormat != Html);

    DocumentExportManager::exportToPdf(rawContent, isMarkdown, savePath, dlg.options());
}

void MarkdownSafePreviewDock::openInBrowser() {
    using enum PreviewFormat;
    if (!currentEditor) {
        return;
    }

    const PreviewFormat effectiveFormat =
        (currentFormat == Auto) ? detectFormat(currentEditor) : currentFormat;

    if (effectiveFormat == Html && currentEditor->isFile() && currentEditor->getFileInfo().exists()) {
        const QString path = currentEditor->getFileInfo().absoluteFilePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        return;
    }

    // Render full styled page
    const QString rawContent = QString::fromUtf8(currentEditor->getText());
    QString bodyHtml;
    if (effectiveFormat == Html) {
        bodyHtml = rawContent;
    } else {
        QTextDocument doc;
        doc.setMarkdown(rawContent);
        bodyHtml = doc.toHtml();
    }

    QTemporaryFile tempFile(QDir::tempPath() + QStringLiteral("/SparkNotePad_preview_XXXXXX.html"));
    tempFile.setAutoRemove(false);
    if (tempFile.open()) {
        const QString fileName = currentEditor->getFileInfo().fileName();
        const QString title = fileName.isEmpty() ? tr("SparkNotePad Preview") : fileName;
        const QString fullHtml = generateFullHtmlDocument(bodyHtml, title, currentTheme, baseFontSize);
        tempFile.write(fullHtml.toUtf8());
        const QString tempPath = tempFile.fileName();
        tempFile.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(tempPath));
    }
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

void MarkdownSafePreviewDock::syncScrollFromEditor(Scintilla::Update updated) {
    if (m_syncingScroll || !currentEditor || !isVisible()) {
        return;
    }
    if (!FlagSet(updated, Scintilla::Update::VScroll) && !FlagSet(updated, Scintilla::Update::Content)) {
        return;
    }
    const auto totalLines = static_cast<int>(currentEditor->lineCount());
    const auto linesOnScreen = static_cast<int>(currentEditor->linesOnScreen());
    const int maxScrollLine = std::max(1, totalLines - linesOnScreen);
    const auto currentFirstLine = static_cast<int>(currentEditor->firstVisibleLine());
    const double ratio =
        std::clamp(static_cast<double>(currentFirstLine) / static_cast<double>(maxScrollLine), 0.0, 1.0);

    auto *const scrollBar = browser->verticalScrollBar();
    if (scrollBar && scrollBar->maximum() > 0) {
        m_syncingScroll = true;
        scrollBar->setValue(static_cast<int>(ratio * scrollBar->maximum()));
        m_syncingScroll = false;
    }
}

void MarkdownSafePreviewDock::syncScrollFromBrowser() {
    if (m_syncingScroll || !currentEditor || !isVisible()) {
        return;
    }
    const auto *scrollBar = browser->verticalScrollBar();
    if (!scrollBar || scrollBar->maximum() <= 0) {
        return;
    }
    const double ratio =
        std::clamp(static_cast<double>(scrollBar->value()) / static_cast<double>(scrollBar->maximum()), 0.0, 1.0);
    const auto totalLines = static_cast<int>(currentEditor->lineCount());
    const auto linesOnScreen = static_cast<int>(currentEditor->linesOnScreen());
    const int maxScrollLine = std::max(1, totalLines - linesOnScreen);
    const auto targetLine = static_cast<int>(ratio * static_cast<double>(maxScrollLine));

    m_syncingScroll = true;
    currentEditor->setFirstVisibleLine(targetLine);
    m_syncingScroll = false;
}
