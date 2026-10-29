#pragma once
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

#include <QDialog>
#include <QString>

class QCheckBox;
class QPlainTextEdit;
class QPushButton;

class GenerateHashDigestDialog : public QDialog {
    Q_OBJECT

public:
    explicit GenerateHashDigestDialog(const QString &algorithmName, const QString &initialText = {},
                                      QWidget *parent = nullptr);
    ~GenerateHashDigestDialog() override = default;

    static QString computeSingleDigest(const QString &algorithmName, const QByteArray &data);

private slots:
    void onInputChanged();
    void onCopyClicked();

private:
    void setupUi();
    void updateDigest();

    QString m_algorithmName;
    QCheckBox *m_chkPerLine = nullptr;
    QPlainTextEdit *m_textInput = nullptr;
    QPlainTextEdit *m_textOutput = nullptr;
    QPushButton *m_btnCopy = nullptr;
    QPushButton *m_btnClose = nullptr;
};
