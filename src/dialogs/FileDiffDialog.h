#pragma once
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

#ifndef FILEDIFFDIALOG_H
#define FILEDIFFDIALOG_H

#include <QDialog>
#include <QVector>
#include "utils/MyersDiffEngine.h"

class QComboBox;
class QLabel;
class QPushButton;
class QToolButton;
class QSplitter;
class MainWindow;
class ScintillaNext;

class FileDiffDialog : public QDialog {
    Q_OBJECT

public:
    explicit FileDiffDialog(MainWindow *parent);
    ~FileDiffDialog() override = default;

    void setInitialFiles(const QString &pathA, const QString &pathB = QString());

private slots:
    void onCompareClicked();
    void onSwapClicked();
    void onBrowseAClicked();
    void onBrowseBClicked();
    void onNextDiffClicked();
    void onPrevDiffClicked();
    void onEditorAScrolled(int value);
    void onEditorBScrolled(int value);

private:
    void setupUi();
    void setupMarkers();
    void populateCombos();
    QString readContent(int comboIndex, const QString &customPath) const;

    MainWindow *m_mainWindow{nullptr};
    QComboBox *m_comboA{nullptr};
    QComboBox *m_comboB{nullptr};
    QPushButton *m_btnBrowseA{nullptr};
    QPushButton *m_btnBrowseB{nullptr};
    QToolButton *m_btnSwap{nullptr};
    QPushButton *m_btnCompare{nullptr};
    QPushButton *m_btnPrevDiff{nullptr};
    QPushButton *m_btnNextDiff{nullptr};
    QLabel *m_lblSummary{nullptr};

    QSplitter *m_splitter{nullptr};
    ScintillaNext *m_editorA{nullptr};
    ScintillaNext *m_editorB{nullptr};

    QString m_customPathA;
    QString m_customPathB;

    QVector<int> m_diffLineIndices;
    int m_currentDiffIndex{-1};
    bool m_syncingScroll{false};

    static constexpr int MARKER_ADDED = 28;
    static constexpr int MARKER_DELETED = 29;
    static constexpr int MARKER_MODIFIED = 30;
};

#endif  // FILEDIFFDIALOG_H
