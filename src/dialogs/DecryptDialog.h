#pragma once
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

#ifndef DECRYPTDIALOG_H
#define DECRYPTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class DecryptDialog;
}

class DecryptDialog : public QDialog {
    Q_OBJECT

public:
    explicit DecryptDialog(QWidget *parent = nullptr, const QString &fileName = QString());
    ~DecryptDialog() override;

    QString passphrase() const;
    void setErrorMessage(const QString &msg);

private slots:
    void onTogglePassVisibility();
    void onTextChanged(const QString &text);

private:
    Ui::DecryptDialog *ui;
};

#endif  // DECRYPTDIALOG_H
