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

#ifndef ACCESSIBILITYMANAGER_H
#define ACCESSIBILITYMANAGER_H

#include <QObject>
#include <QString>

class MainWindow;
class QWidget;
class ScintillaNext;

class AccessibilityManager : public QObject {
    Q_OBJECT

public:
    static AccessibilityManager *instance();

    void setupAccessibility(MainWindow *window);
    void announce(QWidget *context, const QString &message);
    void openAccessibilityHelp(QWidget *parent);
    void announceDocumentState(ScintillaNext *editor, int index, int total);

private:
    explicit AccessibilityManager(QObject *parent = nullptr);
    ~AccessibilityManager() override;

    MainWindow *m_window = nullptr;
};

#endif  // ACCESSIBILITYMANAGER_H
