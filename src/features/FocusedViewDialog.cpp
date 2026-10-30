/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad3 Focused View & line filtering architecture.
 * Sources: rizonesoft/Notepad3 (src/Edit.c, FocusedView)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "FocusedViewDialog.h"
#include "MainWindow.h"
#include "ScintillaNext.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>

FocusedViewDialog::FocusedViewDialog(FocusedView *focusedView, MainWindow *mainWindow, QWidget *parent)
    : QDialog(parent), m_focusedView(focusedView), m_mainWindow(mainWindow) {
    setWindowTitle(tr("Focused View — Filter Lines"));
    setMinimumWidth(420);
    setupUi();
}

void FocusedViewDialog::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // Query group
    auto *filterGroup = new QGroupBox(tr("Filter Criteria"), this);
    auto *filterForm = new QFormLayout(filterGroup);

    m_queryEdit = new QLineEdit(this);
    m_queryEdit->setPlaceholderText(tr("Text or regular expression to match..."));
    m_queryEdit->setClearButtonEnabled(true);
    filterForm->addRow(tr("Filter text:"), m_queryEdit);

    m_contextLines = new QSpinBox(this);
    m_contextLines->setRange(0, 20);
    m_contextLines->setValue(0);
    m_contextLines->setSuffix(tr(" lines"));
    filterForm->addRow(tr("Context lines:"), m_contextLines);

    mainLayout->addWidget(filterGroup);

    // Options group
    auto *optionsGroup = new QGroupBox(tr("Options"), this);
    auto *optionsLayout = new QGridLayout(optionsGroup);

    m_caseSensitive = new QCheckBox(tr("Match &case"), this);
    m_wholeWord = new QCheckBox(tr("&Whole word"), this);
    m_useRegex = new QCheckBox(tr("Regular e&xpression"), this);
    m_invertFilter = new QCheckBox(tr("&Invert filter (hide matching lines)"), this);
    m_bookmarksOnly = new QCheckBox(tr("Filter to &bookmarked lines only"), this);

    optionsLayout->addWidget(m_caseSensitive, 0, 0);
    optionsLayout->addWidget(m_wholeWord, 0, 1);
    optionsLayout->addWidget(m_useRegex, 1, 0);
    optionsLayout->addWidget(m_invertFilter, 1, 1);
    optionsLayout->addWidget(m_bookmarksOnly, 2, 0, 1, 2);

    mainLayout->addWidget(optionsGroup);

    // Status label
    m_statusLabel = new QLabel(tr("Ready to filter lines"), this);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #666; font-style: italic;"));
    mainLayout->addWidget(m_statusLabel);

    // Action buttons
    auto *btnLayout = new QHBoxLayout();
    m_applyBtn = new QPushButton(tr("&Apply Filter"), this);
    m_applyBtn->setDefault(true);
    m_clearBtn = new QPushButton(tr("&Clear / Show All"), this);
    m_closeBtn = new QPushButton(tr("&Close"), this);

    btnLayout->addWidget(m_applyBtn);
    btnLayout->addWidget(m_clearBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_closeBtn);

    mainLayout->addLayout(btnLayout);

    // Connections
    connect(m_applyBtn, &QPushButton::clicked, this, &FocusedViewDialog::onApplyClicked);
    connect(m_clearBtn, &QPushButton::clicked, this, &FocusedViewDialog::onClearClicked);
    connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::close);
    connect(m_queryEdit, &QLineEdit::returnPressed, this, &FocusedViewDialog::onApplyClicked);
    connect(m_bookmarksOnly, &QCheckBox::toggled, this, &FocusedViewDialog::onBookmarksToggled);
}

void FocusedViewDialog::onBookmarksToggled(bool checked) {
    m_queryEdit->setEnabled(!checked);
    m_caseSensitive->setEnabled(!checked);
    m_wholeWord->setEnabled(!checked);
    m_useRegex->setEnabled(!checked);
}

void FocusedViewDialog::prepareForEditor(ScintillaNext *editor) {
    if (!editor)
        return;

    if (m_focusedView->isFocused(editor)) {
        auto opts = m_focusedView->currentOptions(editor);
        m_queryEdit->setText(opts.query);
        m_caseSensitive->setChecked(opts.matchCase);
        m_wholeWord->setChecked(opts.wholeWord);
        m_useRegex->setChecked(opts.useRegex);
        m_invertFilter->setChecked(opts.invert);
        m_bookmarksOnly->setChecked(opts.bookmarksOnly);
        m_contextLines->setValue(opts.contextLines);
        m_statusLabel->setText(tr("Focused View is currently active"));
    } else {
        // Prepopulate with selection or word at cursor if present
        int selStart = editor->selectionStart();
        int selEnd = editor->selectionEnd();
        QString candidate;
        if (selStart != selEnd && (selEnd - selStart) <= 1024) {
            candidate = QString::fromUtf8(editor->get_text_range(selStart, selEnd)).trimmed();
        } else {
            int curPos = editor->currentPos();
            int wStart = editor->wordStartPosition(curPos, true);
            int wEnd = editor->wordEndPosition(curPos, true);
            if (wStart != wEnd) {
                candidate = QString::fromUtf8(editor->get_text_range(wStart, wEnd)).trimmed();
            }
        }
        if (!candidate.isEmpty()) {
            m_queryEdit->setText(candidate);
            m_queryEdit->selectAll();
        }
        m_statusLabel->setText(tr("Ready to filter lines"));
    }
    m_queryEdit->setFocus();
}

void FocusedViewDialog::onApplyClicked() {
    if (!m_mainWindow || !m_focusedView)
        return;
    ScintillaNext *editor = m_mainWindow->currentEditor();
    if (!editor) {
        m_statusLabel->setText(tr("No active editor"));
        return;
    }

    FocusedView::FilterOptions opts;
    opts.query = m_queryEdit->text();
    opts.matchCase = m_caseSensitive->isChecked();
    opts.wholeWord = m_wholeWord->isChecked();
    opts.useRegex = m_useRegex->isChecked();
    opts.invert = m_invertFilter->isChecked();
    opts.bookmarksOnly = m_bookmarksOnly->isChecked();
    opts.contextLines = m_contextLines->value();

    auto res = m_focusedView->applyFilter(editor, opts);
    m_statusLabel->setText(res.message);
    if (m_mainWindow->statusBar()) {
        m_mainWindow->statusBar()->showMessage(res.message, 4000);
    }
}

void FocusedViewDialog::onClearClicked() {
    if (!m_mainWindow || !m_focusedView)
        return;
    ScintillaNext *editor = m_mainWindow->currentEditor();
    if (!editor) {
        m_statusLabel->setText(tr("No active editor"));
        return;
    }

    auto res = m_focusedView->clearFilter(editor);
    m_statusLabel->setText(res.message);
    if (m_mainWindow->statusBar()) {
        m_mainWindow->statusBar()->showMessage(res.message, 4000);
    }
}
