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

#include "MarkdownSafePreviewDock.h"
#include "ScintillaNext.h"
#include "MainWindow.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QVBoxLayout>

MarkdownSafePreviewDock::MarkdownSafePreviewDock(MainWindow *parent)
    : QDockWidget(tr("Markdown Safe Preview"), parent),
      browser(new QTextBrowser(this)),
      debounceTimer(new QTimer(this)) {
    setObjectName(QStringLiteral("MarkdownSafePreviewDock"));

    // Hardened sandboxing: disable automatic link opening to intercept all navigation (CVE-2026-20841)
    browser->setOpenExternalLinks(false);
    browser->setOpenLinks(false);
    connect(browser, &QTextBrowser::anchorClicked, this, &MarkdownSafePreviewDock::handleAnchorClicked);

    auto *container = new QWidget(this);
    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(browser);
    setWidget(container);

    debounceTimer->setSingleShot(true);
    debounceTimer->setInterval(300);
    connect(debounceTimer, &QTimer::timeout, this, &MarkdownSafePreviewDock::renderMarkdown);
}

void MarkdownSafePreviewDock::setActiveEditor(ScintillaNext *editor) {
    if (currentEditor == editor)
        return;

    if (currentEditor) {
        disconnect(currentEditor, nullptr, this, nullptr);
    }

    currentEditor = editor;

    if (currentEditor) {
        connect(currentEditor, &ScintillaNext::notifyChange, this, &MarkdownSafePreviewDock::scheduleRender);
    }

    scheduleRender();
}

void MarkdownSafePreviewDock::scheduleRender() {
    if (isVisible()) {
        debounceTimer->start();
    }
}

void MarkdownSafePreviewDock::renderMarkdown() {
    if (!currentEditor) {
        browser->clear();
        return;
    }

    const QString content = QString::fromUtf8(currentEditor->getText());
    browser->setMarkdown(content);
}

void MarkdownSafePreviewDock::handleAnchorClicked(const QUrl &url) {
    const QString scheme = url.scheme().toLower();

    // Whitelist: strictly allow only safe external protocols
    if (scheme == QStringLiteral("http") || scheme == QStringLiteral("https") || scheme == QStringLiteral("mailto")) {
        QDesktopServices::openUrl(url);
        return;
    }

    // Explicitly block and warn against local execution schemes (CVE-2026-20841)
    qWarning("Blocked unsafe Markdown preview link navigation: %s", qUtf8Printable(url.toString()));
    QMessageBox::warning(
        this, tr("Blocked Untrusted Link"),
        tr("Navigation to link '%1' with scheme '%2' is blocked for security.\n\n"
           "Only http, https, and mailto URLs are permitted in Markdown Safe Preview (mitigating CVE-2026-20841).")
            .arg(url.toString(), scheme));
}
