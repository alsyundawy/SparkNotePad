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

#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QColor>

class MainWindow;
class ScintillaNext;
class ApplicationSettings;

struct ThemePalette {
    const char *id;
    const char *name;
    bool isDark;
    const char *win;
    const char *card;
    const char *border;
    const char *txtPri;
    const char *txtSec;
    const char *cyan;
    const char *blue;
    const char *net;
    const char *sub;
    const char *host;
    const char *sep;
};

class ThemeManager : public QObject {
    Q_OBJECT

public:
    enum class ThemeMode { Auto = 0, Light = 1, Dark = 2 };
    Q_ENUM(ThemeMode)

    // Helper to construct Scintilla BGR colour from RGB components
    static constexpr quint32 scColor(int red, int green, int blue) {
        return static_cast<quint32>((red & 0xFF) | ((green & 0xFF) << 8) | ((blue & 0xFF) << 16));
    }
    static inline quint32 scFromRgb(QRgb rgb) { return scColor(qRed(rgb), qGreen(rgb), qBlue(rgb)); }
    static inline quint32 scFromHex(const char *hex) {
        QColor c(hex);
        return scFromRgb(c.rgb());
    }

    static ThemeManager *instance();

    void initialize(ApplicationSettings *settings);

    ThemeMode themeMode() const { return m_themeMode; }
    void setThemeMode(ThemeMode mode, MainWindow *window = nullptr);
    void toggleTheme(MainWindow *window = nullptr);

    bool isDarkActive() const;

    const ThemePalette *currentPalette() const;

    void applyTheme(MainWindow *window);
    void applyThemeToEditor(ScintillaNext *editor);
    QString getThemeStyleSheet() const;

signals:
    void themeChanged(bool isDark);

private slots:
    void onSystemColorSchemeChanged();

private:
    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager() override;

    ThemeMode m_themeMode = ThemeMode::Light;
    ApplicationSettings *m_settings = nullptr;
    MainWindow *m_activeWindow = nullptr;
};

#endif  // THEMEMANAGER_H
