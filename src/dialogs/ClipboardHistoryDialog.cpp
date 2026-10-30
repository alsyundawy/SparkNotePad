/*
 * This file is part of SparkNotePad.
 *
 * Implements Clipboard History & Monitoring Dialog.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ClipboardHistoryDialog.h"
#include "ui_ClipboardHistoryDialog.h"
#include "../ScintillaNext.h"

#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMessageBox>

ClipboardHistoryDialog::ClipboardHistoryDialog(QWidget *parent, ScintillaNext *currentEditor)
    : QDialog(parent), ui(new Ui::ClipboardHistoryDialog), m_editor(currentEditor) {
    ui->setupUi(this);

    ClipboardMonitor *mon = ClipboardMonitor::instance();
    if (m_editor) {
        mon->setTargetEditor(m_editor);
    }

    ui->checkAutoAppend->setChecked(mon->appendToDocument());
    ui->checkAddTimestamp->setChecked(mon->addTimestamp());
    ui->checkIgnoreInternal->setChecked(mon->ignoreInternalCopy());

    ui->tableHistory->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    connect(ui->btnToggleMonitor, &QPushButton::clicked, this, &ClipboardHistoryDialog::onToggleMonitoring);
    connect(ui->tableHistory, &QTableWidget::itemSelectionChanged, this, &ClipboardHistoryDialog::onSelectionChanged);
    connect(ui->tableHistory, &QTableWidget::cellDoubleClicked, this, [this](int, int) { onInsertClicked(); });

    connect(ui->btnInsert, &QPushButton::clicked, this, &ClipboardHistoryDialog::onInsertClicked);
    connect(ui->btnCopy, &QPushButton::clicked, this, &ClipboardHistoryDialog::onCopyClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &ClipboardHistoryDialog::onClearClicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::accept);

    connect(ui->checkAutoAppend, &QCheckBox::toggled, this, &ClipboardHistoryDialog::onOptionsChanged);
    connect(ui->checkAddTimestamp, &QCheckBox::toggled, this, &ClipboardHistoryDialog::onOptionsChanged);
    connect(ui->checkIgnoreInternal, &QCheckBox::toggled, this, &ClipboardHistoryDialog::onOptionsChanged);

    connect(mon, &ClipboardMonitor::monitoringStateChanged, this, &ClipboardHistoryDialog::onMonitoringStateChanged);
    connect(mon, &ClipboardMonitor::clipCaptured, this, &ClipboardHistoryDialog::onClipCaptured);

    refreshHistoryTable();
    updateStatusLabel();
}

ClipboardHistoryDialog::~ClipboardHistoryDialog() {
    delete ui;
}

void ClipboardHistoryDialog::onToggleMonitoring() {
    ClipboardMonitor *mon = ClipboardMonitor::instance();
    mon->toggleMonitoring(m_editor);
}

void ClipboardHistoryDialog::onMonitoringStateChanged(bool active) {
    Q_UNUSED(active);
    updateStatusLabel();
}

void ClipboardHistoryDialog::updateStatusLabel() {
    const ClipboardMonitor *mon = ClipboardMonitor::instance();
    if (mon->isMonitoring()) {
        ui->labelStatus->setText(tr("<b>Clipboard Monitoring:</b> <font color='#5cb85c'>ACTIVE</font> (%1 captured)")
                                     .arg(mon->capturedCount()));
        ui->btnToggleMonitor->setText(tr("Stop Monitoring"));
    } else {
        ui->labelStatus->setText(tr("<b>Clipboard Monitoring:</b> <font color='#888888'>INACTIVE</font> (%1 captured)")
                                     .arg(mon->capturedCount()));
        ui->btnToggleMonitor->setText(tr("Start Monitoring"));
    }
}

void ClipboardHistoryDialog::onClipCaptured(const ClipboardMonitor::CapturedClip &clip, int totalCount) {
    Q_UNUSED(clip);
    Q_UNUSED(totalCount);
    refreshHistoryTable();
    updateStatusLabel();
}

void ClipboardHistoryDialog::refreshHistoryTable() {
    const ClipboardMonitor *mon = ClipboardMonitor::instance();
    const auto &hist = mon->history();

    ui->tableHistory->setRowCount(hist.size());

    for (int i = 0; i < hist.size(); ++i) {
        const auto &clip = hist.at(i);
        QString preview = clip.text;
        preview.replace(QLatin1Char('\n'), QLatin1String(" ")).replace(QLatin1Char('\r'), QString());
        if (preview.length() > 80) {
            preview = preview.left(80) + QStringLiteral("...");
        }

        ui->tableHistory->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->tableHistory->setItem(i, 1, new QTableWidgetItem(clip.timestamp.toString(QStringLiteral("HH:mm:ss"))));
        ui->tableHistory->setItem(i, 2, new QTableWidgetItem(preview));
        ui->tableHistory->setItem(i, 3, new QTableWidgetItem(QString::number(clip.charCount)));
        ui->tableHistory->setItem(i, 4, new QTableWidgetItem(QString::number(clip.lineCount)));
        ui->tableHistory->setItem(i, 5, new QTableWidgetItem(clip.internal ? tr("SparkNotePad") : tr("External")));
    }

    if (!hist.isEmpty()) {
        ui->tableHistory->selectRow(hist.size() - 1);
    }
}

void ClipboardHistoryDialog::onSelectionChanged() {
    int row = ui->tableHistory->currentRow();
    const ClipboardMonitor *mon = ClipboardMonitor::instance();
    const auto &hist = mon->history();

    if (row >= 0 && row < hist.size()) {
        ui->editPreview->setPlainText(hist.at(row).text);
        ui->btnInsert->setEnabled(true);
        ui->btnCopy->setEnabled(true);
    } else {
        ui->editPreview->clear();
        ui->btnInsert->setEnabled(false);
        ui->btnCopy->setEnabled(false);
    }
}

void ClipboardHistoryDialog::onInsertClicked() {
    int row = ui->tableHistory->currentRow();
    const ClipboardMonitor *mon = ClipboardMonitor::instance();
    const auto &hist = mon->history();

    if (row >= 0 && row < hist.size() && m_editor) {
        const QString &text = hist.at(row).text;
        const QByteArray utf8 = text.toUtf8();
        m_editor->insertText(m_editor->currentPos(), utf8.constData());
        m_editor->goToPos(m_editor->currentPos() + utf8.size());
        m_editor->grabFocus();
    }
}

void ClipboardHistoryDialog::onCopyClicked() {
    int row = ui->tableHistory->currentRow();
    const ClipboardMonitor *mon = ClipboardMonitor::instance();
    const auto &hist = mon->history();

    if (row >= 0 && row < hist.size()) {
        QApplication::clipboard()->setText(hist.at(row).text);
    }
}

void ClipboardHistoryDialog::onClearClicked() {
    ClipboardMonitor *mon = ClipboardMonitor::instance();
    mon->clearHistory();
    ui->editPreview->clear();
    refreshHistoryTable();
    updateStatusLabel();
}

void ClipboardHistoryDialog::onOptionsChanged() {
    ClipboardMonitor *mon = ClipboardMonitor::instance();
    mon->setAppendToDocument(ui->checkAutoAppend->isChecked());
    mon->setAddTimestamp(ui->checkAddTimestamp->isChecked());
    mon->setIgnoreInternalCopy(ui->checkIgnoreInternal->isChecked());
}
