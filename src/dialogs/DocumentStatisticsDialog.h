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

#ifndef DOCUMENTSTATISTICSDIALOG_H
#define DOCUMENTSTATISTICSDIALOG_H

#include <QDialog>
#include "utils/TokenForensicUtils.h"

class QLabel;
class QPushButton;
class QTableWidget;

class DocumentStatisticsDialog : public QDialog {
    Q_OBJECT

public:
    explicit DocumentStatisticsDialog(const QString &documentName, const TokenForensicUtils::DocumentStats &stats,
                                      QWidget *parent = nullptr);
    ~DocumentStatisticsDialog() override = default;

private slots:
    void onCopyStatsClicked();

private:
    void setupUi(const QString &documentName, const TokenForensicUtils::DocumentStats &stats);

    QTableWidget *m_tableWidget{nullptr};
    QPushButton *m_copyButton{nullptr};
    QPushButton *m_closeButton{nullptr};
    QString m_statsSummaryText;
};

#endif  // DOCUMENTSTATISTICSDIALOG_H
