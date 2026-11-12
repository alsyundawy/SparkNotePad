/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ThemeManager.h"
#include "ApplicationSettings.h"
#include "dialogs/MainWindow.h"
#include "ScintillaNext.h"
#include "SciLexer.h"

#include <QGuiApplication>
#include <QApplication>
#include <QStyleHints>
#include <QPalette>
#include <QColor>
#include <QtGlobal>
#include <cmath>
#include <algorithm>

ThemeManager *ThemeManager::instance() {
    static ThemeManager s_instance;
    return &s_instance;
}

ThemeManager::ThemeManager(QObject *parent) : QObject(parent) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    if (QGuiApplication::styleHints()) {
        connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged, this,
                &ThemeManager::onSystemColorSchemeChanged);
    }
#endif
}

void ThemeManager::initialize(ApplicationSettings *settings) {
    m_settings = settings;
    if (m_settings) {
        using enum ThemeManager::ThemeMode;
        if (const QString savedMode =
                m_settings->value(QStringLiteral("App/ThemeMode"), QStringLiteral("Light")).toString().toLower();
            savedMode == QStringLiteral("dark")) {
            m_themeMode = Dark;
        } else if (savedMode == QStringLiteral("auto")) {
            m_themeMode = Auto;
        } else {
            m_themeMode = Light;
        }

        if (QString savedPalette = m_settings->value(QStringLiteral("App/ThemePalette"), QString()).toString();
            !savedPalette.isEmpty() && findThemePaletteById(savedPalette)) {
            m_currentPaletteId = std::move(savedPalette);
        } else {
            m_currentPaletteId =
                (m_themeMode == ThemeMode::Dark) ? QStringLiteral("dracula") : QStringLiteral("sparkNotepad");
        }

        emit themeChanged(isDarkActive());
        emit paletteChanged(m_currentPaletteId);
    }
}

bool ThemeManager::isDarkActive() const {
    if (m_themeMode == ThemeMode::Dark) {
        return true;
    }
    if (m_themeMode == ThemeMode::Light) {
        return false;
    }

    // Auto: Follow system color scheme
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    if (QGuiApplication::styleHints()) {
        return QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
    }
#endif
    return QGuiApplication::palette().color(QPalette::Window).lightness() < 128;
}

const ThemePalette *ThemeManager::currentPalette() const {
    if (!m_currentPaletteId.isEmpty()) {
        const ThemePalette *p = findThemePaletteById(m_currentPaletteId);
        if (p) {
            if (m_themeMode == ThemeMode::Dark && !p->isDark) {
                return findThemePaletteById(QStringLiteral("dracula"));
            }
            if (m_themeMode == ThemeMode::Light && p->isDark) {
                return findThemePaletteById(QStringLiteral("sparkNotepad"));
            }
            return p;
        }
    }
    return isDarkActive() ? findThemePaletteById(QStringLiteral("dracula"))
                          : findThemePaletteById(QStringLiteral("sparkNotepad"));
}

void ThemeManager::setThemeMode(ThemeMode mode, MainWindow *window) {
    const bool oldDark = isDarkActive();
    m_themeMode = mode;
    if (window) {
        m_activeWindow = window;
    }

    const ThemePalette *cur = findThemePaletteById(m_currentPaletteId);
    if (mode == ThemeMode::Dark) {
        if (!cur || !cur->isDark) {
            m_currentPaletteId = QStringLiteral("dracula");
        }
    } else if (mode == ThemeMode::Light && (!cur || cur->isDark)) {
        m_currentPaletteId = QStringLiteral("sparkNotepad");
    }

    if (m_settings) {
        auto modeStr = QStringLiteral("Auto");
        if (mode == ThemeMode::Dark) {
            modeStr = QStringLiteral("Dark");
        } else if (mode == ThemeMode::Light) {
            modeStr = QStringLiteral("Light");
        }
        m_settings->setValue(QStringLiteral("App/ThemeMode"), modeStr);
        m_settings->setValue(QStringLiteral("App/ThemePalette"), m_currentPaletteId);
    }

    if (m_activeWindow) {
        applyTheme(m_activeWindow);
    }

    if (const bool newDark = isDarkActive(); oldDark != newDark) {
        emit themeChanged(newDark);
    }
    emit paletteChanged(m_currentPaletteId);
}

void ThemeManager::setPalette(const QString &paletteId, MainWindow *window) {
    const ThemePalette *p = findThemePaletteById(paletteId);
    if (!p) {
        return;
    }

    if (m_currentPaletteId == paletteId && (!window || window == m_activeWindow)) {
        return;
    }

    const bool oldDark = isDarkActive();
    m_currentPaletteId = paletteId;
    m_themeMode = p->isDark ? ThemeMode::Dark : ThemeMode::Light;

    if (window) {
        m_activeWindow = window;
    }

    if (m_settings) {
        m_settings->setValue(QStringLiteral("App/ThemePalette"), m_currentPaletteId);
        m_settings->setValue(QStringLiteral("App/ThemeMode"),
                             p->isDark ? QStringLiteral("Dark") : QStringLiteral("Light"));
    }

    if (m_activeWindow) {
        applyTheme(m_activeWindow);
    }

    if (const bool newDark = isDarkActive(); oldDark != newDark) {
        emit themeChanged(newDark);
    }
    emit paletteChanged(m_currentPaletteId);
}

void ThemeManager::toggleTheme(MainWindow *window) {
    const bool currentlyDark = isDarkActive();
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

    const ThemePalette *pal = currentPalette();
    const bool dark = isDarkActive();
    if (!pal) {
        pal = dark ? findThemePaletteById(QStringLiteral("dracula"))
                   : findThemePaletteById(QStringLiteral("sparkNotepad"));
    }

    QString win;
    QString card;
    QString txtPri;
    QString txtSec;
    QString accent;

    if (pal) {
        win = QString::fromLatin1(pal->win);
        card = QString::fromLatin1(pal->card);
        txtPri = QString::fromLatin1(pal->txtPri);
        txtSec = QString::fromLatin1(pal->txtSec);
        accent = QString::fromLatin1(pal->blue);
    } else if (dark) {
        win = QStringLiteral("#1e1e1e");
        card = QStringLiteral("#25282c");
        txtPri = QStringLiteral("#e0e0e0");
        txtSec = QStringLiteral("#9aa0a6");
        accent = QStringLiteral("#58a6ff");
    } else {
        win = QStringLiteral("#f0f2f5");
        card = QStringLiteral("#fafbfc");
        txtPri = QStringLiteral("#24292f");
        txtSec = QStringLiteral("#57606a");
        accent = QStringLiteral("#0969da");
    }

    QPalette appPal;
    appPal.setColor(QPalette::Window, QColor(win));
    appPal.setColor(QPalette::WindowText, QColor(txtPri));
    appPal.setColor(QPalette::Base, QColor(card));
    appPal.setColor(QPalette::AlternateBase, dark ? QColor(win).lighter(108) : QColor(win).darker(103));
    appPal.setColor(QPalette::Text, QColor(txtPri));
    appPal.setColor(QPalette::Button, QColor(card));
    appPal.setColor(QPalette::ButtonText, QColor(txtPri));
    appPal.setColor(QPalette::BrightText, Qt::white);
    appPal.setColor(QPalette::Highlight, QColor(accent));
    appPal.setColor(QPalette::HighlightedText, Qt::white);
    appPal.setColor(QPalette::ToolTipBase, QColor(card));
    appPal.setColor(QPalette::ToolTipText, QColor(txtPri));
    appPal.setColor(QPalette::Link, QColor(accent));
    appPal.setColor(QPalette::PlaceholderText, QColor(txtSec));
    appPal.setColor(QPalette::Disabled, QPalette::Text, QColor(txtSec));
    appPal.setColor(QPalette::Disabled, QPalette::WindowText, QColor(txtSec));
    appPal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(txtSec));
    QApplication::setPalette(appPal);

    if (qApp) {
        const QString sheet = getThemeStyleSheet();
        qApp->setStyleSheet(sheet);
    }

    if (!window) {
        return;
    }

    window->applyStyleSheet();

    for (ScintillaNext *editor : window->editors()) {
        applyThemeToEditor(editor);
    }
}

namespace {

struct SyntaxColorPalette {
    QColor card;
    QColor txtPri;
    QColor txtSec;
    QColor kwColor;
    QColor fnColor;
    QColor typeColor;
    QColor strColor;
    QColor numColor;
    QColor constColor;
    QColor escColor;
    QColor commentColor;
    QColor docCommentColor;
    QColor docTagColor;
    QColor taskMarkerColor;
    QColor preprocColor;
    QColor opColor;
    QColor idColor;
    QColor tagColor;
    QColor attrColor;
    QColor jsonKeyColor;
    QColor diffAddColor;
    QColor diffDelColor;
    QColor diffHdrColor;
};

inline void setTokenStyle(ScintillaNext *editor, int styleId, const QColor &fore, bool bold = false,
                          bool italic = false, bool underline = false) {
    if (styleId < 0 || styleId > 127) {
        return;
    }
    editor->styleSetFore(styleId, ThemeManager::scFromRgb(fore.rgb()));
    if (bold) {
        editor->styleSetBold(styleId, true);
    }
    if (italic) {
        editor->styleSetItalic(styleId, true);
    }
    if (underline) {
        editor->styleSetUnderline(styleId, true);
    }
}

SyntaxColorPalette buildSyntaxPalette(const ThemePalette *pal) {
    const bool isDark = pal->isDark;
    const QColor card(pal->card);
    const QColor txtPri(pal->txtPri);
    const QColor txtSec(pal->txtSec);
    const QColor cyan(pal->cyan);
    const QColor blue(pal->blue);
    const QColor net(pal->net);
    const QColor sub(pal->sub);
    const QColor host(pal->host);
    const QColor sep(pal->sep);

    SyntaxColorPalette p;
    p.card = card;
    p.txtPri = ThemeManager::ensureContrast(txtPri, card, isDark, 4.5);
    p.txtSec = ThemeManager::ensureContrast(txtSec, card, isDark, 2.5);
    p.kwColor = ThemeManager::ensureContrast(cyan, card, isDark, 3.5);
    p.fnColor = ThemeManager::ensureContrast(blue, card, isDark, 3.5);

    // Derived type color: distinguishable from keywords and functions
    const QColor derivedType = isDark
                                   ? QColor::fromHsl((cyan.hslHue() + 25) % 360, qMin(255, cyan.hslSaturation() + 20),
                                                     std::clamp(cyan.lightness() + 10, 160, 230))
                                   : QColor::fromHsl((cyan.hslHue() + 25) % 360, qMin(255, cyan.hslSaturation() + 30),
                                                     std::clamp(cyan.lightness() - 15, 40, 110));
    p.typeColor = ThemeManager::ensureContrast(derivedType, card, isDark, 3.5);

    p.strColor = ThemeManager::ensureContrast(host, card, isDark, 3.2);
    p.numColor = ThemeManager::ensureContrast(sub, card, isDark, 3.2);
    p.constColor = ThemeManager::ensureContrast(sub, card, isDark, 3.2);

    // Escape sequences (\n, \t, \x00) - warm golden amber
    p.escColor = isDark ? QColor(255, 185, 75) : QColor(190, 85, 0);

    // Comments & doc comments
    p.commentColor = ThemeManager::ensureContrast(txtSec, card, isDark, 2.5);
    p.docCommentColor =
        ThemeManager::ensureContrast(isDark ? txtSec.lighter(120) : txtSec.darker(115), card, isDark, 3.0);
    p.docTagColor = ThemeManager::ensureContrast(blue, card, isDark, 3.5);

    // Actionable task markers (TODO, FIXME, BUG, HACK, XXX, NOTE) - vibrant gold badge
    p.taskMarkerColor = isDark ? QColor(255, 215, 0) : QColor(185, 105, 0);

    // Preprocessor directives (#include, @decorator, #[derive])
    p.preprocColor = ThemeManager::ensureContrast(net, card, isDark, 3.2);

    // Operators & punctuation
    p.opColor = ThemeManager::ensureContrast(sep, card, isDark, 2.8);

    // Identifiers & default text - guaranteed high-contrast readable
    p.idColor = p.txtPri;

    // Markup tags & attributes
    p.tagColor = ThemeManager::ensureContrast(blue, card, isDark, 3.5);
    p.attrColor = ThemeManager::ensureContrast(cyan, card, isDark, 3.2);

    // JSON property keys
    p.jsonKeyColor = ThemeManager::ensureContrast(blue, card, isDark, 3.5);

    // Diff markers
    p.diffAddColor = isDark ? QColor(80, 250, 123) : QColor(26, 127, 55);
    p.diffDelColor = isDark ? QColor(255, 85, 85) : QColor(207, 34, 46);
    p.diffHdrColor = isDark ? QColor(139, 233, 253) : QColor(9, 105, 218);

    return p;
}

void applyCppFamilyStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_C_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_C_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_C_COMMENTLINE, p.commentColor, false, true);
    setTokenStyle(editor, SCE_C_COMMENTDOC, p.docCommentColor, false, true);
    setTokenStyle(editor, SCE_C_NUMBER, p.numColor);
    setTokenStyle(editor, SCE_C_WORD, p.kwColor, true);
    setTokenStyle(editor, SCE_C_STRING, p.strColor);
    setTokenStyle(editor, SCE_C_CHARACTER, p.strColor);
    setTokenStyle(editor, SCE_C_UUID, p.constColor);
    setTokenStyle(editor, SCE_C_PREPROCESSOR, p.preprocColor);
    setTokenStyle(editor, SCE_C_OPERATOR, p.opColor);
    setTokenStyle(editor, SCE_C_IDENTIFIER, p.idColor);
    setTokenStyle(editor, SCE_C_STRINGEOL, p.diffDelColor, false, false, true);
    setTokenStyle(editor, SCE_C_VERBATIM, p.strColor);
    setTokenStyle(editor, SCE_C_REGEX, p.escColor);
    setTokenStyle(editor, SCE_C_COMMENTLINEDOC, p.docCommentColor, false, true);
    setTokenStyle(editor, SCE_C_WORD2, p.typeColor, true);
    setTokenStyle(editor, SCE_C_COMMENTDOCKEYWORD, p.docTagColor, true);
    setTokenStyle(editor, SCE_C_COMMENTDOCKEYWORDERROR, p.diffDelColor);
    setTokenStyle(editor, SCE_C_GLOBALCLASS, p.typeColor, true);
    setTokenStyle(editor, SCE_C_STRINGRAW, p.strColor);
    setTokenStyle(editor, SCE_C_TRIPLEVERBATIM, p.strColor);
    setTokenStyle(editor, SCE_C_HASHQUOTEDSTRING, p.strColor);
    setTokenStyle(editor, SCE_C_PREPROCESSORCOMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_C_PREPROCESSORCOMMENTDOC, p.docCommentColor, false, true);
    setTokenStyle(editor, SCE_C_USERLITERAL, p.numColor);
    setTokenStyle(editor, SCE_C_TASKMARKER, p.taskMarkerColor, true);
    setTokenStyle(editor, SCE_C_ESCAPESEQUENCE, p.escColor, true);
}

void applyPythonStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_P_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_P_COMMENTLINE, p.commentColor, false, true);
    setTokenStyle(editor, SCE_P_NUMBER, p.numColor);
    setTokenStyle(editor, SCE_P_STRING, p.strColor);
    setTokenStyle(editor, SCE_P_CHARACTER, p.strColor);
    setTokenStyle(editor, SCE_P_WORD, p.kwColor, true);
    setTokenStyle(editor, SCE_P_TRIPLE, p.docCommentColor, false, true);
    setTokenStyle(editor, SCE_P_TRIPLEDOUBLE, p.docCommentColor, false, true);
    setTokenStyle(editor, SCE_P_CLASSNAME, p.typeColor, true);
    setTokenStyle(editor, SCE_P_DEFNAME, p.fnColor, true);
    setTokenStyle(editor, SCE_P_OPERATOR, p.opColor);
    setTokenStyle(editor, SCE_P_IDENTIFIER, p.idColor);
    setTokenStyle(editor, SCE_P_COMMENTBLOCK, p.commentColor, false, true);
    setTokenStyle(editor, SCE_P_STRINGEOL, p.diffDelColor, false, false, true);
    setTokenStyle(editor, SCE_P_WORD2, p.typeColor, true);
    setTokenStyle(editor, SCE_P_DECORATOR, p.preprocColor, false, true);
    setTokenStyle(editor, SCE_P_FSTRING, p.strColor);
    setTokenStyle(editor, SCE_P_FCHARACTER, p.strColor);
    setTokenStyle(editor, SCE_P_FTRIPLE, p.strColor);
    setTokenStyle(editor, SCE_P_FTRIPLEDOUBLE, p.strColor);
    setTokenStyle(editor, SCE_P_ATTRIBUTE, p.attrColor);
}

void applyHtmlXmlStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_H_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_H_TAG, p.tagColor, true);
    setTokenStyle(editor, SCE_H_TAGUNKNOWN, p.tagColor);
    setTokenStyle(editor, SCE_H_ATTRIBUTE, p.attrColor);
    setTokenStyle(editor, SCE_H_ATTRIBUTEUNKNOWN, p.attrColor);
    setTokenStyle(editor, SCE_H_NUMBER, p.numColor);
    setTokenStyle(editor, SCE_H_DOUBLESTRING, p.strColor);
    setTokenStyle(editor, SCE_H_SINGLESTRING, p.strColor);
    setTokenStyle(editor, SCE_H_OTHER, p.opColor);
    setTokenStyle(editor, SCE_H_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_H_ENTITY, p.escColor, true);
    setTokenStyle(editor, SCE_H_TAGEND, p.opColor);
    setTokenStyle(editor, SCE_H_XMLSTART, p.preprocColor, true);
    setTokenStyle(editor, SCE_H_XMLEND, p.preprocColor, true);
    setTokenStyle(editor, SCE_H_SCRIPT, p.idColor);
    setTokenStyle(editor, SCE_H_ASP, p.preprocColor);
    setTokenStyle(editor, SCE_H_ASPAT, p.preprocColor);
    setTokenStyle(editor, SCE_H_CDATA, p.constColor);
    setTokenStyle(editor, SCE_H_QUESTION, p.preprocColor, true);
    setTokenStyle(editor, SCE_H_VALUE, p.strColor);
    setTokenStyle(editor, SCE_H_XCCOMMENT, p.commentColor, false, true);

    // Embedded JavaScript in HTML (<script>)
    for (int s = 40; s <= 55; ++s) {
        if (s == 42 || s == 43 || s == 44) {
            setTokenStyle(editor, s, p.commentColor, false, true);
        } else if (s == 45) {
            setTokenStyle(editor, s, p.numColor);
        } else if (s == 46 || s == 47) {
            setTokenStyle(editor, s, p.strColor);
        } else if (s == 48) {
            setTokenStyle(editor, s, p.kwColor, true);
        } else {
            setTokenStyle(editor, s, p.idColor);
        }
    }

    // Embedded PHP in HTML (<?php ... ?>)
    for (int s = 118; s <= 127; ++s) {
        if (s == 124 || s == 125) {
            setTokenStyle(editor, s, p.commentColor, false, true);
        } else if (s == 122) {
            setTokenStyle(editor, s, p.numColor);
        } else if (s == 119 || s == 120) {
            setTokenStyle(editor, s, p.strColor);
        } else if (s == 121) {
            setTokenStyle(editor, s, p.kwColor, true);
        } else if (s == 123) {
            setTokenStyle(editor, s, p.attrColor);
        } else {
            setTokenStyle(editor, s, p.idColor);
        }
    }
}

void applyJsonStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_JSON_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_JSON_NUMBER, p.numColor);
    setTokenStyle(editor, SCE_JSON_STRING, p.strColor);
    setTokenStyle(editor, SCE_JSON_STRINGEOL, p.diffDelColor, false, false, true);
    setTokenStyle(editor, SCE_JSON_PROPERTYNAME, p.jsonKeyColor, true);
    setTokenStyle(editor, SCE_JSON_ESCAPESEQUENCE, p.escColor, true);
    setTokenStyle(editor, SCE_JSON_LINECOMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_JSON_BLOCKCOMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_JSON_OPERATOR, p.opColor);
    setTokenStyle(editor, SCE_JSON_URI, p.fnColor, false, false, true);
    setTokenStyle(editor, SCE_JSON_COMPACTIRI, p.fnColor);
    setTokenStyle(editor, SCE_JSON_KEYWORD, p.constColor, true);
    setTokenStyle(editor, SCE_JSON_LDKEYWORD, p.kwColor);
    setTokenStyle(editor, SCE_JSON_ERROR, p.diffDelColor, false, false, true);
}

void applyCssStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_CSS_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_CSS_TAG, p.tagColor, true);
    setTokenStyle(editor, SCE_CSS_CLASS, p.kwColor, true);
    setTokenStyle(editor, SCE_CSS_PSEUDOCLASS, p.preprocColor);
    setTokenStyle(editor, SCE_CSS_UNKNOWN_PSEUDOCLASS, p.preprocColor);
    setTokenStyle(editor, SCE_CSS_OPERATOR, p.opColor);
    setTokenStyle(editor, SCE_CSS_IDENTIFIER, p.fnColor);
    setTokenStyle(editor, SCE_CSS_UNKNOWN_IDENTIFIER, p.fnColor);
    setTokenStyle(editor, SCE_CSS_VALUE, p.strColor);
    setTokenStyle(editor, SCE_CSS_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_CSS_ID, p.taskMarkerColor, true);
    setTokenStyle(editor, SCE_CSS_IMPORTANT, p.diffDelColor, true);
    setTokenStyle(editor, SCE_CSS_DIRECTIVE, p.preprocColor, true);
    setTokenStyle(editor, SCE_CSS_DOUBLESTRING, p.strColor);
    setTokenStyle(editor, SCE_CSS_SINGLESTRING, p.strColor);
    setTokenStyle(editor, SCE_CSS_IDENTIFIER2, p.fnColor);
    setTokenStyle(editor, SCE_CSS_ATTRIBUTE, p.attrColor);
    setTokenStyle(editor, SCE_CSS_IDENTIFIER3, p.fnColor);
    setTokenStyle(editor, SCE_CSS_PSEUDOELEMENT, p.preprocColor);
    setTokenStyle(editor, SCE_CSS_EXTENDED_IDENTIFIER, p.fnColor);
    setTokenStyle(editor, SCE_CSS_EXTENDED_PSEUDOCLASS, p.preprocColor);
    setTokenStyle(editor, SCE_CSS_EXTENDED_PSEUDOELEMENT, p.preprocColor);
    setTokenStyle(editor, SCE_CSS_GROUP_RULE, p.preprocColor);
    setTokenStyle(editor, SCE_CSS_VARIABLE, p.escColor, true);
}

void applyMarkdownStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_MARKDOWN_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_MARKDOWN_LINE_BEGIN, p.opColor);
    setTokenStyle(editor, SCE_MARKDOWN_STRONG1, p.idColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_STRONG2, p.idColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_EM1, p.idColor, false, true);
    setTokenStyle(editor, SCE_MARKDOWN_EM2, p.idColor, false, true);
    setTokenStyle(editor, SCE_MARKDOWN_HEADER1, p.fnColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_HEADER2, p.kwColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_HEADER3, p.typeColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_HEADER4, p.strColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_HEADER5, p.numColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_HEADER6, p.preprocColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_PRECHAR, p.opColor);
    setTokenStyle(editor, SCE_MARKDOWN_ULIST_ITEM, p.kwColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_OLIST_ITEM, p.kwColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_BLOCKQUOTE, p.commentColor, false, true);
    setTokenStyle(editor, SCE_MARKDOWN_STRIKEOUT, p.txtSec);
    setTokenStyle(editor, SCE_MARKDOWN_HRULE, p.opColor, true);
    setTokenStyle(editor, SCE_MARKDOWN_LINK, p.fnColor, false, false, true);
    setTokenStyle(editor, SCE_MARKDOWN_CODE, p.numColor);
    setTokenStyle(editor, SCE_MARKDOWN_CODE2, p.numColor);
    setTokenStyle(editor, SCE_MARKDOWN_CODEBK, p.numColor);
    setTokenStyle(editor, SCE_MARKDOWN_FRONT_MARK, p.opColor);
    setTokenStyle(editor, SCE_MARKDOWN_FRONT, p.strColor);
    setTokenStyle(editor, SCE_MARKDOWN_FRONT_KEY, p.kwColor, true);
}

void applyBashStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_SH_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_SH_ERROR, p.diffDelColor, false, false, true);
    setTokenStyle(editor, SCE_SH_COMMENTLINE, p.commentColor, false, true);
    setTokenStyle(editor, SCE_SH_NUMBER, p.numColor);
    setTokenStyle(editor, SCE_SH_WORD, p.kwColor, true);
    setTokenStyle(editor, SCE_SH_STRING, p.strColor);
    setTokenStyle(editor, SCE_SH_CHARACTER, p.strColor);
    setTokenStyle(editor, SCE_SH_OPERATOR, p.opColor);
    setTokenStyle(editor, SCE_SH_IDENTIFIER, p.fnColor);
    setTokenStyle(editor, SCE_SH_SCALAR, p.escColor, true);
    setTokenStyle(editor, SCE_SH_PARAM, p.escColor, true);
    setTokenStyle(editor, SCE_SH_BACKTICKS, p.preprocColor);
    setTokenStyle(editor, SCE_SH_HERE_DELIM, p.preprocColor, true);
    setTokenStyle(editor, SCE_SH_HERE_Q, p.strColor);
}

void applySqlStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_SQL_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_SQL_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_SQL_COMMENTLINE, p.commentColor, false, true);
    setTokenStyle(editor, SCE_SQL_COMMENTDOC, p.docCommentColor, false, true);
    setTokenStyle(editor, SCE_SQL_NUMBER, p.numColor);
    setTokenStyle(editor, SCE_SQL_WORD, p.kwColor, true);
    setTokenStyle(editor, SCE_SQL_STRING, p.strColor);
    setTokenStyle(editor, SCE_SQL_CHARACTER, p.strColor);
    setTokenStyle(editor, SCE_SQL_OPERATOR, p.opColor);
    setTokenStyle(editor, SCE_SQL_IDENTIFIER, p.idColor);
    setTokenStyle(editor, SCE_SQL_WORD2, p.typeColor, true);
    setTokenStyle(editor, SCE_SQL_QUOTEDIDENTIFIER, p.fnColor);
}

void applyYamlStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_YAML_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_YAML_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_YAML_IDENTIFIER, p.jsonKeyColor, true);
    setTokenStyle(editor, SCE_YAML_KEYWORD, p.constColor, true);
    setTokenStyle(editor, SCE_YAML_NUMBER, p.numColor);
    setTokenStyle(editor, SCE_YAML_REFERENCE, p.preprocColor);
    setTokenStyle(editor, SCE_YAML_DOCUMENT, p.opColor, true);
    setTokenStyle(editor, SCE_YAML_TEXT, p.strColor);
    setTokenStyle(editor, SCE_YAML_ERROR, p.diffDelColor, false, false, true);
    setTokenStyle(editor, SCE_YAML_OPERATOR, p.opColor);
}

void applyPropsStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_PROPS_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_PROPS_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_PROPS_SECTION, p.fnColor, true);
    setTokenStyle(editor, SCE_PROPS_ASSIGNMENT, p.opColor);
    setTokenStyle(editor, SCE_PROPS_DEFVAL, p.strColor);
    setTokenStyle(editor, SCE_PROPS_KEY, p.kwColor, true);
}

void applyLuaStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_LUA_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_LUA_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_LUA_COMMENTLINE, p.commentColor, false, true);
    setTokenStyle(editor, SCE_LUA_COMMENTDOC, p.docCommentColor, false, true);
    setTokenStyle(editor, SCE_LUA_NUMBER, p.numColor);
    setTokenStyle(editor, SCE_LUA_WORD, p.kwColor, true);
    setTokenStyle(editor, SCE_LUA_STRING, p.strColor);
    setTokenStyle(editor, SCE_LUA_CHARACTER, p.strColor);
    setTokenStyle(editor, SCE_LUA_LITERALSTRING, p.strColor);
    setTokenStyle(editor, SCE_LUA_PREPROCESSOR, p.preprocColor);
    setTokenStyle(editor, SCE_LUA_OPERATOR, p.opColor);
    setTokenStyle(editor, SCE_LUA_IDENTIFIER, p.idColor);
    setTokenStyle(editor, SCE_LUA_WORD2, p.typeColor, true);
}

void applyDiffStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_DIFF_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_DIFF_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_DIFF_COMMAND, p.preprocColor, true);
    setTokenStyle(editor, SCE_DIFF_HEADER, p.diffHdrColor, true);
    setTokenStyle(editor, SCE_DIFF_POSITION, p.taskMarkerColor, true);
    setTokenStyle(editor, SCE_DIFF_DELETED, p.diffDelColor);
    setTokenStyle(editor, SCE_DIFF_ADDED, p.diffAddColor);
    setTokenStyle(editor, SCE_DIFF_CHANGED, p.numColor);
}

void applyBatchStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_BAT_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_BAT_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_BAT_WORD, p.kwColor, true);
    setTokenStyle(editor, SCE_BAT_LABEL, p.fnColor, true);
    setTokenStyle(editor, SCE_BAT_HIDE, p.opColor);
    setTokenStyle(editor, SCE_BAT_COMMAND, p.fnColor);
    setTokenStyle(editor, SCE_BAT_IDENTIFIER, p.escColor, true);
    setTokenStyle(editor, SCE_BAT_OPERATOR, p.opColor);
}

void applyMakeStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, SCE_MAKE_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_MAKE_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_MAKE_PREPROCESSOR, p.preprocColor, true);
    setTokenStyle(editor, SCE_MAKE_IDENTIFIER, p.escColor);
    setTokenStyle(editor, SCE_MAKE_OPERATOR, p.opColor);
    setTokenStyle(editor, SCE_MAKE_TARGET, p.fnColor, true);
    setTokenStyle(editor, SCE_CMAKE_DEFAULT, p.idColor);
    setTokenStyle(editor, SCE_CMAKE_COMMENT, p.commentColor, false, true);
    setTokenStyle(editor, SCE_CMAKE_STRINGDQ, p.strColor);
    setTokenStyle(editor, SCE_CMAKE_COMMANDS, p.kwColor, true);
    setTokenStyle(editor, SCE_CMAKE_PARAMETERS, p.attrColor);
    setTokenStyle(editor, SCE_CMAKE_VARIABLE, p.escColor, true);
}

void applyUniversalFallbackStyles(ScintillaNext *editor, const SyntaxColorPalette &p) {
    setTokenStyle(editor, 0, p.idColor);
    setTokenStyle(editor, 1, p.commentColor, false, true);
    setTokenStyle(editor, 2, p.commentColor, false, true);
    setTokenStyle(editor, 3, p.docCommentColor, false, true);
    setTokenStyle(editor, 4, p.numColor);
    setTokenStyle(editor, 5, p.kwColor, true);
    setTokenStyle(editor, 6, p.strColor);
    setTokenStyle(editor, 7, p.strColor);
    setTokenStyle(editor, 8, p.constColor);
    setTokenStyle(editor, 9, p.preprocColor);
    setTokenStyle(editor, 10, p.opColor);
    setTokenStyle(editor, 11, p.idColor);
    setTokenStyle(editor, 12, p.strColor);
    setTokenStyle(editor, 13, p.strColor);
    setTokenStyle(editor, 14, p.escColor);
    setTokenStyle(editor, 15, p.typeColor, true);
    setTokenStyle(editor, 16, p.typeColor, true);
    setTokenStyle(editor, 17, p.docTagColor, true);
    setTokenStyle(editor, 18, p.taskMarkerColor, true);
    setTokenStyle(editor, 19, p.typeColor, true);
    setTokenStyle(editor, 20, p.strColor);
    setTokenStyle(editor, 21, p.fnColor);
    setTokenStyle(editor, 22, p.attrColor);
    setTokenStyle(editor, 23, p.preprocColor);
    setTokenStyle(editor, 24, p.preprocColor);
    setTokenStyle(editor, 25, p.numColor);
    setTokenStyle(editor, 26, p.taskMarkerColor, true);
    setTokenStyle(editor, 27, p.escColor, true);
}

}  // namespace

void ThemeManager::applyThemeToEditor(ScintillaNext *editor) const {
    if (!editor) {
        return;
    }

    const ThemePalette *pal = currentPalette();
    if (!pal) {
        pal = isDarkActive() ? findThemePaletteById(QStringLiteral("dracula"))
                             : findThemePaletteById(QStringLiteral("sparkNotepad"));
    }
    if (!pal) {
        return;
    }

    const SyntaxColorPalette synPal = buildSyntaxPalette(pal);
    const QColor border(pal->border);

    // 1. Base canvas background and primary text
    editor->styleSetFore(STYLE_DEFAULT, scFromRgb(synPal.txtPri.rgb()));
    editor->styleSetBack(STYLE_DEFAULT, scFromRgb(synPal.card.rgb()));
    editor->styleClearAll();

    // 2. Pre-seed ALL 128 lexer style slots (0..127) with card background and idColor foreground.
    // This mathematically guarantees that NO token anywhere can EVER be rendered black on a dark theme
    // or washed out on a light theme!
    for (int i = 0; i <= 127; ++i) {
        editor->styleSetBack(i, scFromRgb(synPal.card.rgb()));
        editor->styleSetFore(i, scFromRgb(synPal.idColor.rgb()));
    }

    // 3. Line number margin
    editor->styleSetFore(STYLE_LINENUMBER, scFromRgb(synPal.txtSec.rgb()));
    editor->styleSetBack(STYLE_LINENUMBER, scFromRgb(border.rgb()));

    // 4. Caret color and line visibility
    editor->setCaretFore(scFromRgb(synPal.txtPri.rgb()));
    editor->setCaretWidth(2);

    // 5. Caret line highlight (slight blend)
    const quint32 caretLineColor =
        0xFF000000 | (pal->isDark ? scColor(qMin(synPal.card.red() + 18, 255), qMin(synPal.card.green() + 18, 255),
                                            qMin(synPal.card.blue() + 22, 255))
                                  : scColor(qMax(synPal.card.red() - 12, 0), qMax(synPal.card.green() - 12, 0),
                                            qMax(synPal.card.blue() - 6, 0)));
    editor->setElementColour(SC_ELEMENT_CARET_LINE_BACK, caretLineColor);
    editor->setCaretLineVisible(true);

    // 6. Selection highlight (vivid high-contrast colors for both light and dark themes)
    quint32 selColor = 0;
    quint32 inactiveSelColor = 0;
    quint32 additionalSelColor = 0;
    if (pal->isDark) {
        selColor = 0xFF000000 | scColor(45, 85, 140);
        inactiveSelColor = 0xFF000000 | scColor(55, 65, 80);
        additionalSelColor = 0xFF000000 | scColor(55, 95, 150);
    } else {
        selColor = 0xFF000000 | scColor(172, 206, 247);  // macOS vibrant selection light blue
        inactiveSelColor = 0xFF000000 | scColor(215, 222, 230);
        additionalSelColor = 0xFF000000 | scColor(185, 215, 250);
    }
    editor->setElementColour(SC_ELEMENT_SELECTION_BACK, selColor);
    editor->setElementColour(SC_ELEMENT_SELECTION_INACTIVE_BACK, inactiveSelColor);
    editor->setElementColour(SC_ELEMENT_SELECTION_ADDITIONAL_BACK, additionalSelColor);

    // 7. Fold margin colors
    editor->setFoldMarginColour(true, scFromRgb(synPal.card.rgb()));
    editor->setFoldMarginHiColour(true, scFromRgb(border.rgb()));
    editor->setElementColour(SC_ELEMENT_FOLD_LINE, 0xFF000000 | scFromRgb(border.rgb()));

    // 8. Matching and mismatching braces
    editor->styleSetFore(STYLE_BRACELIGHT, scFromRgb(synPal.kwColor.rgb()));
    editor->styleSetBack(STYLE_BRACELIGHT, scFromRgb(border.rgb()));
    editor->styleSetFore(STYLE_BRACEBAD, scColor(255, 85, 85));
    editor->styleSetBack(STYLE_BRACEBAD, scFromRgb(border.rgb()));

    // 9. Indentation guides and white space
    editor->styleSetFore(STYLE_INDENTGUIDE, scFromRgb(border.rgb()));
    editor->styleSetBack(STYLE_INDENTGUIDE, scFromRgb(synPal.card.rgb()));
    editor->setElementColour(SC_ELEMENT_WHITE_SPACE, 0xFF000000 | scFromRgb(border.rgb()));

    // 10. Smart highlighter indicator (occurrence matching under cursor)
    const int smartIndic = editor->allocateIndicator(QStringLiteral("smart_highlighter"));
    if (smartIndic >= 0) {
        editor->indicSetFore(smartIndic, scFromRgb(synPal.kwColor.rgb()));
        editor->indicSetStyle(smartIndic, INDIC_ROUNDBOX);
        editor->indicSetOutlineAlpha(smartIndic, 140);
        editor->indicSetAlpha(smartIndic, pal->isDark ? 80 : 60);
        editor->indicSetUnder(smartIndic, true);
    }

    // 11. Specialized language-family syntax styling dispatch
    const QString lang = editor->languageName.trimmed().toLower();
    if (lang == QStringLiteral("python") || lang == QStringLiteral("py") || lang == QStringLiteral("mojo")) {
        applyPythonStyles(editor, synPal);
    } else if (lang == QStringLiteral("html") || lang == QStringLiteral("htm") || lang == QStringLiteral("xml") ||
               lang == QStringLiteral("svg") || lang == QStringLiteral("php") || lang == QStringLiteral("asp")) {
        applyHtmlXmlStyles(editor, synPal);
    } else if (lang == QStringLiteral("json") || lang == QStringLiteral("json5")) {
        applyJsonStyles(editor, synPal);
    } else if (lang == QStringLiteral("css") || lang == QStringLiteral("scss") || lang == QStringLiteral("less") ||
               lang == QStringLiteral("sass")) {
        applyCssStyles(editor, synPal);
    } else if (lang == QStringLiteral("markdown") || lang == QStringLiteral("md")) {
        applyMarkdownStyles(editor, synPal);
    } else if (lang == QStringLiteral("bash") || lang == QStringLiteral("sh") || lang == QStringLiteral("shell") ||
               lang == QStringLiteral("zsh")) {
        applyBashStyles(editor, synPal);
    } else if (lang == QStringLiteral("sql")) {
        applySqlStyles(editor, synPal);
    } else if (lang == QStringLiteral("yaml") || lang == QStringLiteral("yml")) {
        applyYamlStyles(editor, synPal);
    } else if (lang == QStringLiteral("toml") || lang == QStringLiteral("ini") ||
               lang == QStringLiteral("properties") || lang == QStringLiteral("props") ||
               lang == QStringLiteral("conf") || lang == QStringLiteral("config")) {
        applyPropsStyles(editor, synPal);
    } else if (lang == QStringLiteral("lua")) {
        applyLuaStyles(editor, synPal);
    } else if (lang == QStringLiteral("diff") || lang == QStringLiteral("patch")) {
        applyDiffStyles(editor, synPal);
    } else if (lang == QStringLiteral("batch") || lang == QStringLiteral("bat") || lang == QStringLiteral("cmd")) {
        applyBatchStyles(editor, synPal);
    } else if (lang == QStringLiteral("makefile") || lang == QStringLiteral("cmake") ||
               lang == QStringLiteral("cmakefile")) {
        applyMakeStyles(editor, synPal);
    } else if (lang == QStringLiteral("c++") || lang == QStringLiteral("cpp") || lang == QStringLiteral("c") ||
               lang == QStringLiteral("c#") || lang == QStringLiteral("cs") || lang == QStringLiteral("java") ||
               lang == QStringLiteral("javascript") || lang == QStringLiteral("js") ||
               lang == QStringLiteral("typescript") || lang == QStringLiteral("ts") || lang == QStringLiteral("rust") ||
               lang == QStringLiteral("go") || lang == QStringLiteral("dart") || lang == QStringLiteral("swift") ||
               lang == QStringLiteral("zig") || lang == QStringLiteral("kotlin") ||
               lang == QStringLiteral("actionscript") || lang == QStringLiteral("objective-c") ||
               lang == QStringLiteral("objc") || lang == QStringLiteral("d")) {
        applyCppFamilyStyles(editor, synPal);
    } else {
        applyUniversalFallbackStyles(editor, synPal);
    }
}

QString ThemeManager::getThemeStyleSheet() const {
    const QString palId = m_currentPaletteId;
    const bool darkActive = isDarkActive();
    const QString cacheKey = palId + (darkActive ? QStringLiteral("-dark") : QStringLiteral("-light"));
    if (const auto it = m_styleSheetCache.constFind(cacheKey); it != m_styleSheetCache.constEnd()) {
        return it.value();
    }

    const ThemePalette *pal = currentPalette();
    if (!pal) {
        pal = isDarkActive() ? findThemePaletteById(QStringLiteral("dracula"))
                             : findThemePaletteById(QStringLiteral("sparkNotepad"));
    }

    QString win;
    QString card;
    QString border;
    QString txtPri;
    QString txtSec;
    QString accent;

    if (pal) {
        win = QString::fromLatin1(pal->win);
        card = QString::fromLatin1(pal->card);
        border = QString::fromLatin1(pal->border);
        txtPri = QString::fromLatin1(pal->txtPri);
        txtSec = QString::fromLatin1(pal->txtSec);
        accent = QString::fromLatin1(pal->blue);
    } else if (darkActive) {
        win = QStringLiteral("#1e1e1e");
        card = QStringLiteral("#25282c");
        border = QStringLiteral("#33373e");
        txtPri = QStringLiteral("#e0e0e0");
        txtSec = QStringLiteral("#9aa0a6");
        accent = QStringLiteral("#58a6ff");
    } else {
        win = QStringLiteral("#f0f2f5");
        card = QStringLiteral("#fafbfc");
        border = QStringLiteral("#e1e4e8");
        txtPri = QStringLiteral("#24292f");
        txtSec = QStringLiteral("#57606a");
        accent = QStringLiteral("#0969da");
    }

    // Permanent requirement: Toolbar background must ALWAYS remain dark mode in ANY mode (light, dark, or auto).
    // In dark palettes, it adopts the palette's dark background. In light palettes/modes, it strictly uses dark
    // charcoal (#21252b).
    const bool isDark = darkActive || (pal && pal->isDark);
    const QString tbBg = isDark ? win : QStringLiteral("#21252b");
    const QString tbBorder = isDark ? border : QStringLiteral("#181a1f");
    const QString tbText = isDark ? txtPri : QStringLiteral("#e6edf3");
    const QString tbHover = isDark ? border : QStringLiteral("rgba(255, 255, 255, 0.12)");

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
    background-color: %1;
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
    border: 1px solid %6;
}
QLineEdit:disabled, QTextEdit:disabled, QPlainTextEdit:disabled {
    background-color: %1;
    color: %4;
    border-color: %3;
}

/* Combo Boxes (Find Dialog search combo, Preferences theme combo, etc.) */
QComboBox {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 5px;
    padding: 4px 26px 4px 8px;
    min-height: 22px;
}
QComboBox:focus, QComboBox:on {
    border: 1px solid %6;
}
QComboBox:disabled {
    background-color: %1;
    color: %4;
    border-color: %3;
}
QComboBox QLineEdit,
QComboBox QLineEdit:hover,
QComboBox QLineEdit:focus {
    background-color: %5;
    color: %2;
    border: none;
    padding: 0px;
    selection-background-color: %6;
    selection-color: #ffffff;
}
QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    width: 22px;
    border-left: 1px solid %3;
    border-top-right-radius: 5px;
    border-bottom-right-radius: 5px;
    background-color: %1;
}
QComboBox::down-arrow {
    width: 0px;
    height: 0px;
    border-left: 4px solid transparent;
    border-right: 4px solid transparent;
    border-top: 5px solid %2;
    margin: 0 auto;
}
QComboBox::down-arrow:hover {
    border-top-color: %6;
}

/* Item Views (ComboBox popup lists, ListViews, Tables) */
QComboBox QAbstractItemView, QListView, QTreeView, QTableView {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 6px;
    padding: 4px;
    selection-background-color: %6;
    selection-color: #ffffff;
    outline: none;
}
QAbstractItemView::item {
    min-height: 24px;
    padding: 3px 8px;
    border-radius: 4px;
    color: %2;
}
QAbstractItemView::item:selected {
    background-color: %6;
    color: #ffffff;
}
QAbstractItemView::item:hover:!selected {
    background-color: rgba(59, 130, 246, 0.2);
    color: %2;
}

/* Spin Boxes */
QSpinBox, QDoubleSpinBox {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 5px;
    padding: 4px 8px;
}
QSpinBox:focus, QDoubleSpinBox:focus {
    border: 1px solid %6;
}
QSpinBox::up-button, QSpinBox::down-button,
QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
    background-color: %1;
    border: 1px solid %3;
    width: 16px;
}

/* Sliders (Transparency slider, etc.) */
QSlider::groove:horizontal {
    border: 1px solid %3;
    height: 6px;
    background: %1;
    border-radius: 3px;
}
QSlider::sub-page:horizontal {
    background: %6;
    border-radius: 3px;
}
QSlider::handle:horizontal {
    background: %5;
    border: 2px solid %6;
    width: 14px;
    height: 14px;
    margin-top: -5px;
    margin-bottom: -5px;
    border-radius: 8px;
}
QSlider::handle:horizontal:hover {
    background: %6;
}

/* Scroll Bars */
QScrollBar:vertical {
    background: %1;
    width: 10px;
    margin: 0px;
    border-radius: 5px;
}
QScrollBar::handle:vertical {
    background: %3;
    min-height: 20px;
    border-radius: 5px;
}
QScrollBar::handle:vertical:hover {
    background: %4;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    background: none;
    height: 0px;
}
QScrollBar:horizontal {
    background: %1;
    height: 10px;
    margin: 0px;
    border-radius: 5px;
}
QScrollBar::handle:horizontal {
    background: %3;
    min-width: 20px;
    border-radius: 5px;
}
QScrollBar::handle:horizontal:hover {
    background: %4;
}
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
    background: none;
    width: 0px;
}

/* Menus & MenuBar */
QMenuBar {
    background-color: %1;
    color: %2;
    border-bottom: 1px solid %3;
}
QMenuBar::item {
    background: transparent;
    padding: 3px 6px;
    font-size: 12px;
    color: %2;
}
QMenuBar::item:selected {
    background-color: %3;
    border-radius: 4px;
}
QMenu {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 6px;
    padding: 4px;
}
QMenu::item {
    padding: 6px 24px 6px 12px;
    border-radius: 4px;
    color: %2;
}
QMenu::item:selected {
    background-color: %6;
    color: #ffffff;
}
QMenu::item:disabled {
    color: %4;
}
QMenu::separator {
    height: 1px;
    background: %3;
    margin: 4px 6px;
}

/* Tooltips - Compact Rounded Font Fit */
QToolTip {
    background-color: rgba(30, 34, 42, 0.95);
    color: #f0f6fc;
    border: 1px solid rgba(255, 255, 255, 0.16);
    border-radius: 6px;
    padding: 2px 6px;
    font-size: 11px;
    font-weight: 400;
    qproperty-margin: 0;
}

/* Status Bar */
QStatusBar {
    background-color: %1;
    color: %2;
    border-top: 1px solid %3;
}
QStatusBar::item {
    border: none;
}

/* Dock Manager Tabs */
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

/* Standard QDockWidget Styling (File Browser, Folder as Workspace, Live Preview, etc.) */
QDockWidget {
    background-color: %1;
    color: %2;
    border: 1px solid %3;
}
QDockWidget::title {
    text-align: left;
    background-color: %1;
    color: %2;
    padding: 6px 10px;
    border: 1px solid %3;
    border-bottom: 2px solid %6;
    font-weight: 600;
    font-size: 11px;
}
QDockWidget::close-button, QDockWidget::float-button {
    background: transparent;
    border: 1px solid transparent;
    border-radius: 3px;
    padding: 2px;
    subcontrol-position: top right;
    top: 4px;
}
QDockWidget::close-button {
    right: 4px;
}
QDockWidget::float-button {
    right: 22px;
}
QDockWidget::close-button:hover, QDockWidget::float-button:hover {
    background-color: %3;
    border-color: %3;
}
QDockWidget::close-button:pressed, QDockWidget::float-button:pressed {
    background-color: %6;
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

/* ToolBars - Always Dark Mode in ANY mode */
QToolBar, #mainToolBar, QToolBar#mainToolBar {
    background-color: %7;
    background: %7;
    border: none;
    border-bottom: 1px solid %8;
    spacing: 3px;
    padding: 2px 6px;
}
QToolBar QToolButton, #mainToolBar QToolButton, QToolBar#mainToolBar QToolButton {
    background: transparent;
    color: %9;
    border: 1px solid transparent;
    border-radius: 4px;
    padding: 3px;
    margin: 1px;
}
QToolBar QToolButton:hover, #mainToolBar QToolButton:hover, QToolBar#mainToolBar QToolButton:hover {
    background-color: %10;
    border-color: %8;
}
QToolBar QToolButton:pressed, #mainToolBar QToolButton:pressed, QToolBar#mainToolBar QToolButton:pressed {
    background-color: %6;
    color: #ffffff;
}
QToolBar QToolButton:checked, #mainToolBar QToolButton:checked, QToolBar#mainToolBar QToolButton:checked {
    background-color: rgba(255, 255, 255, 0.18);
    border-color: %8;
}
QToolBar::separator, #mainToolBar::separator, QToolBar#mainToolBar::separator {
    width: 1px;
    background-color: %8;
    margin: 4px 6px;
}
)");

    const QString result =
        templateStr.arg(win, txtPri, border, txtSec, card, accent, tbBg, tbBorder, tbText).arg(tbHover);
    m_styleSheetCache.insert(cacheKey, result);
    return result;
}
