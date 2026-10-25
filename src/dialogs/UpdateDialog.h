#pragma once
/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QString>
#include "../features/ReleaseManager.h"

class QTextBrowser;
class QLabel;
class QPushButton;

/**
 * @brief Elegant, modern, and theme-responsive update dialog for SparkNotePad.
 *
 * Provides polished dialog presentations for:
 *  - Update Available (version comparison pills, release metadata, changelog preview, action buttons)
 *  - Up to Date (emerald checkmark badge, version confirmation)
 *  - Check Failed (amber diagnostic badge, network error explanation, retry action)
 */
class UpdateDialog : public QDialog {
    Q_OBJECT

public:
    enum class DialogType { UpdateAvailable, UpToDate, CheckFailed };

    explicit UpdateDialog(QWidget *parent = nullptr, DialogType type = DialogType::UpdateAvailable);
    ~UpdateDialog() override = default;

    // Static Drop-in Helpers
    static void showUpdateAvailable(QWidget *parent, const QString &curVer, const ReleaseInfo &info);
    static void showUpToDate(QWidget *parent, const QString &curVer);
    static void showCheckFailed(QWidget *parent, const QString &errorMessage, QWidget *retryParent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupUpdateAvailableUi(const QString &curVer, const ReleaseInfo &info);
    void setupUpToDateUi(const QString &curVer);
    void setupCheckFailedUi(const QString &errorMessage, QWidget *retryParent);

    void applyBaseStyling(int width, int height);
    static QPixmap createBadgePixmap(DialogType type);
    static QString formatReleaseNotes(const QString &rawNotes, const QString &txtPri, const QString &txtSec,
                                      const QString &accent);
    static QString formatPublishDate(const QString &isoDate);

    DialogType m_type;
};

#endif  // UPDATEDIALOG_H
