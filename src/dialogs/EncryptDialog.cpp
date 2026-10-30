/*
 * This file is part of NoteSciPad.
 *
 * Implements AES-256 encryption configuration dialog.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "EncryptDialog.h"
#include "ui_EncryptDialog.h"
#include "../utils/CryptoManager.h"

#include <QPushButton>

EncryptDialog::EncryptDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EncryptDialog) {
    ui->setupUi(this);

    connect(ui->editPassword, &QLineEdit::textChanged, this, &EncryptDialog::onPasswordChanged);
    connect(ui->editConfirm, &QLineEdit::textChanged, this, &EncryptDialog::onPasswordChanged);
    connect(ui->btnTogglePass, &QPushButton::clicked, this, &EncryptDialog::onTogglePassVisibility);
    connect(ui->btnToggleConfirm, &QPushButton::clicked, this, &EncryptDialog::onToggleConfirmVisibility);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EncryptDialog::onAccept);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    updateValidation();
}

EncryptDialog::~EncryptDialog() {
    delete ui;
}

QString EncryptDialog::passphrase() const {
    return ui->editPassword->text();
}

void EncryptDialog::onPasswordChanged() {
    updateValidation();
}

void EncryptDialog::onTogglePassVisibility() {
    if (ui->editPassword->echoMode() == QLineEdit::Password) {
        ui->editPassword->setEchoMode(QLineEdit::Normal);
    } else {
        ui->editPassword->setEchoMode(QLineEdit::Password);
    }
}

void EncryptDialog::onToggleConfirmVisibility() {
    if (ui->editConfirm->echoMode() == QLineEdit::Password) {
        ui->editConfirm->setEchoMode(QLineEdit::Normal);
    } else {
        ui->editConfirm->setEchoMode(QLineEdit::Password);
    }
}

void EncryptDialog::updateValidation() {
    const QString pass = ui->editPassword->text();
    const QString confirm = ui->editConfirm->text();

    CryptoManager::StrengthResult sr = CryptoManager::evaluatePasswordStrength(pass);
    ui->progressBarStrength->setValue(sr.score);
    ui->labelStrengthText->setText(sr.label);

    QString color;
    if (sr.score < 25)
        color = QStringLiteral("#d9534f");
    else if (sr.score < 50)
        color = QStringLiteral("#f0ad4e");
    else if (sr.score < 70)
        color = QStringLiteral("#5bc0de");
    else
        color = QStringLiteral("#5cb85c");

    ui->progressBarStrength->setStyleSheet(
        QStringLiteral("QProgressBar::chunk { background-color: %1; border-radius: 3px; }").arg(color));

    QPushButton *okBtn = ui->buttonBox->button(QDialogButtonBox::Ok);

    if (pass.isEmpty()) {
        ui->labelError->setText(tr("Passphrase cannot be empty."));
        ui->labelError->setVisible(true);
        okBtn->setEnabled(false);
        return;
    }

    if (pass.length() < 6) {
        ui->labelError->setText(tr("Passphrase should be at least 6 characters."));
        ui->labelError->setVisible(true);
        okBtn->setEnabled(false);
        return;
    }

    if (!confirm.isEmpty() && pass != confirm) {
        ui->labelError->setText(tr("Passphrases do not match."));
        ui->labelError->setVisible(true);
        okBtn->setEnabled(false);
        return;
    }

    if (confirm.isEmpty()) {
        ui->labelError->setText(tr("Please confirm your passphrase."));
        ui->labelError->setVisible(true);
        okBtn->setEnabled(false);
        return;
    }

    ui->labelError->setText(QString());
    ui->labelError->setVisible(false);
    okBtn->setEnabled(true);
}

void EncryptDialog::onAccept() {
    if (ui->editPassword->text() == ui->editConfirm->text() && !ui->editPassword->text().isEmpty()) {
        accept();
    }
}
