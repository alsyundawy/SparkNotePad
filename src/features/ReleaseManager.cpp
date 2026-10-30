/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ReleaseManager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include <QAbstractButton>
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

    int maxLen = qMax(lParts.size(), cParts.size());
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
    bool cHasPre = c.contains('-');
    if (!lHasPre && cHasPre)
        return true;  // Full release is newer than pre-release

    return false;
}

void ReleaseManager::checkForUpdates(QWidget *parent, bool silent) {
    m_parentWidget = parent;
    m_silent = silent;

    QUrl apiUrl("https://api.github.com/repos/alsyundawy/NoteSciPad/releases/latest");
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, QString("NoteSciPad-App/%1").arg(currentVersion()));
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    qInfo() << "[ReleaseManager] Checking for updates via:" << apiUrl.toString();
    m_netManager->get(request);
}

void ReleaseManager::onReplyFinished(QNetworkReply *reply) {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "[ReleaseManager] Network request failed:" << reply->errorString();
        if (!m_silent && m_parentWidget) {
            QMessageBox::warning(m_parentWidget, tr("Update Check Failed"),
                                 tr("Unable to connect to update server:\n%1\n\nPlease check your internet connection.")
                                     .arg(reply->errorString()));
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
    info.versionTag = obj.value("tag_name").toString();
    info.releaseName = obj.value("name").toString();
    info.releaseNotes = obj.value("body").toString();
    info.htmlUrl = obj.value("html_url").toString();
    info.publishedAt = obj.value("published_at").toString();
    info.isPrerelease = obj.value("prerelease").toBool();

    QString curVer = currentVersion();
    bool updateAvail = isNewerVersion(info.versionTag, curVer);

    emit updateCheckFinished(updateAvail, info);

    if (updateAvail) {
        if (m_parentWidget) {
            QString msg = tr("<h3>A new version of NoteSciPad is available!</h3>"
                             "<p><b>Installed Version:</b> %1<br/>"
                             "<b>Latest Version:</b> %2 (%3)</p>"
                             "<p>%4</p>"
                             "<p>Would you like to open the download page now?</p>")
                              .arg(curVer)
                              .arg(info.versionTag)
                              .arg(info.releaseName)
                              .arg(info.releaseNotes.left(400).toHtmlEscaped().replace("\n", "<br/>"));

            QMessageBox msgBox(m_parentWidget);
            msgBox.setWindowTitle(tr("NoteSciPad Update Available"));
            msgBox.setText(msg);
            msgBox.setIcon(QMessageBox::Information);
            const QAbstractButton *btnDownload = msgBox.addButton(tr("&Download Update"), QMessageBox::ActionRole);
            QAbstractButton *btnLater = msgBox.addButton(tr("&Remind Me Later"), QMessageBox::RejectRole);
            Q_UNUSED(btnLater);

            msgBox.exec();
            if (msgBox.clickedButton() == btnDownload) {
                QString url =
                    !info.htmlUrl.isEmpty() ? info.htmlUrl : "https://github.com/alsyundawy/NoteSciPad/releases";
                QDesktopServices::openUrl(QUrl(url));
            }
        }
    } else {
        if (!m_silent && m_parentWidget) {
            QMessageBox::information(m_parentWidget, tr("NoteSciPad Up to Date"),
                                     tr("<h3>You are running the latest version!</h3>"
                                        "<p><b>NoteSciPad version %1</b> is currently up to date.</p>")
                                         .arg(curVer));
        }
    }
}
