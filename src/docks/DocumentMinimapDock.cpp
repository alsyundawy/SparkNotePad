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

#include "DocumentMinimapDock.h"
#include <QFontDatabase>
#include <QLabel>
#include <QMouseEvent>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <algorithm>
#include <functional>

#ifndef UNIT_TESTING
#include "ScintillaNext.h"
#endif

namespace {
class ClickableMinimapEdit : public QPlainTextEdit {
public:
    using LineSelectedCallback = std::function<void(double ratio)>;

    explicit ClickableMinimapEdit(LineSelectedCallback cb, QWidget *parent = nullptr)
        : QPlainTextEdit(parent), m_callback(std::move(cb)) {}

protected:
    void mousePressEvent(QMouseEvent *e) override {
        QPlainTextEdit::mousePressEvent(e);
        notifyRatio(e->position().y());
    }

    void mouseMoveEvent(QMouseEvent *e) override {
        QPlainTextEdit::mouseMoveEvent(e);
        if (e->buttons() & Qt::LeftButton) {
            notifyRatio(e->position().y());
        }
    }

private:
    void notifyRatio(double y) {
        if (!m_callback || height() <= 0) {
            return;
        }
        const double ratio = std::clamp(y / static_cast<double>(height()), 0.0, 1.0);
        m_callback(ratio);
    }

    LineSelectedCallback m_callback;
};
}  // namespace

DocumentMinimapDock::DocumentMinimapDock(QWidget *parent) : QDockWidget(tr("Document Minimap"), parent) {
    setObjectName(QStringLiteral("DocumentMinimapDock"));
    setupUi();
}

void DocumentMinimapDock::setupUi() {
    auto *const container = new QWidget(this);
    auto *const stacked = new QStackedLayout(container);
    stacked->setContentsMargins(0, 0, 0, 0);

    m_minimapEdit = new ClickableMinimapEdit(
        [this](double ratio) {
#ifndef UNIT_TESTING
            if (m_currentEditor) {
                if (const auto totalLines = static_cast<int>(m_currentEditor->lineCount()); totalLines > 0) {
                    const auto targetLine = static_cast<int>(ratio * static_cast<double>(totalLines));
                    m_currentEditor->gotoLine(targetLine);
                    m_currentEditor->scrollCaret();
                }
            }
#else
            (void)ratio;
#endif
        },
        container);

    m_minimapEdit->setReadOnly(true);
    m_minimapEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_minimapEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_minimapEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

    QFont miniFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    miniFont.setPointSize(2);
    m_minimapEdit->setFont(miniFont);
    m_minimapEdit->setStyleSheet(
        QStringLiteral("QPlainTextEdit {"
                       "  background-color: #1e1e1e;"
                       "  color: #888888;"
                       "  border: none;"
                       "}"));

    m_lblDisabledNotice = new QLabel(tr("Minimap disabled for large document (> 50 MB)"), container);
    m_lblDisabledNotice->setAlignment(Qt::AlignCenter);
    m_lblDisabledNotice->setWordWrap(true);
    m_lblDisabledNotice->setStyleSheet(QStringLiteral("color: #888888; padding: 12px;"));

    stacked->addWidget(m_minimapEdit);
    stacked->addWidget(m_lblDisabledNotice);
    stacked->setCurrentWidget(m_minimapEdit);

    setWidget(container);
}

void DocumentMinimapDock::syncWithEditor(ScintillaNext *editor) {
#ifdef UNIT_TESTING
    (void)editor;
    m_active = true;
#else
    m_currentEditor = editor;
    if (!editor) {
        m_active = false;
        m_minimapEdit->clear();
        return;
    }

    if (const qint64 docLength = editor->length(); docLength > m_largeFileLimitBytes) {
        m_active = false;
        if (auto *const layout = qobject_cast<QStackedLayout *>(widget()->layout()); layout) {
            layout->setCurrentWidget(m_lblDisabledNotice);
        }
        return;
    }

    m_active = true;
    if (auto *const layout = qobject_cast<QStackedLayout *>(widget()->layout()); layout) {
        layout->setCurrentWidget(m_minimapEdit);
    }

    updateContent();

    connect(editor, &ScintillaNext::updateUi, this, &DocumentMinimapDock::updateViewport, Qt::UniqueConnection);
#endif
}

void DocumentMinimapDock::updateContent() {
#ifdef UNIT_TESTING
#else
    if (!m_active || !m_currentEditor) {
        return;
    }

    const QByteArray bytes = m_currentEditor->getText();
    m_minimapEdit->setPlainText(QString::fromUtf8(bytes));
    updateViewport();
#endif
}

void DocumentMinimapDock::updateViewport() {
#ifdef UNIT_TESTING
#else
    if (!m_active || !m_currentEditor) {
        return;
    }

    const auto firstVisibleLine = static_cast<int>(m_currentEditor->firstVisibleLine());
    const auto totalLines = static_cast<int>(m_currentEditor->lineCount());
    if (totalLines > 0 && m_minimapEdit->document()) {
        const double ratio = static_cast<double>(firstVisibleLine) / static_cast<double>(totalLines);
        const int maxScroll = m_minimapEdit->verticalScrollBar()->maximum();
        m_minimapEdit->verticalScrollBar()->setValue(static_cast<int>(ratio * static_cast<double>(maxScroll)));
    }
#endif
}

void DocumentMinimapDock::setLargeFileLimitBytes(qint64 limitBytes) {
    m_largeFileLimitBytes = limitBytes;
}

qint64 DocumentMinimapDock::largeFileLimitBytes() const {
    return m_largeFileLimitBytes;
}

bool DocumentMinimapDock::isMinimapActive() const {
    return m_active;
}
