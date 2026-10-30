#pragma once
/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 matepath explorer integration.
 * Source: zufuliu/Notepad4 & Notepad3 file browser concept
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef FILEBROWSERDOCK_H
#define FILEBROWSERDOCK_H

#include <QDockWidget>
#include <QString>

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

private slots:
    void onItemDoubleClicked(const QModelIndex &index);
    void onCustomContextMenu(const QPoint &pos);
    void onUpClicked();
    void onHomeClicked();
    void onRefreshClicked();
    void onPathReturnPressed();
    void onFilterChanged(const QString &filterText);

private:
    void setupModel();
    void setupConnections();
    void saveSettings();
    void restoreSettings();

    Ui::FileBrowserDock *ui;
    QFileSystemModel *m_model{nullptr};
    QString m_currentPath;
};

#endif  // FILEBROWSERDOCK_H
