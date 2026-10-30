#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Implements Transparent Window Mode & Window Opacity Manager.
 * Adapted from Notepad3 (rizonesoft/notepad3) window transparency architecture.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef TRANSPARENTWINDOWMANAGER_H
#define TRANSPARENTWINDOWMANAGER_H

#include <QObject>
#include <QMainWindow>
#include <QEvent>

class TransparentWindowManager : public QObject {
    Q_OBJECT

public:
    enum class TransparencyMode { Always = 0, OnlyWhenInactive = 1 };

    explicit TransparentWindowManager(QMainWindow *window, QObject *parent = nullptr);
    ~TransparentWindowManager() override;

    bool isEnabled() const;
    void setEnabled(bool enabled);
    void toggle();

    int opacity() const;  // 20..100
    void setOpacity(int percent);
    void increaseOpacity(int delta = 10);
    void decreaseOpacity(int delta = 10);

    TransparencyMode mode() const;
    void setMode(TransparencyMode targetMode);

    bool isAlwaysOnTop() const;
    void setAlwaysOnTop(bool onTop);

    void loadSettings();
    void saveSettings() const;

signals:
    void transparencyChanged(bool enabled, int opacity, TransparencyMode mode);
    void alwaysOnTopChanged(bool onTop);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void applyOpacity();
    void updateWindowFlags();

    QMainWindow *m_window = nullptr;
    bool m_enabled = false;
    int m_opacity = 75;
    TransparencyMode m_mode = TransparencyMode::Always;
    bool m_alwaysOnTop = false;
};

#endif  // TRANSPARENTWINDOWMANAGER_H
