#pragma once
/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef DOCUMENTATIONMANAGER_H
#define DOCUMENTATIONMANAGER_H

#include <QObject>
#include <QString>

class QWidget;

class DocumentationManager : public QObject {
    Q_OBJECT

public:
    static DocumentationManager &instance();

    void openUserGuide(QWidget *parent = nullptr) const;
    void openDeveloperGuide(QWidget *parent = nullptr) const;
    static void openOnlineRepository(const QWidget *parent = nullptr);
    static void openKeyboardShortcutsGuide(QWidget *parent = nullptr);

    static QString localDocumentationPath(const QString &docFilename);
    static QString onlineDocumentationUrl(const QString &docFilename);

    QString docnotePlaintextPath() const;
    QString changelogPlaintextPath() const;

private:
    explicit DocumentationManager(QObject *parent = nullptr);
    ~DocumentationManager() override = default;
    DocumentationManager(const DocumentationManager &) = delete;
    DocumentationManager &operator=(const DocumentationManager &) = delete;

    static void openUrlOrFile(const QString &filePath, const QString &fallbackUrl, QWidget *parent);
};

#endif  // DOCUMENTATIONMANAGER_H
