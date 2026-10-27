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

#include "FolderAsWorkspaceDock.h"
#include "ApplicationSettings.h"
#include "ui_FolderAsWorkspaceDock.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QToolButton>
#include <QUrl>
#include <qmenu.h>

ApplicationSetting<QString> rootPathSetting{"FolderAsWorkspace/RootPath"};  // NOSONAR(cpp:S5421)

namespace {
// QFileSystemModel's standard column order.
enum class FileSystemColumn : int { NameColumn = 0, SizeColumn, TypeColumn, DateColumn };
}  // namespace

FolderAsWorkspaceDock::FolderAsWorkspaceDock(QWidget *parent)
    : QDockWidget(parent), ui(new Ui::FolderAsWorkspaceDock), model(new QFileSystemModel(this)) {
    ui->setupUi(this);

    ui->treeView->setModel(model);

    auto *menu = new QMenu(ui->toolButton);
    menu->addAction(ui->actionShowSize);
    menu->addAction(ui->actionShowType);
    menu->addAction(ui->actionShowDateModified);
    menu->addSeparator();
    menu->addAction(ui->actionShowHidden);

    ui->toolButton->setMenu(menu);
    ui->toolButton->setPopupMode(QToolButton::MenuButtonPopup);

    auto bindColumnVisibility = [this](const QAction *action, int column) {
        ui->treeView->setColumnHidden(column, !action->isChecked());
        connect(action, &QAction::toggled, this,
                [this, column](bool checked) { ui->treeView->setColumnHidden(column, !checked); });
    };

    auto bindActionToSetting = [](QAction *action, const QString &key) {
        ApplicationSettings settings;

        action->setChecked(settings.value(key, false).toBool());
        connect(action, &QAction::toggled, [key](bool checked) {
            ApplicationSettings settings;
            settings.setValue(key, checked);
        });
    };

    auto bindColumn = [&](QAction *action, FileSystemColumn column, const QString &key) {
        bindColumnVisibility(action, static_cast<int>(column));
        bindActionToSetting(action, key);
    };

    using enum FileSystemColumn;
    bindColumn(ui->actionShowSize, SizeColumn, "FolderAsWorkspace/ShowSize");
    bindColumn(ui->actionShowType, TypeColumn, "FolderAsWorkspace/ShowType");
    bindColumn(ui->actionShowDateModified, DateColumn, "FolderAsWorkspace/ShowDateModified");

    connect(ui->actionShowHidden, &QAction::toggled, this, [this](bool showHidden) {
        auto filters = QDir::AllEntries | QDir::AllDirs | QDir::NoDotAndDotDot;
        if (showHidden)
            filters |= QDir::Hidden;
        model->setFilter(filters);
    });
    bindActionToSetting(ui->actionShowHidden, "FolderAsWorkspace/ShowHidden");

    // Default sorting
    ui->treeView->sortByColumn(0, Qt::AscendingOrder);

    // Interactive filter and controls
    connect(ui->filterEdit, &QLineEdit::textChanged, this, &FolderAsWorkspaceDock::onFilterTextChanged);
    connect(ui->btnRefresh, &QToolButton::clicked, this, &FolderAsWorkspaceDock::onRefreshClicked);
    connect(ui->btnCollapseAll, &QToolButton::clicked, this, &FolderAsWorkspaceDock::onCollapseAllClicked);

    // Context menu
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &FolderAsWorkspaceDock::onCustomContextMenu);

    connect(ui->treeView, &QTreeView::doubleClicked, this, [this](const QModelIndex &sourceIndex) {
        const QFileInfo info = model->fileInfo(sourceIndex);

        if (info.isFile()) {
            emit fileDoubleClicked(info.absoluteFilePath());
        }
    });

    // Configure header settings
    auto *header = ui->treeView->header();
    header->setSectionResizeMode(static_cast<int>(NameColumn), QHeaderView::Stretch);
    header->setSectionResizeMode(static_cast<int>(SizeColumn), QHeaderView::ResizeToContents);
    header->setSectionResizeMode(static_cast<int>(TypeColumn), QHeaderView::ResizeToContents);
    header->setSectionResizeMode(static_cast<int>(DateColumn), QHeaderView::ResizeToContents);

    ApplicationSettings settings;
    setRootPath(settings.get(rootPathSetting));
}

FolderAsWorkspaceDock::~FolderAsWorkspaceDock() {
    delete ui;
}

void FolderAsWorkspaceDock::setRootPath(const QString &dir) {
    ApplicationSettings settings;
    settings.set(rootPathSetting, dir);

    const QModelIndex sourceIndex = model->setRootPath(dir);
    ui->treeView->setRootIndex(sourceIndex);

    if (!dir.isEmpty()) {
        const QString projFilePath = QDir(dir).filePath(QStringLiteral(".sparkproject"));
        if (QFile::exists(projFilePath)) {
            QFile projFile(projFilePath);
            if (projFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QJsonDocument doc = QJsonDocument::fromJson(projFile.readAll());
                if (doc.isObject()) {
                    QString projName = doc.object().value(QStringLiteral("name")).toString();
                    if (!projName.isEmpty()) {
                        setWindowTitle(tr("Project: %1").arg(projName));
                        return;
                    }
                }
            }
        }
        QFileInfo fi(dir);
        setWindowTitle(tr("Workspace: %1").arg(fi.fileName()));
    } else {
        setWindowTitle(tr("Folder as Workspace"));
    }
}

QString FolderAsWorkspaceDock::rootPath() const {
    return model->rootPath();
}

void FolderAsWorkspaceDock::onFilterTextChanged(const QString &text) {
    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        model->setNameFilters(QStringList());
    } else {
        QString pattern = trimmed;
        if (!pattern.contains(QLatin1Char('*')) && !pattern.contains(QLatin1Char('?'))) {
            pattern = QStringLiteral("*%1*").arg(pattern);
        }
        model->setNameFilters(QStringList{pattern});
        model->setNameFilterDisables(false);
    }
}

void FolderAsWorkspaceDock::onRefreshClicked() {
    const QString current = rootPath();
    if (!current.isEmpty()) {
        setRootPath(current);
    }
}

void FolderAsWorkspaceDock::onCollapseAllClicked() {
    ui->treeView->collapseAll();
}

void FolderAsWorkspaceDock::onCustomContextMenu(const QPoint &pos) {
    const QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    const QFileInfo info = model->fileInfo(index);
    const QString filePath = info.absoluteFilePath();
    const QString root = rootPath();

    auto *menu = new QMenu(this);
    if (info.isFile()) {
        menu->addAction(tr("Open in SparkNotePad"), this, [this, filePath]() { emit fileDoubleClicked(filePath); });
        menu->addAction(tr("Open with System Application"), this,
                        [filePath]() { QDesktopServices::openUrl(QUrl::fromLocalFile(filePath)); });
    }

    menu->addAction(tr("Show in File Manager / Finder"), this, [info]() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir() ? info.absoluteFilePath() : info.absolutePath()));
    });

    menu->addSeparator();

    menu->addAction(tr("Copy Full Path"), this, [filePath]() { QApplication::clipboard()->setText(filePath); });

    if (!root.isEmpty() && filePath.startsWith(root)) {
        QString relative = QDir(root).relativeFilePath(filePath);
        menu->addAction(tr("Copy Relative Path"), this, [relative]() { QApplication::clipboard()->setText(relative); });
    }

    menu->popup(ui->treeView->viewport()->mapToGlobal(pos));
}
