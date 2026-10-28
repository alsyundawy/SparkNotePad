/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ModernMessageBox.h"
#include "../features/ThemeManager.h"
#include "../ScintillaNext.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>

ModernMessageBox::ModernMessageBox(QWidget *parent, IconType iconType, const QString &title, const QString &message,
                                   const QString &detail)
    : QDialog(parent) {
    setupUi(iconType, title, message, detail);
}

QPixmap ModernMessageBox::createIconBadge(IconType iconType) const {
    const int size = 52;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor bgColor;
    QColor fgColor;
    QString symbol;

    switch (iconType) {
        case IconType::Question:
            bgColor = QColor(0, 122, 255, 38);
            fgColor = QColor(0, 122, 255);
            symbol = QStringLiteral("?");
            break;
        case IconType::Information:
            bgColor = QColor(14, 165, 233, 38);
            fgColor = QColor(14, 165, 233);
            symbol = QStringLiteral("i");
            break;
        case IconType::Warning:
            bgColor = QColor(245, 158, 11, 38);
            fgColor = QColor(245, 158, 11);
            symbol = QStringLiteral("!");
            break;
        case IconType::Critical:
            bgColor = QColor(239, 68, 68, 38);
            fgColor = QColor(239, 68, 68);
            symbol = QStringLiteral("×");
            break;
    }

    // Outer soft glow circle
    painter.setBrush(bgColor);
    painter.setPen(QPen(fgColor, 1.5));
    painter.drawEllipse(2, 2, size - 4, size - 4);

    // Center symbol
    QFont f = painter.font();
    f.setBold(true);
    f.setPixelSize(iconType == IconType::Critical ? 26 : 24);
    painter.setFont(f);
    painter.setPen(fgColor);
    painter.drawText(QRect(0, 0, size, size), Qt::AlignCenter, symbol);

    return pixmap;
}

void ModernMessageBox::setupUi(IconType iconType, const QString &title, const QString &message, const QString &detail) {
    setWindowTitle(title.isEmpty() ? QStringLiteral("SparkNotePad") : title);
    setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint) | Qt::Sheet);
    setModal(true);
    resize(480, 180);

    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    bool dark = ThemeManager::instance()->isDarkActive();
    QString winBg =
        pal ? QString::fromLatin1(pal->win) : (dark ? QStringLiteral("#1e1e1e") : QStringLiteral("#f6f8fa"));
    QString txtPri =
        pal ? QString::fromLatin1(pal->txtPri) : (dark ? QStringLiteral("#e0e0e0") : QStringLiteral("#24292f"));
    QString txtSec =
        pal ? QString::fromLatin1(pal->txtSec) : (dark ? QStringLiteral("#9aa0a6") : QStringLiteral("#57606a"));
    QString border =
        pal ? QString::fromLatin1(pal->border) : (dark ? QStringLiteral("#33373e") : QStringLiteral("#d0d7de"));
    setStyleSheet(QString("QDialog {"
                          "  background-color: %1;"
                          "  border: 1px solid %2;"
                          "  border-radius: 10px;"
                          "}")
                      .arg(winBg, border));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(18);

    // Body container: icon on left, texts on right
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(18);
    contentLayout->setAlignment(Qt::AlignTop);

    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(createIconBadge(iconType));
    iconLabel->setFixedSize(52, 52);
    contentLayout->addWidget(iconLabel, 0, Qt::AlignTop);

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->setSpacing(6);

    QLabel *msgLabel = new QLabel(this);
    msgLabel->setTextFormat(Qt::RichText);
    msgLabel->setWordWrap(true);
    msgLabel->setStyleSheet(QString("font-size: 15px; font-weight: bold; color: %1;").arg(txtPri));
    msgLabel->setText(message);
    textLayout->addWidget(msgLabel);

    if (!detail.isEmpty()) {
        QLabel *detailLabel = new QLabel(this);
        detailLabel->setTextFormat(Qt::RichText);
        detailLabel->setWordWrap(true);
        detailLabel->setStyleSheet(QString("font-size: 13px; color: %1;").arg(txtSec));
        detailLabel->setText(detail);
        textLayout->addWidget(detailLabel);
    }

    contentLayout->addLayout(textLayout, 1);
    mainLayout->addLayout(contentLayout);

    // Bottom action button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setObjectName(QStringLiteral("buttonLayout"));
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
}

void ModernMessageBox::addButton(const QString &text, ActionChoice role, bool isDefault) {
    QHBoxLayout *layout = findChild<QHBoxLayout *>(QStringLiteral("buttonLayout"));
    if (!layout)
        return;

    QPushButton *btn = new QPushButton(text, this);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setMinimumWidth(85);
    btn->setMinimumHeight(30);

    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    bool dark = ThemeManager::instance()->isDarkActive();
    QString cardBg =
        pal ? QString::fromLatin1(pal->card) : (dark ? QStringLiteral("#25282c") : QStringLiteral("#ffffff"));
    QString border =
        pal ? QString::fromLatin1(pal->border) : (dark ? QStringLiteral("#33373e") : QStringLiteral("#d0d7de"));
    QString txtPri =
        pal ? QString::fromLatin1(pal->txtPri) : (dark ? QStringLiteral("#e0e0e0") : QStringLiteral("#24292f"));
    QString accent =
        pal ? QString::fromLatin1(pal->blue) : (dark ? QStringLiteral("#007aff") : QStringLiteral("#0969da"));

    if (role == ActionChoice::SaveOrAccept) {
        // Vibrant primary accent button
        btn->setStyleSheet(QString("QPushButton {"
                                   "  background-color: %1;"
                                   "  color: #ffffff;"
                                   "  border: 1px solid %1;"
                                   "  border-radius: 6px;"
                                   "  padding: 5px 18px;"
                                   "  font-weight: bold;"
                                   "  font-size: 13px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "  background-color: #0062cc;"
                                   "}"
                                   "QPushButton:pressed {"
                                   "  background-color: #004fb0;"
                                   "}")
                               .arg(accent));
    } else if (role == ActionChoice::Discard) {
        // Destructive / discard neutral button with red highlight on hover
        btn->setStyleSheet(QString("QPushButton {"
                                   "  background-color: %1;"
                                   "  color: %2;"
                                   "  border: 1px solid %3;"
                                   "  border-radius: 6px;"
                                   "  padding: 5px 16px;"
                                   "  font-weight: 500;"
                                   "  font-size: 13px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "  background-color: rgba(239, 68, 68, 0.15);"
                                   "  color: #ef4444;"
                                   "  border-color: #ef4444;"
                                   "}")
                               .arg(cardBg, txtPri, border));
    } else {
        // Neutral / cancel button
        btn->setStyleSheet(QString("QPushButton {"
                                   "  background-color: %1;"
                                   "  color: %2;"
                                   "  border: 1px solid %3;"
                                   "  border-radius: 6px;"
                                   "  padding: 5px 16px;"
                                   "  font-weight: 500;"
                                   "  font-size: 13px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "  border-color: %4;"
                                   "}")
                               .arg(cardBg, txtPri, border, accent));
    }

    if (isDefault) {
        btn->setDefault(true);
        btn->setFocus();
    }

    connect(btn, &QPushButton::clicked, this, [this, role]() {
        m_chosenAction = role;
        accept();
    });

    layout->addWidget(btn);
    m_buttons.append(btn);
}

void ModernMessageBox::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        m_chosenAction = ActionChoice::Cancel;
        reject();
        return;
    }
    QDialog::keyPressEvent(event);
}

ModernMessageBox::ActionChoice ModernMessageBox::promptForSave(QWidget *parent,
                                                               const QVector<ScintillaNext *> &editors) {
    const qsizetype count = editors.size();
    if (count == 0) {
        return ActionChoice::Discard;
    }

    QString mainText = (count == 1) ? tr("Save changes to <b>%1</b>?").arg(editors.first()->getName())
                                    : tr("There are %1 files with unsaved changes.").arg(count);

    QString detailText = (count == 1) ? tr("Your changes will be permanently lost if you close without saving.")
                                      : tr("Do you want to save all documents before closing?");

    ModernMessageBox box(parent, IconType::Question, tr("Save File"), mainText, detailText);
    box.addButton(count > 1 ? tr("Discard All") : tr("Discard"), ActionChoice::Discard);
    box.addButton(tr("Cancel"), ActionChoice::Cancel);
    box.addButton(count > 1 ? tr("Save All") : tr("Save"), ActionChoice::SaveOrAccept, true);

    box.exec();
    return box.chosenAction();
}

bool ModernMessageBox::question(QWidget *parent, const QString &title, const QString &text, const QString &detail) {
    ModernMessageBox box(parent, IconType::Question, title, text, detail);
    box.addButton(tr("No"), ActionChoice::Cancel);
    box.addButton(tr("Yes"), ActionChoice::SaveOrAccept, true);
    box.exec();
    return box.chosenAction() == ActionChoice::SaveOrAccept;
}

void ModernMessageBox::information(QWidget *parent, const QString &title, const QString &text, const QString &detail) {
    ModernMessageBox box(parent, IconType::Information, title, text, detail);
    box.addButton(tr("OK"), ActionChoice::SaveOrAccept, true);
    box.exec();
}

void ModernMessageBox::warning(QWidget *parent, const QString &title, const QString &text, const QString &detail) {
    ModernMessageBox box(parent, IconType::Warning, title, text, detail);
    box.addButton(tr("OK"), ActionChoice::SaveOrAccept, true);
    box.exec();
}

void ModernMessageBox::critical(QWidget *parent, const QString &title, const QString &text, const QString &detail) {
    ModernMessageBox box(parent, IconType::Critical, title, text, detail);
    box.addButton(tr("Close"), ActionChoice::Cancel, true);
    box.exec();
}
