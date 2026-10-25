#pragma once
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

#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QStatusBar>
#include <QTabBar>
#include <cstdint>
#include <memory>

#include "Finder.h"
#include "ISearchResultsHandler.h"

class ScintillaNext;
class MainWindow;
class BookMarkDecorator;

namespace Ui {
class FindReplaceDialog;
}

class FindReplaceDialog : public QDialog {  // NOSONAR(cpp:S1448)
    Q_OBJECT

public:
    enum class TabIndex : std::uint8_t { Find = 0, Replace = 1, Mark = 2 };
    static constexpr int FIND_TAB = static_cast<int>(TabIndex::Find);
    static constexpr int REPLACE_TAB = static_cast<int>(TabIndex::Replace);
    static constexpr int MARK_TAB = static_cast<int>(TabIndex::Mark);

    enum class SearchDirection : std::uint8_t { Forwards, Backwards };

    explicit FindReplaceDialog(ISearchResultsHandler *searchResults, MainWindow *window = nullptr);
    ~FindReplaceDialog() override;

    void setFindString(const QString &string);
    void setTab(int tab);

    QString replaceString() const;

    void setSearchResultsHandler(ISearchResultsHandler *searchResultsHandler);

protected:
    bool event(QEvent *dialogEvent) override;
    void showEvent(QShowEvent *showEvt) override;
    void closeEvent(QCloseEvent *closeEvt) override;

signals:
    void windowActivated();
    void windowDeactivated();

public slots:
    void performNextSearch();
    void performPrevSearch();

    void find();
    void findAllInCurrentDocument();
    void findAllInDocuments();
    void count();
    void replace();
    void replaceAll();
    void markAll();
    void clearAllMarks();
    void copyMarkedText();

private slots:
    void setEditor(ScintillaNext *edit);
    void adjustOpacity(int value);
    void transparencyToggled(bool on);
    void adjustOpacityWhenLosingFocus(bool checked);
    void adjustOpacityAlways(bool on);

    void changeTab(int index);

private:
    QString findString() const;
    void performFind(SearchDirection direction);
    void prepareToPerformSearch(bool isReplace = false);
    void loadSettings();
    void saveSettings() const;

    void savePosition();
    void restorePosition();

    FindOptions findOptions() const;

    enum class MessageType : std::uint8_t { Success, Error, Info };

    void showMessage(const QString &message, MessageType type = MessageType::Info);
    int ensureMarkIndicator();
    BookMarkDecorator *bookMarkDecorator() const;
    void clearAllBookmarks() const;

    void updateFindList(const QString &text);
    void updateReplaceList(const QString &text);

    bool isFirstTime = true;
    QPoint lastClosedPosition;
    std::unique_ptr<Ui::FindReplaceDialog> ui;

    ScintillaNext *editor = nullptr;
    QStatusBar *statusBar = nullptr;
    QTabBar *tabBar = nullptr;
    ISearchResultsHandler *searchResultsHandler = nullptr;
    std::unique_ptr<Finder> finder;
};

#endif  // FINDREPLACEDIALOG_H
