#pragma once
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

#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include "ScintillaTypes.h"

class ScintillaNext;

class GotoDialog : public QDialog {
    Q_OBJECT

public:
    enum class GotoMode {
        VisualLineCol,  // Line:Col with tab expansion
        CharOffset,     // Line:-Col or Character offset (Unicode 17 character semantics)
        ByteOffset      // Direct document byte position
    };

    explicit GotoDialog(ScintillaNext *editor, QWidget *parent = nullptr);

    int targetLine() const { return m_targetLine; }
    int targetColumn() const { return m_targetCol; }
    Scintilla::sptr_t targetPosition() const { return m_targetPosition; }
    GotoMode mode() const { return m_mode; }

    void performJump();

private slots:
    void onInputChanged(const QString &text);
    void onModeChanged();
    void onJumpClicked();

private:
    void setupUi();
    void updateTheme();
    void parseInput(const QString &text);

    ScintillaNext *m_editor{nullptr};
    GotoMode m_mode{GotoMode::VisualLineCol};

    QLineEdit *m_lineEdit{nullptr};
    QRadioButton *m_rbVisualLineCol{nullptr};
    QRadioButton *m_rbCharOffset{nullptr};
    QRadioButton *m_rbByteOffset{nullptr};
    QLabel *m_infoLabel{nullptr};
    QLabel *m_previewLabel{nullptr};
    QPushButton *m_btnJump{nullptr};
    QPushButton *m_btnCancel{nullptr};

    int m_maxLines{1};
    Scintilla::sptr_t m_docLength{0};

    int m_targetLine{1};
    int m_targetCol{1};
    Scintilla::sptr_t m_targetPosition{0};
};

#endif  // GOTODIALOG_H
