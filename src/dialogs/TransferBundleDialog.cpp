/*
 * This file is part of SparkNotePad.
 *
 * Implements Workspace Transfer & Portable Bundle Dialog (.sparkbundle).
 *
 * Copyright 2025-2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "TransferBundleDialog.h"
#include "ModernMessageBox.h"
#include "SparkTransferManager.h"
#include "../features/ThemeManager.h"
#include "utils/CryptoManager.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

TransferBundleDialog::TransferBundleDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("System Transfer — Workspace Bundle (.sparkbundle)"));
    resize(580, 480);
    setupUi();
    updateTheme();
}

void TransferBundleDialog::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);
    m_tabWidget = new QTabWidget(this);

    // ==========================================
    // TAB 1: EXPORT WORKSPACE BUNDLE
    // ==========================================
    auto *exportTab = new QWidget(this);
    auto *exportLayout = new QVBoxLayout(exportTab);

    auto *exportIntro =
        new QLabel(tr("<b>Export Workspace Configuration Bundle</b><br>"
                      "Package your preferences, custom themes, macros, sessions, and scripts into a portable, "
                      "cross-platform <code>.sparkbundle</code> file for macOS, FreeBSD, and Linux."),
                   exportTab);
    exportIntro->setWordWrap(true);
    exportLayout->addWidget(exportIntro);

    auto *itemsGroup = new QGroupBox(tr("Select Configuration Components to Export"), exportTab);
    auto *itemsLayout = new QVBoxLayout(itemsGroup);
    m_chkPreferences = new QCheckBox(tr("Preferences & App Settings (INI/JSON)"), itemsGroup);
    m_chkThemes = new QCheckBox(tr("Custom Visual Themes & Style Sheets (QSS)"), itemsGroup);
    m_chkMacros = new QCheckBox(tr("Recorded Keyboard Macros"), itemsGroup);
    m_chkSessions = new QCheckBox(tr("Workspace Sessions & Tab Layouts"), itemsGroup);
    m_chkScripts = new QCheckBox(tr("User Lua & Python Scripts"), itemsGroup);

    m_chkPreferences->setChecked(true);
    m_chkThemes->setChecked(true);
    m_chkMacros->setChecked(true);
    m_chkSessions->setChecked(true);
    m_chkScripts->setChecked(true);

    itemsLayout->addWidget(m_chkPreferences);
    itemsLayout->addWidget(m_chkThemes);
    itemsLayout->addWidget(m_chkMacros);
    itemsLayout->addWidget(m_chkSessions);
    itemsLayout->addWidget(m_chkScripts);
    exportLayout->addWidget(itemsGroup);

    auto *securityGroup = new QGroupBox(tr("Optional Encryption (AES-256-CBC + HMAC-SHA256)"), exportTab);
    auto *secLayout = new QVBoxLayout(securityGroup);
    m_exportPassEdit = new QLineEdit(securityGroup);
    m_exportPassEdit->setEchoMode(QLineEdit::Password);
    m_exportPassEdit->setPlaceholderText(tr("Enter encryption passphrase (leave blank for unencrypted bundle)..."));
    secLayout->addWidget(m_exportPassEdit);

    auto *strengthLayout = new QHBoxLayout();
    m_exportStrengthBar = new QProgressBar(securityGroup);
    m_exportStrengthBar->setRange(0, 100);
    m_exportStrengthBar->setValue(0);
    m_exportStrengthBar->setTextVisible(false);
    m_exportStrengthBar->setFixedHeight(8);

    m_exportStrengthLabel = new QLabel(tr("No passphrase (unencrypted)"), securityGroup);
    strengthLayout->addWidget(m_exportStrengthBar);
    strengthLayout->addWidget(m_exportStrengthLabel);
    secLayout->addLayout(strengthLayout);
    exportLayout->addWidget(securityGroup);

    connect(m_exportPassEdit, &QLineEdit::textChanged, this, &TransferBundleDialog::onPasswordChanged);

    auto *exportBtnLayout = new QHBoxLayout();
    exportBtnLayout->addStretch();
    m_btnExport = new QPushButton(tr("Export Bundle..."), exportTab);
    m_btnExport->setDefault(true);
    connect(m_btnExport, &QPushButton::clicked, this, &TransferBundleDialog::onExportClicked);
    exportBtnLayout->addWidget(m_btnExport);
    exportLayout->addLayout(exportBtnLayout);

    m_tabWidget->addTab(exportTab, tr("Export Bundle"));

    // ==========================================
    // TAB 2: IMPORT WORKSPACE BUNDLE
    // ==========================================
    auto *importTab = new QWidget(this);
    auto *importLayout = new QVBoxLayout(importTab);

    auto *importIntro =
        new QLabel(tr("<b>Import & Migrate Workspace Configuration Bundle</b><br>"
                      "Restore preferences, custom themes, macros, and scripts. Automatically normalizes path "
                      "conventions, verifies in-memory HMAC digests, and protects against Zip-Slip traversal attacks."),
                   importTab);
    importIntro->setWordWrap(true);
    importLayout->addWidget(importIntro);

    auto *fileSelLayout = new QHBoxLayout();
    m_importPathEdit = new QLineEdit(importTab);
    m_importPathEdit->setPlaceholderText(tr("Select .sparkbundle file to import..."));
    m_btnBrowseImport = new QPushButton(tr("Browse..."), importTab);
    connect(m_btnBrowseImport, &QPushButton::clicked, this, &TransferBundleDialog::onBrowseImportBundle);
    fileSelLayout->addWidget(m_importPathEdit);
    fileSelLayout->addWidget(m_btnBrowseImport);
    importLayout->addLayout(fileSelLayout);

    auto *importSecGroup = new QGroupBox(tr("Passphrase (if encrypted)"), importTab);
    auto *importSecLayout = new QHBoxLayout(importSecGroup);
    m_importPassEdit = new QLineEdit(importSecGroup);
    m_importPassEdit->setEchoMode(QLineEdit::Password);
    m_importPassEdit->setPlaceholderText(tr("Enter bundle passphrase..."));
    m_btnInspect = new QPushButton(tr("Inspect Bundle"), importSecGroup);
    connect(m_btnInspect, &QPushButton::clicked, this, &TransferBundleDialog::onInspectBundle);
    importSecLayout->addWidget(m_importPassEdit);
    importSecLayout->addWidget(m_btnInspect);
    importLayout->addWidget(importSecGroup);

    auto *previewGroup = new QGroupBox(tr("Bundle Contents Preview"), importTab);
    auto *prevLayout = new QVBoxLayout(previewGroup);
    m_importFileList = new QListWidget(previewGroup);
    prevLayout->addWidget(m_importFileList);
    m_importStatusLabel = new QLabel(tr("Select a bundle to preview files."), previewGroup);
    prevLayout->addWidget(m_importStatusLabel);
    importLayout->addWidget(previewGroup);

    auto *importBtnLayout = new QHBoxLayout();
    importBtnLayout->addStretch();
    m_btnImport = new QPushButton(tr("Import & Apply Configuration"), importTab);
    connect(m_btnImport, &QPushButton::clicked, this, &TransferBundleDialog::onImportClicked);
    importBtnLayout->addWidget(m_btnImport);
    importLayout->addLayout(importBtnLayout);

    m_tabWidget->addTab(importTab, tr("Import Bundle"));

    mainLayout->addWidget(m_tabWidget);
}

void TransferBundleDialog::updateTheme() {
    const bool isDark = ThemeManager::instance()->isDarkActive();
    const QString bg = isDark ? QStringLiteral("#1e1e1e") : QStringLiteral("#f8f9fa");
    const QString fg = isDark ? QStringLiteral("#e0e0e0") : QStringLiteral("#212529");
    const QString border = isDark ? QStringLiteral("#333333") : QStringLiteral("#ced4da");

    setStyleSheet(QStringLiteral("QDialog { background-color: %1; color: %2; }"
                                 "QGroupBox { font-weight: bold; border: 1px solid %3; border-radius: 6px; margin-top: "
                                 "10px; padding-top: 10px; }"
                                 "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }"
                                 "QLineEdit, QListWidget { background-color: %1; color: %2; border: 1px solid %3; "
                                 "border-radius: 4px; padding: 4px; }"
                                 "QPushButton { min-width: 90px; padding: 5px 12px; border-radius: 4px; }")
                      .arg(bg, fg, border));
}

void TransferBundleDialog::onPasswordChanged(const QString &pass) {
    if (pass.isEmpty()) {
        m_exportStrengthBar->setValue(0);
        m_exportStrengthLabel->setText(tr("No passphrase (unencrypted)"));
        return;
    }

    const CryptoManager::StrengthResult strength = CryptoManager::evaluatePasswordStrength(pass);
    m_exportStrengthBar->setValue(strength.score);
    m_exportStrengthLabel->setText(strength.label);
}

void TransferBundleDialog::onExportClicked() {
    SparkTransferManager::ExportOptions options;
    options.includePreferences = m_chkPreferences->isChecked();
    options.includeThemes = m_chkThemes->isChecked();
    options.includeMacros = m_chkMacros->isChecked();
    options.includeSessions = m_chkSessions->isChecked();
    options.includeScripts = m_chkScripts->isChecked();
    options.passphrase = m_exportPassEdit->text();

    const QString fileName = QFileDialog::getSaveFileName(
        this, tr("Export SparkNotePad Workspace Bundle"),
        QStringLiteral("SparkNotePad_Workspace_%1.sparkbundle").arg(QDateTime::currentDateTime().toString("yyyyMMdd")),
        tr("SparkNotePad Bundle (*.sparkbundle);;All Files (*)"));

    if (fileName.isEmpty()) {
        return;
    }

    const SparkTransferManager::TransferResult res = SparkTransferManager::exportBundle(fileName, options);
    if (res.success) {
        ModernMessageBox::information(
            this, tr("Export Succeeded"),
            tr("Workspace bundle successfully created!\n\nFiles exported: %1\nTotal size: %2 bytes\nLocation: %3")
                .arg(res.filesCount)
                .arg(res.totalBytes)
                .arg(fileName));
        accept();
    } else {
        ModernMessageBox::critical(this, tr("Export Failed"), res.message);
    }
}

void TransferBundleDialog::onBrowseImportBundle() {
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Open SparkNotePad Workspace Bundle"), QString(),
                                                          tr("SparkNotePad Bundle (*.sparkbundle);;All Files (*)"));

    if (!fileName.isEmpty()) {
        m_importPathEdit->setText(fileName);
        onInspectBundle();
    }
}

void TransferBundleDialog::onInspectBundle() {
    const QString path = m_importPathEdit->text().trimmed();
    if (path.isEmpty()) {
        return;
    }

    m_importFileList->clear();
    QStringList files;
    const SparkTransferManager::TransferResult res =
        SparkTransferManager::inspectBundle(path, m_importPassEdit->text(), &files);

    if (res.success) {
        m_importFileList->addItems(files);
        m_importStatusLabel->setText(
            tr("Found %1 files (%2 bytes). SHA-256 verified.").arg(res.filesCount).arg(res.totalBytes));
    } else {
        m_importStatusLabel->setText(tr("Inspection error: %1").arg(res.message));
    }
}

void TransferBundleDialog::onImportClicked() {
    const QString path = m_importPathEdit->text().trimmed();
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        ModernMessageBox::warning(this, tr("File Not Found"), tr("Please select a valid .sparkbundle file."));
        return;
    }

    const SparkTransferManager::TransferResult res = SparkTransferManager::importBundle(path, m_importPassEdit->text());

    if (res.success) {
        ModernMessageBox::information(
            this, tr("Import Complete"),
            tr("Workspace bundle applied successfully!\n\n%1\n\nPlease restart SparkNotePad to apply all preferences.")
                .arg(res.message));
        accept();
    } else {
        ModernMessageBox::critical(this, tr("Import Failed"), res.message);
    }
}
