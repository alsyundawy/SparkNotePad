#pragma once
/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef DISPLAYINFODIALOG_H
#define DISPLAYINFODIALOG_H

#include <QDialog>

class QTableWidget;
class QLabel;

class DisplayInfoDialog : public QDialog {
    Q_OBJECT

public:
    explicit DisplayInfoDialog(QWidget *parent = nullptr);
    ~DisplayInfoDialog() override = default;

private slots:
    void refreshDisplays();
    void onMoveToNextClicked();
    void onMoveToPrevClicked();
    void onCenterWindowClicked();
    void onMoveToPrimaryClicked();

private:
    void setupUi();

    QLabel *m_summaryLabel = nullptr;
    QTableWidget *m_tableWidget = nullptr;
};

#endif  // DISPLAYINFODIALOG_H
