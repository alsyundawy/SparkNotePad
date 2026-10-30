#pragma once
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

#ifndef ENCRYPTDIALOG_H
#define ENCRYPTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class EncryptDialog;
}

class EncryptDialog : public QDialog {
    Q_OBJECT

public:
    explicit EncryptDialog(QWidget *parent = nullptr);
    ~EncryptDialog() override;

    QString passphrase() const;

private slots:
    void onPasswordChanged();
    void onTogglePassVisibility();
    void onToggleConfirmVisibility();
    void onAccept();

private:
    void updateValidation();

    Ui::EncryptDialog *ui;
};

#endif  // ENCRYPTDIALOG_H
