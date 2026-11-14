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

#include "MarkdownBrowser.h"
#include <QFileInfo>
#include <QImageReader>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QSvgRenderer>
#include <QWindow>

MarkdownBrowser::MarkdownBrowser(QWidget *parent)
    : QTextBrowser(parent), m_networkManager(new QNetworkAccessManager(this)) {
    setOpenExternalLinks(false);
    setOpenLinks(false);
}

MarkdownBrowser::~MarkdownBrowser() = default;

void MarkdownBrowser::setDocumentSourcePath(const QString &filePath) {
    if (filePath.isEmpty()) {
        document()->setBaseUrl(QUrl());
        return;
    }

    const QFileInfo fi(filePath);
    if (fi.exists() || fi.isAbsolute()) {
        document()->setBaseUrl(QUrl::fromLocalFile(fi.absolutePath() + QLatin1Char('/')));
    } else {
        document()->setBaseUrl(QUrl());
    }
}

void MarkdownBrowser::clearImageCache() {
    m_imageCache.clear();
    m_pendingUrls.clear();
    m_failedUrls.clear();
    m_downloadQueue.clear();
}

QImage MarkdownBrowser::renderSvgToImage(const QByteArray &data, qreal devicePixelRatio) {
    QSvgRenderer renderer(data);
    if (!renderer.isValid()) {
        return QImage();
    }

    QSize defaultSize = renderer.defaultSize();
    if (defaultSize.isEmpty() || defaultSize.width() <= 0 || defaultSize.height() <= 0) {
        defaultSize = QSize(120, 28);
    }

    const qreal dpr = (devicePixelRatio > 0.0) ? devicePixelRatio : 1.0;
    const QSize targetSize = defaultSize * dpr;
    QImage image(targetSize, QImage::Format_ARGB32_Premultiplied);
    image.setDevicePixelRatio(dpr);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    renderer.render(&painter);
    return image;
}

QImage MarkdownBrowser::renderSvgFileToImage(const QString &filePath, qreal devicePixelRatio) {
    QSvgRenderer renderer(filePath);
    if (!renderer.isValid()) {
        return QImage();
    }

    QSize defaultSize = renderer.defaultSize();
    if (defaultSize.isEmpty() || defaultSize.width() <= 0 || defaultSize.height() <= 0) {
        defaultSize = QSize(120, 28);
    }

    const qreal dpr = (devicePixelRatio > 0.0) ? devicePixelRatio : 1.0;
    const QSize targetSize = defaultSize * dpr;
    QImage image(targetSize, QImage::Format_ARGB32_Premultiplied);
    image.setDevicePixelRatio(dpr);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    renderer.render(&painter);
    return image;
}

void MarkdownBrowser::fetchRemoteImage(const QUrl &url) {
    if (m_pendingUrls.contains(url) || m_failedUrls.contains(url)) {
        return;
    }

    if (m_pendingUrls.size() >= s_maxConcurrentDownloads) {
        if (!m_downloadQueue.contains(url)) {
            m_downloadQueue.enqueue(url);
        }
        return;
    }

    m_pendingUrls.insert(url);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QStringLiteral("SparkNotePad/1.4.7 (macOS; Universal; Scintilla/5.6.9)"));
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, url] { handleImageReply(reply, url); });
}

void MarkdownBrowser::processDownloadQueue() {
    while (!m_downloadQueue.isEmpty() && m_pendingUrls.size() < s_maxConcurrentDownloads) {
        const QUrl nextUrl = m_downloadQueue.dequeue();
        if (!m_imageCache.contains(nextUrl) && !m_failedUrls.contains(nextUrl) && !m_pendingUrls.contains(nextUrl)) {
            fetchRemoteImage(nextUrl);
        }
    }
}

void MarkdownBrowser::handleImageReply(QNetworkReply *reply, const QUrl &url) {
    m_pendingUrls.remove(url);

    if (reply->error() != QNetworkReply::NoError) {
        m_failedUrls.insert(url);
        reply->deleteLater();
        processDownloadQueue();
        return;
    }

    const QByteArray data = reply->readAll();
    reply->deleteLater();

    if (data.isEmpty()) {
        m_failedUrls.insert(url);
        processDownloadQueue();
        return;
    }

    const qreal dpr = devicePixelRatioF();
    QImage image;

    const QString path = url.path().toLower();
    const QByteArray trimmedHeader = data.left(256).trimmed().toLower();
    const bool isSvg =
        path.endsWith(QStringLiteral(".svg")) || trimmedHeader.contains("<svg") || trimmedHeader.contains("<?xml");

    if (isSvg) {
        image = renderSvgToImage(data, dpr);
    }

    if (image.isNull()) {
        (void)image.loadFromData(data);
    }

    if (image.isNull()) {
        m_failedUrls.insert(url);
        processDownloadQueue();
        return;
    }

    if (m_imageCache.size() >= s_maxCacheEntries) {
        m_imageCache.erase(m_imageCache.begin());
    }
    m_imageCache.insert(url, image);

    document()->addResource(QTextDocument::ImageResource, url, image);
    document()->markContentsDirty(0, document()->characterCount());
    emit remoteImageLoaded(url);
    viewport()->update();

    processDownloadQueue();
}

QVariant MarkdownBrowser::loadResource(int type, const QUrl &name) {
    if (type == QTextDocument::ImageResource) {
        const QString scheme = name.scheme().toLower();
        if (scheme == QStringLiteral("http") || scheme == QStringLiteral("https")) {
            if (const auto it = m_imageCache.constFind(name); it != m_imageCache.constEnd()) {
                return QVariant::fromValue(*it);
            }

            if (!m_failedUrls.contains(name)) {
                fetchRemoteImage(name);
            }
            return QVariant();
        }

        QUrl resolvedUrl = name;
        if (name.isRelative() && document()->baseUrl().isValid()) {
            resolvedUrl = document()->baseUrl().resolved(name);
        }

        if (resolvedUrl.isLocalFile()) {
            const QString localPath = resolvedUrl.toLocalFile();
            if (localPath.endsWith(QStringLiteral(".svg"), Qt::CaseInsensitive)) {
                if (const auto it = m_imageCache.constFind(resolvedUrl); it != m_imageCache.constEnd()) {
                    return QVariant::fromValue(*it);
                }
                const QImage svgImg = renderSvgFileToImage(localPath, devicePixelRatioF());
                if (!svgImg.isNull()) {
                    if (m_imageCache.size() >= s_maxCacheEntries) {
                        m_imageCache.erase(m_imageCache.begin());
                    }
                    m_imageCache.insert(resolvedUrl, svgImg);
                    return QVariant::fromValue(svgImg);
                }
            }
        }
        return QTextBrowser::loadResource(type, resolvedUrl);
    }

    return QTextBrowser::loadResource(type, name);
}
