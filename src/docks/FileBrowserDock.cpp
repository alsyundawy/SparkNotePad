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
#include "../features/ThemeManager.h"
#include "../features/ThemePaletteData.h"

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
#include <QStorageInfo>
#include <QStandardPaths>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QEvent>

#include <array>
#include <memory>

FileBrowserDock::FileBrowserDock(QWidget *parent)
    : QDockWidget(parent), ui(std::make_unique<Ui::FileBrowserDock>()), m_model(new QFileSystemModel(this)) {
    ui->setupUi(this);
    setupModel();
    setupCommanderBar();
    setupConnections();
    restoreSettings();
    updateCommanderTheme();
    updateStatus();
}

FileBrowserDock::~FileBrowserDock() {
    saveSettings();
}

void FileBrowserDock::setupModel() {
    m_model->setRootPath(QStringLiteral(""));
    m_model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs);
    m_model->setNameFilterDisables(false);

    ui->treeView->setModel(m_model);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setColumnWidth(0, 190);
    ui->treeView->setColumnWidth(1, 75);
    ui->treeView->setColumnWidth(2, 75);
    ui->treeView->setColumnWidth(3, 125);
}

void FileBrowserDock::setupCommanderBar() {
    m_bookmarkBar = new QWidget(this);
    m_bookmarkBar->setObjectName(QStringLiteral("commanderBookmarkBar"));
    auto *barLayout = new QHBoxLayout(m_bookmarkBar);
    barLayout->setContentsMargins(0, 2, 0, 2);
    barLayout->setSpacing(4);

    struct BookmarkItem {
        QString label;
        QString path;
        QString toolTip;
    };

    const std::array<BookmarkItem, 5> bookmarks = {{
        {tr("🏠 Home"), QDir::homePath(), tr("Home Directory")},
        {tr("📁 Docs"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), tr("Documents")},
        {tr("⬇️ Downloads"), QStandardPaths::writableLocation(QStandardPaths::DownloadLocation), tr("Downloads")},
        {tr("💻 Desktop"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), tr("Desktop")},
        {tr("💼 Workspace"), QDir::currentPath(), tr("Workspace Directory")},
    }};

    for (const auto &bm : bookmarks) {
        if (bm.path.isEmpty() || !QDir(bm.path).exists()) {
            continue;
        }
        auto *btn = new QPushButton(bm.label, m_bookmarkBar);
        btn->setToolTip(QStringLiteral("%1: %2").arg(bm.toolTip, bm.path));
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, this, [this, path = bm.path]() { setCurrentDirectory(path); });
        barLayout->addWidget(btn);
    }

    if (const int filterIdx = ui->verticalLayout->indexOf(ui->filterLineEdit); filterIdx >= 0) {
        ui->verticalLayout->insertWidget(filterIdx, m_bookmarkBar);
    } else {
        ui->verticalLayout->insertWidget(1, m_bookmarkBar);
    }

    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName(QStringLiteral("commanderStatusBar"));
    m_statusLabel->setText(tr("Ready"));
    m_statusLabel->setWordWrap(false);
    ui->verticalLayout->addWidget(m_statusLabel);
}

void FileBrowserDock::setupConnections() {
    connect(ui->treeView, &QTreeView::doubleClicked, this, &FileBrowserDock::onItemDoubleClicked);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &FileBrowserDock::onCustomContextMenu);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &FileBrowserDock::onSelectionChanged);

    ui->btnUp->setIcon(QIcon(QStringLiteral(":/icons/arrow_up.svg")));
    ui->btnUp->setText(QString());
    ui->btnHome->setIcon(QIcon(QStringLiteral(":/icons/folder.svg")));
    ui->btnHome->setText(QString());
    ui->btnRefresh->setIcon(QIcon(QStringLiteral(":/icons/restart.svg")));
    ui->btnRefresh->setText(QString());

    connect(ui->btnUp, &QToolButton::clicked, this, &FileBrowserDock::onUpClicked);
    connect(ui->btnHome, &QToolButton::clicked, this, &FileBrowserDock::onHomeClicked);
    connect(ui->btnRefresh, &QToolButton::clicked, this, &FileBrowserDock::onRefreshClicked);

    connect(ui->pathLineEdit, &QLineEdit::returnPressed, this, &FileBrowserDock::onPathReturnPressed);
    connect(ui->filterLineEdit, &QLineEdit::textChanged, this, &FileBrowserDock::onFilterChanged);

    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this, &FileBrowserDock::updateCommanderTheme);
    connect(ThemeManager::instance(), &ThemeManager::paletteChanged, this, &FileBrowserDock::updateCommanderTheme);
}

void FileBrowserDock::changeEvent(QEvent *event) {
    if (event->type() == QEvent::PaletteChange) {
        updateCommanderTheme();
    }
    QDockWidget::changeEvent(event);
}

void FileBrowserDock::setCurrentDirectory(const QString &dirPath) {
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir = QDir::home();
    }

    m_currentPath = dir.absolutePath();
    ui->pathLineEdit->setText(m_currentPath);

    if (const QModelIndex index = m_model->index(m_currentPath); index.isValid()) {
        ui->treeView->setRootIndex(index);
    }

    updateStatus();
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
        std::ranges::transform(rawFilters, std::back_inserter(cleanedFilters),
                               [](const QString &f) { return f.trimmed(); });
        m_model->setNameFilters(cleanedFilters);
    }
}

void FileBrowserDock::handleNewFile(const QFileInfo &fileInfo) {
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
}

void FileBrowserDock::handleNewFolder(const QFileInfo &fileInfo) {
    const QString targetDir = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath();
    bool ok = false;
    const QString folderName =
        QInputDialog::getText(this, tr("New Folder"), tr("Enter folder name:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !folderName.trimmed().isEmpty()) {
        const QDir dir(targetDir);
        if (dir.mkdir(folderName.trimmed())) {
            onRefreshClicked();
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Could not create folder."));
        }
    }
}

void FileBrowserDock::handleMoveToTrash(const QString &filePath) {
    const int ret =
        QMessageBox::question(this, tr("Confirm Delete"),
                              tr("Are you sure you want to move \"%1\" to trash?").arg(QFileInfo(filePath).fileName()),
                              QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (QFile::moveToTrash(filePath)) {
            onRefreshClicked();
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Could not move to trash."));
        }
    }
}

void FileBrowserDock::onCustomContextMenu(const QPoint &pos) {
    const QModelIndex index = ui->treeView->indexAt(pos);
    const QString filePath = index.isValid() ? m_model->filePath(index) : m_currentPath;
    const QFileInfo fileInfo(filePath);

    QMenu menu(this);

    if (fileInfo.isFile()) {
        const QAction *openAct = menu.addAction(tr("Open in SparkNotePad"));
        connect(openAct, &QAction::triggered, this, [this, filePath]() { emit fileOpenRequested(filePath); });
    } else if (fileInfo.isDir()) {
        const QAction *enterAct = menu.addAction(tr("Enter Directory"));
        connect(enterAct, &QAction::triggered, this, [this, filePath]() { setCurrentDirectory(filePath); });
    }

    menu.addSeparator();

#if defined(Q_OS_MACOS)
    const QAction *revealAct = menu.addAction(tr("Reveal in Finder"));
#else
    const QAction *revealAct = menu.addAction(tr("Reveal in File Manager"));
#endif
    connect(revealAct, &QAction::triggered, this, [fileInfo]() {
        const QString dirToOpen = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(dirToOpen));
    });

    menu.addSeparator();

    const QAction *copyPathAct = menu.addAction(tr("Copy Full Path"));
    connect(copyPathAct, &QAction::triggered, this, [filePath]() { QGuiApplication::clipboard()->setText(filePath); });

    const QAction *copyNameAct = menu.addAction(tr("Copy Name"));
    connect(copyNameAct, &QAction::triggered, this,
            [fileInfo]() { QGuiApplication::clipboard()->setText(fileInfo.fileName()); });

    menu.addSeparator();

    // New File
    const QAction *newFileAct = menu.addAction(tr("New File..."));
    connect(newFileAct, &QAction::triggered, this, [this, fileInfo]() { handleNewFile(fileInfo); });

    // New Folder
    const QAction *newFolderAct = menu.addAction(tr("New Folder..."));
    connect(newFolderAct, &QAction::triggered, this, [this, fileInfo]() { handleNewFolder(fileInfo); });

    menu.addSeparator();

    // Delete / Trash
    if (index.isValid()) {
        const QAction *trashAct = menu.addAction(tr("Move to Trash"));
        connect(trashAct, &QAction::triggered, this, [this, filePath]() { handleMoveToTrash(filePath); });
    }

    menu.exec(ui->treeView->viewport()->mapToGlobal(pos));
}

void FileBrowserDock::saveSettings() const {
    QSettings settings;
    settings.setValue(QStringLiteral("FileBrowser/CurrentPath"), m_currentPath);
}

void FileBrowserDock::restoreSettings() {
    QSettings settings;
    const QString savedPath = settings.value(QStringLiteral("FileBrowser/CurrentPath"), QDir::currentPath()).toString();
    setCurrentDirectory(savedPath);
}

void FileBrowserDock::onSelectionChanged() {
    const QModelIndexList selected = ui->treeView->selectionModel()->selectedIndexes();
    if (selected.isEmpty()) {
        updateStatus();
        return;
    }

    const QModelIndex idx = selected.first();
    const QString path = m_model->filePath(idx);
    const QFileInfo fi(path);

    if (fi.isFile()) {
        const double kb = static_cast<double>(fi.size()) / 1024.0;
        const QString sizeStr = kb >= 1024.0 ? QString::number(kb / 1024.0, 'f', 2) + QStringLiteral(" MB")
                                             : QString::number(kb, 'f', 1) + QStringLiteral(" KB");
        m_statusLabel->setText(
            QStringLiteral("%1  (%2)  •  %3")
                .arg(fi.fileName(), sizeStr, fi.lastModified().toString(QStringLiteral("yyyy-MM-dd hh:mm"))));
    } else if (fi.isDir()) {
        QDir d(path);
        const auto entryCount = d.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).size();
        m_statusLabel->setText(QStringLiteral("📁 %1  (%2 items)").arg(fi.fileName(), QString::number(entryCount)));
    }
}

void FileBrowserDock::updateStatus() {
    if (!m_statusLabel) {
        return;
    }

    const QDir dir(m_currentPath);
    const auto files = static_cast<int>(dir.entryList(QDir::Files).size());
    const auto dirs = static_cast<int>(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size());

    const QStorageInfo storage(m_currentPath);
    QString freeStr;
    if (storage.isValid() && storage.isReady()) {
        const double freeGb = static_cast<double>(storage.bytesAvailable()) / (1024.0 * 1024.0 * 1024.0);
        freeStr = tr("Free: %1 GB").arg(QString::number(freeGb, 'f', 1));
    }

    if (!freeStr.isEmpty()) {
        m_statusLabel->setText(tr("%1 files, %2 folders  •  %3").arg(files).arg(dirs).arg(freeStr));
    } else {
        m_statusLabel->setText(tr("%1 files, %2 folders").arg(files).arg(dirs));
    }
}

void FileBrowserDock::updateCommanderTheme() {
    if (m_isUpdatingTheme) {
        return;
    }
    m_isUpdatingTheme = true;

    const bool isDark = ThemeManager::instance()->isDarkActive();
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();

    const QString border = isDark ? QStringLiteral("#30363d") : QStringLiteral("#d0d7de");
    const QString textPri = isDark ? QStringLiteral("#e6edf3") : QStringLiteral("#1f2328");
    const QString textSec = isDark ? QStringLiteral("#8b949e") : QStringLiteral("#57606a");
    const QString accent = pal ? QString::fromLatin1(pal->blue) : QStringLiteral("#0969da");
    const QString cardBg = isDark ? QStringLiteral("#0d1117") : QStringLiteral("#ffffff");
    const QString btnBg = isDark ? QStringLiteral("#21262d") : QStringLiteral("#f6f8fa");
    const QString dockBg = isDark ? QStringLiteral("#161b22") : QStringLiteral("#f6f8fa");

    const QString style = QStringLiteral(
                              "QDockWidget {"
                              "  background-color: %7;"
                              "  color: %2;"
                              "}"
                              "QDockWidget::title {"
                              "  background-color: %7;"
                              "  color: %2;"
                              "  padding: 6px 10px;"
                              "  border-bottom: 2px solid %4;"
                              "  font-weight: 600;"
                              "  font-size: 11px;"
                              "}"
                              "QLineEdit {"
                              "  background-color: %1;"
                              "  color: %2;"
                              "  border: 1px solid %3;"
                              "  border-radius: 4px;"
                              "  padding: 3px 6px;"
                              "  font-size: 11px;"
                              "}"
                              "QLineEdit:focus {"
                              "  border-color: %4;"
                              "}"
                              "QToolButton {"
                              "  background-color: %5;"
                              "  color: %2;"
                              "  border: 1px solid %3;"
                              "  border-radius: 4px;"
                              "  padding: 3px 5px;"
                              "  font-size: 11px;"
                              "}"
                              "QToolButton:hover {"
                              "  border-color: %4;"
                              "}"
                              "QWidget#commanderBookmarkBar QPushButton {"
                              "  background-color: %5;"
                              "  color: %2;"
                              "  border: 1px solid %3;"
                              "  border-radius: 3px;"
                              "  padding: 2px 6px;"
                              "  font-size: 10px;"
                              "  font-weight: 500;"
                              "}"
                              "QWidget#commanderBookmarkBar QPushButton:hover {"
                              "  border-color: %4;"
                              "  color: %4;"
                              "}"
                              "QTreeView {"
                              "  background-color: %1;"
                              "  color: %2;"
                              "  border: 1px solid %3;"
                              "  border-radius: 4px;"
                              "  font-size: 11px;"
                              "}"
                              "QTreeView::item:hover {"
                              "  background-color: %3;"
                              "}"
                              "QTreeView::item:selected {"
                              "  background-color: %4;"
                              "  color: #ffffff;"
                              "}"
                              "QHeaderView::section {"
                              "  background-color: %5;"
                              "  color: %6;"
                              "  border: none;"
                              "  border-right: 1px solid %3;"
                              "  border-bottom: 1px solid %3;"
                              "  padding: 2px 6px;"
                              "  font-size: 10px;"
                              "  font-weight: 600;"
                              "}"
                              "QLabel#commanderStatusBar {"
                              "  background-color: %5;"
                              "  color: %6;"
                              "  border: 1px solid %3;"
                              "  border-radius: 4px;"
                              "  padding: 3px 8px;"
                              "  font-size: 10px;"
                              "  font-weight: 500;"
                              "}")
                              .arg(cardBg, textPri, border, accent, btnBg, textSec, dockBg);

    setStyleSheet(style);
    m_isUpdatingTheme = false;
}
