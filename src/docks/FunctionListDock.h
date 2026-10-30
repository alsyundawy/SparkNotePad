#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad++ Function List panel concept.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef FUNCTIONLISTDOCK_H
#define FUNCTIONLISTDOCK_H

#include <QDockWidget>
#include <QVector>
#include <QTimer>

#include "FunctionParser.h"

namespace Ui {
class FunctionListDock;
}

class MainWindow;
class ScintillaNext;
class QTreeWidgetItem;

class FunctionListDock : public QDockWidget {
    Q_OBJECT

public:
    explicit FunctionListDock(MainWindow *parent = nullptr);
    ~FunctionListDock() override;

public slots:
    void setEditor(ScintillaNext *editor);
    void refreshFunctions();

private slots:
    void onSearchTextChanged(const QString &text);
    void onSortAlphaToggled(bool checked);
    void onGroupScopeToggled(bool checked);
    void onItemActivated(QTreeWidgetItem *item, int column);
    void onEditorModified();

private:
    void populateTree();
    void connectToEditor(ScintillaNext *editor);
    void disconnectFromEditor();

    Ui::FunctionListDock *ui;
    MainWindow *m_window;
    ScintillaNext *m_editor{nullptr};
    QVector<FunctionSymbol> m_symbols;
    QTimer *m_debounceTimer;
    bool m_sortAlpha = false;
    bool m_groupByScope = true;
    QString m_filterText;
    QMetaObject::Connection m_modifiedConn;
    QMetaObject::Connection m_lexerConn;
};

#endif  // FUNCTIONLISTDOCK_H
