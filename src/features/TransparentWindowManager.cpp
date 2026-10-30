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

#include "TransparentWindowManager.h"

#include <QSettings>
#include <algorithm>

TransparentWindowManager::TransparentWindowManager(QMainWindow *window, QObject *parent)
    : QObject(parent), m_window(window) {
    if (m_window) {
        m_window->installEventFilter(this);
    }
    loadSettings();
}

TransparentWindowManager::~TransparentWindowManager() {
    saveSettings();
}

bool TransparentWindowManager::isEnabled() const {
    return m_enabled;
}

void TransparentWindowManager::setEnabled(bool enabled) {
    if (m_enabled != enabled) {
        m_enabled = enabled;
        applyOpacity();
        saveSettings();
        emit transparencyChanged(m_enabled, m_opacity, m_mode);
    }
}

void TransparentWindowManager::toggle() {
    setEnabled(!m_enabled);
}

int TransparentWindowManager::opacity() const {
    return m_opacity;
}

void TransparentWindowManager::setOpacity(int percent) {
    int clamped = std::clamp(percent, 20, 100);
    if (m_opacity != clamped) {
        m_opacity = clamped;
        if (m_enabled) {
            applyOpacity();
        }
        saveSettings();
        emit transparencyChanged(m_enabled, m_opacity, m_mode);
    }
}

void TransparentWindowManager::increaseOpacity(int delta) {
    setOpacity(m_opacity + delta);
}

void TransparentWindowManager::decreaseOpacity(int delta) {
    setOpacity(m_opacity - delta);
}

TransparentWindowManager::TransparencyMode TransparentWindowManager::mode() const {
    return m_mode;
}

void TransparentWindowManager::setMode(TransparencyMode targetMode) {
    if (m_mode != targetMode) {
        m_mode = targetMode;
        if (m_enabled) {
            applyOpacity();
        }
        saveSettings();
        emit transparencyChanged(m_enabled, m_opacity, m_mode);
    }
}

bool TransparentWindowManager::isAlwaysOnTop() const {
    return m_alwaysOnTop;
}

void TransparentWindowManager::setAlwaysOnTop(bool onTop) {
    if (m_alwaysOnTop != onTop) {
        m_alwaysOnTop = onTop;
        updateWindowFlags();
        saveSettings();
        emit alwaysOnTopChanged(m_alwaysOnTop);
    }
}

void TransparentWindowManager::applyOpacity() {
    if (!m_window)
        return;

    if (!m_enabled) {
        m_window->setWindowOpacity(1.0);
        return;
    }

    if (m_mode == TransparencyMode::OnlyWhenInactive) {
        if (m_window->isActiveWindow()) {
            m_window->setWindowOpacity(1.0);
        } else {
            m_window->setWindowOpacity(m_opacity * 0.01);
        }
    } else {
        m_window->setWindowOpacity(m_opacity * 0.01);
    }
}

void TransparentWindowManager::updateWindowFlags() {
    if (!m_window)
        return;

    Qt::WindowFlags flags = m_window->windowFlags();
    if (m_alwaysOnTop) {
        flags |= Qt::WindowStaysOnTopHint;
    } else {
        flags &= ~Qt::WindowStaysOnTopHint;
    }

    const bool wasVisible = m_window->isVisible();
    m_window->setWindowFlags(flags);
    if (wasVisible) {
        m_window->show();
    }
}

bool TransparentWindowManager::eventFilter(QObject *watched, QEvent *event) {
    if (watched == m_window && m_enabled && m_mode == TransparencyMode::OnlyWhenInactive) {
        if (event->type() == QEvent::WindowActivate || event->type() == QEvent::WindowDeactivate) {
            applyOpacity();
        }
    }
    return QObject::eventFilter(watched, event);
}

void TransparentWindowManager::loadSettings() {
    QSettings settings;
    settings.beginGroup(QStringLiteral("Transparency"));
    m_enabled = settings.value(QStringLiteral("Enabled"), false).toBool();
    m_opacity = settings.value(QStringLiteral("Opacity"), 75).toInt();
    int modeVal = settings.value(QStringLiteral("Mode"), static_cast<int>(TransparencyMode::Always)).toInt();
    m_mode = (modeVal == 1) ? TransparencyMode::OnlyWhenInactive : TransparencyMode::Always;
    m_alwaysOnTop = settings.value(QStringLiteral("AlwaysOnTop"), false).toBool();
    settings.endGroup();

    if (m_alwaysOnTop) {
        updateWindowFlags();
    }
    if (m_enabled) {
        applyOpacity();
    }
}

void TransparentWindowManager::saveSettings() const {
    QSettings settings;
    settings.beginGroup(QStringLiteral("Transparency"));
    settings.setValue(QStringLiteral("Enabled"), m_enabled);
    settings.setValue(QStringLiteral("Opacity"), m_opacity);
    settings.setValue(QStringLiteral("Mode"), static_cast<int>(m_mode));
    settings.setValue(QStringLiteral("AlwaysOnTop"), m_alwaysOnTop);
    settings.endGroup();
}
