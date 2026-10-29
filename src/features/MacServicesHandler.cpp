/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "MacServicesHandler.h"
#include "dialogs/MainWindow.h"
#include "SparkNotePadApplication.h"

MacServicesHandler *MacServicesHandler::instance() {
    static MacServicesHandler s_instance;
    return &s_instance;
}

MacServicesHandler::MacServicesHandler(QObject *parent) : QObject(parent) {}

MacServicesHandler::~MacServicesHandler() {}

void MacServicesHandler::initialize(SparkNotePadApplication *app) {
    m_app = app;
}

void MacServicesHandler::setActiveWindow(MainWindow *window) {
    m_activeWindow = window;
}

void MacServicesHandler::showEmojiAndSymbolsPalette() {
    // Native macOS character palette is handled by Cocoa; Linux uses standard Compose/IBus
}

void MacServicesHandler::postVoiceOverAnnouncement(const QString &text) {
    Q_UNUSED(text);
}

void MacServicesHandler::makeWidgetWindowTranslucent(const QWidget *w) {
    Q_UNUSED(w);
}

void MacServicesHandler::handleOpenSelection(const QString &text) {
    emit openSelectionRequested(text);
}

void MacServicesHandler::handleAppendSelection(const QString &text) {
    emit appendSelectionRequested(text);
}

void MacServicesHandler::handleOpenFiles(const QStringList &filePaths) {
    emit openFilesRequested(filePaths);
}
