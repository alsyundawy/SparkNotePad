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

#include <QDialog>
#include "features/DocumentExportManager.h"

class QComboBox;
class QCheckBox;
class QLineEdit;
class QDialogButtonBox;

class ExportOptionsDialog : public QDialog {
    Q_OBJECT

public:
    enum class TargetFormat { Html, Pdf };

    explicit ExportOptionsDialog(TargetFormat format, const QString &defaultTitle = QString(),
                                 QWidget *parent = nullptr);
    ~ExportOptionsDialog() override = default;

    [[nodiscard]] DocumentExportManager::ExportOptions options() const;

private:
    void setupUi();

    TargetFormat m_format;
    QString m_initialTitle;

    QLineEdit *editTitle = nullptr;
    QComboBox *comboPageSize = nullptr;
    QComboBox *comboOrientation = nullptr;
    QComboBox *comboMargins = nullptr;
    QCheckBox *chkIncludeCss = nullptr;
    QCheckBox *chkDarkTheme = nullptr;
    QCheckBox *chkUsePandoc = nullptr;
    QDialogButtonBox *buttonBox = nullptr;
};
