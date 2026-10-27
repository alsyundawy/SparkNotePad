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
#include <QTextBrowser>
#include <QTimer>
#include <QUrl>
#include <QPointer>

class ScintillaNext;
class MainWindow;

class MarkdownSafePreviewDock : public QDockWidget {
    Q_OBJECT

public:
    explicit MarkdownSafePreviewDock(MainWindow *parent = nullptr);
    ~MarkdownSafePreviewDock() override = default;

public slots:
    void setActiveEditor(ScintillaNext *editor);
    void scheduleRender();
    void renderMarkdown();

private slots:
    void handleAnchorClicked(const QUrl &url);

private:
    QTextBrowser *browser = nullptr;
    QTimer *debounceTimer = nullptr;
    QPointer<ScintillaNext> currentEditor;
};
