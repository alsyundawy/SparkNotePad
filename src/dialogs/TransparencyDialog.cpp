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

#include "TransparencyDialog.h"
#include "ui_TransparencyDialog.h"

#include <QPushButton>

TransparencyDialog::TransparencyDialog(TransparentWindowManager *manager, QWidget *parent)
    : QDialog(parent), ui(new Ui::TransparencyDialog), m_manager(manager) {
    ui->setupUi(this);

    connect(ui->sliderOpacity, &QSlider::valueChanged, this, &TransparencyDialog::onSliderValueChanged);
    connect(ui->spinOpacity, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &TransparencyDialog::onSpinValueChanged);

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &TransparencyDialog::onApply);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &TransparencyDialog::onAccept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    loadCurrentValues();
}

TransparencyDialog::~TransparencyDialog() {
    delete ui;
}

void TransparencyDialog::loadCurrentValues() {
    if (!m_manager)
        return;

    ui->checkEnable->setChecked(m_manager->isEnabled());
    ui->sliderOpacity->setValue(m_manager->opacity());
    ui->spinOpacity->setValue(m_manager->opacity());

    if (m_manager->mode() == TransparentWindowManager::TransparencyMode::OnlyWhenInactive) {
        ui->radioInactive->setChecked(true);
    } else {
        ui->radioAlways->setChecked(true);
    }

    ui->checkAlwaysOnTop->setChecked(m_manager->isAlwaysOnTop());
}

void TransparencyDialog::onSliderValueChanged(int value) {
    ui->spinOpacity->blockSignals(true);
    ui->spinOpacity->setValue(value);
    ui->spinOpacity->blockSignals(false);
}

void TransparencyDialog::onSpinValueChanged(int value) {
    ui->sliderOpacity->blockSignals(true);
    ui->sliderOpacity->setValue(value);
    ui->sliderOpacity->blockSignals(false);
}

void TransparencyDialog::onApply() {
    if (!m_manager)
        return;

    m_manager->setEnabled(ui->checkEnable->isChecked());
    m_manager->setOpacity(ui->sliderOpacity->value());

    auto mode = ui->radioInactive->isChecked() ? TransparentWindowManager::TransparencyMode::OnlyWhenInactive
                                               : TransparentWindowManager::TransparencyMode::Always;
    m_manager->setMode(mode);

    m_manager->setAlwaysOnTop(ui->checkAlwaysOnTop->isChecked());
}

void TransparencyDialog::onAccept() {
    onApply();
    accept();
}
