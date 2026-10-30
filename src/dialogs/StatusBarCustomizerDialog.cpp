/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad3 status bar customization architecture.
 * Sources: rizonesoft/Notepad3 (src/Notepad3.ini, Statusbar Settings)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "StatusBarCustomizerDialog.h"
#include "EditorInfoStatusBar.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

StatusBarCustomizerDialog::StatusBarCustomizerDialog(EditorInfoStatusBar *statusBar, QWidget *parent)
    : QDialog(parent), m_statusBar(statusBar) {
    setWindowTitle(tr("Customize Status Bar Fields"));
    setMinimumWidth(380);
    setupUi();
}

void StatusBarCustomizerDialog::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);

    auto *descLabel = new QLabel(tr("Select which fields to display on the status bar:"), this);
    mainLayout->addWidget(descLabel);

    auto *group = new QGroupBox(tr("Status Bar Fields"), this);
    auto *groupLayout = new QVBoxLayout(group);
    groupLayout->setSpacing(8);

    struct FieldMeta {
        EditorInfoStatusBar::Field id;
        QString name;
        QString description;
    };

    QList<FieldMeta> fields = {
        {EditorInfoStatusBar::FieldLanguage, tr("Document Language / Syntax Scheme"),
         tr("Current language syntax highlighter (e.g. C++, HTML, Markdown)")},
        {EditorInfoStatusBar::FieldDocSize, tr("Document Size (Length / Lines)"),
         tr("Total character length and line count")},
        {EditorInfoStatusBar::FieldDocPos, tr("Cursor Position & Selection (Ln / Col / Sel)"),
         tr("Current line, column, and selection range")},
        {EditorInfoStatusBar::FieldUnicodeChar, tr("Unicode Code Point (U+XXXX)"),
         tr("Hexadecimal Unicode code point of character at cursor")},
        {EditorInfoStatusBar::FieldOccurrences, tr("Marked Occurrences Count (Occ: N)"),
         tr("Count of marked occurrences in document")},
        {EditorInfoStatusBar::FieldEol, tr("End-of-Line Format (CRLF / LF)"),
         tr("Line break mode (Windows, Unix, Mac)")},
        {EditorInfoStatusBar::FieldEncoding, tr("File Encoding (UTF-8 / ANSI)"), tr("Active encoding and BOM status")},
        {EditorInfoStatusBar::FieldOverType, tr("Insert / Overtype Mode (INS / OVR)"),
         tr("Keyboard typing insert or replace mode")}};

    for (const auto &fm : fields) {
        auto *cb = new QCheckBox(fm.name, group);
        cb->setToolTip(fm.description);
        cb->setChecked(m_statusBar ? m_statusBar->isFieldVisible(fm.id) : true);
        connect(cb, &QCheckBox::toggled, this, [this, id = fm.id](bool checked) {
            if (m_statusBar) {
                m_statusBar->setFieldVisible(id, checked);
            }
        });
        groupLayout->addWidget(cb);
        m_checkBoxes[fm.id] = cb;
    }

    mainLayout->addWidget(group);

    // Quick action buttons
    auto *quickLayout = new QHBoxLayout();
    auto *btnSelectAll = new QPushButton(tr("Select &All"), this);
    auto *btnClearAll = new QPushButton(tr("&Deselect All"), this);
    auto *btnReset = new QPushButton(tr("&Reset Defaults"), this);

    quickLayout->addWidget(btnSelectAll);
    quickLayout->addWidget(btnClearAll);
    quickLayout->addWidget(btnReset);
    mainLayout->addLayout(quickLayout);

    connect(btnSelectAll, &QPushButton::clicked, this, &StatusBarCustomizerDialog::onSelectAll);
    connect(btnClearAll, &QPushButton::clicked, this, &StatusBarCustomizerDialog::onClearAll);
    connect(btnReset, &QPushButton::clicked, this, &StatusBarCustomizerDialog::onResetDefaults);

    // Bottom close button
    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    auto *btnClose = new QPushButton(tr("&Close"), this);
    btnClose->setDefault(true);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
    bottomLayout->addWidget(btnClose);
    mainLayout->addLayout(bottomLayout);
}

void StatusBarCustomizerDialog::onSelectAll() {
    for (auto it = m_checkBoxes.begin(); it != m_checkBoxes.end(); ++it) {
        it.value()->setChecked(true);
    }
}

void StatusBarCustomizerDialog::onClearAll() {
    for (auto it = m_checkBoxes.begin(); it != m_checkBoxes.end(); ++it) {
        it.value()->setChecked(false);
    }
}

void StatusBarCustomizerDialog::onResetDefaults() {
    if (m_statusBar) {
        m_statusBar->resetToDefaults();
        for (auto it = m_checkBoxes.begin(); it != m_checkBoxes.end(); ++it) {
            it.value()->setChecked(m_statusBar->isFieldVisible(static_cast<EditorInfoStatusBar::Field>(it.key())));
        }
    }
}
