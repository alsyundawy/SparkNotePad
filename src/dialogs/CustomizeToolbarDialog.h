#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 * https://github.com/alsyundawy/SparkNotePad
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef CUSTOMIZETOOLBARDIALOG_H
#define CUSTOMIZETOOLBARDIALOG_H

#include <QDialog>
#include <QMap>
#include <QStringList>

class QListWidget;
class QListWidgetItem;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;
class QAction;
class MainWindow;

class CustomizeToolbarDialog : public QDialog {
    Q_OBJECT

public:
    explicit CustomizeToolbarDialog(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~CustomizeToolbarDialog() override = default;

    static const QStringList &defaultToolbarActions();
    [[nodiscard]] QStringList currentActionNames() const;

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onAddSeparatorClicked();
    void onMoveUpClicked();
    void onMoveDownClicked();
    void onResetDefaultsClicked();
    void onFilterTextChanged(const QString &text);
    void onCurrentItemSelectionChanged();
    void onApplyClicked();

private:
    void setupUi();
    void populateAvailableActions();
    void populateCurrentActions();
    void insertCurrentActionItem(int row, const QString &actionName);
    static QString cleanActionText(const QString &text);

    MainWindow *m_mainWindow = nullptr;
    QLineEdit *m_filterEdit = nullptr;
    QListWidget *m_availableList = nullptr;
    QListWidget *m_currentList = nullptr;

    QPushButton *m_btnAdd = nullptr;
    QPushButton *m_btnRemove = nullptr;
    QPushButton *m_btnAddSeparator = nullptr;
    QPushButton *m_btnMoveUp = nullptr;
    QPushButton *m_btnMoveDown = nullptr;
    QPushButton *m_btnReset = nullptr;
    QDialogButtonBox *m_buttonBox = nullptr;

    QMap<QString, QAction *> m_availableActions;
};

#endif  // CUSTOMIZETOOLBARDIALOG_H
