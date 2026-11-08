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

#include "QuickSwitcherDialog.h"
#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <algorithm>

QuickSwitcherDialog::QuickSwitcherDialog(QWidget *parent) : QDialog(parent, Qt::FramelessWindowHint | Qt::Popup) {
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setupUi();
}

void QuickSwitcherDialog::setupUi() {
    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(12, 12, 12, 12);

    auto *card = new QWidget(this);
    card->setObjectName(QStringLiteral("quickSwitcherCard"));
    card->setStyleSheet(
        QStringLiteral("#quickSwitcherCard {"
                       "    background-color: #21262d;"
                       "    border: 1px solid #30363d;"
                       "    border-radius: 8px;"
                       "}"
                       "QLineEdit {"
                       "    background-color: #0d1117;"
                       "    color: #e6edf3;"
                       "    border: 1px solid #388bfd;"
                       "    border-radius: 6px;"
                       "    padding: 8px 12px;"
                       "    font-size: 14px;"
                       "}"
                       "QListWidget {"
                       "    background-color: transparent;"
                       "    color: #c9d1d9;"
                       "    border: none;"
                       "    outline: none;"
                       "    font-size: 13px;"
                       "}"
                       "QListWidget::item {"
                       "    padding: 6px 10px;"
                       "    border-radius: 4px;"
                       "    margin-bottom: 2px;"
                       "}"
                       "QListWidget::item:selected {"
                       "    background-color: #1f6feb;"
                       "    color: #ffffff;"
                       "}"
                       "QListWidget::item:hover:!selected {"
                       "    background-color: #161b22;"
                       "}"));

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(10, 10, 10, 10);
    cardLayout->setSpacing(8);

    auto *headerLayout = new QHBoxLayout();
    auto *iconLabel = new QLabel(QStringLiteral("🔍"), card);
    headerLayout->addWidget(iconLabel);

    m_searchEdit = new QLineEdit(card);
    m_searchEdit->setPlaceholderText(tr("Search files, tabs, or symbols... (Esc to close)"));
    m_searchEdit->installEventFilter(this);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &QuickSwitcherDialog::onSearchTextChanged);
    headerLayout->addWidget(m_searchEdit);
    cardLayout->addLayout(headerLayout);

    m_listWidget = new QListWidget(card);
    m_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_listWidget->installEventFilter(this);
    connect(m_listWidget, &QListWidget::itemActivated, this, &QuickSwitcherDialog::onItemActivated);
    connect(m_listWidget, &QListWidget::itemClicked, this, &QuickSwitcherDialog::onItemActivated);
    cardLayout->addWidget(m_listWidget);

    outerLayout->addWidget(card);

    auto *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 160));
    shadow->setOffset(0, 6);
    card->setGraphicsEffect(shadow);

    resize(580, 360);
}

int QuickSwitcherDialog::fuzzyScore(const QString &pattern, const QString &text) {
    if (pattern.isEmpty()) {
        return 1;
    }
    if (text.isEmpty()) {
        return -1;
    }

    const QString lowerPattern = pattern.toLower();
    const QString lowerText = text.toLower();

    // Exact matches
    if (lowerPattern == lowerText) {
        return 1000;
    }

    // Extract filename if path
    qsizetype lastSlash = text.lastIndexOf(QLatin1Char('/'));
    if (lastSlash == -1) {
        lastSlash = text.lastIndexOf(QLatin1Char('\\'));
    }
    const QString fileName = (lastSlash != -1) ? text.mid(lastSlash + 1) : text;
    const QString lowerFileName = fileName.toLower();

    if (lowerPattern == lowerFileName) {
        return 900;
    }

    int score = 0;

    // Bonus if filename or path starts with pattern
    if (lowerFileName.startsWith(lowerPattern)) {
        score += 250;
    } else if (lowerText.startsWith(lowerPattern)) {
        score += 150;
    }

    // Subsequence check
    qsizetype pIdx = 0;
    qsizetype prevTIdx = -2;
    int consecutive = 0;

    for (qsizetype tIdx = 0; tIdx < lowerText.length() && pIdx < lowerPattern.length(); ++tIdx) {
        if (lowerText[tIdx] == lowerPattern[pIdx]) {
            score += 10;

            // Filename bonus: if this match is inside the filename component
            if (tIdx > lastSlash) {
                score += 25;
            }

            // Consecutive match bonus
            if (tIdx == prevTIdx + 1) {
                consecutive++;
                score += (consecutive * 15);
            } else {
                consecutive = 0;
            }

            // Word boundary bonus
            if (tIdx == 0 || tIdx == lastSlash + 1) {
                score += 30;
            } else {
                const QChar prevChar = text[tIdx - 1];
                if (prevChar == QLatin1Char('/') || prevChar == QLatin1Char('\\') || prevChar == QLatin1Char('_') ||
                    prevChar == QLatin1Char('-') || prevChar == QLatin1Char('.')) {
                    score += 20;
                } else if (prevChar.isLower() && text[tIdx].isUpper()) {
                    // camelCase boundary
                    score += 20;
                }
            }

            // Exact case match bonus
            if (pattern[pIdx] == text[tIdx]) {
                score += 5;
            }

            prevTIdx = tIdx;
            pIdx++;
        }
    }

    if (pIdx < lowerPattern.length()) {
        return -1;  // Subsequence not fully matched
    }

    // Minor penalty for total length to favor compact matches
    score -= std::min(static_cast<int>(text.length() / 2), 50);

    return std::max(score, 1);
}

QVector<QuickSwitcherDialog::Item> QuickSwitcherDialog::filterItems(const QVector<Item> &items, const QString &query) {
    if (query.trimmed().isEmpty()) {
        return items;
    }

    QVector<Item> matched;
    matched.reserve(items.size());

    for (const auto &item : items) {
        const int titleScore = fuzzyScore(query, item.title);
        const int pathScore = fuzzyScore(query, item.detailPath);
        const int bestScore = std::max(titleScore, pathScore);
        if (bestScore > 0) {
            Item scoredItem = item;
            scoredItem.score = bestScore;
            matched.append(scoredItem);
        }
    }

    std::ranges::sort(matched, [](const Item &a, const Item &b) {
        if (a.score != b.score) {
            return a.score > b.score;
        }
        return a.title < b.title;
    });

    return matched;
}

void QuickSwitcherDialog::populateItems(const QVector<Item> &items) {
    m_allItems = items;
    execFuzzySearch(m_searchEdit ? m_searchEdit->text() : QString());
}

void QuickSwitcherDialog::execFuzzySearch(const QString &query) {
    m_filteredItems = filterItems(m_allItems, query);
    renderResults(m_filteredItems);
}

void QuickSwitcherDialog::renderResults(const QVector<Item> &results) {
    if (!m_listWidget) {
        return;
    }

    m_listWidget->clear();
    for (qsizetype i = 0; i < results.size(); ++i) {
        const auto &item = results[i];
        QString label = item.title;
        if (!item.detailPath.isEmpty() && item.detailPath != item.title) {
            label += QStringLiteral("  —  ") + item.detailPath;
        }

        auto *const listItem = new QListWidgetItem(label, m_listWidget);
        listItem->setData(Qt::UserRole, static_cast<int>(i));
        listItem->setToolTip(item.detailPath);
        m_listWidget->addItem(listItem);
    }

    if (m_listWidget->count() > 0) {
        m_listWidget->setCurrentRow(0);
    }
}

void QuickSwitcherDialog::onSearchTextChanged(const QString &text) {
    execFuzzySearch(text);
}

void QuickSwitcherDialog::onItemActivated(QListWidgetItem *listItem) {
    if (!listItem) {
        return;
    }
    const int idx = listItem->data(Qt::UserRole).toInt();
    if (idx >= 0 && idx < m_filteredItems.size()) {
        emit itemSelected(m_filteredItems[idx]);
        accept();
    }
}

bool QuickSwitcherDialog::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        const auto *const keyEvent = static_cast<const QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Escape) {
            reject();
            return true;
        }
        if (watched == m_searchEdit) {
            if (keyEvent->key() == Qt::Key_Down) {
                if (const int nextRow = m_listWidget->currentRow() + 1; nextRow < m_listWidget->count()) {
                    m_listWidget->setCurrentRow(nextRow);
                }
                return true;
            }
            if (keyEvent->key() == Qt::Key_Up) {
                if (const int prevRow = m_listWidget->currentRow() - 1; prevRow >= 0) {
                    m_listWidget->setCurrentRow(prevRow);
                }
                return true;
            }
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                if (m_listWidget->currentItem()) {
                    onItemActivated(m_listWidget->currentItem());
                }
                return true;
            }
        }
    }
    return QDialog::eventFilter(watched, event);
}

void QuickSwitcherDialog::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    if (parentWidget()) {
        const QRect parentGeo = parentWidget()->geometry();
        const int x = parentGeo.x() + (parentGeo.width() - width()) / 2;
        const int y = parentGeo.y() + (parentGeo.height() / 6);
        move(x, y);
    }
    if (m_searchEdit) {
        m_searchEdit->selectAll();
        m_searchEdit->setFocus();
    }
}
