/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "UpdateDialog.h"
#include "../features/ThemeManager.h"
#include "../features/ReleaseManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>
#include <QLocale>

namespace {

struct DialogColors {
    QString winBg;
    QString cardBg;
    QString border;
    QString txtPri;
    QString txtSec;
    QString accent;
    QString green;
    QString amber;
};

DialogColors fetchDialogColors() {
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    const bool dark = ThemeManager::instance()->isDarkActive();

    DialogColors c;
    c.winBg = pal ? QString::fromLatin1(pal->win) : (dark ? QStringLiteral("#181a1f") : QStringLiteral("#f6f8fa"));
    c.cardBg = pal ? QString::fromLatin1(pal->card) : (dark ? QStringLiteral("#21252b") : QStringLiteral("#ffffff"));
    c.border = pal ? QString::fromLatin1(pal->border) : (dark ? QStringLiteral("#33373e") : QStringLiteral("#d0d7de"));
    c.txtPri = pal ? QString::fromLatin1(pal->txtPri) : (dark ? QStringLiteral("#e0e0e0") : QStringLiteral("#24292f"));
    c.txtSec = pal ? QString::fromLatin1(pal->txtSec) : (dark ? QStringLiteral("#9aa0a6") : QStringLiteral("#57606a"));
    c.accent = pal ? QString::fromLatin1(pal->blue) : (dark ? QStringLiteral("#007aff") : QStringLiteral("#0969da"));
    c.green = QStringLiteral("#10b981");
    c.amber = QStringLiteral("#f59e0b");
    return c;
}

QPushButton *createPrimaryButton(const QString &text, const QString &accentColor, QWidget *parent) {
    QPushButton *btn = new QPushButton(text, parent);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setMinimumHeight(32);
    btn->setStyleSheet(QString("QPushButton {"
                               "  background-color: %1;"
                               "  color: #ffffff;"
                               "  border: 1px solid %1;"
                               "  border-radius: 6px;"
                               "  padding: 6px 18px;"
                               "  font-weight: 600;"
                               "  font-size: 13px;"
                               "}"
                               "QPushButton:hover {"
                               "  background-color: #0062cc;"
                               "  border-color: #0062cc;"
                               "}"
                               "QPushButton:pressed {"
                               "  background-color: #004fb0;"
                               "  border-color: #004fb0;"
                               "}")
                           .arg(accentColor));
    return btn;
}

QPushButton *createSecondaryButton(const QString &text, const DialogColors &colors, QWidget *parent) {
    QPushButton *btn = new QPushButton(text, parent);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setMinimumHeight(32);
    btn->setStyleSheet(QString("QPushButton {"
                               "  background-color: %1;"
                               "  color: %2;"
                               "  border: 1px solid %3;"
                               "  border-radius: 6px;"
                               "  padding: 6px 16px;"
                               "  font-weight: 500;"
                               "  font-size: 13px;"
                               "}"
                               "QPushButton:hover {"
                               "  background-color: rgba(128, 128, 128, 0.12);"
                               "  border-color: %4;"
                               "}"
                               "QPushButton:pressed {"
                               "  background-color: rgba(128, 128, 128, 0.22);"
                               "}")
                           .arg(colors.cardBg, colors.txtPri, colors.border, colors.accent));
    return btn;
}

}  // namespace

UpdateDialog::UpdateDialog(QWidget *parent, DialogType type) : QDialog(parent), m_type(type) {}

void UpdateDialog::applyBaseStyling(int width, int height) {
    setWindowTitle(tr("SparkNotePad - Software Update"));
    setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint) | Qt::Sheet);
    setModal(true);
    resize(width, height);

    const DialogColors colors = fetchDialogColors();
    setStyleSheet(QString("QDialog {"
                          "  background-color: %1;"
                          "  border: 1px solid %2;"
                          "  border-radius: 12px;"
                          "}")
                      .arg(colors.winBg, colors.border));
}

QPixmap UpdateDialog::createBadgePixmap(DialogType type) {
    const int size = 56;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor ringColor;
    QColor bgColor;

    switch (type) {
        case DialogType::UpdateAvailable:
            ringColor = QColor(0, 122, 255);
            bgColor = QColor(0, 122, 255, 32);
            break;
        case DialogType::UpToDate:
            ringColor = QColor(16, 185, 129);
            bgColor = QColor(16, 185, 129, 32);
            break;
        case DialogType::CheckFailed:
            ringColor = QColor(245, 158, 11);
            bgColor = QColor(245, 158, 11, 32);
            break;
    }

    // Outer subtle glow
    painter.setBrush(bgColor);
    painter.setPen(QPen(ringColor, 1.8));
    painter.drawEllipse(2, 2, size - 4, size - 4);

    // Inner icon graphics
    if (type == DialogType::UpdateAvailable) {
        // Modern spark arrow: arrow pointing down into a base bracket
        painter.setPen(QPen(ringColor, 2.2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        // Arrow stem & head
        painter.drawLine(size / 2, 14, size / 2, 32);
        painter.drawLine(size / 2 - 6, 26, size / 2, 32);
        painter.drawLine(size / 2 + 6, 26, size / 2, 32);
        // Base tray
        painter.drawLine(size / 2 - 9, 38, size / 2 + 9, 38);
    } else if (type == DialogType::UpToDate) {
        // Crisp checkmark
        const qreal halfSize = static_cast<qreal>(size) / 2.0;
        QPainterPath path;
        path.moveTo(halfSize - 9.0, halfSize);
        path.lineTo(halfSize - 2.0, halfSize + 7.0);
        path.lineTo(halfSize + 10.0, halfSize - 7.0);
        painter.strokePath(path, QPen(ringColor, 2.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else {
        // Exclamation point
        QFont f = painter.font();
        f.setBold(true);
        f.setPixelSize(26);
        painter.setFont(f);
        painter.setPen(ringColor);
        painter.drawText(QRect(0, 0, size, size - 2), Qt::AlignCenter, QStringLiteral("!"));
    }

    return pixmap;
}

QString UpdateDialog::formatPublishDate(const QString &isoDate) {
    if (isoDate.isEmpty()) {
        return tr("Official Stable Release");
    }
    const QDateTime dt = QDateTime::fromString(isoDate, Qt::ISODate);
    if (dt.isValid()) {
        return dt.toString(QStringLiteral("dd MMMM yyyy"));
    }
    return isoDate;
}

QString UpdateDialog::formatReleaseNotes(const QString &rawNotes, const QString &txtPri, const QString &txtSec,
                                         const QString &accent) {
    if (rawNotes.trimmed().isEmpty()) {
        return QString("<p style='color:%1;'>%2</p>")
            .arg(txtSec, tr("Performance enhancements, stability improvements, and bug fixes."));
    }

    QString html;
    const QStringList lines = rawNotes.split('\n');
    bool inList = false;

    for (const QString &rawLine : lines) {
        QString line = rawLine.trimmed();
        if (line.isEmpty()) {
            if (inList) {
                html += QStringLiteral("</ul>");
                inList = false;
            }
            continue;
        }

        if (line.startsWith(QStringLiteral("### "))) {
            if (inList) {
                html += QStringLiteral("</ul>");
                inList = false;
            }
            QString title = line.mid(4).toHtmlEscaped();
            html += QString(
                        "<div style='font-size:12px; font-weight:700; color:%1; margin-top:10px; margin-bottom:4px; "
                        "text-transform:uppercase;'>%2</div>")
                        .arg(accent, title);
        } else if (line.startsWith(QStringLiteral("## "))) {
            if (inList) {
                html += QStringLiteral("</ul>");
                inList = false;
            }
            QString title = line.mid(3).toHtmlEscaped();
            html += QString(
                        "<div style='font-size:13px; font-weight:700; color:%1; margin-top:12px; "
                        "margin-bottom:6px;'>%2</div>")
                        .arg(txtPri, title);
        } else if (line.startsWith(QStringLiteral("- ")) || line.startsWith(QStringLiteral("* "))) {
            if (!inList) {
                html += QStringLiteral("<ul style='margin: 4px 0 6px 18px; padding: 0;'>");
                inList = true;
            }
            QString item = line.mid(2).toHtmlEscaped();
            html += QString("<li style='margin-bottom:3px; color:%1;'>%2</li>").arg(txtPri, item);
        } else {
            if (inList) {
                html += QStringLiteral("</ul>");
                inList = false;
            }
            html += QString("<p style='margin: 3px 0; color:%1;'>%2</p>").arg(txtPri, line.toHtmlEscaped());
        }
    }

    if (inList) {
        html += QStringLiteral("</ul>");
    }

    return html;
}

void UpdateDialog::setupUpdateAvailableUi(const QString &curVer, const ReleaseInfo &info) {
    applyBaseStyling(560, 440);
    const DialogColors colors = fetchDialogColors();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 22, 24, 20);
    mainLayout->setSpacing(16);

    // 1. Header Banner
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(16);

    QLabel *badgeLabel = new QLabel(this);
    badgeLabel->setPixmap(createBadgePixmap(DialogType::UpdateAvailable));
    badgeLabel->setFixedSize(56, 56);
    headerLayout->addWidget(badgeLabel, 0, Qt::AlignTop);

    QVBoxLayout *headerTextLayout = new QVBoxLayout();
    headerTextLayout->setSpacing(4);

    QLabel *titleLabel = new QLabel(tr("SparkNotePad Update Available"), this);
    titleLabel->setStyleSheet(QString("font-size: 17px; font-weight: 700; color: %1;").arg(colors.txtPri));
    headerTextLayout->addWidget(titleLabel);

    QLabel *subtitleLabel = new QLabel(
        tr("A new version is available for download with performance, stability, and feature improvements."), this);
    subtitleLabel->setWordWrap(true);
    subtitleLabel->setStyleSheet(QString("font-size: 13px; color: %1;").arg(colors.txtSec));
    headerTextLayout->addWidget(subtitleLabel);

    headerLayout->addLayout(headerTextLayout, 1);
    mainLayout->addLayout(headerLayout);

    // 2. Version Comparison Card
    QFrame *versionCard = new QFrame(this);
    versionCard->setStyleSheet(QString("QFrame {"
                                       "  background-color: %1;"
                                       "  border: 1px solid %2;"
                                       "  border-radius: 8px;"
                                       "  padding: 10px;"
                                       "}")
                                   .arg(colors.cardBg, colors.border));

    QVBoxLayout *cardLayout = new QVBoxLayout(versionCard);
    cardLayout->setContentsMargins(12, 10, 12, 10);
    cardLayout->setSpacing(8);

    QHBoxLayout *pillsLayout = new QHBoxLayout();
    pillsLayout->setSpacing(10);

    QLabel *curPill = new QLabel(this);
    curPill->setTextFormat(Qt::RichText);
    curPill->setText(
        QString("<span style='color:%1;'>%2:</span> "
                "<b style='background:rgba(128,128,128,0.18); color:%3; padding:2px 8px; border-radius:4px;'>%4</b>")
            .arg(colors.txtSec, tr("Installed"), colors.txtPri, curVer));
    pillsLayout->addWidget(curPill);

    QLabel *arrowLabel = new QLabel(QStringLiteral("➜"), this);
    arrowLabel->setStyleSheet(QString("font-size: 14px; font-weight: bold; color: %1;").arg(colors.accent));
    pillsLayout->addWidget(arrowLabel);

    QLabel *newPill = new QLabel(this);
    newPill->setTextFormat(Qt::RichText);
    newPill->setText(
        QString("<span style='color:%1;'>%2:</span> "
                "<b style='background:rgba(16,185,129,0.18); color:%3; padding:2px 8px; border-radius:4px;'>%4</b>")
            .arg(colors.txtSec, tr("Latest"), colors.green, info.versionTag));
    pillsLayout->addWidget(newPill);
    pillsLayout->addStretch();

    cardLayout->addLayout(pillsLayout);

    // Release subtitle metadata
    QString relName = !info.releaseName.isEmpty() ? info.releaseName : info.versionTag;
    QString pubDate = formatPublishDate(info.publishedAt);
    QLabel *metaLabel = new QLabel(this);
    metaLabel->setStyleSheet(QString("font-size: 12px; color: %1;").arg(colors.txtSec));
    metaLabel->setText(QString("%1 • %2").arg(relName, pubDate));
    cardLayout->addWidget(metaLabel);

    mainLayout->addWidget(versionCard);

    // 3. Release Notes Preview Area
    QLabel *notesHeader = new QLabel(tr("Release Notes"), this);
    notesHeader->setStyleSheet(
        QString("font-size: 11px; font-weight: 700; color: %1; letter-spacing: 0.5px;").arg(colors.txtSec));
    mainLayout->addWidget(notesHeader);

    QTextBrowser *notesBrowser = new QTextBrowser(this);
    notesBrowser->setOpenExternalLinks(true);
    notesBrowser->setStyleSheet(QString("QTextBrowser {"
                                        "  background-color: %1;"
                                        "  color: %2;"
                                        "  border: 1px solid %3;"
                                        "  border-radius: 8px;"
                                        "  padding: 8px 12px;"
                                        "  font-size: 12px;"
                                        "}"
                                        "QScrollBar:vertical {"
                                        "  border: none;"
                                        "  background: transparent;"
                                        "  width: 8px;"
                                        "  margin: 4px 2px;"
                                        "}"
                                        "QScrollBar::handle:vertical {"
                                        "  background: %3;"
                                        "  border-radius: 4px;"
                                        "  min-height: 20px;"
                                        "}"
                                        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                                        "  height: 0px;"
                                        "}")
                                    .arg(colors.cardBg, colors.txtPri, colors.border));

    notesBrowser->setHtml(formatReleaseNotes(info.releaseNotes, colors.txtPri, colors.txtSec, colors.accent));
    notesBrowser->setMinimumHeight(130);
    mainLayout->addWidget(notesBrowser, 1);

    // 4. Action Buttons Bar
    QHBoxLayout *buttonBar = new QHBoxLayout();
    buttonBar->setSpacing(10);

    // GitHub link button
    QPushButton *btnGitHub = new QPushButton(tr("View on GitHub ↗"), this);
    btnGitHub->setCursor(Qt::PointingHandCursor);
    btnGitHub->setStyleSheet(QString("QPushButton {"
                                     "  background: transparent;"
                                     "  border: none;"
                                     "  color: %1;"
                                     "  font-size: 12px;"
                                     "  text-decoration: underline;"
                                     "}"
                                     "QPushButton:hover {"
                                     "  color: #0062cc;"
                                     "}")
                                 .arg(colors.accent));
    connect(btnGitHub, &QPushButton::clicked, this, [info]() {
        QString url = !info.htmlUrl.isEmpty() ? info.htmlUrl
                                              : QStringLiteral("https://github.com/alsyundawy/SparkNotePad/releases");
        QDesktopServices::openUrl(QUrl(url));
    });
    buttonBar->addWidget(btnGitHub);
    buttonBar->addStretch();

    // Neutral Remind Later button
    QPushButton *btnLater = createSecondaryButton(tr("Remind Me Later"), colors, this);
    connect(btnLater, &QPushButton::clicked, this, &QDialog::reject);
    buttonBar->addWidget(btnLater);

    // Primary Download Update button
    QPushButton *btnDownload = createPrimaryButton(tr("Download Update ⤓"), colors.accent, this);
    btnDownload->setDefault(true);
    btnDownload->setFocus();
    connect(btnDownload, &QPushButton::clicked, this, [this, info]() {
        QString url = !info.htmlUrl.isEmpty() ? info.htmlUrl
                                              : QStringLiteral("https://github.com/alsyundawy/SparkNotePad/releases");
        QDesktopServices::openUrl(QUrl(url));
        accept();
    });
    buttonBar->addWidget(btnDownload);

    mainLayout->addLayout(buttonBar);
}

void UpdateDialog::setupUpToDateUi(const QString &curVer) {
    applyBaseStyling(490, 220);
    const DialogColors colors = fetchDialogColors();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 22, 24, 20);
    mainLayout->setSpacing(16);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(16);

    QLabel *badgeLabel = new QLabel(this);
    badgeLabel->setPixmap(createBadgePixmap(DialogType::UpToDate));
    badgeLabel->setFixedSize(56, 56);
    headerLayout->addWidget(badgeLabel, 0, Qt::AlignTop);

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->setSpacing(6);

    QLabel *titleLabel = new QLabel(tr("SparkNotePad is Up to Date"), this);
    titleLabel->setStyleSheet(QString("font-size: 16px; font-weight: 700; color: %1;").arg(colors.txtPri));
    textLayout->addWidget(titleLabel);

    QLabel *descLabel = new QLabel(this);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(QString("font-size: 13px; color: %1;").arg(colors.txtSec));
    descLabel->setText(tr("You are using the latest version. All security and stability features are active."));
    textLayout->addWidget(descLabel);

    headerLayout->addLayout(textLayout, 1);
    mainLayout->addLayout(headerLayout);

    // Status Pill Card
    QFrame *statusCard = new QFrame(this);
    statusCard->setStyleSheet(QString("QFrame {"
                                      "  background-color: %1;"
                                      "  border: 1px solid %2;"
                                      "  border-radius: 8px;"
                                      "  padding: 8px 12px;"
                                      "}")
                                  .arg(colors.cardBg, colors.border));

    QHBoxLayout *cardLayout = new QHBoxLayout(statusCard);
    cardLayout->setContentsMargins(10, 6, 10, 6);

    QLabel *statusText = new QLabel(this);
    statusText->setTextFormat(Qt::RichText);
    statusText->setText(QString("<span style='color:%1;'>%2: <b>v%3</b></span>"
                                "<span style='margin-left: 12px; background:rgba(16,185,129,0.18); color:%4; "
                                "padding:2px 8px; border-radius:4px; font-weight:bold;'>✓ Up to Date</span>")
                            .arg(colors.txtPri, tr("Active Version"), curVer, colors.green));
    cardLayout->addWidget(statusText);
    cardLayout->addStretch();

    mainLayout->addWidget(statusCard);

    // Action button
    QHBoxLayout *buttonBar = new QHBoxLayout();
    buttonBar->addStretch();

    QPushButton *btnOk = createPrimaryButton(tr("Done"), colors.accent, this);
    btnOk->setDefault(true);
    btnOk->setFocus();
    connect(btnOk, &QPushButton::clicked, this, &QDialog::accept);
    buttonBar->addWidget(btnOk);

    mainLayout->addLayout(buttonBar);
}

void UpdateDialog::setupCheckFailedUi(const QString &errorMessage, QWidget *retryParent) {
    applyBaseStyling(510, 240);
    const DialogColors colors = fetchDialogColors();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 22, 24, 20);
    mainLayout->setSpacing(16);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(16);

    QLabel *badgeLabel = new QLabel(this);
    badgeLabel->setPixmap(createBadgePixmap(DialogType::CheckFailed));
    badgeLabel->setFixedSize(56, 56);
    headerLayout->addWidget(badgeLabel, 0, Qt::AlignTop);

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->setSpacing(6);

    QLabel *titleLabel = new QLabel(tr("Update Check Failed"), this);
    titleLabel->setStyleSheet(QString("font-size: 16px; font-weight: 700; color: %1;").arg(colors.txtPri));
    textLayout->addWidget(titleLabel);

    QLabel *descLabel =
        new QLabel(tr("Unable to connect to the GitHub update server. Please check your internet connection."), this);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(QString("font-size: 13px; color: %1;").arg(colors.txtSec));
    textLayout->addWidget(descLabel);

    headerLayout->addLayout(textLayout, 1);
    mainLayout->addLayout(headerLayout);

    // Diagnostic Error Box
    if (!errorMessage.isEmpty()) {
        QFrame *errBox = new QFrame(this);
        errBox->setStyleSheet(QString("QFrame {"
                                      "  background-color: %1;"
                                      "  border: 1px solid %2;"
                                      "  border-radius: 6px;"
                                      "  padding: 6px 10px;"
                                      "}")
                                  .arg(colors.cardBg, colors.border));

        QVBoxLayout *errLayout = new QVBoxLayout(errBox);
        errLayout->setContentsMargins(8, 6, 8, 6);

        QLabel *errLabel = new QLabel(errorMessage, this);
        errLabel->setWordWrap(true);
        errLabel->setStyleSheet(QString("font-family: monospace; font-size: 11px; color: %1;").arg(colors.amber));
        errLayout->addWidget(errLabel);

        mainLayout->addWidget(errBox);
    }

    // Button Bar
    QHBoxLayout *buttonBar = new QHBoxLayout();
    buttonBar->addStretch();

    if (retryParent) {
        QPushButton *btnRetry = createSecondaryButton(tr("Retry"), colors, this);
        connect(btnRetry, &QPushButton::clicked, this, [this, retryParent]() {
            accept();
            ReleaseManager::instance().checkForUpdates(retryParent, false);
        });
        buttonBar->addWidget(btnRetry);
    }

    QPushButton *btnClose = createPrimaryButton(tr("Close"), colors.accent, this);
    btnClose->setDefault(true);
    btnClose->setFocus();
    connect(btnClose, &QPushButton::clicked, this, &QDialog::reject);
    buttonBar->addWidget(btnClose);

    mainLayout->addLayout(buttonBar);
}

void UpdateDialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        reject();
        return;
    }
    QDialog::keyPressEvent(event);
}

void UpdateDialog::showUpdateAvailable(QWidget *parent, const QString &curVer, const ReleaseInfo &info) {
    UpdateDialog dlg(parent, DialogType::UpdateAvailable);
    dlg.setupUpdateAvailableUi(curVer, info);
    dlg.exec();
}

void UpdateDialog::showUpToDate(QWidget *parent, const QString &curVer) {
    UpdateDialog dlg(parent, DialogType::UpToDate);
    dlg.setupUpToDateUi(curVer);
    dlg.exec();
}

void UpdateDialog::showCheckFailed(QWidget *parent, const QString &errorMessage, QWidget *retryParent) {
    UpdateDialog dlg(parent, DialogType::CheckFailed);
    dlg.setupCheckFailedUi(errorMessage, retryParent);
    dlg.exec();
}
