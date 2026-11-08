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

#include "TimestampConverterDialog.h"
#include "../features/DeveloperToolsEngine.h"
#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimeZone>
#include <QVBoxLayout>

TimestampConverterDialog::TimestampConverterDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Epoch & ISO-8601 Timestamp Converter"));
    setMinimumWidth(440);
    setupUi();
    setToCurrentTime();
}

void TimestampConverterDialog::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);

    auto *grpEpoch = new QGroupBox(tr("Unix Epoch Timestamp"), this);
    auto *layoutEpoch = new QFormLayout(grpEpoch);

    editEpochSec = new QLineEdit(grpEpoch);
    editEpochSec->setPlaceholderText(tr("e.g. 1767225600"));
    layoutEpoch->addRow(tr("Seconds (s):"), editEpochSec);

    editEpochMs = new QLineEdit(grpEpoch);
    editEpochMs->setPlaceholderText(tr("e.g. 1767225600000"));
    layoutEpoch->addRow(tr("Milliseconds (ms):"), editEpochMs);

    mainLayout->addWidget(grpEpoch);

    auto *grpIso = new QGroupBox(tr("Human & ISO-8601 Formats"), this);
    auto *layoutIso = new QFormLayout(grpIso);

    editIsoUtc = new QLineEdit(grpIso);
    editIsoUtc->setPlaceholderText(tr("e.g. 2026-01-01T00:00:00Z"));
    layoutIso->addRow(tr("ISO-8601 UTC:"), editIsoUtc);

    editIsoLocal = new QLineEdit(grpIso);
    layoutIso->addRow(tr("ISO-8601 Local:"), editIsoLocal);

    lblHuman = new QLabel(this);
    lblHuman->setStyleSheet(QStringLiteral("font-weight: 500; color: #0969da;"));
    layoutIso->addRow(tr("Formatted Date:"), lblHuman);

    mainLayout->addWidget(grpIso);

    auto *layoutBtns = new QHBoxLayout();
    btnNow = new QPushButton(tr("Set to Now"), this);
    btnConvert = new QPushButton(tr("Convert"), this);
    btnCopy = new QPushButton(tr("Copy ISO"), this);
    btnClose = new QPushButton(tr("Close"), this);

    connect(btnNow, &QPushButton::clicked, this, &TimestampConverterDialog::setToCurrentTime);
    connect(btnConvert, &QPushButton::clicked, this, &TimestampConverterDialog::convertFromEpoch);
    connect(btnCopy, &QPushButton::clicked, this, &TimestampConverterDialog::copySelectedToClipboard);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);

    connect(editEpochSec, &QLineEdit::returnPressed, this, &TimestampConverterDialog::convertFromEpoch);
    connect(editIsoUtc, &QLineEdit::returnPressed, this, &TimestampConverterDialog::convertFromIso);

    layoutBtns->addWidget(btnNow);
    layoutBtns->addWidget(btnConvert);
    layoutBtns->addWidget(btnCopy);
    layoutBtns->addStretch();
    layoutBtns->addWidget(btnClose);

    mainLayout->addLayout(layoutBtns);
}

void TimestampConverterDialog::setToCurrentTime() {
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const qint64 sec = now.toSecsSinceEpoch();
    const qint64 ms = now.toMSecsSinceEpoch();

    editEpochSec->setText(QString::number(sec));
    editEpochMs->setText(QString::number(ms));
    editIsoUtc->setText(now.toString(Qt::ISODateWithMs));
    editIsoLocal->setText(now.toLocalTime().toString(Qt::ISODateWithMs));
    lblHuman->setText(now.toLocalTime().toString(QStringLiteral("dddd, MMMM d, yyyy h:mm:ss AP")));
}

void TimestampConverterDialog::convertFromEpoch() {
    const QString txt = editEpochSec->text().trimmed();
    bool ok = false;
    qint64 sec = txt.toLongLong(&ok);
    if (!ok) {
        return;
    }

    if (txt.length() >= 13) {
        sec = sec / 1000;
        editEpochMs->setText(txt);
        editEpochSec->setText(QString::number(sec));
    } else {
        editEpochMs->setText(QString::number(sec * 1000));
    }

    const QDateTime dt = QDateTime::fromSecsSinceEpoch(sec, QTimeZone::UTC);
    editIsoUtc->setText(dt.toString(Qt::ISODate));
    editIsoLocal->setText(dt.toLocalTime().toString(Qt::ISODate));
    lblHuman->setText(dt.toLocalTime().toString(QStringLiteral("dddd, MMMM d, yyyy h:mm:ss AP")));
}

void TimestampConverterDialog::convertFromIso() {
    const QString txt = editIsoUtc->text().trimmed();
    bool ok = false;
    const qint64 sec = DeveloperToolsEngine::isoToEpoch(txt, &ok);
    if (!ok) {
        return;
    }

    editEpochSec->setText(QString::number(sec));
    editEpochMs->setText(QString::number(sec * 1000));

    const QDateTime dt = QDateTime::fromSecsSinceEpoch(sec, QTimeZone::UTC);
    editIsoLocal->setText(dt.toLocalTime().toString(Qt::ISODate));
    lblHuman->setText(dt.toLocalTime().toString(QStringLiteral("dddd, MMMM d, yyyy h:mm:ss AP")));
}

void TimestampConverterDialog::copySelectedToClipboard() const {
    if (QClipboard *const cb = QApplication::clipboard(); cb && editIsoUtc) {
        cb->setText(editIsoUtc->text());
    }
}
