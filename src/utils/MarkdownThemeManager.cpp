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

#include "MarkdownThemeManager.h"
#include "MarkdownPreviewEnhancer.h"
#include <algorithm>
#include <array>

namespace {

using ThemeId = MarkdownThemeManager::ThemeId;
using ThemeColors = MarkdownThemeManager::ThemeColors;

constexpr std::array<ThemeColors, 30> s_themes = {{
    // 0: GITHUB
    {"GITHUB", "#1f2328", "#ffffff", "#d0d7de", "#656d76", "rgba(175, 184, 193, 0.2)", "#f6f8fa", "#0969da",
     "-apple-system, BlinkMacSystemFont, 'Segoe UI', 'Noto Sans', Helvetica, Arial, sans-serif", "#d0d7de", false},
    // 1: GITHUB-DARK
    {"GITHUB-DARK", "#e6edf3", "#0d1117", "#30363d", "#848d97", "rgba(110, 118, 129, 0.4)", "#161b22", "#2f81f7",
     "-apple-system, BlinkMacSystemFont, 'Segoe UI', 'Noto Sans', Helvetica, Arial, sans-serif", "#30363d", true},
    // 2: ALMOND
    {"ALMOND", "#333333", "#fcfcfc", "#e5e5e5", "#666666", "#f0f0f0", "#f4f4f4", "#007acc",
     "system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif", "#e5e5e5", false},
    // 3: AWSM
    {"AWSM", "#222222", "#ffffff", "#dcdcdc", "#777777", "#f5f5f5", "#fbfbfb", "#b22222",
     "Georgia, Cambria, 'Times New Roman', Times, serif", "#b22222", false},
    // 4: AXIST
    {"AXIST", "#111111", "#ffffff", "#222222", "#555555", "#f3f3f3", "#fafafa", "#0000ee",
     "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif", "#222222", false},
    // 5: BAMBOO
    {"BAMBOO", "#2b303a", "#fdfdfd", "#a3c9a8", "#6b7280", "#edf7ed", "#f1f8e9", "#2e7d32",
     "'Noto Sans', -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif", "#a3c9a8", false},
    // 6: BULLFRAME
    {"BULLFRAME", "#1a202c", "#ffffff", "#cbd5e0", "#718096", "#edf2f7", "#f7fafc", "#3f51b5",
     "'Inter', 'Segoe UI', Helvetica, Arial, sans-serif", "#cbd5e0", false},
    // 7: HOLIDAY
    {"HOLIDAY", "#2c3e50", "#fffdf9", "#fcd34d", "#78716c", "#fef3c7", "#fffbeb", "#d97706",
     "'Century Gothic', 'Segoe UI', sans-serif", "#fcd34d", false},
    // 8: KACIT
    {"KACIT", "#2c3e50", "#ffffff", "#bdc3c7", "#7f8c8d", "#ecf0f1", "#f8f9fa", "#2980b9",
     "'Segoe UI', Helvetica, Arial, sans-serif", "#bdc3c7", false},
    // 9: LATEX
    {"LATEX", "#000000", "#ffffff", "#000000", "#444444", "#f4f4f4", "#fbfbfb", "#0000cc",
     "'Latin Modern Roman', 'Computer Modern', 'Times New Roman', Times, serif", "#000000", false},
    // 10: MARX
    {"MARX", "#111111", "#ffffff", "#eaeaea", "#666666", "#f3f3f3", "#f7f7f7", "#0076df",
     "Georgia, 'Times New Roman', serif", "#eaeaea", false},
    // 11: MINI
    {"MINI", "#212121", "#ffffff", "#e0e0e0", "#757575", "#f5f5f5", "#fafafa", "#1976d2",
     "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif", "#e0e0e0", false},
    // 12: MODEST
    {"MODEST", "#444444", "#fcfcfc", "#e0e0e0", "#888888", "#f5f5f5", "#f9f9f9", "#16a085",
     "'Open Sans', 'Segoe UI', sans-serif", "#e0e0e0", false},
    // 13: NEW
    {"NEW", "#222222", "#ffffff", "#cccccc", "#666666", "#f4f4f4", "#f9f9f9", "#0066cc",
     "system-ui, -apple-system, sans-serif", "#cccccc", false},
    // 14: NO-CLASS
    {"NO-CLASS", "#222222", "#ffffff", "#dddddd", "#777777", "#f2f2f2", "#f8f8f8", "#1b6acb",
     "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif", "#dddddd", false},
    // 15: PICO
    {"PICO", "#374151", "#ffffff", "#e5e7eb", "#6b7280", "#f3f4f6", "#f9fafb", "#0172ad",
     "system-ui, -apple-system, 'Segoe UI', Roboto, sans-serif", "#e5e7eb", false},
    // 16: RETRO
    {"RETRO", "#00ff66", "#121212", "#00aa44", "#008833", "#1a1a1a", "#0d0d0d", "#33ff99",
     "'Courier New', Courier, 'Lucida Console', monospace", "#00aa44", true},
    // 17: SAKURA
    {"SAKURA", "#222222", "#fbf7f8", "#f3d1dc", "#777777", "#fae8ee", "#fdf2f5", "#d63384",
     "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif", "#f3d1dc", false},
    // 18: SAKURA-VADER
    {"SAKURA-VADER", "#e0e0e0", "#181518", "#4a2c3a", "#a08a95", "#281e25", "#201820", "#ff79c6",
     "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif", "#4a2c3a", true},
    // 19: SEMANTIC
    {"SEMANTIC", "#2c3e50", "#ffffff", "#d4d4d5", "#767676", "#f8f8f9", "#f3f4f5", "#2185d0",
     "'Segoe UI', Helvetica, Arial, sans-serif", "#d4d4d5", false},
    // 20: SIMPLE
    {"SIMPLE", "#212529", "#fffff8", "#e9ecef", "#6c757d", "#f8f9fa", "#f1f3f5", "#d9534f",
     "system-ui, -apple-system, 'Segoe UI', Roboto, sans-serif", "#e9ecef", false},
    // 21: STYLE-SANS
    {"STYLE-SANS", "#111111", "#ffffff", "#cccccc", "#666666", "#eeeeee", "#f6f6f6", "#0066cc",
     "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif", "#cccccc", false},
    // 22: STYLE-SERIF
    {"STYLE-SERIF", "#2b2b2b", "#faf8f5", "#d8d2c7", "#6e675f", "#f0ece4", "#f5f0e8", "#8b0000",
     "'Merriweather', 'Georgia', 'Garamond', serif", "#d8d2c7", false},
    // 23: STYLIZE
    {"STYLIZE", "#1e1e2f", "#ffffff", "#e2e8f0", "#64748b", "#f1f5f9", "#f8fafc", "#7c3aed",
     "'Poppins', 'Inter', sans-serif", "#e2e8f0", false},
    // 24: SUPERSTYLIN
    {"SUPERSTYLIN", "#1a202c", "#ffffff", "#b2dfdb", "#546e7a", "#e0f2f1", "#e8f5e9", "#00897b",
     "'Ubuntu', 'Segoe UI', sans-serif", "#b2dfdb", false},
    // 25: TACIT
    {"TACIT", "#333333", "#ffffff", "#dcdcdc", "#777777", "#f3f3f3", "#fbfbfb", "#1b98f8",
     "-apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif", "#dcdcdc", false},
    // 26: VANILLA
    {"VANILLA", "#000000", "#ffffff", "#dddddd", "#555555", "#f0f0f0", "#fafafa", "#0000ee",
     "system-ui, sans-serif", "#dddddd", false},
    // 27: WATER
    {"WATER", "#212121", "#fefefe", "#e0e0e0", "#616161", "#f1f1f1", "#f7f7f7", "#0076df",
     "system-ui, -apple-system, BlinkMacSystemFont, sans-serif", "#e0e0e0", false},
    // 28: WATER-DARK
    {"WATER-DARK", "#dbdbdb", "#202b38", "#364659", "#8ba2b9", "#161f27", "#1a232e", "#41adff",
     "system-ui, -apple-system, BlinkMacSystemFont, sans-serif", "#364659", true},
    // 29: WRIT
    {"WRIT", "#1f2421", "#fefefa", "#dce1de", "#5b655f", "#f0f2f0", "#f5f7f5", "#216869",
     "'Palatino', 'Palatino Linotype', 'Georgia', serif", "#dce1de", false},
}};

}  // namespace

QStringList MarkdownThemeManager::themeNames() {
    QStringList names;
    names.reserve(static_cast<qsizetype>(s_themes.size()));
    std::transform(s_themes.begin(), s_themes.end(), std::back_inserter(names),
                   [](const ThemeColors &t) { return QString::fromLatin1(t.name); });
    return names;
}

QString MarkdownThemeManager::themeName(ThemeId id) {
    const auto idx = static_cast<std::size_t>(id);
    if (idx < s_themes.size()) {
        return QString::fromLatin1(s_themes[idx].name);
    }
    return QStringLiteral("GITHUB");
}

ThemeId MarkdownThemeManager::themeFromName(const QString &name) {
    const QString upper = name.trimmed().toUpper();
    for (std::size_t i = 0; i < s_themes.size(); ++i) {
        if (upper == QLatin1String(s_themes[i].name)) {
            return static_cast<ThemeId>(i);
        }
    }
    return ThemeId::GitHub;
}

bool MarkdownThemeManager::isThemeDark(ThemeId id) {
    const auto idx = static_cast<std::size_t>(id);
    if (idx < s_themes.size()) {
        return s_themes[idx].isDark;
    }
    return false;
}

const ThemeColors &MarkdownThemeManager::themeColors(ThemeId id) {
    const auto idx = static_cast<std::size_t>(id);
    if (idx < s_themes.size()) {
        return s_themes[idx];
    }
    return s_themes[0];
}

QColor MarkdownThemeManager::themeBackgroundColor(ThemeId id) {
    return QColor(themeColors(id).bgColor);
}

QColor MarkdownThemeManager::themeTextColor(ThemeId id) {
    return QColor(themeColors(id).textColor);
}

MarkdownThemeManager::ThemeId MarkdownThemeManager::defaultTheme(bool isDark) {
    return isDark ? ThemeId::GitHubDark : ThemeId::GitHub;
}

QString MarkdownThemeManager::generateThemeCss(ThemeId id, int baseFontSize) {
    const ThemeColors &c = themeColors(id);
    const QString sizeStr = QString::number(baseFontSize > 0 ? baseFontSize : 13);
    const QString textColor = QString::fromLatin1(c.textColor);
    const QString bgColor = QString::fromLatin1(c.bgColor);
    const QString borderColor = QString::fromLatin1(c.borderColor);
    const QString mutedColor = QString::fromLatin1(c.mutedColor);
    const QString codeBg = QString::fromLatin1(c.codeBg);
    const QString blockBg = QString::fromLatin1(c.blockBg);
    const QString linkColor = QString::fromLatin1(c.linkColor);
    const QString fontFamily = QString::fromLatin1(c.fontFamily);
    const QString hBorder = QString::fromLatin1(c.headingBorderColor);

    const QString baseCss = QStringLiteral(
                                "body { "
                                "  font-family: %1; "
                                "  font-size: %2px; line-height: 1.6; color: %3; background-color: %4; margin: 0; padding: 18px 24px; "
                                "} "
                                "h1, h2, h3, h4, h5, h6 { "
                                "  font-family: %1; font-weight: 600; line-height: 1.25; margin-top: 24px; margin-bottom: 16px; color: %3; "
                                "} "
                                "h1 { font-size: 2em; padding-bottom: 0.3em; border-bottom: 1px solid %5; } "
                                "h2 { font-size: 1.5em; padding-bottom: 0.3em; border-bottom: 1px solid %5; } "
                                "h3 { font-size: 1.25em; } "
                                "h4 { font-size: 1em; } "
                                "h5 { font-size: 0.875em; } "
                                "h6 { font-size: 0.85em; color: %6; } "
                                "p { margin-top: 0; margin-bottom: 16px; line-height: 1.6; } "
                                "blockquote { "
                                "  margin: 0 0 16px 0; padding: 6px 16px; color: %6; border-left: 4px solid %7; background-color: %8; "
                                "  border-radius: 0 4px 4px 0; "
                                "} "
                                "ul, ol { margin-top: 0; margin-bottom: 16px; padding-left: 2em; } "
                                "li { margin-top: 0.3em; } "
                                "code { "
                                "  font-family: 'JetBrains Mono', 'SFMono-Regular', Consolas, 'Liberation Mono', Menlo, monospace; "
                                "  font-size: 85%; padding: 0.2em 0.4em; background-color: %9; border-radius: 4px; "
                                "} "
                                "pre { "
                                "  margin-top: 0; margin-bottom: 16px; padding: 14px; line-height: 1.45; "
                                "  background-color: %8; border: 1px solid %7; border-radius: 6px; "
                                "  font-family: 'JetBrains Mono', 'SFMono-Regular', Consolas, 'Liberation Mono', Menlo, monospace; font-size: 85%; "
                                "} "
                                "table { border-collapse: collapse; border-spacing: 0; margin-top: 0; margin-bottom: 16px; width: 100%; } "
                                "th, td { padding: 8px 14px; border: 1px solid %7; } "
                                "th { font-weight: 600; background-color: %8; } "
                                "tr:nth-child(2n) { background-color: %8; } "
                                "hr { height: 2px; padding: 0; margin: 24px 0; background-color: %7; border: 0; } "
                                "a { color: %10; text-decoration: none; font-weight: 500; } "
                                "kbd { "
                                "  display: inline-block; padding: 3px 6px; font-size: 11px; line-height: 10px; color: %3; "
                                "  background-color: %8; border: 1px solid %7; border-radius: 4px; box-shadow: inset 0 -1px 0 %7; "
                                "} "
                                "img { max-width: 100%; height: auto; border-radius: 4px; } "
                                "mark { background-color: #fff59d; color: #111111; padding: 2px 4px; border-radius: 3px; } "
                                "del, s { text-decoration: line-through; color: %6; } "
                                "ins { text-decoration: underline; } "
                                "sub, sup { font-size: 75%; line-height: 0; } "
                                ".footnotes { font-size: 85%; border-top: 1px solid %7; margin-top: 32px; padding-top: 16px; color: %6; } "
                                ".footnote-ref { font-size: 80%; text-decoration: none; }\n")
                                .arg(fontFamily, sizeStr, textColor, bgColor, hBorder, mutedColor, borderColor, blockBg,
                                     codeBg, linkColor);

    return baseCss + MarkdownPreviewEnhancer::mathAndMermaidCss(c.isDark);
}
