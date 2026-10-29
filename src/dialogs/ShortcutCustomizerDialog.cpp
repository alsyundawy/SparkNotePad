/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ShortcutCustomizerDialog.h"
#include "../features/ThemeManager.h"
#include "../features/ThemePaletteData.h"
#include "ApplicationSettings.h"
#include "SparkNotePadApplication.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QKeySequenceEdit>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>

ShortcutCustomizerWidget::ShortcutCustomizerWidget(QMainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow) {
    setupUi();
    collectActions();
    populateTree();
    applyThemeStyles();
}

QString ShortcutCustomizerWidget::cleanActionText(const QString &text) {
    QString res = text;
    res.remove(QLatin1Char('&'));
    res.remove(QStringLiteral("..."));
    return res.trimmed();
}

void ShortcutCustomizerWidget::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Filter bar
    auto *filterLayout = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    auto *filterLabel = new QLabel(tr("Filter:"), this);
    filterLayout->addWidget(filterLabel);

    m_filterEdit = new QLineEdit(this);
    m_filterEdit->setPlaceholderText(
        tr("Search by action name, category, or shortcut (e.g. 'Diff', 'Ctrl+N', 'PHP')..."));
    m_filterEdit->setClearButtonEnabled(true);
    connect(m_filterEdit, &QLineEdit::textChanged, this, &ShortcutCustomizerWidget::onFilterTextChanged);
    filterLayout->addWidget(m_filterEdit);
    mainLayout->addLayout(filterLayout);

    // Tree of actions
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabels({tr("Category / Action"), tr("Current Shortcut"), tr("Status")});
    m_treeWidget->setColumnWidth(0, 420);
    m_treeWidget->setColumnWidth(1, 180);
    m_treeWidget->header()->setStretchLastSection(true);
    m_treeWidget->setAlternatingRowColors(true);
    m_treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_treeWidget, &QTreeWidget::itemSelectionChanged, this, &ShortcutCustomizerWidget::onItemSelectionChanged);
    mainLayout->addWidget(m_treeWidget, 1);

    // Shortcut Editor Box
    auto *editGroup = new QGroupBox(tr("Modify Shortcut"), this);
    auto *editLayout = new QVBoxLayout(editGroup);
    editLayout->setSpacing(8);

    m_selectedActionLabel = new QLabel(tr("Select an action above to configure its shortcut."), editGroup);
    m_selectedActionLabel->setStyleSheet(QStringLiteral("font-weight: bold;"));
    editLayout->addWidget(m_selectedActionLabel);

    auto *inputRow = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    auto *shortcutLabel = new QLabel(tr("Key Sequence:"), editGroup);
    inputRow->addWidget(shortcutLabel);

    m_seqEdit = new QKeySequenceEdit(editGroup);
    m_seqEdit->setEnabled(false);
    connect(m_seqEdit, &QKeySequenceEdit::keySequenceChanged, this, &ShortcutCustomizerWidget::updateConflictWarning);
    inputRow->addWidget(m_seqEdit, 1);

    m_btnAssign = new QPushButton(tr("Assign"), editGroup);
    m_btnAssign->setEnabled(false);
    connect(m_btnAssign, &QPushButton::clicked, this, &ShortcutCustomizerWidget::onAssignClicked);
    inputRow->addWidget(m_btnAssign);

    m_btnClear = new QPushButton(tr("Clear"), editGroup);
    m_btnClear->setEnabled(false);
    connect(m_btnClear, &QPushButton::clicked, this, &ShortcutCustomizerWidget::onClearClicked);
    inputRow->addWidget(m_btnClear);

    m_btnResetSelected = new QPushButton(tr("Reset"), editGroup);
    m_btnResetSelected->setEnabled(false);
    m_btnResetSelected->setToolTip(tr("Reset selected action to default shortcut"));
    connect(m_btnResetSelected, &QPushButton::clicked, this, &ShortcutCustomizerWidget::onResetSelectedClicked);
    inputRow->addWidget(m_btnResetSelected);

    editLayout->addLayout(inputRow);

    m_conflictLabel = new QLabel(editGroup);
    m_conflictLabel->setStyleSheet(QStringLiteral("color: #e57373; font-style: italic;"));
    m_conflictLabel->setVisible(false);
    editLayout->addWidget(m_conflictLabel);

    mainLayout->addWidget(editGroup);

    // Bottom Action Buttons
    auto *bottomRow = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    m_btnResetAll = new QPushButton(tr("Reset All to Defaults"), this);
    connect(m_btnResetAll, &QPushButton::clicked, this, &ShortcutCustomizerWidget::onResetAllClicked);
    bottomRow->addWidget(m_btnResetAll);

    bottomRow->addStretch();

    m_btnApply = new QPushButton(tr("Apply Shortcuts"), this);
    m_btnApply->setIcon(QIcon(QStringLiteral(":/icons/checkmark.svg")));
    connect(m_btnApply, &QPushButton::clicked, this, &ShortcutCustomizerWidget::onApplyClicked);
    bottomRow->addWidget(m_btnApply);

    mainLayout->addLayout(bottomRow);
}

void ShortcutCustomizerWidget::registerAction(QAction *action, const QString &category) {
    if (!action || action->isSeparator())
        return;

    const QString cleanText = cleanActionText(action->text());
    if (cleanText.isEmpty())
        return;

    QString objName = action->objectName();
    if (objName.isEmpty()) {
        objName = QStringLiteral("action_%1_%2")
                      .arg(category.toLower().remove(QLatin1Char(' ')), cleanText.toLower().remove(QLatin1Char(' ')));
        action->setObjectName(objName);
    }

    if (!m_entries.contains(objName)) {
        ActionShortcutEntry entry;
        entry.action = action;
        entry.objectName = objName;
        entry.category = category;
        entry.description = cleanText;
        entry.currentShortcut = action->shortcut();
        entry.defaultShortcut = action->property("defaultShortcut").value<QKeySequence>();
        if (entry.defaultShortcut.isEmpty() && !action->shortcut().isEmpty()) {
            action->setProperty("defaultShortcut", action->shortcut());
            entry.defaultShortcut = action->shortcut();
        }
        entry.isModified = false;
        m_entries.insert(objName, entry);
    }
}

void ShortcutCustomizerWidget::traverseMenu(const QMenu *menu, const QString &catPrefix) {
    if (!menu)
        return;

    const QString menuTitle = cleanActionText(menu->title());
    const QString fullCat = catPrefix.isEmpty() ? menuTitle : QStringLiteral("%1 > %2").arg(catPrefix, menuTitle);

    for (QAction *action : menu->actions()) {
        if (action && !action->isSeparator()) {
            if (action->menu()) {
                traverseMenu(action->menu(), fullCat);
            } else {
                registerAction(action, fullCat);
            }
        }
    }
}

void ShortcutCustomizerWidget::collectActions() {
    if (!m_mainWindow) {
        const auto topLevels = QApplication::topLevelWidgets();
        for (QWidget *w : topLevels) {
            if (auto *mw = qobject_cast<QMainWindow *>(w)) {
                m_mainWindow = mw;
                break;
            }
        }
    }
    if (!m_mainWindow)
        return;

    m_entries.clear();

    if (const auto *menuBar = m_mainWindow->menuBar()) {
        for (const QAction *action : menuBar->actions()) {
            if (action && action->menu()) {
                traverseMenu(action->menu(), QString());
            }
        }
    }

    // Also include top-level main window actions (such as Command Palette, Side-by-Side Diff)
    for (QAction *action : m_mainWindow->actions()) {
        registerAction(action, tr("Global / Window"));
    }
}

void ShortcutCustomizerWidget::refreshActions() {
    collectActions();
    populateTree();
}

void ShortcutCustomizerWidget::populateTree() {
    m_treeWidget->clear();

    // Group entries by category
    QMap<QString, QTreeWidgetItem *> catItems;

    for (const auto &entry : m_entries) {
        QTreeWidgetItem *parentItem = catItems.value(entry.category, nullptr);
        if (!parentItem) {
            parentItem = new QTreeWidgetItem(m_treeWidget);  // NOSONAR(cpp:S5025)
            parentItem->setText(0, entry.category);
            parentItem->setExpanded(true);
            QFont f = parentItem->font(0);
            f.setBold(true);
            parentItem->setFont(0, f);
            catItems.insert(entry.category, parentItem);
        }

        auto *item = new QTreeWidgetItem(parentItem);  // NOSONAR(cpp:S5025)
        item->setText(0, entry.description);
        item->setText(1, entry.currentShortcut.toString(QKeySequence::NativeText));
        item->setText(2, entry.isModified ? tr("Modified") : tr("Default"));
        item->setData(0, Qt::UserRole, entry.objectName);
    }
}

void ShortcutCustomizerWidget::applyThemeStyles() {
    const auto *tm = ThemeManager::instance();
    if (!tm)
        return;

    const bool isDark = tm->isDarkActive();
    const ThemePalette *pal = tm->currentPalette();

    const QString defaultBg = isDark ? QStringLiteral("#1e293b") : QStringLiteral("#ffffff");
    const QString defaultWinBg = isDark ? QStringLiteral("#0f172a") : QStringLiteral("#f8fafc");
    const QString defaultTextCol = isDark ? QStringLiteral("#f8fafc") : QStringLiteral("#0f172a");
    const QString defaultBorderCol = isDark ? QStringLiteral("#334155") : QStringLiteral("#cbd5e1");
    const QString defaultAccentCol = isDark ? QStringLiteral("#38bdf8") : QStringLiteral("#2563eb");

    const QString cardBg = pal ? QString::fromLatin1(pal->card) : defaultBg;
    const QString winBg = pal ? QString::fromLatin1(pal->win) : defaultWinBg;
    const QString textCol = pal ? QString::fromLatin1(pal->txtPri) : defaultTextCol;
    const QString borderCol = pal ? QString::fromLatin1(pal->border) : defaultBorderCol;
    const QString accentCol = pal ? QString::fromLatin1(pal->blue) : defaultAccentCol;

    setStyleSheet(
        QStringLiteral("QWidget { background-color: transparent; color: %1; }"
                       "QTreeWidget { background-color: %2; color: %1; border: 1px solid %3; border-radius: 6px; }"
                       "QHeaderView::section { background-color: %4; color: %1; padding: 6px; font-weight: bold; "
                       "border: none; border-bottom: 1px solid %3; }"
                       "QTreeWidget::item:selected { background-color: %5; color: #ffffff; }"
                       "QTreeWidget::item:hover:!selected { background-color: rgba(56, 189, 248, 0.15); }"
                       "QGroupBox { background-color: %2; border: 1px solid %3; border-radius: 6px; margin-top: 10px; "
                       "padding: 12px; font-weight: 600; color: %1; }"
                       "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 6px; "
                       "color: %1; font-weight: bold; }")
            .arg(textCol, cardBg, borderCol, winBg, accentCol));
}

void ShortcutCustomizerWidget::onFilterTextChanged(const QString &text) const {
    const QString query = text.trimmed();

    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *catItem = m_treeWidget->topLevelItem(i);
        bool hasVisibleChild = false;

        for (int j = 0; j < catItem->childCount(); ++j) {
            QTreeWidgetItem *child = catItem->child(j);
            bool match = query.isEmpty() || child->text(0).contains(query, Qt::CaseInsensitive) ||
                         child->text(1).contains(query, Qt::CaseInsensitive) ||
                         catItem->text(0).contains(query, Qt::CaseInsensitive);

            child->setHidden(!match);
            if (match) {
                hasVisibleChild = true;
            }
        }

        catItem->setHidden(!hasVisibleChild && !query.isEmpty());
        if (hasVisibleChild && !query.isEmpty()) {
            catItem->setExpanded(true);
        }
    }
}

void ShortcutCustomizerWidget::onItemSelectionChanged() {
    QList<QTreeWidgetItem *> selected = m_treeWidget->selectedItems();
    if (selected.isEmpty()) {
        m_selectedActionLabel->setText(tr("Select an action above to configure its shortcut."));
        m_seqEdit->setEnabled(false);
        m_seqEdit->setKeySequence(QKeySequence());
        m_btnAssign->setEnabled(false);
        m_btnClear->setEnabled(false);
        m_btnResetSelected->setEnabled(false);
        m_conflictLabel->setVisible(false);
        return;
    }

    const QTreeWidgetItem *item = selected.first();
    const QString objName = item->data(0, Qt::UserRole).toString();
    if (!m_entries.contains(objName)) {
        // Parent category item selected
        m_selectedActionLabel->setText(tr("Category: %1").arg(item->text(0)));
        m_seqEdit->setEnabled(false);
        m_seqEdit->setKeySequence(QKeySequence());
        m_btnAssign->setEnabled(false);
        m_btnClear->setEnabled(false);
        m_btnResetSelected->setEnabled(false);
        m_conflictLabel->setVisible(false);
        return;
    }

    const ActionShortcutEntry &entry = m_entries[objName];
    m_selectedActionLabel->setText(tr("Action: <b>%1</b> (%2)").arg(entry.description, entry.category));
    m_seqEdit->setEnabled(true);
    m_seqEdit->setKeySequence(entry.currentShortcut);
    m_btnAssign->setEnabled(true);
    m_btnClear->setEnabled(!entry.currentShortcut.isEmpty());
    m_btnResetSelected->setEnabled(entry.currentShortcut != entry.defaultShortcut);
    updateConflictWarning();
}

void ShortcutCustomizerWidget::updateConflictWarning() {
    QList<QTreeWidgetItem *> selected = m_treeWidget->selectedItems();
    if (selected.isEmpty()) {
        m_conflictLabel->setVisible(false);
        return;
    }

    const QString curObj = selected.first()->data(0, Qt::UserRole).toString();
    const QKeySequence candidate = m_seqEdit->keySequence();

    if (candidate.isEmpty()) {
        m_conflictLabel->setVisible(false);
        return;
    }

    for (auto it = m_entries.constBegin(); it != m_entries.constEnd(); ++it) {
        if (it.key() != curObj && it.value().currentShortcut == candidate) {
            m_conflictLabel->setText(tr("⚠ Conflict: Shortcut is already used by '%1' (%2)")
                                         .arg(it.value().description, it.value().category));
            m_conflictLabel->setVisible(true);
            return;
        }
    }

    m_conflictLabel->setVisible(false);
}

void ShortcutCustomizerWidget::onAssignClicked() {
    QList<QTreeWidgetItem *> selected = m_treeWidget->selectedItems();
    if (selected.isEmpty())
        return;

    QTreeWidgetItem *item = selected.first();
    const QString objName = item->data(0, Qt::UserRole).toString();
    if (!m_entries.contains(objName))
        return;

    ActionShortcutEntry &entry = m_entries[objName];
    const QKeySequence newSeq = m_seqEdit->keySequence();
    entry.currentShortcut = newSeq;
    entry.isModified = (entry.currentShortcut != entry.defaultShortcut);

    item->setText(1, entry.currentShortcut.toString(QKeySequence::NativeText));
    item->setText(2, entry.isModified ? tr("Modified") : tr("Default"));

    m_btnClear->setEnabled(!entry.currentShortcut.isEmpty());
    m_btnResetSelected->setEnabled(entry.isModified);
    updateConflictWarning();
}

void ShortcutCustomizerWidget::onClearClicked() {
    m_seqEdit->setKeySequence(QKeySequence());
    onAssignClicked();
}

void ShortcutCustomizerWidget::onResetSelectedClicked() {
    QList<QTreeWidgetItem *> selected = m_treeWidget->selectedItems();
    if (selected.isEmpty())
        return;

    QTreeWidgetItem *item = selected.first();
    const QString objName = item->data(0, Qt::UserRole).toString();
    if (!m_entries.contains(objName))
        return;

    ActionShortcutEntry &entry = m_entries[objName];
    entry.currentShortcut = entry.defaultShortcut;
    entry.isModified = false;
    m_seqEdit->setKeySequence(entry.defaultShortcut);

    item->setText(1, entry.currentShortcut.toString(QKeySequence::NativeText));
    item->setText(2, tr("Default"));

    m_btnClear->setEnabled(!entry.currentShortcut.isEmpty());
    m_btnResetSelected->setEnabled(false);
    updateConflictWarning();
}

void ShortcutCustomizerWidget::onResetAllClicked() {
    if (QMessageBox::question(this, tr("Reset All Shortcuts"),
                              tr("Are you sure you want to restore all shortcuts to their default settings?")) !=
        QMessageBox::Yes) {
        return;
    }

    for (auto it = m_entries.begin(); it != m_entries.end(); ++it) {
        it.value().currentShortcut = it.value().defaultShortcut;
        it.value().isModified = false;
    }

    populateTree();
    onItemSelectionChanged();
}

void ShortcutCustomizerWidget::saveShortcuts() const {
    const auto *app = qobject_cast<SparkNotePadApplication *>(qApp);
    ApplicationSettings *settings = app ? app->getSettings() : nullptr;
    if (!settings)
        return;

    settings->beginGroup(QStringLiteral("Shortcuts"));

    for (const auto &entry : m_entries) {
        if (!entry.action) {
            // Skip invalid action
        } else {
            // Apply immediately to the live QAction
            entry.action->setShortcut(entry.currentShortcut);

            // Normalize setting key name (strip 'action' prefix if present for legacy compatibility)
            QString keyName = entry.objectName;
            if (keyName.startsWith(QStringLiteral("action"))) {
                keyName.remove(0, 6);
            }

            if (entry.currentShortcut != entry.defaultShortcut) {
                // Store modified shortcut string list
                settings->setValue(keyName, QStringList{entry.currentShortcut.toString(QKeySequence::PortableText)});
            } else {
                // Restore default -> remove override
                settings->remove(keyName);
                settings->remove(entry.objectName);
            }
        }
    }

    settings->endGroup();
    settings->sync();
}

void ShortcutCustomizerWidget::onApplyClicked() {
    saveShortcuts();
    emit shortcutsChanged();
}

ShortcutCustomizerDialog::ShortcutCustomizerDialog(QMainWindow *mainWindow, QWidget *parent)
    : QDialog(parent ? parent : mainWindow) {
    setWindowTitle(tr("Shortcut Mapper / Customize Shortcuts"));
    resize(780, 560);
    setMinimumSize(600, 420);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(14, 14, 14, 14);

    m_customizerWidget = new ShortcutCustomizerWidget(mainWindow, this);
    mainLayout->addWidget(m_customizerWidget, 1);

    auto *bottomRow = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    bottomRow->addStretch();

    auto *btnOk = new QPushButton(tr("OK"), this);
    btnOk->setDefault(true);
    connect(btnOk, &QPushButton::clicked, this, [this]() {
        if (m_customizerWidget) {
            m_customizerWidget->saveShortcuts();
        }
        accept();
    });
    bottomRow->addWidget(btnOk);

    auto *btnCancel = new QPushButton(tr("Cancel"), this);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    bottomRow->addWidget(btnCancel);

    mainLayout->addLayout(bottomRow);
}
