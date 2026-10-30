/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 matepath explorer integration.
 * Source: zufuliu/Notepad4 & Notepad3 file browser concept
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "FileBrowserDock.h"
#include "ui_FileBrowserDock.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QGuiApplication>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>

FileBrowserDock::FileBrowserDock(QWidget *parent)
    : QDockWidget(parent), ui(new Ui::FileBrowserDock), m_model(new QFileSystemModel(this)) {
    ui->setupUi(this);
    setupModel();
    setupConnections();
    restoreSettings();
}

FileBrowserDock::~FileBrowserDock() {
    saveSettings();
    delete ui;
}

void FileBrowserDock::setupModel() {
    m_model->setRootPath(QStringLiteral(""));
    m_model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs);
    m_model->setNameFilterDisables(false);

    ui->treeView->setModel(m_model);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setColumnWidth(0, 200);
}

void FileBrowserDock::setupConnections() {
    connect(ui->treeView, &QTreeView::doubleClicked, this, &FileBrowserDock::onItemDoubleClicked);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &FileBrowserDock::onCustomContextMenu);

    connect(ui->btnUp, &QToolButton::clicked, this, &FileBrowserDock::onUpClicked);
    connect(ui->btnHome, &QToolButton::clicked, this, &FileBrowserDock::onHomeClicked);
    connect(ui->btnRefresh, &QToolButton::clicked, this, &FileBrowserDock::onRefreshClicked);

    connect(ui->pathLineEdit, &QLineEdit::returnPressed, this, &FileBrowserDock::onPathReturnPressed);
    connect(ui->filterLineEdit, &QLineEdit::textChanged, this, &FileBrowserDock::onFilterChanged);
}

void FileBrowserDock::setCurrentDirectory(const QString &dirPath) {
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir = QDir::home();
    }

    m_currentPath = dir.absolutePath();
    ui->pathLineEdit->setText(m_currentPath);

    const QModelIndex index = m_model->index(m_currentPath);
    if (index.isValid()) {
        ui->treeView->setRootIndex(index);
    }
}

const QString &FileBrowserDock::currentDirectory() const {
    return m_currentPath;
}

void FileBrowserDock::onItemDoubleClicked(const QModelIndex &index) {
    if (!index.isValid())
        return;

    const QString path = m_model->filePath(index);
    const QFileInfo fileInfo(path);

    if (fileInfo.isDir()) {
        setCurrentDirectory(path);
    } else if (fileInfo.isFile()) {
        emit fileOpenRequested(path);
    }
}

void FileBrowserDock::onUpClicked() {
    QDir dir(m_currentPath);
    if (dir.cdUp()) {
        setCurrentDirectory(dir.absolutePath());
    }
}

void FileBrowserDock::onHomeClicked() {
    setCurrentDirectory(QDir::homePath());
}

void FileBrowserDock::onRefreshClicked() {
    m_model->setRootPath(QStringLiteral(""));
    const QModelIndex index = m_model->index(m_currentPath);
    if (index.isValid()) {
        ui->treeView->setRootIndex(index);
    }
}

void FileBrowserDock::onPathReturnPressed() {
    const QString enteredPath = ui->pathLineEdit->text().trimmed();
    if (!enteredPath.isEmpty()) {
        setCurrentDirectory(enteredPath);
    }
}

void FileBrowserDock::onFilterChanged(const QString &filterText) {
    if (filterText.trimmed().isEmpty()) {
        m_model->setNameFilters(QStringList());
    } else {
        const QStringList rawFilters = filterText.split(QLatin1Char(';'), Qt::SkipEmptyParts);
        QStringList cleanedFilters;
        cleanedFilters.reserve(rawFilters.size());
        std::transform(rawFilters.cbegin(), rawFilters.cend(), std::back_inserter(cleanedFilters),
                       [](const QString &f) { return f.trimmed(); });
        m_model->setNameFilters(cleanedFilters);
    }
}

void FileBrowserDock::onCustomContextMenu(const QPoint &pos) {
    const QModelIndex index = ui->treeView->indexAt(pos);
    const QString filePath = index.isValid() ? m_model->filePath(index) : m_currentPath;
    const QFileInfo fileInfo(filePath);

    QMenu menu(this);

    if (fileInfo.isFile()) {
        QAction *openAct = menu.addAction(tr("Open in SparkNotePad"));
        connect(openAct, &QAction::triggered, this, [this, filePath]() { emit fileOpenRequested(filePath); });
    } else if (fileInfo.isDir()) {
        QAction *enterAct = menu.addAction(tr("Enter Directory"));
        connect(enterAct, &QAction::triggered, this, [this, filePath]() { setCurrentDirectory(filePath); });
    }

    menu.addSeparator();

#if defined(Q_OS_MACOS)
    QAction *revealAct = menu.addAction(tr("Reveal in Finder"));
#else
    QAction *revealAct = menu.addAction(tr("Reveal in File Manager"));
#endif
    connect(revealAct, &QAction::triggered, this, [fileInfo]() {
        const QString dirToOpen = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(dirToOpen));
    });

    menu.addSeparator();

    QAction *copyPathAct = menu.addAction(tr("Copy Full Path"));
    connect(copyPathAct, &QAction::triggered, this, [filePath]() { QGuiApplication::clipboard()->setText(filePath); });

    QAction *copyNameAct = menu.addAction(tr("Copy Name"));
    connect(copyNameAct, &QAction::triggered, this,
            [fileInfo]() { QGuiApplication::clipboard()->setText(fileInfo.fileName()); });

    menu.addSeparator();

    // New File
    QAction *newFileAct = menu.addAction(tr("New File..."));
    connect(newFileAct, &QAction::triggered, this, [this, fileInfo]() {
        const QString targetDir = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath();
        bool ok = false;
        const QString fileName =
            QInputDialog::getText(this, tr("New File"), tr("Enter file name:"), QLineEdit::Normal, QString(), &ok);
        if (ok && !fileName.trimmed().isEmpty()) {
            QFile newFile(QDir(targetDir).filePath(fileName.trimmed()));
            if (newFile.open(QIODevice::WriteOnly)) {
                newFile.close();
                onRefreshClicked();
                emit fileOpenRequested(newFile.fileName());
            } else {
                QMessageBox::warning(this, tr("Error"), tr("Could not create file: %1").arg(newFile.errorString()));
            }
        }
    });

    // New Folder
    QAction *newFolderAct = menu.addAction(tr("New Folder..."));
    connect(newFolderAct, &QAction::triggered, this, [this, fileInfo]() {
        const QString targetDir = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath();
        bool ok = false;
        const QString folderName =
            QInputDialog::getText(this, tr("New Folder"), tr("Enter folder name:"), QLineEdit::Normal, QString(), &ok);
        if (ok && !folderName.trimmed().isEmpty()) {
            QDir dir(targetDir);
            if (dir.mkdir(folderName.trimmed())) {
                onRefreshClicked();
            } else {
                QMessageBox::warning(this, tr("Error"), tr("Could not create folder."));
            }
        }
    });

    menu.addSeparator();

    // Delete / Trash
    if (index.isValid()) {
        QAction *trashAct = menu.addAction(tr("Move to Trash"));
        connect(trashAct, &QAction::triggered, this, [this, filePath]() {
            int ret = QMessageBox::question(
                this, tr("Confirm Delete"),
                tr("Are you sure you want to move \"%1\" to trash?").arg(QFileInfo(filePath).fileName()),
                QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                if (QFile::moveToTrash(filePath)) {
                    onRefreshClicked();
                } else {
                    QMessageBox::warning(this, tr("Error"), tr("Could not move to trash."));
                }
            }
        });
    }

    menu.exec(ui->treeView->viewport()->mapToGlobal(pos));
}

void FileBrowserDock::saveSettings() {
    QSettings settings;
    settings.setValue(QStringLiteral("FileBrowser/CurrentPath"), m_currentPath);
}

void FileBrowserDock::restoreSettings() {
    QSettings settings;
    const QString savedPath = settings.value(QStringLiteral("FileBrowser/CurrentPath"), QDir::currentPath()).toString();
    setCurrentDirectory(savedPath);
}
