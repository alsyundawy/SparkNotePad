#pragma once
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

#ifndef TRANSFERBUNDLEDIALOG_H
#define TRANSFERBUNDLEDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QTabWidget>

class TransferBundleDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransferBundleDialog(QWidget *parent = nullptr);

private slots:
    void onExportClicked();
    void onBrowseImportBundle();
    void onInspectBundle();
    void onImportClicked();
    void onPasswordChanged(const QString &pass);

private:
    void setupUi();
    void updateTheme();

    QTabWidget *m_tabWidget{nullptr};

    // Export Tab
    QCheckBox *m_chkPreferences{nullptr};
    QCheckBox *m_chkThemes{nullptr};
    QCheckBox *m_chkMacros{nullptr};
    QCheckBox *m_chkSessions{nullptr};
    QCheckBox *m_chkScripts{nullptr};
    QLineEdit *m_exportPassEdit{nullptr};
    QLabel *m_exportStrengthLabel{nullptr};
    QProgressBar *m_exportStrengthBar{nullptr};
    QPushButton *m_btnExport{nullptr};

    // Import Tab
    QLineEdit *m_importPathEdit{nullptr};
    QPushButton *m_btnBrowseImport{nullptr};
    QLineEdit *m_importPassEdit{nullptr};
    QPushButton *m_btnInspect{nullptr};
    QListWidget *m_importFileList{nullptr};
    QLabel *m_importStatusLabel{nullptr};
    QPushButton *m_btnImport{nullptr};
};

#endif  // TRANSFERBUNDLEDIALOG_H
