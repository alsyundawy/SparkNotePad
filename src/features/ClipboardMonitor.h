#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Implements Clipboard Monitoring Mode.
 * Adapted from Notepad3 (rizonesoft/notepad3) clipboard monitoring architecture.
 * Captures system clipboard text changes and automatically appends them to the active document
 * or stores them in the session history.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef CLIPBOARDMONITOR_H
#define CLIPBOARDMONITOR_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QPointer>

class ScintillaNext;

class ClipboardMonitor : public QObject {
    Q_OBJECT

public:
    struct CapturedClip {
        QString text;
        QDateTime timestamp;
        int charCount = 0;
        int lineCount = 0;
        bool internal = false;
    };

    static ClipboardMonitor *instance();

    bool isMonitoring() const;
    void startMonitoring(ScintillaNext *editor = nullptr);
    void stopMonitoring();
    void toggleMonitoring(ScintillaNext *editor = nullptr);

    void setTargetEditor(ScintillaNext *editor);
    ScintillaNext *targetEditor() const;

    bool appendToDocument() const;
    void setAppendToDocument(bool append);

    bool addTimestamp() const;
    void setAddTimestamp(bool add);

    bool addSeparator() const;
    void setAddSeparator(bool add);

    bool ignoreInternalCopy() const;
    void setIgnoreInternalCopy(bool ignore);

    int capturedCount() const;
    const QList<CapturedClip> &history() const;
    void clearHistory();

signals:
    void monitoringStateChanged(bool active);
    void clipCaptured(const CapturedClip &clip, int totalCount);

private slots:
    void onClipboardDataChanged();

private:
    explicit ClipboardMonitor(QObject *parent = nullptr);
    ~ClipboardMonitor() override = default;

    void appendClipToEditor(ScintillaNext *editor, const CapturedClip &clip);

    bool m_isMonitoring = false;
    bool m_appendToDocument = true;
    bool m_addTimestamp = false;
    bool m_addSeparator = true;
    bool m_ignoreInternalCopy = true;

    QString m_lastCapturedText;
    int m_capturedCount = 0;
    QList<CapturedClip> m_history;
    QPointer<ScintillaNext> m_targetEditor;
};

#endif  // CLIPBOARDMONITOR_H
