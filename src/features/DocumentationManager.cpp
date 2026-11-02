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
#include <QStandardPaths>
#include <QSettings>
#include <QLocale>

namespace {
bool isIndonesianActive() {
    const QSettings settings;
    QString trSetting = settings.value(QStringLiteral("General/Translation"), QString()).toString();
    if (trSetting.isEmpty()) {
        trSetting = QLocale::system().name();
    }
    return trSetting.startsWith(QStringLiteral("id"), Qt::CaseInsensitive);
}
QString extractDocResourceIfMissing(const QString &resourcePath, const QString &filename) {
    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/doc";
    if (const QDir d; !d.exists(cacheDir) && !d.mkpath(cacheDir)) {
        qWarning("Failed to create documentation cache directory: %s", qUtf8Printable(cacheDir));
    }
    const QString targetPath = cacheDir + "/" + filename;

    QFile res(resourcePath);
    if (!res.exists()) {
        return QString();
    }

    const QFileInfo targetInfo(targetPath);
    if (!targetInfo.exists() || targetInfo.size() != res.size()) {
        if (targetInfo.exists()) {
            QFile::remove(targetPath);
        }
        if (res.copy(targetPath)) {
            QFile::setPermissions(targetPath,
                                  QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser | QFile::ReadGroup);
        }
    }

    return targetInfo.exists() ? targetInfo.canonicalFilePath() : targetPath;
}
}  // namespace

DocumentationManager &DocumentationManager::instance() {
    static DocumentationManager inst;
    return inst;
}

DocumentationManager::DocumentationManager(QObject *parent) : QObject(parent) {}

QString DocumentationManager::localDocumentationPath(const QString &docFilename) {
    const QString appDir = QCoreApplication::applicationDirPath();

    // Check possible local doc paths:
    // 1. Inside macOS app bundle: <App.app>/Contents/Resources/doc/
    // 2. Relative to build dir: <build>/doc/ or <build>/../doc/
    // 3. System shared path: /usr/share/doc/SparkNotePad/
    // 4. Source root / parent directories
    const QStringList candidates = {appDir + "/../Resources/doc/" + docFilename,
                                    appDir + "/doc/" + docFilename,
                                    appDir + "/../doc/" + docFilename,
                                    appDir + "/../../doc/" + docFilename,
                                    appDir + "/../../../doc/" + docFilename,
                                    appDir + "/../../../../doc/" + docFilename,
                                    appDir + "/../" + docFilename,
                                    appDir + "/../../" + docFilename,
                                    appDir + "/../../../" + docFilename,
                                    appDir + "/" + docFilename,
                                    "/usr/share/doc/SparkNotePad/" + docFilename,
                                    "/usr/local/share/doc/SparkNotePad/" + docFilename};

    if (const auto it = std::ranges::find_if(candidates, [](const QString &path) { return QFile::exists(path); });
        it != candidates.cend()) {
        return QFileInfo(*it).canonicalFilePath();
    }

    return QString();
}

QString DocumentationManager::docnotePlaintextPath() const {
    const QString localizedFile =
        isIndonesianActive() ? QStringLiteral("DOCNOTE_id.txt") : QStringLiteral("DOCNOTE_en.txt");
    if (const QString local = localDocumentationPath(localizedFile); !local.isEmpty()) {
        return local;
    }
    const QString resPath =
        isIndonesianActive() ? QStringLiteral(":/doc/DOCNOTE_id.txt") : QStringLiteral(":/doc/DOCNOTE_en.txt");
    if (const QString extracted = extractDocResourceIfMissing(resPath, localizedFile);
        !extracted.isEmpty() && QFile::exists(extracted)) {
        return extracted;
    }

    if (const QString localFallback = localDocumentationPath(QStringLiteral("DOCNOTE.txt")); !localFallback.isEmpty()) {
        return localFallback;
    }
    return extractDocResourceIfMissing(QStringLiteral(":/doc/DOCNOTE.txt"), QStringLiteral("DOCNOTE.txt"));
}

QString DocumentationManager::changelogPlaintextPath() const {
    const QString localizedFile =
        isIndonesianActive() ? QStringLiteral("CHANGELOG_id.txt") : QStringLiteral("CHANGELOG_en.txt");
    if (const QString local = localDocumentationPath(localizedFile); !local.isEmpty()) {
        return local;
    }
    const QString resPath =
        isIndonesianActive() ? QStringLiteral(":/doc/CHANGELOG_id.txt") : QStringLiteral(":/doc/CHANGELOG_en.txt");
    if (const QString extracted = extractDocResourceIfMissing(resPath, localizedFile);
        !extracted.isEmpty() && QFile::exists(extracted)) {
        return extracted;
    }

    if (const QString localFallback = localDocumentationPath(QStringLiteral("CHANGELOG.txt"));
        !localFallback.isEmpty()) {
        return localFallback;
    }
    return extractDocResourceIfMissing(QStringLiteral(":/doc/CHANGELOG.txt"), QStringLiteral("CHANGELOG.txt"));
}

QString DocumentationManager::onlineDocumentationUrl(const QString &docFilename) {
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

void DocumentationManager::openUserGuide(QWidget *parent) const {
    const QString local = localDocumentationPath("USER_GUIDE.md");
    const QString online = onlineDocumentationUrl("USER_GUIDE.md");
    openUrlOrFile(local, online, parent);
}

void DocumentationManager::openDeveloperGuide(QWidget *parent) const {
    const QString local = localDocumentationPath("DEVELOPER_GUIDE.md");
    const QString online = onlineDocumentationUrl("DEVELOPER_GUIDE.md");
    openUrlOrFile(local, online, parent);
}

void DocumentationManager::openOnlineRepository(const QWidget * /*parent*/) {
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://github.com/alsyundawy/SparkNotePad")));
}

void DocumentationManager::openKeyboardShortcutsGuide(QWidget *parent) {
    AccessibilityManager::openAccessibilityHelp(parent);
}
