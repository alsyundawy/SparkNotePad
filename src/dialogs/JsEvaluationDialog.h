#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Interactive JavaScript Expression & Script Evaluator Dialog.
 * Adapted from Notepad4 (zufuliu/notepad4) Evaluate JS Expression feature.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef JSEVALUATIONDIALOG_H
#define JSEVALUATIONDIALOG_H

#include <QDialog>

namespace Ui {
class JsEvaluationDialog;
}

class ScintillaNext;

class JsEvaluationDialog : public QDialog {
    Q_OBJECT

public:
    explicit JsEvaluationDialog(QWidget *parent = nullptr, ScintillaNext *activeEditor = nullptr,
                                const QString &initialCode = QString());
    ~JsEvaluationDialog() override;

    void setCode(const QString &code);

private slots:
    void evaluateCode();
    void clearAll();
    void copyResult();
    void insertResult();
    void onPresetSelected(int index);

private:
    void setupPresets();

    Ui::JsEvaluationDialog *ui;
    ScintillaNext *m_editor{nullptr};
};

#endif  // JSEVALUATIONDIALOG_H
