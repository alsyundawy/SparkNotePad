/*
 * This file is part of SparkNotePad.
 *
 * Enhanced Multi-Mode Go To Dialog (Visual Line:Column, Character Offset, Byte Offset)
 * Inspired by Notepad4 v26.10r6392 / Notepad3.
 *
 * Copyright 2025-2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "GotoDialog.h"
#include "ScintillaNext.h"
#include "../features/ThemeManager.h"

#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <algorithm>

using Scintilla::sptr_t;

GotoDialog::GotoDialog(ScintillaNext *editor, QWidget *parent) : QDialog(parent), m_editor(editor) {
    setWindowTitle(tr("Go To Line / Position"));
    resize(420, 260);

    if (m_editor) {
        m_maxLines = static_cast<int>(m_editor->lineCount());
        m_docLength = m_editor->length();
        const auto curPos = static_cast<int>(m_editor->currentPos());
        m_targetLine = static_cast<int>(m_editor->lineFromPosition(curPos)) + 1;
        m_targetCol = static_cast<int>(m_editor->column(curPos)) + 1;
        m_targetPosition = curPos;
    }

    setupUi();
    updateTheme();

    if (m_lineEdit) {
        m_lineEdit->setText(QStringLiteral("%1:%2").arg(m_targetLine).arg(m_targetCol));
        m_lineEdit->selectAll();
        m_lineEdit->setFocus();
    }
}

void GotoDialog::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);

    auto *modeGroup = new QGroupBox(tr("Navigation Mode"), this);
    auto *modeLayout = new QHBoxLayout(modeGroup);
    m_rbVisualLineCol = new QRadioButton(tr("Line : Column"), modeGroup);
    m_rbCharOffset = new QRadioButton(tr("Character Offset"), modeGroup);
    m_rbByteOffset = new QRadioButton(tr("Byte Offset"), modeGroup);

    m_rbVisualLineCol->setChecked(true);
    auto *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(m_rbVisualLineCol);
    btnGroup->addButton(m_rbCharOffset);
    btnGroup->addButton(m_rbByteOffset);

    modeLayout->addWidget(m_rbVisualLineCol);
    modeLayout->addWidget(m_rbCharOffset);
    modeLayout->addWidget(m_rbByteOffset);
    mainLayout->addWidget(modeGroup);

    connect(m_rbVisualLineCol, &QRadioButton::toggled, this, &GotoDialog::onModeChanged);
    connect(m_rbCharOffset, &QRadioButton::toggled, this, &GotoDialog::onModeChanged);
    connect(m_rbByteOffset, &QRadioButton::toggled, this, &GotoDialog::onModeChanged);

    auto *inputLayout = new QVBoxLayout();  // NOSONAR(cpp:S5025)
    m_infoLabel = new QLabel(tr("Enter Line (1 - %1) or Line:Col:").arg(m_maxLines), this);
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setPlaceholderText(tr("e.g. 42 or 42:10"));
    connect(m_lineEdit, &QLineEdit::textChanged, this, &GotoDialog::onInputChanged);
    connect(m_lineEdit, &QLineEdit::returnPressed, this, &GotoDialog::onJumpClicked);

    inputLayout->addWidget(m_infoLabel);
    inputLayout->addWidget(m_lineEdit);
    mainLayout->addLayout(inputLayout);

    m_previewLabel = new QLabel(this);
    m_previewLabel->setStyleSheet(QStringLiteral("color: #888888; font-size: 11px;"));
    mainLayout->addWidget(m_previewLabel);

    mainLayout->addStretch();

    auto *buttonLayout = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    buttonLayout->addStretch();
    m_btnJump = new QPushButton(tr("Go To"), this);
    m_btnJump->setDefault(true);
    connect(m_btnJump, &QPushButton::clicked, this, &GotoDialog::onJumpClicked);

    m_btnCancel = new QPushButton(tr("Cancel"), this);
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    buttonLayout->addWidget(m_btnJump);
    buttonLayout->addWidget(m_btnCancel);
    mainLayout->addLayout(buttonLayout);

    onInputChanged(m_lineEdit->text());
}

void GotoDialog::updateTheme() {
    const bool isDark = ThemeManager::instance()->isDarkActive();
    const QString bg = isDark ? QStringLiteral("#1e1e1e") : QStringLiteral("#f8f9fa");
    const QString fg = isDark ? QStringLiteral("#e0e0e0") : QStringLiteral("#212529");
    const QString border = isDark ? QStringLiteral("#333333") : QStringLiteral("#ced4da");

    setStyleSheet(QStringLiteral("QDialog { background-color: %1; color: %2; }"
                                 "QGroupBox { font-weight: bold; border: 1px solid %3; border-radius: 6px; margin-top: "
                                 "10px; padding-top: 10px; }"
                                 "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }"
                                 "QLineEdit { background-color: %1; color: %2; border: 1px solid %3; border-radius: "
                                 "4px; padding: 6px; font-size: 12px; }"
                                 "QPushButton { min-width: 80px; padding: 5px 12px; border-radius: 4px; }")
                      .arg(bg, fg, border));
}

void GotoDialog::onModeChanged() {
    using enum GotoMode;
    if (m_rbVisualLineCol->isChecked()) {
        m_mode = VisualLineCol;
        m_infoLabel->setText(tr("Enter Line (1 - %1) or Line:Col:").arg(m_maxLines));
        m_lineEdit->setPlaceholderText(tr("e.g. 42 or 42:10"));
    } else if (m_rbCharOffset->isChecked()) {
        m_mode = CharOffset;
        m_infoLabel->setText(tr("Enter Character Offset (0 - %1) or Line:-Col:").arg(m_docLength));
        m_lineEdit->setPlaceholderText(tr("e.g. 1500 or 42:-5"));
    } else {
        m_mode = ByteOffset;
        m_infoLabel->setText(tr("Enter Document Byte Offset (0 - %1):").arg(m_docLength));
        m_lineEdit->setPlaceholderText(tr("e.g. 2048"));
    }
    onInputChanged(m_lineEdit->text());
}

void GotoDialog::parseInput(const QString &text) {
    if (!m_editor) {
        return;
    }

    const QString str = text.trimmed();
    if (str.isEmpty()) {
        m_previewLabel->setText(tr("Please enter a target location."));
        return;
    }

    if (m_mode == GotoMode::VisualLineCol) {
        int line = 1;
        int col = 1;
        if (str.contains(QLatin1Char(':')) || str.contains(QLatin1Char(','))) {
            const QString sep = str.contains(QLatin1Char(':')) ? QStringLiteral(":") : QStringLiteral(",");
            const QStringList parts = str.split(sep, Qt::SkipEmptyParts);
            if (!parts.isEmpty()) {
                line = parts.at(0).trimmed().toInt();
            }
            if (parts.size() > 1) {
                col = parts.at(1).trimmed().toInt();
            }
        } else {
            line = str.toInt();
        }

        line = std::clamp(line, 1, m_maxLines);
        col = std::max(1, col);

        const sptr_t lineStart = m_editor->positionFromLine(line - 1);
        const sptr_t lineEnd = m_editor->lineEndPosition(line - 1);
        sptr_t pos = m_editor->findColumn(line - 1, col - 1);
        pos = std::clamp(pos, lineStart, lineEnd);

        m_targetLine = line;
        m_targetCol = col;
        m_targetPosition = pos;

        m_previewLabel->setText(tr("Target: Line %1, Visual Column %2 (Byte pos: %3)").arg(line).arg(col).arg(pos));
    } else if (m_mode == GotoMode::CharOffset) {
        if (str.contains(QStringLiteral(":-"))) {
            const QStringList parts = str.split(QStringLiteral(":-"));
            int line = parts.value(0).toInt();
            int charCol = parts.value(1).toInt();
            line = std::clamp(line, 1, m_maxLines);
            charCol = std::max(1, charCol);

            const sptr_t lineStart = m_editor->positionFromLine(line - 1);
            const sptr_t lineEnd = m_editor->lineEndPosition(line - 1);
            const sptr_t pos = std::min(m_editor->positionRelative(lineStart, charCol - 1), lineEnd);

            m_targetLine = line;
            m_targetCol = charCol;
            m_targetPosition = pos;
            m_previewLabel->setText(tr("Target: Line %1, Char %2 (Byte pos: %3)").arg(line).arg(charCol).arg(pos));
        } else {
            sptr_t charOffset = str.toLongLong();
            charOffset = std::clamp<sptr_t>(charOffset, 0, m_docLength);
            const sptr_t pos = m_editor->positionRelative(0, charOffset);
            const int line = static_cast<int>(m_editor->lineFromPosition(pos)) + 1;
            const int col = static_cast<int>(m_editor->column(pos)) + 1;

            m_targetLine = line;
            m_targetCol = col;
            m_targetPosition = pos;
            m_previewLabel->setText(tr("Target: Char Offset %1 -> Line %2, Col %3").arg(charOffset).arg(line).arg(col));
        }
    } else {
        // Byte Offset
        sptr_t bytePos = str.toLongLong();
        bytePos = std::clamp<sptr_t>(bytePos, 0, m_docLength);
        const int line = static_cast<int>(m_editor->lineFromPosition(bytePos)) + 1;
        const int col = static_cast<int>(m_editor->column(bytePos)) + 1;

        m_targetLine = line;
        m_targetCol = col;
        m_targetPosition = bytePos;
        m_previewLabel->setText(tr("Target: Byte Offset %1 -> Line %2, Col %3").arg(bytePos).arg(line).arg(col));
    }
}

void GotoDialog::onInputChanged(const QString &text) {
    parseInput(text);
}

void GotoDialog::onJumpClicked() {
    performJump();
    accept();
}

void GotoDialog::performJump() {
    if (!m_editor) {
        return;
    }
    m_editor->gotoPos(m_targetPosition);
    m_editor->ensureVisible(m_targetLine - 1);
    m_editor->verticalCentreCaret();
    m_editor->setFocus();
}
