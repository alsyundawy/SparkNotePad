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

#ifndef ACCESSIBILITYHELPDIALOG_H
#define ACCESSIBILITYHELPDIALOG_H

#include <QDialog>

namespace Ui {
class AccessibilityHelpDialog;
}

class QTreeWidgetItem;

class AccessibilityHelpDialog : public QDialog {
    Q_OBJECT

public:
    explicit AccessibilityHelpDialog(QWidget *parent = nullptr);
    ~AccessibilityHelpDialog() override;

private slots:
    void filterShortcuts(const QString &text);

private:
    Ui::AccessibilityHelpDialog *ui;
    void populateShortcuts();
    void addShortcutItem(const QString &action, const QString &shortcut, const QString &category,
                         const QString &description);
};

#endif  // ACCESSIBILITYHELPDIALOG_H
