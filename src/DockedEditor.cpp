/*
 *
 * This file is part of SparkNotepad.
 *
 * Copyright 2018-2026 Justin Dailey
 * Copyright 2025-2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotepad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Notepad Next is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Notepad Next.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "DockedEditor.h"
#include "DockAreaTabBar.h"
#include "DockAreaWidget.h"
#include "DockWidgetTab.h"
#include "DockComponentsFactory.h"
#include "DockedEditorTitleBar.h"
#include "DockAreaTitleBar.h"

#include "ScintillaNext.h"
#include "features/TabColorManager.h"
#include "features/ThemeManager.h"

#include <QDateTime>
#include <QFileInfo>
#include <QLocale>
#include <QUuid>

static QString buildEnrichedToolTip(const ScintillaNext *editor) {
    if (!editor)
        return QString();

    QStringList parts;
    const bool isFile = editor->isFile();
    const bool isSaved = editor->isSavedToDisk();
    const QString name = editor->getName();

    parts << QStringLiteral("<b>%1</b> %2")
                 .arg(name.toHtmlEscaped(),
                      isSaved ? QString() : QStringLiteral("<span style='color:#e5a50a;'>[Modified]</span>"));

    if (isFile) {
        const QString path = editor->getFilePath();
        parts << QStringLiteral("<b>Path:</b> %1").arg(path.toHtmlEscaped());
        QFileInfo fi(path);
        if (fi.exists()) {
            parts << QStringLiteral("<b>Size:</b> %1").arg(QLocale().formattedDataSize(fi.size()));
            parts << QStringLiteral("<b>Modified:</b> %1")
                         .arg(fi.lastModified().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
        }
    }

    parts << QStringLiteral("<b>Lines:</b> %1 | <b>Length:</b> %2").arg(editor->lineCount()).arg(editor->length());

    parts << QStringLiteral("<b>Encoding:</b> %1 | <b>EOL:</b> %2")
                 .arg(editor->encodingName().toHtmlEscaped(),
                      ScintillaNext::eolModeToString(static_cast<int>(editor->eOLMode())).toHtmlEscaped());

    return parts.join(QStringLiteral("<br>"));
}

class DockedEditorComponentsFactory : public ads::CDockComponentsFactory {
public:
    ads::CDockAreaTitleBar *createDockAreaTitleBar(ads::CDockAreaWidget *DockArea) const override {
        auto *titleBar = new DockedEditorTitleBar(DockArea);

        // Disable the built in context menu for the title bar since it has options we don't want
        titleBar->setContextMenuPolicy(Qt::NoContextMenu);

        return titleBar;
    }
};

DockedEditor::DockedEditor(QWidget *parent) : QObject(parent) {
    ads::CDockComponentsFactory::setFactory(new DockedEditorComponentsFactory());

    ads::CDockManager::setConfigFlag(ads::CDockManager::AllTabsHaveCloseButton, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::AlwaysShowTabs, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::OpaqueSplitterResize, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DragPreviewIsDynamic, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DragPreviewShowsContentPixmap, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasCloseButton, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasUndockButton, false);
    // When tabs title/text elide disabled and lots of tabs opened, tabs menu button will not show
    // as it only shows when tab title elided.
    // So disable dynamic tabs menu visibility.
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaDynamicTabsMenuButtonVisibility, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::EqualSplitOnInsertion, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::MiddleMouseButtonClosesTab, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasTabsMenuButton, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DisableStylesheet, true);

    dockManager = new ads::CDockManager(parent);
    dockManager->setStyleSheet(ThemeManager::instance()->getThemeStyleSheet());

    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this,
            [this](bool /*isDark*/) { dockManager->setStyleSheet(ThemeManager::instance()->getThemeStyleSheet()); });
    connect(ThemeManager::instance(), &ThemeManager::paletteChanged, this, [this](const QString & /*palId*/) {
        dockManager->setStyleSheet(ThemeManager::instance()->getThemeStyleSheet());
    });

    connect(dockManager, &ads::CDockManager::focusedDockWidgetChanged, this,
            [this](const ads::CDockWidget * /*old*/, const ads::CDockWidget *now) {
                if (!now || !now->widget()) {
                    auto edList = editors();
                    currentEditor = edList.isEmpty() ? nullptr : edList.first();
                    if (currentEditor) {
                        emit editorActivated(currentEditor.data());
                    }
                    return;
                }

                ScintillaNext *editor = qobject_cast<ScintillaNext *>(now->widget());
                if (!editor) {
                    return;
                }

                currentEditor = editor;
                editor->grabFocus();
                emit editorActivated(editor);
            });

    connect(dockManager, &ads::CDockManager::dockAreaCreated, this, [this](const ads::CDockAreaWidget *DockArea) {
        if (const auto *titleBar = qobject_cast<const DockedEditorTitleBar *>(DockArea->titleBar());
            titleBar != nullptr) {
            connect(titleBar, &DockedEditorTitleBar::doubleClicked, this, &DockedEditor::titleBarDoubleClicked);
            connect(titleBar, &DockedEditorTitleBar::plusButtonClicked, this, &DockedEditor::newFileRequested);
        }

        connect(DockArea->titleBar()->tabBar(), &ads::CDockAreaTabBar::tabMoved, this,
                [this](int /*from*/, int /*to*/) { emit editorOrderChanged(); });

        // In theory the order changes when a new dock area is created (e.g. editor is dragged and dropped),
        // but the dockAreaCreated() signal is triggered before it is actually added to the CDockManager,
        // so interrogating the dock manager during the signal doesn't help.
        // emit editorOrderChanged();
    });
}

ScintillaNext *DockedEditor::getCurrentEditor() const {
    return currentEditor;
}

int DockedEditor::count() const {
    return static_cast<int>(editors().size());
}

QVector<ScintillaNext *> DockedEditor::editors() const {
    QVector<ScintillaNext *> editors;

    // For each area, for each widget, append it to our list if it is a ScintillaNext editor
    for (const ads::CDockAreaWidget *areaWidget : dockManager->openedDockAreas()) {
        const auto &widgets = areaWidget->dockWidgets();
        for (const ads::CDockWidget *dockWidget : widgets) {
            if (auto *scintilla = qobject_cast<ScintillaNext *>(dockWidget->widget())) {
                editors.append(scintilla);
            }
        }
    }

    return editors;
}

void DockedEditor::switchToEditor(const ScintillaNext *editor) const {
    if (!editor) {
        return;
    }

    ads::CDockWidget *dockWidget = qobject_cast<ads::CDockWidget *>(editor->parentWidget());

    if (dockWidget == Q_NULLPTR) {
        qWarning() << "Expected editor's parent to be CDockWidget";
    } else {
        dockWidget->raise();
    }
}

void DockedEditor::dockWidgetCloseRequested() {
    auto *dockWidget = qobject_cast<ads::CDockWidget *>(sender());
    if (!dockWidget || !dockWidget->widget()) {
        return;
    }
    ScintillaNext *editor = qobject_cast<ScintillaNext *>(dockWidget->widget());
    if (editor) {
        emit editorCloseRequested(editor);
    } else {
        dockWidget->closeDockWidget();
    }
}

ads::CDockWidget *DockedEditor::addTabWidget(QWidget *widget, const QString &title, const QIcon &icon,
                                             const QString &toolTip) {
    if (!widget) {
        return nullptr;
    }

    ads::CDockWidget *dockWidget = dockManager->createDockWidget(title);
    dockWidget->tabWidget()->setElideMode(Qt::ElideNone);
    dockWidget->setObjectName(QUuid::createUuid().toString());
    dockWidget->setWidget(widget);
    dockWidget->setFeature(ads::CDockWidget::DockWidgetFeature::DockWidgetDeleteOnClose, true);
    dockWidget->setFeature(ads::CDockWidget::DockWidgetFeature::CustomCloseHandling, true);
    dockWidget->setFeature(ads::CDockWidget::DockWidgetFeature::DockWidgetFloatable, false);

    if (!icon.isNull()) {
        dockWidget->tabWidget()->setIcon(icon);
    }

    if (!toolTip.isEmpty()) {
        dockWidget->tabWidget()->setToolTip(toolTip);
    } else {
        dockWidget->tabWidget()->setToolTip(title);
    }

    connect(dockWidget, &ads::CDockWidget::closeRequested, this, &DockedEditor::dockWidgetCloseRequested);

    latestDockArea = dockManager->addDockWidget(ads::CenterDockWidgetArea, dockWidget, currentDockArea());
    dockWidget->raise();
    return dockWidget;
}

ads::CDockAreaWidget *DockedEditor::currentDockArea() const {
    if (dockManager->focusedDockWidget() && dockManager->focusedDockWidget()->dockAreaWidget()) {
        return dockManager->focusedDockWidget()->dockAreaWidget();
    }
    if (latestDockArea && dockManager->openedDockAreas().contains(latestDockArea.data())) {
        return latestDockArea.data();
    }
    if (!dockManager->openedDockAreas().isEmpty()) {
        return dockManager->openedDockAreas().first();
    }
    return nullptr;
}

void DockedEditor::addEditor(ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    Q_ASSERT(editor != Q_NULLPTR);

    if (currentEditor == Q_NULLPTR) {
        currentEditor = editor;
    }

    // Create the dock widget for the editor
    ads::CDockWidget *dockWidget = dockManager->createDockWidget(editor->getName());

    // Disable elide, elided file names not readable when lots of files opened
    dockWidget->tabWidget()->setElideMode(Qt::ElideNone);

    // We need a unique object name. Can't use the name or file path so use a uuid
    dockWidget->setObjectName(QUuid::createUuid().toString());

    dockWidget->setWidget(editor);
    dockWidget->setFeature(ads::CDockWidget::DockWidgetFeature::DockWidgetDeleteOnClose, true);
    dockWidget->setFeature(ads::CDockWidget::DockWidgetFeature::CustomCloseHandling, true);
    dockWidget->setFeature(ads::CDockWidget::DockWidgetFeature::DockWidgetFloatable, false);

    dockWidget->tabWidget()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(dockWidget->tabWidget(), &QWidget::customContextMenuRequested, this, [this, editor](const QPoint &pos) {
        Q_UNUSED(pos)

        emit contextMenuRequestedForEditor(editor);
    });

    // Set the tooltip based on the buffer
    if (editor->isFile()) {
        dockWidget->tabWidget()->setToolTip(editor->getFilePath());
    } else {
        dockWidget->tabWidget()->setToolTip(editor->getName());
    }

    // Set the icon
    if (editor->readOnly()) {
        dockWidget->tabWidget()->setIcon(QIcon(":/icons/readonly.svg"));
    } else {
        dockWidget->tabWidget()->setIcon(QIcon(editor->canSaveToDisk() ? ":/icons/unsaved.svg" : ":/icons/saved.svg"));
        connect(editor, &ScintillaNext::savePointChanged, dockWidget, [editor, dockWidget](bool dirty) {
            Q_UNUSED(dirty)
            const bool actuallyDirty = editor->canSaveToDisk();
            const QString iconPath = actuallyDirty ? ":/icons/unsaved.svg" : ":/icons/saved.svg";
            dockWidget->tabWidget()->setIcon(QIcon(iconPath));
        });
    }

    connect(editor, &ScintillaNext::closed, dockWidget, &ads::CDockWidget::closeDockWidget);
    connect(editor, &ScintillaNext::closed, this, [this, editor] { emit editorClosed(editor); });
    connect(editor, &ScintillaNext::renamed, this, [this, editor] { editorRenamed(editor); });
    connect(editor, &ScintillaNext::savePointChanged, this, [this, editor](bool) { editorRenamed(editor); });
    connect(editor, &QObject::destroyed, this, [this](QObject *obj) {
        if (currentEditor == obj) {
            currentEditor = nullptr;
            auto edList = editors();
            edList.removeAll(static_cast<ScintillaNext *>(obj));
            if (!edList.isEmpty()) {
                currentEditor = edList.first();
                emit editorActivated(currentEditor.data());
            } else {
                emit editorActivated(nullptr);
            }
        }
    });

    connect(dockWidget, &ads::CDockWidget::closeRequested, this, &DockedEditor::dockWidgetCloseRequested);

    latestDockArea = dockManager->addDockWidget(ads::CenterDockWidgetArea, dockWidget, currentDockArea());

    const QColor initialColor = TabColorManager::instance()->tabColor(editor->getFilePath());
    if (initialColor.isValid()) {
        setEditorTabColor(editor, initialColor);
    }

    emit editorAdded(editor);
}

void DockedEditor::setEditorTabColor(ScintillaNext *editor, const QColor &color) {
    if (!editor) {
        return;
    }
    editor->setQtProperty("tabColor", color);
    auto *dockWidget = qobject_cast<ads::CDockWidget *>(editor->parentWidget());
    if (dockWidget && dockWidget->tabWidget()) {
        if (color.isValid()) {
            dockWidget->tabWidget()->setStyleSheet(
                QStringLiteral("border-bottom: 3px solid %1;").arg(color.name(QColor::HexRgb)));
        } else {
            dockWidget->tabWidget()->setStyleSheet(QString());
        }
    }
}

void DockedEditor::editorRenamed(const ScintillaNext *editor) const {
    Q_ASSERT(editor != Q_NULLPTR);

    ads::CDockWidget *dockWidget = qobject_cast<ads::CDockWidget *>(editor->parentWidget());
    if (!dockWidget) {
        return;
    }

    dockWidget->setWindowTitle(editor->getName());
    dockWidget->tabWidget()->setToolTip(buildEnrichedToolTip(editor));
}

void DockedEditor::splitToRight(const ScintillaNext *editor) {
    Q_ASSERT(editor != Q_NULLPTR);

    ads::CDockWidget *newDockWidget = qobject_cast<ads::CDockWidget *>(editor->parentWidget());
    if (newDockWidget) {
        ads::CDockAreaWidget *currentArea = currentDockArea();
        if (currentArea) {
            dockManager->addDockWidget(ads::RightDockWidgetArea, newDockWidget, currentArea);
        }
    }
}

void DockedEditor::splitToBottom(const ScintillaNext *editor) {
    Q_ASSERT(editor != Q_NULLPTR);

    ads::CDockWidget *newDockWidget = qobject_cast<ads::CDockWidget *>(editor->parentWidget());
    if (newDockWidget) {
        ads::CDockAreaWidget *currentArea = currentDockArea();
        if (currentArea) {
            dockManager->addDockWidget(ads::BottomDockWidgetArea, newDockWidget, currentArea);
        }
    }
}
