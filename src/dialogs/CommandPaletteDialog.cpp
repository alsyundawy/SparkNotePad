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

#include "CommandPaletteDialog.h"
#include "DockedEditor.h"
#include "MainWindow.h"
#include "RecentFilesListManager.h"
#include "ScintillaNext.h"
#include "SparkNotePadApplication.h"
#include "features/ThemeManager.h"
#include "features/ThemePaletteData.h"

#include <QAction>
#include <QFileInfo>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPainter>
#include <QVBoxLayout>

CommandPaletteDialog::CommandPaletteDialog(MainWindow *parent, InitialMode mode)
    : QDialog(parent, Qt::Popup | Qt::FramelessWindowHint), m_mainWindow(parent), m_initialMode(mode) {
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(true);
    setMinimumWidth(620);
    setMaximumWidth(700);
    setFixedHeight(400);

    setupUi();
    applyTheme();
    populateItems();

    if (m_initialMode == InitialMode::Commands) {
        m_searchEdit->setText(QStringLiteral(">"));
        m_searchEdit->setPlaceholderText(tr("> Type command name (e.g. wrap, format, theme, split)..."));
    } else {
        m_searchEdit->setPlaceholderText(tr("Search tabs, recent files, or type :line or >command..."));
        filterItems(QString());
    }
}

void CommandPaletteDialog::setupUi() {
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setClearButtonEnabled(true);
    m_searchEdit->installEventFilter(this);
    layout->addWidget(m_searchEdit);

    m_listWidget = new QListWidget(this);
    m_listWidget->setFrameShape(QFrame::NoFrame);
    m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_listWidget->installEventFilter(this);
    layout->addWidget(m_listWidget);

    connect(m_searchEdit, &QLineEdit::textChanged, this, &CommandPaletteDialog::onSearchTextChanged);
    connect(m_listWidget, &QListWidget::itemActivated, this, &CommandPaletteDialog::onItemActivated);
}

void CommandPaletteDialog::applyTheme() {
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    const bool dark = ThemeManager::instance()->isDarkActive();

    const QString bg =
        pal ? QString::fromLatin1(pal->card) : (dark ? QStringLiteral("#252526") : QStringLiteral("#ffffff"));
    const QString winBg =
        pal ? QString::fromLatin1(pal->win) : (dark ? QStringLiteral("#1e1e1e") : QStringLiteral("#f3f3f3"));
    const QString border =
        pal ? QString::fromLatin1(pal->border) : (dark ? QStringLiteral("#3f3f46") : QStringLiteral("#d0d7de"));
    const QString textPri =
        pal ? QString::fromLatin1(pal->txtPri) : (dark ? QStringLiteral("#e0e0e0") : QStringLiteral("#24292f"));
    const QString accent = pal ? QString::fromLatin1(pal->blue) : QStringLiteral("#0969da");

    setStyleSheet(QStringLiteral("QDialog {"
                                 "  background-color: %1;"
                                 "  border: 1px solid %2;"
                                 "  border-radius: 8px;"
                                 "}"
                                 "QLineEdit {"
                                 "  background-color: %3;"
                                 "  color: %4;"
                                 "  border: 1px solid %2;"
                                 "  border-radius: 6px;"
                                 "  padding: 8px 12px;"
                                 "  font-size: 13px;"
                                 "}"
                                 "QLineEdit:focus {"
                                 "  border: 1px solid %5;"
                                 "}"
                                 "QListWidget {"
                                 "  background-color: transparent;"
                                 "  border: none;"
                                 "  outline: none;"
                                 "}"
                                 "QListWidget::item {"
                                 "  color: %4;"
                                 "  padding: 6px 10px;"
                                 "  border-radius: 4px;"
                                 "  margin: 1px 0px;"
                                 "}"
                                 "QListWidget::item:selected {"
                                 "  background-color: %5;"
                                 "  color: #ffffff;"
                                 "}")
                      .arg(winBg, border, bg, textPri, accent));
}

void CommandPaletteDialog::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    if (parentWidget()) {
        const QPoint globalCenter = parentWidget()->mapToGlobal(parentWidget()->rect().center());
        const int x = globalCenter.x() - (width() / 2);
        const int y = parentWidget()->mapToGlobal(QPoint(0, 0)).y() + 60;
        move(x, y);
    }
    m_searchEdit->setFocus();
}

bool CommandPaletteDialog::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Escape) {
            close();
            return true;
        }
        if (watched == m_searchEdit) {
            if (keyEvent->key() == Qt::Key_Down) {
                int nextRow = m_listWidget->currentRow() + 1;
                if (nextRow < m_listWidget->count()) {
                    m_listWidget->setCurrentRow(nextRow);
                }
                return true;
            }
            if (keyEvent->key() == Qt::Key_Up) {
                int prevRow = m_listWidget->currentRow() - 1;
                if (prevRow >= 0) {
                    m_listWidget->setCurrentRow(prevRow);
                }
                return true;
            }
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                if (auto *item = m_listWidget->currentItem()) {
                    onItemActivated(item);
                }
                return true;
            }
        }
    }
    return QDialog::eventFilter(watched, event);
}

void CommandPaletteDialog::populateItems() {
    m_allItems.clear();

    if (!m_mainWindow) {
        return;
    }

    // 1. Populate Open Tabs
    if (auto *docked = m_mainWindow->getDockedEditor()) {
        const auto editors = docked->editors();
        const auto *current = m_mainWindow->currentEditor();
        for (auto *ed : editors) {
            if (!ed) {
                continue;
            }
            PaletteItem item;
            item.type = PaletteItem::Type::OpenTab;
            item.title = ed->getName();
            item.filePath = ed->getFilePath();
            item.editor = ed;
            item.subtitle =
                ed == current ? tr("[Active Tab] %1").arg(item.filePath) : tr("[Open Tab] %1").arg(item.filePath);
            m_allItems.append(item);
        }
    }

    // 2. Populate Recent Files
    auto *app = qobject_cast<SparkNotePadApplication *>(qApp);
    if (app && app->getRecentFilesListManager()) {
        const auto recentList = app->getRecentFilesListManager()->fileList();
        for (const auto &filePath : recentList) {
            QFileInfo fi(filePath);
            PaletteItem item;
            item.type = PaletteItem::Type::RecentFile;
            item.title = fi.fileName();
            item.filePath = filePath;
            item.subtitle = tr("[Recent] %1").arg(fi.absolutePath());
            m_allItems.append(item);
        }
    }

    // 3. Populate Actions (Commands)
    const auto actions = m_mainWindow->findChildren<QAction *>();
    for (auto *act : actions) {
        if (!act || act->isSeparator() || act->text().trimmed().isEmpty() || !act->isEnabled()) {
            continue;
        }

        QString cleanText = act->text();
        cleanText.remove(QLatin1Char('&'));

        PaletteItem item;
        item.type = PaletteItem::Type::Command;
        item.title = cleanText;
        item.action = act;

        QString shortcut = act->shortcut().toString(QKeySequence::NativeText);
        if (!shortcut.isEmpty()) {
            item.shortcut = shortcut;
            item.subtitle = tr("[Action] Shortcut: %1").arg(shortcut);
        } else {
            item.subtitle = tr("[Action]");
        }
        m_allItems.append(item);
    }
}

void CommandPaletteDialog::onSearchTextChanged(const QString &text) {
    filterItems(text);
}

void CommandPaletteDialog::filterItems(const QString &rawQuery) {
    m_listWidget->clear();
    const QString query = rawQuery.trimmed();

    // Mode 1: Go to line (starts with ':')
    if (query.startsWith(QLatin1Char(':'))) {
        bool ok = false;
        int lineNum = query.mid(1).trimmed().toInt(&ok);
        auto *editor = m_mainWindow ? m_mainWindow->currentEditor() : nullptr;
        if (editor && ok && lineNum > 0) {
            auto *listItem = new QListWidgetItem(m_listWidget);
            listItem->setText(tr("Jump to line %1 (Max: %2)").arg(lineNum).arg(editor->lineCount()));
            listItem->setData(Qt::UserRole, static_cast<int>(PaletteItem::Type::GoToLine));
            listItem->setData(Qt::UserRole + 1, lineNum);
            m_listWidget->setCurrentRow(0);
        }
        return;
    }

    // Mode 2: Commands mode (starts with '>')
    const bool commandOnly = query.startsWith(QLatin1Char('>'));
    const QString cleanQuery = commandOnly ? query.mid(1).trimmed().toLower() : query.toLower();

    for (const auto &item : m_allItems) {
        if (commandOnly && item.type != PaletteItem::Type::Command) {
            continue;
        }
        if (!commandOnly && item.type == PaletteItem::Type::Command) {
            continue;
        }

        bool match = false;
        if (cleanQuery.isEmpty()) {
            match = true;
        } else {
            const QString target = (item.title + QLatin1Char(' ') + item.subtitle).toLower();
            match = target.contains(cleanQuery);
        }

        if (match) {
            auto *listItem = new QListWidgetItem(m_listWidget);
            QString label = item.title;
            if (!item.shortcut.isEmpty()) {
                label += QStringLiteral("  [%1]").arg(item.shortcut);
            }
            if (!item.subtitle.isEmpty()) {
                label += QStringLiteral(" — %1").arg(item.subtitle);
            }
            listItem->setText(label);
            listItem->setData(Qt::UserRole, static_cast<int>(item.type));
            listItem->setData(Qt::UserRole + 1, item.filePath);
            listItem->setData(Qt::UserRole + 2, QVariant::fromValue(static_cast<void *>(item.editor.data())));
            listItem->setData(Qt::UserRole + 3, QVariant::fromValue(static_cast<void *>(item.action.data())));
        }
    }

    if (m_listWidget->count() > 0) {
        m_listWidget->setCurrentRow(0);
    }
}

void CommandPaletteDialog::onItemActivated(QListWidgetItem *listItem) {
    if (!listItem) {
        return;
    }

    const auto type = static_cast<PaletteItem::Type>(listItem->data(Qt::UserRole).toInt());

    if (type == PaletteItem::Type::GoToLine) {
        int line = listItem->data(Qt::UserRole + 1).toInt();
        if (m_mainWindow && m_mainWindow->currentEditor() && line > 0) {
            m_mainWindow->currentEditor()->gotoLine(line - 1);
        }
        close();
        return;
    }

    if (type == PaletteItem::Type::OpenTab) {
        auto *editor = static_cast<ScintillaNext *>(listItem->data(Qt::UserRole + 2).value<void *>());
        if (m_mainWindow && m_mainWindow->getDockedEditor() && editor) {
            m_mainWindow->getDockedEditor()->switchToEditor(editor);
        }
        close();
        return;
    }

    if (type == PaletteItem::Type::RecentFile) {
        QString path = listItem->data(Qt::UserRole + 1).toString();
        if (m_mainWindow && !path.isEmpty()) {
            m_mainWindow->openFile(path);
        }
        close();
        return;
    }

    if (type == PaletteItem::Type::Command) {
        auto *action = static_cast<QAction *>(listItem->data(Qt::UserRole + 3).value<void *>());
        close();
        if (action && action->isEnabled()) {
            action->trigger();
        }
    }
}
