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

#ifndef SCREENDISPLAYMANAGER_H
#define SCREENDISPLAYMANAGER_H

#include <QObject>
#include <QPointer>
#include <QRect>
#include <QString>
#include <QList>

class QMainWindow;
class QWidget;
class QScreen;

struct DisplayInfo {
    int index = 0;
    QString name;
    QString model;
    QRect geometry;
    QRect availableGeometry;
    qreal devicePixelRatio = 1.0;
    qreal logicalDpi = 96.0;
    qreal physicalDpi = 96.0;
    qreal refreshRate = 60.0;
    int depth = 24;
    bool isPrimary = false;
    bool hasWindow = false;
    bool isRetina = false;
};

class ScreenDisplayManager : public QObject {
    Q_OBJECT

public:
    static ScreenDisplayManager &instance();

    void registerMainWindow(QMainWindow *window);
    void ensureWindowOnScreen(QWidget *window);
    void moveToNextScreen(QWidget *window);
    void moveToPreviousScreen(QWidget *window);
    void centerOnCurrentScreen(QWidget *window);
    void moveToPrimaryScreen(QWidget *window);

    QList<DisplayInfo> getConnectedDisplaysInfo(const QWidget *window = nullptr) const;
    void showDisplayInfoDialog(QWidget *parent);
    qreal currentDevicePixelRatio(const QWidget *window) const;

signals:
    void screenLayoutChanged();
    void devicePixelRatioChanged(qreal oldDpr, qreal newDpr);

private slots:
    void handleScreenAdded(QScreen *screen);
    void handleScreenRemoved(QScreen *screen);
    void handlePrimaryScreenChanged(QScreen *screen);

private:
    explicit ScreenDisplayManager(QObject *parent = nullptr);
    ~ScreenDisplayManager() override = default;
    ScreenDisplayManager(const ScreenDisplayManager &) = delete;
    ScreenDisplayManager &operator=(const ScreenDisplayManager &) = delete;

    void onScreenChangedForWindow(QScreen *screen);

    QPointer<QMainWindow> m_mainWindow = nullptr;
    qreal m_lastDpr = 1.0;
};

#endif  // SCREENDISPLAYMANAGER_H
