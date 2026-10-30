/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ScreenDisplayManager.h"
#include "dialogs/DisplayInfoDialog.h"
#include "ScintillaNext.h"

#include <QGuiApplication>
#include <QScreen>
#include <QMainWindow>
#include <QWindow>
#include <QDebug>
#include <QtMath>

ScreenDisplayManager &ScreenDisplayManager::instance() {
    static ScreenDisplayManager inst;
    return inst;
}

ScreenDisplayManager::ScreenDisplayManager(QObject *parent) : QObject(parent) {
    connect(qApp, &QGuiApplication::screenAdded, this, &ScreenDisplayManager::handleScreenAdded);
    connect(qApp, &QGuiApplication::screenRemoved, this, &ScreenDisplayManager::handleScreenRemoved);
    connect(qApp, &QGuiApplication::primaryScreenChanged, this, &ScreenDisplayManager::handlePrimaryScreenChanged);
}

void ScreenDisplayManager::registerMainWindow(QMainWindow *window) {
    m_mainWindow = window;
    if (!window)
        return;

    m_lastDpr = window->devicePixelRatio();

    if (window->windowHandle()) {
        connect(window->windowHandle(), &QWindow::screenChanged, this, &ScreenDisplayManager::onScreenChangedForWindow);
    }
}

void ScreenDisplayManager::handleScreenAdded(QScreen *screen) {
    Q_UNUSED(screen);
    qInfo() << "[ScreenDisplayManager] New display detected:" << (screen ? screen->name() : "unknown");
    emit screenLayoutChanged();
}

void ScreenDisplayManager::handleScreenRemoved(QScreen *screen) {
    Q_UNUSED(screen);
    qInfo() << "[ScreenDisplayManager] Display disconnected. Validating window bounds...";
    if (m_mainWindow) {
        ensureWindowOnScreen(m_mainWindow);
    }
    emit screenLayoutChanged();
}

void ScreenDisplayManager::handlePrimaryScreenChanged(QScreen *screen) {
    Q_UNUSED(screen);
    qInfo() << "[ScreenDisplayManager] Primary display changed to:" << (screen ? screen->name() : "none");
    emit screenLayoutChanged();
}

void ScreenDisplayManager::onScreenChangedForWindow(QScreen *screen) {
    if (!screen)
        return;

    qreal dpr = screen->devicePixelRatio();
    qInfo() << "[ScreenDisplayManager] Window moved to screen:" << screen->name() << "DPR:" << dpr
            << "Refresh:" << screen->refreshRate() << "Hz";

    if (!qFuzzyCompare(dpr, m_lastDpr)) {
        qreal oldDpr = m_lastDpr;
        m_lastDpr = dpr;
        emit devicePixelRatioChanged(oldDpr, dpr);

        if (m_mainWindow) {
            m_mainWindow->update();
            const auto editors = m_mainWindow->findChildren<ScintillaNext *>();
            for (ScintillaNext *editor : editors) {
                if (editor) {
                    editor->update();
                }
            }
        }
    }
}

void ScreenDisplayManager::ensureWindowOnScreen(QWidget *window) {
    if (!window)
        return;

    const auto screens = QGuiApplication::screens();
    if (screens.isEmpty())
        return;

    QRect winGeo = window->geometry();
    bool visibleOnAnyScreen = false;

    for (QScreen *screen : screens) {
        QRect avail = screen->availableGeometry();
        QRect intersection = avail.intersected(winGeo);
        // Window is considered safely visible if at least 150x150 px intersects active display
        if (intersection.width() >= 150 && intersection.height() >= 150) {
            visibleOnAnyScreen = true;
            break;
        }
    }

    if (!visibleOnAnyScreen) {
        qWarning() << "[ScreenDisplayManager] Window is off-screen. Repositioning to primary display.";
        QScreen *primary = QGuiApplication::primaryScreen();
        if (primary) {
            QRect avail = primary->availableGeometry();
            int w = qMin(winGeo.width(), avail.width() - 40);
            int h = qMin(winGeo.height(), avail.height() - 40);
            if (w < 400)
                w = qMin(800, avail.width() - 40);
            if (h < 300)
                h = qMin(600, avail.height() - 40);
            int x = avail.x() + (avail.width() - w) / 2;
            int y = avail.y() + (avail.height() - h) / 2;
            window->setGeometry(x, y, w, h);
        }
    }
}

void ScreenDisplayManager::moveToNextScreen(QWidget *window) {
    if (!window)
        return;

    const auto screens = QGuiApplication::screens();
    if (screens.size() <= 1) {
        centerOnCurrentScreen(window);
        return;
    }

    QScreen *currentScreen = window->screen();
    if (!currentScreen) {
        currentScreen = QGuiApplication::screenAt(window->geometry().center());
    }

    int idx = screens.indexOf(currentScreen);
    int nextIdx = (idx + 1) % screens.size();
    QScreen *targetScreen = screens.at(nextIdx);

    QRect targetAvail = targetScreen->availableGeometry();
    int w = qMin(window->width(), targetAvail.width() - 40);
    int h = qMin(window->height(), targetAvail.height() - 40);
    int x = targetAvail.x() + (targetAvail.width() - w) / 2;
    int y = targetAvail.y() + (targetAvail.height() - h) / 2;

    window->setGeometry(x, y, w, h);
    window->raise();
    window->activateWindow();
}

void ScreenDisplayManager::moveToPreviousScreen(QWidget *window) {
    if (!window)
        return;

    const auto screens = QGuiApplication::screens();
    if (screens.size() <= 1) {
        centerOnCurrentScreen(window);
        return;
    }

    QScreen *currentScreen = window->screen();
    if (!currentScreen) {
        currentScreen = QGuiApplication::screenAt(window->geometry().center());
    }

    int idx = screens.indexOf(currentScreen);
    int prevIdx = (idx - 1 + screens.size()) % screens.size();
    QScreen *targetScreen = screens.at(prevIdx);

    QRect targetAvail = targetScreen->availableGeometry();
    int w = qMin(window->width(), targetAvail.width() - 40);
    int h = qMin(window->height(), targetAvail.height() - 40);
    int x = targetAvail.x() + (targetAvail.width() - w) / 2;
    int y = targetAvail.y() + (targetAvail.height() - h) / 2;

    window->setGeometry(x, y, w, h);
    window->raise();
    window->activateWindow();
}

void ScreenDisplayManager::centerOnCurrentScreen(QWidget *window) {
    if (!window)
        return;

    QScreen *scr = window->screen();
    if (!scr) {
        scr = QGuiApplication::screenAt(window->geometry().center());
    }
    if (!scr) {
        scr = QGuiApplication::primaryScreen();
    }
    if (!scr)
        return;

    QRect avail = scr->availableGeometry();
    int w = qMin(window->width(), avail.width() - 40);
    int h = qMin(window->height(), avail.height() - 40);
    int x = avail.x() + (avail.width() - w) / 2;
    int y = avail.y() + (avail.height() - h) / 2;

    window->setGeometry(x, y, w, h);
    window->raise();
    window->activateWindow();
}

void ScreenDisplayManager::moveToPrimaryScreen(QWidget *window) {
    if (!window)
        return;

    QScreen *primary = QGuiApplication::primaryScreen();
    if (!primary)
        return;

    QRect avail = primary->availableGeometry();
    int w = qMin(window->width(), avail.width() - 40);
    int h = qMin(window->height(), avail.height() - 40);
    int x = avail.x() + (avail.width() - w) / 2;
    int y = avail.y() + (avail.height() - h) / 2;

    window->setGeometry(x, y, w, h);
    window->raise();
    window->activateWindow();
}

qreal ScreenDisplayManager::currentDevicePixelRatio(const QWidget *window) const {
    if (window)
        return window->devicePixelRatio();
    const QScreen *primary = QGuiApplication::primaryScreen();
    return primary ? primary->devicePixelRatio() : 1.0;
}

QList<DisplayInfo> ScreenDisplayManager::getConnectedDisplaysInfo(const QWidget *window) const {
    QList<DisplayInfo> list;
    const auto screens = QGuiApplication::screens();
    const QScreen *activeScreen = window ? window->screen() : nullptr;
    if (!activeScreen && window) {
        activeScreen = QGuiApplication::screenAt(window->geometry().center());
    }
    const QScreen *primary = QGuiApplication::primaryScreen();

    for (int i = 0; i < screens.size(); ++i) {
        const QScreen *scr = screens.at(i);
        DisplayInfo info;
        info.index = i + 1;
        info.name = scr->name();
        info.model = scr->model();
        info.geometry = scr->geometry();
        info.availableGeometry = scr->availableGeometry();
        info.devicePixelRatio = scr->devicePixelRatio();
        info.logicalDpi = scr->logicalDotsPerInch();
        info.physicalDpi = scr->physicalDotsPerInch();
        info.refreshRate = scr->refreshRate();
        info.depth = scr->depth();
        info.isPrimary = (scr == primary);
        info.hasWindow = (scr == activeScreen);
        info.isRetina = (info.devicePixelRatio >= 1.5);
        list.append(info);
    }

    return list;
}

void ScreenDisplayManager::showDisplayInfoDialog(QWidget *parent) {
    DisplayInfoDialog dlg(parent);
    dlg.exec();
}
