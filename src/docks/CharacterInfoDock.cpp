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

#include "CharacterInfoDock.h"
#include <QApplication>
#include <QClipboard>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

#ifndef UNIT_TESTING
#include "ScintillaNext.h"
#endif

CharacterInfoDock::CharacterInfoDock(QWidget *parent) : QDockWidget(tr("Character Inspector"), parent) {
    setObjectName(QStringLiteral("CharacterInfoDock"));
    setupUi();
    updateCharacter(0);
}

void CharacterInfoDock::setupUi() {
    auto *const container = new QWidget(this);
    auto *const mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(6);

    // Hero box showing glyph & code point
    auto *const heroWidget = new QWidget(container);
    heroWidget->setStyleSheet(
        QStringLiteral("background-color: #0d1117;"
                       "border: 1px solid #30363d;"
                       "border-radius: 6px;"
                       "padding: 8px;"));
    auto *const heroLayout = new QVBoxLayout(heroWidget);
    heroLayout->setAlignment(Qt::AlignCenter);

    m_lblGlyph = new QLabel(QStringLiteral("-"), heroWidget);
    m_lblGlyph->setAlignment(Qt::AlignCenter);
    m_lblGlyph->setStyleSheet(
        QStringLiteral("font-size: 42px; font-weight: bold; color: #58a6ff; font-family: monospace;"));
    heroLayout->addWidget(m_lblGlyph);

    m_lblCodePoint = new QLabel(QStringLiteral("U+0000"), heroWidget);
    m_lblCodePoint->setAlignment(Qt::AlignCenter);
    m_lblCodePoint->setStyleSheet(QStringLiteral("font-size: 14px; font-weight: 600; color: #8b949e;"));
    heroLayout->addWidget(m_lblCodePoint);

    m_lblName = new QLabel(tr("None"), heroWidget);
    m_lblName->setAlignment(Qt::AlignCenter);
    m_lblName->setWordWrap(true);
    m_lblName->setStyleSheet(QStringLiteral("font-size: 13px; font-weight: 500; color: #e6edf3;"));
    heroLayout->addWidget(m_lblName);

    m_lblCategory = new QLabel(heroWidget);
    m_lblCategory->setAlignment(Qt::AlignCenter);
    m_lblCategory->setStyleSheet(QStringLiteral("font-size: 11px; color: #7ee787;"));
    heroLayout->addWidget(m_lblCategory);

    mainLayout->addWidget(heroWidget);

    // Detailed properties table
    m_tableDetails = new QTableWidget(0, 2, container);
    m_tableDetails->setHorizontalHeaderLabels({tr("Property"), tr("Value")});
    m_tableDetails->horizontalHeader()->setStretchLastSection(true);
    m_tableDetails->verticalHeader()->setVisible(false);
    m_tableDetails->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableDetails->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableDetails->setAlternatingRowColors(true);
    mainLayout->addWidget(m_tableDetails);

    // Copy buttons
    auto *const btnGrid = new QGridLayout();
    m_btnCopyGlyph = new QPushButton(tr("Copy Character"), container);
    m_btnCopyCodePoint = new QPushButton(tr("Copy Code Point"), container);
    m_btnCopyUtf8 = new QPushButton(tr("Copy UTF-8"), container);
    m_btnCopyHtml = new QPushButton(tr("Copy HTML"), container);

    connect(m_btnCopyGlyph, &QPushButton::clicked, this, &CharacterInfoDock::onCopyGlyphClicked);
    connect(m_btnCopyCodePoint, &QPushButton::clicked, this, &CharacterInfoDock::onCopyCodePointClicked);
    connect(m_btnCopyUtf8, &QPushButton::clicked, this, &CharacterInfoDock::onCopyUtf8Clicked);
    connect(m_btnCopyHtml, &QPushButton::clicked, this, &CharacterInfoDock::onCopyHtmlClicked);

    btnGrid->addWidget(m_btnCopyGlyph, 0, 0);
    btnGrid->addWidget(m_btnCopyCodePoint, 0, 1);
    btnGrid->addWidget(m_btnCopyUtf8, 1, 0);
    btnGrid->addWidget(m_btnCopyHtml, 1, 1);
    mainLayout->addLayout(btnGrid);

    setWidget(container);
}

void CharacterInfoDock::updateCharacter(uint32_t codePoint) {
    const CharacterInfo::Details details = CharacterInfo::getDetailsForCodePoint(codePoint);
    populateDetails(details);
}

void CharacterInfoDock::updateFromEditor(ScintillaNext *editor) {
#ifdef UNIT_TESTING
    (void)editor;
#else
    if (!editor) {
        return;
    }
    const CharacterInfo::Details details = CharacterInfo::getDetailsAt(editor);
    populateDetails(details);
#endif
}

void CharacterInfoDock::populateDetails(const CharacterInfo::Details &d) {
    m_details = d;
    if (!d.isValid) {
        m_lblGlyph->setText(QStringLiteral("-"));
        m_lblCodePoint->setText(QStringLiteral("U+0000"));
        m_lblName->setText(tr("None"));
        m_lblCategory->setText(QString());
        m_tableDetails->setRowCount(0);
        return;
    }

    m_lblGlyph->setText(d.displayGlyph.isEmpty() ? d.glyph : d.displayGlyph);
    m_lblCodePoint->setText(CharacterInfo::formatCodePoint(d.codePoint));
    m_lblName->setText(d.name);
    m_lblCategory->setText(QStringLiteral("%1 | %2").arg(d.block, d.categoryDescription));

    struct KeyVal {
        QString key;
        QString val;
    };

    const QList<KeyVal> rows = {{tr("Code Point"), CharacterInfo::formatCodePoint(d.codePoint)},
                                {tr("Decimal"), QString::number(d.codePoint)},
                                {tr("Unicode Block"), d.block},
                                {tr("Category"), QStringLiteral("%1 (%2)").arg(d.categoryDescription, d.categoryCode)},
                                {tr("Script"), d.script},
                                {tr("Direction"), d.direction},
                                {tr("UTF-8 Hex"), d.utf8Hex},
                                {tr("UTF-16 Hex"), d.utf16Hex},
                                {tr("UTF-32 Hex"), d.utf32Hex},
                                {tr("HTML Entity"), d.htmlNamedEntity.isEmpty() ? d.htmlDecEntity : d.htmlNamedEntity},
                                {tr("C/C++ Escape"), d.cEscape},
                                {tr("JSON Escape"), d.jsonEscape},
                                {tr("URL Percent"), d.urlPercent}};

    m_tableDetails->setRowCount(static_cast<int>(rows.size()));
    for (qsizetype i = 0; i < rows.size(); ++i) {
        m_tableDetails->setItem(static_cast<int>(i), 0, new QTableWidgetItem(rows[i].key));
        m_tableDetails->setItem(static_cast<int>(i), 1, new QTableWidgetItem(rows[i].val));
    }
}

uint32_t CharacterInfoDock::currentCodePoint() const {
    return m_details.codePoint;
}

QString CharacterInfoDock::characterName() const {
    return m_details.name;
}

void CharacterInfoDock::onCopyGlyphClicked() const {
    if (QClipboard *const cb = QApplication::clipboard(); cb && m_details.isValid) {
        cb->setText(m_details.glyph);
    }
}

void CharacterInfoDock::onCopyCodePointClicked() const {
    if (QClipboard *const cb = QApplication::clipboard(); cb && m_details.isValid) {
        cb->setText(CharacterInfo::formatCodePoint(m_details.codePoint));
    }
}

void CharacterInfoDock::onCopyUtf8Clicked() const {
    if (QClipboard *const cb = QApplication::clipboard(); cb && m_details.isValid) {
        cb->setText(m_details.utf8Hex);
    }
}

void CharacterInfoDock::onCopyHtmlClicked() const {
    if (QClipboard *const cb = QApplication::clipboard(); cb && m_details.isValid) {
        cb->setText(m_details.htmlNamedEntity.isEmpty() ? m_details.htmlDecEntity : m_details.htmlNamedEntity);
    }
}
