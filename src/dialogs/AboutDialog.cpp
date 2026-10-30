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
#include "features/ThemeManager.h"

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

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    setupUi();
}

void AboutDialog::setupUi() {
    setWindowTitle(tr("About NoteSciPad"));
    resize(540, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createAboutTab(), tr("&About"));
    m_tabWidget->addTab(createCreditsTab(), tr("&Credits & Attribution"));
    m_tabWidget->addTab(createLicenseTab(), tr("&License"));
    m_tabWidget->addTab(createEnvironmentTab(), tr("&Environment"));

    mainLayout->addWidget(m_tabWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}

QWidget *AboutDialog::createAboutTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(12);

    QLabel *titleLabel = new QLabel(this);
    titleLabel->setText(QString("<h2>NoteSciPad v%1</h2>"
                                "<p><b>Independent, Lightweight & High-Performance Text Editor</b><br/>"
                                "Optimized for macOS & Linux with Vendored Scintilla and Lexilla Engines</p>")
                            .arg(APP_VERSION));
    layout->addWidget(titleLabel);

    QLabel *noticeBox = new QLabel(this);
    noticeBox->setWordWrap(true);
    bool dark = ThemeManager::instance()->isDarkActive();
    QString cardBg = dark ? QStringLiteral("#282a36") : QStringLiteral("#fafbfc");
    QString borderCol = dark ? QStringLiteral("#44475a") : QStringLiteral("#e1e4e8");
    QString txtPri = dark ? QStringLiteral("#f8f8f2") : QStringLiteral("#24292f");
    noticeBox->setStyleSheet(QString("QLabel {"
                                     "  padding: 10px;"
                                     "  background-color: %1;"
                                     "  border: 1px solid %2;"
                                     "  border-radius: 6px;"
                                     "  font-size: 12px;"
                                     "  color: %3;"
                                     "}")
                                 .arg(cardBg, borderCol, txtPri));
    noticeBox->setText(
        tr("<b>Important Legal & Non-Affiliation Statement:</b><br/>"
           "<i>NoteSciPad is an independent open-source project. It is not affiliated with, "
           "endorsed by, or associated with Notepad++, NotepadNext, Notepad4, or Notepad3.</i>"));
    layout->addWidget(noticeBox);

    QLabel *metaLabel = new QLabel(this);
    metaLabel->setOpenExternalLinks(true);
    metaLabel->setText(
        QString("<p><b>Author & Maintainer:</b> Harry DS Alsyundawy<br/>"
                "<b>Website / Repository:</b> <a "
                "href='https://github.com/alsyundawy/NoteSciPad'>https://github.com/alsyundawy/NoteSciPad</a><br/>"
                "<b>Distribution:</b> %1<br/>"
                "<b>License:</b> GNU General Public License v3 (GPL-3.0-or-later)</p>")
            .arg(APP_DISTRIBUTION));
    layout->addWidget(metaLabel);

    layout->addStretch();
    return tab;
}

QWidget *AboutDialog::createCreditsTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QTextBrowser *browser = new QTextBrowser(this);
    browser->setOpenExternalLinks(true);
    browser->setHtml(tr(
        "<h3>Credits & Upstream Attribution</h3>"
        "<p>NoteSciPad stands on the shoulders of giants. We express our deepest gratitude to the following projects "
        "and authors:</p>"
        "<ul>"
        "<li><b>Scintilla:</b> Created by Neil Hodgson and contributors. The legendary core editing component "
        "providing high-performance text rendering, multi-caret editing, and fold margins. (<a "
        "href='https://www.scintilla.org'>scintilla.org</a>)</li>"
        "<li><b>Lexilla:</b> Created by Neil Hodgson and contributors. Provides syntax highlighting and styling lexers "
        "for over 100 programming languages.</li>"
        "<li><b>NotepadNext:</b> Created by Justin Dailey (dail8859). Provided the cross-platform Qt scaffolding and "
        "docking framework foundation. (<a "
        "href='https://github.com/dail8859/NotepadNext'>github.com/dail8859/NotepadNext</a>)</li>"
        "<li><b>Notepad4:</b> Created by Zufu Liu (zufuliu). Provided invaluable architectural ideas, lexer updates, "
        "and utility concepts. (<a href='https://github.com/zufuliu/notepad4'>github.com/zufuliu/notepad4</a>)</li>"
        "<li><b>Notepad3:</b> Created by Rizonesoft and contributors. Inspired Scintilla power-user text "
        "transformation patterns and UI polish. (<a href='https://www.rizonesoft.com'>rizonesoft.com</a>)</li>"
        "<li><b>Qt Project:</b> Developed by The Qt Company and the Qt Open Source Community. Powers NoteSciPad's "
        "modern UI. (<a href='https://www.qt.io'>qt.io</a>)</li>"
        "<li><b>uchardet:</b> Maintained by Freedesktop. Enables fast universal character encoding detection.</li>"
        "<li><b>Qt Advanced Docking System (ADS):</b> Advanced dock widget management.</li>"
        "</ul>"));
    layout->addWidget(browser);
    return tab;
}

QWidget *AboutDialog::createLicenseTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QTextBrowser *browser = new QTextBrowser(this);
    browser->setHtml(
        tr("<h3>GNU General Public License v3.0</h3>"
           "<p>NoteSciPad is free software: you can redistribute it and/or modify "
           "it under the terms of the GNU General Public License as published by "
           "the Free Software Foundation, either version 3 of the License, or "
           "(at your option) any later version.</p>"
           "<p>This program is distributed in the hope that it will be useful, "
           "but WITHOUT ANY WARRANTY; without even the implied warranty of "
           "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
           "GNU General Public License for more details.</p>"
           "<p>For complete license details, visit: <a "
           "href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a></p>"));
    layout->addWidget(browser);
    return tab;
}

QWidget *AboutDialog::createEnvironmentTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(12);

    QLabel *infoLabel = new QLabel(this);
    infoLabel->setTextFormat(Qt::RichText);

    QScreen *primary = QGuiApplication::primaryScreen();
    qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

    QString infoText = QString(
                           "<b>Operating System:</b> %1 (%2)<br/>"
                           "<b>Architecture:</b> %3 (Build: %4)<br/>"
                           "<b>Qt Version:</b> %5 (Built against %6)<br/>"
                           "<b>Display Device Pixel Ratio:</b> %7x<br/>"
                           "<b>App Version:</b> %8")
                           .arg(QSysInfo::prettyProductName(), QSysInfo::kernelType(),
                                QSysInfo::currentCpuArchitecture(), QSysInfo::buildCpuArchitecture(), qVersion(),
                                QT_VERSION_STR, QString::number(dpr, 'f', 1), APP_VERSION);

    infoLabel->setText(infoText);
    layout->addWidget(infoLabel);

    QPushButton *copyBtn = new QPushButton(tr("Copy Environment Info to Clipboard"), this);
    connect(copyBtn, &QPushButton::clicked, this, &AboutDialog::copySystemInfoToClipboard);
    layout->addWidget(copyBtn);

    layout->addStretch();
    return tab;
}

void AboutDialog::copySystemInfoToClipboard() {
    QScreen *primary = QGuiApplication::primaryScreen();
    qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

    QString plainInfo =
        QString(
            "NoteSciPad Version: %1\n"
            "OS: %2 (%3)\n"
            "CPU: %4 (Build: %5)\n"
            "Qt: %6 (Runtime: %7)\n"
            "DPR: %8x\n")
            .arg(APP_VERSION, QSysInfo::prettyProductName(), QSysInfo::kernelType(), QSysInfo::currentCpuArchitecture(),
                 QSysInfo::buildCpuArchitecture(), QT_VERSION_STR, qVersion(), QString::number(dpr, 'f', 1));

    QClipboard *clipboard = QGuiApplication::clipboard();
    if (clipboard) {
        clipboard->setText(plainInfo);
    }
}
