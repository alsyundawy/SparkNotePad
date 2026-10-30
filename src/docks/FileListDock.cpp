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

#include "FileListDock.h"
#include "ApplicationSettings.h"
#include "ui_FileListDock.h"

#include "MainWindow.h"

const ApplicationSetting sortByName{"FileList/SortByName", false};

FileListDock::FileListDock(MainWindow *parent)
    : QDockWidget(parent), ui(std::make_unique<Ui::FileList>()), window(parent) {
    qInfo(Q_FUNC_INFO);

    ui->setupUi(this);
    ui->btnSettings->addAction(ui->actionSortbyFileName);

    // Set the initial state
    const ApplicationSettings settings;
    ui->actionSortbyFileName->setChecked(settings.get(sortByName));

    // Track it if it changes
    connect(ui->actionSortbyFileName, &QAction::toggled, this, [this](bool b) {
        ApplicationSettings appSettings;
        appSettings.set(sortByName, b);
        refreshList();
    });

    connect(this, &QDockWidget::visibilityChanged, this, &FileListDock::onVisibilityChanged);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &FileListDock::itemClicked);
}

FileListDock::~FileListDock() = default;

void FileListDock::onVisibilityChanged(bool visible) {
    if (visible) {
        // Only get events when the dock is visible
        connect(window->getDockedEditor(), &DockedEditor::editorAdded, this, [this](const ScintillaNext *editor) {
            Q_UNUSED(editor)
            refreshList();
        });
        connect(window->getDockedEditor(), &DockedEditor::editorActivated, this, &FileListDock::selectCurrentEditor);
        connect(window->getDockedEditor(), &DockedEditor::editorClosed, this, &FileListDock::removeEditor);
        connect(window->getDockedEditor(), &DockedEditor::editorOrderChanged, this, &FileListDock::refreshList);

        refreshList();
        selectCurrentEditor();
    } else {
        // Disconnect all the editors
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            const QListWidgetItem *item = ui->listWidget->item(i);
            const ScintillaNext *other = item->data(Qt::UserRole).value<ScintillaNext *>();
            disconnect(other, Q_NULLPTR, this, Q_NULLPTR);
        }

        ui->listWidget->clear();
        disconnect(window->getDockedEditor(), Q_NULLPTR, this, Q_NULLPTR);
    }
}

void FileListDock::refreshList() {
    qInfo(Q_FUNC_INFO);

    ui->listWidget->clear();

    QVector<ScintillaNext *> editors = window->getDockedEditor()->editors();

    if (const ApplicationSettings settings; settings.get(sortByName)) {
        std::ranges::sort(editors, [](const ScintillaNext *e1, const ScintillaNext *e2) {
            return QString::compare(e1->getName(), e2->getName(), Qt::CaseInsensitive) < 0;
        });
    }

    for (ScintillaNext *editor : editors) {
        addEditor(editor);
    }

    selectCurrentEditor();
}

void FileListDock::addEditor(ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    auto *item = new QListWidgetItem(editor->getName(), ui->listWidget);
    item->setToolTip(editor->getToolTip());
    item->setIcon(QIcon(":/icons/saved.svg"));
    item->setData(Qt::UserRole, QVariant::fromValue(editor));
    item->setIcon(QIcon(editor->canSaveToDisk() ? ":/icons/unsaved.svg" : ":/icons/saved.svg"));

    // Need some notifications from the editor itself
    connect(editor, &ScintillaNext::savePointChanged, this, &FileListDock::editorSavePointChanged,
            Qt::UniqueConnection);
    connect(editor, &ScintillaNext::renamed, this, &FileListDock::editorRenamed, Qt::UniqueConnection);
}

void FileListDock::removeEditor(const ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    if (const QListWidgetItem *item = lookupItemByEditor(editor); item) {
        const std::unique_ptr<QListWidgetItem> itemHolder(ui->listWidget->takeItem(ui->listWidget->row(item)));
    }
}

void FileListDock::selectCurrentEditor() {
    qInfo(Q_FUNC_INFO);

    const ScintillaNext *editor = window->currentEditor();
    QListWidgetItem *item = lookupItemByEditor(editor);

    if (item) {
        ui->listWidget->setCurrentItem(item);
    }
}

void FileListDock::itemClicked(const QListWidgetItem *item) const {
    qInfo(Q_FUNC_INFO);

    const auto *editor = item->data(Qt::UserRole).value<ScintillaNext *>();
    window->getDockedEditor()->switchToEditor(editor);
}

void FileListDock::editorSavePointChanged(bool dirty) const {
    Q_UNUSED(dirty)
    qInfo(Q_FUNC_INFO);

    if (const auto *editor = qobject_cast<const ScintillaNext *>(sender()); editor) {
        if (QListWidgetItem *item = lookupItemByEditor(editor); item) {
            const QString iconPath = editor->canSaveToDisk() ? ":/icons/unsaved.svg" : ":/icons/saved.svg";
            item->setIcon(QIcon(iconPath));
        }
    }
}

void FileListDock::editorRenamed() const {
    qInfo(Q_FUNC_INFO);

    if (const auto *editor = qobject_cast<const ScintillaNext *>(sender()); editor) {
        if (QListWidgetItem *item = lookupItemByEditor(editor); item) {
            item->setText(editor->getName());
            item->setToolTip(editor->getToolTip());
        }
    }
}

QListWidgetItem *FileListDock::lookupItemByEditor(const ScintillaNext *editor) const {
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        const auto *other = item->data(Qt::UserRole).value<const ScintillaNext *>();

        if (other == editor) {
            return item;
        }
    }

    return Q_NULLPTR;
}
