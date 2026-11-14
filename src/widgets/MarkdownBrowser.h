#pragma once
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

#include <QHash>
#include <QImage>
#include <QPointer>
#include <QSet>
#include <QTextBrowser>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

class MarkdownBrowser : public QTextBrowser {
    Q_OBJECT

public:
    explicit MarkdownBrowser(QWidget *parent = nullptr);
    ~MarkdownBrowser() override;

    void setDocumentSourcePath(const QString &filePath);
    void clearImageCache();

    [[nodiscard]] QVariant loadResource(int type, const QUrl &name) override;

signals:
    void remoteImageLoaded(const QUrl &url);

private slots:
    void handleImageReply(QNetworkReply *reply, const QUrl &url);

private:
    void fetchRemoteImage(const QUrl &url);
    [[nodiscard]] static QImage renderSvgToImage(const QByteArray &data, qreal devicePixelRatio);
    [[nodiscard]] static QImage renderSvgFileToImage(const QString &filePath, qreal devicePixelRatio);

    QNetworkAccessManager *m_networkManager = nullptr;
    QHash<QUrl, QImage> m_imageCache;
    QSet<QUrl> m_pendingUrls;
    QSet<QUrl> m_failedUrls;

    static constexpr int s_maxCacheEntries = 64;
    static constexpr int s_maxConcurrentDownloads = 8;
};
