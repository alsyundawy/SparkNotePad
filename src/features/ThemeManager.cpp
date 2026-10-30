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

static const ThemePalette kLightPalette = {
    "light",   "Light Mode", false,
    "#f0f2f5",  // win (NotepadNext default workspace window)
    "#fafbfc",  // card (NotepadNext default workspace canvas)
    "#e1e4e8",  // border
    "#24292f",  // txtPri
    "#57606a",  // txtSec
    "#0550ae",  // cyan
    "#0969da",  // blue
    "#116329",  // net
    "#953800",  // sub
    "#0a3069",  // host
    "#57606a"   // sep
};

static const ThemePalette kDarkPalette = {
    "dark",    "Dracula Dark Mode", true,
    "#1e1f29",  // win (Dracula window background)
    "#282a36",  // card (Dracula editor background)
    "#44475a",  // border
    "#f8f8f2",  // txtPri (Dracula foreground)
    "#6272a4",  // txtSec (Dracula comment)
    "#8be9fd",  // cyan (Dracula cyan)
    "#bd93f9",  // blue / purple (Dracula purple)
    "#ff79c6",  // net / pink (Dracula pink)
    "#ffb86c",  // sub / orange (Dracula orange)
    "#f1fa8c",  // host / yellow (Dracula yellow)
    "#50fa7b"   // sep / green (Dracula green)
};

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
        QString savedMode =
            m_settings->value(QStringLiteral("App/ThemeMode"), QStringLiteral("Light")).toString().toLower();
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

const ThemePalette *ThemeManager::currentPalette() const {
    return isDarkActive() ? &kDarkPalette : &kLightPalette;
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
        m_settings->setValue(QStringLiteral("App/ThemeMode"), modeStr);
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

    const ThemePalette *pal = currentPalette();
    if (!pal) {
        pal = isDarkActive() ? &kDarkPalette : &kLightPalette;
    }
    if (!pal)
        return;

    QColor card(pal->card);
    QColor border(pal->border);
    QColor txtPri(pal->txtPri);
    QColor txtSec(pal->txtSec);
    QColor cyan(pal->cyan);
    QColor blue(pal->blue);
    QColor net(pal->net);
    QColor sub(pal->sub);
    QColor host(pal->host);
    QColor sep(pal->sep);

    // 1. Base canvas background and primary text
    editor->styleSetFore(STYLE_DEFAULT, scFromRgb(txtPri.rgb()));
    editor->styleSetBack(STYLE_DEFAULT, scFromRgb(card.rgb()));
    editor->styleClearAll();

    // 2. Enforce canvas background across all lexer style slots (0..127)
    for (int i = 0; i <= 127; ++i) {
        editor->styleSetBack(i, scFromRgb(card.rgb()));
    }

    // 3. Line number margin
    editor->styleSetFore(STYLE_LINENUMBER, scFromRgb(txtSec.rgb()));
    editor->styleSetBack(STYLE_LINENUMBER, scFromRgb(border.rgb()));

    // 4. Caret color and line visibility
    editor->setCaretFore(scFromRgb(txtPri.rgb()));
    editor->setCaretWidth(2);

    // 5. Caret line highlight (slight blend)
    quint32 caretLineColor =
        0xFF000000 |
        (pal->isDark ? scColor(qMin(card.red() + 18, 255), qMin(card.green() + 18, 255), qMin(card.blue() + 22, 255))
                     : scColor(qMax(card.red() - 12, 0), qMax(card.green() - 12, 0), qMax(card.blue() - 6, 0)));

    editor->setElementColour(SC_ELEMENT_CARET_LINE_BACK, caretLineColor);
    editor->setCaretLineVisible(true);

    // 6. Selection highlight
    quint32 selColor = 0xFF000000 | scFromRgb(border.rgb());
    editor->setElementColour(SC_ELEMENT_SELECTION_BACK, selColor);
    editor->setElementColour(SC_ELEMENT_SELECTION_INACTIVE_BACK, selColor);

    // 7. Fold margin colors
    editor->setFoldMarginColour(true, scFromRgb(card.rgb()));
    editor->setFoldMarginHiColour(true, scFromRgb(border.rgb()));
    editor->setElementColour(SC_ELEMENT_FOLD_LINE, 0xFF000000 | scFromRgb(border.rgb()));

    // 8. Matching and mismatching braces
    editor->styleSetFore(STYLE_BRACELIGHT, scFromRgb(cyan.rgb()));
    editor->styleSetBack(STYLE_BRACELIGHT, scFromRgb(border.rgb()));
    editor->styleSetFore(STYLE_BRACEBAD, scColor(207, 34, 46));
    editor->styleSetBack(STYLE_BRACEBAD, scFromRgb(border.rgb()));

    // 9. Indentation guides and white space
    editor->styleSetFore(STYLE_INDENTGUIDE, scFromRgb(border.rgb()));
    editor->styleSetBack(STYLE_INDENTGUIDE, scFromRgb(card.rgb()));
    editor->setElementColour(SC_ELEMENT_WHITE_SPACE, 0xFF000000 | scFromRgb(border.rgb()));

    // 10. Common Lexilla tokens styling (styles 0 through 31 across Lexilla standard lexers)
    // 0: Default / Plain text (Critical: ensures text is high-contrast readable, NEVER black on dark!)
    editor->styleSetFore(0, scFromRgb(txtPri.rgb()));

    // 1: Comment, 2: Comment line, 3: Comment doc
    editor->styleSetFore(1, scFromRgb(txtSec.rgb()));
    editor->styleSetItalic(1, true);
    editor->styleSetFore(2, scFromRgb(txtSec.rgb()));
    editor->styleSetItalic(2, true);
    editor->styleSetFore(3, scFromRgb(txtSec.rgb()));
    editor->styleSetItalic(3, true);

    // 4: Number
    editor->styleSetFore(4, scFromRgb(sub.rgb()));

    // 5: Keyword / Word
    editor->styleSetFore(5, scFromRgb(cyan.rgb()));
    editor->styleSetBold(5, true);

    // 6: String, 7: Character
    editor->styleSetFore(6, scFromRgb(host.rgb()));
    editor->styleSetFore(7, scFromRgb(host.rgb()));

    // 8: Regex / UUID / Class name
    editor->styleSetFore(8, scFromRgb(cyan.rgb()));

    // 9: Preprocessor
    editor->styleSetFore(9, scFromRgb(net.rgb()));

    // 10: Operator
    editor->styleSetFore(10, scFromRgb(sep.rgb()));

    // 11: Identifier / Variable (Never black on dark!)
    editor->styleSetFore(11, scFromRgb(txtPri.rgb()));

    // 12: Regex / String EOL
    editor->styleSetFore(12, scFromRgb(host.rgb()));

    // 13: Verbatim string
    editor->styleSetFore(13, scFromRgb(host.rgb()));

    // 14: Escape sequence
    editor->styleSetFore(14, scFromRgb(net.rgb()));

    // 15: Keywords 2
    editor->styleSetFore(15, scFromRgb(cyan.rgb()));

    // 16: Types / Built-ins (e.g. C++ types int, float, void)
    editor->styleSetFore(16, scFromRgb(cyan.rgb()));

    // 17: User literal / Doc keyword
    editor->styleSetFore(17, scFromRgb(sub.rgb()));

    // 18: Task marker (TODO, FIXME, NOTE)
    editor->styleSetFore(18, scFromRgb(net.rgb()));
    editor->styleSetBold(18, true);

    // 19: Character escape
    editor->styleSetFore(19, scFromRgb(sub.rgb()));

    // 20: Raw strings
    editor->styleSetFore(20, scFromRgb(host.rgb()));

    // 21: Functions / Methods
    editor->styleSetFore(21, scFromRgb(blue.rgb()));

    // 22: Attributes / Annotations
    editor->styleSetFore(22, scFromRgb(cyan.rgb()));

    // 23: Labels
    editor->styleSetFore(23, scFromRgb(net.rgb()));

    // 24: Decorators
    editor->styleSetFore(24, scFromRgb(cyan.rgb()));
}

QString ThemeManager::getThemeStyleSheet() const {
    const ThemePalette *pal = currentPalette();
    if (!pal) {
        pal = isDarkActive() ? &kDarkPalette : &kLightPalette;
    }

    bool darkActive = isDarkActive();
    QString win =
        pal ? QString::fromLatin1(pal->win) : (darkActive ? QStringLiteral("#1e1e1e") : QStringLiteral("#f0f2f5"));
    QString card =
        pal ? QString::fromLatin1(pal->card) : (darkActive ? QStringLiteral("#25282c") : QStringLiteral("#fafbfc"));
    QString border =
        pal ? QString::fromLatin1(pal->border) : (darkActive ? QStringLiteral("#33373e") : QStringLiteral("#e1e4e8"));
    QString txtPri =
        pal ? QString::fromLatin1(pal->txtPri) : (darkActive ? QStringLiteral("#e0e0e0") : QStringLiteral("#24292f"));
    QString txtSec =
        pal ? QString::fromLatin1(pal->txtSec) : (darkActive ? QStringLiteral("#9aa0a6") : QStringLiteral("#57606a"));
    QString accent =
        pal ? QString::fromLatin1(pal->blue) : (darkActive ? QStringLiteral("#58a6ff") : QStringLiteral("#0969da"));

    // User requirement: In light mode, toolbar background must be dark (charcoal/dracula),
    // and light mode default is NotepadNext. In dark mode, toolbar background matches dark theme.
    QString tbBg = darkActive ? win : QStringLiteral("#21252b");
    QString tbBorder = darkActive ? border : QStringLiteral("#181a1f");
    QString tbText = darkActive ? txtPri : QStringLiteral("#e6edf3");
    QString tbHover = darkActive ? border : QStringLiteral("rgba(255, 255, 255, 0.12)");

    const QString templateStr = QString::fromUtf8(R"(
/* Universal Base Typography & Text */
QWidget {
    font-family: -apple-system, BlinkMacSystemFont, "SF Pro Text", "Inter", "Ubuntu", "Roboto", "Segoe UI", "DejaVu Sans", "Liberation Sans", "Helvetica Neue", Arial, sans-serif;
    color: %2;
}

/* Windows, Dialogs, Message Boxes */
QMainWindow, QDialog, QMessageBox {
    background-color: %1;
    color: %2;
}

/* Labels */
QLabel {
    color: %2;
    background-color: transparent;
}

/* Group Boxes (Find Dialog searchMode, transparency, Preferences, etc.) */
QGroupBox {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 6px;
    margin-top: 18px;
    padding: 14px 10px 10px 10px;
    font-weight: 600;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 10px;
    padding: 2px 8px;
    color: %2;
    background-color: %5;
    border: 1px solid %3;
    border-radius: 4px;
    font-weight: bold;
}
QGroupBox::indicator {
    width: 16px;
    height: 16px;
    border: 1px solid %3;
    background-color: %5;
    border-radius: 4px;
}
QGroupBox::indicator:hover {
    border-color: %6;
}
QGroupBox::indicator:checked {
    background-color: %6;
    border-color: %6;
    image: url(:/icons/checkmark.svg);
}
QGroupBox::indicator:disabled {
    border-color: %3;
    background-color: %1;
}

/* Checkboxes & Radio Buttons */
QCheckBox, QRadioButton {
    color: %2;
    spacing: 8px;
    padding: 3px 4px;
    min-height: 20px;
    background-color: transparent;
}
QCheckBox:hover, QRadioButton:hover {
    color: %6;
}
QCheckBox:disabled, QRadioButton:disabled {
    color: %4;
}
QCheckBox::indicator, QRadioButton::indicator {
    width: 16px;
    height: 16px;
    border: 1px solid %3;
    background-color: %5;
}
QCheckBox::indicator {
    border-radius: 4px;
}
QRadioButton::indicator {
    border-radius: 9px;
}
QCheckBox::indicator:hover, QRadioButton::indicator:hover {
    border-color: %6;
}
QCheckBox::indicator:checked {
    background-color: %6;
    border-color: %6;
    image: url(:/icons/checkmark.svg);
}
QRadioButton::indicator:checked {
    background-color: %6;
    border: 3px solid %5;
}
QCheckBox::indicator:disabled, QRadioButton::indicator:disabled {
    border-color: %3;
    background-color: %1;
}

/* Text Inputs, Line Edits, Combo LineEdit */
QLineEdit, QTextEdit, QPlainTextEdit {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 5px;
    padding: 4px 8px;
    selection-background-color: %6;
    selection-color: #ffffff;
}
QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
    border-color: %6;
}
QLineEdit:disabled, QTextEdit:disabled, QPlainTextEdit:disabled {
    background-color: %1;
    color: %4;
    border-color: %3;
}

/* Combo Boxes */
QComboBox {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 5px;
    padding: 4px 8px;
    min-height: 20px;
}
QComboBox:hover {
    border-color: %6;
}
QComboBox:disabled {
    background-color: %1;
    color: %4;
    border-color: %3;
}
QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    width: 20px;
    border-left: 1px solid %3;
    border-top-right-radius: 5px;
    border-bottom-right-radius: 5px;
}
QComboBox QAbstractItemView {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    selection-background-color: %6;
    selection-color: #ffffff;
}

/* Spin Boxes */
QSpinBox, QDoubleSpinBox {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 5px;
    padding: 3px 6px;
}
QSpinBox:focus, QDoubleSpinBox:focus {
    border-color: %6;
}

/* Push Buttons */
QPushButton {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 5px;
    padding: 5px 14px;
    font-weight: 500;
}
QPushButton:hover {
    border-color: %6;
    color: %6;
}
QPushButton:pressed {
    background-color: %3;
}
QPushButton:disabled {
    color: %4;
    border-color: %3;
    background-color: %1;
}

/* Sliders */
QSlider::groove:horizontal {
    height: 6px;
    background: %3;
    border-radius: 3px;
}
QSlider::sub-page:horizontal {
    background: %6;
    border-radius: 3px;
}
QSlider::handle:horizontal {
    background: #ffffff;
    border: 2px solid %6;
    width: 16px;
    margin-top: -5px;
    margin-bottom: -5px;
    border-radius: 8px;
}

/* ScrollBars */
QScrollBar:vertical {
    background: %1;
    width: 12px;
    margin: 0px;
}
QScrollBar::handle:vertical {
    background: %3;
    min-height: 24px;
    border-radius: 4px;
    margin: 2px;
}
QScrollBar::handle:vertical:hover {
    background: %4;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}
QScrollBar:horizontal {
    background: %1;
    height: 12px;
    margin: 0px;
}
QScrollBar::handle:horizontal {
    background: %3;
    min-width: 24px;
    border-radius: 4px;
    margin: 2px;
}
QScrollBar::handle:horizontal:hover {
    background: %4;
}
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* Menus */
QMenu {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 6px;
    padding: 4px;
}
QMenu::item {
    padding: 5px 22px 5px 12px;
    border-radius: 4px;
}
QMenu::item:selected {
    background-color: %6;
    color: #ffffff;
}
QMenu::separator {
    height: 1px;
    background: %3;
    margin: 4px 6px;
}

QStatusBar {
    background-color: %1;
    color: %2;
    border-top: 1px solid %3;
}
QStatusBar::item {
    border: none;
}
ads--CDockWidgetTab ads--CElidingLabel {
    color: %4;
}
ads--CDockWidgetTab {
    background: %1;
    margin-top: 2px;
    border-left: 1px solid %3;
    border-top: 2px solid %3;
    border-right: 1px solid %3;
    padding: 3px 6px 1px 6px;
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
}
ads--CDockWidgetTab:hover[activeTab="false"] {
    background: %3;
    color: %2;
}
ads--CDockWidgetTab[activeTab="true"] {
    background: %5;
    border-left: 1px solid %3;
    border-right: 1px solid %3;
    border-top: 4px solid rgb(255, 202, 176);
    padding-bottom: 3px;
    margin-top: 0px;
}
ads--CDockWidgetTab[activeTab="true"] ads--CElidingLabel {
    color: %2;
    font-weight: bold;
}
ads--CDockWidgetTab[focused="true"] {
    background: %5;
    border-left: 1px solid %3;
    border-right: 1px solid %3;
    border-top: 4px solid orange;
}
ads--CDockAreaTitleBar {
    background: %1;
    border-bottom: 1px solid %3;
    padding-bottom: 0px;
}
#tabCloseButton {
    background: transparent;
    margin-bottom: 1px;
    margin-left: 4px;
    border: none;
    padding: 0px 0px;
    border-radius: 3px;
}
#tabCloseButton:hover {
    background-color: rgba(235, 87, 87, 0.35);
    color: #eb5757;
}
#tabCloseButton:pressed {
    background-color: #eb5757;
    color: #ffffff;
}
#QuickFindWidget {
    border-left: 1px solid %3;
    border-right: 1px solid %3;
    border-bottom: 3px solid %6;
    background: %1;
    color: %2;
}
/* Tabs & Tab Widget */
QTabWidget::pane {
    border: 1px solid %3;
    background-color: %1;
    border-radius: 8px;
}
QTabBar {
    background: transparent;
    border: none;
    qproperty-drawBase: 0;
}
QTabBar::tab {
    background-color: %1;
    color: %4;
    padding: 7px 16px;
    border: 1px solid %3;
    border-bottom: none;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    margin-right: 2px;
}
QTabBar::tab:selected {
    background-color: %5;
    color: %2;
    font-weight: bold;
    border-top: 2px solid rgb(255, 202, 176);
}
QTabBar::tab:hover:!selected {
    background-color: %3;
    color: %2;
}

/* Buttons */
QPushButton {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 6px;
    padding: 6px 16px;
    font-weight: 500;
}
QPushButton:hover {
    border-color: %6;
    color: %6;
}
QPushButton:pressed {
    background-color: %6;
    color: #ffffff;
}
QPushButton:default {
    background-color: %6;
    color: #ffffff;
    border: 1px solid %6;
}
QPushButton:disabled {
    background-color: %1;
    color: %4;
    border-color: %3;
}

/* Text Browser */
QTextBrowser {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 8px;
}

/* ToolBars */
QToolBar {
    background-color: %7;
    border: none;
    border-bottom: 1px solid %8;
    spacing: 3px;
    padding: 2px 6px;
}
QToolBar QToolButton {
    background: transparent;
    color: %9;
    border: 1px solid transparent;
    border-radius: 4px;
    padding: 3px;
    margin: 1px;
}
QToolBar QToolButton:hover {
    background-color: %10;
    border-color: %8;
}
QToolBar QToolButton:pressed {
    background-color: %6;
    color: #ffffff;
}
QToolBar::separator {
    width: 1px;
    background-color: %8;
    margin: 4px 6px;
}
)");

    return templateStr.arg(win, txtPri, border, txtSec, card, accent, tbBg, tbBorder, tbText).arg(tbHover);
}
