/*
 * This file is part of SparkNotePad.
 *
 * Implements Clipboard Monitoring Mode.
 * Adapted from Notepad3 (rizonesoft/notepad3) clipboard monitoring architecture.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ClipboardMonitor.h"
#include "../ScintillaNext.h"

#include <QApplication>
#include <QClipboard>
#include <QDateTime>

ClipboardMonitor *ClipboardMonitor::instance() {
    static ClipboardMonitor s_instance;
    return &s_instance;
}

ClipboardMonitor::ClipboardMonitor(QObject *parent) : QObject(parent) {}

bool ClipboardMonitor::isMonitoring() const {
    return m_isMonitoring;
}

void ClipboardMonitor::startMonitoring(ScintillaNext *editor) {
    if (editor) {
        m_targetEditor = editor;
    }

    if (!m_isMonitoring) {
        m_isMonitoring = true;
        m_lastCapturedText = QApplication::clipboard()->text();
        connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &ClipboardMonitor::onClipboardDataChanged);
        emit monitoringStateChanged(true);
    }
}

void ClipboardMonitor::stopMonitoring() {
    if (m_isMonitoring) {
        m_isMonitoring = false;
        disconnect(QApplication::clipboard(), &QClipboard::dataChanged, this,
                   &ClipboardMonitor::onClipboardDataChanged);
        emit monitoringStateChanged(false);
    }
}

void ClipboardMonitor::toggleMonitoring(ScintillaNext *editor) {
    if (m_isMonitoring) {
        stopMonitoring();
    } else {
        startMonitoring(editor);
    }
}

void ClipboardMonitor::setTargetEditor(ScintillaNext *editor) {
    m_targetEditor = editor;
}

ScintillaNext *ClipboardMonitor::targetEditor() const {
    return m_targetEditor;
}

bool ClipboardMonitor::appendToDocument() const {
    return m_appendToDocument;
}

void ClipboardMonitor::setAppendToDocument(bool append) {
    m_appendToDocument = append;
}

bool ClipboardMonitor::addTimestamp() const {
    return m_addTimestamp;
}

void ClipboardMonitor::setAddTimestamp(bool add) {
    m_addTimestamp = add;
}

bool ClipboardMonitor::addSeparator() const {
    return m_addSeparator;
}

void ClipboardMonitor::setAddSeparator(bool add) {
    m_addSeparator = add;
}

bool ClipboardMonitor::ignoreInternalCopy() const {
    return m_ignoreInternalCopy;
}

void ClipboardMonitor::setIgnoreInternalCopy(bool ignore) {
    m_ignoreInternalCopy = ignore;
}

int ClipboardMonitor::capturedCount() const {
    return m_capturedCount;
}

const QList<ClipboardMonitor::CapturedClip> &ClipboardMonitor::history() const {
    return m_history;
}

void ClipboardMonitor::clearHistory() {
    m_history.clear();
    m_capturedCount = 0;
}

void ClipboardMonitor::onClipboardDataChanged() {
    if (!m_isMonitoring)
        return;

    const QString text = QApplication::clipboard()->text();
    if (text.isEmpty())
        return;

    if (text == m_lastCapturedText)
        return;
    m_lastCapturedText = text;

    bool isInternal = (QApplication::activeWindow() != nullptr);
    if (m_ignoreInternalCopy && isInternal) {
        return;
    }

    CapturedClip clip;
    clip.text = text;
    clip.timestamp = QDateTime::currentDateTime();
    clip.charCount = text.length();
    clip.lineCount = text.count(QLatin1Char('\n')) + 1;
    clip.internal = isInternal;

    m_capturedCount++;
    m_history.append(clip);

    if (m_appendToDocument && m_targetEditor) {
        appendClipToEditor(m_targetEditor, clip);
    }

    emit clipCaptured(clip, m_capturedCount);
}

void ClipboardMonitor::appendClipToEditor(ScintillaNext *editor, const CapturedClip &clip) {
    if (!editor)
        return;

    int docLen = editor->length();
    QString toAppend;

    if (docLen > 0) {
        char lastChar = editor->charAt(docLen - 1);
        if (lastChar != '\n' && lastChar != '\r') {
            toAppend += QLatin1Char('\n');
        }
        if (m_addSeparator) {
            toAppend += QLatin1Char('\n');
        }
    }

    if (m_addTimestamp) {
        toAppend += QStringLiteral("[%1] ").arg(clip.timestamp.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
    }

    toAppend += clip.text;
    if (!toAppend.endsWith(QLatin1Char('\n'))) {
        toAppend += QLatin1Char('\n');
    }

    const QByteArray utf8 = toAppend.toUtf8();
    editor->appendText(utf8.size(), utf8.constData());
    editor->scrollRange(editor->length(), editor->length());
}
