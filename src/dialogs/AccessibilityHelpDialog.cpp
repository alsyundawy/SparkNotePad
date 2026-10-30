/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AccessibilityHelpDialog.h"
#include "ui_AccessibilityHelpDialog.h"

#include <QTreeWidgetItem>
#include <QHeaderView>

AccessibilityHelpDialog::AccessibilityHelpDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AccessibilityHelpDialog) {
    ui->setupUi(this);

    // Set accessible properties
    setAccessibleName(tr("NoteSciPad Keyboard Shortcuts and Accessibility Guide"));
    ui->editFilter->setAccessibleName(tr("Filter keyboard shortcuts"));
    ui->treeShortcuts->setAccessibleName(tr("Keyboard shortcuts table"));

    connect(ui->editFilter, &QLineEdit::textChanged, this, &AccessibilityHelpDialog::filterShortcuts);

    populateShortcuts();

    // VoiceOver Guide
    ui->textVoiceOver->setHtml(
        tr("<h3>Apple VoiceOver &amp; Linux Orca Integration in NoteSciPad</h3>"
           "<p>NoteSciPad is engineered with first-class screen reader accessibility support:</p>"
           "<ul>"
           "<li><b>Document Announcements:</b> When switching tabs (via <code>Ctrl+Tab</code> or the Window menu), "
           "NoteSciPad automatically broadcasts an accessibility notification containing the document name, tab "
           "position, and dirty status.</li>"
           "<li><b>Focusing the Editor:</b> Press <code>Ctrl+Alt+E</code> or <code>Escape</code> from docks and search "
           "bars to instantly return keyboard focus to the main editor.</li>"
           "<li><b>VoiceOver Cursor &amp; Rotor:</b> Use <code>VO + Left/Right Arrow</code> to navigate through "
           "toolbar buttons, status indicators, and tabs. "
           "Use <code>VO + U</code> to open the VoiceOver Rotor for quick heading and link jumping.</li>"
           "<li><b>Line &amp; Position Spoken Alerts:</b> The status bar provides live line, column, and selection "
           "information readable by screen readers.</li>"
           "<li><b>Native Emoji &amp; Symbol Picker:</b> Press <code>Ctrl+Cmd+Space</code> to activate the native "
           "macOS Character Palette at the cursor.</li>"
           "</ul>"));

    // Standards & Contrast
    ui->textFeatures->setHtml(
        tr("<h3>Accessibility Standards Compliance (WCAG 2.2 AAA &amp; Section 508)</h3>"
           "<p>NoteSciPad adheres to modern digital accessibility best practices:</p>"
           "<ul>"
           "<li><b>Full Keyboard Operability:</b> 100% of editor features, dialogs, menus, and tools are operable "
           "without a pointing device (mouse or trackpad).</li>"
           "<li><b>Dynamic Zoom &amp; Font Scaling:</b> Zoom text smoothly with <code>Ctrl + Plus</code> and "
           "<code>Ctrl + Minus</code>, or reset with <code>Ctrl + 0</code>.</li>"
           "<li><b>High-Contrast Dark &amp; Light Themes:</b> NoteSciPad adapts dynamically to system dark mode "
           "preferences with high-contrast text and indicator colors.</li>"
           "<li><b>Full Screen &amp; Distraction-Free:</b> Press <code>Cmd+Ctrl+F</code> (macOS) or <code>F11</code> "
           "(Linux) for full screen, and <code>F9</code> for Focused View mode.</li>"
           "<li><b>No Rapid Flashing:</b> Visual indicators and brace match decorators use steady, "
           "non-seizure-inducing visual highlights.</li>"
           "</ul>"));

    ui->treeShortcuts->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeShortcuts->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeShortcuts->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeShortcuts->header()->setSectionResizeMode(3, QHeaderView::Stretch);
}

AccessibilityHelpDialog::~AccessibilityHelpDialog() {
    delete ui;
}

void AccessibilityHelpDialog::addShortcutItem(const QString &action, const QString &shortcut, const QString &category,
                                              const QString &description) {
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeShortcuts);
    item->setText(0, action);
    item->setText(1, shortcut);
    item->setText(2, category);
    item->setText(3, description);
}

void AccessibilityHelpDialog::populateShortcuts() {
    ui->treeShortcuts->clear();

    // Document & Tabs
    addShortcutItem(tr("New Document"), QStringLiteral("Ctrl+N"), tr("File"), tr("Create a new blank document tab"));
    addShortcutItem(tr("Open File"), QStringLiteral("Ctrl+O"), tr("File"), tr("Open an existing file from disk"));
    addShortcutItem(tr("Save Current File"), QStringLiteral("Ctrl+S"), tr("File"), tr("Save modifications to disk"));
    addShortcutItem(tr("Save All"), QStringLiteral("Ctrl+Shift+S"), tr("File"),
                    tr("Save all unsaved open document tabs"));
    addShortcutItem(tr("Close Current Tab"), QStringLiteral("Ctrl+W"), tr("File"), tr("Close the active document tab"));
    addShortcutItem(tr("Close All Tabs"), QStringLiteral("Ctrl+Shift+W"), tr("File"), tr("Close all open documents"));
    addShortcutItem(tr("Next Document Tab"), QStringLiteral("Ctrl+Tab / Ctrl+PageDown"), tr("Navigation"),
                    tr("Switch to the next document tab"));
    addShortcutItem(tr("Previous Document Tab"), QStringLiteral("Ctrl+Shift+Tab / Ctrl+PageUp"), tr("Navigation"),
                    tr("Switch to the previous document tab"));
    addShortcutItem(tr("Focus Editor Canvas"), QStringLiteral("Ctrl+Alt+E / Escape"), tr("Navigation"),
                    tr("Return keyboard focus immediately to the editor"));

    // Editing
    addShortcutItem(tr("Undo"), QStringLiteral("Ctrl+Z"), tr("Edit"), tr("Revert last text change"));
    addShortcutItem(tr("Redo"), QStringLiteral("Ctrl+Y / Ctrl+Shift+Z"), tr("Edit"),
                    tr("Reapply previously reverted text change"));
    addShortcutItem(tr("Cut"), QStringLiteral("Ctrl+X"), tr("Edit"), tr("Cut selection to clipboard"));
    addShortcutItem(tr("Copy"), QStringLiteral("Ctrl+C"), tr("Edit"), tr("Copy selection to clipboard"));
    addShortcutItem(tr("Paste"), QStringLiteral("Ctrl+V"), tr("Edit"), tr("Paste text from clipboard"));
    addShortcutItem(tr("Select All"), QStringLiteral("Ctrl+A"), tr("Edit"), tr("Select all text in document"));
    addShortcutItem(tr("Duplicate Line"), QStringLiteral("Ctrl+D"), tr("Edit"),
                    tr("Duplicate the current line or selection"));
    addShortcutItem(tr("Delete Line"), QStringLiteral("Ctrl+Shift+L"), tr("Edit"), tr("Delete the current line"));
    addShortcutItem(tr("Toggle Single Line Comment"), QStringLiteral("Ctrl+/"), tr("Edit"),
                    tr("Comment or uncomment selected lines"));
    addShortcutItem(tr("Emoji & Symbols Palette"), QStringLiteral("Ctrl+Cmd+Space"), tr("Edit"),
                    tr("Open native macOS Character Palette"));

    // Search & Navigation
    addShortcutItem(tr("Find"), QStringLiteral("Ctrl+F"), tr("Search"), tr("Open search bar or dialog"));
    addShortcutItem(tr("Replace"), QStringLiteral("Ctrl+H"), tr("Search"), tr("Open find and replace dialog"));
    addShortcutItem(tr("Find Next Occurrence"), QStringLiteral("F3"), tr("Search"),
                    tr("Jump to the next search occurrence"));
    addShortcutItem(tr("Find Previous Occurrence"), QStringLiteral("Shift+F3"), tr("Search"),
                    tr("Jump to the previous search occurrence"));
    addShortcutItem(tr("Mark All Occurrences"), QStringLiteral("Ctrl+Alt+M"), tr("Search"),
                    tr("Highlight all occurrences of selected text with indicator"));
    addShortcutItem(tr("Clear Marks"), QStringLiteral("Ctrl+Alt+C"), tr("Search"),
                    tr("Clear all highlighted search marks"));
    addShortcutItem(tr("Go to Line"), QStringLiteral("Ctrl+G"), tr("Search"), tr("Jump to a specific line number"));

    // View & Window
    addShortcutItem(tr("Zoom In"), QStringLiteral("Ctrl++"), tr("View"), tr("Increase editor font size"));
    addShortcutItem(tr("Zoom Out"), QStringLiteral("Ctrl+-"), tr("View"), tr("Decrease editor font size"));
    addShortcutItem(tr("Reset Zoom"), QStringLiteral("Ctrl+0"), tr("View"), tr("Restore editor font size to default"));
    addShortcutItem(tr("Toggle Full Screen"), QStringLiteral("Cmd+Ctrl+F / F11"), tr("Window"),
                    tr("Switch full screen view on or off"));
    addShortcutItem(tr("Minimize Window"), QStringLiteral("Cmd+M / Alt+F9"), tr("Window"),
                    tr("Minimize the active application window"));
    addShortcutItem(tr("Toggle Focused View"), QStringLiteral("F9"), tr("View"),
                    tr("Dim all lines except the active line"));

    // Tools & Diagnostics
    addShortcutItem(tr("Evaluate JavaScript Expression"), QStringLiteral("Ctrl+Alt+J"), tr("Tools"),
                    tr("Evaluate JS math/string expression via JavaScriptCore"));
    addShortcutItem(tr("Performance Diagnostics"), QStringLiteral("Ctrl+Alt+Shift+P"), tr("Tools"),
                    tr("Inspect CPU, RAM, and hardware metrics"));
    addShortcutItem(tr("Keyboard Shortcuts & Accessibility"), QStringLiteral("Shift+F1 / Cmd+Shift+?"), tr("Help"),
                    tr("Open this Accessibility & Shortcut Guide"));

    ui->treeShortcuts->sortItems(0, Qt::AscendingOrder);
}

void AccessibilityHelpDialog::filterShortcuts(const QString &text) {
    QString query = text.trimmed();
    int count = ui->treeShortcuts->topLevelItemCount();
    for (int i = 0; i < count; ++i) {
        QTreeWidgetItem *item = ui->treeShortcuts->topLevelItem(i);
        if (query.isEmpty()) {
            item->setHidden(false);
        } else {
            bool matches = item->text(0).contains(query, Qt::CaseInsensitive) ||
                           item->text(1).contains(query, Qt::CaseInsensitive) ||
                           item->text(2).contains(query, Qt::CaseInsensitive) ||
                           item->text(3).contains(query, Qt::CaseInsensitive);
            item->setHidden(!matches);
        }
    }
}
