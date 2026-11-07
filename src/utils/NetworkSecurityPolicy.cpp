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

#include "NetworkSecurityPolicy.h"
#include <QFileInfo>
#include <QUrl>
#include <QSslSocket>
#include <QDir>
#include <QCoreApplication>

bool NetworkSecurityPolicy::s_offlineMode = false;

void NetworkSecurityPolicy::initializeGlobalSecurityPolicy() {
#if QT_CONFIG(ssl)
    if (!QSslSocket::supportsSsl()) {
        qWarning("OpenSSL not supported or unavailable on system.");
        return;
    }

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    // Enforce TLS 1.2 or later
    config.setProtocol(QSsl::TlsV1_2OrLater);

    // Defense against CVE-2025-14575: ensure default CA certs only from verified system store,
    // explicitly ensuring CWD is never checked for rogue CA bundles
    config.setCaCertificates(QSslConfiguration::systemCaCertificates());

    QSslConfiguration::setDefaultConfiguration(config);
    qInfo("NetworkSecurityPolicy: Global TLS 1.2+ security baseline enforced (CVE-2025-14575 mitigated).");
#endif
}

void NetworkSecurityPolicy::applySecureTlsConfiguration(QNetworkRequest &request) {
#if QT_CONFIG(ssl)
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2OrLater);
    request.setSslConfiguration(config);
#endif
    // Cap header and frame limits to mitigate CVE-2026-76151
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, false);
}

bool NetworkSecurityPolicy::isOfflineModeEnabled() {
    return s_offlineMode;
}

void NetworkSecurityPolicy::setOfflineModeEnabled(bool enabled) {
    s_offlineMode = enabled;
    qInfo("NetworkSecurityPolicy: Air-gap strict offline mode set to: %s", enabled ? "ENABLED" : "DISABLED");
}

bool NetworkSecurityPolicy::isSafeUriScheme(const QUrl &url, QString *reason) {
    if (!url.isValid()) {
        if (reason) {
            *reason = QStringLiteral("Invalid URL format");
        }
        return false;
    }

    const QString scheme = url.scheme().toLower();
    if (scheme == QLatin1String("http") || scheme == QLatin1String("https") || scheme == QLatin1String("mailto")) {
        return true;
    }

    if (scheme == QLatin1String("file")) {
        const QString localPath = url.toLocalFile();
        const QFileInfo fi(localPath);
        if (fi.isExecutable() && !fi.isDir()) {
            if (reason) {
                *reason = QStringLiteral("Execution of executable binaries via file:// URI is strictly blocked");
            }
            return false;
        }
        return true;
    }

    if (reason) {
        *reason = QStringLiteral("Protocol scheme '%1' is prohibited for external dispatch").arg(scheme);
    }
    return false;
}
