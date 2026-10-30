/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "DisplayInfoDialog.h"
#include "features/ScreenDisplayManager.h"
#include "features/ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGuiApplication>
#include <QScreen>

DisplayInfoDialog::DisplayInfoDialog(QWidget *parent) : QDialog(parent) {
    setupUi();
    refreshDisplays();
}

void DisplayInfoDialog::setupUi() {
    setWindowTitle(tr("Display & Retina Diagnostics — NoteSciPad"));
    resize(760, 420);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);

    // Header Summary
    m_summaryLabel = new QLabel(this);
    m_summaryLabel->setWordWrap(true);
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    bool dark = ThemeManager::instance()->isDarkActive();
    QString cardBg =
        pal ? QString::fromLatin1(pal->card) : (dark ? QStringLiteral("#282a36") : QStringLiteral("#fafbfc"));
    QString borderCol =
        pal ? QString::fromLatin1(pal->border) : (dark ? QStringLiteral("#44475a") : QStringLiteral("#e1e4e8"));
    QString txtPri =
        pal ? QString::fromLatin1(pal->txtPri) : (dark ? QStringLiteral("#f8f8f2") : QStringLiteral("#24292f"));
    m_summaryLabel->setStyleSheet(QString("QLabel {"
                                          "  font-size: 13px;"
                                          "  padding: 10px;"
                                          "  background-color: %1;"
                                          "  border: 1px solid %2;"
                                          "  border-radius: 6px;"
                                          "  color: %3;"
                                          "}")
                                      .arg(cardBg, borderCol, txtPri));
    mainLayout->addWidget(m_summaryLabel);

    // Table of Displays
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(6);
    m_tableWidget->setHorizontalHeaderLabels({tr("Display Name"), tr("Resolution"), tr("DPR / Retina"),
                                              tr("Refresh Rate"), tr("Logical / Phys DPI"), tr("Role / Status")});
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_tableWidget->setAlternatingRowColors(true);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainLayout->addWidget(m_tableWidget);

    // Multi-Monitor Action Buttons
    QHBoxLayout *actionLayout = new QHBoxLayout();

    QPushButton *btnNext = new QPushButton(tr("Move to &Next Screen"), this);
    btnNext->setToolTip(tr("Move NoteSciPad window to the next connected monitor (Cmd+Alt+Right)"));
    connect(btnNext, &QPushButton::clicked, this, &DisplayInfoDialog::onMoveToNextClicked);
    actionLayout->addWidget(btnNext);

    QPushButton *btnPrev = new QPushButton(tr("Move to &Previous Screen"), this);
    btnPrev->setToolTip(tr("Move NoteSciPad window to the previous monitor (Cmd+Alt+Left)"));
    connect(btnPrev, &QPushButton::clicked, this, &DisplayInfoDialog::onMoveToPrevClicked);
    actionLayout->addWidget(btnPrev);

    QPushButton *btnCenter = new QPushButton(tr("&Center Window"), this);
    btnCenter->setToolTip(tr("Center NoteSciPad on the current active display (Cmd+Alt+C)"));
    connect(btnCenter, &QPushButton::clicked, this, &DisplayInfoDialog::onCenterWindowClicked);
    actionLayout->addWidget(btnCenter);

    QPushButton *btnPrimary = new QPushButton(tr("&Move to Primary"), this);
    btnPrimary->setToolTip(tr("Position NoteSciPad onto the OS primary screen"));
    connect(btnPrimary, &QPushButton::clicked, this, &DisplayInfoDialog::onMoveToPrimaryClicked);
    actionLayout->addWidget(btnPrimary);

    actionLayout->addStretch();

    QPushButton *btnRefresh = new QPushButton(tr("&Refresh"), this);
    connect(btnRefresh, &QPushButton::clicked, this, &DisplayInfoDialog::refreshDisplays);
    actionLayout->addWidget(btnRefresh);

    mainLayout->addLayout(actionLayout);

    // Close Button Box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}

void DisplayInfoDialog::refreshDisplays() {
    QWidget *parentWin = parentWidget();
    const auto displays = ScreenDisplayManager::instance().getConnectedDisplaysInfo(parentWin);

    m_tableWidget->setRowCount(displays.size());

    int retinaCount = 0;
    QString activeName = tr("Unknown");
    qreal activeDpr = 1.0;

    for (int r = 0; r < displays.size(); ++r) {
        const auto &d = displays.at(r);
        if (d.isRetina)
            retinaCount++;
        if (d.hasWindow) {
            activeName = d.name;
            activeDpr = d.devicePixelRatio;
        }

        // Display Name
        QString displayName = d.name;
        if (!d.model.isEmpty() && d.model != d.name) {
            displayName += QString(" (%1)").arg(d.model);
        }
        m_tableWidget->setItem(r, 0, new QTableWidgetItem(displayName));

        // Resolution
        QString resStr = QString("%1x%2 (avail: %3x%4)")
                             .arg(d.geometry.width())
                             .arg(d.geometry.height())
                             .arg(d.availableGeometry.width())
                             .arg(d.availableGeometry.height());
        m_tableWidget->setItem(r, 1, new QTableWidgetItem(resStr));

        // DPR / Retina
        QString dprStr = QString("%1x %2")
                             .arg(d.devicePixelRatio, 0, 'f', 1)
                             .arg(d.isRetina ? tr("[Retina / HiDPI]") : tr("[Standard]"));
        m_tableWidget->setItem(r, 2, new QTableWidgetItem(dprStr));

        // Refresh Rate
        QString rateStr = QString("%1 Hz").arg(qRound(d.refreshRate));
        m_tableWidget->setItem(r, 3, new QTableWidgetItem(rateStr));

        // DPI
        QString dpiStr = QString("%1 / %2").arg(qRound(d.logicalDpi)).arg(qRound(d.physicalDpi));
        m_tableWidget->setItem(r, 4, new QTableWidgetItem(dpiStr));

        // Role / Status
        QStringList roles;
        if (d.isPrimary)
            roles << tr("Primary");
        if (d.hasWindow)
            roles << tr("Active Window");
        if (roles.isEmpty())
            roles << tr("Secondary");
        m_tableWidget->setItem(r, 5, new QTableWidgetItem(roles.join(", ")));
    }

    QString summary = tr("<b>Connected Displays:</b> %1 total (%2 Retina/HiDPI)<br/>"
                         "<b>Active Window Display:</b> %3 (Scale: %4x DPR)<br/>"
                         "<b>Fractional Scale Policy:</b> PassThrough (Subpixel Sharpness Enabled)")
                          .arg(displays.size())
                          .arg(retinaCount)
                          .arg(activeName)
                          .arg(activeDpr, 0, 'f', 1);
    m_summaryLabel->setText(summary);
}

void DisplayInfoDialog::onMoveToNextClicked() {
    QWidget *parentWin = parentWidget();
    if (parentWin) {
        ScreenDisplayManager::instance().moveToNextScreen(parentWin);
        refreshDisplays();
    }
}

void DisplayInfoDialog::onMoveToPrevClicked() {
    QWidget *parentWin = parentWidget();
    if (parentWin) {
        ScreenDisplayManager::instance().moveToPreviousScreen(parentWin);
        refreshDisplays();
    }
}

void DisplayInfoDialog::onCenterWindowClicked() {
    QWidget *parentWin = parentWidget();
    if (parentWin) {
        ScreenDisplayManager::instance().centerOnCurrentScreen(parentWin);
        refreshDisplays();
    }
}

void DisplayInfoDialog::onMoveToPrimaryClicked() {
    QWidget *parentWin = parentWidget();
    if (parentWin) {
        ScreenDisplayManager::instance().moveToPrimaryScreen(parentWin);
        refreshDisplays();
    }
}
