#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad++ Run menu custom commands management.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef MANAGETOOLSDIALOG_H
#define MANAGETOOLSDIALOG_H

#include <QDialog>
#include "ExternalToolManager.h"

namespace Ui {
class ManageToolsDialog;
}

class ManageToolsDialog : public QDialog {
    Q_OBJECT

public:
    explicit ManageToolsDialog(QWidget *parent = nullptr);
    ~ManageToolsDialog() override;

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void populateList();
    void editTool(int index);

    Ui::ManageToolsDialog *ui;
    QList<ExternalTool> m_tools;
};

#endif  // MANAGETOOLSDIALOG_H
