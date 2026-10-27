#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SparkNotePad is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef COMMANDPALETTEDIALOG_H
#define COMMANDPALETTEDIALOG_H

#include <QDialog>
#include <QPointer>
#include <QString>
#include <QVector>

class QLineEdit;
class QListWidget;
class QListWidgetItem;
class MainWindow;
class ScintillaNext;
class QAction;

class CommandPaletteDialog : public QDialog {
    Q_OBJECT

public:
    enum class InitialMode { FilesAndTabs, Commands };

    explicit CommandPaletteDialog(MainWindow *parent, InitialMode mode = InitialMode::FilesAndTabs);
    ~CommandPaletteDialog() override = default;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onSearchTextChanged(const QString &text);
    void onItemActivated(QListWidgetItem *item);

private:
    struct PaletteItem {
        enum class Type { OpenTab, RecentFile, Command, GoToLine };
        Type type{Type::OpenTab};
        QString title;
        QString subtitle;
        QString shortcut;
        QPointer<ScintillaNext> editor;
        QString filePath;
        QPointer<QAction> action;
        int targetLine{0};
    };

    void setupUi();
    void applyTheme();
    void populateItems();
    void filterItems(const QString &query);
    void executeItem(const PaletteItem &item);

    MainWindow *m_mainWindow{nullptr};
    QLineEdit *m_searchEdit{nullptr};
    QListWidget *m_listWidget{nullptr};

    QVector<PaletteItem> m_allItems;
    InitialMode m_initialMode{InitialMode::FilesAndTabs};
};

#endif  // COMMANDPALETTEDIALOG_H
