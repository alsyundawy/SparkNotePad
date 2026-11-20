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
#include <QAbstractTextDocumentLayout>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QRegularExpression>
#include <QScrollBar>
#include <QtSvg/QSvgRenderer>
#include <QSvgRenderer>
#include <QWindow>

namespace {

QByteArray normalizeSvgForRendering(const QByteArray &data) {
    if (!data.contains("scale(.1)") && !data.contains("scale(0.1)")) {
        return data;
    }

    QString svg = QString::fromUtf8(data);
    svg.replace(QStringLiteral("font-size=\"100\""), QStringLiteral("font-size=\"10\""));
    svg.replace(QStringLiteral("font-size:100"), QStringLiteral("font-size:10"));
    svg.replace(QStringLiteral("font-family=\"Verdana,Geneva,DejaVu Sans,sans-serif\""),
                QStringLiteral("font-family=\"'Helvetica Neue', Helvetica, Arial, sans-serif\""));

    static const QRegularExpression textTagRegex(QStringLiteral(R"(<text\b([^>]*?)>)"),
                                                 QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression transformScaleRegex(QStringLiteral(R"(\s*transform\s*=\s*["']scale\(0?\.1\)["'])"),
                                                        QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression numAttrRegex(QStringLiteral(R"(\b(x|y|textLength)\s*=\s*["']([0-9.]+)["'])"),
                                                 QRegularExpression::CaseInsensitiveOption);

    auto matchIt = textTagRegex.globalMatch(svg);
    qsizetype offset = 0;
    while (matchIt.hasNext()) {
        const auto match = matchIt.next();
        QString tag = match.captured(0);
        tag.remove(transformScaleRegex);

        auto attrIt = numAttrRegex.globalMatch(tag);
        qsizetype innerOffset = 0;
        while (attrIt.hasNext()) {
            const auto am = attrIt.next();
            bool ok = false;
            const double val = am.captured(2).toDouble(&ok);
            if (ok) {
                const QString scaled =
                    QStringLiteral("%1=\"%2\"").arg(am.captured(1), QString::number(val * 0.1, 'f', 2));
                tag.replace(am.capturedStart() + innerOffset, am.capturedLength(), scaled);
                innerOffset += scaled.length() - am.capturedLength();
            }
        }

        const qsizetype start = match.capturedStart() + offset;
        const qsizetype len = match.capturedLength();
        svg.replace(start, len, tag);
        offset += tag.length() - len;
    }

    return svg.toUtf8();
}

}  // namespace

MarkdownBrowser::MarkdownBrowser(QWidget *parent)
    : QTextBrowser(parent), m_networkManager(new QNetworkAccessManager(this)), m_relayoutTimer(new QTimer(this)) {
    setOpenExternalLinks(false);
    setOpenLinks(false);
    m_relayoutTimer->setSingleShot(true);
    connect(m_relayoutTimer, &QTimer::timeout, this, &MarkdownBrowser::performDeferredRelayout);
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

void MarkdownBrowser::setDocumentHtml(const QString &html) {
    m_currentHtml = html;
    setHtml(html);
}

void MarkdownBrowser::performDeferredRelayout() {
    if (m_currentHtml.isEmpty()) {
        if (document()) {
            document()->markContentsDirty(0, document()->characterCount());
            document()->adjustSize();
            if (auto *layout = document()->documentLayout()) {
                emit layout->update();
            }
        }
        viewport()->update();
        return;
    }

    const int scrollVal = verticalScrollBar() ? verticalScrollBar()->value() : 0;
    setHtml(m_currentHtml);
    if (verticalScrollBar() && scrollVal > 0) {
        verticalScrollBar()->setValue(scrollVal);
    }
}

void MarkdownBrowser::clearImageCache() {
    m_imageCache.clear();
    m_pendingUrls.clear();
    m_failedUrls.clear();
    m_downloadQueue.clear();
}

QImage MarkdownBrowser::renderSvgToImage(const QByteArray &data, qreal devicePixelRatio) {
    const QByteArray cleanData = normalizeSvgForRendering(data);
    QSvgRenderer renderer(cleanData);
    if (!renderer.isValid()) {
        return QImage();
    }

    const QRectF viewBox = renderer.viewBoxF();
    qreal w = viewBox.width();
    qreal h = viewBox.height();
    if (w <= 0.0 || h <= 0.0) {
        const QSize ds = renderer.defaultSize();
        w = ds.width();
        h = ds.height();
    }
    if (w <= 0.0 || h <= 0.0) {
        w = 120.0;
        h = 28.0;
    }

    const qreal dpr = (devicePixelRatio > 0.0) ? devicePixelRatio : 1.0;
    const QSize pixelSize(std::max(1, static_cast<int>(std::ceil(w * dpr))),
                          std::max(1, static_cast<int>(std::ceil(h * dpr))));
    QImage image(pixelSize, QImage::Format_ARGB32_Premultiplied);
    image.setDevicePixelRatio(dpr);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setClipRect(QRectF(0.0, 0.0, w, h));
    renderer.render(&painter, QRectF(0.0, 0.0, w, h));
    return image;
}

QImage MarkdownBrowser::renderSvgFileToImage(const QString &filePath, qreal devicePixelRatio) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QImage();
    }
    const QByteArray rawData = file.readAll();
    file.close();
    return renderSvgToImage(rawData, devicePixelRatio);
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
                      QStringLiteral("SparkNotePad/1.5.0 (macOS; Universal; Scintilla/5.6.9)"));
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

    const int availableWidth = std::max(200, (viewport() ? viewport()->width() : 800) - 32);
    const int maxImgWidth = std::min(availableWidth, 850);
    if (image.width() > maxImgWidth && !isSvg) {
        image = image.scaledToWidth(maxImgWidth, Qt::SmoothTransformation);
    }

    if (m_imageCache.size() >= s_maxCacheEntries) {
        m_imageCache.erase(m_imageCache.begin());
    }
    m_imageCache.insert(url, image);

    document()->addResource(QTextDocument::ImageResource, url, image);
    document()->markContentsDirty(0, document()->characterCount());
    emit remoteImageLoaded(url);
    if (m_relayoutTimer) {
        m_relayoutTimer->start(60);
    }
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
            if (const auto it = m_imageCache.constFind(resolvedUrl); it != m_imageCache.constEnd()) {
                return QVariant::fromValue(*it);
            }
            if (localPath.endsWith(QStringLiteral(".svg"), Qt::CaseInsensitive)) {
                const QImage svgImg = renderSvgFileToImage(localPath, devicePixelRatioF());
                if (!svgImg.isNull()) {
                    if (m_imageCache.size() >= s_maxCacheEntries) {
                        m_imageCache.erase(m_imageCache.begin());
                    }
                    m_imageCache.insert(resolvedUrl, svgImg);
                    return QVariant::fromValue(svgImg);
                }
            } else {
                QImage img(localPath);
                if (!img.isNull()) {
                    const int availableWidth = std::max(200, (viewport() ? viewport()->width() : 800) - 32);
                    const int maxImgWidth = std::min(availableWidth, 850);
                    if (img.width() > maxImgWidth) {
                        img = img.scaledToWidth(maxImgWidth, Qt::SmoothTransformation);
                    }
                    if (m_imageCache.size() >= s_maxCacheEntries) {
                        m_imageCache.erase(m_imageCache.begin());
                    }
                    m_imageCache.insert(resolvedUrl, img);
                    return QVariant::fromValue(img);
                }
            }
        }
        return QTextBrowser::loadResource(type, resolvedUrl);
    }

    return QTextBrowser::loadResource(type, name);
}
