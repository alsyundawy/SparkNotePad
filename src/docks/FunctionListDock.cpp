/*
 * This file is part of NoteSciPad.
 *
 * Function and symbol dock panel for NoteSciPad.
 * Adapted from Notepad++ Function List panel concept.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "FunctionListDock.h"
#include "ui_FunctionListDock.h"

#include "MainWindow.h"
#include "ScintillaNext.h"

#include <QStyle>
#include <QHeaderView>

FunctionListDock::FunctionListDock(MainWindow *parent)
    : QDockWidget(parent), ui(new Ui::FunctionListDock), m_window(parent), m_debounceTimer(new QTimer(this)) {
    ui->setupUi(this);

    // Setup tree view columns & behavior
    ui->treeWidget->header()->setStretchLastSection(false);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    // Debounce timer for editing updates (300ms)
    m_debounceTimer->setSingleShot(true);
    m_debounceTimer->setInterval(300);
    connect(m_debounceTimer, &QTimer::timeout, this, &FunctionListDock::refreshFunctions);

    // Toolbar buttons
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &FunctionListDock::onSearchTextChanged);
    connect(ui->btnSortAlpha, &QToolButton::toggled, this, &FunctionListDock::onSortAlphaToggled);
    connect(ui->btnGroupScope, &QToolButton::toggled, this, &FunctionListDock::onGroupScopeToggled);
    ui->btnGroupScope->setChecked(true);  // Default to grouped
    connect(ui->btnRefresh, &QToolButton::clicked, this, &FunctionListDock::refreshFunctions);

    // Item activation (double click or enter)
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this,
            [this](QTreeWidgetItem *item, int col) { onItemActivated(item, col); });
    connect(ui->treeWidget, &QTreeWidget::itemActivated, this, &FunctionListDock::onItemActivated);

    // CPU preservation: only active when visible
    connect(this, &QDockWidget::visibilityChanged, this, [this](bool visible) {
        if (visible) {
            connectToEditor(m_window ? m_window->currentEditor() : nullptr);
            if (m_window) {
                connect(m_window, &MainWindow::editorActivated, this, &FunctionListDock::setEditor,
                        Qt::UniqueConnection);
            }
        } else {
            disconnectFromEditor();
            if (m_window) {
                disconnect(m_window, &MainWindow::editorActivated, this, &FunctionListDock::setEditor);
            }
        }
    });
}

FunctionListDock::~FunctionListDock() {
    delete ui;
}

void FunctionListDock::setEditor(ScintillaNext *editor) {
    if (m_editor == editor)
        return;

    connectToEditor(editor);
}

void FunctionListDock::connectToEditor(ScintillaNext *editor) {
    disconnectFromEditor();
    m_editor = editor;

    if (m_editor) {
        m_modifiedConn = connect(m_editor, &ScintillaNext::notifyChange, this, &FunctionListDock::onEditorModified);
        m_lexerConn = connect(m_editor, &ScintillaNext::lexerChanged, this, &FunctionListDock::refreshFunctions);
        refreshFunctions();
    } else {
        ui->treeWidget->clear();
        ui->labelStatus->setText(tr("No active editor"));
    }
}

void FunctionListDock::disconnectFromEditor() {
    if (m_modifiedConn) {
        disconnect(m_modifiedConn);
    }
    if (m_lexerConn) {
        disconnect(m_lexerConn);
    }
    m_editor = nullptr;
}

void FunctionListDock::onEditorModified() {
    // Debounce reparse while typing
    if (isVisible() && m_editor) {
        m_debounceTimer->start();
    }
}

void FunctionListDock::refreshFunctions() {
    if (!isVisible() || !m_editor)
        return;

    m_symbols = FunctionParser::parse(m_editor);
    populateTree();
}

void FunctionListDock::onSearchTextChanged(const QString &text) {
    m_filterText = text.trimmed();
    populateTree();
}

void FunctionListDock::onSortAlphaToggled(bool checked) {
    m_sortAlpha = checked;
    populateTree();
}

void FunctionListDock::onGroupScopeToggled(bool checked) {
    m_groupByScope = checked;
    populateTree();
}

void FunctionListDock::onItemActivated(QTreeWidgetItem *item, int /*column*/) {
    if (!item || !m_editor)
        return;

    int line = item->data(0, Qt::UserRole).toInt();
    int pos = item->data(0, Qt::UserRole + 1).toInt();

    if (line > 0) {
        m_editor->gotoLine(line - 1);
        m_editor->scrollVertical(line - 1, 0);
        m_editor->setSelection(pos, pos);
        m_editor->setFocus();
    }
}

void FunctionListDock::populateTree() {
    ui->treeWidget->clear();
    if (!m_editor) {
        ui->labelStatus->setText(tr("No active editor"));
        return;
    }

    QVector<FunctionSymbol> filtered;
    for (const auto &sym : m_symbols) {
        if (!m_filterText.isEmpty()) {
            if (!sym.name.contains(m_filterText, Qt::CaseInsensitive) &&
                !sym.parentScope.contains(m_filterText, Qt::CaseInsensitive)) {
                continue;
            }
        }
        filtered.push_back(sym);
    }

    if (filtered.isEmpty()) {
        ui->labelStatus->setText(m_filterText.isEmpty() ? tr("No symbols detected") : tr("No matching symbols"));
        return;
    }

    if (m_sortAlpha) {
        std::sort(filtered.begin(), filtered.end(), [](const FunctionSymbol &a, const FunctionSymbol &b) {
            return a.name.compare(b.name, Qt::CaseInsensitive) < 0;
        });
    }

    QIcon funcIcon = style()->standardIcon(QStyle::SP_FileIcon);
    QIcon classIcon = style()->standardIcon(QStyle::SP_DirIcon);

    if (m_groupByScope) {
        QMap<QString, QTreeWidgetItem *> scopeItems;
        QVector<FunctionSymbol> standalone;

        for (const auto &sym : filtered) {
            if (sym.type == FunctionSymbol::Class || sym.type == FunctionSymbol::Struct) {
                QTreeWidgetItem *classItem = new QTreeWidgetItem(ui->treeWidget);
                classItem->setText(0, sym.name);
                classItem->setText(1, QString::number(sym.line));
                classItem->setIcon(0, classIcon);
                classItem->setData(0, Qt::UserRole, sym.line);
                classItem->setData(0, Qt::UserRole + 1, sym.position);
                scopeItems[sym.name] = classItem;
            } else if (!sym.parentScope.isEmpty()) {
                // Method or scoped function
                QTreeWidgetItem *parentItem = scopeItems.value(sym.parentScope, nullptr);
                if (!parentItem) {
                    parentItem = new QTreeWidgetItem(ui->treeWidget);
                    parentItem->setText(0, sym.parentScope);
                    parentItem->setIcon(0, classIcon);
                    scopeItems[sym.parentScope] = parentItem;
                }
                QTreeWidgetItem *childItem = new QTreeWidgetItem(parentItem);
                QString displayName =
                    sym.signature.isEmpty() ? sym.name : QStringLiteral("%1%2").arg(sym.name, sym.signature);
                childItem->setText(0, displayName);
                childItem->setText(1, QString::number(sym.line));
                childItem->setIcon(0, funcIcon);
                childItem->setData(0, Qt::UserRole, sym.line);
                childItem->setData(0, Qt::UserRole + 1, sym.position);
            } else {
                standalone.push_back(sym);
            }
        }

        // Add standalone / global functions
        for (const auto &sym : standalone) {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            QString displayName =
                sym.signature.isEmpty() ? sym.name : QStringLiteral("%1%2").arg(sym.name, sym.signature);
            item->setText(0, displayName);
            item->setText(1, QString::number(sym.line));
            item->setIcon(0, sym.type == FunctionSymbol::Heading ? classIcon : funcIcon);
            item->setData(0, Qt::UserRole, sym.line);
            item->setData(0, Qt::UserRole + 1, sym.position);
        }

        ui->treeWidget->expandAll();
    } else {
        // Flat list
        for (const auto &sym : filtered) {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            QString prefix = sym.parentScope.isEmpty() ? QString() : QStringLiteral("%1::").arg(sym.parentScope);
            QString displayName = QStringLiteral("%1%2%3").arg(prefix, sym.name, sym.signature);
            item->setText(0, displayName);
            item->setText(1, QString::number(sym.line));
            item->setIcon(0, (sym.type == FunctionSymbol::Class || sym.type == FunctionSymbol::Struct ||
                              sym.type == FunctionSymbol::Heading)
                                 ? classIcon
                                 : funcIcon);
            item->setData(0, Qt::UserRole, sym.line);
            item->setData(0, Qt::UserRole + 1, sym.position);
        }
    }

    ui->labelStatus->setText(tr("%1 symbol(s) found").arg(filtered.size()));
}
