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

#include "JsEvaluationDialog.h"
#include "ui_JsEvaluationDialog.h"
#include "JsEvaluator.h"
#include "ScintillaNext.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QKeySequence>
#include <QShortcut>

JsEvaluationDialog::JsEvaluationDialog(QWidget *parent, ScintillaNext *activeEditor, const QString &initialCode)
    : QDialog(parent), ui(new Ui::JsEvaluationDialog), m_editor(activeEditor) {
    ui->setupUi(this);

    setupPresets();

    if (!initialCode.trimmed().isEmpty()) {
        ui->editInput->setPlainText(initialCode.trimmed());
    }

    connect(ui->btnEvaluate, &QPushButton::clicked, this, &JsEvaluationDialog::evaluateCode);
    connect(ui->btnClear, &QPushButton::clicked, this, &JsEvaluationDialog::clearAll);
    connect(ui->btnCopy, &QPushButton::clicked, this, &JsEvaluationDialog::copyResult);
    connect(ui->btnInsert, &QPushButton::clicked, this, &JsEvaluationDialog::insertResult);
    connect(ui->comboPresets, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &JsEvaluationDialog::onPresetSelected);
    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::accept);

    auto *evalShortcut1 = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), this);
    connect(evalShortcut1, &QShortcut::activated, this, &JsEvaluationDialog::evaluateCode);

    auto *evalShortcut2 = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Enter), this);
    connect(evalShortcut2, &QShortcut::activated, this, &JsEvaluationDialog::evaluateCode);

    if (!initialCode.trimmed().isEmpty()) {
        evaluateCode();
    }
}

JsEvaluationDialog::~JsEvaluationDialog() {
    delete ui;
}

void JsEvaluationDialog::setCode(const QString &code) {
    ui->editInput->setPlainText(code);
    evaluateCode();
}

void JsEvaluationDialog::setupPresets() {
    ui->comboPresets->addItem(tr("-- Select Preset or Template --"), QString());
    ui->comboPresets->addItem(tr("Math: Trigonometry & Powers"), QStringLiteral("Math.hypot(3, 4) + Math.pow(2, 8)"));
    ui->comboPresets->addItem(tr("Math: Normal Distribution / Pi"),
                              QStringLiteral("const x = 0.5; Math.exp(-0.5 * x * x) / Math.sqrt(2 * Math.PI)"));
    ui->comboPresets->addItem(tr("Array: Map, Filter, Reduce"),
                              QStringLiteral("[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]\n  .filter(n => n % 2 === 0)\n  .map(n "
                                             "=> n * n)\n  .reduce((acc, curr) => acc + curr, 0)"));
    ui->comboPresets->addItem(
        tr("JSON: Format & Pretty-print"),
        QStringLiteral("JSON.stringify({\n  appName: 'NoteSciPad',\n  version: '1.0.0',\n  engine: 'Scintilla + "
                       "Lexilla',\n  tags: ['macOS', 'Linux', 'C++20', 'Qt6']\n}, null, 2)"));
    ui->comboPresets->addItem(tr("Date: Current UTC & Unix Timestamp"),
                              QStringLiteral("({\n  iso: new Date().toISOString(),\n  unix: Math.floor(Date.now() / "
                                             "1000),\n  locale: new Date().toLocaleString()\n})"));
    ui->comboPresets->addItem(tr("String: Reverse & Case Transform"),
                              QStringLiteral("'The quick brown fox jumps over the lazy dog'\n  .split(' ')\n  .map(w "
                                             "=> w.toUpperCase())\n  .reverse()\n  .join(' ')"));
    ui->comboPresets->addItem(
        tr("Regex: Extract Matching Groups"),
        QStringLiteral(
            "const text = '2026-10-19 NoteSciPad Release v1.0.0';\nconst match = "
            "text.match(/(\\d{4}-\\d{2}-\\d{2})\\s+(.+)/);\nmatch ? { date: match[1], title: match[2] } : null"));
    ui->comboPresets->addItem(tr("Crypto: Hex Encoding"),
                              QStringLiteral("Array.from(new TextEncoder().encode('NoteSciPad'))\n  .map(b => "
                                             "b.toString(16).padStart(2, '0'))\n  .join(' ')"));
}

void JsEvaluationDialog::onPresetSelected(int index) {
    if (index <= 0)
        return;
    QString snippet = ui->comboPresets->itemData(index).toString();
    if (!snippet.isEmpty()) {
        ui->editInput->setPlainText(snippet);
        evaluateCode();
    }
}

void JsEvaluationDialog::evaluateCode() {
    const QString script = ui->editInput->toPlainText().trimmed();
    if (script.isEmpty()) {
        ui->editOutput->setPlainText(QString());
        ui->lblStatus->setText(tr("Ready. Enter an expression to evaluate."));
        ui->lblStatus->setStyleSheet(QString());
        return;
    }

    auto result = JsEvaluator::evaluate(script);
    if (result.success) {
        ui->editOutput->setPlainText(result.resultString);
        ui->lblStatus->setText(
            tr("✓ Evaluated successfully in %1 ms").arg(QString::number(result.executionTimeMs, 'f', 2)));
        ui->lblStatus->setStyleSheet(QStringLiteral("color: #2e7d32; font-weight: bold;"));
    } else {
        ui->editOutput->setPlainText(result.errorMessage);
        ui->lblStatus->setText(
            tr("✗ Error during execution (%1 ms)").arg(QString::number(result.executionTimeMs, 'f', 2)));
        ui->lblStatus->setStyleSheet(QStringLiteral("color: #c62828; font-weight: bold;"));
    }
}

void JsEvaluationDialog::clearAll() {
    ui->editInput->clear();
    ui->editOutput->clear();
    ui->lblStatus->setText(tr("Ready."));
    ui->lblStatus->setStyleSheet(QString());
    ui->comboPresets->setCurrentIndex(0);
}

void JsEvaluationDialog::copyResult() {
    const QString text = ui->editOutput->toPlainText();
    if (!text.isEmpty()) {
        QGuiApplication::clipboard()->setText(text);
        ui->lblStatus->setText(tr("Copied to clipboard!"));
        ui->lblStatus->setStyleSheet(QStringLiteral("color: #1565c0; font-weight: bold;"));
    }
}

void JsEvaluationDialog::insertResult() {
    const QString text = ui->editOutput->toPlainText();
    if (text.isEmpty() || !m_editor) {
        return;
    }

    if (m_editor->readOnly()) {
        ui->lblStatus->setText(tr("Cannot insert: Editor is read-only."));
        ui->lblStatus->setStyleSheet(QStringLiteral("color: #c62828; font-weight: bold;"));
        return;
    }

    const int startPos = m_editor->selectionStart();
    const int endPos = m_editor->selectionEnd();

    m_editor->beginUndoAction();
    m_editor->setTargetRange(startPos, endPos);
    const QByteArray utf8 = text.toUtf8();
    m_editor->replaceTarget(utf8.length(), utf8.constData());
    const int newPos = startPos + utf8.length();
    m_editor->setSelection(newPos, newPos);
    m_editor->endUndoAction();

    accept();
}
