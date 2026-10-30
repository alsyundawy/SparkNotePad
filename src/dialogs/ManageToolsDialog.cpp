/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad++ Run menu custom commands management.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ManageToolsDialog.h"
#include "ui_ManageToolsDialog.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QTreeWidgetItem>

namespace {

class ToolEditDialog : public QDialog {
public:
    explicit ToolEditDialog(QWidget *parent, const ExternalTool &tool = {}) : QDialog(parent) {
        setWindowTitle(tool.name.isEmpty() ? tr("Add External Tool") : tr("Edit External Tool"));
        resize(480, 220);

        QFormLayout *layout = new QFormLayout(this);

        editName = new QLineEdit(tool.name, this);
        editCommand = new QLineEdit(tool.command, this);
        editCommand->setPlaceholderText(tr("e.g. python3 \"$(FULL_CURRENT_PATH)\""));
        editWorkDir = new QLineEdit(tool.workingDirectory, this);
        editWorkDir->setPlaceholderText(tr("Optional, defaults to file directory"));

        chkTerminal = new QCheckBox(tr("Run in Terminal window"), this);
        chkTerminal->setChecked(tool.runInTerminal);

        chkSave = new QCheckBox(tr("Save current document before executing"), this);
        chkSave->setChecked(tool.saveBeforeRun);

        layout->addRow(tr("Name:"), editName);
        layout->addRow(tr("Command:"), editCommand);
        layout->addRow(tr("Working Dir:"), editWorkDir);
        layout->addRow(QString(), chkTerminal);
        layout->addRow(QString(), chkSave);

        QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        layout->addRow(btnBox);

        connect(btnBox, &QDialogButtonBox::accepted, this, [this]() {
            if (editName->text().trimmed().isEmpty()) {
                QMessageBox::warning(this, tr("Invalid Input"), tr("Please provide a name for the tool."));
                return;
            }
            if (editCommand->text().trimmed().isEmpty()) {
                QMessageBox::warning(this, tr("Invalid Input"), tr("Please provide a command for the tool."));
                return;
            }
            accept();
        });
        connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    ExternalTool getTool(const QString &origId = QString()) const {
        ExternalTool t;
        t.id = origId;
        t.name = editName->text().trimmed();
        t.command = editCommand->text().trimmed();
        t.workingDirectory = editWorkDir->text().trimmed();
        t.runInTerminal = chkTerminal->isChecked();
        t.saveBeforeRun = chkSave->isChecked();
        return t;
    }

private:
    QLineEdit *editName;
    QLineEdit *editCommand;
    QLineEdit *editWorkDir;
    QCheckBox *chkTerminal;
    QCheckBox *chkSave;
};

}  // namespace

ManageToolsDialog::ManageToolsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ManageToolsDialog) {
    ui->setupUi(this);

    ui->treeTools->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeTools->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeTools->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    connect(ui->btnAdd, &QPushButton::clicked, this, &ManageToolsDialog::onAddClicked);
    connect(ui->btnEdit, &QPushButton::clicked, this, &ManageToolsDialog::onEditClicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &ManageToolsDialog::onDeleteClicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::accept);

    connect(ui->treeTools, &QTreeWidget::itemDoubleClicked, this,
            [this](QTreeWidgetItem * /*item*/, int /*col*/) { onEditClicked(); });

    populateList();
}

ManageToolsDialog::~ManageToolsDialog() {
    delete ui;
}

void ManageToolsDialog::populateList() {
    ui->treeTools->clear();
    m_tools = ExternalToolManager::loadCustomTools();

    for (const auto &tool : m_tools) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeTools);
        item->setText(0, tool.name);
        item->setText(1, tool.command);
        item->setText(2, tool.runInTerminal ? tr("Yes") : tr("No"));
        item->setData(0, Qt::UserRole, tool.id);
    }
}

void ManageToolsDialog::onAddClicked() {
    ToolEditDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        ExternalTool newTool = dlg.getTool();
        ExternalToolManager::addCustomTool(newTool);
        populateList();
    }
}

void ManageToolsDialog::onEditClicked() {
    QTreeWidgetItem *cur = ui->treeTools->currentItem();
    if (!cur)
        return;

    int idx = ui->treeTools->indexOfTopLevelItem(cur);
    if (idx >= 0 && idx < m_tools.size()) {
        editTool(idx);
    }
}

void ManageToolsDialog::editTool(int index) {
    const ExternalTool orig = m_tools.at(index);
    ToolEditDialog dlg(this, orig);
    if (dlg.exec() == QDialog::Accepted) {
        ExternalTool updated = dlg.getTool(orig.id);
        m_tools[index] = updated;
        ExternalToolManager::saveCustomTools(m_tools);
        populateList();
    }
}

void ManageToolsDialog::onDeleteClicked() {
    QTreeWidgetItem *cur = ui->treeTools->currentItem();
    if (!cur)
        return;

    int idx = ui->treeTools->indexOfTopLevelItem(cur);
    if (idx < 0 || idx >= m_tools.size())
        return;

    const auto &tool = m_tools.at(idx);
    auto res =
        QMessageBox::question(this, tr("Delete Tool"), tr("Are you sure you want to delete tool '%1'?").arg(tool.name),
                              QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes) {
        ExternalToolManager::deleteCustomTool(tool.id);
        populateList();
    }
}
