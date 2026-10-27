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

#include "FileDiffDialog.h"
#include "DockedEditor.h"
#include "MainWindow.h"
#include "ScintillaNext.h"

#include <QComboBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollBar>
#include <QSplitter>
#include <QToolButton>
#include <QVBoxLayout>

FileDiffDialog::FileDiffDialog(MainWindow *parent) : QDialog(parent), m_mainWindow(parent) {
    setWindowTitle(tr("Side-by-Side File Compare & Diff"));
    resize(1020, 680);

    setupUi();
    setupMarkers();
    populateCombos();
}

void FileDiffDialog::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    // Top control bar
    auto *topLayout = new QHBoxLayout();
    topLayout->setSpacing(6);

    auto *lblA = new QLabel(tr("Original (A):"), this);
    topLayout->addWidget(lblA);

    m_comboA = new QComboBox(this);
    m_comboA->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    topLayout->addWidget(m_comboA);

    m_btnBrowseA = new QPushButton(tr("Browse..."), this);
    connect(m_btnBrowseA, &QPushButton::clicked, this, &FileDiffDialog::onBrowseAClicked);
    topLayout->addWidget(m_btnBrowseA);

    m_btnSwap = new QToolButton(this);
    m_btnSwap->setText(QStringLiteral("⇄"));
    m_btnSwap->setToolTip(tr("Swap Left and Right Files"));
    connect(m_btnSwap, &QToolButton::clicked, this, &FileDiffDialog::onSwapClicked);
    topLayout->addWidget(m_btnSwap);

    auto *lblB = new QLabel(tr("Modified (B):"), this);
    topLayout->addWidget(lblB);

    m_comboB = new QComboBox(this);
    m_comboB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    topLayout->addWidget(m_comboB);

    m_btnBrowseB = new QPushButton(tr("Browse..."), this);
    connect(m_btnBrowseB, &QPushButton::clicked, this, &FileDiffDialog::onBrowseBClicked);
    topLayout->addWidget(m_btnBrowseB);

    m_btnCompare = new QPushButton(tr("Compare"), this);
    m_btnCompare->setDefault(true);
    connect(m_btnCompare, &QPushButton::clicked, this, &FileDiffDialog::onCompareClicked);
    topLayout->addWidget(m_btnCompare);

    mainLayout->addLayout(topLayout);

    // Navigation and Summary Bar
    auto *navLayout = new QHBoxLayout();
    m_lblSummary = new QLabel(tr("Select two files or open tabs to compare."), this);
    navLayout->addWidget(m_lblSummary);

    navLayout->addStretch();

    m_btnPrevDiff = new QPushButton(tr("◀ Prev Diff (Shift+F7)"), this);
    m_btnPrevDiff->setEnabled(false);
    connect(m_btnPrevDiff, &QPushButton::clicked, this, &FileDiffDialog::onPrevDiffClicked);
    navLayout->addWidget(m_btnPrevDiff);

    m_btnNextDiff = new QPushButton(tr("Next Diff (F7) ▶"), this);
    m_btnNextDiff->setEnabled(false);
    connect(m_btnNextDiff, &QPushButton::clicked, this, &FileDiffDialog::onNextDiffClicked);
    navLayout->addWidget(m_btnNextDiff);

    mainLayout->addLayout(navLayout);

    // Center Splitter with dual Scintilla editors
    m_splitter = new QSplitter(Qt::Horizontal, this);

    m_editorA = new ScintillaNext(tr("File A"), m_splitter);
    m_editorA->setReadOnly(true);

    m_editorB = new ScintillaNext(tr("File B"), m_splitter);
    m_editorB->setReadOnly(true);

    m_splitter->addWidget(m_editorA);
    m_splitter->addWidget(m_editorB);
    m_splitter->setSizes({500, 500});

    mainLayout->addWidget(m_splitter, 1);

    // Sync vertical scrolling
    if (auto *scrollA = m_editorA->verticalScrollBar()) {
        connect(scrollA, &QScrollBar::valueChanged, this, &FileDiffDialog::onEditorAScrolled);
    }
    if (auto *scrollB = m_editorB->verticalScrollBar()) {
        connect(scrollB, &QScrollBar::valueChanged, this, &FileDiffDialog::onEditorBScrolled);
    }

    // Bottom close button
    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    auto *btnClose = new QPushButton(tr("Close"), this);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
    bottomLayout->addWidget(btnClose);
    mainLayout->addLayout(bottomLayout);
}

void FileDiffDialog::setupMarkers() {
    auto configureEditor = [](ScintillaNext *ed) {
        if (!ed)
            return;
        ed->setMarginWidthN(0, 45);
        ed->setMarginTypeN(0, SC_MARGIN_NUMBER);
        ed->setMarginWidthN(1, 16);
        ed->setMarginTypeN(1, SC_MARGIN_SYMBOL);
        ed->setMarginMaskN(1, ~0);

        // Marker for added lines (Green: #28a745)
        ed->markerDefine(MARKER_ADDED, SC_MARK_BACKGROUND);
        ed->markerSetBack(MARKER_ADDED, 0x45a728);

        // Marker for deleted lines (Red: #d73a49)
        ed->markerDefine(MARKER_DELETED, SC_MARK_BACKGROUND);
        ed->markerSetBack(MARKER_DELETED, 0x493ad7);

        // Marker for modified lines (Yellow: #e3b341)
        ed->markerDefine(MARKER_MODIFIED, SC_MARK_BACKGROUND);
        ed->markerSetBack(MARKER_MODIFIED, 0x41b3e3);
    };

    configureEditor(m_editorA);
    configureEditor(m_editorB);
}

void FileDiffDialog::populateCombos() {
    m_comboA->clear();
    m_comboB->clear();

    if (!m_mainWindow || !m_mainWindow->getDockedEditor()) {
        return;
    }

    const auto editors = m_mainWindow->getDockedEditor()->editors();
    int idx = 0;
    for (auto *ed : editors) {
        if (!ed)
            continue;
        const QString title = tr("Tab: %1 (%2)").arg(ed->getName()).arg(ed->getFilePath());
        m_comboA->addItem(title, ed->getFilePath());
        m_comboB->addItem(title, ed->getFilePath());
        idx++;
    }

    if (m_comboA->count() > 0) {
        m_comboA->setCurrentIndex(0);
    }
    if (m_comboB->count() > 1) {
        m_comboB->setCurrentIndex(1);
    } else if (m_comboB->count() > 0) {
        m_comboB->setCurrentIndex(0);
    }
}

void FileDiffDialog::setInitialFiles(const QString &pathA, const QString &pathB) {
    if (!pathA.isEmpty()) {
        int idx = m_comboA->findData(pathA);
        if (idx != -1) {
            m_comboA->setCurrentIndex(idx);
        } else {
            m_customPathA = pathA;
            m_comboA->insertItem(0, tr("File: %1").arg(QFileInfo(pathA).fileName()), pathA);
            m_comboA->setCurrentIndex(0);
        }
    }
    if (!pathB.isEmpty()) {
        int idx = m_comboB->findData(pathB);
        if (idx != -1) {
            m_comboB->setCurrentIndex(idx);
        } else {
            m_customPathB = pathB;
            m_comboB->insertItem(0, tr("File: %1").arg(QFileInfo(pathB).fileName()), pathB);
            m_comboB->setCurrentIndex(0);
        }
    }
    if (!pathA.isEmpty() && !pathB.isEmpty()) {
        onCompareClicked();
    }
}

void FileDiffDialog::onBrowseAClicked() {
    const QString file = QFileDialog::getOpenFileName(this, tr("Select Original File (A)"));
    if (!file.isEmpty()) {
        m_customPathA = file;
        m_comboA->insertItem(0, tr("Disk: %1").arg(QFileInfo(file).fileName()), file);
        m_comboA->setCurrentIndex(0);
    }
}

void FileDiffDialog::onBrowseBClicked() {
    const QString file = QFileDialog::getOpenFileName(this, tr("Select Modified File (B)"));
    if (!file.isEmpty()) {
        m_customPathB = file;
        m_comboB->insertItem(0, tr("Disk: %1").arg(QFileInfo(file).fileName()), file);
        m_comboB->setCurrentIndex(0);
    }
}

void FileDiffDialog::onSwapClicked() {
    int curA = m_comboA->currentIndex();
    int curB = m_comboB->currentIndex();
    m_comboA->setCurrentIndex(curB);
    m_comboB->setCurrentIndex(curA);
    onCompareClicked();
}

QString FileDiffDialog::readContent(int comboIndex, const QString &customPath) const {
    if (m_mainWindow && m_mainWindow->getDockedEditor()) {
        const auto editors = m_mainWindow->getDockedEditor()->editors();
        if (comboIndex >= 0 && comboIndex < editors.size()) {
            if (auto *ed = editors[comboIndex]) {
                return QString::fromUtf8(ed->getText(ed->length() + 1));
            }
        }
    }

    // Try reading from file path
    QString path = customPath;
    if (path.isEmpty() && comboIndex >= 0 && comboIndex < m_comboA->count()) {
        path = m_comboA->itemData(comboIndex).toString();
    }
    if (!path.isEmpty() && QFile::exists(path)) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return QString::fromUtf8(file.readAll());
        }
    }
    return QString();
}

void FileDiffDialog::onCompareClicked() {
    const QString textA = readContent(m_comboA->currentIndex(), m_customPathA);
    const QString textB = readContent(m_comboB->currentIndex(), m_customPathB);

    m_editorA->setReadOnly(false);
    m_editorB->setReadOnly(false);
    m_editorA->markerDeleteAll(-1);
    m_editorB->markerDeleteAll(-1);

    const auto diffResult = MyersDiffEngine::compare(textA, textB);

    QString alignedA;
    QString alignedB;
    m_diffLineIndices.clear();

    for (int idx = 0; idx < diffResult.lines.size(); ++idx) {
        const auto &dl = diffResult.lines[idx];
        if (idx > 0) {
            alignedA += QLatin1Char('\n');
            alignedB += QLatin1Char('\n');
        }

        if (dl.op == MyersDiffEngine::DiffOp::Equal) {
            alignedA += dl.textA;
            alignedB += dl.textB;
        } else if (dl.op == MyersDiffEngine::DiffOp::Insert) {
            alignedA += QString();  // Blank line to keep alignment
            alignedB += dl.textB;
            m_diffLineIndices.append(idx);
        } else if (dl.op == MyersDiffEngine::DiffOp::Delete) {
            alignedA += dl.textA;
            alignedB += QString();  // Blank line to keep alignment
            m_diffLineIndices.append(idx);
        } else if (dl.op == MyersDiffEngine::DiffOp::Modify) {
            alignedA += dl.textA;
            alignedB += dl.textB;
            m_diffLineIndices.append(idx);
        }
    }

    const QByteArray bytesA = alignedA.toUtf8();
    const QByteArray bytesB = alignedB.toUtf8();
    m_editorA->setText(bytesA.constData());
    m_editorB->setText(bytesB.constData());

    // Highlight diff markers
    for (int idx = 0; idx < diffResult.lines.size(); ++idx) {
        const auto &dl = diffResult.lines[idx];
        if (dl.op == MyersDiffEngine::DiffOp::Insert) {
            m_editorB->markerAdd(idx, MARKER_ADDED);
        } else if (dl.op == MyersDiffEngine::DiffOp::Delete) {
            m_editorA->markerAdd(idx, MARKER_DELETED);
        } else if (dl.op == MyersDiffEngine::DiffOp::Modify) {
            m_editorA->markerAdd(idx, MARKER_MODIFIED);
            m_editorB->markerAdd(idx, MARKER_MODIFIED);
        }
    }

    m_editorA->setReadOnly(true);
    m_editorB->setReadOnly(true);

    m_lblSummary->setText(tr("<b>Diff Summary:</b> <font color='#28a745'>+%1 Added</font> | "
                             "<font color='#d73a49'>-%2 Removed</font> | "
                             "<font color='#d9a300'>~%3 Modified</font>")
                              .arg(diffResult.additions)
                              .arg(diffResult.deletions)
                              .arg(diffResult.modifications));

    const bool hasDiff = !m_diffLineIndices.isEmpty();
    m_btnNextDiff->setEnabled(hasDiff);
    m_btnPrevDiff->setEnabled(hasDiff);
    m_currentDiffIndex = hasDiff ? 0 : -1;

    if (hasDiff) {
        onNextDiffClicked();
    }
}

void FileDiffDialog::onNextDiffClicked() {
    if (m_diffLineIndices.isEmpty())
        return;
    m_currentDiffIndex = (m_currentDiffIndex + 1) % m_diffLineIndices.size();
    const int targetLine = m_diffLineIndices[m_currentDiffIndex];
    m_editorA->gotoLine(targetLine);
    m_editorB->gotoLine(targetLine);
    m_editorA->scrollVertical(targetLine, 0);
    m_editorB->scrollVertical(targetLine, 0);
}

void FileDiffDialog::onPrevDiffClicked() {
    if (m_diffLineIndices.isEmpty())
        return;
    m_currentDiffIndex = (m_currentDiffIndex - 1 + m_diffLineIndices.size()) % m_diffLineIndices.size();
    const int targetLine = m_diffLineIndices[m_currentDiffIndex];
    m_editorA->gotoLine(targetLine);
    m_editorB->gotoLine(targetLine);
    m_editorA->scrollVertical(targetLine, 0);
    m_editorB->scrollVertical(targetLine, 0);
}

void FileDiffDialog::onEditorAScrolled(int value) {
    if (m_syncingScroll)
        return;
    m_syncingScroll = true;
    if (auto *scrollB = m_editorB->verticalScrollBar()) {
        scrollB->setValue(value);
    }
    m_syncingScroll = false;
}

void FileDiffDialog::onEditorBScrolled(int value) {
    if (m_syncingScroll)
        return;
    m_syncingScroll = true;
    if (auto *scrollA = m_editorA->verticalScrollBar()) {
        scrollA->setValue(value);
    }
    m_syncingScroll = false;
}
