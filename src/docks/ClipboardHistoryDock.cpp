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

#include "ClipboardHistoryDock.h"
#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

ClipboardHistoryDock::ClipboardHistoryDock(QWidget *parent) : QDockWidget(tr("Clipboard History"), parent) {
    setObjectName(QStringLiteral("ClipboardHistoryDock"));
    setupUi();

    if (QClipboard *const clipboard = QApplication::clipboard(); clipboard) {
        connect(clipboard, &QClipboard::dataChanged, this, &ClipboardHistoryDock::onClipboardChanged);
    }
}

void ClipboardHistoryDock::setupUi() {
    auto *const container = new QWidget(this);
    auto *const mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    m_searchEdit = new QLineEdit(container);
    m_searchEdit->setPlaceholderText(tr("Search clipboard history..."));
    m_searchEdit->setClearButtonEnabled(true);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &ClipboardHistoryDock::onSearchTextChanged);
    mainLayout->addWidget(m_searchEdit);

    m_listWidget = new QListWidget(container);
    m_listWidget->setAlternatingRowColors(true);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, &ClipboardHistoryDock::onItemDoubleClicked);
    mainLayout->addWidget(m_listWidget);

    auto *const btnLayout = new QHBoxLayout();
    m_btnInsert = new QPushButton(tr("Insert"), container);
    m_btnCopy = new QPushButton(tr("Copy"), container);
    m_btnClear = new QPushButton(tr("Clear"), container);

    connect(m_btnInsert, &QPushButton::clicked, this, &ClipboardHistoryDock::onInsertClicked);
    connect(m_btnCopy, &QPushButton::clicked, this, &ClipboardHistoryDock::onCopyClicked);
    connect(m_btnClear, &QPushButton::clicked, this, &ClipboardHistoryDock::clear);

    btnLayout->addWidget(m_btnInsert);
    btnLayout->addWidget(m_btnCopy);
    btnLayout->addWidget(m_btnClear);
    mainLayout->addLayout(btnLayout);

    setWidget(container);
}

QStringList ClipboardHistoryDock::entries() const {
    return m_history;
}

void ClipboardHistoryDock::addClip(const QString &text) {
    if (text.trimmed().isEmpty()) {
        return;
    }

    if (!m_history.isEmpty() && m_history.first() == text) {
        return;
    }

    m_history.prepend(text);
    while (m_history.size() > m_maxEntries) {
        m_history.removeLast();
    }

    refreshList();
}

void ClipboardHistoryDock::clear() {
    m_history.clear();
    refreshList();
}

void ClipboardHistoryDock::setMaxEntries(int max) {
    if (max > 0) {
        m_maxEntries = max;
        while (m_history.size() > m_maxEntries) {
            m_history.removeLast();
        }
        refreshList();
    }
}

int ClipboardHistoryDock::maxEntries() const {
    return m_maxEntries;
}

void ClipboardHistoryDock::onClipboardChanged() {
    const QClipboard *const clipboard = QApplication::clipboard();
    if (!clipboard) {
        return;
    }

    const QString text = clipboard->text();
    if (!text.isEmpty()) {
        addClip(text);
    }
}

void ClipboardHistoryDock::onSearchTextChanged(const QString &filter) {
    (void)filter;
    refreshList();
}

void ClipboardHistoryDock::refreshList() {
    if (!m_listWidget) {
        return;
    }

    m_listWidget->clear();
    const QString filter = m_searchEdit ? m_searchEdit->text().trimmed() : QString();

    for (const QString &item : m_history) {
        if (!filter.isEmpty() && !item.contains(filter, Qt::CaseInsensitive)) {
            continue;
        }

        QString display = item;
        display.replace(QLatin1Char('\n'), QStringLiteral(" ↵ "));
        display.replace(QLatin1Char('\r'), QString());
        display.replace(QLatin1Char('\t'), QStringLiteral("   "));
        if (display.length() > 80) {
            display = display.left(77) + QStringLiteral("...");
        }

        auto *const listItem = new QListWidgetItem(display, m_listWidget);
        listItem->setData(Qt::UserRole, item);
        listItem->setToolTip(item.left(500));
        m_listWidget->addItem(listItem);
    }
}

void ClipboardHistoryDock::onItemDoubleClicked(const QListWidgetItem *item) {
    if (!item) {
        return;
    }
    const QString text = item->data(Qt::UserRole).toString();
    emit clipSelected(text);
}

void ClipboardHistoryDock::onInsertClicked() {
    if (!m_listWidget) {
        return;
    }
    if (const QListWidgetItem *const item = m_listWidget->currentItem(); item) {
        onItemDoubleClicked(item);
    }
}

void ClipboardHistoryDock::onCopyClicked() {
    if (!m_listWidget) {
        return;
    }
    if (const QListWidgetItem *const item = m_listWidget->currentItem(); item) {
        const QString text = item->data(Qt::UserRole).toString();
        if (QClipboard *const clipboard = QApplication::clipboard(); clipboard) {
            clipboard->setText(text);
        }
    }
}
