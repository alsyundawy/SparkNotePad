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

#ifndef MACSERVICESHANDLER_H
#define MACSERVICESHANDLER_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>

class SparkNotePadApplication;
class MainWindow;

class MacServicesHandler : public QObject {
    Q_OBJECT

public:
    static MacServicesHandler *instance();

    // Register macOS Services Provider with NSApp
    void initialize(SparkNotePadApplication *app);

    // Set current active window
    void setActiveWindow(MainWindow *window);

    // Trigger macOS native emoji & symbols palette
    static void showEmojiAndSymbolsPalette();
    static void postVoiceOverAnnouncement(const QString &text);
    static void activateApplication();

    // Handling external actions
    void handleOpenSelection(const QString &text);
    void handleAppendSelection(const QString &text);
    void handleOpenFiles(const QStringList &filePaths);

signals:
    void openSelectionRequested(const QString &text);
    void appendSelectionRequested(const QString &text);
    void openFilesRequested(const QStringList &filePaths);

private:
    explicit MacServicesHandler(QObject *parent = nullptr);
    ~MacServicesHandler() override;

    SparkNotePadApplication *m_app = nullptr;
    QPointer<MainWindow> m_activeWindow;
};

#endif  // MACSERVICESHANDLER_H
