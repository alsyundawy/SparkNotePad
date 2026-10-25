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
 * Notepad Next is free software: you can redistribute it and/or modify
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QActionGroup>

#include "DockedEditor.h"

#include "MacroManager.h"
#include "ScintillaNext.h"
#include "NppImporter.h"
#include "SearchResultsCollector.h"

namespace Ui {
class MainWindow;
}

class NotepadNextApplication;
class Macro;
class Settings;
class QuickFindWidget;
class ZoomEventWatcher;
class ShiftMiddleClickBlocker;
class ShiftWheelToHorizontalScrollFilter;
class Converter;
class DefaultDirectoryManager;
class TabsQuickActionsBar;
class FocusedView;
class FocusedViewDialog;
class FunctionListDock;
class RunDialog;
class ManageToolsDialog;
class RegexHelperDialog;
class LanguageInspectorDock;
class TailOptionsDialog;
class PerformanceDiagnosticsDialog;
class MarkOccurrences;
class FileBrowserDock;

#include "ClipboardMonitor.h"
#include "TransparentWindowManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(NotepadNextApplication *app);
    ~MainWindow() override;

    bool isAnyUnsaved() const;

    void setupLanguageMenu();
    ScintillaNext *currentEditor() const;
    int editorCount() const;
    QVector<ScintillaNext *> editors() const;
    DockedEditor *getDockedEditor() const { return dockedEditor; }
    void applyStyleSheet();
    friend class ThemeManager;

public slots:
    void newFile();
    void newWindow();
    void exportPdf();

    void openFileDialog();
    void openFile(const QString &filePath);

    void openFolderAsWorkspaceDialog();
    void setFolderAsWorkspacePath(const QString &dir);

    void reloadFile();

    void closeCurrentFile();
    void closeFile(ScintillaNext *editor);
    void closeAllFiles();
    void closeAllExceptActive();
    void closeAllToLeft();
    void closeAllToRight();

    bool saveCurrentFile();
    bool saveFile(ScintillaNext *editor);
    bool saveCurrentFileWithOriginalTime();
    void updateCurrentFileTime();
    void togglePreserveFileTime(bool checked);

    void upperCaseSelection();
    void lowerCaseSelection();
    void invertCaseSelection();
    void titleCaseSelection();
    void sentenceCaseSelection();

    bool saveCurrentFileAsDialog();
    bool saveCurrentFileAs(const QString &fileName);
    bool saveFileAs(ScintillaNext *editor, const QString &fileName);

    bool saveCopyAsDialog();
    bool saveCopyAs(const QString &fileName);
    bool saveAll();
    bool saveAllEditors(const QVector<ScintillaNext *> &editorList);

    void exportAsFormat(Converter *converter, const QString &filter);
    void copyAsFormat(Converter *converter, const QString &mimeType);

    void renameFile();

    void moveCurrentFileToTrash();
    void moveFileToTrash(ScintillaNext *editor);

    void print();

    void convertEOLs(int eolMode);

    void showFindReplaceDialog(int index);

    void updateFileStatusBasedUi(ScintillaNext *editor);
    void updateEOLBasedUi(ScintillaNext *editor);
    void updateDocumentBasedUi(Scintilla::Update updated);
    void updateSelectionBasedUi(ScintillaNext *editor);
    void updateContentBasedUi(ScintillaNext *editor);
    void updateSaveStatusBasedUi(ScintillaNext *editor);
    void updateEditorPositionBasedUi();
    void updateLanguageBasedUi(ScintillaNext *editor);
    void updateGui(ScintillaNext *editor);

    void detectLanguage(ScintillaNext *editor);

    void setLanguage(ScintillaNext *editor, const QString &languageName);

    void bringWindowToForeground();
    void focusIn();

    void addEditor(ScintillaNext *editor);

    void checkForUpdates(bool silent = false);

    void restoreWindowState();

    void switchToEditor(const ScintillaNext *editor);

signals:
    void editorActivated(ScintillaNext *editor);
    void aboutToClose();
    void fileDialogAccepted(const QString &filePath);

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
    void tabBarRightClicked(ScintillaNext *editor);
    void languageMenuTriggered();
    void checkForUpdatesFinished(const QString &url);
    void activateEditor(ScintillaNext *editor);

private:
    Ui::MainWindow *ui = Q_NULLPTR;
    NotepadNextApplication *app = Q_NULLPTR;
    DockedEditor *dockedEditor = Q_NULLPTR;

    QScopedPointer<SearchResultsCollector> searchResults;

    template <typename Method>
    void connectEditorAction(QAction *action, Method method) {
        connect(action, &QAction::triggered, this, [this, method]() {
            if (auto *editor = currentEditor()) {
                (editor->*method)();
            }
        });
    }
    template <typename Method, typename Arg>
    void connectEditorAction(QAction *action, Method method, Arg value) {
        connect(action, &QAction::triggered, this, [this, method, value]() {
            if (auto *editor = currentEditor()) {
                (editor->*method)(value);
            }
        });
    }
    void applyCustomShortcuts();
    void initUpdateCheck();
    ScintillaNext *getInitialEditor();
    void openFileList(const QStringList &fileNames);
    bool checkEditorsBeforeClose(const QVector<ScintillaNext *> &editorList);
    bool checkFileForModification(ScintillaNext *editor);
    void showSaveErrorMessage(ScintillaNext *editor, QFileDevice::FileError error);
    void showEditorZoomLevelIndicator();

    enum class UserSaveAction { SaveAll, DiscardAll, Cancel };
    UserSaveAction promptForSave(const QVector<ScintillaNext *> &editorList);

    void saveSettings() const;
    void restoreSettings();

    ISearchResultsHandler *determineSearchResultsHandler();

    QActionGroup *languageActionGroup;

    TabsQuickActionsBar *tabsQuickActionsBar = Q_NULLPTR;
    MarkOccurrences *markOccurrences = nullptr;
    FileBrowserDock *fileBrowserDock = nullptr;
    FunctionListDock *functionListDock = nullptr;

    void setupRunMenu();
    void updateCustomToolsMenu();
    void onRunTriggered();
    void onRunCurrentFileTriggered();
    void onOpenTerminalHereTriggered();
    void onRevealInFileManagerTriggered();
    void onOpenInBrowserTriggered();
    void onSearchGoogleTriggered();
    void onSearchWikipediaTriggered();
    void onSearchGitHubTriggered();
    void onSearchStackOverflowTriggered();
    void onManageToolsTriggered();
    void onRegexHelperTriggered();
    void onEncryptDocumentTriggered();
    void onDecryptDocumentTriggered();
    void onToggleClipboardMonitoringTriggered();
    void onClipboardHistoryTriggered();
    void onClipboardMonitoringStateChanged(bool active);
    void onToggleTailMonitoringTriggered();
    void onTailMonitoringStateChanged(ScintillaNext *editor, bool isMonitoring);
    void updateTailMonitoringUi(ScintillaNext *editor);
    void onClipboardClipCaptured(const ClipboardMonitor::CapturedClip &clip, int totalCount);
    void onToggleTransparencyTriggered();
    void onIncreaseOpacityTriggered();
    void onDecreaseOpacityTriggered();
    void onTransparencyOptionsTriggered();
    void onTransparencyChanged(bool enabled, int opacity, TransparentWindowManager::TransparencyMode mode);
    void onNextDocumentTriggered();
    void onPreviousDocumentTriggered();
    void populateWindowMenuDocuments();
    void setupWindowMenu();
    void setupThemeMenu();
    void onMoveToNextScreenTriggered();
    void onMoveToPreviousScreenTriggered();
    void onCenterOnScreenTriggered();
    void onDisplayDiagnosticsTriggered();
    void onUserGuideTriggered();
    void onDeveloperGuideTriggered();
    void onGenerateCertTriggered();

    QMenu *menuRun = nullptr;
    QMenu *menuSavedTools = nullptr;
    QList<QAction *> customToolActions;
    QMenu *menuEncryption = nullptr;
    QAction *actionEncryptDoc = nullptr;
    QAction *actionDecryptDoc = nullptr;
    QMenu *menuClipboardMonitoring = nullptr;
    QAction *actionToggleClipboardMonitoring = nullptr;
    QAction *actionClipboardHistory = nullptr;
    QMenu *menuTailMonitoring = nullptr;
    QAction *actionToggleTailMonitoring = nullptr;
    QAction *actionTailAutoScroll = nullptr;
    QAction *actionTailReloadNow = nullptr;
    QAction *actionTailOptions = nullptr;
    TransparentWindowManager *transparentWindowManager = nullptr;
    QMenu *menuTransparency = nullptr;
    QAction *actionToggleTransparency = nullptr;
    QAction *actionIncreaseOpacity = nullptr;
    QAction *actionDecreaseOpacity = nullptr;
    QAction *actionTransparencyOptions = nullptr;
    FocusedView *focusedView = nullptr;
    QMenu *menuWindow = nullptr;
    QMenu *menuTheme = nullptr;
    QAction *actionThemeSwitcher = nullptr;
    QAction *actionThemeAuto = nullptr;
    QAction *actionThemeLight = nullptr;
    QAction *actionThemeDark = nullptr;
    QAction *actionToggleTheme = nullptr;
    QAction *actionAccessibilityHelp = nullptr;
    QAction *actionFocusEditor = nullptr;
    QList<QAction *> m_windowDocActions;
    FocusedViewDialog *focusedViewDialog = nullptr;
    LanguageInspectorDock *languageInspectorDock = nullptr;

    // NppImporter *npp;

    MacroManager macroManager;
    DefaultDirectoryManager *defaultDirectoryManager;

    ZoomEventWatcher *zoomEventWatcher;
    ShiftMiddleClickBlocker *shiftMiddleClickBlocker;
    ShiftWheelToHorizontalScrollFilter *shiftWheelToHorizontalScrollFilter;
    int zoomLevel = 0;
    int contextMenuPos = 0;
    QMenu *buildMenu(QStringList actionNames);
};

#endif  // MAINWINDOW_H
