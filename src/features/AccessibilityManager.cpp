/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AccessibilityManager.h"
#include "MacServicesHandler.h"
#include "dialogs/MainWindow.h"
#include "dialogs/AccessibilityHelpDialog.h"
#include "ScintillaNext.h"

#include <QAccessible>
#include <QAccessibleEvent>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>

AccessibilityManager *AccessibilityManager::instance() {
    static AccessibilityManager s_instance;
    return &s_instance;
}

AccessibilityManager::AccessibilityManager(QObject *parent) : QObject(parent) {}

AccessibilityManager::~AccessibilityManager() {}

void AccessibilityManager::setupAccessibility(MainWindow *window) {
    m_window = window;
    if (!window)
        return;

    window->setAccessibleName(tr("NoteSciPad Main Window"));
    window->setAccessibleDescription(tr("Primary editing workspace with tabbed documents, toolbars, and status bar."));

    QList<QToolBar *> toolBars = window->findChildren<QToolBar *>();
    for (QToolBar *tb : toolBars) {
        tb->setAccessibleName(tb->windowTitle());
        for (QAction *act : tb->actions()) {
            if (!act->text().isEmpty()) {
                QWidget *w = tb->widgetForAction(act);
                if (w) {
                    w->setAccessibleName(act->text().remove('&'));
                    w->setAccessibleDescription(act->toolTip());
                }
            }
        }
    }
}

void AccessibilityManager::announce(QWidget *context, const QString &message) {
    if (message.isEmpty())
        return;

    // Native macOS VoiceOver announcement
    MacServicesHandler::postVoiceOverAnnouncement(message);

    // Cross-platform Qt Accessibility
#if QT_CONFIG(accessibility)
    if (context && QAccessible::isActive()) {
        QAccessibleEvent event(context, QAccessible::Alert);
        context->setAccessibleDescription(message);
        QAccessible::updateAccessibility(&event);
    }
#else
    Q_UNUSED(context);
#endif
}

void AccessibilityManager::announceDocumentState(ScintillaNext *editor, int index, int total) {
    if (!editor)
        return;

    QString dirty = editor->isSavedToDisk() ? QString() : tr(" (unsaved)");
    QString msg = tr("Document %1 of %2: %3%4").arg(index + 1).arg(total).arg(editor->getName()).arg(dirty);

    editor->setAccessibleName(tr("Editor: %1").arg(editor->getName()));
    editor->setAccessibleDescription(
        tr("Text editing area for %1. Total lines: %2.").arg(editor->getName()).arg(editor->lineCount()));

    announce(editor, msg);
}

void AccessibilityManager::openAccessibilityHelp(QWidget *parent) {
    AccessibilityHelpDialog dlg(parent);
    dlg.exec();
}
