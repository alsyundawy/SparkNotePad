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

#ifndef DOCUMENTATIONMANAGER_H
#define DOCUMENTATIONMANAGER_H

#include <QObject>
#include <QString>

class QWidget;

class DocumentationManager : public QObject {
    Q_OBJECT

public:
    static DocumentationManager &instance();

    void openUserGuide(QWidget *parent = nullptr);
    void openDeveloperGuide(QWidget *parent = nullptr);
    void openOnlineRepository(QWidget *parent = nullptr);
    void openKeyboardShortcutsGuide(QWidget *parent = nullptr);

    QString localDocumentationPath(const QString &docFilename) const;
    QString onlineDocumentationUrl(const QString &docFilename) const;

private:
    explicit DocumentationManager(QObject *parent = nullptr);
    ~DocumentationManager() override = default;
    DocumentationManager(const DocumentationManager &) = delete;
    DocumentationManager &operator=(const DocumentationManager &) = delete;

    void openUrlOrFile(const QString &filePath, const QString &fallbackUrl, QWidget *parent);
};

#endif  // DOCUMENTATIONMANAGER_H
