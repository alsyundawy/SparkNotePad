#pragma once
/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef PREVIEWTABWIDGET_H
#define PREVIEWTABWIDGET_H

#include <QWidget>
#include <QPointer>
#include <QUrl>

#include "ScintillaNext.h"
class MarkdownBrowser;
class QLabel;
class QPushButton;
class QTimer;

class PreviewTabWidget : public QWidget {
    Q_OBJECT

public:
    explicit PreviewTabWidget(ScintillaNext *editor, QWidget *parent = nullptr);
    ~PreviewTabWidget() override = default;

    [[nodiscard]] ScintillaNext *sourceEditor() const { return m_editor.data(); }
    [[nodiscard]] QString documentTitle() const;

public slots:
    void renderPreview();
    void scheduleRender();
    void openInBrowser();
    void copyHtml();
    void exportHtml();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void updateThemeColors();

signals:
    void titleChanged(const QString &newTitle);
    void closeRequested();

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void handleAnchorClicked(const QUrl &url);
    void onEditorRenamed();
    void onEditorDestroyed();

private:
    void setupUi();
    void applyTheme();
    [[nodiscard]] QString buildDevDocsStyleSheet(bool isDark) const;
    [[nodiscard]] QString wrapHtmlDocument(const QString &rawBody, bool isDark) const;

    QPointer<ScintillaNext> m_editor;
    int m_baseFontSize{13};

    MarkdownBrowser *m_browser{nullptr};
    QTimer *m_debounceTimer{nullptr};

    QLabel *m_badgeLabel{nullptr};
    QLabel *m_titleLabel{nullptr};
    QLabel *m_statsLabel{nullptr};
    QPushButton *m_btnRefresh{nullptr};
    QPushButton *m_btnZoomOut{nullptr};
    QPushButton *m_btnResetZoom{nullptr};
    QPushButton *m_btnZoomIn{nullptr};
    QPushButton *m_btnCopyHtml{nullptr};
    QPushButton *m_btnExportHtml{nullptr};
    QPushButton *m_btnOpenBrowser{nullptr};
    bool m_isUpdatingTheme{false};
};

#endif  // PREVIEWTABWIDGET_H
