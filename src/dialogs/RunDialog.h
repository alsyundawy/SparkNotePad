#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad++ Run dialog concept.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef RUNDIALOG_H
#define RUNDIALOG_H

#include <QDialog>

class ScintillaNext;
class MainWindow;

namespace Ui {
class RunDialog;
}

class RunDialog : public QDialog {
    Q_OBJECT

public:
    explicit RunDialog(MainWindow *parent, ScintillaNext *editor);
    ~RunDialog() override;

private slots:
    void onCommandTextChanged(const QString &text);
    void onBrowseClicked();
    void onInsertVarTriggered(const QString &varName);
    void onRunClicked();
    void onSaveClicked();

private:
    void setupInsertVarMenu();
    void updatePreview();

    Ui::RunDialog *ui;
    MainWindow *m_window;
    ScintillaNext *m_editor;
};

#endif  // RUNDIALOG_H
