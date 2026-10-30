#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad3 Focused View & line filtering architecture.
 * Sources: rizonesoft/Notepad3 (src/Edit.c, FocusedView)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef FOCUSEDVIEWDIALOG_H
#define FOCUSEDVIEWDIALOG_H

#include <QDialog>
#include "FocusedView.h"

class QLineEdit;
class QCheckBox;
class QSpinBox;
class QLabel;
class QPushButton;
class MainWindow;
class ScintillaNext;

class FocusedViewDialog : public QDialog {
    Q_OBJECT

public:
    explicit FocusedViewDialog(FocusedView *focusedView, MainWindow *mainWindow, QWidget *parent = nullptr);
    ~FocusedViewDialog() override = default;

    void prepareForEditor(ScintillaNext *editor);

private slots:
    void onApplyClicked();
    void onClearClicked();
    void onBookmarksToggled(bool checked);

private:
    void setupUi();

    FocusedView *m_focusedView;
    MainWindow *m_mainWindow;

    QLineEdit *m_queryEdit{nullptr};
    QCheckBox *m_caseSensitive{nullptr};
    QCheckBox *m_wholeWord{nullptr};
    QCheckBox *m_useRegex{nullptr};
    QCheckBox *m_invertFilter{nullptr};
    QCheckBox *m_bookmarksOnly{nullptr};
    QSpinBox *m_contextLines{nullptr};
    QLabel *m_statusLabel{nullptr};
    QPushButton *m_applyBtn{nullptr};
    QPushButton *m_clearBtn{nullptr};
    QPushButton *m_closeBtn{nullptr};
};

#endif  // FOCUSEDVIEWDIALOG_H
