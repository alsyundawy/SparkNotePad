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
 */

#include <QDialog>
#include <QString>
#include <QVector>

class QLineEdit;
class QListWidget;
class QListWidgetItem;
class MainWindow;
class ScintillaNext;

class QuickSwitcherDialog : public QDialog {
    Q_OBJECT

public:
    struct Item {
        QString title;
        QString detailPath;
        ScintillaNext *editor = nullptr;
        int score = 0;
    };

    explicit QuickSwitcherDialog(QWidget *parent = nullptr);
    ~QuickSwitcherDialog() override = default;

    static int fuzzyScore(const QString &pattern, const QString &text);
    static QVector<Item> filterItems(const QVector<Item> &items, const QString &query);

    void populateItems(const QVector<Item> &items);
    void execFuzzySearch(const QString &query);

signals:
    void itemSelected(const Item &item);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onSearchTextChanged(const QString &text);
    void onItemActivated(QListWidgetItem *listItem);

private:
    void setupUi();
    void renderResults(const QVector<Item> &results);

    MainWindow *m_mainWindow{nullptr};
    QLineEdit *m_searchEdit{nullptr};
    QListWidget *m_listWidget{nullptr};
    QVector<Item> m_allItems;
    QVector<Item> m_filteredItems;
};
