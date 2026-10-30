/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AboutDialog.h"
#include "../features/ThemeManager.h"

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

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    setupUi();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this, &AboutDialog::updateThemeColors);
    updateThemeColors();
}

void AboutDialog::setupUi() {
    setWindowTitle(tr("About NoteSciPad"));
    resize(540, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createAboutTab(), tr("&About"));
    m_tabWidget->addTab(createMaintainerTab(), tr("&Maintainer"));
    m_tabWidget->addTab(createCreditsTab(), tr("&Credits && Attribution"));
    m_tabWidget->addTab(createEnvironmentTab(), tr("&System && Diagnostics"));
    m_tabWidget->addTab(createLicenseTab(), tr("&License"));

    mainLayout->addWidget(m_tabWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}

void AboutDialog::updateThemeColors() {
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    bool dark = ThemeManager::instance()->isDarkActive();

    // Explicit color resolution ensuring 100% theme fidelity (zero dark leakage in light mode)
    QString winBg =
        pal ? QString::fromLatin1(pal->win) : (dark ? QStringLiteral("#1e1f29") : QStringLiteral("#f0f2f5"));
    QString cardBg =
        pal ? QString::fromLatin1(pal->card) : (dark ? QStringLiteral("#282a36") : QStringLiteral("#fafbfc"));
    QString borderCol =
        pal ? QString::fromLatin1(pal->border) : (dark ? QStringLiteral("#44475a") : QStringLiteral("#e1e4e8"));
    QString accentCol =
        pal ? QString::fromLatin1(pal->blue) : (dark ? QStringLiteral("#bd93f9") : QStringLiteral("#0969da"));
    QString txtPri =
        pal ? QString::fromLatin1(pal->txtPri) : (dark ? QStringLiteral("#f8f8f2") : QStringLiteral("#24292f"));
    QString txtSec =
        pal ? QString::fromLatin1(pal->txtSec) : (dark ? QStringLiteral("#6272a4") : QStringLiteral("#57606a"));

    QString cardStyle = QString(
                            "QLabel {"
                            "  padding: 8px 10px;"
                            "  background-color: %1;"
                            "  border: 1px solid %2;"
                            "  border-left: 4px solid %3;"
                            "  border-radius: 6px;"
                            "  color: %4;"
                            "  font-size: 11px;"
                            "  line-height: 1.4;"
                            "}")
                            .arg(cardBg, borderCol, accentCol, txtPri);

    QString dialogStyle = QString(
                              "QDialog {"
                              "  background-color: %1;"
                              "  color: %4;"
                              "  font-size: 12px;"
                              "}"
                              "QTabWidget::pane {"
                              "  background-color: %2;"
                              "  border: 1px solid %3;"
                              "  border-radius: 6px;"
                              "}"
                              "QTabBar {"
                              "  background: transparent;"
                              "  border: none;"
                              "  qproperty-drawBase: 0;"
                              "}"
                              "QTabBar::tab {"
                              "  background-color: %1;"
                              "  color: %5;"
                              "  padding: 5px 12px;"
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
                              "  border-top: 3px solid %3;"
                              "}"
                              "QTabBar::tab:hover:!selected {"
                              "  background-color: %3;"
                              "  color: %4;"
                              "}")
                              .arg(winBg, cardBg, borderCol, txtPri, txtSec);

    setStyleSheet(dialogStyle);

    QString browserStyle = QString(
                               "QTextBrowser {"
                               "  background-color: %1;"
                               "  border: 1px solid %2;"
                               "  border-radius: 6px;"
                               "  padding: 8px 10px;"
                               "  font-size: 11px;"
                               "  line-height: 1.4;"
                               "  color: %3;"
                               "}")
                               .arg(cardBg, borderCol, txtPri);

    if (m_noticeBox) {
        m_noticeBox->setStyleSheet(cardStyle);
        m_noticeBox->setText(tr("<div style='color: %1;'>"
                                "<b>Important Legal &amp; Non-Affiliation Statement:</b><br/>"
                                "<i>NoteSciPad is an independent open-source project. It is not affiliated with, "
                                "endorsed by, or associated with Notepad++, NotepadNext, Notepad4, or Notepad3.</i>"
                                "</div>")
                                 .arg(txtPri));
    }

    if (m_metaLabel) {
        m_metaLabel->setStyleSheet(cardStyle);
        m_metaLabel->setText(
            QString("<div style='color: %1;'>"
                    "<p style='margin: 0;'><b>Distribution Channel:</b> %2<br/>"
                    "<b>Official Repository:</b> <a style='color: %3;' "
                    "href='https://github.com/alsyundawy/NoteSciPad'>https://github.com/alsyundawy/NoteSciPad</a><br/>"
                    "<b>License:</b> GNU General Public License v3 (GPL-3.0-or-later)<br/>"
                    "<b>Engine Architecture:</b> Vendored Scintilla 5.6.x + Lexilla 5.5.x C++ Core</p>"
                    "</div>")
                .arg(txtPri, QString::fromLatin1(APP_DISTRIBUTION), accentCol));
    }

    if (m_titleLabel) {
        m_titleLabel->setText(QString("<div style='color: %1;'>"
                                      "<h3 style='margin: 0 0 2px 0; font-size: 15px; color: %1;'>NoteSciPad v%2</h3>"
                                      "<p style='margin: 0; color: %1; font-weight: bold; font-size: 11px;'>"
                                      "Independent, Ultra-Lightweight &amp; High-Performance Text Editor</p>"
                                      "<p style='margin: 1px 0 0 0; color: %3; font-style: italic; font-size: 10px;'>"
                                      "Powered natively by Vendored Scintilla &amp; Lexilla with Modern Qt 6</p>"
                                      "</div>")
                                  .arg(txtPri, QString::fromLatin1(APP_VERSION), txtSec));
    }

    if (m_maintainerBrowser) {
        m_maintainerBrowser->setStyleSheet(browserStyle);
        m_maintainerBrowser->setHtml(
            tr("<div style='color: %1; line-height: 1.4;'>"
               "<h3 style='margin-top: 0; font-size: 13px; color: %1;'>Maintainer &amp; Official Support Channels</h3>"
               "<p><b>Lead Architect &amp; Maintainer:</b> HARRY DERTIN SUTISNA ALSYUNDAWY "
               "(<code>@alsyundawy</code>)<br/>"
               "<b>Organization:</b> ALSYUNDAWY IT SOLUTION<br/>"
               "<b>Official Website:</b> <a style='color: %2;' "
               "href='https://alsyundawy.com'>https://alsyundawy.com</a><br/>"
               "<b>Official Email:</b> <a style='color: %2;' "
               "href='mailto:alsyundawy@gmail.com'>alsyundawy@gmail.com</a><br/>"
               "<b>GitHub Profile:</b> <a style='color: %2;' href='https://github.com/alsyundawy'>@alsyundawy</a><br/>"
               "<b>Telegram Channel:</b> <a style='color: %2;' href='https://t.me/alsyundawy'>@alsyundawy</a></p>"
               "<hr style='border: 0; border-top: 1px solid %3; margin: 10px 0;'/>"
               "<h4 style='font-size: 12px; margin: 6px 0; color: %1;'>Sponsorship &amp; Donation Channels</h4>"
               "<p>If you find NoteSciPad useful for your development and daily workflow, consider supporting our "
               "ongoing "
               "development:</p>"
               "<ul>"
               "<li>💳 <b>PayPal:</b> <a style='color: %2;' "
               "href='https://www.paypal.me/alsyundawy'>paypal.me/alsyundawy</a></li>"
               "<li>☕ <b>Buy Me A Coffee:</b> <a style='color: %2;' "
               "href='https://www.buymeacoffee.com/0TC98Sk'>buymeacoffee.com/0TC98Sk</a></li>"
               "<li>🇮🇩 <b>QRIS (NMID: ID1020021153676):</b> <a style='color: %2;' "
               "href='https://github.com/user-attachments/assets/a0126f28-6dde-43da-ba14-d7c9a27de0df'>View Official "
               "QRIS Barcode</a></li>"
               "</ul>"
               "</div>")
                .arg(txtPri, accentCol, borderCol));
    }

    if (m_creditsBrowser) {
        m_creditsBrowser->setStyleSheet(browserStyle);
        m_creditsBrowser->setHtml(
            tr("<div style='color: %1; line-height: 1.4;'>"
               "<h3 style='margin-top: 0; font-size: 13px; color: %1;'>Credits &amp; Upstream Attribution</h3>"
               "<p>NoteSciPad stands on the shoulders of giants. We express our deepest gratitude to the following "
               "projects and authors:</p>"
               "<ul>"
               "<li><b>Scintilla:</b> Created by Neil Hodgson and contributors. The legendary core editing component "
               "providing high-performance text rendering, multi-caret editing, and fold margins. (<a style='color: "
               "%2;' "
               "href='https://www.scintilla.org'>scintilla.org</a>)</li>"
               "<li><b>Lexilla:</b> Created by Neil Hodgson and contributors. Provides syntax highlighting and styling "
               "lexers for over 100 programming languages. (<a style='color: %2;' "
               "href='https://www.scintilla.org'>scintilla.org/Lexilla.html</a>)</li>"
               "<li><b>NotepadNext:</b> Created by Justin Dailey (dail8859). Provided the cross-platform Qt "
               "scaffolding "
               "and docking framework foundation. (<a style='color: %2;' "
               "href='https://github.com/dail8859/NotepadNext'>github.com/dail8859/NotepadNext</a>)</li>"
               "<li><b>Notepad4:</b> Created by Zufu Liu (zufuliu). Provided invaluable architectural ideas, lexer "
               "updates, and utility concepts. (<a style='color: %2;' "
               "href='https://github.com/zufuliu/notepad4'>github.com/zufuliu/notepad4</a>)</li>"
               "<li><b>Notepad3:</b> Created by Rizonesoft and contributors. Inspired Scintilla power-user text "
               "transformation patterns and UI polish. (<a style='color: %2;' "
               "href='https://www.rizonesoft.com'>rizonesoft.com</a>)</li>"
               "<li><b>SubnetCalc for macOS:</b> Inspired UI elegance, theme architecture, and release documentation "
               "standards. (<a style='color: %2;' "
               "href='https://github.com/alsyundawy/SubnetCalc-MacOS'>github.com/alsyundawy/SubnetCalc-MacOS</a>)</li>"
               "<li><b>Lon McGregor (NotepadPlusPlusIcons):</b> Designer of the iconic open-source editor and "
               "chameleon "
               "artwork series, adapted as NoteSciPad's official application icon. (<a style='color: %2;' "
               "href='https://github.com/LonMcGregor/NotepadPlusPlusIcons'>github.com/LonMcGregor/"
               "NotepadPlusPlusIcons</a>)</li>"
               "<li><b>Qt Project:</b> Developed by The Qt Company and the Qt Open Source Community. (<a style='color: "
               "%2;' "
               "href='https://www.qt.io'>qt.io</a>)</li>"
               "<li><b>uchardet:</b> Maintained by Freedesktop. Enables fast universal character encoding "
               "detection.</li>"
               "<li><b>Qt Advanced Docking System (ADS):</b> Advanced dock widget management.</li>"
               "</ul>"
               "</div>")
                .arg(txtPri, accentCol));
    }

    if (m_diagnosticsCard) {
        m_diagnosticsCard->setStyleSheet(cardStyle);
        QScreen *primary = QGuiApplication::primaryScreen();
        qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

        QString infoText = QString(
                               "<div style='color: %1;'>"
                               "<b>Operating System:</b> %2 (%3)<br/>"
                               "<b>Architecture:</b> %4 (Build Target: %5)<br/>"
                               "<b>Qt Version:</b> %6 (Built with %7)<br/>"
                               "<b>Display Scale Ratio:</b> %8x (Retina / High-DPI)<br/>"
                               "<b>NoteSciPad Version:</b> v%9<br/>"
                               "<b>Kernel Type:</b> %10"
                               "</div>")
                               .arg(txtPri)
                               .arg(QSysInfo::prettyProductName())
                               .arg(QSysInfo::kernelVersion())
                               .arg(QSysInfo::currentCpuArchitecture())
                               .arg(QSysInfo::buildCpuArchitecture())
                               .arg(qVersion())
                               .arg(QT_VERSION_STR)
                               .arg(QString::number(dpr, 'f', 1))
                               .arg(QString::fromLatin1(APP_VERSION))
                               .arg(QSysInfo::kernelType());

        m_diagnosticsCard->setText(infoText);
    }

    if (m_licenseBrowser) {
        m_licenseBrowser->setStyleSheet(browserStyle);
        m_licenseBrowser->setHtml(
            tr("<div style='color: %1; line-height: 1.4;'>"
               "<h3 style='margin-top: 0; font-size: 13px; color: %1;'>GNU General Public License v3.0</h3>"
               "<p>NoteSciPad is free software: you can redistribute it and/or modify "
               "it under the terms of the GNU General Public License as published by "
               "the Free Software Foundation, either version 3 of the License, or "
               "(at your option) any later version.</p>"
               "<p>This program is distributed in the hope that it will be useful, "
               "but WITHOUT ANY WARRANTY; without even the implied warranty of "
               "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
               "GNU General Public License for more details.</p>"
               "<p>For complete license details, visit: <a style='color: %2;' "
               "href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a></p>"
               "</div>")
                .arg(txtPri, accentCol));
    }
}

QWidget *AboutDialog::createAboutTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    // Header layout with icon and title
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(12);

    QLabel *iconLabel = new QLabel(this);
    QPixmap iconPixmap(":/icons/NoteSciPad.png");
    if (!iconPixmap.isNull()) {
        iconLabel->setPixmap(iconPixmap.scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    iconLabel->setFixedSize(52, 52);
    headerLayout->addWidget(iconLabel);

    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(2);

    m_titleLabel = new QLabel(this);
    titleLayout->addWidget(m_titleLabel);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    layout->addLayout(headerLayout);

    // Legal notice box
    m_noticeBox = new QLabel(this);
    m_noticeBox->setWordWrap(true);
    layout->addWidget(m_noticeBox);

    m_metaLabel = new QLabel(this);
    m_metaLabel->setOpenExternalLinks(true);
    layout->addWidget(m_metaLabel);

    layout->addStretch();
    return tab;
}

QWidget *AboutDialog::createMaintainerTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    m_maintainerBrowser = new QTextBrowser(this);
    m_maintainerBrowser->setOpenExternalLinks(true);
    layout->addWidget(m_maintainerBrowser);
    return tab;
}

QWidget *AboutDialog::createCreditsTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);

    m_creditsBrowser = new QTextBrowser(this);
    m_creditsBrowser->setOpenExternalLinks(true);
    layout->addWidget(m_creditsBrowser);
    return tab;
}

QWidget *AboutDialog::createEnvironmentTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    m_diagnosticsCard = new QLabel(this);
    m_diagnosticsCard->setTextFormat(Qt::RichText);
    layout->addWidget(m_diagnosticsCard);

    QPushButton *copyBtn = new QPushButton(tr("Copy Diagnostics to Clipboard"), this);
    copyBtn->setCursor(Qt::PointingHandCursor);
    copyBtn->setStyleSheet(
        "QPushButton {"
        "  padding: 5px 14px;"
        "  font-size: 11px;"
        "  font-weight: 600;"
        "}");
    connect(copyBtn, &QPushButton::clicked, this, &AboutDialog::copySystemInfoToClipboard);
    layout->addWidget(copyBtn);

    layout->addStretch();
    return tab;
}

QWidget *AboutDialog::createLicenseTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);

    m_licenseBrowser = new QTextBrowser(this);
    m_licenseBrowser->setOpenExternalLinks(true);
    layout->addWidget(m_licenseBrowser);
    return tab;
}

void AboutDialog::copySystemInfoToClipboard() {
    QScreen *primary = QGuiApplication::primaryScreen();
    qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

    QString plainInfo = QString(
                            "NoteSciPad Version: v%1\n"
                            "OS: %2 (%3)\n"
                            "Kernel: %4\n"
                            "CPU: %5 (Build Target: %6)\n"
                            "Qt: %7 (Built with %8)\n"
                            "Display Scale: %9x\n")
                            .arg(QString::fromLatin1(APP_VERSION))
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
