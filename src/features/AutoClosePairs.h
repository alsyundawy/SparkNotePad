#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 auto-completion & auto-close pair logic.
 * Source: zufuliu/Notepad4 (src/EditAutoC.cpp, Edit.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef AUTOCLOSEPAIRS_H
#define AUTOCLOSEPAIRS_H

#include "EditorDecorator.h"

class AutoClosePairs : public EditorDecorator {
    Q_OBJECT

public:
    explicit AutoClosePairs(ScintillaNext *editor);
    ~AutoClosePairs() override = default;

    bool autoCloseBrackets() const { return m_autoCloseBrackets; }
    void setAutoCloseBrackets(bool enabled) { m_autoCloseBrackets = enabled; }

    bool autoCloseQuotes() const { return m_autoCloseQuotes; }
    void setAutoCloseQuotes(bool enabled) { m_autoCloseQuotes = enabled; }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void notify(const Scintilla::NotificationData *pscn) override;

private:
    bool handleKeyPress(class QKeyEvent *keyEvent);
    static bool isWordChar(char ch);

    bool m_autoCloseBrackets{true};
    bool m_autoCloseQuotes{true};
};

#endif  // AUTOCLOSEPAIRS_H
