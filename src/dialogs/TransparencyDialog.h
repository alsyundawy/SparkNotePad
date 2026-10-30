#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Implements Window Transparency Options Dialog.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef TRANSPARENCYDIALOG_H
#define TRANSPARENCYDIALOG_H

#include <QDialog>
#include "../features/TransparentWindowManager.h"

namespace Ui {
class TransparencyDialog;
}

class TransparencyDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransparencyDialog(TransparentWindowManager *manager, QWidget *parent = nullptr);
    ~TransparencyDialog() override;

private slots:
    void onSliderValueChanged(int value);
    void onSpinValueChanged(int value);
    void onApply();
    void onAccept();

private:
    void loadCurrentValues();

    Ui::TransparencyDialog *ui;
    TransparentWindowManager *m_manager;
};

#endif  // TRANSPARENCYDIALOG_H
