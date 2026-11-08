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

#include <QDockWidget>
#include <QStringList>

class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QPushButton;

class ClipboardHistoryDock : public QDockWidget {
    Q_OBJECT

public:
    explicit ClipboardHistoryDock(QWidget *parent = nullptr);
    ~ClipboardHistoryDock() override = default;

    [[nodiscard]] QStringList entries() const;
    void addClip(const QString &text);
    void clear();
    void setMaxEntries(int max);
    [[nodiscard]] int maxEntries() const;

signals:
    void clipSelected(const QString &text);

public slots:
    void onSearchTextChanged(const QString &filter);
    void onInsertClicked();
    void onCopyClicked();
    void onItemDoubleClicked(const QListWidgetItem *item);
    void onClipboardChanged();

private:
    void setupUi();
    void refreshList();

    QStringList m_history;
    int m_maxEntries{20};
    QLineEdit *m_searchEdit{nullptr};
    QListWidget *m_listWidget{nullptr};
    QPushButton *m_btnInsert{nullptr};
    QPushButton *m_btnCopy{nullptr};
    QPushButton *m_btnClear{nullptr};
};
