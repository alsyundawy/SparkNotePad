#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4, UrlThreshold) and Notepad3 URL features.
 * Sources: zufuliu/notepad4 & rizonesoft/Notepad3
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef URLFINDER_H
#define URLFINDER_H

#include "EditorDecorator.h"

class URLFinder : public EditorDecorator {
    Q_OBJECT

public:
    explicit URLFinder(ScintillaNext *editor);
    bool isURL(int position) const;
    void copyURLToClipboard(int position) const;
    void openURLAt(int position) const;

    void setUrlThreshold(int thresholdMB);
    int urlThreshold() const { return m_urlThresholdMB; }

private slots:
    void findURLs();
    void clearURLs();

public slots:
    void notify(const Scintilla::NotificationData *pscn) override;

private:
    QTimer *timer;
    int indicator;
    int m_urlThresholdMB{5};
};

#endif  // URLFINDER_H
