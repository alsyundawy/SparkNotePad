/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "DocumentationManager.h"
#include "AccessibilityManager.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>

DocumentationManager &DocumentationManager::instance() {
    static DocumentationManager inst;
    return inst;
}

DocumentationManager::DocumentationManager(QObject *parent) : QObject(parent) {}

QString DocumentationManager::localDocumentationPath(const QString &docFilename) const {
    QString appDir = QCoreApplication::applicationDirPath();

    // Check possible local doc paths:
    // 1. Inside macOS app bundle: <App.app>/Contents/Resources/doc/
    // 2. Relative to build dir: <build>/doc/ or <build>/../doc/
    // 3. System shared path: /usr/share/doc/SparkNotePad/
    QStringList candidates = {appDir + "/../Resources/doc/" + docFilename,
                              appDir + "/doc/" + docFilename,
                              appDir + "/../doc/" + docFilename,
                              appDir + "/../../doc/" + docFilename,
                              "/usr/share/doc/SparkNotePad/" + docFilename,
                              "/usr/local/share/doc/SparkNotePad/" + docFilename};

    const auto it =
        std::find_if(candidates.cbegin(), candidates.cend(), [](const QString &path) { return QFile::exists(path); });
    if (it != candidates.cend()) {
        return QFileInfo(*it).canonicalFilePath();
    }

    return QString();
}

QString DocumentationManager::onlineDocumentationUrl(const QString &docFilename) const {
    return QString("https://github.com/alsyundawy/SparkNotePad/blob/main/doc/%1").arg(docFilename);
}

void DocumentationManager::openUrlOrFile(const QString &filePath, const QString &fallbackUrl, QWidget *parent) {
    bool opened = false;
    if (!filePath.isEmpty() && QFile::exists(filePath)) {
        opened = QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }

    if (!opened && !fallbackUrl.isEmpty()) {
        opened = QDesktopServices::openUrl(QUrl(fallbackUrl));
    }

    if (!opened && parent) {
        QMessageBox::warning(
            parent, tr("Documentation Error"),
            tr("Unable to open documentation at:\n%1\n\nOr fallback online URL:\n%2").arg(filePath, fallbackUrl));
    }
}

void DocumentationManager::openUserGuide(QWidget *parent) {
    QString local = localDocumentationPath("USER_GUIDE.md");
    QString online = onlineDocumentationUrl("USER_GUIDE.md");
    openUrlOrFile(local, online, parent);
}

void DocumentationManager::openDeveloperGuide(QWidget *parent) {
    QString local = localDocumentationPath("DEVELOPER_GUIDE.md");
    QString online = onlineDocumentationUrl("DEVELOPER_GUIDE.md");
    openUrlOrFile(local, online, parent);
}

void DocumentationManager::openOnlineRepository(QWidget *parent) {
    Q_UNUSED(parent);
    QDesktopServices::openUrl(QUrl("https://github.com/alsyundawy/SparkNotePad"));
}

void DocumentationManager::openKeyboardShortcutsGuide(QWidget *parent) {
    AccessibilityManager::instance()->openAccessibilityHelp(parent);
}
