/*
 * This file is part of NoteSciPad.
 *
 * Implements AES-256 decryption prompt dialog.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "DecryptDialog.h"
#include "ui_DecryptDialog.h"

#include <QPushButton>

DecryptDialog::DecryptDialog(QWidget *parent, const QString &fileName) : QDialog(parent), ui(new Ui::DecryptDialog) {
    ui->setupUi(this);

    if (!fileName.isEmpty()) {
        ui->labelFile->setText(tr("Document: <b><i>%1</i></b>").arg(fileName));
    } else {
        ui->labelFile->setVisible(false);
    }

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Unlock"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->btnTogglePass, &QPushButton::clicked, this, &DecryptDialog::onTogglePassVisibility);
    connect(ui->editPassword, &QLineEdit::textChanged, this, &DecryptDialog::onTextChanged);

    ui->editPassword->setFocus();
}

DecryptDialog::~DecryptDialog() {
    delete ui;
}

QString DecryptDialog::passphrase() const {
    return ui->editPassword->text();
}

void DecryptDialog::setErrorMessage(const QString &msg) {
    ui->labelError->setText(msg);
    ui->labelError->setVisible(!msg.isEmpty());
    ui->editPassword->selectAll();
    ui->editPassword->setFocus();
}

void DecryptDialog::onTogglePassVisibility() {
    if (ui->editPassword->echoMode() == QLineEdit::Password) {
        ui->editPassword->setEchoMode(QLineEdit::Normal);
    } else {
        ui->editPassword->setEchoMode(QLineEdit::Password);
    }
}

void DecryptDialog::onTextChanged(const QString &text) {
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
    ui->labelError->clear();
}
