#pragma once
/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef SHORTCUTCUSTOMIZERDIALOG_H
#define SHORTCUTCUSTOMIZERDIALOG_H

#include <QDialog>
#include <QKeySequence>
#include <QMap>

class QAction;
class QKeySequenceEdit;
class QLineEdit;
class QLabel;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
class QMainWindow;

struct ActionShortcutEntry {
    QAction *action = nullptr;
    QString objectName;
    QString category;
    QString description;
    QKeySequence defaultShortcut;
    QKeySequence currentShortcut;
    bool isModified = false;
};

class ShortcutCustomizerDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShortcutCustomizerDialog(QMainWindow *mainWindow, QWidget *parent = nullptr);
    ~ShortcutCustomizerDialog() override = default;

private slots:
    void onFilterTextChanged(const QString &text) const;
    void onItemSelectionChanged();
    void onAssignClicked();
    void onClearClicked();
    void onResetSelectedClicked();
    void onResetAllClicked();
    void onApplyClicked() const;
    void onOkClicked();
    void updateConflictWarning();

private:
    void setupUi();
    void collectActions();
    void registerAction(QAction *action, const QString &category);
    void traverseMenu(const QMenu *menu, const QString &catPrefix);
    void populateTree();
    void applyThemeStyles();
    void saveShortcuts() const;
    static QString cleanActionText(const QString &text);

    QMainWindow *m_mainWindow = nullptr;
    QTreeWidget *m_treeWidget = nullptr;
    QLineEdit *m_filterEdit = nullptr;
    QKeySequenceEdit *m_seqEdit = nullptr;
    QLabel *m_selectedActionLabel = nullptr;
    QLabel *m_conflictLabel = nullptr;
    QPushButton *m_btnAssign = nullptr;
    QPushButton *m_btnClear = nullptr;
    QPushButton *m_btnResetSelected = nullptr;
    QPushButton *m_btnResetAll = nullptr;
    QPushButton *m_btnOk = nullptr;
    QPushButton *m_btnCancel = nullptr;
    QPushButton *m_btnApply = nullptr;

    QMap<QString, ActionShortcutEntry> m_entries;  // Keyed by objectName
};

#endif  // SHORTCUTCUSTOMIZERDIALOG_H
