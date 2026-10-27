/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ReleaseManager.h"
#include "../dialogs/UpdateDialog.h"
#include "../utils/NetworkSecurityPolicy.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QDebug>
#include <QRegularExpression>

ReleaseManager &ReleaseManager::instance() {
    static ReleaseManager inst;
    return inst;
}

ReleaseManager::ReleaseManager(QObject *parent) : QObject(parent) {
    m_netManager = new QNetworkAccessManager(this);
    connect(m_netManager, &QNetworkAccessManager::finished, this, &ReleaseManager::onReplyFinished);
}

QString ReleaseManager::currentVersion() const {
#ifdef APP_VERSION
    return QString(APP_VERSION);
#else
    return QString("0.9.0");
#endif
}

bool ReleaseManager::isNewerVersion(const QString &latestVersion, const QString &currentVersionStr) const {
    // Clean versions: strip 'v' or 'V'
    QString l = latestVersion.trimmed();
    if (l.startsWith("v", Qt::CaseInsensitive)) {
        l.remove(0, 1);
    }
    QString c = currentVersionStr.trimmed();
    if (c.startsWith("v", Qt::CaseInsensitive)) {
        c.remove(0, 1);
    }

    // Split components before pre-release identifiers (e.g. 1.0.0-beta1)
    QString lCore = l.section('-', 0, 0);
    QString cCore = c.section('-', 0, 0);

    QStringList lParts = lCore.split('.');
    QStringList cParts = cCore.split('.');

    const auto maxLen = static_cast<int>(qMax(lParts.size(), cParts.size()));
    for (int i = 0; i < maxLen; ++i) {
        int lNum = (i < lParts.size()) ? lParts[i].toInt() : 0;
        int cNum = (i < cParts.size()) ? cParts[i].toInt() : 0;
        if (lNum > cNum)
            return true;
        if (lNum < cNum)
            return false;
    }

    // If core numeric parts are equal, check pre-release tags
    bool lHasPre = l.contains('-');
    if (const bool cHasPre = c.contains('-'); !lHasPre && cHasPre)
        return true;  // Full release is newer than pre-release

    return false;
}

void ReleaseManager::checkForUpdates(QWidget *parent, bool silent) {
    m_parentWidget = parent;
    m_silent = silent;

    if (NetworkSecurityPolicy::isOfflineModeEnabled()) {
        qInfo() << "[ReleaseManager] Check for updates suppressed: Strict Offline Mode is active.";
        return;
    }

    QUrl apiUrl("https://api.github.com/repos/alsyundawy/SparkNotePad/releases/latest");
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, QString("SparkNotePad-App/%1").arg(currentVersion()));
    request.setRawHeader("Accept", "application/vnd.github.v3+json");
    // Security: CVE-2026-95620 & CVE-2025-14575 mitigations
    NetworkSecurityPolicy::applySecureTlsConfiguration(request);

    qInfo() << "[ReleaseManager] Checking for updates via:" << apiUrl.toString();
    m_netManager->get(request);
}

void ReleaseManager::onReplyFinished(QNetworkReply *reply) {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "[ReleaseManager] Network request failed:" << reply->errorString();
        if (!m_silent && m_parentWidget) {
            UpdateDialog::showCheckFailed(m_parentWidget, reply->errorString(), m_parentWidget);
        }
        return;
    }

    QByteArray data = reply->readAll();
    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "[ReleaseManager] Failed to parse JSON response:" << parseErr.errorString();
        return;
    }

    QJsonObject obj = doc.object();
    ReleaseInfo info;
    info.versionTag = obj.value("tag_name").toString().trimmed().toHtmlEscaped();
    info.releaseName = obj.value("name").toString().toHtmlEscaped();
    info.releaseNotes = obj.value("body").toString();
    info.htmlUrl = obj.value("html_url").toString();
    info.publishedAt = obj.value("published_at").toString();
    info.isPrerelease = obj.value("prerelease").toBool();

    QString curVer = currentVersion();
    bool updateAvail = isNewerVersion(info.versionTag, curVer);

    emit updateCheckFinished(updateAvail, info);

    if (updateAvail) {
        if (m_parentWidget) {
            UpdateDialog::showUpdateAvailable(m_parentWidget, curVer, info);
        }
    } else {
        if (!m_silent && m_parentWidget) {
            UpdateDialog::showUpToDate(m_parentWidget, curVer);
        }
    }
}
