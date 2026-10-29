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

#include <QSettings>

ReleaseManager &ReleaseManager::instance() {
    static ReleaseManager inst;
    return inst;
}

ReleaseManager::ReleaseManager(QObject *parent) : QObject(parent) {
    m_netManager = new QNetworkAccessManager(this);
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

    // ETag caching to avoid burning rate limits
    QSettings settings;
    const QString cachedEtag = settings.value(QStringLiteral("ReleaseManager/LatestETag")).toString();
    if (!cachedEtag.isEmpty()) {
        request.setRawHeader("If-None-Match", cachedEtag.toUtf8());
    }

    // Security: CVE-2026-95620 & CVE-2025-14575 mitigations
    NetworkSecurityPolicy::applySecureTlsConfiguration(request);

    qInfo() << "[ReleaseManager] Checking for updates via:" << apiUrl.toString();
    QNetworkReply *reply = m_netManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { onReplyFinished(reply); });
}

void ReleaseManager::onReplyFinished(QNetworkReply *reply) {
    reply->deleteLater();

    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    // 1. HTTP 304 Not Modified: Cache is fresh, no rate limit consumed, app is up-to-date
    if (statusCode == 304) {
        qInfo() << "[ReleaseManager] Release check returned 304 Not Modified. App is up-to-date.";
        emit updateCheckFinished(false, ReleaseInfo{});
        if (!m_silent && m_parentWidget) {
            UpdateDialog::showUpToDate(m_parentWidget, currentVersion());
        }
        return;
    }

    // 2. HTTP 403 (Rate limit exceeded) or other network failure -> Fall back to unmetered web redirect
    if (statusCode == 403 || reply->error() == QNetworkReply::ContentAccessDenied ||
        reply->error() != QNetworkReply::NoError) {
        qWarning() << "[ReleaseManager] GitHub REST API rate limit hit or error (HTTP" << statusCode << ":"
                   << reply->errorString() << "). Activating unmetered GitHub web redirect fallback...";
        triggerWebFallback();
        return;
    }

    // 3. Cache ETag if provided by GitHub
    const QByteArray etag = reply->rawHeader("ETag");
    if (!etag.isEmpty()) {
        QSettings settings;
        settings.setValue(QStringLiteral("ReleaseManager/LatestETag"), QString::fromUtf8(etag));
    }

    QByteArray data = reply->readAll();
    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "[ReleaseManager] Failed to parse JSON response:" << parseErr.errorString()
                   << ". Falling back to web redirect...";
        triggerWebFallback();
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

void ReleaseManager::triggerWebFallback() {
    QUrl webUrl("https://github.com/alsyundawy/SparkNotePad/releases/latest");
    QNetworkRequest request(webUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, QString("SparkNotePad-App/%1").arg(currentVersion()));
    NetworkSecurityPolicy::applySecureTlsConfiguration(request);

    qInfo() << "[ReleaseManager] Triggering unmetered web redirect fallback via:" << webUrl.toString();
    QNetworkReply *fallbackReply = m_netManager->get(request);
    connect(fallbackReply, &QNetworkReply::finished, this,
            [this, fallbackReply]() { onFallbackReplyFinished(fallbackReply); });
}

void ReleaseManager::onFallbackReplyFinished(QNetworkReply *reply) {
    reply->deleteLater();

    // Check redirect target or final redirected URL
    QUrl targetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (targetUrl.isEmpty()) {
        targetUrl = reply->url();
    }

    QString fullUrl = targetUrl.toString();
    static const QRegularExpression tagRegex(QStringLiteral("tag/(v?[0-9]+\\.[0-9]+(?:\\.[0-9]+)?)"));
    auto match = tagRegex.match(fullUrl);

    if (!match.hasMatch()) {
        const QByteArray locHeader = reply->rawHeader("Location");
        if (!locHeader.isEmpty()) {
            match = tagRegex.match(QString::fromUtf8(locHeader));
        }
    }

    if (match.hasMatch()) {
        const QString versionTag = match.captured(1);
        qInfo() << "[ReleaseManager] Unmetered web fallback successfully resolved latest release tag:" << versionTag;

        ReleaseInfo info;
        info.versionTag = versionTag;
        info.releaseName = QStringLiteral("SparkNotePad %1").arg(versionTag);
        info.releaseNotes = tr("SparkNotePad %1 is available on GitHub.").arg(versionTag);
        info.htmlUrl = QStringLiteral("https://github.com/alsyundawy/SparkNotePad/releases/tag/%1").arg(versionTag);
        info.isPrerelease = false;

        const QString curVer = currentVersion();
        const bool updateAvail = isNewerVersion(info.versionTag, curVer);

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
        return;
    }

    qWarning() << "[ReleaseManager] Web fallback failed or could not parse tag from URL:" << fullUrl;
    if (!m_silent && m_parentWidget) {
        UpdateDialog::showCheckFailed(
            m_parentWidget, tr("Unable to check for updates. GitHub rate limit exceeded and fallback redirect failed."),
            m_parentWidget);
    }
}
