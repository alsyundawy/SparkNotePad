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

#include "CorrespondingFileSwitcher.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <algorithm>

namespace {

QStringList jsTsCandidates(const QString &dir, const QString &base, const QString &ext) {
    QStringList candidates;
    if (base.endsWith(QStringLiteral(".spec")) || base.endsWith(QStringLiteral(".test"))) {
        const QString stripped = base.left(base.lastIndexOf(QLatin1Char('.')));
        candidates << QDir(dir).filePath(stripped + QLatin1Char('.') + ext);
    } else if (ext == QStringLiteral("ts") || ext == QStringLiteral("js") || ext == QStringLiteral("jsx") ||
               ext == QStringLiteral("tsx")) {
        candidates << QDir(dir).filePath(base + QStringLiteral(".spec.") + ext);
        candidates << QDir(dir).filePath(base + QStringLiteral(".test.") + ext);
    }
    return candidates;
}

QStringList collectSearchDirs(const QString &dir) {
    QStringList searchDirs;
    searchDirs << dir;

    QDir d(dir);
    if (const QString dirName = d.dirName().toLower();
        dirName == QStringLiteral("src") || dirName == QStringLiteral("source")) {
        (void)d.cdUp();
        if (d.cd(QStringLiteral("include"))) {
            searchDirs << d.absolutePath();
            (void)d.cdUp();
        }
        if (d.cd(QStringLiteral("inc"))) {
            searchDirs << d.absolutePath();
            (void)d.cdUp();
        }
        if (d.cd(QStringLiteral("headers"))) {
            searchDirs << d.absolutePath();
            (void)d.cdUp();
        }
    } else if (dirName == QStringLiteral("include") || dirName == QStringLiteral("inc") ||
               dirName == QStringLiteral("headers") || dirName == QStringLiteral("header")) {
        (void)d.cdUp();
        if (d.cd(QStringLiteral("src"))) {
            searchDirs << d.absolutePath();
            (void)d.cdUp();
        }
        if (d.cd(QStringLiteral("source"))) {
            searchDirs << d.absolutePath();
            (void)d.cdUp();
        }
    }

    return searchDirs;
}

QStringList cppCandidates(const QString &dir, const QString &base, const QString &ext) {
    static const QStringList headerExts = {QStringLiteral("h"), QStringLiteral("hpp"), QStringLiteral("hxx"),
                                           QStringLiteral("hh")};
    static const QStringList sourceExts = {QStringLiteral("cpp"), QStringLiteral("c"), QStringLiteral("cc"),
                                           QStringLiteral("cxx"), QStringLiteral("m"), QStringLiteral("mm")};

    const bool isHeader = headerExts.contains(ext);
    if (const bool isSource = sourceExts.contains(ext); isHeader || isSource) {
        const QStringList &targetExts = isHeader ? sourceExts : headerExts;
        const QStringList searchDirs = collectSearchDirs(dir);

        QStringList candidates;
        for (const auto &sDir : searchDirs) {
            for (const auto &tExt : targetExts) {
                candidates << QDir(sDir).filePath(base + QLatin1Char('.') + tExt);
            }
        }
        return candidates;
    }
    return {};
}

}  // namespace

QStringList CorrespondingFileSwitcher::candidatePaths(const QString &currentPath) {
    if (currentPath.isEmpty()) {
        return {};
    }

    const QFileInfo fi(currentPath);
    const QString dir = fi.absolutePath();
    const QString base = fi.completeBaseName();
    const QString ext = fi.suffix().toLower();

    QStringList candidates = jsTsCandidates(dir, base, ext);
    candidates.append(cppCandidates(dir, base, ext));
    return candidates;
}

QString CorrespondingFileSwitcher::findCorrespondingPath(const QString &currentPath) {
    const QStringList candidates = candidatePaths(currentPath);
    const auto it =
        std::find_if(candidates.cbegin(), candidates.cend(), [](const QString &cand) { return QFile::exists(cand); });
    if (it != candidates.cend()) {
        return QDir::cleanPath(*it);
    }
    return QString();
}
