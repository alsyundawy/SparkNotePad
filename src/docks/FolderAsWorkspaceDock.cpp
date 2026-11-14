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
#include "../features/ThemeManager.h"
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
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QUrl>
#include <qmenu.h>

ApplicationSetting<QString> rootPathSetting{"FolderAsWorkspace/RootPath"};  // NOSONAR(cpp:S5421)

namespace {
// QFileSystemModel's standard column order.
enum class FileSystemColumn : int { NameColumn = 0, SizeColumn, TypeColumn, DateColumn };

QString readProjectName(const QString &dir) {
    if (const QString projFilePath = QDir(dir).filePath(QStringLiteral(".sparkproject")); QFile::exists(projFilePath)) {
        QFile projFile(projFilePath);
        if (projFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            const QJsonDocument doc = QJsonDocument::fromJson(projFile.readAll());
            if (doc.isObject()) {
                return doc.object().value(QStringLiteral("name")).toString();
            }
        }
    }
    return {};
}

struct RenameValidationResult {
    QString errorMsg;
    QString targetPath;
    bool isOk = false;
};

RenameValidationResult validateRenameTarget(const QString &rawName, const QString &currentName,
                                            const QString &parentDir) {
    RenameValidationResult res;
    const QString newName = rawName.trimmed();
    if (newName.isEmpty()) {
        res.errorMsg = QObject::tr("Name cannot be empty.");
        return res;
    }
    if (newName.contains(QLatin1Char('/')) || newName.contains(QLatin1Char('\\')) ||
        newName.contains(QStringLiteral(".."))) {
        res.errorMsg = QObject::tr("Invalid characters or path traversal sequence in name.");
        return res;
    }
    res.targetPath = QDir(parentDir).filePath(newName);
    if (newName == currentName) {
        res.errorMsg = QObject::tr("Name is unchanged.");
        return res;
    }
    if (QFileInfo::exists(res.targetPath)) {
        res.errorMsg = QObject::tr("A file or folder with this name already exists.");
        return res;
    }
    res.isOk = true;
    return res;
}
}  // namespace

FolderAsWorkspaceDock::FolderAsWorkspaceDock(QWidget *parent)
    : QDockWidget(parent), ui(std::make_unique<Ui::FolderAsWorkspaceDock>()), model(new QFileSystemModel(this)) {
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
        const ApplicationSettings settings;

        action->setChecked(settings.value(key, false).toBool());
        connect(action, &QAction::toggled, [key](bool checked) {
            ApplicationSettings settings;
            settings.setValue(key, checked);
        });
    };

    const auto bindColumn = [&](QAction *action, FileSystemColumn column, const QString &key) {
        bindColumnVisibility(action, static_cast<int>(column));
        bindActionToSetting(action, key);
    };

    using enum FileSystemColumn;
    bindColumn(ui->actionShowSize, SizeColumn, "FolderAsWorkspace/ShowSize");
    bindColumn(ui->actionShowType, TypeColumn, "FolderAsWorkspace/ShowType");
    bindColumn(ui->actionShowDateModified, DateColumn, "FolderAsWorkspace/ShowDateModified");

    connect(ui->actionShowHidden, &QAction::toggled, this, [this](bool showHidden) {
        auto filters = QDir::AllEntries | QDir::AllDirs | QDir::NoDotAndDotDot;
        if (showHidden) {
            filters |= QDir::Hidden;
        }
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

    const ApplicationSettings settings;
    setRootPath(settings.get(rootPathSetting));

    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this, &FolderAsWorkspaceDock::updateTheme);
    connect(ThemeManager::instance(), &ThemeManager::paletteChanged, this, &FolderAsWorkspaceDock::updateTheme);
    updateTheme();
}

FolderAsWorkspaceDock::~FolderAsWorkspaceDock() = default;

void FolderAsWorkspaceDock::setRootPath(const QString &dir) {
    ApplicationSettings settings;
    settings.set(rootPathSetting, dir);

    const QModelIndex sourceIndex = model->setRootPath(dir);
    ui->treeView->setRootIndex(sourceIndex);

    if (dir.isEmpty()) {
        setWindowTitle(tr("Folder as Workspace"));
        return;
    }

    if (const QString projName = readProjectName(dir); !projName.isEmpty()) {
        setWindowTitle(tr("Project: %1").arg(projName));
        return;
    }

    const QFileInfo fi(dir);
    setWindowTitle(tr("Workspace: %1").arg(fi.fileName()));
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
        menu->addAction(tr("Open in SparkNotePad"), this, [this, filePath] { emit fileDoubleClicked(filePath); });
        menu->addAction(tr("Open with System Application"), this,
                        [filePath] { QDesktopServices::openUrl(QUrl::fromLocalFile(filePath)); });
    }

    menu->addAction(tr("Show in File Manager / Finder"), this, [info] {
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir() ? info.absoluteFilePath() : info.absolutePath()));
    });

    menu->addSeparator();

    menu->addAction(tr("Rename..."), this, [this, filePath] { renameItemWithPreview(filePath); });

    menu->addAction(tr("Copy Full Path"), this, [filePath] { QApplication::clipboard()->setText(filePath); });

    if (!root.isEmpty() && filePath.startsWith(root)) {
        const QString relative = QDir(root).relativeFilePath(filePath);
        menu->addAction(tr("Copy Relative Path"), this, [relative] { QApplication::clipboard()->setText(relative); });
    }

    menu->popup(ui->treeView->viewport()->mapToGlobal(pos));
}

void FolderAsWorkspaceDock::changeEvent(QEvent *event) {
    if (event->type() == QEvent::PaletteChange) {
        updateTheme();
    }
    QDockWidget::changeEvent(event);
}

void FolderAsWorkspaceDock::updateTheme() {
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
    const QString inactiveSel =
        isDark ? QStringLiteral("rgba(56, 139, 253, 0.25)") : QStringLiteral("rgba(9, 105, 218, 0.15)");

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
                              "QTreeView {"
                              "  background-color: %1;"
                              "  color: %2;"
                              "  border: 1px solid %3;"
                              "  border-radius: 4px;"
                              "  font-size: 11px;"
                              "  show-decoration-selected: 1;"
                              "  selection-background-color: %4;"
                              "  selection-color: #ffffff;"
                              "  outline: none;"
                              "}"
                              "QTreeView::item {"
                              "  min-height: 22px;"
                              "  padding: 2px 4px;"
                              "  border-radius: 4px;"
                              "}"
                              "QTreeView::item:hover {"
                              "  background-color: %3;"
                              "}"
                              "QTreeView::item:selected {"
                              "  background-color: %4;"
                              "  color: #ffffff;"
                              "}"
                              "QTreeView::item:selected:!active {"
                              "  background-color: %8;"
                              "  color: %2;"
                              "}"
                              "QTreeView::branch:selected {"
                              "  background-color: %4;"
                              "}"
                              "QTreeView::branch:selected:!active {"
                              "  background-color: %8;"
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
                              "}")
                              .arg(cardBg, textPri, border, accent, btnBg, textSec, dockBg, inactiveSel);

    setStyleSheet(style);
    m_isUpdatingTheme = false;
}

void FolderAsWorkspaceDock::renameItemWithPreview(const QString &filePath) {
    const QFileInfo info(filePath);
    if (!info.exists()) {
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(info.isDir() ? tr("Rename Folder — Preview & Validate")
                                    : tr("Rename File — Preview & Validate"));
    dlg.resize(460, 240);

    auto *layout = new QVBoxLayout(&dlg);
    layout->addWidget(new QLabel(tr("<b>Current Path:</b> %1").arg(info.absoluteFilePath()), &dlg));

    auto *nameEdit = new QLineEdit(&dlg);
    nameEdit->setText(info.fileName());
    nameEdit->selectAll();
    layout->addWidget(new QLabel(tr("New Name:"), &dlg));
    layout->addWidget(nameEdit);

    auto *previewLabel = new QLabel(&dlg);
    previewLabel->setStyleSheet(QStringLiteral("color: #0969da; font-size: 11px;"));
    layout->addWidget(previewLabel);

    auto *statusLabel = new QLabel(&dlg);
    statusLabel->setStyleSheet(QStringLiteral("color: #cf222e; font-size: 11px;"));
    layout->addWidget(statusLabel);

    auto *btnLayout = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    btnLayout->addStretch();
    auto *btnRename = new QPushButton(tr("Rename"), &dlg);
    btnRename->setDefault(true);
    auto *btnCancel = new QPushButton(tr("Cancel"), &dlg);
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    btnLayout->addWidget(btnRename);
    btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);

    const QString parentDir = info.dir().absolutePath();
    const QString currentName = info.fileName();

    const auto validate = [nameEdit, statusLabel, previewLabel, btnRename, parentDir, currentName] {
        const auto res = validateRenameTarget(nameEdit->text(), currentName, parentDir);
        if (!res.isOk) {
            statusLabel->setText(res.errorMsg);
            statusLabel->setStyleSheet(QStringLiteral("color: #cf222e; font-size: 11px;"));
            if (res.targetPath.isEmpty()) {
                previewLabel->clear();
            } else {
                previewLabel->setText(QObject::tr("Target: %1").arg(res.targetPath));
            }
            btnRename->setEnabled(false);
        } else {
            previewLabel->setText(QObject::tr("Target: %1").arg(res.targetPath));
            statusLabel->setText(QObject::tr("Ready to rename (no conflicts)."));
            statusLabel->setStyleSheet(QStringLiteral("color: #1a7f37; font-size: 11px;"));
            btnRename->setEnabled(true);
        }
    };

    connect(nameEdit, &QLineEdit::textChanged, &dlg, validate);
    validate();

    connect(btnRename, &QPushButton::clicked, &dlg, [&dlg, &info, filePath, nameEdit, parentDir, statusLabel] {
        const QString newName = nameEdit->text().trimmed();
        const QString targetPath = QDir(parentDir).filePath(newName);
        bool ok = false;
        if (info.isDir()) {
            ok = QDir().rename(filePath, targetPath);
        } else {
            ok = QFile::rename(filePath, targetPath);
        }
        if (ok) {
            dlg.accept();
        } else {
            statusLabel->setText(QObject::tr("Failed to rename on disk. Check file permissions."));
            statusLabel->setStyleSheet(QStringLiteral("color: #cf222e; font-size: 11px;"));
        }
    });

    dlg.exec();
}
