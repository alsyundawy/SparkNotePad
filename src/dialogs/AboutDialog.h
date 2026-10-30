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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class QTabWidget;

class AboutDialog : public QDialog {
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override = default;

private slots:
    void copySystemInfoToClipboard();

private:
    void setupUi();
    QWidget *createAboutTab();
    QWidget *createCreditsTab();
    QWidget *createLicenseTab();
    QWidget *createEnvironmentTab();

    QTabWidget *m_tabWidget = nullptr;
};

#endif  // ABOUTDIALOG_H
