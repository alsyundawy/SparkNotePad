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

#include "PerformanceDiagnosticsDialog.h"
#include "ui_PerformanceDiagnosticsDialog.h"
#include "PerformanceProfiler.h"
#include "ScintillaNext.h"

#include <QClipboard>
#include <QGuiApplication>

PerformanceDiagnosticsDialog::PerformanceDiagnosticsDialog(QWidget *parent, ScintillaNext *currentEditor)
    : QDialog(parent),
      ui(new Ui::PerformanceDiagnosticsDialog),
      m_editor(currentEditor),
      m_refreshTimer(new QTimer(this)) {
    ui->setupUi(this);

    setupUiData();
    refreshMetrics();

    connect(ui->btnRunBenchmark, &QPushButton::clicked, this, &PerformanceDiagnosticsDialog::runBenchmark);
    connect(ui->chkHighPerf, &QCheckBox::toggled, this, &PerformanceDiagnosticsDialog::toggleHighPerformanceMode);
    connect(ui->btnCopyReport, &QPushButton::clicked, this, &PerformanceDiagnosticsDialog::copyReport);
    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::accept);

    connect(m_refreshTimer, &QTimer::timeout, this, &PerformanceDiagnosticsDialog::refreshMetrics);
    m_refreshTimer->start(1000);  // 1-second live memory polling
}

PerformanceDiagnosticsDialog::~PerformanceDiagnosticsDialog() {
    delete ui;
}

void PerformanceDiagnosticsDialog::setupUiData() {
    const auto sys = PerformanceProfiler::getSystemInfo();
    ui->lblOsVal->setText(sys.osName);
    ui->lblCpuVal->setText(sys.cpuModel);
    ui->lblArchVal->setText(QStringLiteral("%1 (%2 logical cores)%3")
                                .arg(sys.cpuArchitecture)
                                .arg(sys.logicalCores)
                                .arg(sys.isAppleSilicon ? QStringLiteral(" - Apple Silicon Native") : QString()));

    if (m_editor) {
        ui->chkHighPerf->setChecked(PerformanceProfiler::isHighPerformanceMode(m_editor));
    } else {
        ui->chkHighPerf->setEnabled(false);
    }
}

void PerformanceDiagnosticsDialog::refreshMetrics() {
    const auto mem = PerformanceProfiler::getMemoryMetrics();
    ui->lblRssVal->setText(QStringLiteral("%1 MB").arg(QString::number(mem.rssMegabytes, 'f', 2)));
    ui->lblVmsVal->setText(QStringLiteral("%1 MB").arg(QString::number(mem.virtualMegabytes, 'f', 2)));

    if (m_editor) {
        const auto doc = PerformanceProfiler::getDocumentMetrics(m_editor);
        ui->lblDocVal->setText(QStringLiteral("%1 (%2 lines, %3 MB)")
                                   .arg(doc.fileName)
                                   .arg(doc.lineCount)
                                   .arg(QString::number(doc.byteSize / (1024.0 * 1024.0), 'f', 2)));
    } else {
        ui->lblDocVal->setText(tr("None"));
    }
}

void PerformanceDiagnosticsDialog::runBenchmark() {
    ui->lblBenchmarkStatus->setText(tr("Benchmarking Scintilla throughput (50,000 lines)..."));
    ui->lblBenchmarkStatus->setStyleSheet(QStringLiteral("color: #1565c0; font-weight: bold;"));
    QGuiApplication::processEvents();

    const auto res = PerformanceProfiler::runScintillaInsertBenchmark(50000);

    ui->lblBenchmarkStatus->setText(QStringLiteral("✓ %1").arg(res.details));
    ui->lblBenchmarkStatus->setStyleSheet(QStringLiteral("color: #2e7d32; font-weight: bold;"));

    refreshMetrics();
}

void PerformanceDiagnosticsDialog::toggleHighPerformanceMode(bool checked) {
    if (m_editor) {
        PerformanceProfiler::setHighPerformanceMode(m_editor, checked);
        refreshMetrics();
    }
}

void PerformanceDiagnosticsDialog::copyReport() {
    const QString report = PerformanceProfiler::generateDiagnosticReport(m_editor);
    QGuiApplication::clipboard()->setText(report);
    ui->lblBenchmarkStatus->setText(tr("Report copied to clipboard!"));
    ui->lblBenchmarkStatus->setStyleSheet(QStringLiteral("color: #1565c0; font-weight: bold;"));
}
