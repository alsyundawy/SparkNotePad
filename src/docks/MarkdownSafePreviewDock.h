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
#include <QTextBrowser>
#include <QTimer>
#include <QUrl>
#include <cstdint>
#include "ScintillaTypes.h"
#include "../utils/MarkdownThemeManager.h"

class ScintillaNext;
class MainWindow;
class QComboBox;
class QLabel;
class QPushButton;

class MarkdownSafePreviewDock : public QDockWidget {
    Q_OBJECT

public:
    enum class PreviewFormat : std::uint8_t {
        Auto = 0,
        Markdown = 1,
        Html = 2,
    };
    Q_ENUM(PreviewFormat)

    explicit MarkdownSafePreviewDock(MainWindow *parent = nullptr);
    ~MarkdownSafePreviewDock() override = default;

    PreviewFormat format() const { return currentFormat; }
    MarkdownThemeManager::ThemeId theme() const { return currentTheme; }
    static PreviewFormat detectFormat(const ScintillaNext *editor);
    bool isAutoShowEnabled() const { return autoShowEnabled; }

public slots:
    void setActiveEditor(ScintillaNext *editor);
    void scheduleRender();
    void renderPreview();
    void setPreviewFormat(PreviewFormat newFormat);
    void setPreviewTheme(MarkdownThemeManager::ThemeId newTheme);
    void setAutoShowEnabled(bool enabled);
    void openInBrowser();
    void copyHtml();
    void exportHtml();
    void exportPdf();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void syncScrollFromEditor(Scintilla::Update updated);
    void syncScrollFromBrowser();

signals:
    void formatChanged(PreviewFormat newFormat);
    void autoShowChanged(bool enabled);

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void handleAnchorClicked(const QUrl &url);
    void onFormatComboChanged(int index);
    void onThemeComboChanged(int index);

private:
    struct ToolbarUi {
        QComboBox *comboFormat = nullptr;
        QComboBox *comboTheme = nullptr;
        QLabel *lblFormatStatus = nullptr;
        QLabel *lblStats = nullptr;
        QPushButton *btnOpenBrowser = nullptr;
        QPushButton *btnCopyHtml = nullptr;
        QPushButton *btnExportHtml = nullptr;
        QPushButton *btnExportPdf = nullptr;
        QPushButton *btnRefresh = nullptr;
        QPushButton *btnZoomIn = nullptr;
        QPushButton *btnZoomOut = nullptr;
        QPushButton *btnResetZoom = nullptr;
    };

    void setupUi();
    void updateFormatLabel(PreviewFormat effectiveFormat);
    void applyGitHubTheme();

    ToolbarUi m_ui;
    QTextBrowser *browser = nullptr;
    QTimer *debounceTimer = nullptr;
    QPointer<ScintillaNext> currentEditor;

    PreviewFormat currentFormat = PreviewFormat::Auto;
    MarkdownThemeManager::ThemeId currentTheme = MarkdownThemeManager::ThemeId::GitHub;
    bool m_userExplicitTheme = false;
    bool autoShowEnabled = false;
    int baseFontSize = 13;
    bool m_isUpdatingTheme{false};
    bool m_syncingScroll{false};
};
