#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <QColor>
#include <QString>
#include <QStringList>
#include <cstdint>

class MarkdownThemeManager {
public:
    enum class ThemeId : std::uint8_t {
        GitHub = 0,
        GitHubDark,
        Almond,
        Awsm,
        Axist,
        Bamboo,
        Bullframe,
        Holiday,
        Kacit,
        Latex,
        Marx,
        Mini,
        Modest,
        NewTheme,
        NoClass,
        Pico,
        Retro,
        Sakura,
        SakuraVader,
        Semantic,
        Simple,
        StyleSans,
        StyleSerif,
        Stylize,
        Superstylin,
        Tacit,
        Vanilla,
        Water,
        WaterDark,
        Writ
    };

    struct ThemeColors {
        const char *name;
        const char *textColor;
        const char *bgColor;
        const char *borderColor;
        const char *mutedColor;
        const char *codeBg;
        const char *blockBg;
        const char *linkColor;
        const char *fontFamily;
        const char *headingBorderColor;
        bool isDark;
    };

    static QStringList themeNames();
    static QString themeName(ThemeId id);
    static ThemeId themeFromName(const QString &name);
    static bool isThemeDark(ThemeId id);
    static QColor themeBackgroundColor(ThemeId id);
    static QColor themeTextColor(ThemeId id);
    static QString generateThemeCss(ThemeId id, int baseFontSize = 13);
    static ThemeId defaultTheme(bool isDark);
    static const ThemeColors &themeColors(ThemeId id);
};
