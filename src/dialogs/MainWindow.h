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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#include <QMainWindow>
#include <QLabel>
#include <QActionGroup>
#include <cstdint>

#include "ClipboardMonitor.h"
#include "DockedEditor.h"
#include "MacroManager.h"
#include "ScintillaNext.h"
#include "SearchResultsCollector.h"
#include "TransparentWindowManager.h"

namespace Ui {
class MainWindow;
}

class SparkNotePadApplication;
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
class MarkdownSafePreviewDock;

class MainWindow : public QMainWindow {  // NOSONAR(cpp:S1448, cpp:S1820)
    Q_OBJECT

public:
    explicit MainWindow(SparkNotePadApplication *app);
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
    void newFile() const;
    void newWindow() const;
    void exportPdf();

    void openFileDialog();
    void openFile(const QString &filePath);
    void showCommandPalette(bool commandMode = false);
    void showDocumentStatistics();
    void decodeJwtToken();
    void showFileDiffDialog(const QString &filePathA = QString(), const QString &filePathB = QString());

    void openFolderAsWorkspaceDialog();
    void setFolderAsWorkspacePath(const QString &dir) const;

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
    void moveFileToTrash(const ScintillaNext *editor);

    void print();

    void convertEOLs(int eolMode);

    void showFindReplaceDialog(int index);

    void updateFileStatusBasedUi(const ScintillaNext *editor);
    void updateEOLBasedUi(const ScintillaNext *editor);
    void updateDocumentBasedUi(Scintilla::Update updated);
    void updateSelectionBasedUi(ScintillaNext *editor);
    void updateContentBasedUi(ScintillaNext *editor);
    void updateSaveStatusBasedUi(const ScintillaNext *editor);
    void updateEditorPositionBasedUi();
    void updateLanguageBasedUi(const ScintillaNext *editor) const;
    void updateGui(ScintillaNext *editor);

    void detectLanguage(ScintillaNext *editor) const;

    void setLanguage(ScintillaNext *editor, const QString &languageName) const;

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
    void showEvent(QShowEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void changeEvent(QEvent *event) override;

    void ensureMenuIconsVisible(QWidget *root = nullptr);

private slots:
    void tabBarRightClicked(const ScintillaNext *editor);
    void languageMenuTriggered() const;
    static void checkForUpdatesFinished(const QString &url);
    void activateEditor(ScintillaNext *editor);

    // Application lifecycle — Restart/Exit (v1.3.5)
    // Security: uses absolute paths to prevent CVE-2026-25926 untrusted search path.
    void restartApplication(bool safeMode = false);
    void exitApplication();

private:
    Ui::MainWindow *ui = Q_NULLPTR;
    SparkNotePadApplication *app = nullptr;
    DockedEditor *dockedEditor = Q_NULLPTR;

    QScopedPointer<SearchResultsCollector> searchResults;

    template <typename Method>
    void connectEditorAction(QAction *action, Method method) {
        connect(action, &QAction::triggered, this, [this, method]() {
            if (auto *editor = currentEditor()) {  // NOSONAR(cpp:S5417)
                (editor->*method)();
            }
        });
    }
    template <typename Method, typename Arg>
    void connectEditorAction(QAction *action, Method method, Arg value) {
        connect(action, &QAction::triggered, this, [this, method, value]() {
            if (auto *editor = currentEditor()) {  // NOSONAR(cpp:S5417)
                (editor->*method)(value);
            }
        });
    }
    void applyCustomShortcuts() const;
    void initUpdateCheck();
    ScintillaNext *getInitialEditor() const;
    void openFileList(const QStringList &fileNames);
    bool checkEditorsBeforeClose(const QVector<ScintillaNext *> &editorList);
    bool checkFileForModification(ScintillaNext *editor);
    void showSaveErrorMessage(const ScintillaNext *editor, QFileDevice::FileError error);
    void showEditorZoomLevelIndicator() const;
    void showEditorContextMenu(ScintillaNext *editor, const QPoint &pos);

    enum class UserSaveAction : std::uint8_t { SaveAll, DiscardAll, Cancel };
    UserSaveAction promptForSave(const QVector<ScintillaNext *> &editorList);

    void saveSettings() const;
    void restoreSettings();

    ISearchResultsHandler *determineSearchResultsHandler();

    QActionGroup *languageActionGroup;

    TabsQuickActionsBar *tabsQuickActionsBar = Q_NULLPTR;
    MarkOccurrences *markOccurrences = nullptr;
    FileBrowserDock *fileBrowserDock = nullptr;
    FunctionListDock *functionListDock = nullptr;
    MarkdownSafePreviewDock *markdownSafePreviewDock = nullptr;

    void setupRunMenu();
    void updateCustomToolsMenu();
    void onRunTriggered();
    void onRunCurrentFileTriggered();
    void onOpenTerminalHereTriggered() const;
    void onRevealInFileManagerTriggered() const;
    void onOpenInBrowserTriggered() const;
    void onSearchGoogleTriggered() const;
    void onSearchWikipediaTriggered() const;
    void onSearchGitHubTriggered() const;
    void onSearchStackOverflowTriggered() const;
    void onManageToolsTriggered();
    void onRegexHelperTriggered();
    void onEncryptDocumentTriggered();
    void onDecryptDocumentTriggered();
    void onHashValidatorTriggered();
    void onToggleClipboardMonitoringTriggered() const;
    void onClipboardHistoryTriggered();
    void onClipboardMonitoringStateChanged(bool active);
    void onToggleTailMonitoringTriggered();
    void onTailMonitoringStateChanged(const ScintillaNext *editor, bool isMonitoring);
    void updateTailMonitoringUi(const ScintillaNext *editor);
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

    MacroManager macroManager;
    DefaultDirectoryManager *defaultDirectoryManager;

    ZoomEventWatcher *zoomEventWatcher;
    ShiftMiddleClickBlocker *shiftMiddleClickBlocker;
    ShiftWheelToHorizontalScrollFilter *shiftWheelToHorizontalScrollFilter;
    int zoomLevel = 0;
    int contextMenuPos = 0;
    QMenu *buildMenu(const QStringList &actionNames);
};

#endif  // MAINWINDOW_H
