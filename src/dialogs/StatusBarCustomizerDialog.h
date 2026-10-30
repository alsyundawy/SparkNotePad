#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad3 status bar customization architecture.
 * Sources: rizonesoft/Notepad3 (src/Notepad3.ini, Statusbar Settings)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef STATUSBARCUSTOMIZERDIALOG_H
#define STATUSBARCUSTOMIZERDIALOG_H

#include <QDialog>
#include <QMap>

class QCheckBox;
class QPushButton;
class EditorInfoStatusBar;

class StatusBarCustomizerDialog : public QDialog {
    Q_OBJECT

public:
    explicit StatusBarCustomizerDialog(EditorInfoStatusBar *statusBar, QWidget *parent = nullptr);
    ~StatusBarCustomizerDialog() override = default;

private slots:
    void onSelectAll();
    void onClearAll();
    void onResetDefaults();

private:
    void setupUi();

    EditorInfoStatusBar *m_statusBar;
    QMap<int, QCheckBox *> m_checkBoxes;
};

#endif  // STATUSBARCUSTOMIZERDIALOG_H
