#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Implements Clipboard History & Monitoring Dialog.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef CLIPBOARDHISTORYDIALOG_H
#define CLIPBOARDHISTORYDIALOG_H

#include <QDialog>
#include "../features/ClipboardMonitor.h"

namespace Ui {
class ClipboardHistoryDialog;
}

class ScintillaNext;

class ClipboardHistoryDialog : public QDialog {
    Q_OBJECT

public:
    explicit ClipboardHistoryDialog(QWidget *parent = nullptr, ScintillaNext *currentEditor = nullptr);
    ~ClipboardHistoryDialog() override;

private slots:
    void onToggleMonitoring();
    void onMonitoringStateChanged(bool active);
    void onClipCaptured(const ClipboardMonitor::CapturedClip &clip, int totalCount);
    void onSelectionChanged();
    void onInsertClicked();
    void onCopyClicked();
    void onClearClicked();
    void onOptionsChanged();

private:
    void refreshHistoryTable();
    void updateStatusLabel();

    Ui::ClipboardHistoryDialog *ui;
    ScintillaNext *m_editor;
};

#endif  // CLIPBOARDHISTORYDIALOG_H
