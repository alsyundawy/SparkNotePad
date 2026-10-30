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

#ifndef MACSERVICESHANDLER_H
#define MACSERVICESHANDLER_H

#include <QObject>
#include <QString>
#include <QStringList>

class NotepadNextApplication;
class MainWindow;

class MacServicesHandler : public QObject {
    Q_OBJECT

public:
    static MacServicesHandler *instance();

    // Register macOS Services Provider with NSApp
    void initialize(NotepadNextApplication *app);

    // Set current active window
    void setActiveWindow(MainWindow *window);

    // Trigger macOS native emoji & symbols palette
    static void showEmojiAndSymbolsPalette();
    static void postVoiceOverAnnouncement(const QString &text);

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

    NotepadNextApplication *m_app = nullptr;
    MainWindow *m_activeWindow = nullptr;
};

#endif  // MACSERVICESHANDLER_H
