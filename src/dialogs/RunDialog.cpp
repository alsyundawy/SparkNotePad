/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad++ Run dialog concept.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "RunDialog.h"
#include "ui_RunDialog.h"

#include "ExternalToolManager.h"
#include "ScintillaNext.h"
#include "MainWindow.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QLineEdit>

RunDialog::RunDialog(MainWindow *parent, ScintillaNext *editor)
    : QDialog(parent), ui(new Ui::RunDialog), m_window(parent), m_editor(editor) {
    ui->setupUi(this);

    // Setup history
    const QStringList history = ExternalToolManager::loadCommandHistory();
    ui->comboCommand->addItems(history);
    if (!history.isEmpty()) {
        ui->comboCommand->setCurrentIndex(0);
    } else {
        ui->comboCommand->setEditText(QStringLiteral("python3 \"$(FULL_CURRENT_PATH)\""));
    }

    setupInsertVarMenu();

    connect(ui->comboCommand, &QComboBox::editTextChanged, this, &RunDialog::onCommandTextChanged);
    connect(ui->btnBrowse, &QToolButton::clicked, this, &RunDialog::onBrowseClicked);
    connect(ui->btnRun, &QPushButton::clicked, this, &RunDialog::onRunClicked);
    connect(ui->btnSave, &QPushButton::clicked, this, &RunDialog::onSaveClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    updatePreview();
}

RunDialog::~RunDialog() {
    delete ui;
}

void RunDialog::setupInsertVarMenu() {
    QMenu *varMenu = new QMenu(ui->btnInsertVar);

    const struct VarEntry {
        const char *var;
        const char *desc;
    } entries[] = {{"$(FULL_CURRENT_PATH)", "Full native file path"},
                   {"$(CURRENT_DIRECTORY)", "Directory of current file"},
                   {"$(FILE_NAME)", "File name with extension"},
                   {"$(NAME_PART)", "Base name without extension"},
                   {"$(EXT_PART)", "Extension (suffix)"},
                   {"$(CURRENT_WORD)", "Selected text or word under cursor"},
                   {"$(CURRENT_LINE)", "Current line number (1-based)"},
                   {"$(CURRENT_COLUMN)", "Current column number (1-based)"},
                   {"$(APP_DIR)", "NoteSciPad application directory"}};

    for (const auto &e : entries) {
        QString varStr = QString::fromLatin1(e.var);
        QString descStr = QString::fromLatin1(e.desc);
        QAction *act = varMenu->addAction(QStringLiteral("%1 (%2)").arg(varStr, descStr));
        connect(act, &QAction::triggered, this, [this, varStr]() { onInsertVarTriggered(varStr); });
    }

    ui->btnInsertVar->setMenu(varMenu);
}

void RunDialog::onInsertVarTriggered(const QString &varName) {
    QLineEdit *lineEdit = ui->comboCommand->lineEdit();
    if (lineEdit) {
        lineEdit->insert(varName);
        updatePreview();
    }
}

void RunDialog::onCommandTextChanged(const QString & /*text*/) {
    updatePreview();
}

void RunDialog::updatePreview() {
    const QString cmd = ui->comboCommand->currentText();
    const QString expanded = ExternalToolManager::expandVariables(cmd, m_editor);
    ui->editPreview->setText(expanded);
}

void RunDialog::onBrowseClicked() {
    QString dir;
    if (m_editor && m_editor->isFile()) {
        dir = m_editor->getFileInfo().dir().absolutePath();
    }
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Select Program or Document"), dir);
    if (!filePath.isEmpty()) {
        ui->comboCommand->setEditText(QStringLiteral("\"%1\"").arg(filePath));
        updatePreview();
    }
}

void RunDialog::onRunClicked() {
    const QString cmd = ui->comboCommand->currentText().trimmed();
    if (cmd.isEmpty()) {
        QMessageBox::information(this, tr("Run"), tr("Please specify a program or command to run."));
        return;
    }

    ExternalToolManager::addCommandToHistory(cmd);

    if (ui->chkSaveBeforeRun->isChecked() && m_editor && !m_editor->isSavedToDisk()) {
        m_editor->save();
    }

    const QString expanded = ExternalToolManager::expandVariables(cmd, m_editor);
    QString workDir;
    if (m_editor && m_editor->isFile()) {
        workDir = m_editor->getFileInfo().dir().absolutePath();
    }

    QString errorOut;
    bool ok = ExternalToolManager::executeCommand(expanded, workDir, ui->chkRunInTerminal->isChecked(), &errorOut);
    if (!ok && !errorOut.isEmpty()) {
        QMessageBox::warning(this, tr("Run Error"), errorOut);
        return;
    }

    accept();
}

void RunDialog::onSaveClicked() {
    const QString cmd = ui->comboCommand->currentText().trimmed();
    if (cmd.isEmpty()) {
        QMessageBox::information(this, tr("Save Tool"), tr("Please specify a command before saving."));
        return;
    }

    bool ok = false;
    QString toolName = QInputDialog::getText(this, tr("Save as External Tool"), tr("Tool Name:"), QLineEdit::Normal,
                                             tr("My Custom Tool"), &ok);
    if (ok && !toolName.trimmed().isEmpty()) {
        ExternalTool tool;
        tool.name = toolName.trimmed();
        tool.command = cmd;
        tool.runInTerminal = ui->chkRunInTerminal->isChecked();
        tool.saveBeforeRun = ui->chkSaveBeforeRun->isChecked();
        ExternalToolManager::addCustomTool(tool);

        QMessageBox::information(this, tr("Tool Saved"),
                                 tr("Tool '%1' was successfully added to the Run menu.").arg(tool.name));
    }
}
