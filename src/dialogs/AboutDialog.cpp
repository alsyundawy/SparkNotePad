/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AboutDialog.h"
#include "../features/ThemeManager.h"
#include "../features/ThemePaletteData.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGuiApplication>
#include <QClipboard>
#include <QSysInfo>
#include <QScreen>
#include <QPixmap>
#include <QScrollArea>
#include <QTabBar>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    setupUi();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this, &AboutDialog::updateThemeColors);
    connect(ThemeManager::instance(), &ThemeManager::paletteChanged, this, &AboutDialog::updateThemeColors);
    updateThemeColors();
}

void AboutDialog::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QDialog::changeEvent(event);
}

void AboutDialog::setupUi() {
    setWindowTitle(tr("About SparkNotePad"));
    setMinimumSize(680, 520);
    resize(720, 560);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->tabBar()->setElideMode(Qt::ElideNone);
    m_tabWidget->setUsesScrollButtons(true);

    m_tabWidget->addTab(createAboutTab(), tr("&About"));
    m_tabWidget->setTabToolTip(0, tr("About SparkNotePad"));
    m_tabWidget->addTab(createMaintainerTab(), tr("&Maintainer"));
    m_tabWidget->setTabToolTip(1, tr("Maintainer & Official Support Channels"));
    m_tabWidget->addTab(createCreditsTab(), tr("&Credits"));
    m_tabWidget->setTabToolTip(2, tr("Credits & Attribution"));
    m_tabWidget->addTab(createEnvironmentTab(), tr("&Diagnostics"));
    m_tabWidget->setTabToolTip(3, tr("System & Engine Diagnostics"));
    m_tabWidget->addTab(createShortcutsTab(), tr("&Shortcuts"));
    m_tabWidget->setTabToolTip(4, tr("Key Productivity Shortcuts"));
    m_tabWidget->addTab(createLicenseTab(), tr("&License"));
    m_tabWidget->setTabToolTip(5, tr("GNU General Public License v3.0"));

    mainLayout->addWidget(m_tabWidget);

    auto *buttonBox = new QDialogButtonBox(this);
    m_closeBtn = buttonBox->addButton(tr("&Close"), QDialogButtonBox::RejectRole);
    if (m_closeBtn) {
        m_closeBtn->setCursor(Qt::PointingHandCursor);
    }
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}

void AboutDialog::retranslateUi() {
    setWindowTitle(tr("About SparkNotePad"));
    if (m_tabWidget) {
        m_tabWidget->setTabText(0, tr("&About"));
        m_tabWidget->setTabToolTip(0, tr("About SparkNotePad"));
        m_tabWidget->setTabText(1, tr("&Maintainer"));
        m_tabWidget->setTabToolTip(1, tr("Maintainer & Official Support Channels"));
        m_tabWidget->setTabText(2, tr("&Credits"));
        m_tabWidget->setTabToolTip(2, tr("Credits & Attribution"));
        m_tabWidget->setTabText(3, tr("&Diagnostics"));
        m_tabWidget->setTabToolTip(3, tr("System & Engine Diagnostics"));
        m_tabWidget->setTabText(4, tr("&Shortcuts"));
        m_tabWidget->setTabToolTip(4, tr("Key Productivity Shortcuts"));
        m_tabWidget->setTabText(5, tr("&License"));
        m_tabWidget->setTabToolTip(5, tr("GNU General Public License v3.0"));
    }
    if (m_closeBtn) {
        m_closeBtn->setText(tr("&Close"));
    }
    updateThemeColors();
}

void AboutDialog::updateThemeColors() {
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    bool dark = ThemeManager::instance()->isDarkActive();

    QString winBg;
    QString cardBg;
    QString borderCol;
    QString accentCol;
    QString txtPri;
    QString txtSec;

    if (pal) {
        winBg = QString::fromLatin1(pal->win);
        cardBg = QString::fromLatin1(pal->card);
        borderCol = QString::fromLatin1(pal->border);
        accentCol = QString::fromLatin1(pal->blue);
        txtPri = QString::fromLatin1(pal->txtPri);
        txtSec = QString::fromLatin1(pal->txtSec);
    } else if (dark) {
        winBg = QStringLiteral("#0f172a");
        cardBg = QStringLiteral("#1e293b");
        borderCol = QStringLiteral("#334155");
        accentCol = QStringLiteral("#38bdf8");
        txtPri = QStringLiteral("#f8fafc");
        txtSec = QStringLiteral("#94a3b8");
    } else {
        winBg = QStringLiteral("#f8fafc");
        cardBg = QStringLiteral("#ffffff");
        borderCol = QStringLiteral("#cbd5e1");
        accentCol = QStringLiteral("#2563eb");
        txtPri = QStringLiteral("#0f172a");
        txtSec = QStringLiteral("#475569");
    }

    QString badgeBg = dark ? QStringLiteral("rgba(255,255,255,0.08)") : QStringLiteral("rgba(0,0,0,0.05)");
    QString panelBg = dark ? QStringLiteral("rgba(15, 23, 42, 0.65)") : QStringLiteral("rgba(241, 245, 249, 0.85)");
    QString cardStyle =
        QString(
            "QLabel {"
            "  padding: 10px 12px;"
            "  background-color: %1;"
            "  border: 1px solid %2;"
            "  border-left: 4px solid %3;"
            "  border-radius: 6px;"
            "  color: %4;"
            "  font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', 'Segoe UI', Roboto, sans-serif;"
            "  font-size: 11px;"
            "}")
            .arg(panelBg, borderCol, accentCol, txtPri);

    QString dialogStyle =
        QString(
            "QDialog {"
            "  background-color: %1;"
            "  color: %4;"
            "  font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', 'Segoe UI', Roboto, sans-serif;"
            "  font-size: 12px;"
            "}"
            "QTabWidget::pane {"
            "  background-color: %2;"
            "  border: 1px solid %3;"
            "  border-radius: 6px;"
            "}"
            "QTabWidget > QWidget,"
            "QTabWidget > QWidget > QWidget,"
            "QWidget#aboutTab,"
            "QWidget#aboutContent,"
            "QWidget#maintainerTab,"
            "QWidget#creditsTab,"
            "QWidget#diagnosticsTab,"
            "QWidget#diagnosticsContent,"
            "QWidget#shortcutsTab,"
            "QWidget#licenseTab {"
            "  background-color: %2;"
            "}"
            "QScrollArea,"
            "QScrollArea > QWidget,"
            "QScrollArea > QWidget > QWidget {"
            "  background-color: %2;"
            "  border: none;"
            "}"
            "QTabBar {"
            "  background: transparent;"
            "  border: none;"
            "  qproperty-drawBase: 0;"
            "}"
            "QTabBar::tab {"
            "  background-color: %1;"
            "  color: %5;"
            "  padding: 6px 14px;"
            "  border: 1px solid %3;"
            "  border-bottom: none;"
            "  border-top-left-radius: 5px;"
            "  border-top-right-radius: 5px;"
            "  margin-right: 2px;"
            "  font-size: 11px;"
            "  font-weight: 500;"
            "}"
            "QTabBar::tab:selected {"
            "  background-color: %2;"
            "  color: %4;"
            "  font-weight: bold;"
            "  border-top: 2px solid %6;"
            "}"
            "QTabBar::tab:hover:!selected {"
            "  background-color: %3;"
            "  color: %4;"
            "}"
            "QLabel {"
            "  color: %4;"
            "}"
            "QTextBrowser {"
            "  background-color: %2;"
            "  color: %4;"
            "  border: none;"
            "  padding: 4px;"
            "  font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', 'Segoe UI', Roboto, sans-serif;"
            "  font-size: 11px;"
            "}"
            "QPushButton {"
            "  background-color: %2;"
            "  color: %4;"
            "  border: 1px solid %3;"
            "  border-radius: 5px;"
            "  padding: 6px 14px;"
            "  font-size: 11px;"
            "  font-weight: 600;"
            "}"
            "QPushButton:hover {"
            "  border-color: %6;"
            "  color: %6;"
            "}"
            "QPushButton:pressed {"
            "  background-color: %6;"
            "  color: #ffffff;"
            "}"
            "QScrollBar:vertical {"
            "  background: transparent;"
            "  width: 8px;"
            "  margin: 0;"
            "}"
            "QScrollBar::handle:vertical {"
            "  background: %3;"
            "  min-height: 20px;"
            "  border-radius: 4px;"
            "}"
            "QScrollBar::handle:vertical:hover {"
            "  background: %6;"
            "}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
            "  height: 0px;"
            "}")
            .arg(winBg, cardBg, borderCol, txtPri, txtSec, accentCol);

    setStyleSheet(dialogStyle);

    if (m_titleLabel) {
        m_titleLabel->setText(
            QString("<h3 style='color: %1; margin: 0 0 3px 0; font-size: 15px;'>SparkNotePad v%2 "
                    "<span style='font-size: 10px; padding: 2px 6px; background-color: %3; color: #ffffff; "
                    "border-radius: 4px; font-weight: bold;'>%4</span></h3>"
                    "<p style='margin: 0; font-weight: 600; font-size: 11px; color: %1;'>%5</p>"
                    "<p style='margin: 2px 0 0 0; font-style: italic; font-size: 10px; color: %6;'>%7</p>")
                .arg(txtPri, APP_VERSION, accentCol, tr("STABLE"),
                     tr("Independent, Ultra-Lightweight & High-Performance Text Editor"), txtSec,
                     tr("Powered natively by Vendored Scintilla 5.6.x & Lexilla 5.5.x with Modern Qt 6")));
    }

    if (m_badgesLabel) {
        m_badgesLabel->setText(
            QString("<p style='margin-top: 5px; margin-bottom: 0; font-size: 10px; line-height: 1.6;'>"
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>%4</span> "
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>📐 %5</span> "
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>🔤 %6</span> "
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>📄 %7</span> "
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>⚡ Scintilla 5.6.x</span>"
                    "</p>")
                .arg(badgeBg, txtPri, borderCol, tr("75 Themes"), tr("Math Eval"), tr("100+ Lexers"),
                     tr("Export PDF")));
    }

    if (m_noticeBox) {
        m_noticeBox->setStyleSheet(cardStyle);
        m_noticeBox->setText(
            QString("<b style='color: %1; font-size: 11px;'>%2</b><br/>"
                    "<span style='color: %1; font-size: 11px;'>%3</span>")
                .arg(txtPri, tr("Important Legal & Non-Affiliation Statement:"),
                     tr("SparkNotePad is an independent open-source project. It is not affiliated with, "
                        "endorsed by, or associated with Notepad++, NotepadNext, Notepad4, or Notepad3.")));
    }

    if (m_metaLabel) {
        m_metaLabel->setStyleSheet(cardStyle);
        m_metaLabel->setText(
            QString("<div style='color: %1; font-size: 11px; line-height: 1.5;'>"
                    "<div style='margin-bottom: 5px;'><b style='color: %1;'>%2:</b> "
                    "<a style='color: %3; text-decoration: none; font-weight: 600;' "
                    "href='https://github.com/alsyundawy/SparkNotePad'>📦 "
                    "https://github.com/alsyundawy/SparkNotePad</a></div>"
                    "<div style='margin-bottom: 5px;'><b style='color: %1;'>%4:</b> "
                    "<a style='text-decoration: none;' "
                    "href='https://github.com/alsyundawy/SparkNotePad/releases/tag/v1.3.7'>"
                    "<span style='background-color: #238636; color: #ffffff; padding: 2px 8px; border-radius: 10px; "
                    "font-weight: bold; font-size: 10px;'>🏷️ v1.3.7 %5</span></a></div>"
                    "<div style='margin-bottom: 5px;'><b style='color: %1;'>%6:</b> "
                    "<b>Harry Dertin Sutisna Alsyundawy</b> (<a style='color: %3; text-decoration: none;' "
                    "href='https://github.com/alsyundawy'>@alsyundawy</a>) — <b>ALSYUNDAWY IT SOLUTION</b></div>"
                    "<div><b style='color: %1;'>%7:</b> GNU General Public License v3.0 &bull; Scintilla 5.6.7 + "
                    "Lexilla 5.5.4 &bull; Qt 6</div>"
                    "</div>")
                .arg(txtPri, tr("GitHub Repository:"), accentCol, tr("Release Tag:"), tr("Official Release"),
                     tr("Lead Maintainer:"), tr("License & Engine:")));
    }

    if (m_diagnosticsCard) {
        m_diagnosticsCard->setStyleSheet(cardStyle);
        const QScreen *primary = QGuiApplication::primaryScreen();
        qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

        QString infoText =
            QString(
                "<div style='color: %1; line-height: 1.6; font-size: 11px;'>"
                "<b>%2:</b> %3 (%4)<br/>"
                "<b>%5:</b> %6 (%7: %8)<br/>"
                "<b>%9:</b> %10 (%11 %12)<br/>"
                "<b>%13:</b> %14x (Retina / High-DPI)<br/>"
                "<b>%15:</b> v%16<br/>"
                "<b>%17:</b> %18<br/>"
                "<b>%19:</b> Scintilla 5.6.7 &bull; Lexilla 5.5.4 &bull; Lua 5.5.1<br/>"
                "<b>%20:</b> Google DeepMind Antigravity IDE &bull; Gemini 3.8 Flash"
                "</div>")
                .arg(txtPri, tr("Operating System:"), QSysInfo::prettyProductName(), QSysInfo::kernelVersion(),
                     tr("Architecture:"), QSysInfo::currentCpuArchitecture(), tr("Build Target:"),
                     QSysInfo::buildCpuArchitecture(), tr("Qt Version:"), qVersion(), tr("Built with"), QT_VERSION_STR,
                     tr("Display Scale Ratio:"), QString::number(dpr, 'f', 1), tr("SparkNotePad Version:"), APP_VERSION,
                     tr("Kernel Type:"), QSysInfo::kernelType(), tr("Core Engines:"), tr("AI Code Review & Auditor:"));

        m_diagnosticsCard->setText(infoText);
    }

    if (m_maintainerBrowser) {
        m_maintainerBrowser->setHtml(
            QString(
                "<div style='color: %1; line-height: 1.5; font-size: 11px;'>"
                "<h3 style='color: %1; margin-top: 0; font-size: 13px;'>%4</h3>"
                "<p><b>%5:</b> HARRY DERTIN SUTISNA ALSYUNDAWY (<code>@alsyundawy</code>)<br/>"
                "<b>%6:</b> ALSYUNDAWY IT SOLUTION<br/>"
                "<b>%7:</b> <a style='color: %2;' href='https://alsyundawy.com'>https://alsyundawy.com</a><br/>"
                "<b>%8:</b> <a style='color: %2;' href='mailto:alsyundawy@gmail.com'>alsyundawy@gmail.com</a><br/>"
                "<b>%9:</b> <a style='color: %2;' href='https://github.com/alsyundawy'>@alsyundawy</a><br/>"
                "<b>%10:</b> <a style='color: %2;' href='https://t.me/alsyundawy'>@alsyundawy</a></p>"
                "<hr style='border: 0; border-top: 1px solid %3; margin: 10px 0;'/>"
                "<h4 style='color: %1; font-size: 12px; margin: 6px 0;'>%11</h4>"
                "<p>%12</p>"
                "<ul>"
                "<li>💳 <b>PayPal:</b> <a style='color: %2;' "
                "href='https://www.paypal.me/alsyundawy'>paypal.me/alsyundawy</a></li>"
                "<li>🇮🇩 <b>QRIS (NMID: ID1020021153676):</b> <a style='color: %2;' "
                "href='https://github.com/user-attachments/assets/a0126f28-6dde-43da-ba14-d7c9a27de0df'>%13</a></li>"
                "</ul>"
                "</div>")
                .arg(txtPri, accentCol, borderCol, tr("Maintainer & Official Support Channels"),
                     tr("Lead Architect & Maintainer:"), tr("Organization:"), tr("Official Website:"),
                     tr("Official Email:"), tr("GitHub Profile:"), tr("Telegram Channel:"),
                     tr("Sponsorship & Donation Channels"),
                     tr("If you find SparkNotePad useful for your development and daily workflow, consider supporting "
                        "our ongoing development:"),
                     tr("View Official QRIS Barcode")));
    }

    if (m_creditsBrowser) {
        QString creditsHtml;
        creditsHtml += QString(
                           "<div style='color: %1; line-height: 1.6; font-size: 11px;'>"
                           "<h3 style='color: %1; margin-top: 0; font-size: 13px; font-weight: bold;'>%3</h3>"
                           "<p>%4</p>")
                           .arg(txtPri, accentCol, tr("Credits, Resources & Upstream Attribution"),
                                tr("SparkNotePad stands on the shoulders of giants. We express our deepest gratitude "
                                   "to the following projects, communities, and tools (v1.0.0 – v1.3.7):"));

        creditsHtml += QString(
                           "<h4 style='color: %1; margin: 12px 0 4px 0; font-size: 12px;'>🤖 %2</h4>"
                           "<ul>"
                           "<li><b>Google DeepMind Antigravity IDE:</b> %3 (<a style='color: %1;' "
                           "href='https://github.com/google-deepmind'>google-deepmind</a>)</li>"
                           "<li><b>Gemini 3.8 Flash (Google DeepMind):</b> %4</li>"
                           "</ul>")
                           .arg(accentCol, tr("AI Engineering, Code Review & Auditing"),
                                tr("Lead AI Pair Programming IDE & Agentic Workflow Environment driving architecture, "
                                   "refactoring, and quality assurance."),
                                tr("Continuous AI Code Review, 13-Pillar Security Auditing, Static Analysis, and "
                                   "Linter Compliance Engine."));

        creditsHtml +=
            QString(
                "<h4 style='color: %1; margin: 12px 0 4px 0; font-size: 12px;'>🌿 %2</h4>"
                "<ul>"
                "<li><b>NotepadNext:</b> %3 (<a style='color: %1;' "
                "href='https://github.com/dail8859/NotepadNext'>github.com/dail8859/NotepadNext</a>)</li>"
                "<li><b>Notepad4:</b> %4 (<a style='color: %1;' "
                "href='https://github.com/zufuliu/notepad4'>github.com/zufuliu/notepad4</a>)</li>"
                "<li><b>Notepad3:</b> %5 (<a style='color: %1;' "
                "href='https://www.rizonesoft.com'>rizonesoft.com</a>)</li>"
                "<li><b>Notepad--:</b> %6 (<a style='color: %1;' "
                "href='https://github.com/cxasm/notepad--'>github.com/cxasm/notepad--</a>)</li>"
                "<li><b>Notepad++:</b> %7 (<a style='color: %1;' "
                "href='https://notepad-plus-plus.org'>notepad-plus-plus.org</a>)</li>"
                "<li><b>SubnetCalc for macOS:</b> %8 (<a style='color: %1;' "
                "href='https://github.com/alsyundawy/SubnetCalc-MacOS'>github.com/alsyundawy/SubnetCalc-MacOS</a>)</li>"
                "</ul>")
                .arg(accentCol, tr("Upstream Projects & Peer Editors"),
                     tr("Created by Justin Dailey (dail8859). Provided the cross-platform Qt scaffolding and docking "
                        "framework foundation."),
                     tr("Created by Zufu Liu (zufuliu). Architectural ideas, lexer updates, and utility concepts."),
                     tr("Created by Rizonesoft and Florian Balmer. Scintilla power-user text transformation patterns "
                        "and UI polish."),
                     tr("Created by cxasm. Cross-platform text comparison, encoding detection, and editor concepts."),
                     tr("Created by Don Ho. The legendary open-source editor that inspired the notepad generation."),
                     tr("Inspired UI elegance, theme architecture, and release documentation standards."));

        creditsHtml +=
            QString(
                "<h4 style='color: %1; margin: 12px 0 4px 0; font-size: 12px;'>⚡ %2</h4>"
                "<ul>"
                "<li><b>Scintilla 5.6.7:</b> %3 (<a style='color: %1;' "
                "href='https://www.scintilla.org'>scintilla.org</a>)</li>"
                "<li><b>Lexilla 5.5.4:</b> %4 (<a style='color: %1;' "
                "href='https://www.scintilla.org/Lexilla.html'>scintilla.org/Lexilla.html</a>)</li>"
                "<li><b>Lua 5.5.1 / 5.4.x:</b> %5 (<a style='color: %1;' href='https://www.lua.org'>lua.org</a>)</li>"
                "</ul>")
                .arg(accentCol, tr("Core Editing Engines & Lexers"),
                     tr("Created by Neil Hodgson and contributors. Core editing component providing high-performance "
                        "text rendering, multi-caret editing, and fold margins."),
                     tr("Created by Neil Hodgson and contributors. Syntax highlighting and styling lexers for over 100 "
                        "programming languages."),
                     tr("Created by Roberto Ierusalimschy, Waldemar Celes, and Luiz Henrique de Figueiredo. Embedded "
                        "scripting engine."));

        creditsHtml +=
            QString(
                "<h4 style='color: %1; margin: 12px 0 4px 0; font-size: 12px;'>📦 %2</h4>"
                "<ul>"
                "<li><b>The Qt Project:</b> %3 (<a style='color: %1;' href='https://www.qt.io'>qt.io</a>)</li>"
                "<li><b>Qt Advanced Docking System (ADS 5.1.1):</b> %4 (<a style='color: %1;' "
                "href='https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System'>github.com</a>)</li>"
                "<li><b>QSimpleUpdater:</b> %5 (<a style='color: %1;' "
                "href='https://github.com/alex-spataru/QSimpleUpdater'>github.com</a>)</li>"
                "<li><b>SingleApplication (3.5.6):</b> %6 (<a style='color: %1;' "
                "href='https://github.com/itay-grudev/SingleApplication'>github.com</a>)</li>"
                "<li><b>EditorConfig Core Qt:</b> %7 (<a style='color: %1;' "
                "href='https://github.com/editorconfig/editorconfig-core-qt'>github.com</a>)</li>"
                "<li><b>uchardet:</b> %8 (<a style='color: %1;' "
                "href='https://www.freedesktop.org/wiki/Software/uchardet/'>freedesktop.org</a>)</li>"
                "<li><b>OpenSSL:</b> %9 (<a style='color: %1;' href='https://www.openssl.org'>openssl.org</a>)</li>"
                "</ul>")
                .arg(accentCol, tr("Libraries & Frameworks"),
                     tr("Developed by The Qt Company and the Qt Open Source Community (Qt 6.5–6.12)."),
                     tr("Created by Manuel Freiholz. Advanced dock widget management and movable panels."),
                     tr("Created by Alex Spataru. Multi-platform automatic updater integration."),
                     tr("Created by Itay Grudev. Secure single-instance process and IPC communication."),
                     tr("Created by Hong Xu. Universal .editorconfig format enforcement."),
                     tr("Maintained by Freedesktop. Enables fast universal character encoding detection."),
                     tr("Standard open-source cryptography and TLS/SSL security library."));

        creditsHtml +=
            QString(
                "<h4 style='color: %1; margin: 12px 0 4px 0; font-size: 12px;'>🎨 %2</h4>"
                "<ul>"
                "<li><b>Lon McGregor (NotepadPlusPlusIcons):</b> %3 (<a style='color: %1;' "
                "href='https://github.com/LonMcGregor/NotepadPlusPlusIcons'>github.com</a>)</li>"
                "<li><b>SparkNotePad Monoline Vector Suite:</b> %4</li>"
                "</ul>"
                "</div>")
                .arg(accentCol, tr("Visual Artwork & Design"),
                     tr("Designer of the open-source editor and chameleon artwork series, adapted for SparkNotePad "
                        "official branding."),
                     tr("Modern high-DPI Retina vector icons customized for 75 themes, menus, and toolbars."));

        m_creditsBrowser->setHtml(creditsHtml);
    }

    if (m_shortcutsBrowser) {
        m_shortcutsBrowser->setHtml(
            QString(
                "<div style='color: %1; line-height: 1.5; font-size: 11px;'>"
                "<h3 style='color: %1; margin-top: 0; font-size: 13px;'>%3</h3>"
                "<table border='0' cellspacing='4' cellpadding='2' width='100%' style='color: %1; font-size: 11px;'>"
                "<tr><td colspan='2' style='color: %2; font-weight: bold; padding-top: 4px;'>%4</td></tr>"
                "<tr><td style='color: %2; font-weight: bold; width: 190px;'>Ctrl / Cmd + N</td><td>%5</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + N</td><td>%6</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + P</td><td>%7</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + P</td><td>%8</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Alt + D</td><td>%9</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + E</td><td>%10</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + T</td><td>%11</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + F</td><td>%12</td></tr>"
                "<tr><td colspan='2' style='color: %2; font-weight: bold; padding-top: 8px;'>%13</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Alt / Opt + F1</td><td>%14</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Alt / Opt + F2</td><td>%15</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Alt / Opt + F3</td><td>%16</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Alt+Shift+F1 / Ctrl+Alt+W</td><td>%17</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>F5</td><td>%18</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + F5</td><td>%19</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Alt + T</td><td>%20</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Alt + R</td><td>%21</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Alt + C</td><td>%22</td></tr>"
                "<tr><td colspan='2' style='color: %2; font-weight: bold; padding-top: 8px;'>%23</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + D</td><td>%24</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + /</td><td>%25</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + /</td><td>%26</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + U</td><td>%27</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + U</td><td>%28</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + B</td><td>%29</td></tr>"
                "<tr><td style='color: %2; font-weight: bold;'>Alt + Click &amp; Drag</td><td>%30</td></tr>"
                "<tr><td colspan='2' style='color: %2; font-style: italic; padding-top: 8px;'>%31</td></tr>"
                "</table>"
                "</div>")
                .arg(txtPri, accentCol, tr("SparkNotePad Keyboard Shortcuts &amp; Hotkeys"),
                     tr("Core &amp; Productivity"), tr("Create New File"), tr("Open New Window (Independent Instance)"),
                     tr("Command Palette (Quick Actions &amp; Navigation)"), tr("Quick Open File / Print Document"),
                     tr("Side-by-Side Dual-Editor Diff View"), tr("Evaluate Inline Math Expression"),
                     tr("Interactive Theme Switcher (75 Palettes)"), tr("Find in Files / Workspace"),
                     tr("Developer &amp; Documentation Tools"), tr("Get PHP Help (php.net)"),
                     tr("Rust Documentation (rust-lang.org/learn)"), tr("Python Documentation (w3schools.com/python)"),
                     tr("Wikipedia Search"), tr("Run External Tool"), tr("Run Current File with Interpreter"),
                     tr("Open Terminal Here"), tr("Run Python Script / Selection (3.13+)"),
                     tr("Generate Development SSL Certificate (mkcert/OpenSSL)"), tr("Editing &amp; Selection"),
                     tr("Duplicate Current Line or Selection"), tr("Toggle Single-Line Comment"),
                     tr("Toggle Block / Stream Comment"), tr("Convert Selection to UPPERCASE"),
                     tr("Convert Selection to lowercase"), tr("Toggle Clipboard Monitoring"),
                     tr("Column / Rectangular Box Selection"),
                     tr("Customize all shortcuts anytime in <b>Preferences &gt; Shortcuts</b> (or <b>Settings &gt; "
                        "Shortcut Mapper</b>).")));
    }

    if (m_licenseBrowser) {
        m_licenseBrowser->setHtml(
            QString(
                "<div style='color: %1; line-height: 1.5; font-size: 11px;'>"
                "<h3 style='color: %1; margin-top: 0; font-size: 13px;'>%3</h3>"
                "<p>%4</p>"
                "<p>%5</p>"
                "<p>%6 <a style='color: %2;' href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a></p>"
                "</div>")
                .arg(txtPri, accentCol, tr("GNU General Public License v3.0"),
                     tr("SparkNotePad is free software: you can redistribute it and/or modify "
                        "it under the terms of the GNU General Public License as published by "
                        "the Free Software Foundation, either version 3 of the License, or "
                        "(at your option) any later version."),
                     tr("This program is distributed in the hope that it will be useful, "
                        "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
                        "GNU General Public License for more details."),
                     tr("For complete license details, visit:")));
    }
}

QWidget *AboutDialog::createAboutTab() {
    auto *tab = new QWidget(this);
    tab->setObjectName(QStringLiteral("aboutTab"));
    auto *tabLayout = new QVBoxLayout(tab);
    tabLayout->setContentsMargins(0, 0, 0, 0);

    auto *scroll = new QScrollArea(tab);
    scroll->setObjectName(QStringLiteral("aboutScroll"));
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setFrameShape(QFrame::NoFrame);

    auto *contentWidget = new QWidget(scroll);
    contentWidget->setObjectName(QStringLiteral("aboutContent"));
    auto *layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(10);

    auto *headerLayout = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    headerLayout->setSpacing(14);

    auto *iconLabel = new QLabel(contentWidget);
    if (const QIcon appIcon(QStringLiteral(":/icons/SparkNotePad.svg")); !appIcon.isNull()) {
        iconLabel->setPixmap(appIcon.pixmap(56, 56));
    }
    iconLabel->setFixedSize(56, 56);
    headerLayout->addWidget(iconLabel, 0, Qt::AlignTop);

    auto *titleLayout = new QVBoxLayout();  // NOSONAR(cpp:S5025)
    titleLayout->setSpacing(3);

    m_titleLabel = new QLabel(contentWidget);
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    titleLayout->addWidget(m_titleLabel);

    m_badgesLabel = new QLabel(contentWidget);
    m_badgesLabel->setWordWrap(true);
    m_badgesLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    titleLayout->addWidget(m_badgesLabel);

    headerLayout->addLayout(titleLayout, 1);
    layout->addLayout(headerLayout);

    m_noticeBox = new QLabel(contentWidget);
    m_noticeBox->setWordWrap(true);
    m_noticeBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->addWidget(m_noticeBox);

    m_metaLabel = new QLabel(contentWidget);
    m_metaLabel->setOpenExternalLinks(true);
    m_metaLabel->setWordWrap(true);
    m_metaLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->addWidget(m_metaLabel);

    layout->addStretch();
    scroll->setWidget(contentWidget);
    tabLayout->addWidget(scroll);
    return tab;
}

QWidget *AboutDialog::createMaintainerTab() {
    auto *tab = new QWidget(this);
    tab->setObjectName(QStringLiteral("maintainerTab"));
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(8);

    m_maintainerBrowser = new QTextBrowser(tab);
    m_maintainerBrowser->setOpenExternalLinks(true);
    layout->addWidget(m_maintainerBrowser);
    return tab;
}

QWidget *AboutDialog::createCreditsTab() {
    auto *tab = new QWidget(this);
    tab->setObjectName(QStringLiteral("creditsTab"));
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(14, 14, 14, 14);

    m_creditsBrowser = new QTextBrowser(tab);
    m_creditsBrowser->setOpenExternalLinks(true);
    layout->addWidget(m_creditsBrowser);
    return tab;
}

QWidget *AboutDialog::createEnvironmentTab() {
    auto *tab = new QWidget(this);
    tab->setObjectName(QStringLiteral("diagnosticsTab"));
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(10);

    auto *scroll = new QScrollArea(tab);
    scroll->setObjectName(QStringLiteral("diagnosticsScroll"));
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    auto *cardContainer = new QWidget(scroll);
    cardContainer->setObjectName(QStringLiteral("diagnosticsContent"));
    auto *cardLayout = new QVBoxLayout(cardContainer);
    cardLayout->setContentsMargins(0, 0, 0, 0);

    m_diagnosticsCard = new QLabel(cardContainer);
    m_diagnosticsCard->setTextFormat(Qt::RichText);
    m_diagnosticsCard->setWordWrap(true);
    m_diagnosticsCard->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    m_diagnosticsCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    cardLayout->addWidget(m_diagnosticsCard);
    cardLayout->addStretch();
    scroll->setWidget(cardContainer);
    layout->addWidget(scroll);

    auto *copyBtn = new QPushButton(tr("Copy Diagnostics to Clipboard"), tab);
    copyBtn->setCursor(Qt::PointingHandCursor);
    connect(copyBtn, &QPushButton::clicked, this, &AboutDialog::copySystemInfoToClipboard);
    layout->addWidget(copyBtn);

    return tab;
}

QWidget *AboutDialog::createShortcutsTab() {
    auto *tab = new QWidget(this);
    tab->setObjectName(QStringLiteral("shortcutsTab"));
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(14, 14, 14, 14);

    m_shortcutsBrowser = new QTextBrowser(tab);
    m_shortcutsBrowser->setOpenExternalLinks(false);
    layout->addWidget(m_shortcutsBrowser);
    return tab;
}

QWidget *AboutDialog::createLicenseTab() {
    auto *tab = new QWidget(this);
    tab->setObjectName(QStringLiteral("licenseTab"));
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(14, 14, 14, 14);

    m_licenseBrowser = new QTextBrowser(tab);
    m_licenseBrowser->setOpenExternalLinks(true);
    layout->addWidget(m_licenseBrowser);
    return tab;
}

void AboutDialog::copySystemInfoToClipboard() const {
    const QScreen *primary = QGuiApplication::primaryScreen();
    qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

    QString plainInfo = QString(
                            "SparkNotePad Version: v%1\n"
                            "OS: %2 (%3)\n"
                            "Kernel: %4\n"
                            "CPU: %5 (Build Target: %6)\n"
                            "Qt: %7 (Built with %8)\n"
                            "Display Scale: %9x\n"
                            "Core Engines: Scintilla 5.6.7, Lexilla 5.5.4, Lua 5.5.1\n"
                            "AI Code Review & Auditor: Google Gemini 3.8 Flash via Antigravity IDE\n"
                            "License: GNU General Public License v3.0\n")
                            .arg(APP_VERSION)
                            .arg(QSysInfo::prettyProductName())
                            .arg(QSysInfo::kernelType())
                            .arg(QSysInfo::kernelVersion())
                            .arg(QSysInfo::currentCpuArchitecture())
                            .arg(QSysInfo::buildCpuArchitecture())
                            .arg(qVersion())
                            .arg(QT_VERSION_STR)
                            .arg(QString::number(dpr, 'f', 1));

    QClipboard *clipboard = QGuiApplication::clipboard();
    if (clipboard) {
        clipboard->setText(plainInfo);
    }
}
