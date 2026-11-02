#pragma once
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

#ifndef FILEBROWSERDOCK_H
#define FILEBROWSERDOCK_H

#include <QDockWidget>
#include <QFileInfo>
#include <QString>

#include <memory>

namespace Ui {
class FileBrowserDock;
}

class QFileSystemModel;
class QModelIndex;
class QPoint;

class FileBrowserDock : public QDockWidget {
    Q_OBJECT

public:
    explicit FileBrowserDock(QWidget *parent = nullptr);
    ~FileBrowserDock() override;

    void setCurrentDirectory(const QString &dirPath);
    const QString &currentDirectory() const;

signals:
    void fileOpenRequested(const QString &filePath);

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void onItemDoubleClicked(const QModelIndex &index);
    void onCustomContextMenu(const QPoint &pos);
    void onUpClicked();
    void onHomeClicked();
    void onRefreshClicked();
    void onPathReturnPressed();
    void onFilterChanged(const QString &filterText);
    void onSelectionChanged();
    void updateCommanderTheme();

private:
    void setupModel();
    void setupConnections();
    void setupCommanderBar();
    void updateStatus();
    void saveSettings() const;
    void restoreSettings();
    void handleNewFile(const QFileInfo &fileInfo);
    void handleNewFolder(const QFileInfo &fileInfo);
    void handleMoveToTrash(const QString &filePath);

    std::unique_ptr<Ui::FileBrowserDock> ui;
    QFileSystemModel *m_model{nullptr};
    QString m_currentPath;
    class QLabel *m_statusLabel{nullptr};
    class QWidget *m_bookmarkBar{nullptr};
    bool m_isUpdatingTheme{false};
};

#endif  // FILEBROWSERDOCK_H
