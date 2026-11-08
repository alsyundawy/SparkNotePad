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

#include <QDockWidget>
#include <QPointer>
#include <cstdint>

class QLabel;
class QPlainTextEdit;
class ScintillaNext;

class DocumentMinimapDock : public QDockWidget {
    Q_OBJECT

public:
    explicit DocumentMinimapDock(QWidget *parent = nullptr);
    ~DocumentMinimapDock() override = default;

    void syncWithEditor(ScintillaNext *editor);
    void setLargeFileLimitBytes(qint64 limitBytes);
    qint64 largeFileLimitBytes() const;
    bool isMinimapActive() const;

public slots:
    void updateContent();
    void updateViewport();

private:
    void setupUi();

    QPointer<ScintillaNext> m_currentEditor;
    QPlainTextEdit *m_minimapEdit{nullptr};
    QLabel *m_lblDisabledNotice{nullptr};
    qint64 m_largeFileLimitBytes{50 * 1024 * 1024};  // 50 MB
    bool m_active{false};
};
