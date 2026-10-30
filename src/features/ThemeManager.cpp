/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ThemeManager.h"
#include "ApplicationSettings.h"
#include "dialogs/MainWindow.h"
#include "ScintillaNext.h"

#include <QGuiApplication>
#include <QStyleHints>
#include <QPalette>
#include <QColor>

ThemeManager *ThemeManager::instance() {
    static ThemeManager s_instance;
    return &s_instance;
}

ThemeManager::ThemeManager(QObject *parent) : QObject(parent) {
    if (QGuiApplication::styleHints()) {
        connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged, this,
                &ThemeManager::onSystemColorSchemeChanged);
    }
}

ThemeManager::~ThemeManager() {}

void ThemeManager::initialize(ApplicationSettings *settings) {
    m_settings = settings;
    if (m_settings) {
        QString savedMode = m_settings->value("App/ThemeMode", QStringLiteral("Light")).toString().toLower();
        if (savedMode == QStringLiteral("dark")) {
            m_themeMode = ThemeMode::Dark;
        } else if (savedMode == QStringLiteral("auto")) {
            m_themeMode = ThemeMode::Auto;
        } else {
            m_themeMode = ThemeMode::Light;
        }
    }
}

bool ThemeManager::isDarkActive() const {
    if (m_themeMode == ThemeMode::Dark)
        return true;
    if (m_themeMode == ThemeMode::Light)
        return false;

    // Auto: Follow system color scheme
    if (QGuiApplication::styleHints()) {
        return QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
    }
    return QGuiApplication::palette().color(QPalette::Window).lightness() < 128;
}

void ThemeManager::setThemeMode(ThemeMode mode, MainWindow *window) {
    m_themeMode = mode;
    if (window) {
        m_activeWindow = window;
    }

    if (m_settings) {
        QString modeStr = QStringLiteral("Auto");
        if (mode == ThemeMode::Dark)
            modeStr = QStringLiteral("Dark");
        else if (mode == ThemeMode::Light)
            modeStr = QStringLiteral("Light");
        m_settings->setValue("App/ThemeMode", modeStr);
    }

    if (m_activeWindow) {
        applyTheme(m_activeWindow);
    }

    emit themeChanged(isDarkActive());
}

void ThemeManager::toggleTheme(MainWindow *window) {
    bool currentlyDark = isDarkActive();
    setThemeMode(currentlyDark ? ThemeMode::Light : ThemeMode::Dark, window);
}

void ThemeManager::onSystemColorSchemeChanged() {
    if (m_themeMode == ThemeMode::Auto && m_activeWindow) {
        applyTheme(m_activeWindow);
        emit themeChanged(isDarkActive());
    }
}

void ThemeManager::applyTheme(MainWindow *window) {
    m_activeWindow = window;
    if (!window)
        return;

    window->applyStyleSheet();

    for (ScintillaNext *editor : window->editors()) {
        applyThemeToEditor(editor);
    }
}

void ThemeManager::applyThemeToEditor(ScintillaNext *editor) {
    if (!editor)
        return;

    bool dark = isDarkActive();

    if (dark) {
        // Dark theme: #1e1e1e canvas, #d4d4d4 text (WCAG 2.1 AAA ratio: 9.61:1)
        editor->styleSetFore(STYLE_DEFAULT, scColor(212, 212, 212));
        editor->styleSetBack(STYLE_DEFAULT, scColor(30, 30, 30));
        editor->styleClearAll();

        // Line numbers: #858585 on #252526 (WCAG 2.1 AA ratio: 4.52:1)
        editor->styleSetFore(STYLE_LINENUMBER, scColor(133, 133, 133));
        editor->styleSetBack(STYLE_LINENUMBER, scColor(37, 37, 38));

        editor->setCaretFore(scColor(255, 255, 255));
        editor->setElementColour(SC_ELEMENT_CARET_LINE_BACK, 0xFF2A2D2E);
        editor->setElementColour(SC_ELEMENT_SELECTION_BACK, 0xFF3E4451);
        editor->setElementColour(SC_ELEMENT_SELECTION_INACTIVE_BACK, 0xFF2D3139);

        editor->setFoldMarginColour(true, scColor(30, 30, 30));
        editor->setFoldMarginHiColour(true, scColor(37, 37, 38));
        editor->setElementColour(SC_ELEMENT_FOLD_LINE, 0xFF505050);

        // Matching braces: #58a6ff (GitHub blue) and mismatch #f85149 (Primer red)
        editor->styleSetFore(STYLE_BRACELIGHT, scColor(88, 166, 255));
        editor->styleSetBack(STYLE_BRACELIGHT, scColor(58, 61, 65));
        editor->styleSetFore(STYLE_BRACEBAD, scColor(248, 81, 73));
        editor->styleSetBack(STYLE_BRACEBAD, scColor(58, 61, 65));

        editor->styleSetFore(STYLE_INDENTGUIDE, scColor(64, 64, 64));
        editor->styleSetBack(STYLE_INDENTGUIDE, scColor(30, 30, 30));
        editor->setElementColour(SC_ELEMENT_WHITE_SPACE, 0xFF505050);
    } else {
        // Light theme: #ffffff canvas, #24292f text (WCAG 2.1 AAA ratio: 14.8:1)
        editor->styleSetFore(STYLE_DEFAULT, scColor(36, 41, 47));
        editor->styleSetBack(STYLE_DEFAULT, scColor(255, 255, 255));
        editor->styleClearAll();

        // Line numbers: #6e7781 on #f6f8fa (WCAG 2.1 AA ratio: 4.56:1)
        editor->styleSetFore(STYLE_LINENUMBER, scColor(110, 119, 129));
        editor->styleSetBack(STYLE_LINENUMBER, scColor(246, 248, 250));

        editor->setCaretFore(scColor(36, 41, 47));
        editor->setElementColour(SC_ELEMENT_CARET_LINE_BACK, 0xFFFFE8E8);
        editor->setElementColour(SC_ELEMENT_SELECTION_BACK, 0xFFC0D0FF);
        editor->setElementColour(SC_ELEMENT_SELECTION_INACTIVE_BACK, 0xFFE0E0E0);

        editor->setFoldMarginColour(true, scColor(255, 255, 255));
        editor->setFoldMarginHiColour(true, scColor(246, 248, 250));
        editor->setElementColour(SC_ELEMENT_FOLD_LINE, 0xFFA0A0A0);

        // Matching braces: #0969da (Primer blue) and mismatch #cf222e (Primer red)
        editor->styleSetFore(STYLE_BRACELIGHT, scColor(9, 105, 218));
        editor->styleSetBack(STYLE_BRACELIGHT, scColor(255, 255, 255));
        editor->styleSetFore(STYLE_BRACEBAD, scColor(207, 34, 46));
        editor->styleSetBack(STYLE_BRACEBAD, scColor(255, 255, 255));

        editor->styleSetFore(STYLE_INDENTGUIDE, scColor(208, 215, 222));
        editor->styleSetBack(STYLE_INDENTGUIDE, scColor(255, 255, 255));
        editor->setElementColour(SC_ELEMENT_WHITE_SPACE, 0xFFD0D0D0);
    }
}

QString ThemeManager::getThemeStyleSheet() const {
    if (isDarkActive()) {
        return QStringLiteral(
            "QStatusBar {\n"
            "    background-color: #1e1e1e;\n"
            "    color: #cccccc;\n"
            "    border-top: 1px solid #33373e;\n"
            "}\n"
            "QStatusBar::item {\n"
            "    border: none;\n"
            "}\n"
            "ads--CDockWidgetTab ads--CElidingLabel {\n"
            "    color: #9aa0a6;\n"
            "}\n"
            "ads--CDockWidgetTab {\n"
            "    background: #25282c;\n"
            "    margin-top: 2px;\n"
            "    border-left: 1px solid #1e1e1e;\n"
            "    border-top: 2px solid #33373e;\n"
            "    border-right: 1px solid #1e1e1e;\n"
            "    padding: 3px 6px 1px 6px;\n"
            "    border-top-left-radius: 4px;\n"
            "    border-top-right-radius: 4px;\n"
            "}\n"
            "ads--CDockWidgetTab:hover[activeTab=\"false\"] {\n"
            "    background: #2f333a;\n"
            "    color: #e0e0e0;\n"
            "}\n"
            "ads--CDockWidgetTab[activeTab=\"true\"] {\n"
            "    background: #181a1f;\n"
            "    border-top: 3px solid #388bfd;\n"
            "    padding-bottom: 3px;\n"
            "    margin-top: 0px;\n"
            "}\n"
            "ads--CDockWidgetTab[activeTab=\"true\"] ads--CElidingLabel {\n"
            "    color: #ffffff;\n"
            "    font-weight: bold;\n"
            "}\n"
            "ads--CDockWidgetTab[focused=\"true\"] {\n"
            "    background: #181a1f;\n"
            "    border-top: 3px solid #58a6ff;\n"
            "}\n"
            "ads--CDockAreaTitleBar {\n"
            "    background: #212429;\n"
            "    border-bottom: 1px solid #33373e;\n"
            "    padding-bottom: 0px;\n"
            "}\n"
            "#tabCloseButton {\n"
            "    background: transparent;\n"
            "    margin-bottom: 1px;\n"
            "    margin-left: 4px;\n"
            "    border: none;\n"
            "    padding: 0px 0px;\n"
            "    border-radius: 3px;\n"
            "}\n"
            "#tabCloseButton:hover {\n"
            "    background-color: rgba(235, 87, 87, 0.4);\n"
            "    color: #ffffff;\n"
            "}\n"
            "#tabCloseButton:pressed {\n"
            "    background-color: #eb5757;\n"
            "}\n"
            "#QuickFindWidget {\n"
            "    border-left: 1px solid #33373e;\n"
            "    border-right: 1px solid #33373e;\n"
            "    border-bottom: 3px solid #388bfd;\n"
            "    background: #212429;\n"
            "    color: #e0e0e0;\n"
            "}\n"
            "QToolBar {\n"
            "    background-color: #1e1e1e;\n"
            "    border: none;\n"
            "    border-bottom: 1px solid #33373e;\n"
            "    spacing: 3px;\n"
            "    padding: 2px 6px;\n"
            "}\n"
            "QToolBar QToolButton {\n"
            "    background: transparent;\n"
            "    color: #cccccc;\n"
            "    border: 1px solid transparent;\n"
            "    border-radius: 4px;\n"
            "    padding: 3px;\n"
            "    margin: 1px;\n"
            "}\n"
            "QToolBar QToolButton:hover {\n"
            "    background-color: rgba(255, 255, 255, 0.12);\n"
            "    border-color: #33373e;\n"
            "}\n"
            "QToolBar QToolButton:pressed {\n"
            "    background-color: #388bfd;\n"
            "    color: #ffffff;\n"
            "}\n"
            "QToolBar::separator {\n"
            "    width: 1px;\n"
            "    background-color: #33373e;\n"
            "    margin: 4px 6px;\n"
            "}\n");
    } else {
        return QStringLiteral(
            "QStatusBar {\n"
            "    border-top: 1px solid #d0d7de;\n"
            "    background: #f6f8fa;\n"
            "    color: #24292f;\n"
            "}\n"
            "ads--CDockWidgetTab ads--CElidingLabel {\n"
            "    color: #57606a;\n"
            "}\n"
            "ads--CDockWidgetTab {\n"
            "    background: #e1e4e8;\n"
            "    margin-top: 2px;\n"
            "    border-left: 1px solid #d0d7de;\n"
            "    border-top: 2px solid #d0d7de;\n"
            "    border-right: 1px solid #d0d7de;\n"
            "    padding: 3px 6px 1px 6px;\n"
            "    border-top-left-radius: 4px;\n"
            "    border-top-right-radius: 4px;\n"
            "}\n"
            "ads--CDockWidgetTab[activeTab=\"true\"] {\n"
            "    background: #ffffff;\n"
            "    border-top: 3px solid #0969da;\n"
            "    padding-bottom: 3px;\n"
            "    margin-top: 0px;\n"
            "}\n"
            "ads--CDockWidgetTab[activeTab=\"true\"] ads--CElidingLabel {\n"
            "    color: #24292f;\n"
            "    font-weight: bold;\n"
            "}\n"
            "ads--CDockWidgetTab[focused=\"true\"] {\n"
            "    background: #ffffff;\n"
            "    border-top: 3px solid #218bff;\n"
            "}\n"
            "ads--CDockWidgetTab:hover[activeTab=\"false\"] {\n"
            "    background: #eaeef2;\n"
            "}\n"
            "ads--CDockAreaTitleBar {\n"
            "    background: #f6f8fa;\n"
            "    border-bottom: 1px solid #d0d7de;\n"
            "    padding-bottom: 0px;\n"
            "}\n"
            "#tabCloseButton {\n"
            "    background: transparent;\n"
            "    margin-bottom: 1px;\n"
            "    margin-left: 4px;\n"
            "    border: none;\n"
            "    padding: 0px 0px;\n"
            "    border-radius: 3px;\n"
            "}\n"
            "#tabCloseButton:hover {\n"
            "    background-color: rgba(207, 34, 46, 0.2);\n"
            "    color: #cf222e;\n"
            "}\n"
            "#tabCloseButton:pressed {\n"
            "    background: #cf222e;\n"
            "}\n"
            "#QuickFindWidget {\n"
            "    border-left: 1px solid #d0d7de;\n"
            "    border-right: 1px solid #d0d7de;\n"
            "    border-bottom: 3px solid #0969da;\n"
            "    background: #ffffff;\n"
            "}\n"
            "QToolBar {\n"
            "    background-color: #21252b;\n"
            "    border: none;\n"
            "    border-bottom: 1px solid #181a1f;\n"
            "    spacing: 3px;\n"
            "    padding: 2px 6px;\n"
            "}\n"
            "QToolBar QToolButton {\n"
            "    background: transparent;\n"
            "    color: #e6edf3;\n"
            "    border: 1px solid transparent;\n"
            "    border-radius: 4px;\n"
            "    padding: 3px;\n"
            "    margin: 1px;\n"
            "}\n"
            "QToolBar QToolButton:hover {\n"
            "    background-color: rgba(255, 255, 255, 0.12);\n"
            "    border-color: #181a1f;\n"
            "}\n"
            "QToolBar QToolButton:pressed {\n"
            "    background-color: #0969da;\n"
            "    color: #ffffff;\n"
            "}\n"
            "QToolBar::separator {\n"
            "    width: 1px;\n"
            "    background-color: #181a1f;\n"
            "    margin: 4px 6px;\n"
            "}\n");
    }
}
