#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) Color Preview via CallTip and Color Picker features.
 * Source: zufuliu/notepad4 (src/Edit.cpp, src/Styles.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef COLORPREVIEWCALLTIP_H
#define COLORPREVIEWCALLTIP_H

#include <QColor>
#include <QString>
#include "EditorDecorator.h"

class ScintillaNext;

class ColorPreviewCallTip : public EditorDecorator {
    Q_OBJECT

public:
    enum ColorFormat {
        Hex3,   // #rgb
        Hex4,   // #rgba
        Hex6,   // #rrggbb
        Hex8,   // #rrggbbaa
        Hex0x,  // 0x...
        Rgb,    // rgb(r, g, b)
        Rgba,   // rgba(r, g, b, a)
        Hsl,    // hsl(h, s, l)
        Hsla,   // hsla(h, s, l, a)
        Named   // named css color
    };

    struct ColorMatch {
        bool found{false};
        int startPos{0};
        int endPos{0};
        QString text;
        QColor color;
        ColorFormat format{Hex6};
    };

    explicit ColorPreviewCallTip(ScintillaNext *editor);
    ~ColorPreviewCallTip() override;

    // Trigger preview and color picker at current caret/selection
    bool triggerColorPreviewOrPick();

    // Show preview at a specific document character position
    bool showPreviewAt(int pos);

    // Cancel / dismiss active calltip and tooltip
    void dismissPreview();

    // Open QColorDialog and replace matched text with the newly selected color
    bool openColorDialogAndReplace();

    // Parsing helpers
    static ColorMatch findColorAt(ScintillaNext *editor, int pos);
    static QString formatColor(const QColor &color, ColorFormat format);

public slots:
    void notify(const Scintilla::NotificationData *pscn) override;

private slots:
    void onDwellStart(int x, int y);
    void onDwellEnd(int x, int y);
    void onCallTipClick();

private:
    ColorMatch m_currentMatch;
};

#endif  // COLORPREVIEWCALLTIP_H
