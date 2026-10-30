#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Performance Diagnostics & Benchmark Dialog.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef PERFORMANCEDIAGNOSTICSDIALOG_H
#define PERFORMANCEDIAGNOSTICSDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class PerformanceDiagnosticsDialog;
}

class ScintillaNext;

class PerformanceDiagnosticsDialog : public QDialog {
    Q_OBJECT

public:
    explicit PerformanceDiagnosticsDialog(QWidget *parent = nullptr, ScintillaNext *currentEditor = nullptr);
    ~PerformanceDiagnosticsDialog() override;

private slots:
    void refreshMetrics();
    void runBenchmark();
    void toggleHighPerformanceMode(bool checked);
    void copyReport();

private:
    void setupUiData();

    Ui::PerformanceDiagnosticsDialog *ui;
    ScintillaNext *m_editor{nullptr};
    QTimer *m_refreshTimer{nullptr};
};

#endif  // PERFORMANCEDIAGNOSTICSDIALOG_H
