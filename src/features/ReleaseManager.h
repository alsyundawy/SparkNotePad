#pragma once
/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef RELEASEMANAGER_H
#define RELEASEMANAGER_H

#include <QObject>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;
class QWidget;

struct ReleaseInfo {
    QString versionTag;
    QString releaseName;
    QString releaseNotes;
    QString htmlUrl;
    QString publishedAt;
    bool isPrerelease = false;
};

class ReleaseManager : public QObject {
    Q_OBJECT

public:
    static ReleaseManager &instance();

    void checkForUpdates(QWidget *parent = nullptr, bool silent = false);
    QString currentVersion() const;
    bool isNewerVersion(const QString &latestVersion, const QString &currentVersion) const;

signals:
    void updateCheckFinished(bool updateAvailable, const ReleaseInfo &info);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    explicit ReleaseManager(QObject *parent = nullptr);
    ~ReleaseManager() override = default;
    ReleaseManager(const ReleaseManager &) = delete;
    ReleaseManager &operator=(const ReleaseManager &) = delete;

    QNetworkAccessManager *m_netManager = nullptr;
    QWidget *m_parentWidget = nullptr;
    bool m_silent = false;
};

#endif  // RELEASEMANAGER_H
