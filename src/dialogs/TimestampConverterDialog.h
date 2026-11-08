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

class QLineEdit;
class QLabel;
class QPushButton;

class TimestampConverterDialog : public QDialog {
    Q_OBJECT

public:
    explicit TimestampConverterDialog(QWidget *parent = nullptr);
    ~TimestampConverterDialog() override = default;

public slots:
    void setToCurrentTime();
    void convertFromEpoch();
    void convertFromIso();
    void copySelectedToClipboard() const;

private:
    void setupUi();

    QLineEdit *editEpochSec = nullptr;
    QLineEdit *editEpochMs = nullptr;
    QLineEdit *editIsoUtc = nullptr;
    QLineEdit *editIsoLocal = nullptr;
    QLabel *lblHuman = nullptr;

    QPushButton *btnNow = nullptr;
    QPushButton *btnConvert = nullptr;
    QPushButton *btnCopy = nullptr;
    QPushButton *btnClose = nullptr;
};
