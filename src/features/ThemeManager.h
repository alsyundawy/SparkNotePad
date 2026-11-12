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

#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <QColor>
#include <QHash>
#include <QList>
#include <QObject>
#include <QPointer>
#include <QString>
#include "ThemePaletteData.h"

class MainWindow;
class ScintillaNext;
class ApplicationSettings;

class ThemeManager : public QObject {
    Q_OBJECT

public:
    enum class ThemeMode : std::uint8_t { Auto = 0, Light = 1, Dark = 2 };
    Q_ENUM(ThemeMode)

    // Helper to construct Scintilla BGR colour from RGB components
    static constexpr quint32 scColor(int red, int green, int blue) {
        return static_cast<quint32>((red & 0xFF) | ((green & 0xFF) << 8) | ((blue & 0xFF) << 16));
    }
    static quint32 scFromRgb(QRgb rgb) { return scColor(qRed(rgb), qGreen(rgb), qBlue(rgb)); }
    static quint32 scFromHex(const char *hex) {
        const QColor c(hex);
        return scFromRgb(c.rgb());
    }

    // WCAG 2.1 Contrast & Luminance calculations for safe syntax styling
    static double relativeLuminance(const QColor &c) {
        const auto channel = [](double val) {
            return (val <= 0.03928) ? (val / 12.92) : std::pow((val + 0.055) / 1.055, 2.4);
        };
        return (0.2126 * channel(c.redF())) + (0.7152 * channel(c.greenF())) + (0.0722 * channel(c.blueF()));
    }

    static double contrastRatio(const QColor &c1, const QColor &c2) {
        double l1 = relativeLuminance(c1);
        double l2 = relativeLuminance(c2);
        if (l1 < l2) {
            std::swap(l1, l2);
        }
        return (l1 + 0.05) / (l2 + 0.05);
    }

    static QColor ensureContrast(const QColor &fg, const QColor &bg, bool isDark, double minRatio = 3.2) {
        if (contrastRatio(fg, bg) >= minRatio) {
            return fg;
        }
        QColor adjusted = fg;
        for (int step = 0; step < 12; ++step) {
            if (isDark) {
                adjusted = adjusted.lighter(115);
            } else {
                adjusted = adjusted.darker(115);
            }
            if (contrastRatio(adjusted, bg) >= minRatio) {
                return adjusted;
            }
        }
        return isDark ? QColor(245, 245, 250) : QColor(20, 24, 30);
    }

    static ThemeManager *instance();

    void initialize(ApplicationSettings *settings);

    ThemeMode themeMode() const { return m_themeMode; }
    void setThemeMode(ThemeMode mode, MainWindow *window = nullptr);
    void toggleTheme(MainWindow *window = nullptr);

    bool isDarkActive() const;

    const QString &currentPaletteId() const { return m_currentPaletteId; }
    const ThemePalette *currentPalette() const;
    void setPalette(const QString &paletteId, MainWindow *window = nullptr);

    void applyTheme(MainWindow *window);
    void applyThemeToEditor(ScintillaNext *editor) const;
    QString getThemeStyleSheet() const;

signals:
    void themeChanged(bool isDark);
    void paletteChanged(const QString &paletteId);

private slots:
    void onSystemColorSchemeChanged();

private:
    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager() override = default;

    ThemeMode m_themeMode = ThemeMode::Light;
    QString m_currentPaletteId = QStringLiteral("sparkNotepad");
    ApplicationSettings *m_settings = nullptr;
    QPointer<MainWindow> m_activeWindow;
    mutable QHash<QString, QString> m_styleSheetCache;
};

#endif  // THEMEMANAGER_H
