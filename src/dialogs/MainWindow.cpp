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

#include <QtGlobal>
#include "MainWindow.h"
#include "SparkNotePadApplication.h"
#include "ThemeSwitcherDialog.h"
#include "features/ThemePaletteData.h"
#include "AboutDialog.h"
#include "features/ReleaseManager.h"
#include "features/DocumentationManager.h"
#include "features/ScreenDisplayManager.h"
#include "ThemeManager.h"
#include "AccessibilityManager.h"
#include "MacServicesHandler.h"
#include "MarkOccurrences.h"
#include "BookMarkDecorator.h"
#include "DefaultDirectoryManager.h"
#include "MarkerAppDecorator.h"
#include "ScintillaSorter.h"
#include "URLFinder.h"
#include "UrlUtils.h"
#include "EncodingSelectionDialog.h"
#include "CharacterInfo.h"
#include "CharacterInfoDialog.h"
#include "FileTimeUtils.h"
#include "CaseConverter.h"
#include "SessionManager.h"
#include "UndoAction.h"
#include "ModernMessageBox.h"
#include "docks/MarkdownSafePreviewDock.h"
#include "dialogs/HashIntegrityDialog.h"
#include "ui_MainWindow.h"

#ifndef APP_VERSION
#define APP_VERSION "1.3.6"
#endif

#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QClipboard>
#include <QStandardPaths>
#include <QWindow>
#include <QPushButton>
#include <QAbstractButton>
#include <QTimer>
#include <QInputDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPdfWriter>
#include <QProcess>  // Required for restartApplication() (v1.3.5)
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QDirIterator>
#include <QProcess>
#include <QScreen>
#include <QFontDatabase>

#ifdef Q_OS_WIN
#include <QSimpleUpdater.h>
#include <Windows.h>
#endif

#include "DockAreaWidget.h"

#include "SparkNotePadApplication.h"
#include "ApplicationSettings.h"

#include "ScintillaNext.h"

#include "RecentFilesListManager.h"
#include "RecentFilesListMenuBuilder.h"
#include "EditorManager.h"

#include "LuaConsoleDock.h"
#include "LanguageInspectorDock.h"
#include "EditorInspectorDock.h"
#include "FolderAsWorkspaceDock.h"
#include "FileBrowserDock.h"
#include "Base64Utils.h"
#include "NumberConverter.h"
#include "GuidTimestampUtils.h"
#include "ColorPreviewCallTip.h"
#include "CodeCompressPretty.h"
#include "MathEvaluator.h"
#include "JsEvaluator.h"
#include "JsEvaluationDialog.h"
#include "FocusedView.h"
#include "FocusedViewDialog.h"
#include "LogAutoTimestamp.h"
#include "SearchResultsDock.h"
#include "DebugLogDock.h"
#include "FileListDock.h"
#include "FunctionListDock.h"
#include "ChangeHistoryDecorator.h"
#include "ExternalToolManager.h"
#include "RunDialog.h"
#include "ManageToolsDialog.h"
#include "RegexHelperDialog.h"
#include "EncryptDialog.h"
#include "DecryptDialog.h"
#include "ClipboardHistoryDialog.h"
#include "TransparencyDialog.h"
#include "MkcertDialog.h"
#include "../features/TransparentWindowManager.h"
#include "../features/ClipboardMonitor.h"
#include "../features/PythonScriptEngine.h"
#include "../utils/CryptoManager.h"
#include "../features/TailMonitoringManager.h"
#include "TailOptionsDialog.h"
#include "../features/PerformanceProfiler.h"
#include "PerformanceDiagnosticsDialog.h"

#include "FindReplaceDialog.h"
#include "MacroRunDialog.h"
#include "MacroSaveDialog.h"
#include "PreferencesDialog.h"
#include "ColumnEditorDialog.h"

#include "TabsQuickActionsBar.h"

#include "QuickFindWidget.h"

#include "EditorPrintPreviewRenderer.h"
#include "MacroEditorDialog.h"

#include "ZoomEventWatcher.h"
#include "ShiftMiddleClickBlocker.h"
#include "ShiftWheelToHorizontalScrollFilter.h"

#include "FileDialogHelpers.h"

#include "HtmlConverter.h"
#include "RtfConverter.h"

#include "FadingIndicator.h"

#include "ActionUtils.h"

MainWindow::MainWindow(SparkNotePadApplication *app)  // NOSONAR(cpp:S3776)
    : ui(new Ui::MainWindow),                         // NOSONAR(cpp:S5025)
      app(app),
      zoomEventWatcher(new ZoomEventWatcher(this)),
      shiftMiddleClickBlocker(new ShiftMiddleClickBlocker(this)),
      shiftWheelToHorizontalScrollFilter(new ShiftWheelToHorizontalScrollFilter(this)) {
    qInfo(Q_FUNC_INFO);

    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
    ui->mainToolBar->setIconSize(QSize(20, 20));
    ui->mainToolBar->setAttribute(Qt::WA_AlwaysShowToolTips, true);
    for (QAction *act : ui->mainToolBar->actions()) {
        if (!act->isSeparator()) {
            if (QWidget *w = ui->mainToolBar->widgetForAction(act)) {
                w->setAttribute(Qt::WA_AlwaysShowToolTips, true);
                w->setToolTip(act->toolTip());
            }
        }
    }

    applyCustomShortcuts();

    qInfo("setupUi Completed");

    defaultDirectoryManager = new DefaultDirectoryManager(this, app->getSettings());

    connect(this, &MainWindow::aboutToClose, this, &MainWindow::saveSettings);

    // Create and set up the connections to the docked editor
    dockedEditor = new DockedEditor(this);
    connect(dockedEditor, &DockedEditor::editorCloseRequested, this, &MainWindow::closeFile);
    connect(dockedEditor, &DockedEditor::editorActivated, this, &MainWindow::activateEditor);
    connect(dockedEditor, &DockedEditor::contextMenuRequestedForEditor, this, &MainWindow::tabBarRightClicked);
    connect(dockedEditor, &DockedEditor::titleBarDoubleClicked, this, &MainWindow::newFile);

    // Set up the menus
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->actionNewWindow, &QAction::triggered, this, &MainWindow::newWindow);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFileDialog);
    connect(ui->actionReload, &QAction::triggered, this, &MainWindow::reloadFile);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::closeCurrentFile);
    connect(ui->actionCloseAll, &QAction::triggered, this, &MainWindow::closeAllFiles);
    connect(ui->actionRestart, &QAction::triggered, this, [this]() { restartApplication(false); });
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitApplication);

    // Split editor actions
    connect(ui->actionSplitHorizontal, &QAction::triggered, this, [this]() {
        newFile();
        ScintillaNext *newEditor = currentEditor();
        if (newEditor) {
            dockedEditor->splitToRight(newEditor);
        }
    });
    connect(ui->actionSplitVertical, &QAction::triggered, this, [this]() {
        newFile();
        ScintillaNext *newEditor = currentEditor();
        if (newEditor) {
            dockedEditor->splitToBottom(newEditor);
        }
    });

    ui->actionRestart->setStatusTip(tr("Gracefully saves settings and restarts SparkNotePad"));
    ui->actionExit->setStatusTip(tr("Prompts to save open documents and quits SparkNotePad"));

#ifdef Q_OS_WIN
    ui->actionExit->setShortcut(QKeySequence("Alt+F4"));
#else
    ui->actionExit->setShortcut(QKeySequence::Quit);
#endif

    connect(ui->actionOpenFolderasWorkspace, &QAction::triggered, this, &MainWindow::openFolderAsWorkspaceDialog);

    connect(ui->actionCloseAllExceptActive, &QAction::triggered, this, &MainWindow::closeAllExceptActive);
    connect(ui->actionCloseAllToLeft, &QAction::triggered, this, &MainWindow::closeAllToLeft);
    connect(ui->actionCloseAllToRight, &QAction::triggered, this, &MainWindow::closeAllToRight);

    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveCurrentFile);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveCurrentFileAsDialog);
    connect(ui->actionSaveCopyAs, &QAction::triggered, this, &MainWindow::saveCopyAsDialog);
    connect(ui->actionSaveWithOriginalTime, &QAction::triggered, this, &MainWindow::saveCurrentFileWithOriginalTime);
    connect(ui->actionSaveAll, &QAction::triggered, this, &MainWindow::saveAll);
    connect(ui->actionUpdateFileTime, &QAction::triggered, this, &MainWindow::updateCurrentFileTime);
    connect(ui->actionPreserveFileTime, &QAction::toggled, this, &MainWindow::togglePreserveFileTime);

    ui->actionPreserveFileTime->setChecked(app->getSettings()->preserveFileTime());
    connect(app->getSettings(), &ApplicationSettings::preserveFileTimeChanged, ui->actionPreserveFileTime,
            &QAction::setChecked);
    connect(ui->actionRename, &QAction::triggered, this, &MainWindow::renameFile);

    connect(ui->actionExportHtml, &QAction::triggered, this, [this]() {
        HtmlConverter html(currentEditor());
        exportAsFormat(&html, QStringLiteral("HTML files (*.html)"));
    });

    connect(ui->actionExportRtf, &QAction::triggered, this, [this]() {
        RtfConverter rtf(currentEditor());
        exportAsFormat(&rtf, QStringLiteral("RTF Files (*.rtf)"));
    });

    connect(ui->actionExportPdf, &QAction::triggered, this, &MainWindow::exportPdf);

    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::print);

    connectEditorAction(ui->actionToggleSingleLineComment, &ScintillaNext::toggleCommentSelection);
    connectEditorAction(ui->actionSingleLineComment, &ScintillaNext::commentLineSelection);
    connectEditorAction(ui->actionSingleLineUncomment, &ScintillaNext::uncommentLineSelection);

    connect(ui->actionConvertToHex, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = NumberConverter::convert(editor, NumberConverter::ToHex);
        if (result.success) {
            ui->statusBar->showMessage(tr("Converted '%1' → %2").arg(result.original, result.converted), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });

    connect(ui->actionInsertDateTimeShort, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = GuidTimestampUtils::insertTimestamp(editor, GuidTimestampUtils::DateTimeShort);
        if (result.success) {
            ui->statusBar->showMessage(tr("Inserted timestamp: %1").arg(result.insertedText), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionInsertDateTimeLong, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = GuidTimestampUtils::insertTimestamp(editor, GuidTimestampUtils::DateTimeLong);
        if (result.success) {
            ui->statusBar->showMessage(tr("Inserted timestamp: %1").arg(result.insertedText), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionInsertTimestampIso, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = GuidTimestampUtils::insertTimestamp(editor, GuidTimestampUtils::Iso8601Local);
        if (result.success) {
            ui->statusBar->showMessage(tr("Inserted ISO timestamp: %1").arg(result.insertedText), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionInsertTimestampUnix, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = GuidTimestampUtils::insertTimestamp(editor, GuidTimestampUtils::UnixTimestampSecs);
        if (result.success) {
            ui->statusBar->showMessage(tr("Inserted Unix timestamp: %1").arg(result.insertedText), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionInsertDateOnly, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = GuidTimestampUtils::insertTimestamp(editor, GuidTimestampUtils::DateOnly);
        if (result.success) {
            ui->statusBar->showMessage(tr("Inserted date: %1").arg(result.insertedText), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionInsertTimeOnly, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = GuidTimestampUtils::insertTimestamp(editor, GuidTimestampUtils::TimeOnly);
        if (result.success) {
            ui->statusBar->showMessage(tr("Inserted time: %1").arg(result.insertedText), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionInsertGuidHyphens, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = GuidTimestampUtils::insertGuid(editor, GuidTimestampUtils::GuidHyphenatedLower);
        if (result.success) {
            ui->statusBar->showMessage(tr("Inserted GUID: %1").arg(result.insertedText), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionInsertGuidBraces, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = GuidTimestampUtils::insertGuid(editor, GuidTimestampUtils::GuidWithBracesUpper);
        if (result.success) {
            ui->statusBar->showMessage(tr("Inserted GUID: %1").arg(result.insertedText), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionInsertGuidCompact, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = GuidTimestampUtils::insertGuid(editor, GuidTimestampUtils::GuidCompactUpper);
        if (result.success) {
            ui->statusBar->showMessage(tr("Inserted GUID: %1").arg(result.insertedText), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionConvertToDec, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = NumberConverter::convert(editor, NumberConverter::ToDec);
        if (result.success) {
            ui->statusBar->showMessage(tr("Converted '%1' → %2").arg(result.original, result.converted), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionConvertToBin, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = NumberConverter::convert(editor, NumberConverter::ToBin);
        if (result.success) {
            ui->statusBar->showMessage(tr("Converted '%1' → %2").arg(result.original, result.converted), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionConvertToOct, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = NumberConverter::convert(editor, NumberConverter::ToOct);
        if (result.success) {
            ui->statusBar->showMessage(tr("Converted '%1' → %2").arg(result.original, result.converted), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });

    auto handleFormat = [this](CodeCompressPretty::Language lang, bool minify) {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        auto result = minify ? CodeCompressPretty::compress(editor, lang) : CodeCompressPretty::prettify(editor, lang);
        if (result.success) {
            const QString actionName = minify ? tr("Minified") : tr("Prettified");
            const QString target = result.wholeDocument ? tr("entire document") : tr("selection");
            const QString langStr = CodeCompressPretty::languageName(result.detectedLanguage);
            ui->statusBar->showMessage(tr("%1 %2 as %3").arg(actionName, target, langStr), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 5000);
        }
    };

    connect(ui->actionFormatAuto, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::Auto, false); });
    connect(ui->actionMinifyAuto, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::Auto, true); });
    connect(ui->actionFormatJson, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::Json, false); });
    connect(ui->actionMinifyJson, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::Json, true); });
    connect(ui->actionFormatJson5, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::Json5, false); });
    connect(ui->actionMinifyJson5, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::Json5, true); });
    connect(ui->actionFormatCss, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::Css, false); });
    connect(ui->actionMinifyCss, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::Css, true); });
    connect(ui->actionFormatJs, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::JavaScript, false); });
    connect(ui->actionMinifyJs, &QAction::triggered, this,
            [handleFormat]() { handleFormat(CodeCompressPretty::JavaScript, true); });

    ui->actionEvaluateMathAppend->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_E));
    ui->actionEvaluateMathReplace->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_E));

    connect(ui->actionEvaluateMathAppend, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        auto result = MathEvaluator::evaluateInEditor(editor, MathEvaluator::AppendResult);
        if (result.success) {
            ui->statusBar->showMessage(tr("Calculated: %1 = %2").arg(result.expression, result.formattedResult), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });

    connect(ui->actionEvaluateMathReplace, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        auto result = MathEvaluator::evaluateInEditor(editor, MathEvaluator::ReplaceExpression);
        if (result.success) {
            ui->statusBar->showMessage(tr("Calculated: %1 → %2").arg(result.expression, result.formattedResult), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });

    connect(ui->actionEvaluateJsReplace, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        auto result = JsEvaluator::evaluateInEditor(editor, JsEvaluator::ReplaceSelection);
        if (result.success) {
            ui->statusBar->showMessage(tr("JS Evaluated in %1 ms: %2")
                                           .arg(QString::number(result.executionTimeMs, 'f', 2), result.resultString),
                                       3000);
        } else {
            ui->statusBar->showMessage(tr("JS Error: %1").arg(result.errorMessage), 4000);
        }
    });

    connect(ui->actionEvaluateJsAppend, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        auto result = JsEvaluator::evaluateInEditor(editor, JsEvaluator::AppendComment);
        if (result.success) {
            ui->statusBar->showMessage(tr("JS Evaluated in %1 ms").arg(QString::number(result.executionTimeMs, 'f', 2)),
                                       3000);
        } else {
            ui->statusBar->showMessage(tr("JS Error: %1").arg(result.errorMessage), 4000);
        }
    });

    connect(ui->actionEvaluateJsDialog, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        QString initialCode;
        if (editor && editor->selectionStart() != editor->selectionEnd()) {
            initialCode = QString::fromUtf8(editor->get_text_range(static_cast<int>(editor->selectionStart()),
                                                                   static_cast<int>(editor->selectionEnd())));
        }
        JsEvaluationDialog dlg(this, editor, initialCode);
        dlg.exec();
    });

    connect(ui->actionColorPreview, &QAction::triggered, this, [this]() {
        const ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }

        auto *cp = editor->findChild<ColorPreviewCallTip *>(QString(), Qt::FindDirectChildrenOnly);
        if (cp) {
            const auto pos = static_cast<int>(
                (editor->selectionStart() != editor->selectionEnd()) ? editor->selectionStart() : editor->currentPos());
            if (cp->showPreviewAt(pos)) {
                cp->openColorDialogAndReplace();
            } else {
                ui->statusBar->showMessage(tr("No color code found under cursor (supports #HEX, rgb(), rgba(), hsl())"),
                                           4000);
            }
        }
    });

    connect(ui->actionBase64Encode, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = Base64Utils::encode(editor);
        if (result.success) {
            ui->statusBar->showMessage(
                result.wholeDocument ? tr("Base64 encoded entire document") : tr("Base64 encoded selection"), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionURLEncode, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        const QByteArray selection = editor->getSelText();
        editor->replaceSel(selection.toPercentEncoding().constData());
    });
    connect(ui->actionBase64Decode, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        auto result = Base64Utils::decode(editor);
        if (result.success) {
            ui->statusBar->showMessage(
                result.wholeDocument ? tr("Base64 decoded entire document") : tr("Base64 decoded selection"), 3000);
        } else {
            ui->statusBar->showMessage(result.errorMessage, 4000);
        }
    });
    connect(ui->actionURLDecode, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        const QByteArray selection = editor->getSelText();
        editor->replaceSel(QByteArray::fromPercentEncoding(selection).constData());
    });
    connect(ui->actionCopyURL, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto pos = static_cast<int>((contextMenuPos != -1) ? contextMenuPos : editor->currentPos());
        contextMenuPos = -1;
        UrlUtils::copyUrlAtPosition(editor, pos);
    });

    connect(ui->actionOpenURL, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto pos = static_cast<int>((contextMenuPos != -1) ? contextMenuPos : editor->currentPos());
        contextMenuPos = -1;
        UrlUtils::openUrlAtPosition(editor, pos);
    });

    // Encoding actions
    connect(ui->actionShowEncodingInfo, &QAction::triggered, this, [this]() {
        if (auto *ed = currentEditor()) {
            EncodingSelectionDialog dlg(ed, this);
            if (dlg.exec() == QDialog::Accepted) {
                ui->statusBar->refresh(ed);
            }
        }
    });

    connect(ui->actionConvertToUtf8, &QAction::triggered, this, [this]() {
        if (auto *ed = currentEditor()) {
            ed->convertToEncoding(QStringLiteral("UTF-8"), ScintillaNext::BomType::None);
            ui->statusBar->refresh(ed);
        }
    });
    connect(ui->actionConvertToUtf8Bom, &QAction::triggered, this, [this]() {
        if (auto *ed = currentEditor()) {
            ed->convertToEncoding(QStringLiteral("UTF-8"), ScintillaNext::BomType::Utf8);
            ui->statusBar->refresh(ed);
        }
    });
    connect(ui->actionConvertToAnsi, &QAction::triggered, this, [this]() {
        if (auto *ed = currentEditor()) {
            ed->convertToEncoding(QStringLiteral("windows-1252"), ScintillaNext::BomType::None);
            ui->statusBar->refresh(ed);
        }
    });
    connect(ui->actionConvertToUtf16Le, &QAction::triggered, this, [this]() {
        if (auto *ed = currentEditor()) {
            ed->convertToEncoding(QStringLiteral("UTF-16LE"), ScintillaNext::BomType::Utf16LE);
            ui->statusBar->refresh(ed);
        }
    });
    connect(ui->actionConvertToUtf16Be, &QAction::triggered, this, [this]() {
        if (auto *ed = currentEditor()) {
            ed->convertToEncoding(QStringLiteral("UTF-16BE"), ScintillaNext::BomType::Utf16BE);
            ui->statusBar->refresh(ed);
        }
    });

    auto reopenWith = [this](const QString &enc) {
        if (auto *ed = currentEditor()) {
            if (!ed->isFile()) {
                ModernMessageBox::information(this, tr("SparkNotePad"),
                                              tr("Cannot reopen unsaved new buffer from disk."));
                return;
            }
            ed->reloadWithEncoding(enc);
            ui->statusBar->refresh(ed);
        }
    };

    connect(ui->actionReopenUtf8, &QAction::triggered, this, [reopenWith]() { reopenWith(QStringLiteral("UTF-8")); });
    connect(ui->actionReopenAnsi, &QAction::triggered, this,
            [reopenWith]() { reopenWith(QStringLiteral("windows-1252")); });
    connect(ui->actionReopenUtf16Le, &QAction::triggered, this,
            [reopenWith]() { reopenWith(QStringLiteral("UTF-16LE")); });
    connect(ui->actionReopenUtf16Be, &QAction::triggered, this,
            [reopenWith]() { reopenWith(QStringLiteral("UTF-16BE")); });
    connect(ui->actionReopenShiftJis, &QAction::triggered, this,
            [reopenWith]() { reopenWith(QStringLiteral("Shift-JIS")); });
    connect(ui->actionReopenGbk, &QAction::triggered, this, [reopenWith]() { reopenWith(QStringLiteral("GB18030")); });
    connect(ui->actionReopenBig5, &QAction::triggered, this, [reopenWith]() { reopenWith(QStringLiteral("Big5")); });
    connect(ui->actionReopenEucKr, &QAction::triggered, this, [reopenWith]() { reopenWith(QStringLiteral("EUC-KR")); });
    connect(ui->actionReopenWin1251, &QAction::triggered, this,
            [reopenWith]() { reopenWith(QStringLiteral("windows-1251")); });
    connect(ui->actionReopenWin1250, &QAction::triggered, this,
            [reopenWith]() { reopenWith(QStringLiteral("windows-1250")); });
    connect(ui->actionReopenCp437, &QAction::triggered, this, [reopenWith]() { reopenWith(QStringLiteral("IBM850")); });
    connect(ui->actionReopenMore, &QAction::triggered, this, [this]() {
        if (auto *ed = currentEditor()) {
            EncodingSelectionDialog dlg(ed, this);
            if (dlg.exec() == QDialog::Accepted) {
                ui->statusBar->refresh(ed);
            }
        }
    });

    connect(ui->actionShowCharacterInfo, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        CharacterInfo::Details details = CharacterInfo::getDetailsAt(editor);
        if (!details.isValid) {
            ModernMessageBox::information(this, tr("SparkNotePad"), tr("No character found at the current position."));
            return;
        }
        CharacterInfoDialog dlg(details, this);
        dlg.exec();
    });

    connect(ui->actionClearRecentFilesList, &QAction::triggered, app->getRecentFilesListManager(),
            &RecentFilesListManager::clear);

    connect(ui->actionMoveToTrash, &QAction::triggered, this, &MainWindow::moveCurrentFileToTrash);

    auto *recentFileListMenuBuilder = new RecentFilesListMenuBuilder(app->getRecentFilesListManager());
    connect(ui->menuRecentFiles, &QMenu::aboutToShow, this, [this, recentFileListMenuBuilder]() {
        // NOTE: its unfortunate that this has to be hard coded, but there's no way
        // to easily determine what should or shouldn't be there
        while (ui->menuRecentFiles->actions().size() > 4) {
            delete ui->menuRecentFiles->actions().takeLast();  // NOSONAR(cpp:S5025)
        }

        recentFileListMenuBuilder->populateMenu(ui->menuRecentFiles);
    });

    connect(ui->actionRestoreRecentlyClosedFile, &QAction::triggered, this, [this, app]() {
        if (app->getRecentFilesListManager()->count() > 0) {
            openFileList(QStringList() << app->getRecentFilesListManager()->mostRecentFile());
        }
    });

    connect(ui->actionOpenAllRecentFiles, &QAction::triggered, this,
            [this, app]() { openFileList(app->getRecentFilesListManager()->fileList()); });

    connect(recentFileListMenuBuilder, &RecentFilesListMenuBuilder::fileOpenRequest, this, &MainWindow::openFile);

    auto *eolActionGroup = new QActionGroup(this);
    eolActionGroup->addAction(ui->actionWindows);
    eolActionGroup->addAction(ui->actionUnix);
    eolActionGroup->addAction(ui->actionMacintosh);

    ui->actionWindows->setData(SC_EOL_CRLF);
    ui->actionUnix->setData(SC_EOL_LF);
    ui->actionMacintosh->setData(SC_EOL_CR);

    auto handleEolTrigger = [this]() {
        // qobject_cast lets us look at which specific action was clicked
        if (const auto *action = qobject_cast<QAction *>(sender())) {
            int eolMode = action->data().toInt();
            convertEOLs(eolMode);
        }
    };

    // Connect all three to the same handler
    connect(ui->actionWindows, &QAction::triggered, this, handleEolTrigger);
    connect(ui->actionUnix, &QAction::triggered, this, handleEolTrigger);
    connect(ui->actionMacintosh, &QAction::triggered, this, handleEolTrigger);

    connect(ui->actionUpperCase, &QAction::triggered, this, &MainWindow::upperCaseSelection);
    connect(ui->actionLowerCase, &QAction::triggered, this, &MainWindow::lowerCaseSelection);
    connect(ui->actionInvertCase, &QAction::triggered, this, &MainWindow::invertCaseSelection);
    connect(ui->actionTitleCase, &QAction::triggered, this, &MainWindow::titleCaseSelection);
    connect(ui->actionSentenceCase, &QAction::triggered, this, &MainWindow::sentenceCaseSelection);

    connectEditorAction(ui->actionDuplicateCurrentLine, &ScintillaNext::lineDuplicate);
    connectEditorAction(ui->actionMoveSelectedLinesUp, &ScintillaNext::moveSelectedLinesUp);
    connectEditorAction(ui->actionMoveSelectedLinesDown, &ScintillaNext::moveSelectedLinesDown);

    connect(ui->actionSplitLines, &QAction::triggered, this, [this]() {
        currentEditor()->targetFromSelection();
        currentEditor()->linesSplit(0);
    });

    connect(ui->actionJoinLines, &QAction::triggered, this, [this]() {
        currentEditor()->targetFromSelection();
        currentEditor()->linesJoin();
    });

    connect(ui->actionRemoveEmptyLines, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        Finder f(editor);
        const UndoAction ua(editor);

        f.options().text = QStringLiteral("\\R\\R+");
        f.options().flags = Scintilla::FindOption::RegExp;
        f.replaceAll(editor->eolString());

        // The regex will not entirely remove a blank first line
        editor->deleteLeadingEmptyLines();

        // Regex will also not delete the final blank line
        editor->deleteTrailingEmptyLines();
    });

    connectEditorAction(ui->actionTrimTrailingSpace, &ScintillaNext::trimTrailingWhitespace);
    connectEditorAction(ui->actionRemoveDuplicateLines, &ScintillaNext::removeDuplicateLines);
    connectEditorAction(ui->actionRemoveConsecutiveDuplicateLines, &ScintillaNext::removeConsecutiveDuplicateLines);

    connect(ui->actionReverseLineOrder, &QAction::triggered, this, [this]() {
        ScintillaSorter scintillaSorter(currentEditor());
        scintillaSorter.sort(ReverseSorter(Sorter::Direction::Ascending));
    });
    connect(ui->actionSortLinesAsc, &QAction::triggered, this, [this]() {
        ScintillaSorter scintillaSorter(currentEditor());
        scintillaSorter.sort(CaseSensitiveSorter(Sorter::Direction::Ascending));
    });
    connect(ui->actionSortLinesAscCaseInsensitive, &QAction::triggered, this, [this]() {
        ScintillaSorter scintillaSorter(currentEditor());
        scintillaSorter.sort(CaseInsensitiveSorter(Sorter::Direction::Ascending));
    });
    connect(ui->actionSortLinesbyLengthAsc, &QAction::triggered, this, [this]() {
        ScintillaSorter scintillaSorter(currentEditor());
        scintillaSorter.sort(LineLengthSorter(Sorter::Direction::Ascending));
    });
    connect(ui->actionSortLinesDesc, &QAction::triggered, this, [this]() {
        ScintillaSorter scintillaSorter(currentEditor());
        scintillaSorter.sort(CaseSensitiveSorter(Sorter::Direction::Descending));
    });
    connect(ui->actionSortLinesDescCaseInsensitive, &QAction::triggered, this, [this]() {
        ScintillaSorter scintillaSorter(currentEditor());
        scintillaSorter.sort(CaseInsensitiveSorter(Sorter::Direction::Descending));
    });
    connect(ui->actionSortLinesbyLengthDesc, &QAction::triggered, this, [this]() {
        ScintillaSorter scintillaSorter(currentEditor());
        scintillaSorter.sort(LineLengthSorter(Sorter::Direction::Descending));
    });

    connect(ui->actionColumnMode, &QAction::triggered, this, [this]() {
        ColumnEditorDialog *columnEditor = findChild<ColumnEditorDialog *>(QString(), Qt::FindDirectChildrenOnly);

        if (columnEditor == Q_NULLPTR) {
            columnEditor = new ColumnEditorDialog(this);
        }

        columnEditor->show();
        columnEditor->raise();
        columnEditor->activateWindow();
    });

    connectEditorAction(ui->actionUndo, &ScintillaNext::undo);
    connectEditorAction(ui->actionRedo, &ScintillaNext::redo);
    connectEditorAction(ui->actionCut, &ScintillaNext::cutAllowLine);
    connectEditorAction(ui->actionCopy, &ScintillaNext::copyAllowLine);
    connectEditorAction(ui->actionDelete, &ScintillaNext::clear);
    connectEditorAction(ui->actionPaste, &ScintillaNext::paste);
    connectEditorAction(ui->actionSelectAll, &ScintillaNext::selectAll);
    connect(ui->actionSelectNext, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();

        editor->setSearchFlags(SCFIND_NONE);
        editor->targetWholeDocument();
        editor->multipleSelectAddNext();
    });
    connect(ui->actionCopyFullPath, &QAction::triggered, this, [this]() {
        auto editor = currentEditor();
        if (editor->isFile()) {
            QApplication::clipboard()->setText(editor->getFilePath());
        }
    });
    connect(ui->actionCopyFileName, &QAction::triggered, this,
            [this]() { QApplication::clipboard()->setText(currentEditor()->getName()); });
    connect(ui->actionCopyFileDirectory, &QAction::triggered, this, [this]() {
        auto editor = currentEditor();
        if (editor->isFile()) {
            QApplication::clipboard()->setText(editor->getPath());
        }
    });

    connect(ui->actionCopyAsHtml, &QAction::triggered, this, [this]() {
        HtmlConverter html(currentEditor());
        copyAsFormat(&html, "text/html");
    });

    connect(ui->actionCopyAsRtf, &QAction::triggered, this, [this]() {
        RtfConverter rtf(currentEditor());
        copyAsFormat(&rtf, "Rich Text Format");
    });

    connectEditorAction(ui->actionIncreaseIndent, &ScintillaNext::tab);
    connectEditorAction(ui->actionDecreaseIndent, &ScintillaNext::backTab);

    addAction(ui->actionToggleOverType);
    connect(ui->actionToggleOverType, &QAction::triggered, this, [this]() {
        currentEditor()->editToggleOvertype();
        ui->statusBar->refresh(currentEditor());
    });

    auto *srDock = new SearchResultsDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, srDock);
    srDock->toggleViewAction()->setShortcut(Qt::Key_F7);
    ui->menuView->addAction(srDock->toggleViewAction());

    // NOLINTBEGIN(bugprone-easily-swappable-parameters)
    connect(srDock, &SearchResultsDock::searchResultActivated, this,
            [this](ScintillaNext *editor, int lineNumber, int startPositionFromBeginning,
                   int endPositionFromBeginning)  // NOSONAR(cpp:S5817)
            {
                dockedEditor->switchToEditor(editor);

                auto linePos = static_cast<int>(editor->positionFromLine(lineNumber));
                editor->goToRange({linePos + startPositionFromBeginning, linePos + endPositionFromBeginning});
                editor->verticalCentreCaret();

                editor->grabFocus();
            });
    // NOLINTEND(bugprone-easily-swappable-parameters)

    auto *actionRegexHelper = new QAction(tr("Regular Expression &Tester && Reference..."), this);
    actionRegexHelper->setIcon(QIcon(QStringLiteral(":/icons/find.svg")));
    actionRegexHelper->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::SHIFT | Qt::Key_R));
    connect(actionRegexHelper, &QAction::triggered, this, &MainWindow::onRegexHelperTriggered);
    ui->menuSearch->addAction(actionRegexHelper);

    menuEncryption = new QMenu(tr("&Encryption (AES-256)"), this);
    menuEncryption->setIcon(QIcon(QStringLiteral(":/icons/readonly.svg")));
    actionEncryptDoc = new QAction(tr("&Encrypt Current Document..."), this);
    actionEncryptDoc->setIcon(QIcon(QStringLiteral(":/icons/readonly.svg")));
    actionEncryptDoc->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_E));
    connect(actionEncryptDoc, &QAction::triggered, this, &MainWindow::onEncryptDocumentTriggered);
    menuEncryption->addAction(actionEncryptDoc);

    actionDecryptDoc = new QAction(tr("&Remove Encryption / Decrypt..."), this);
    actionDecryptDoc->setIcon(QIcon(QStringLiteral(":/icons/readonly.svg")));
    actionDecryptDoc->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_D));
    connect(actionDecryptDoc, &QAction::triggered, this, &MainWindow::onDecryptDocumentTriggered);
    menuEncryption->addAction(actionDecryptDoc);
    ui->menuFile->addMenu(menuEncryption);

    auto *actionHashValidator = new QAction(tr("Cryptographic &Hash & Authenticity Validator..."), this);
    actionHashValidator->setIcon(QIcon(QStringLiteral(":/icons/saved.svg")));
    connect(actionHashValidator, &QAction::triggered, this, &MainWindow::onHashValidatorTriggered);
    ui->menuFile->addAction(actionHashValidator);

    menuClipboardMonitoring = new QMenu(tr("Clipboard &Monitoring"), this);
    actionToggleClipboardMonitoring = new QAction(tr("&Monitor Clipboard (Append to Document)"), this);
    actionToggleClipboardMonitoring->setIcon(QIcon(QStringLiteral(":/icons/copy.svg")));
    actionToggleClipboardMonitoring->setCheckable(true);
    actionToggleClipboardMonitoring->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_B));
    connect(actionToggleClipboardMonitoring, &QAction::triggered, this,
            &MainWindow::onToggleClipboardMonitoringTriggered);
    menuClipboardMonitoring->addAction(actionToggleClipboardMonitoring);

    actionClipboardHistory = new QAction(tr("View Clipboard &History..."), this);
    actionClipboardHistory->setIcon(QIcon(QStringLiteral(":/icons/list_with_icons.svg")));
    connect(actionClipboardHistory, &QAction::triggered, this, &MainWindow::onClipboardHistoryTriggered);
    menuClipboardMonitoring->addAction(actionClipboardHistory);

    ui->menuEdit->addSeparator();
    ui->menuEdit->addMenu(menuClipboardMonitoring);

    // Setup Python Scripting (3.13+) Menu
    auto *menuPython = new QMenu(tr("&Python Scripting (3.13+)"), this);
    auto *actionRunPython = new QAction(tr("&Run Script / Selection"), this);
    actionRunPython->setIcon(QIcon(QStringLiteral(":/icons/playRecord.svg")));
    actionRunPython->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_P));
    connect(actionRunPython, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        QString msg;
        bool ok = PythonScriptEngine::executeInEditor(editor, false, &msg);
        ui->statusBar->showMessage(msg, ok ? 4000 : 6000);
    });

    auto *actionRunPythonFile = new QAction(tr("Run Python &File..."), this);
    actionRunPythonFile->setIcon(QIcon(QStringLiteral(":/icons/openFile.svg")));
    connect(actionRunPythonFile, &QAction::triggered, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, tr("Select Python Script"), QString(),
                                                        tr("Python Scripts (*.py);;All Files (*)"));
        if (filePath.isEmpty()) {
            return;
        }
        auto res = PythonScriptEngine::executeFile(filePath);
        if (auto *editor = currentEditor(); editor && !res.combinedOutput.isEmpty()) {
            QByteArray outBytes = ("\n" + res.combinedOutput).toUtf8();
            editor->insertText(editor->length(), outBytes.constData());
        }
        ui->statusBar->showMessage(res.success ? tr("Script executed successfully.")
                                               : tr("Script finished with exit code %1").arg(res.exitCode),
                                   4000);
    });

    menuPython->addAction(actionRunPython);
    menuPython->addAction(actionRunPythonFile);
    ui->menuEdit->addMenu(menuPython);

    // Setup Tail Monitoring (tail -f) Menu
    menuTailMonitoring = new QMenu(tr("Monitoring (tail -f)"), this);
    actionToggleTailMonitoring = new QAction(tr("&Monitoring (tail -f)"), this);
    actionToggleTailMonitoring->setCheckable(true);
    actionToggleTailMonitoring->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Y));
    actionToggleTailMonitoring->setToolTip(
        tr("Live disk tail monitoring: streaming appended log lines without modal dialogs"));
    actionToggleTailMonitoring->setStatusTip(
        tr("Live disk tail monitoring: streaming appended log lines without modal dialogs"));
    connect(actionToggleTailMonitoring, &QAction::triggered, this, &MainWindow::onToggleTailMonitoringTriggered);

    actionTailAutoScroll = new QAction(tr("&Auto-scroll to End on Update"), this);
    actionTailAutoScroll->setCheckable(true);
    actionTailAutoScroll->setChecked(true);
    connect(actionTailAutoScroll, &QAction::toggled, this,
            [this](bool checked) { TailMonitoringManager::instance()->setAutoScrollToEnd(currentEditor(), checked); });

    actionTailReloadNow = new QAction(tr("&Force Check / Reload Now"), this);
    actionTailReloadNow->setIcon(QIcon(QStringLiteral(":/icons/arrow_refresh.svg")));
    connect(actionTailReloadNow, &QAction::triggered, this,
            [this]() { TailMonitoringManager::instance()->forceCheck(currentEditor()); });

    actionTailOptions = new QAction(tr("Tail Monitoring &Options..."), this);
    actionTailOptions->setIcon(QIcon(QStringLiteral(":/icons/settings.svg")));
    connect(actionTailOptions, &QAction::triggered, this, [this]() {
        TailOptionsDialog dlg(this, currentEditor());
        dlg.exec();
    });

    menuTailMonitoring->addAction(actionToggleTailMonitoring);
    menuTailMonitoring->addAction(actionTailAutoScroll);
    menuTailMonitoring->addAction(actionTailReloadNow);
    menuTailMonitoring->addSeparator();
    menuTailMonitoring->addAction(actionTailOptions);

    ui->menuView->addMenu(menuTailMonitoring);

    connect(TailMonitoringManager::instance(), &TailMonitoringManager::monitoringStateChanged, this,
            &MainWindow::onTailMonitoringStateChanged);
    connect(TailMonitoringManager::instance(), &TailMonitoringManager::statusMessage, this,
            [this](const QString &msg) { ui->statusBar->showMessage(msg, 4000); });

    transparentWindowManager = new TransparentWindowManager(this, this);
    connect(transparentWindowManager, &TransparentWindowManager::transparencyChanged, this,
            &MainWindow::onTransparencyChanged);

    menuTransparency = new QMenu(tr("&Window Transparency"), this);
    actionToggleTransparency = new QAction(tr("&Toggle Transparent Mode"), this);
    actionToggleTransparency->setCheckable(true);
    actionToggleTransparency->setChecked(transparentWindowManager->isEnabled());
    actionToggleTransparency->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_T));
    connect(actionToggleTransparency, &QAction::triggered, this, &MainWindow::onToggleTransparencyTriggered);
    menuTransparency->addAction(actionToggleTransparency);

    actionIncreaseOpacity = new QAction(tr("&Increase Opacity (+10%)"), this);
    actionIncreaseOpacity->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Up));
    connect(actionIncreaseOpacity, &QAction::triggered, this, &MainWindow::onIncreaseOpacityTriggered);
    menuTransparency->addAction(actionIncreaseOpacity);

    actionDecreaseOpacity = new QAction(tr("&Decrease Opacity (-10%)"), this);
    actionDecreaseOpacity->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Down));
    connect(actionDecreaseOpacity, &QAction::triggered, this, &MainWindow::onDecreaseOpacityTriggered);
    menuTransparency->addAction(actionDecreaseOpacity);

    auto *menuPresets = new QMenu(tr("Opacity &Presets"), this);
    const QVector presets = {100, 90, 80, 75, 60, 50, 40, 25};
    for (int p : presets) {
        auto *act = new QAction(QStringLiteral("%1%").arg(p), this);
        connect(act, &QAction::triggered, this, [this, p]() {
            transparentWindowManager->setOpacity(p);
            if (!transparentWindowManager->isEnabled()) {
                transparentWindowManager->setEnabled(true);
            }
        });
        menuPresets->addAction(act);
    }
    menuTransparency->addMenu(menuPresets);

    menuTransparency->addSeparator();
    actionTransparencyOptions = new QAction(tr("Transparency &Options..."), this);
    actionTransparencyOptions->setIcon(QIcon(QStringLiteral(":/icons/settings.svg")));
    connect(actionTransparencyOptions, &QAction::triggered, this, &MainWindow::onTransparencyOptionsTriggered);
    menuTransparency->addAction(actionTransparencyOptions);

    ui->menuView->addSeparator();
    ui->menuView->addMenu(menuTransparency);

    connect(ClipboardMonitor::instance(), &ClipboardMonitor::monitoringStateChanged, this,
            &MainWindow::onClipboardMonitoringStateChanged);
    connect(ClipboardMonitor::instance(), &ClipboardMonitor::clipCaptured, this, &MainWindow::onClipboardClipCaptured);

    connect(ui->actionFind, &QAction::triggered, this,
            [this]() { showFindReplaceDialog(FindReplaceDialog::FIND_TAB); });

    connect(ui->actionFindNext, &QAction::triggered, this, [this]() {
        FindReplaceDialog *f = findChild<FindReplaceDialog *>(QString(), Qt::FindDirectChildrenOnly);

        if (f) {
            f->performNextSearch();
        }
    });

    connect(ui->actionFindPrevious, &QAction::triggered, this, [this]() {
        FindReplaceDialog *f = findChild<FindReplaceDialog *>(QString(), Qt::FindDirectChildrenOnly);

        if (f) {
            f->performPrevSearch();
        }
    });

    auto selectAndFind = [this](bool forward) {
        auto editor = currentEditor();
        auto range = editor->getContextText();

        if (range.cpMin == range.cpMax)
            return;

        auto text = editor->get_text_range(static_cast<int>(range.cpMin), static_cast<int>(range.cpMax));

        Finder f(editor);
        f.options().text = QString::fromUtf8(text);
        f.options().wrapAround = true;

        FindResult result = forward ? f.findNext() : f.findPrev();

        if (result)
            editor->goToRange(result.range);
    };

    connect(ui->actionSelectandFindNext, &QAction::triggered, this, [selectAndFind]() { selectAndFind(true); });

    connect(ui->actionSelectandFindPrevious, &QAction::triggered, this, [selectAndFind]() { selectAndFind(false); });

    connect(ui->actionQuickFind, &QAction::triggered, this, [this]() {
        QuickFindWidget *quickFind = findChild<QuickFindWidget *>(QString(), Qt::FindDirectChildrenOnly);

        if (quickFind == Q_NULLPTR) {
            quickFind = new QuickFindWidget(this);
        }

        quickFind->setEditor(currentEditor());
        quickFind->setFocus();
        quickFind->show();
    });

    connect(ui->actionReplace, &QAction::triggered, this,
            [this]() { showFindReplaceDialog(FindReplaceDialog::REPLACE_TAB); });

    connect(ui->actionSearchAndBookmark, &QAction::triggered, this,
            [this]() { showFindReplaceDialog(FindReplaceDialog::MARK_TAB); });

    connect(ui->actionGoToLine, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        const int currentLine = static_cast<int>(editor->lineFromPosition(editor->currentPos())) + 1;
        const auto maxLine = static_cast<int>(editor->lineCount());
        bool ok;

        auto d = QInputDialog(this);
        Qt::WindowFlags flags = d.windowFlags() & ~Qt::WindowContextHelpButtonHint;
        int lineToGoTo = QInputDialog::getInt(this, tr("Go to line"), tr("Line Number (1 - %1)").arg(maxLine),
                                              currentLine, 1, maxLine, 1, &ok, flags);

        if (ok) {
            editor->ensureVisible(lineToGoTo - 1);
            editor->gotoLine(lineToGoTo - 1);
            editor->verticalCentreCaret();
        }
    });

    // Style all actions that have a MarkerNumber and interpret that as the color needed
    const auto *markerAppDecorator = app->findChild<MarkerAppDecorator *>(QString(), Qt::FindDirectChildrenOnly);
    for (QAction *action : findChildren<QAction *>()) {
        if (action->property("MarkerNumber").isValid()) {
            int markerNumber = action->property("MarkerNumber").toInt();
            action->setIcon(ActionUtils::createSolidIcon(markerAppDecorator->markerColor(markerNumber)));
        }
    }

    auto mark_callback = [this, app]() {
        if (auto *decorator = app->findChild<MarkerAppDecorator *>(QString(), Qt::FindDirectChildrenOnly);
            decorator && decorator->isEnabled() && sender()->property("MarkerNumber").isValid()) {
            ScintillaNext *editor = currentEditor();
            decorator->mark(editor, sender()->property("MarkerNumber").toInt());
        }
    };

    auto clear_mark_callback = [this, app]() {
        if (auto *decorator = app->findChild<MarkerAppDecorator *>(QString(), Qt::FindDirectChildrenOnly);
            decorator && decorator->isEnabled() && sender()->property("MarkerNumber").isValid()) {
            ScintillaNext *editor = currentEditor();
            decorator->clear(editor, sender()->property("MarkerNumber").toInt());
        }
    };

    focusedView = new FocusedView(this);

    auto showFocusedViewDialog = [this](ScintillaNext *editor) {
        if (!focusedViewDialog) {
            focusedViewDialog = new FocusedViewDialog(focusedView, this, this);
        }
        focusedViewDialog->prepareForEditor(editor);
        focusedViewDialog->show();
        focusedViewDialog->raise();
        focusedViewDialog->activateWindow();
    };

    connect(ui->actionToggleFocusedView, &QAction::triggered, this, [this, showFocusedViewDialog]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        bool dialogNeeded = false;
        const auto result = focusedView->toggleFocusedView(editor, &dialogNeeded);
        if (!dialogNeeded) {
            ui->statusBar->showMessage(result.message, 4000);
            updateContentBasedUi(editor);
            return;
        }
        showFocusedViewDialog(editor);
    });

    connect(ui->actionFocusedViewFilterDialog, &QAction::triggered, this, [this, showFocusedViewDialog]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        showFocusedViewDialog(editor);
    });

    connect(ui->actionFocusedViewBookmarks, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        auto result = focusedView->filterByBookmarks(editor);
        ui->statusBar->showMessage(result.message, 4000);
        updateContentBasedUi(editor);
    });

    connect(ui->actionFocusedViewClear, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        auto result = focusedView->clearFilter(editor);
        ui->statusBar->showMessage(result.message, 4000);
        updateContentBasedUi(editor);
    });

    connect(ui->actionCustomizeStatusBarFields, &QAction::triggered, this,
            [this]() { ui->statusBar->showCustomizerDialog(); });

    ui->actionToggleAutoLogTimestamp->setChecked(LogAutoTimestamp::isAutoLogEnabled());
    connect(ui->actionToggleAutoLogTimestamp, &QAction::toggled, this,
            [](bool checked) { LogAutoTimestamp::setAutoLogEnabled(checked); });

    connect(ui->actionInsertLogHeader, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        if (LogAutoTimestamp::insertLogHeader(editor)) {
            ui->statusBar->showMessage(tr("Inserted .LOG header at line 1"), 3000);
        } else {
            ui->statusBar->showMessage(tr("Document already contains .LOG header"), 3000);
        }
    });

    connect(ui->actionAppendLogTimestamp, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor) {
            ui->statusBar->showMessage(tr("No active editor"), 3000);
            return;
        }
        if (LogAutoTimestamp::appendLogTimestamp(editor)) {
            ui->statusBar->showMessage(tr("Appended log timestamp at end of document"), 3000);
        }
    });

    markOccurrences = new MarkOccurrences(this);
    connect(markOccurrences, &MarkOccurrences::occurrencesMarked, ui->statusBar,
            &EditorInfoStatusBar::setOccurrencesCount);
    connect(markOccurrences, &MarkOccurrences::marksCleared, ui->statusBar,
            &EditorInfoStatusBar::clearOccurrencesCount);

    connect(ui->actionMarkAllOccurrences, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        int count = markOccurrences->markAll(editor);
        QString query = markOccurrences->lastQuery();
        if (count > 0) {
            ui->statusBar->showMessage(tr("Marked %1 occurrence(s) of \"%2\"").arg(count).arg(query), 5000);
        } else if (!query.isEmpty()) {
            ui->statusBar->showMessage(tr("No occurrences found for \"%1\"").arg(query), 4000);
        } else {
            ui->statusBar->showMessage(tr("No text selected to mark"), 3000);
        }
    });

    connect(ui->actionClearAllOccurrencesMarks, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        markOccurrences->clearMarks(editor);
        ui->statusBar->showMessage(tr("Occurrence marks cleared"), 3000);
    });

    connect(ui->actionMarkStyle1, &QAction::triggered, this, mark_callback);
    connect(ui->actionMarkStyle2, &QAction::triggered, this, mark_callback);
    connect(ui->actionMarkStyle3, &QAction::triggered, this, mark_callback);

    connect(ui->actionClearStyle1, &QAction::triggered, this, clear_mark_callback);
    connect(ui->actionClearStyle2, &QAction::triggered, this, clear_mark_callback);
    connect(ui->actionClearStyle3, &QAction::triggered, this, clear_mark_callback);

    connect(ui->actionClearAllStyles, &QAction::triggered, this, [this, app]() {
        if (auto *decorator = app->findChild<MarkerAppDecorator *>(QString(), Qt::FindDirectChildrenOnly);
            decorator && decorator->isEnabled()) {
            decorator->clearAll(currentEditor());
        }
    });

    connect(ui->actionToggleBookmark, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto *bookMarkDecorator = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);

        if (bookMarkDecorator && bookMarkDecorator->isEnabled()) {
            editor->forEachLineInSelection(static_cast<int>(editor->mainSelection()),
                                           [bookMarkDecorator](int line) { bookMarkDecorator->toggleBookmark(line); });
        }
    });

    connect(ui->actionNextBookmark, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto *bookMarkDecorator = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);

        if (bookMarkDecorator && bookMarkDecorator->isEnabled()) {
            auto currentLine = static_cast<int>(editor->lineFromPosition(editor->currentPos()));
            int nextBookmarkedLine = bookMarkDecorator->nextBookmarkAfter(currentLine + 1);

            if (nextBookmarkedLine != -1) {
                editor->ensureVisibleEnforcePolicy(nextBookmarkedLine);
                editor->gotoLine(nextBookmarkedLine);
            }
        }
    });

    connect(ui->actionClearBookmarks, &QAction::triggered, this, [this]() {
        const ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto *bookMarkDecorator = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);

        if (bookMarkDecorator && bookMarkDecorator->isEnabled()) {
            bookMarkDecorator->clearAllBookmarks();
        }
    });

    connect(ui->actionInvertBookmarks, &QAction::triggered, this, [this]() {
        const ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto *bookMarkDecorator = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);

        if (bookMarkDecorator && bookMarkDecorator->isEnabled()) {
            for (int line = 0; line < editor->lineCount(); line++) {
                bookMarkDecorator->toggleBookmark(line);
            }
        }
    });

    connect(ui->actionPreviousBookmark, &QAction::triggered, this, [this]() {
        ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto *bookMarkDecorator = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);

        if (bookMarkDecorator && bookMarkDecorator->isEnabled()) {
            auto currentLine = static_cast<int>(editor->lineFromPosition(editor->currentPos()));
            int prevBookmarkedLine = bookMarkDecorator->previousBookMarkBefore(currentLine - 1);

            if (prevBookmarkedLine != -1) {
                editor->ensureVisibleEnforcePolicy(prevBookmarkedLine);
                editor->gotoLine(prevBookmarkedLine);
            }
        }
    });

    connect(ui->actionCutBookmarkedLines, &QAction::triggered, this, [this]() {
        const ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto *bookMarkDecorator = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);

        if (bookMarkDecorator && bookMarkDecorator->isEnabled()) {
            QString s = bookMarkDecorator->cutBookMarkedLines();

            if (!s.isEmpty()) {
                QApplication::clipboard()->setText(s);
            }
        }
    });

    connect(ui->actionCopyBookmarkedLines, &QAction::triggered, this, [this]() {
        const ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto *bookMarkDecorator = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);

        if (bookMarkDecorator && bookMarkDecorator->isEnabled()) {
            QString s = bookMarkDecorator->copyBookMarkedLines();

            if (!s.isEmpty()) {
                QApplication::clipboard()->setText(s);
            }
        }
    });

    connect(ui->actionDeleteBookmarkedLines, &QAction::triggered, this, [this]() {
        const ScintillaNext *editor = currentEditor();
        if (!editor)
            return;
        auto *bookMarkDecorator = editor->findChild<BookMarkDecorator *>(QString(), Qt::FindDirectChildrenOnly);

        if (bookMarkDecorator && bookMarkDecorator->isEnabled()) {
            bookMarkDecorator->deleteBookMarkedLines();
        }
    });

    // The action needs added to the window so it can be triggered via the keyboard
    addAction(ui->actionNextTab);
    ui->actionNextTab->setShortcuts(ui->actionNextTab->shortcuts() << QKeySequence(Qt::CTRL | Qt::Key_PageDown));
    connect(ui->actionNextTab, &QAction::triggered, this, [this]() {
        int index = dockedEditor->currentDockArea()->currentIndex();
        int total = dockedEditor->currentDockArea()->dockWidgetsCount();

        index++;
        dockedEditor->currentDockArea()->setCurrentIndex(index < total ? index : 0);
    });

    // The action needs added to the window so it can be triggered via the keyboard
    addAction(ui->actionPreviousTab);
    ui->actionPreviousTab->setShortcuts(ui->actionPreviousTab->shortcuts() << QKeySequence(Qt::CTRL | Qt::Key_PageUp));
    connect(ui->actionPreviousTab, &QAction::triggered, this, [this]() {
        int index = dockedEditor->currentDockArea()->currentIndex();
        int total = dockedEditor->currentDockArea()->dockWidgetsCount();

        index--;
        dockedEditor->currentDockArea()->setCurrentIndex(index >= 0 ? index : total - 1);
    });

    ui->pushExitFullScreen->setParent(this);  // This is important
    ui->pushExitFullScreen->setVisible(false);
#if defined(Q_OS_MACOS)
    ui->actionFullScreen->setShortcuts({QKeySequence(Qt::META | Qt::CTRL | Qt::Key_F), QKeySequence(Qt::Key_F11)});
#else
    ui->actionFullScreen->setShortcut(QKeySequence::FullScreen);
#endif
    connect(ui->pushExitFullScreen, &QPushButton::clicked, ui->actionFullScreen, &QAction::trigger);
    connect(ui->actionFullScreen, &QAction::triggered, this, [this](bool b) {
        static bool wasMaximized;
        if (b) {
            ui->menuBar->setMaximumHeight(0);
            ui->mainToolBar->setMaximumHeight(0);
            wasMaximized = isMaximized();
            if (wasMaximized) {
                setGeometry(screen()->availableGeometry());
            }
            showFullScreen();
            ui->pushExitFullScreen->setGeometry(width() - 20, 0, 20, 20);
            ui->pushExitFullScreen->show();
            ui->pushExitFullScreen->raise();
        } else {
            ui->menuBar->setMaximumHeight(QWIDGETSIZE_MAX);
            ui->mainToolBar->setMaximumHeight(QWIDGETSIZE_MAX);
            wasMaximized ? showMaximized() : showNormal();
            ui->pushExitFullScreen->hide();
        }
    });

    // Show All Characters is just a short cut to toggle whitespace and EOL on
    ui->actionShowAllCharacters->setChecked(app->getSettings()->showWhitespace() &&
                                            app->getSettings()->showEndOfLine());
    connect(ui->actionShowAllCharacters, &QAction::triggered, app->getSettings(),
            &ApplicationSettings::setShowWhitespace);
    connect(ui->actionShowAllCharacters, &QAction::triggered, app->getSettings(),
            &ApplicationSettings::setShowEndOfLine);

    // Show White Space
    ui->actionShowWhitespace->setChecked(app->getSettings()->showWhitespace());
    connect(app->getSettings(), &ApplicationSettings::showWhitespaceChanged, ui->actionShowWhitespace,
            &QAction::setChecked);
    connect(ui->actionShowWhitespace, &QAction::toggled, app->getSettings(), &ApplicationSettings::setShowWhitespace);
    // Update the "Show All Character" action
    connect(ui->actionShowWhitespace, &QAction::toggled, this,
            [this](bool b) { ui->actionShowAllCharacters->setChecked(b && ui->actionShowEndofLine->isChecked()); });

    // Show EOL
    ui->actionShowEndofLine->setChecked(app->getSettings()->showEndOfLine());
    connect(app->getSettings(), &ApplicationSettings::showEndOfLineChanged, ui->actionShowEndofLine,
            &QAction::setChecked);
    connect(ui->actionShowEndofLine, &QAction::toggled, app->getSettings(), &ApplicationSettings::setShowEndOfLine);
    // Update the "Show All Character" action
    connect(ui->actionShowEndofLine, &QAction::toggled, this,
            [this](bool b) { ui->actionShowAllCharacters->setChecked(b && ui->actionShowWhitespace->isChecked()); });

    // Show Wrap Symbol
    ui->actionShowWrapSymbol->setChecked(app->getSettings()->showWrapSymbol());
    connect(app->getSettings(), &ApplicationSettings::showWrapSymbolChanged, ui->actionShowWrapSymbol,
            &QAction::setChecked);
    connect(ui->actionShowWrapSymbol, &QAction::toggled, app->getSettings(), &ApplicationSettings::setShowWrapSymbol);

    // Show Indentation Guide
    ui->actionShowIndentGuide->setChecked(app->getSettings()->showIndentGuide());
    connect(app->getSettings(), &ApplicationSettings::showIndentGuideChanged, ui->actionShowIndentGuide,
            &QAction::setChecked);
    connect(ui->actionShowIndentGuide, &QAction::toggled, app->getSettings(), &ApplicationSettings::setShowIndentGuide);

    // Word Wrap
    ui->actionWordWrap->setChecked(app->getSettings()->wordWrap());
    connect(app->getSettings(), &ApplicationSettings::wordWrapChanged, ui->actionWordWrap, &QAction::setChecked);
    connect(ui->actionWordWrap, &QAction::toggled, app->getSettings(), &ApplicationSettings::setWordWrap);

    // URL Highlighting
    ui->actionToggleURLHighlighting->setChecked(app->getSettings()->urlHighlighting());
    connect(app->getSettings(), &ApplicationSettings::urlHighlightingChanged, ui->actionToggleURLHighlighting,
            &QAction::setChecked);
    connect(ui->actionToggleURLHighlighting, &QAction::toggled, app->getSettings(),
            &ApplicationSettings::setURLHighlighting);

    // Change History Margin
    auto *actionChangeHistory = new QAction(tr("Change &History Margin"), this);
    actionChangeHistory->setCheckable(true);
    actionChangeHistory->setChecked(app->getSettings()->showChangeHistory());
    actionChangeHistory->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_H));
    connect(app->getSettings(), &ApplicationSettings::showChangeHistoryChanged, actionChangeHistory,
            &QAction::setChecked);
    connect(actionChangeHistory, &QAction::toggled, app->getSettings(), &ApplicationSettings::setShowChangeHistory);
    ui->menuView->addAction(actionChangeHistory);

    auto *actionNextChange = new QAction(tr("&Next Change"), this);
    actionNextChange->setShortcut(QKeySequence(Qt::ALT | Qt::Key_F7));
    connect(actionNextChange, &QAction::triggered, this, [this]() {
        if (auto *ed = currentEditor()) {
            if (auto *dec = ed->findChild<ChangeHistoryDecorator *>(QString(), Qt::FindDirectChildrenOnly)) {
                auto curLine = static_cast<int>(ed->lineFromPosition(ed->currentPos()));
                int targetLine = dec->nextChangeAfter(curLine);
                if (targetLine != -1) {
                    ed->gotoLine(targetLine);
                    ed->scrollVertical(targetLine, 0);
                    ed->setFocus();
                }
            }
        }
    });
    ui->menuView->addAction(actionNextChange);

    auto *actionPrevChange = new QAction(tr("&Previous Change"), this);
    actionPrevChange->setShortcut(QKeySequence(Qt::ALT | Qt::SHIFT | Qt::Key_F7));
    connect(actionPrevChange, &QAction::triggered, this, [this]() {
        if (auto *ed = currentEditor()) {
            if (auto *dec = ed->findChild<ChangeHistoryDecorator *>(QString(), Qt::FindDirectChildrenOnly)) {
                auto curLine = static_cast<int>(ed->lineFromPosition(ed->currentPos()));
                int targetLine = dec->previousChangeBefore(curLine);
                if (targetLine != -1) {
                    ed->gotoLine(targetLine);
                    ed->scrollVertical(targetLine, 0);
                    ed->setFocus();
                }
            }
        }
    });
    ui->menuView->addAction(actionPrevChange);

    // Zooming controls all editors simulaneously
    connect(ui->actionZoomIn, &QAction::triggered, this, [this]() {
        for (ScintillaNext *editor : editors()) {
            editor->zoomIn();
        }
        zoomLevel = static_cast<int>(currentEditor()->zoom());

        showEditorZoomLevelIndicator();
    });
    connect(ui->actionZoomOut, &QAction::triggered, this, [this]() {
        // Scintilla can zoom out to "-10" but on a screen with fractional scaling it throws a lot of Qt warnings
        if (zoomLevel == -9)
            return;

        for (ScintillaNext *editor : editors()) {
            editor->zoomOut();
        }
        zoomLevel = static_cast<int>(currentEditor()->zoom());

        showEditorZoomLevelIndicator();
    });
    connect(ui->actionZoomReset, &QAction::triggered, this, [this]() {
        for (ScintillaNext *editor : editors()) {
            editor->setZoom(0);
        }
        zoomLevel = 0;

        showEditorZoomLevelIndicator();
    });

    // Zoom watcher has detected a zoom event, so just trigger the UI action
    connect(zoomEventWatcher, &ZoomEventWatcher::zoomIn, ui->actionZoomIn, &QAction::trigger);
    connect(zoomEventWatcher, &ZoomEventWatcher::zoomOut, ui->actionZoomOut, &QAction::trigger);

    connectEditorAction(ui->actionFoldAll, &ScintillaNext::foldAll,
                        SC_FOLDACTION_CONTRACT | SC_FOLDACTION_CONTRACT_EVERY_LEVEL);
    connectEditorAction(ui->actionUnfoldAll, &ScintillaNext::foldAll,
                        SC_FOLDACTION_EXPAND | SC_FOLDACTION_CONTRACT_EVERY_LEVEL);

    connectEditorAction(ui->actionFoldLevel1, &ScintillaNext::foldAllLevels, 0);
    connectEditorAction(ui->actionFoldLevel2, &ScintillaNext::foldAllLevels, 1);
    connectEditorAction(ui->actionFoldLevel3, &ScintillaNext::foldAllLevels, 2);
    connectEditorAction(ui->actionFoldLevel4, &ScintillaNext::foldAllLevels, 3);
    connectEditorAction(ui->actionFoldLevel5, &ScintillaNext::foldAllLevels, 4);
    connectEditorAction(ui->actionFoldLevel6, &ScintillaNext::foldAllLevels, 5);
    connectEditorAction(ui->actionFoldLevel7, &ScintillaNext::foldAllLevels, 6);
    connectEditorAction(ui->actionFoldLevel8, &ScintillaNext::foldAllLevels, 7);
    connectEditorAction(ui->actionFoldLevel9, &ScintillaNext::foldAllLevels, 8);

    connectEditorAction(ui->actionUnfoldLevel1, &ScintillaNext::unFoldAllLevels, 0);
    connectEditorAction(ui->actionUnfoldLevel2, &ScintillaNext::unFoldAllLevels, 1);
    connectEditorAction(ui->actionUnfoldLevel3, &ScintillaNext::unFoldAllLevels, 2);
    connectEditorAction(ui->actionUnfoldLevel4, &ScintillaNext::unFoldAllLevels, 3);
    connectEditorAction(ui->actionUnfoldLevel5, &ScintillaNext::unFoldAllLevels, 4);
    connectEditorAction(ui->actionUnfoldLevel6, &ScintillaNext::unFoldAllLevels, 5);
    connectEditorAction(ui->actionUnfoldLevel7, &ScintillaNext::unFoldAllLevels, 6);
    connectEditorAction(ui->actionUnfoldLevel8, &ScintillaNext::unFoldAllLevels, 7);
    connectEditorAction(ui->actionUnfoldLevel9, &ScintillaNext::unFoldAllLevels, 8);

    languageActionGroup = new QActionGroup(this);
    languageActionGroup->setExclusive(true);

    connect(ui->actionPreferences, &QAction::triggered, this, [this, app] {
        PreferencesDialog *pd = findChild<PreferencesDialog *>(QString(), Qt::FindDirectChildrenOnly);

        if (pd == Q_NULLPTR) {
            pd = new PreferencesDialog(app->getSettings(), this);
        }

        pd->show();
        pd->raise();
        pd->activateWindow();
    });

    // The macro manager has already loaded any saved macros, so it might have some already
    ui->actionRunMacroMultipleTimes->setEnabled(macroManager.availableMacros().size() > 0);
    ui->actionEditMacros->setEnabled(macroManager.availableMacros().size() > 0);

    connect(ui->actionMacroRecording, &QAction::triggered, this, [this](bool b) {
        if (b) {
            macroManager.startRecording(currentEditor());
        } else {
            macroManager.stopRecording();
        }
    });

    connect(&macroManager, &MacroManager::recordingStarted, this, [this]() {
        ui->actionMacroRecording->setText(tr("Stop Recording"));

        // A macro is being recorded so disable some macro options
        ui->actionPlayback->setEnabled(false);
        ui->actionRunMacroMultipleTimes->setEnabled(false);
        ui->actionSaveCurrentRecordedMacro->setEnabled(false);
    });

    connect(&macroManager, &MacroManager::recordingStopped, this, [this]() {
        ui->actionMacroRecording->setText(tr("Start Recording"));

        // Only enable these if the macro manager recorded a valid macro
        ui->actionPlayback->setEnabled(macroManager.hasCurrentUnsavedMacro());
        ui->actionSaveCurrentRecordedMacro->setEnabled(macroManager.hasCurrentUnsavedMacro());

        // The macro manager might have other macros
        ui->actionRunMacroMultipleTimes->setEnabled(macroManager.availableMacros().size() > 0 ||
                                                    macroManager.hasCurrentUnsavedMacro());
    });

    connect(ui->actionPlayback, &QAction::triggered, this,
            [this]() { macroManager.replayCurrentMacro(currentEditor()); });

    connect(ui->actionSaveCurrentRecordedMacro, &QAction::triggered, this, [this]() {
        MacroSaveDialog macroSaveDialog;

        macroSaveDialog.show();
        macroSaveDialog.raise();
        macroSaveDialog.activateWindow();

        if (macroSaveDialog.exec() == QDialog::Accepted) {
            // We have at least 1 saved macro at this point
            ui->actionEditMacros->setEnabled(true);

            // The macro has been saved so disable save option
            ui->actionSaveCurrentRecordedMacro->setEnabled(false);

            // Note: does the macro name already exist? Make the user retry

            macroManager.saveCurrentMacro(macroSaveDialog.getName());

            // Note: handle shortcuts
            if (!macroSaveDialog.getShortcut().isEmpty()) {
                // do something with msd.getShortcut().isEmpty()
            }
        }
    });

    connect(ui->actionRunMacroMultipleTimes, &QAction::triggered, this, [this]() {
        MacroRunDialog *macroRunDialog = findChild<MacroRunDialog *>(QString(), Qt::FindDirectChildrenOnly);

        if (macroRunDialog == Q_NULLPTR) {
            macroRunDialog = new MacroRunDialog(this, &macroManager);

            connect(macroRunDialog, &MacroRunDialog::execute, this, [this](const Macro *macro, int times) {
                if (times > 0)
                    macro->replay(currentEditor(), times);
                else if (times == -1)
                    macro->replayTillEndOfFile(currentEditor());
            });
        }

        macroRunDialog->show();
        macroRunDialog->raise();
        macroRunDialog->activateWindow();
    });

    connect(ui->actionEditMacros, &QAction::triggered, this, [this]() {
        MacroEditorDialog med(this, &macroManager);

        med.show();
        med.raise();
        med.activateWindow();

        med.exec();

        ui->actionEditMacros->setEnabled(macroManager.availableMacros().size() > 0);
    });

    connect(ui->menuMacro, &QMenu::aboutToShow, this, [this]() {
        // NOTE: its unfortunate that this has to be hard coded, but there's no way
        // to easily determine what should or shouldn't be there
        while (ui->menuMacro->actions().size() > 6) {
            delete ui->menuMacro->actions().takeLast();  // NOSONAR(cpp:S5025)
        }

        for (const Macro *m : macroManager.availableMacros()) {
            ui->menuMacro->addAction(m->getName(), [this, m]() { m->replay(currentEditor()); });
        }
    });

    ui->actionAboutQt->setIcon(QPixmap(QLatin1String(":/qt-project.org/qmessagebox/images/qtlogo-64.png")));
    connect(ui->actionAboutQt, &QAction::triggered, &QApplication::aboutQt);

    // Keyboard Shortcuts & Accessibility Guide
    actionAccessibilityHelp = new QAction(tr("&Keyboard Shortcuts && Accessibility..."), this);
    actionAccessibilityHelp->setIcon(QIcon(QStringLiteral(":/icons/SparkNotePad.svg")));
#if defined(Q_OS_MACOS)
    actionAccessibilityHelp->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F1));
#else
    actionAccessibilityHelp->setShortcut(QKeySequence(Qt::Key_F1));
#endif
    connect(actionAccessibilityHelp, &QAction::triggered, this,
            [this]() { AccessibilityManager::instance()->openAccessibilityHelp(this); });
    ui->menuHelp->insertAction(ui->actionAboutSparkNotePad, actionAccessibilityHelp);

    // User & Developer Documentation
    auto *actionUserGuide = new QAction(tr("SparkNotePad &User Guide"), this);
    actionUserGuide->setIcon(QIcon(QStringLiteral(":/icons/SparkNotePad.svg")));
#if defined(Q_OS_MACOS)
    actionUserGuide->setShortcut(QKeySequence(Qt::Key_F1));
#else
    actionUserGuide->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F1));
#endif
    connect(actionUserGuide, &QAction::triggered, this, &MainWindow::onUserGuideTriggered);
    ui->menuHelp->insertAction(ui->actionAboutSparkNotePad, actionUserGuide);

    auto *actionDeveloperGuide = new QAction(tr("SparkNotePad &Developer Guide"), this);
    actionDeveloperGuide->setIcon(QIcon(QStringLiteral(":/icons/SparkNotePad.svg")));
    actionDeveloperGuide->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F12));
    connect(actionDeveloperGuide, &QAction::triggered, this, &MainWindow::onDeveloperGuideTriggered);
    ui->menuHelp->insertAction(ui->actionAboutSparkNotePad, actionDeveloperGuide);

    ui->actionAboutSparkNotePad->setShortcut(QKeySequence::HelpContents);
    connect(ui->actionAboutSparkNotePad, &QAction::triggered, this, [this]() {
        AboutDialog dlg(this);
        dlg.exec();
    });

    connect(ui->actionPerformanceDiagnostics, &QAction::triggered, this, [this]() {
        PerformanceDiagnosticsDialog dlg(this, currentEditor());
        dlg.exec();
    });

    connect(ui->actionDebugInfo, &QAction::triggered, this,
            [this, app]() { ModernMessageBox::information(this, tr("Debug Info"), app->debugInfo().join('\n')); });

    tabsQuickActionsBar = new TabsQuickActionsBar(ui->menuBar);
    ui->menuBar->setCornerWidget(tabsQuickActionsBar, Qt::TopRightCorner);
    connect(tabsQuickActionsBar, &TabsQuickActionsBar::createNewTabClicked, this, &MainWindow::newFile);
    connect(tabsQuickActionsBar, &TabsQuickActionsBar::closeCurrentTabClicked, this, &MainWindow::closeCurrentFile);
    connect(tabsQuickActionsBar, &TabsQuickActionsBar::tabsMenuAboutToShow, this, [this](QMenu *editorsMenu) {
        const auto editorsList = editors();

        editorsMenu->clear();

        for (const auto editor : editorsList) {
            const auto iconPath = editor->isSavedToDisk() ? ":/icons/saved.svg" : ":/icons/unsaved.svg";
            const auto action = editorsMenu->addAction(QIcon(iconPath), editor->getName());

            if (editor->isActiveWindow()) {
                auto font = action->font();
                font.setBold(true);
                action->setFont(font);
            }

            connect(action, &QAction::triggered, this, [this, editor]() { switchToEditor(editor); });
        }
    });

    connect(ui->actionShowInExplorer, &QAction::triggered, this, &MainWindow::onRevealInFileManagerTriggered);
    connect(ui->actionOpenTerminalHere, &QAction::triggered, this, &MainWindow::onOpenTerminalHereTriggered);

    auto *editorInspectorDock = new EditorInspectorDock(this);
    editorInspectorDock->hide();
    addDockWidget(Qt::RightDockWidgetArea, editorInspectorDock);

    languageInspectorDock = new LanguageInspectorDock(this);
    languageInspectorDock->hide();
    addDockWidget(Qt::RightDockWidgetArea, languageInspectorDock);
    auto *toggleLanguageInspectorAction = languageInspectorDock->toggleViewAction();
    toggleLanguageInspectorAction->setText(tr("Language Inspector"));
    toggleLanguageInspectorAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_I));
    ui->menuView->addAction(toggleLanguageInspectorAction);

    auto *luaConsoleDock = new LuaConsoleDock(app->getLuaState(), this);
    luaConsoleDock->hide();
    addDockWidget(Qt::BottomDockWidgetArea, luaConsoleDock);

    auto *debugLogDock = new DebugLogDock(this);
    debugLogDock->hide();
    addDockWidget(Qt::RightDockWidgetArea, debugLogDock);

    ui->menuHelp->insertActions(ui->menuHelp->actions().at(0),
                                {luaConsoleDock->toggleViewAction(), languageInspectorDock->toggleViewAction(),
                                 editorInspectorDock->toggleViewAction(), debugLogDock->toggleViewAction()});

    fileBrowserDock = new FileBrowserDock(this);
    fileBrowserDock->hide();
    addDockWidget(Qt::LeftDockWidgetArea, fileBrowserDock);
    auto *toggleFileBrowserAction = fileBrowserDock->toggleViewAction();
    toggleFileBrowserAction->setText(tr("File Browser"));
    toggleFileBrowserAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_O));
    ui->menuView->addAction(toggleFileBrowserAction);
    connect(fileBrowserDock, &FileBrowserDock::fileOpenRequested, this, &MainWindow::openFile);

    auto *fawDock = new FolderAsWorkspaceDock(this);
    fawDock->hide();
    addDockWidget(Qt::LeftDockWidgetArea, fawDock);
    ui->menuView->addAction(fawDock->toggleViewAction());
    connect(fawDock, &FolderAsWorkspaceDock::fileDoubleClicked, this, &MainWindow::openFile);

    auto *fileListDock = new FileListDock(this);
    fileListDock->hide();
    addDockWidget(Qt::LeftDockWidgetArea, fileListDock);
    ui->menuView->addAction(fileListDock->toggleViewAction());

    functionListDock = new FunctionListDock(this);
    functionListDock->hide();
    addDockWidget(Qt::RightDockWidgetArea, functionListDock);
    auto *toggleFunctionListAction = functionListDock->toggleViewAction();
    toggleFunctionListAction->setText(tr("Function List"));
    toggleFunctionListAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_F));
    ui->menuView->addAction(toggleFunctionListAction);

    markdownSafePreviewDock = new MarkdownSafePreviewDock(this);
    markdownSafePreviewDock->hide();
    addDockWidget(Qt::RightDockWidgetArea, markdownSafePreviewDock);
    auto *toggleMarkdownPreviewAction = markdownSafePreviewDock->toggleViewAction();
    toggleMarkdownPreviewAction->setText(tr("Markdown Safe Preview"));
    toggleMarkdownPreviewAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_M));
    ui->menuView->addAction(toggleMarkdownPreviewAction);
    connect(markdownSafePreviewDock, &QDockWidget::visibilityChanged, this, [this](bool visible) {
        if (visible) {
            markdownSafePreviewDock->setActiveEditor(currentEditor());
        }
    });

    connect(app->getSettings(), &ApplicationSettings::showMenuBarChanged, this, [this](bool showMenuBar) {
        // Don't 'hide' it, else the actions won't be enabled
        ui->menuBar->setMaximumHeight(showMenuBar ? QWIDGETSIZE_MAX : 0);
    });
    connect(app->getSettings(), &ApplicationSettings::showToolBarChanged, ui->mainToolBar, &QToolBar::setVisible);
    connect(app->getSettings(), &ApplicationSettings::showStatusBarChanged, ui->statusBar, &QStatusBar::setVisible);
    connect(ui->statusBar, &EditorInfoStatusBar::customContextMenuRequestedForEOLLabel, this,
            [this](const QPoint &pos) { ui->menuEOLConversion->popup(pos); });

    // It seems restoreState() does not affect the status bar so set it manually
    ui->statusBar->setVisible(app->getSettings()->showStatusBar());

    // Register active window with MacServicesHandler
    MacServicesHandler::instance()->setActiveWindow(this);

    // Native Emoji & Symbols
    auto *actionEmoji = new QAction(tr("&Emoji && Symbols"), this);
#if defined(Q_OS_MACOS)
    actionEmoji->setShortcut(QKeySequence(Qt::CTRL | Qt::META | Qt::Key_Space));
#endif
    connect(actionEmoji, &QAction::triggered, this, []() { MacServicesHandler::showEmojiAndSymbolsPalette(); });
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(actionEmoji);

    setupLanguageMenu();
    setupRunMenu();
    setupWindowMenu();
    ThemeManager::instance()->initialize(app->getSettings());
    setupThemeMenu();
    ScreenDisplayManager::instance().registerMainWindow(this);
    AccessibilityManager::instance()->setupAccessibility(this);

    applyStyleSheet();

    restoreSettings();

    initUpdateCheck();

    ensureMenuIconsVisible(this);
}

MainWindow::~MainWindow() {
    delete ui;  // NOSONAR(cpp:S5025)
}

void MainWindow::applyCustomShortcuts() const {
    ApplicationSettings *settings = app->getSettings();
    settings->beginGroup("Shortcuts");

    for (const QString &actionName : settings->childKeys()) {
        QAction *action = findChild<QAction *>(QStringLiteral("action") + actionName, Qt::FindDirectChildrenOnly);

        if (!action) {
            qWarning() << "CustomShortcut: Cannot find action" << actionName;
            continue;
        }

        const QVariant value = settings->value(actionName);
        if (!value.canConvert<QStringList>()) {
            qWarning() << "CustomShortcut: Invalid shortcut format for" << actionName;
            continue;
        }

        QList<QKeySequence> shortcuts;
        for (const QString &shortcutString : value.toStringList()) {
            auto sequence = QKeySequence(shortcutString);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if (!sequence.isEmpty() && sequence[0].key() != Qt::Key_unknown) {
#else
            if (!sequence.isEmpty() && (sequence[0] & ~Qt::KeyboardModifierMask) != Qt::Key_unknown) {
#endif
                shortcuts.append(sequence);
            } else {
                qWarning() << "CustomShortcut: Cannot create QKeySequence(" << shortcutString << ") for " << actionName;
            }
        }

        if (!shortcuts.isEmpty()) {
            action->setShortcuts(shortcuts);
        }
    }

    settings->endGroup();
}

void MainWindow::setupLanguageMenu() {
    qInfo(Q_FUNC_INFO);

    QStringList language_names = app->getLanguages();

    int i = 0;
    while (i < language_names.size()) {
        QList<QAction *> actions;
        int j = i;

        // Get all consecutive names that start with the same letter
        // NOTE: this loop always runs once since i == j the first time
        while (j < language_names.size() && language_names[i][0].toUpper() == language_names[j][0].toUpper()) {
            const QString &key = language_names[j];
            auto *action = new QAction(key);  // NOSONAR(cpp:S5025)
            action->setCheckable(true);
            action->setData(key);
            connect(action, &QAction::triggered, this, &MainWindow::languageMenuTriggered);
            languageActionGroup->addAction(action);
            actions.append(action);

            ++j;
        }

        if (actions.size() == 1) {
            ui->menuLanguage->addActions(actions);
        } else {
            // Create a sub menu with the actions
            auto *compactMenu = new QMenu(actions[0]->text().at(0).toUpper());  // NOSONAR(cpp:S5025)
            compactMenu->addActions(actions);
            ui->menuLanguage->addMenu(compactMenu);
        }
        i = j;
    }

    ui->menuLanguage->addSeparator();
    if (languageInspectorDock) {
        ui->menuLanguage->addAction(languageInspectorDock->toggleViewAction());
    }
}

ScintillaNext *MainWindow::currentEditor() const {
    return dockedEditor->getCurrentEditor();
}

int MainWindow::editorCount() const {
    return dockedEditor->count();
}

QVector<ScintillaNext *> MainWindow::editors() const {
    // NOTE: this will need re-evaluated in the future.
    // So far it has been assumed 1 ScintillaNext instance is 1 DockedEditor widget instance.
    // If in the future a ScintillaNext can be cloned then the DockedEditor could return
    // the same ScintillaNext instance multiple times since 1 ScintillaNext could mean >= 1 DockedEditor widget instance
    return dockedEditor->editors();
}

void MainWindow::newFile() const {
    qInfo(Q_FUNC_INFO);

    // NOTE: in theory need to check all editors in the editorManager to future proof this.
    // If there is another window it would need to check those too to see if New X exists. The editor
    // manager would encompass all editors

    int count = 1;
    forever {
        QString newFileName = tr("New %1").arg(count);
        const auto eds = editors();
        if (const bool canUseName = !std::ranges::any_of(
                eds, [&](const ScintillaNext *ed) { return !ed->isFile() && ed->getName() == newFileName; });
            canUseName) {
            ScintillaNext *editor = app->getEditorManager()->createEditor(newFileName);
            editor->grabFocus();
            break;
        }

        count++;
    }
}

// One unedited, new blank document
ScintillaNext *MainWindow::getInitialEditor() const {
    if (editorCount() == 1) {
        ScintillaNext *editor = currentEditor();

        // If the editor:
        //   is a temporary file
        //   is a 'real' file (or a 'missing' file)
        //   can undo any actions
        //   can redo any actions
        // Then do not treat it as an 'initial editor' that can be transparently closed for the user
        if (editor->isTemporary() || editor->isFile() || editor->canUndo() || editor->canRedo()) {
            return Q_NULLPTR;
        }

        return editor;
    }

    return Q_NULLPTR;
}

ScintillaNext *MainWindow::openOrDecryptFile(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    if (!fileInfo.isFile()) {
        if (ModernMessageBox::question(this, tr("Create File"),
                                       tr("<b>%1</b> does not exist. Do you want to create it?").arg(filePath))) {
            return app->getEditorManager()->createEditorFromFile(filePath, true);
        }
        app->getRecentFilesListManager()->removeFile(filePath);
        return nullptr;
    }

    if (!CryptoManager::isEncryptedFile(filePath)) {
        return app->getEditorManager()->createEditorFromFile(filePath);
    }

    DecryptDialog dlg(this, fileInfo.fileName());
    while (dlg.exec() == QDialog::Accepted) {
        QString pass = dlg.passphrase();
        QFile encFile(filePath);
        if (!encFile.open(QIODevice::ReadOnly)) {
            dlg.setErrorMessage(tr("Cannot open file: %1").arg(encFile.errorString()));
            continue;
        }
        QByteArray cipherData = encFile.readAll();
        encFile.close();

        CryptoManager::CryptoResult decRes = CryptoManager::decrypt(cipherData, pass);
        if (!decRes.success) {
            dlg.setErrorMessage(decRes.errorMessage);
            continue;
        }

        ScintillaNext *editor = app->getEditorManager()->createEditorFromFile(filePath);
        if (editor) {
            editor->clearAll();
            editor->setUndoCollection(false);
            editor->appendText(decRes.data.size(), decRes.data.constData());
            editor->setUndoCollection(true);
            editor->setSavePoint();
            editor->QObject::setProperty("IsEncrypted", true);
            editor->QObject::setProperty("EncryptionPassword", pass);
            ui->statusBar->showMessage(tr("Unlocked encrypted file \"%1\" (AES-256)").arg(fileInfo.fileName()), 4000);
        }
        return editor;
    }

    app->getRecentFilesListManager()->removeFile(filePath);
    return nullptr;
}

void MainWindow::openFileList(const QStringList &fileNames) {  // NOSONAR(cpp:S3776)
    qInfo(Q_FUNC_INFO);

    if (fileNames.isEmpty())
        return;

    QList<ScintillaNext *> openedEditors;
    ScintillaNext *initialEditor = getInitialEditor();

    for (const QString &filePath : fileNames) {
        qInfo("%s", qUtf8Printable(filePath));

        // Search currently open editors to see if it is already open
        ScintillaNext *editor = app->getEditorManager()->getEditorByFilePath(filePath);

        if (!editor) {
            editor = openOrDecryptFile(filePath);
        }

        if (editor) {
            openedEditors.append(editor);
        }
    }

    // If any were successful
    if (!openedEditors.empty()) {
        dockedEditor->switchToEditor(openedEditors.last());

        if (initialEditor) {
            initialEditor->close();
        }
    }
}

bool MainWindow::checkEditorsBeforeClose(const QVector<ScintillaNext *> &editorList) {
    QVector<ScintillaNext *> unsaved;
    for (auto *e : editorList) {
        if (!e->isSavedToDisk())
            unsaved.append(e);
    }

    if (unsaved.isEmpty())
        return true;

    // Focus the user's attention on the first unsaved file
    dockedEditor->switchToEditor(unsaved.first());

    // Single point of interaction
    UserSaveAction action = promptForSave(unsaved);
    using enum UserSaveAction;

    switch (action) {
        case DiscardAll:
            return true;
        case SaveAll:
            return saveAllEditors(unsaved);
        case Cancel:
        default:
            return false;
    }
}

void MainWindow::openFileDialog() {
    const QString filter = app->getFileDialogFilter();

    QStringList fileNames =
        FileDialogHelpers::getOpenFileNames(this, QString(), defaultDirectoryManager->getDefaultDirectory(), filter);

    if (!fileNames.empty())
        emit fileDialogAccepted(fileNames.last());

    openFileList(fileNames);
}

void MainWindow::openFile(const QString &filePath) {
    openFileList(QStringList() << filePath);
}

void MainWindow::openFolderAsWorkspaceDialog() {
    QString dir =
        QFileDialog::getExistingDirectory(this, tr("Open Folder as Workspace"),
                                          defaultDirectoryManager->getDefaultDirectory(), QFileDialog::ShowDirsOnly);

    setFolderAsWorkspacePath(dir);
}

void MainWindow::setFolderAsWorkspacePath(const QString &dir) const {
    if (!dir.isEmpty()) {
        FolderAsWorkspaceDock *fawDock = findChild<FolderAsWorkspaceDock *>();
        fawDock->setRootPath(dir);
        fawDock->setVisible(true);
    }
}

void MainWindow::reloadFile() {
    auto editor = currentEditor();

    if (!editor->isFile() && !editor->isSavedToDisk()) {
        return;
    }

    const QString filePath = editor->getFilePath();
    if (ModernMessageBox::question(
            this, tr("Reload File"),
            tr("Are you sure you want to reload <b>%1</b>? Any unsaved changes will be lost.").arg(filePath))) {
        editor->QObject::setProperty("LogTimestampApplied", false);
        editor->reload();
        if (LogAutoTimestamp::isAutoLogEnabled() && LogAutoTimestamp::checkAndApplyLogTimestamp(editor)) {
            ui->statusBar->showMessage(tr(".LOG auto-timestamp appended to log file"), 3000);
        }
    }
}

void MainWindow::closeCurrentFile() {
    closeFile(currentEditor());
}

void MainWindow::closeFile(ScintillaNext *editor) {
    if (!editor) {
        return;
    }

    // Early out. If we aren't exiting on last tab closed, and it exists, there's no point in continuing
    if (!app->getSettings()->exitOnLastTabClosed() && getInitialEditor() != Q_NULLPTR) {
        return;
    }

    if (!checkEditorsBeforeClose({editor})) {
        return;
    }

    editor->close();

    // If the last document was closed, figure out what to do next
    if (editorCount() == 0) {
        if (app->getSettings()->exitOnLastTabClosed()) {
            close();
        } else {
            newFile();
        }
    }
}

void MainWindow::closeAllFiles() {
    if (!checkEditorsBeforeClose(editors())) {
        return;
    }

    // Ask the manager to close the editors the dockedEditor knows about
    for (ScintillaNext *editor : editors()) {
        editor->close();
    }

    newFile();
}

void MainWindow::closeAllExceptActive() {
    auto e = currentEditor();
    auto editor_list = editors();

    editor_list.removeOne(e);

    if (checkEditorsBeforeClose(editor_list)) {
        for (ScintillaNext *editor : editor_list) {
            editor->close();
        }
    }
}

void MainWindow::closeAllToLeft() {
    const ads::CDockAreaWidget *dockArea = dockedEditor->currentDockArea();
    if (!dockArea) {
        return;
    }
    const int index = dockArea->currentIndex();
    QVector<ScintillaNext *> targetEditors;

    for (int i = 0; i < index; ++i) {
        if (dockArea->dockWidget(i) && dockArea->dockWidget(i)->widget()) {
            auto editor = qobject_cast<ScintillaNext *>(dockArea->dockWidget(i)->widget());
            if (editor) {
                targetEditors.append(editor);
            }
        }
    }

    if (checkEditorsBeforeClose(targetEditors)) {
        for (ScintillaNext *editor : targetEditors) {
            editor->close();
        }
    }
}

void MainWindow::closeAllToRight() {
    const ads::CDockAreaWidget *dockArea = dockedEditor->currentDockArea();
    if (!dockArea) {
        return;
    }
    const int index = dockArea->currentIndex();
    const int total = dockArea->dockWidgetsCount();
    QVector<ScintillaNext *> targetEditors;

    for (int i = index + 1; i < total; ++i) {
        if (dockArea->dockWidget(i) && dockArea->dockWidget(i)->widget()) {
            auto editor = qobject_cast<ScintillaNext *>(dockArea->dockWidget(i)->widget());
            if (editor) {
                targetEditors.append(editor);
            }
        }
    }

    if (checkEditorsBeforeClose(targetEditors)) {
        for (ScintillaNext *editor : targetEditors) {
            editor->close();
        }
    }
}

bool MainWindow::saveCurrentFile() {
    return saveFile(currentEditor());
}

bool MainWindow::saveFile(ScintillaNext *editor) {
    if (editor->isSavedToDisk())
        return true;

    if (!editor->isFile()) {
        // Switch to the editor and show the saveas dialog
        dockedEditor->switchToEditor(editor);
        return saveCurrentFileAsDialog();
    }

    if (editor->QObject::property("IsEncrypted").toBool()) {
        QString pass = editor->QObject::property("EncryptionPassword").toString();
        if (pass.isEmpty()) {
            EncryptDialog dlg(this);
            if (dlg.exec() != QDialog::Accepted) {
                return false;
            }
            pass = dlg.passphrase();
            editor->QObject::setProperty("EncryptionPassword", pass);
        }

        const QByteArray rawText = editor->getText();
        CryptoManager::CryptoResult encRes = CryptoManager::encrypt(rawText, pass);
        if (!encRes.success) {
            ModernMessageBox::critical(this, tr("Encryption Failed"),
                                       tr("Could not encrypt document: %1").arg(encRes.errorMessage));
            return false;
        }

        const QString path = editor->getFilePath();
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            showSaveErrorMessage(editor, file.error());
            return false;
        }
        if (file.write(encRes.data) == -1) {
            showSaveErrorMessage(editor, file.error());
            return false;
        }
        file.close();

        editor->setSavePoint();
        ui->statusBar->showMessage(tr("Saved \"%1\" encrypted (AES-256)").arg(editor->getName()), 4000);
        return true;
    }

    bool preserve = app->getSettings()->preserveFileTime();
    QDateTime origTime;
    if (preserve) {
        origTime = FileTimeUtils::getFileModificationTime(editor->getFilePath());
    }

    QFileDevice::FileError error = editor->save(preserve);
    if (error == QFileDevice::NoError) {
        if (preserve) {
            ui->statusBar->showMessage(
                tr("Saved \"%1\" with original modification time preserved (%2)")
                    .arg(editor->getName(), origTime.isValid() ? origTime.toString(Qt::ISODate) : tr("unchanged")),
                4000);
        }
        return true;
    } else {
        showSaveErrorMessage(editor, error);
        return false;
    }
}

bool MainWindow::saveCurrentFileWithOriginalTime() {
    ScintillaNext *editor = currentEditor();
    if (!editor)
        return false;

    if (!editor->isFile()) {
        dockedEditor->switchToEditor(editor);
        return saveCurrentFileAsDialog();
    }

    if (editor->isSavedToDisk()) {
        ui->statusBar->showMessage(tr("File \"%1\" has no unsaved changes").arg(editor->getName()), 3000);
        return true;
    }

    QDateTime origTime = FileTimeUtils::getFileModificationTime(editor->getFilePath());
    QFileDevice::FileError error = editor->save(/*preserveTimestamp=*/true);
    if (error == QFileDevice::NoError) {
        ui->statusBar->showMessage(
            tr("Saved \"%1\" with original modification time preserved (%2)")
                .arg(editor->getName(), origTime.isValid() ? origTime.toString(Qt::ISODate) : tr("unchanged")),
            4000);
        return true;
    } else {
        showSaveErrorMessage(editor, error);
        return false;
    }
}

void MainWindow::updateCurrentFileTime() {
    ScintillaNext *editor = currentEditor();
    if (!editor) {
        ui->statusBar->showMessage(tr("No active editor"), 3000);
        return;
    }

    if (!editor->isFile()) {
        ui->statusBar->showMessage(tr("Cannot update file time: document has not been saved to disk yet"), 4000);
        return;
    }

    FileTimeUtils::FileTimeResult res = FileTimeUtils::touchEditorFile(editor);
    if (res.success) {
        ui->statusBar->showMessage(
            tr("Updated file timestamp for \"%1\" to %2").arg(editor->getName(), res.newTime.toString(Qt::ISODate)),
            4000);
    } else {
        ui->statusBar->showMessage(tr("Failed to update file time: %1").arg(res.errorMessage), 5000);
    }
}

void MainWindow::togglePreserveFileTime(bool checked) {
    app->getSettings()->setPreserveFileTime(checked);
    ui->statusBar->showMessage(
        checked ? tr("Preserve File Time on Save: Enabled") : tr("Preserve File Time on Save: Disabled"), 3000);
}

void MainWindow::upperCaseSelection() {
    ScintillaNext *editor = currentEditor();
    if (!editor) {
        ui->statusBar->showMessage(tr("No active editor"), 3000);
        return;
    }
    auto res = CaseConverter::upperCase(editor);
    if (res.success) {
        ui->statusBar->showMessage(res.message, 3000);
    } else {
        ui->statusBar->showMessage(res.errorMessage, 4000);
    }
}

void MainWindow::lowerCaseSelection() {
    ScintillaNext *editor = currentEditor();
    if (!editor) {
        ui->statusBar->showMessage(tr("No active editor"), 3000);
        return;
    }
    auto res = CaseConverter::lowerCase(editor);
    if (res.success) {
        ui->statusBar->showMessage(res.message, 3000);
    } else {
        ui->statusBar->showMessage(res.errorMessage, 4000);
    }
}

void MainWindow::invertCaseSelection() {
    ScintillaNext *editor = currentEditor();
    if (!editor) {
        ui->statusBar->showMessage(tr("No active editor"), 3000);
        return;
    }
    auto res = CaseConverter::invertCase(editor);
    if (res.success) {
        ui->statusBar->showMessage(res.message, 3000);
    } else {
        ui->statusBar->showMessage(res.errorMessage, 4000);
    }
}

void MainWindow::titleCaseSelection() {
    ScintillaNext *editor = currentEditor();
    if (!editor) {
        ui->statusBar->showMessage(tr("No active editor"), 3000);
        return;
    }
    auto res = CaseConverter::titleCase(editor);
    if (res.success) {
        ui->statusBar->showMessage(res.message, 3000);
    } else {
        ui->statusBar->showMessage(res.errorMessage, 4000);
    }
}

void MainWindow::sentenceCaseSelection() {
    ScintillaNext *editor = currentEditor();
    if (!editor) {
        ui->statusBar->showMessage(tr("No active editor"), 3000);
        return;
    }
    auto res = CaseConverter::sentenceCase(editor);
    if (res.success) {
        ui->statusBar->showMessage(res.message, 3000);
    } else {
        ui->statusBar->showMessage(res.errorMessage, 4000);
    }
}

bool MainWindow::saveCurrentFileAsDialog() {
    const QString filter = app->getFileDialogFilter();
    ScintillaNext *editor = currentEditor();

    QString selectedFilter = app->getFileDialogFilterForLanguage(editor->languageName);
    QString fileName = FileDialogHelpers::getSaveFileName(
        this, QString(), defaultDirectoryManager->getDefaultDirectory(), filter, &selectedFilter);

    if (fileName.isEmpty()) {
        return false;
    }

    emit fileDialogAccepted(fileName);

    // Note: distinguish between the above case (i.e. the user cancels the dialog) and a failure
    // calling editor->saveAs() as it might fail.

    return saveFileAs(editor, fileName);
}

bool MainWindow::saveCurrentFileAs(const QString &fileName) {
    return saveFileAs(currentEditor(), fileName);
}

bool MainWindow::saveFileAs(ScintillaNext *editor, const QString &fileName) {
    qInfo("saveFileAs(%s)", qUtf8Printable(fileName));

    if (editor->QObject::property("IsEncrypted").toBool()) {
        QString pass = editor->QObject::property("EncryptionPassword").toString();
        if (pass.isEmpty()) {
            EncryptDialog dlg(this);
            if (dlg.exec() != QDialog::Accepted) {
                return false;
            }
            pass = dlg.passphrase();
            editor->QObject::setProperty("EncryptionPassword", pass);
        }

        const QByteArray rawText = editor->getText();
        CryptoManager::CryptoResult encRes = CryptoManager::encrypt(rawText, pass);
        if (!encRes.success) {
            ModernMessageBox::critical(this, tr("Encryption Failed"),
                                       tr("Could not encrypt document: %1").arg(encRes.errorMessage));
            return false;
        }

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            showSaveErrorMessage(editor, file.error());
            return false;
        }
        if (file.write(encRes.data) == -1) {
            showSaveErrorMessage(editor, file.error());
            return false;
        }
        file.close();

        editor->setFileInfo(fileName);
        editor->setSavePoint();
        ui->statusBar->showMessage(tr("Saved \"%1\" encrypted (AES-256)").arg(editor->getName()), 4000);
        return true;
    }

    QFileDevice::FileError error = editor->saveAs(fileName);

    if (error == QFileDevice::NoError) {
        return true;
    } else {
        showSaveErrorMessage(editor, error);
        return false;
    }
}

bool MainWindow::saveCopyAsDialog() {
    const QString filter = app->getFileDialogFilter();
    const QString languageName = currentEditor()->languageName;

    QString selectedFilter = app->getFileDialogFilterForLanguage(languageName);
    const QString fileName = FileDialogHelpers::getSaveFileName(
        this, tr("Save a Copy As"), defaultDirectoryManager->getDefaultDirectory(), filter, &selectedFilter);

    if (fileName.isEmpty()) {
        return false;
    }

    emit fileDialogAccepted(fileName);

    return saveCopyAs(fileName);
}

bool MainWindow::saveCopyAs(const QString &fileName) {
    auto editor = currentEditor();

    QFileDevice::FileError error = editor->saveCopyAs(fileName);

    if (error == QFileDevice::NoError) {
        return true;
    } else {
        showSaveErrorMessage(editor, error);
        return false;
    }
}

bool MainWindow::saveAll() {
    return saveAllEditors(editors());
}

bool MainWindow::saveAllEditors(const QVector<ScintillaNext *> &editorList) {
    return std::ranges::all_of(editorList, [this](ScintillaNext *editor) { return saveFile(editor); });
}

void MainWindow::exportAsFormat(Converter *converter, const QString &filter) {
    const QString fileName =
        FileDialogHelpers::getSaveFileName(this, tr("Export As"), QString(), filter + ";;All files (*)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile f(fileName);

    if (f.open(QIODevice::WriteOnly)) {
        QTextStream s(&f);
        converter->convert(s);
        f.close();
    }
}

void MainWindow::copyAsFormat(Converter *converter, const QString &mimeType) {
    const ScintillaNext *editor = currentEditor();
    if (!editor) {
        return;
    }

    QByteArray buffer;
    QTextStream stream(&buffer);

    const int startPos = editor->selectionEmpty() ? 0 : static_cast<int>(editor->selectionStart());
    const int endPos =
        editor->selectionEmpty() ? static_cast<int>(editor->length()) : static_cast<int>(editor->selectionEnd());

    converter->convertRange(stream, startPos, endPos);
    stream.flush();

    auto *mimeData = new QMimeData();  // NOSONAR(cpp:S5025)
    mimeData->setData(mimeType, buffer);

    if (mimeType.contains(QLatin1String("rtf"), Qt::CaseInsensitive) || mimeType == QLatin1String("Rich Text Format")) {
        mimeData->setData(QStringLiteral("text/rtf"), buffer);
        mimeData->setData(QStringLiteral("application/rtf"), buffer);
        mimeData->setData(QStringLiteral("public.rtf"), buffer);
        mimeData->setData(QStringLiteral("Rich Text Format"), buffer);
    } else if (mimeType == QLatin1String("text/html")) {
        mimeData->setHtml(QString::fromUtf8(buffer));
    }

    // Always provide plain text fallback
    const QString plainText = QString::fromUtf8(editor->get_text_range(startPos, endPos));
    mimeData->setText(plainText);

    QApplication::clipboard()->setMimeData(mimeData);

    const QString formatName =
        (mimeType.contains(QLatin1String("rtf"), Qt::CaseInsensitive) || mimeType == QLatin1String("Rich Text Format"))
            ? tr("RTF")
            : tr("HTML");
    ui->statusBar->showMessage(tr("Copied syntax-colored %1 to clipboard (%2)")
                                   .arg(formatName)
                                   .arg(editor->selectionEmpty() ? tr("entire document") : tr("selection")),
                               3000);
}

void MainWindow::renameFile() {
    ScintillaNext *editor = currentEditor();

    if (editor->isFile()) {
        const QString filter = app->getFileDialogFilter();
        QString selectedFilter = app->getFileDialogFilterForLanguage(editor->languageName);
        QString fileName =
            FileDialogHelpers::getSaveFileName(this, tr("Rename"), editor->getFilePath(), filter, &selectedFilter);

        if (fileName.isEmpty()) {
            return;
        }

        emit fileDialogAccepted(fileName);

        // Note: The new fileName might be to one of the existing editors.

        bool renameSuccessful = editor->rename(fileName);
        Q_UNUSED(renameSuccessful)
    } else {
        bool ok;
        QString text =
            QInputDialog::getText(this, tr("Rename"), tr("Name:"), QLineEdit::Normal, editor->getName(), &ok);

        if (ok && !text.isEmpty()) {
            editor->setName(text);
        }
    }
}

void MainWindow::moveCurrentFileToTrash() {
    const ScintillaNext *editor = currentEditor();

    moveFileToTrash(editor);
}

void MainWindow::moveFileToTrash(const ScintillaNext *editor) {
    Q_ASSERT(editor->isFile());

    const QString filePath = editor->getFilePath();
    if (ModernMessageBox::question(this, tr("Delete File"),
                                   tr("Are you sure you want to move <b>%1</b> to the trash?").arg(filePath))) {
        if (editor->moveToTrash()) {
            closeCurrentFile();

            // Since the file no longer exists, specifically remove it from the recent files list
            app->getRecentFilesListManager()->removeFile(editor->getFilePath());
        } else {
            ModernMessageBox::warning(this, tr("Error Deleting File"),
                                      tr("Something went wrong deleting <b>%1</b>?").arg(filePath));
        }
    }
}

void MainWindow::print() {
    QPrintPreviewDialog printDialog(this, Qt::Window);
    EditorPrintPreviewRenderer renderer(currentEditor());

    connect(&printDialog, &QPrintPreviewDialog::paintRequested, &renderer, &EditorPrintPreviewRenderer::render);

    // Note: load/save the page layout that was used and reload it next time

    printDialog.printer()->setPageMargins(QMarginsF(.5, .5, .5, .5), QPageLayout::Inch);

    connect(&printDialog, &QPrintPreviewDialog::accepted, this,
            [&printDialog]() { qInfo() << printDialog.printer()->pageLayout(); });

    printDialog.exec();
}

void MainWindow::convertEOLs(int eolMode) {
    ScintillaNext *editor = currentEditor();

    // Note: does convertEOLs trigger SCN_MODIFIED notifications? If so can these be turned off to increase performance?
    editor->convertEOLs(eolMode);
    editor->setEOLMode(eolMode);

    updateEOLBasedUi(editor);

    // There's no simple Scintilla notification that the EOL mode has changed
    // So tell the status bar to refresh its info
    ui->statusBar->refresh(editor);
}

void MainWindow::showFindReplaceDialog(int index) {
    ScintillaNext *editor = currentEditor();
    FindReplaceDialog *frd = findChild<FindReplaceDialog *>(QString(), Qt::FindDirectChildrenOnly);

    if (frd == Q_NULLPTR) {
        frd = new FindReplaceDialog(determineSearchResultsHandler(), this);
    } else {
        frd->setSearchResultsHandler(determineSearchResultsHandler());
    }

    // Note: if dockedEditor::editorActivated() is fired, FindReplaceDialog editor pointer should be refreshed

    // Get any selected text
    if (const auto range = editor->getContextText(); range.cpMin != range.cpMax) {
        editor->goToRange(range);
        auto text = editor->get_text_range(static_cast<int>(range.cpMin), static_cast<int>(range.cpMax));
        frd->setFindString(QString::fromUtf8(text));
    }

    frd->setTab(index);
    frd->show();
    frd->raise();
    frd->activateWindow();
}

void MainWindow::updateFileStatusBasedUi(const ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    bool isFile = editor->isFile();
    QString fileName;

    if (isFile) {
        fileName = editor->getFilePath();
    } else {
        fileName = editor->getName();
    }

    QString title = QStringLiteral("[*]%1 - SparkNotePad").arg(fileName);
    if (app->isRunningAsAdmin()) {
        title += QStringLiteral(" - [%1]").arg(tr("Administrator"));
    }
    setWindowTitle(title);
    setWindowFilePath(isFile ? editor->getFilePath() : QString());

    ui->actionReload->setEnabled(isFile);
    ui->actionMoveToTrash->setEnabled(isFile);
    ui->actionCopyFullPath->setEnabled(isFile);
    ui->actionCopyFileDirectory->setEnabled(isFile);
    ui->actionShowInExplorer->setEnabled(isFile);
    ui->actionOpenTerminalHere->setEnabled(isFile);
    ui->actionUpdateFileTime->setEnabled(isFile);
}

bool MainWindow::isAnyUnsaved() const {
    const auto eds = editors();
    return std::ranges::any_of(eds, [](const ScintillaNext *editor) { return !editor->isSavedToDisk(); });
}

void MainWindow::updateEOLBasedUi(const ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    switch (editor->eOLMode()) {
        case SC_EOL_CR:
            ui->actionMacintosh->setChecked(true);
            break;
        case SC_EOL_CRLF:
            ui->actionWindows->setChecked(true);
            break;
        case SC_EOL_LF:
            ui->actionUnix->setChecked(true);
            break;
        default:
            break;
    }
}

void MainWindow::updateSaveStatusBasedUi(const ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    bool isDirty = editor && !editor->isSavedToDisk();

    setWindowModified(isDirty);

    ui->actionSave->setEnabled(isDirty);
    ui->actionSaveWithOriginalTime->setEnabled(isDirty);
    ui->actionSaveAll->setEnabled(isDirty || isAnyUnsaved());
    ui->actionUpdateFileTime->setEnabled(editor && editor->isFile());

    if (editorCount() == 1) {
        bool ableToClose = editor && (editor->isFile() || isDirty);
        ui->actionClose->setEnabled(ableToClose);
        ui->actionCloseAll->setEnabled(ableToClose);
    } else {
        ui->actionClose->setEnabled(true);
        ui->actionCloseAll->setEnabled(true);
    }
}

void MainWindow::updateEditorPositionBasedUi() {
    const auto *dockArea = dockedEditor->currentDockArea();
    if (!dockArea) {
        ui->actionCloseAllToLeft->setEnabled(false);
        ui->actionCloseAllToRight->setEnabled(false);
        ui->actionCloseAllExceptActive->setEnabled(false);
        return;
    }

    const int index = dockArea->currentIndex();
    const int total = dockArea->dockWidgetsCount();

    ui->actionCloseAllToLeft->setEnabled(index > 0);
    ui->actionCloseAllToRight->setEnabled(index < (total - 1));
    ui->actionCloseAllExceptActive->setEnabled(editorCount() > 1);
}

void MainWindow::updateLanguageBasedUi(const ScintillaNext *editor) const {
    qInfo(Q_FUNC_INFO);

    const QString language_name = editor->languageName;
    const auto actions = languageActionGroup->actions();

    if (auto it = std::ranges::find_if(
            actions, [&](const QAction *action) { return action->data().toString() == language_name; });
        it != actions.end()) {
        (*it)->setChecked(true);
        return;
    }

    for (QAction *action : actions) {
        if (action->isChecked()) {
            action->setChecked(false);
        }
    }
}

void MainWindow::updateGui(ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    if (!editor) {
        return;
    }

    updateFileStatusBasedUi(editor);
    updateSaveStatusBasedUi(editor);
    updateEOLBasedUi(editor);
    updateEditorPositionBasedUi();
    updateSelectionBasedUi(editor);
    updateContentBasedUi(editor);
    updateLanguageBasedUi(editor);

    if (markdownSafePreviewDock && markdownSafePreviewDock->isVisible()) {
        markdownSafePreviewDock->setActiveEditor(editor);
    }
}

void MainWindow::updateDocumentBasedUi(Scintilla::Update updated) {
    using enum Scintilla::Update;
    ScintillaNext *editor = qobject_cast<ScintillaNext *>(sender());

    // Note: what if this is triggered by an editor that is not the active editor?

    if (Scintilla::FlagSet(updated, Text)) {
        updateSelectionBasedUi(editor);
    }

    if (Scintilla::FlagSet(updated, Text) || Scintilla::FlagSet(updated, Selection)) {
        updateContentBasedUi(editor);
    }
}

void MainWindow::updateSelectionBasedUi(ScintillaNext *editor) {
    ui->actionUndo->setEnabled(editor->canUndo());
    ui->actionRedo->setEnabled(editor->canRedo());
}

void MainWindow::updateContentBasedUi(ScintillaNext *editor) {
    bool hasAnySelections = !editor->selectionEmpty();

    ui->actionPaste->setEnabled(editor->canPaste());

    const bool canConvert = (editor != nullptr && !editor->readOnly() && editor->textLength() > 0);
    ui->actionLowerCase->setEnabled(canConvert);
    ui->actionUpperCase->setEnabled(canConvert);
    ui->actionInvertCase->setEnabled(canConvert);
    ui->actionTitleCase->setEnabled(canConvert);
    ui->actionSentenceCase->setEnabled(canConvert);

    ui->actionBase64Encode->setEnabled(editor != nullptr);
    ui->actionBase64Decode->setEnabled(editor != nullptr);
    ui->actionConvertToHex->setEnabled(editor != nullptr);
    ui->actionConvertToDec->setEnabled(editor != nullptr);
    ui->actionConvertToBin->setEnabled(editor != nullptr);
    ui->actionConvertToOct->setEnabled(editor != nullptr);

    const bool canInsert = (editor != nullptr && !editor->readOnly());
    ui->actionInsertDateTimeShort->setEnabled(canInsert);
    ui->actionInsertDateTimeLong->setEnabled(canInsert);
    ui->actionInsertTimestampIso->setEnabled(canInsert);
    ui->actionInsertTimestampUnix->setEnabled(canInsert);
    ui->actionInsertDateOnly->setEnabled(canInsert);
    ui->actionInsertTimeOnly->setEnabled(canInsert);
    ui->actionInsertGuidHyphens->setEnabled(canInsert);
    ui->actionInsertGuidBraces->setEnabled(canInsert);
    ui->actionInsertGuidCompact->setEnabled(canInsert);
    ui->actionColorPreview->setEnabled(canInsert);
    ui->actionFormatAuto->setEnabled(canInsert);
    ui->actionMinifyAuto->setEnabled(canInsert);
    ui->actionFormatJson->setEnabled(canInsert);
    ui->actionMinifyJson->setEnabled(canInsert);
    ui->actionFormatJson5->setEnabled(canInsert);
    ui->actionMinifyJson5->setEnabled(canInsert);
    ui->actionFormatCss->setEnabled(canInsert);
    ui->actionMinifyCss->setEnabled(canInsert);
    ui->actionFormatJs->setEnabled(canInsert);
    ui->actionMinifyJs->setEnabled(canInsert);
    ui->actionEvaluateMathAppend->setEnabled(canInsert);
    ui->actionEvaluateMathReplace->setEnabled(canInsert);
    ui->actionCopyAsRtf->setEnabled(editor != nullptr);
    ui->actionCopyAsHtml->setEnabled(editor != nullptr);
    ui->actionURLEncode->setEnabled(hasAnySelections);
    ui->actionURLDecode->setEnabled(hasAnySelections);

    const bool hasEditor = (editor != nullptr);
    const bool isFocused = hasEditor && focusedView && focusedView->isFocused(editor);
    ui->actionToggleFocusedView->setEnabled(hasEditor);
    ui->actionToggleFocusedView->setChecked(isFocused);
    ui->actionFocusedViewFilterDialog->setEnabled(hasEditor);
    ui->actionFocusedViewBookmarks->setEnabled(hasEditor);
    ui->actionFocusedViewClear->setEnabled(isFocused);
    ui->actionCustomizeStatusBarFields->setEnabled(true);
    ui->actionInsertLogHeader->setEnabled(canInsert);
    ui->actionAppendLogTimestamp->setEnabled(canInsert);
    ui->actionToggleAutoLogTimestamp->setEnabled(true);
}

void MainWindow::detectLanguage(ScintillaNext *editor) const {
    qInfo(Q_FUNC_INFO);

    if (!editor->isFile()) {
        // Default to some specific language if it is not a file.
        setLanguage(editor, "Text");
        return;
    }

    const QString language_name = app->detectLanguage(editor);
    setLanguage(editor, language_name);
}

void MainWindow::activateEditor(ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    checkFileForModification(editor);
    if (editor) {
        auto curIdx = static_cast<int>(editors().indexOf(editor));
        int total = editorCount();
        AccessibilityManager::instance()->announceDocumentState(editor, curIdx, total);
    }

    updateGui(editor);
    updateTailMonitoringUi(editor);

    if (editor && ClipboardMonitor::instance()->isMonitoring()) {
        ClipboardMonitor::instance()->setTargetEditor(editor);
    }

    if (editor && LogAutoTimestamp::isAutoLogEnabled() && !editor->QObject::property("LogTimestampApplied").toBool() &&
        LogAutoTimestamp::checkAndApplyLogTimestamp(editor)) {
        ui->statusBar->showMessage(tr(".LOG auto-timestamp appended to log file"), 3000);
    }

    emit editorActivated(editor);
}

void MainWindow::applyStyleSheet() {
    qInfo(Q_FUNC_INFO);

    QString sheet = ThemeManager::instance()->getThemeStyleSheet();

    // If there is a "custom.css" file where the ini is located, load it as a style sheet addition
    const QString directoryPath = QFileInfo(app->getSettings()->fileName()).absolutePath();
    if (const QString fullPath = QDir(directoryPath).filePath("custom.css"); QFile::exists(fullPath)) {
        QFile custom(fullPath);
        qInfo() << "Loading stylesheet:" << custom.fileName();

        if (custom.open(QFile::ReadOnly)) {
            sheet += custom.readAll();
            custom.close();
        }
    }

    setStyleSheet(sheet);
    if (qApp) {
        qApp->setStyleSheet(sheet);
    }
}

void MainWindow::setLanguage(ScintillaNext *editor, const QString &languageName) const {
    qInfo(Q_FUNC_INFO);
    qInfo("Language Name: %s", qUtf8Printable(languageName));

    app->setEditorLanguage(editor, languageName);
}

void MainWindow::bringWindowToForeground() {
    qInfo(Q_FUNC_INFO);

    // There doesn't seem to be a cross platform way to force the window to the foreground

#ifdef Q_OS_WIN
    HWND hWnd = reinterpret_cast<HWND>(effectiveWinId());

    if (hWnd) {
        // I have no idea what this does, but it seems to work on Windows
        // References:
        // https://stackoverflow.com/questions/916259/win32-bring-a-window-to-top
        // https://github.com/notepad-plus-plus/notepad-plus-plus/blob/ebe7648ee1a5a560d4fc65297cbdcf08055e56e3/PowerEditor/src/winmain.cpp#L596

        HWND hCurWnd = GetForegroundWindow();
        DWORD threadId = GetCurrentThreadId();
        DWORD procId = GetWindowThreadProcessId(hCurWnd, NULL);

        int sw = 0;
        if (IsZoomed(hWnd)) {
            sw = SW_MAXIMIZE;
        } else if (IsIconic(hWnd)) {
            sw = SW_RESTORE;
        }

        if (sw != 0) {
            ShowWindow(hWnd, sw);
        }

        AttachThreadInput(procId, threadId, TRUE);
        SetForegroundWindow(hWnd);
        SetFocus(hWnd);
        AttachThreadInput(procId, threadId, FALSE);
    }
#elif defined(Q_OS_MACOS)
    MacServicesHandler::activateApplication();
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    raise();
    activateWindow();
#else
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    raise();
    activateWindow();
#endif
}

bool MainWindow::checkFileForModification(ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    if (!editor || TailMonitoringManager::instance()->isMonitoring(editor)) {
        return false;
    }

    if (const auto state = editor->checkFileForStateChange(); state == ScintillaNext::FileStateChange::NoChange) {
        return false;
    } else if (state == ScintillaNext::FileStateChange::Modified) {
        qInfo("ScintillaNext::Modified");
        const QString filePath = editor->getFilePath();
        if (ModernMessageBox::question(
                this, tr("Reload File"),
                tr("<b>%1</b> has been modified by another program. Do you want to reload it?").arg(filePath))) {
            editor->reload();
        } else {
            editor->omitModifications();
        }
    } else if (state == ScintillaNext::FileStateChange::Deleted) {
        qInfo("ScintillaNext::Deleted");
    } else if (state == ScintillaNext::FileStateChange::Restored) {
        qInfo("ScintillaNext::Restored");
    }

    return true;
}

void MainWindow::showSaveErrorMessage(const ScintillaNext *editor, QFileDevice::FileError error) {
    const QString name = editor->isFile() ? editor->getFilePath() : editor->getName();

    // Map error code to human-readable string
    QString errorString;
    switch (error) {
        case QFileDevice::ReadError:
            errorString = tr("Read error");
            break;
        case QFileDevice::WriteError:
            errorString = tr("Write error");
            break;
        case QFileDevice::FatalError:
            errorString = tr("Fatal error");
            break;
        case QFileDevice::ResourceError:
            errorString = tr("Resource error");
            break;
        case QFileDevice::OpenError:
            errorString = tr("Open error");
            break;
        case QFileDevice::AbortError:
            errorString = tr("Abort error");
            break;
        case QFileDevice::TimeOutError:
            errorString = tr("Timeout error");
            break;
        case QFileDevice::UnspecifiedError:
            errorString = tr("Unspecified error");
            break;
        case QFileDevice::RemoveError:
            errorString = tr("Remove error");
            break;
        case QFileDevice::RenameError:
            errorString = tr("Rename error");
            break;
        case QFileDevice::PositionError:
            errorString = tr("Position error");
            break;
        case QFileDevice::ResizeError:
            errorString = tr("Resize error");
            break;
        case QFileDevice::PermissionsError:
            errorString = tr("Permissions error");
            break;
        case QFileDevice::CopyError:
            errorString = tr("Copy error");
            break;
        default:
            errorString = tr("Unknown error (%1)").arg(static_cast<int>(error));
            break;
    }

    ModernMessageBox::warning(this, tr("Error Saving File"),
                              tr("An error occurred when saving <b>%1</b><br><br>Error: %2").arg(name, errorString));
}

void MainWindow::showEditorZoomLevelIndicator() const {
    const QString msg = tr("Zoom: %1%").arg(zoomLevel * 10 + 100);
    if (statusBar()) {
        statusBar()->showMessage(msg, 2000);
    }
}

MainWindow::UserSaveAction MainWindow::promptForSave(const QVector<ScintillaNext *> &editorList) {
    using enum UserSaveAction;
    ModernMessageBox::ActionChoice choice = ModernMessageBox::promptForSave(this, editorList);
    if (choice == ModernMessageBox::ActionChoice::SaveOrAccept)
        return SaveAll;
    if (choice == ModernMessageBox::ActionChoice::Discard)
        return DiscardAll;
    return Cancel;
}

void MainWindow::newWindow() const {
#if defined(Q_OS_MACOS)
    if (QDir dir(QCoreApplication::applicationFilePath()); dir.cdUp() && dir.cdUp() && dir.cdUp()) {
        const QString bundlePath = dir.absolutePath();
        if (bundlePath.endsWith(QStringLiteral(".app"), Qt::CaseInsensitive)) {
            QProcess::startDetached(QStringLiteral("/usr/bin/open"),
                                    QStringList() << QStringLiteral("-n") << bundlePath << QStringLiteral("--args")
                                                  << QStringLiteral("--multi-instance"));
            return;
        }
    }
#endif
    QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList()
                                                                         << QStringLiteral("--multi-instance"));
}

void MainWindow::exportPdf() {
    ScintillaNext *editor = currentEditor();
    if (!editor)
        return;

    QString defaultName = editor->getName();
    if (defaultName.isEmpty())
        defaultName = tr("Untitled");
    if (!defaultName.endsWith(QStringLiteral(".pdf"), Qt::CaseInsensitive)) {
        if (const auto dot = static_cast<int>(defaultName.lastIndexOf('.')); dot > 0)
            defaultName = defaultName.left(dot);
        defaultName += QStringLiteral(".pdf");
    }

    QString savePath = QFileDialog::getSaveFileName(this, tr("Export to PDF"), defaultName,
                                                    tr("PDF Documents (*.pdf);;All Files (*)"));

    if (savePath.isEmpty())
        return;

    QPdfWriter pdfWriter(savePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    pdfWriter.setTitle(editor->getName());
    pdfWriter.setCreator(QStringLiteral("SparkNotePad v%1").arg(APP_VERSION));

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        ModernMessageBox::warning(this, tr("Export Failed"),
                                  tr("Could not initialize PDF renderer for:\n%1").arg(savePath));
        return;
    }

    QFont docFont(QStringLiteral("JetBrains Mono"), 10);
#if defined(Q_OS_MACOS)
    docFont.setFamilies({QStringLiteral("-apple-system"), QStringLiteral("SF Mono"), QStringLiteral("Menlo"),
                         QStringLiteral("JetBrains Mono"), QStringLiteral("Courier New")});
#elif defined(Q_OS_LINUX)
    docFont.setFamilies({QStringLiteral("Inter"), QStringLiteral("DejaVu Sans Mono"), QStringLiteral("Liberation Mono"),
                         QStringLiteral("JetBrains Mono"), QStringLiteral("Monospace")});
#endif
    painter.setFont(docFont);
    QFontMetrics fm(docFont);

    int lineHeight = fm.lineSpacing();
    QRect pageRect = pdfWriter.pageLayout().paintRectPixels(pdfWriter.resolution());

    QFont headerFont = painter.font();
    headerFont.setBold(true);
    headerFont.setPointSize(11);
    painter.setFont(headerFont);
    painter.setPen(QColor(100, 100, 100));
    painter.drawText(pageRect.left(), pageRect.top() + 20, tr("%1 — SparkNotePad").arg(editor->getName()));
    painter.drawLine(pageRect.left(), pageRect.top() + 28, pageRect.right(), pageRect.top() + 28);

    painter.setFont(docFont);
    painter.setPen(Qt::black);

    int startY = pageRect.top() + 45;
    int yPos = startY;
    int pageNum = 1;

    auto totalLines = static_cast<int>(editor->linesOnScreen() > 0 ? editor->lineCount() : 1);
    for (int i = 0; i < totalLines; ++i) {
        if (yPos + lineHeight > pageRect.bottom() - 30) {
            painter.setPen(QColor(128, 128, 128));
            painter.drawText(pageRect.right() - 80, pageRect.bottom() - 10, tr("Page %1").arg(pageNum));

            pdfWriter.newPage();
            pageNum++;

            painter.setFont(headerFont);
            painter.setPen(QColor(100, 100, 100));
            painter.drawText(pageRect.left(), pageRect.top() + 20, tr("%1 — SparkNotePad").arg(editor->getName()));
            painter.drawLine(pageRect.left(), pageRect.top() + 28, pageRect.right(), pageRect.top() + 28);

            painter.setFont(docFont);
            painter.setPen(Qt::black);
            yPos = startY;
        }

        QString lineText = QString::fromUtf8(editor->getLine(i));
        lineText.remove('\r');
        lineText.remove('\n');

        QString lineNumStr = QString::number(i + 1).rightJustified(4, ' ') + "  ";
        painter.setPen(QColor(160, 160, 160));
        painter.drawText(pageRect.left(), yPos, lineNumStr);

        int lineNumWidth = fm.horizontalAdvance(lineNumStr);
        painter.setPen(Qt::black);
        painter.drawText(pageRect.left() + lineNumWidth, yPos, lineText);

        yPos += lineHeight;
    }

    painter.setPen(QColor(128, 128, 128));
    painter.drawText(pageRect.right() - 80, pageRect.bottom() - 10, tr("Page %1").arg(pageNum));
    painter.end();

    FadingIndicator::showText(editor, tr("Exported to PDF: %1").arg(QFileInfo(savePath).fileName()));
}

void MainWindow::saveSettings() const {
    qInfo(Q_FUNC_INFO);

    ApplicationSettings *settings = app->getSettings();

    settings->setValue("MainWindow/geometry", saveGeometry());
    settings->setValue("MainWindow/windowState", saveState());
    settings->setValue("MainWindow/isFullScreen", isFullScreen());

    settings->setValue("Editor/ZoomLevel", zoomLevel);
}

void MainWindow::restoreSettings() {
    qInfo(Q_FUNC_INFO);

    const ApplicationSettings *settings = app->getSettings();

    zoomLevel = settings->value("Editor/ZoomLevel", 0).toInt();

    if (settings->contains("Gui/ToolBar")) {
        QStringList actionNames;
        actionNames = settings->value("Gui/ToolBar").toStringList();

        ui->mainToolBar->clear();

        ActionUtils::populateActionContainer(ui->mainToolBar, this, actionNames);
    }
}

ISearchResultsHandler *MainWindow::determineSearchResultsHandler() {
    // Determine what will get the search results
    if (app->getSettings()->combineSearchResults()) {
        searchResults.reset(new SearchResultsCollector(findChild<SearchResultsDock *>()));

        return searchResults.data();
    } else {
        return findChild<SearchResultsDock *>();
    }
}

void MainWindow::restoreWindowState() {
    const ApplicationSettings *settings = app->getSettings();

    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());
    ScreenDisplayManager::instance().ensureWindowOnScreen(this);

    if (settings->value("MainWindow/isFullScreen", false).toBool()) {
        ui->actionFullScreen->setChecked(true);
        ui->actionFullScreen->trigger();
    }

    // Always hide the dock no matter how the application was closed
    SearchResultsDock *srDock = findChild<SearchResultsDock *>();
    srDock->hide();
}

void MainWindow::switchToEditor(const ScintillaNext *editor) {
    dockedEditor->switchToEditor(editor);
}

void MainWindow::focusIn() {
    qInfo(Q_FUNC_INFO);

    if (const ScintillaNext *editor = currentEditor(); editor && checkFileForModification(currentEditor())) {
        updateGui(currentEditor());
    }
}

void MainWindow::addEditor(ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    detectLanguage(editor);

    // These should only ever occur for the focused editor??
    // Note: look at editor inspector as an example to ensure updates are only coming from one editor.
    // Can save the connection objects and disconnected from them and only connect to the editor as it is activated.
    connect(editor, &ScintillaNext::savePointChanged, this, [this, editor]() { updateSaveStatusBasedUi(editor); });
    connect(editor, &ScintillaNext::renamed, this, [this, editor]() { detectLanguage(editor); });
    connect(editor, &ScintillaNext::renamed, this, [this, editor]() { updateFileStatusBasedUi(editor); });
    connect(editor, &ScintillaNext::updateUi, this, &MainWindow::updateDocumentBasedUi);

    // Scintilla pastes the primary selection on middle-click. Suppress an
    // accidental wheel-button press while Shift is held for horizontal scrolling.
    editor->viewport()->installEventFilter(shiftMiddleClickBlocker);

    // Translate shift+vertical wheel to vertical
    editor->viewport()->installEventFilter(shiftWheelToHorizontalScrollFilter);

    // Watch for any zoom events (Ctrl+Scroll or pinch-to-zoom (Qt translates it as Ctrl+Scroll)) so that the event
    // can be handled before the ScintillaEditBase widget, so that it can be applied to all editors to keep zoom level
    // equal. NOTE: Need to install this on the scroll area's viewport, not on the editor widget itself...that was
    // painful to learn
    editor->viewport()->installEventFilter(zoomEventWatcher);

    editor->setZoom(zoomLevel);

    editor->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(editor, &ScintillaNext::customContextMenuRequested, this,
            [this, editor](const QPoint &pos) { showEditorContextMenu(editor, pos); });

    // The editor has been entirely configured at this point, so add it to the docked editor
    dockedEditor->addEditor(editor);
}

void MainWindow::showEditorContextMenu(ScintillaNext *editor, const QPoint &pos) {
    contextMenuPos = static_cast<int>(editor->positionFromPoint(pos.x(), pos.y()));

    // If the click landed outside the current selection, move the caret there
    // and clear the selection so caret-based actions use the clicked location.
    if (editor->selectionFromPoint(pos.x(), pos.y()) == -1) {
        editor->setEmptySelection(contextMenuPos);
    }

    const QStringList defaultActionNames = {"Undo",
                                            "Redo",
                                            "",
                                            "Cut",
                                            "Copy",
                                            "Paste",
                                            "Delete",
                                            "",
                                            "SelectAll",
                                            "",
                                            "Find",
                                            "",
                                            "Open",
                                            "Save",
                                            "",
                                            "ShowCharacterInfo",
                                            "",
                                            "Base64Encode",
                                            "URLEncode",
                                            "",
                                            "Base64Decode",
                                            "URLDecode"};

    QStringList actionNames = defaultActionNames;

    // If the entry exists in the settings, use that
    if (const auto *settings = app->getSettings(); settings->contains("Gui/EditorContextMenu")) {
        actionNames = settings->value("Gui/EditorContextMenu").toStringList();
        // Upgrade legacy context menu settings so Undo, Redo, Open, Save, and Find are always included
        if (!actionNames.contains("Undo") || !actionNames.contains("Redo") || !actionNames.contains("Open") ||
            !actionNames.contains("Save") || !actionNames.contains("Find")) {
            actionNames = defaultActionNames;
        }
    }

    // If the cursor is at a URL, prepend the actions
    if (const auto *urlFinder = editor->findChild<URLFinder *>(QString(), Qt::FindDirectChildrenOnly);
        urlFinder && urlFinder->isEnabled() && urlFinder->isURL(contextMenuPos)) {
        actionNames.prepend("");
        actionNames.prepend("CopyURL");
        actionNames.prepend("OpenURL");
    }

    auto menu = buildMenu(actionNames);
    menu->addSeparator();
    menu->addMenu(ui->menuConvertCase);
    menu->addMenu(ui->menuMarkAllOccurrences);
    menu->addMenu(ui->menuClearMarks);
    if (menuRun) {
        menu->addMenu(menuRun);
    }

    // Ensure all context menu actions and submenus display their icons
    for (QAction *action : menu->findChildren<QAction *>()) {
        action->setIconVisibleInMenu(true);
    }
    for (QAction *action : menu->actions()) {
        action->setIconVisibleInMenu(true);
    }

    menu->popup(QCursor::pos());
}

void MainWindow::checkForUpdates(bool silent) {
    ReleaseManager::instance().checkForUpdates(this, silent);
    if (app && app->getSettings()) {
        app->getSettings()->setValue("App/LastUpdateCheck", QDateTime::currentDateTime());
    }
}

void MainWindow::checkForUpdatesFinished(const QString & /*url*/) {
    // Intentionally empty: update notification is handled by ReleaseManager dialog.
}

void MainWindow::initUpdateCheck() {
    ui->actionCheckForUpdates->setEnabled(true);
    ui->actionCheckForUpdates->setVisible(true);
    ui->actionCheckForUpdates->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_U));
    connect(ui->actionCheckForUpdates, &QAction::triggered, this, [this]() { checkForUpdates(false); });

    // Check periodically in background
    QTimer::singleShot(15000, this, [this]() {
        ApplicationSettings settings;
        QDateTime dt = settings.value("App/LastUpdateCheck", QDateTime::currentDateTime()).toDateTime();

        if (dt.isValid()) {
            qInfo("Last checked for updates at: %s", qUtf8Printable(dt.toString()));

            if (dt.addDays(7) < QDateTime::currentDateTime()) {
                checkForUpdates(true);
            }
        }
    });
}

void MainWindow::ensureMenuIconsVisible(QWidget *root) {
    QWidget *target = root ? root : this;
    for (auto *action : target->findChildren<QAction *>()) {
        if (!action->icon().isNull()) {
            action->setIconVisibleInMenu(true);
        }
    }
    if (const auto *menu = qobject_cast<const QMenu *>(target)) {
        for (auto *action : menu->actions()) {
            if (!action->icon().isNull()) {
                action->setIconVisibleInMenu(true);
            }
        }
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    ensureMenuIconsVisible(this);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    bringWindowToForeground();

    // Prompt user for unsaved changes or new documents across all open tabs
    if (!checkEditorsBeforeClose(editors())) {
        event->ignore();
        return;
    }

    emit aboutToClose();

    event->accept();

    QMainWindow::closeEvent(event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    qInfo(Q_FUNC_INFO);

    // NOTE: for urls these can be dragged anywhere in the application, editor, tabs, menu
    // because the ScintillaNext editor ignores urls so they can be handled by the main
    // application
    // Text dragging within the editor object itself is handled by Scintilla, but if the text
    // is dragged to other parts (tabs, menu, etc) it will be handled by the application to
    // create a new editor from the text.

    // Accept urls and text
    if (event->mimeData()->hasUrls() || event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    qInfo(Q_FUNC_INFO);

    if (event->mimeData()->hasUrls()) {
        // Get the urls into a stringlist
        QStringList fileNames;
        for (const QUrl &url : event->mimeData()->urls()) {
            if (!url.isLocalFile()) {
                continue;
            }
            QFileInfo info(url.toLocalFile());
            if (!info.exists()) {
                continue;
            }
            if (!info.isDir()) {
                fileNames.append(url.toLocalFile());
                continue;
            }
            QDirIterator it(url.toLocalFile(), QDir::Files,
                            QDirIterator::FollowSymlinks | QDirIterator::Subdirectories);
            while (it.hasNext()) {
                fileNames << it.next();
            }
        }

        openFileList(fileNames);
        bringWindowToForeground();
        event->acceptProposedAction();
    } else if (event->mimeData()->hasText()) {
        if (event->source()) {
            // if it is from an editor, remove the text
            ScintillaNext *sn = qobject_cast<ScintillaNext *>(event->source());
            if (sn) {
                sn->replaceSel("");
            }
        }

        newFile();
        currentEditor()->setText(event->mimeData()->text().toLocal8Bit().constData());
        bringWindowToForeground();
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        const bool fs = isFullScreen();
        if (ui->actionFullScreen->isChecked() != fs) {
            ui->actionFullScreen->setChecked(fs);
        }
        if (!fs) {
            ui->menuBar->setMaximumHeight(QWIDGETSIZE_MAX);
            ui->mainToolBar->setMaximumHeight(QWIDGETSIZE_MAX);
            ui->pushExitFullScreen->hide();
        } else {
            ui->pushExitFullScreen->setGeometry(width() - 20, 0, 20, 20);
            ui->pushExitFullScreen->show();
            ui->pushExitFullScreen->raise();
        }
    }
    QMainWindow::changeEvent(event);
}

QMenu *MainWindow::buildMenu(const QStringList &actionNames) {
    auto *menu = new QMenu(this);  // NOSONAR(cpp:S5025)
    menu->setAttribute(Qt::WA_DeleteOnClose);

    ActionUtils::populateActionContainer(menu, this, actionNames);
    ensureMenuIconsVisible(menu);

    return menu;
}

void MainWindow::tabBarRightClicked(const ScintillaNext *editor) {
    qInfo(Q_FUNC_INFO);

    // Focus on the correct tab
    dockedEditor->switchToEditor(editor);

    // Default actions
    QStringList actionNames{"Close",
                            "CloseAllExceptActive",
                            "CloseAllToLeft",
                            "CloseAllToRight",
                            "",
                            "Save",
                            "SaveAs",
                            "Rename",
                            "",
                            "Reload",
                            "",
                            "ShowInExplorer",
                            "OpenTerminalHere",
                            "",
                            "CopyFullPath",
                            "CopyFileName",
                            "CopyFileDirectory"};

    // If the entry exists in the settings, use that
    if (const auto *settings = app->getSettings(); settings->contains("Gui/TabBarContextMenu")) {
        actionNames = settings->value("Gui/TabBarContextMenu").toStringList();
    }

    buildMenu(actionNames)->popup(QCursor::pos());
}

void MainWindow::languageMenuTriggered() const {
    const QAction *act = qobject_cast<QAction *>(sender());
    auto editor = currentEditor();
    QVariant v = act->data();

    setLanguage(editor, v.toString());
}

void MainWindow::setupRunMenu() {
    if (menuRun)
        return;

    menuRun = new QMenu(tr("&Run"), this);
    ui->menuBar->insertMenu(ui->menuHelp->menuAction(), menuRun);

    // 1. Run... (Ctrl+Alt+R, F6)
    auto *actionRun = new QAction(tr("&Run..."), this);
    actionRun->setIcon(QIcon(QStringLiteral(":/icons/playRecord.svg")));
    actionRun->setShortcuts({QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_R), QKeySequence(Qt::Key_F6)});
    connect(actionRun, &QAction::triggered, this, &MainWindow::onRunTriggered);
    menuRun->addAction(actionRun);

    menuRun->addSeparator();

    // 2. Run Current File in Terminal (Ctrl+Alt+Return)
    auto *actionRunCurrentFile = new QAction(tr("Run Current &File in Terminal"), this);
    actionRunCurrentFile->setIcon(QIcon(QStringLiteral(":/icons/terminal.svg")));
    actionRunCurrentFile->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Return));
    connect(actionRunCurrentFile, &QAction::triggered, this, &MainWindow::onRunCurrentFileTriggered);
    menuRun->addAction(actionRunCurrentFile);

    // 3. Open Terminal Here (Ctrl+Alt+P)
    auto *actionTerminalHere = new QAction(tr("Open &Terminal Here"), this);
    actionTerminalHere->setIcon(QIcon(QStringLiteral(":/icons/terminal.svg")));
    actionTerminalHere->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_P));
    connect(actionTerminalHere, &QAction::triggered, this, &MainWindow::onOpenTerminalHereTriggered);
    menuRun->addAction(actionTerminalHere);

    // 4. Reveal in File Manager
    auto *actionReveal = new QAction(tr("Reveal in &File Manager"), this);
    actionReveal->setIcon(QIcon(QStringLiteral(":/icons/folder.svg")));
    connect(actionReveal, &QAction::triggered, this, &MainWindow::onRevealInFileManagerTriggered);
    menuRun->addAction(actionReveal);

    // 5. Open in Default Browser
    auto *actionBrowser = new QAction(tr("Open in Default &Web Browser"), this);
    actionBrowser->setIcon(QIcon(QStringLiteral(":/icons/folder.svg")));
    connect(actionBrowser, &QAction::triggered, this, &MainWindow::onOpenInBrowserTriggered);
    menuRun->addAction(actionBrowser);

    // 6. Generate Development SSL Certificate (mkcert / OpenSSL)
    auto *actionGenerateCert = new QAction(tr("Generate Development SSL &Certificate (mkcert/OpenSSL)..."), this);
    actionGenerateCert->setIcon(QIcon(QStringLiteral(":/icons/SparkNotePad.svg")));
    actionGenerateCert->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C));
    connect(actionGenerateCert, &QAction::triggered, this, &MainWindow::onGenerateCertTriggered);
    menuRun->addAction(actionGenerateCert);

    menuRun->addSeparator();

    // 6. Web Searches
    auto *actionGoogle = new QAction(tr("Search on &Google"), this);
    actionGoogle->setIcon(QIcon(QStringLiteral(":/icons/find.svg")));
    connect(actionGoogle, &QAction::triggered, this, &MainWindow::onSearchGoogleTriggered);
    menuRun->addAction(actionGoogle);

    auto *actionWiki = new QAction(tr("Search on &Wikipedia"), this);
    actionWiki->setIcon(QIcon(QStringLiteral(":/icons/find.svg")));
    actionWiki->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_W));
    connect(actionWiki, &QAction::triggered, this, &MainWindow::onSearchWikipediaTriggered);
    menuRun->addAction(actionWiki);

    auto *actionGitHub = new QAction(tr("Search on &GitHub"), this);
    actionGitHub->setIcon(QIcon(QStringLiteral(":/icons/find.svg")));
    connect(actionGitHub, &QAction::triggered, this, &MainWindow::onSearchGitHubTriggered);
    menuRun->addAction(actionGitHub);

    auto *actionSO = new QAction(tr("Search on &Stack Overflow"), this);
    actionSO->setIcon(QIcon(QStringLiteral(":/icons/find.svg")));
    connect(actionSO, &QAction::triggered, this, &MainWindow::onSearchStackOverflowTriggered);
    menuRun->addAction(actionSO);

    menuRun->addSeparator();

    // 7. Manage Tools
    auto *actionManage = new QAction(tr("&Manage External Tools..."), this);
    actionManage->setIcon(QIcon(QStringLiteral(":/icons/settings.svg")));
    connect(actionManage, &QAction::triggered, this, &MainWindow::onManageToolsTriggered);
    menuRun->addAction(actionManage);

    // 8. Saved Tools Submenu
    menuSavedTools = menuRun->addMenu(tr("Saved &Tools"));
    updateCustomToolsMenu();
    ensureMenuIconsVisible(menuRun);
}

void MainWindow::updateCustomToolsMenu() {
    if (!menuSavedTools)
        return;

    menuSavedTools->clear();
    const auto tools = ExternalToolManager::loadCustomTools();
    if (tools.isEmpty()) {
        QAction *emptyAct = menuSavedTools->addAction(tr("(No saved tools)"));
        emptyAct->setEnabled(false);
        return;
    }

    for (const auto &tool : tools) {
        QAction *act = menuSavedTools->addAction(tool.name);
        if (!tool.shortcut.isEmpty()) {
            act->setShortcut(QKeySequence(tool.shortcut));
        }
        connect(act, &QAction::triggered, this, [this, tool]() {
            QString err;
            if (!ExternalToolManager::executeTool(tool, currentEditor(), &err) && !err.isEmpty()) {
                ModernMessageBox::warning(this, tr("Tool Execution Error"), err);
            }
        });
    }
}

void MainWindow::onRunTriggered() {
    RunDialog dlg(this, currentEditor());
    if (dlg.exec() == QDialog::Accepted) {
        updateCustomToolsMenu();
    }
}

void MainWindow::onRunCurrentFileTriggered() {
    QString err;
    if (!ExternalToolManager::runCurrentFileWithInterpreter(currentEditor(), &err) && !err.isEmpty()) {
        ModernMessageBox::warning(this, tr("Run Current File"), err);
    }
}

void MainWindow::onOpenTerminalHereTriggered() const {
    QString dir;
    if (currentEditor() && currentEditor()->isFile()) {
        dir = currentEditor()->getFileInfo().dir().absolutePath();
    }
    ExternalToolManager::openTerminal(dir);
}

void MainWindow::onRevealInFileManagerTriggered() const {
    if (currentEditor() && currentEditor()->isFile()) {
        ExternalToolManager::revealInFileManager(currentEditor()->getFileInfo().absoluteFilePath());
    }
}

void MainWindow::onOpenInBrowserTriggered() const {
    if (currentEditor() && currentEditor()->isFile()) {
        ExternalToolManager::openInBrowser(currentEditor()->getFileInfo().absoluteFilePath());
    }
}

void MainWindow::onSearchGoogleTriggered() const {
    ExternalToolManager::webSearch(ExternalToolManager::getSelectedOrCurrentWord(currentEditor()),
                                   QStringLiteral("google"));
}

void MainWindow::onSearchWikipediaTriggered() const {
    ExternalToolManager::webSearch(ExternalToolManager::getSelectedOrCurrentWord(currentEditor()),
                                   QStringLiteral("wikipedia"));
}

void MainWindow::onSearchGitHubTriggered() const {
    ExternalToolManager::webSearch(ExternalToolManager::getSelectedOrCurrentWord(currentEditor()),
                                   QStringLiteral("github"));
}

void MainWindow::onSearchStackOverflowTriggered() const {
    ExternalToolManager::webSearch(ExternalToolManager::getSelectedOrCurrentWord(currentEditor()),
                                   QStringLiteral("stackoverflow"));
}

void MainWindow::onManageToolsTriggered() {
    ManageToolsDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        updateCustomToolsMenu();
    }
}

void MainWindow::onGenerateCertTriggered() {
    MkcertDialog dlg(this);
    dlg.exec();
}

void MainWindow::onRegexHelperTriggered() {
    QString pattern;
    if (auto *ed = currentEditor()) {
        pattern = ExternalToolManager::getSelectedOrCurrentWord(ed);
    }
    RegexHelperDialog dlg(this, pattern);
    dlg.exec();
}

void MainWindow::onEncryptDocumentTriggered() {
    ScintillaNext *editor = currentEditor();
    if (!editor) {
        ui->statusBar->showMessage(tr("No active editor"), 3000);
        return;
    }

    EncryptDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        editor->QObject::setProperty("IsEncrypted", true);
        editor->QObject::setProperty("EncryptionPassword", dlg.passphrase());
        editor->insertText(editor->currentPos(), "");
        ui->statusBar->showMessage(tr("🔒 Document configured for AES-256 encryption. Save to write to disk."), 5000);
        ModernMessageBox::information(this, tr("Encryption Configured"),
                                      tr("This document is now set to be encrypted with AES-256 upon saving.\n\n"
                                         "Warning: Lost passphrases cannot be recovered by SparkNotePad. Ensure you "
                                         "keep your passphrase secure."));
    }
}

void MainWindow::onDecryptDocumentTriggered() {
    ScintillaNext *editor = currentEditor();
    if (!editor) {
        ui->statusBar->showMessage(tr("No active editor"), 3000);
        return;
    }

    if (!editor->QObject::property("IsEncrypted").toBool()) {
        ui->statusBar->showMessage(tr("Document is not encrypted"), 3000);
        return;
    }

    if (ModernMessageBox::question(this, tr("Remove Encryption"),
                                   tr("Are you sure you want to remove encryption from <b>%1</b>?<br>The next save "
                                      "will store it as plain unencrypted text.")
                                       .arg(editor->getName()))) {
        editor->QObject::setProperty("IsEncrypted", false);
        editor->QObject::setProperty("EncryptionPassword", QString());
        editor->insertText(editor->currentPos(), "");
        ui->statusBar->showMessage(tr("🔓 Encryption removed. Save to write as plain text."), 4000);
    }
}

void MainWindow::onHashValidatorTriggered() {
    HashIntegrityDialog dlg(this, currentEditor());
    dlg.exec();
}

void MainWindow::onToggleClipboardMonitoringTriggered() const {
    ClipboardMonitor *mon = ClipboardMonitor::instance();
    mon->toggleMonitoring(currentEditor());
}

void MainWindow::onClipboardHistoryTriggered() {
    ClipboardHistoryDialog dlg(this, currentEditor());
    dlg.exec();
}

void MainWindow::onClipboardMonitoringStateChanged(bool active) {
    if (actionToggleClipboardMonitoring) {
        actionToggleClipboardMonitoring->setChecked(active);
    }
    if (active) {
        ui->statusBar->showMessage(tr("📋 Clipboard Monitoring: ACTIVE (capturing copied text to document)"), 5000);
    } else {
        ui->statusBar->showMessage(tr("📋 Clipboard Monitoring: STOPPED (Total captured: %1)")
                                       .arg(ClipboardMonitor::instance()->capturedCount()),
                                   4000);
    }
}

void MainWindow::onClipboardClipCaptured(const ClipboardMonitor::CapturedClip &clip, int totalCount) {
    ui->statusBar->showMessage(tr("📋 Captured clipboard item #%1 (%2 chars)").arg(totalCount).arg(clip.charCount),
                               3000);
}

void MainWindow::onToggleTransparencyTriggered() {
    if (transparentWindowManager) {
        transparentWindowManager->toggle();
    }
}

void MainWindow::onIncreaseOpacityTriggered() {
    if (transparentWindowManager) {
        transparentWindowManager->increaseOpacity(10);
    }
}

void MainWindow::onDecreaseOpacityTriggered() {
    if (transparentWindowManager) {
        transparentWindowManager->decreaseOpacity(10);
    }
}

void MainWindow::onTransparencyOptionsTriggered() {
    if (transparentWindowManager) {
        TransparencyDialog dlg(transparentWindowManager, this);
        dlg.exec();
    }
}

void MainWindow::onTransparencyChanged(bool enabled, int opacity, TransparentWindowManager::TransparencyMode /*mode*/) {
    if (actionToggleTransparency) {
        actionToggleTransparency->setChecked(enabled);
    }
    if (enabled) {
        ui->statusBar->showMessage(tr("Window Transparency: ON (Opacity: %1%)").arg(opacity), 3000);
    } else {
        ui->statusBar->showMessage(tr("Window Transparency: OFF (100% Opaque)"), 3000);
    }
}

void MainWindow::onToggleTailMonitoringTriggered() {
    ScintillaNext *editor = currentEditor();
    if (!editor) {
        ui->statusBar->showMessage(tr("No active editor"), 3000);
        if (actionToggleTailMonitoring)
            actionToggleTailMonitoring->setChecked(false);
        return;
    }

    if (const QString filePath = editor->getFilePath(); filePath.isEmpty() || !QFile::exists(filePath)) {
        ui->statusBar->showMessage(tr("Cannot monitor: File is not saved to disk."), 4000);
        if (actionToggleTailMonitoring)
            actionToggleTailMonitoring->setChecked(false);
        return;
    }

    TailMonitoringManager::instance()->toggleMonitoring(editor);
}

void MainWindow::onTailMonitoringStateChanged(const ScintillaNext *editor, bool isMonitoring) {
    if (editor == currentEditor() && actionToggleTailMonitoring) {
        actionToggleTailMonitoring->setChecked(isMonitoring);
        if (actionTailAutoScroll) {
            actionTailAutoScroll->setChecked(TailMonitoringManager::instance()->autoScrollToEnd(editor));
        }
    }
}

void MainWindow::updateTailMonitoringUi(const ScintillaNext *editor) {
    if (!actionToggleTailMonitoring)
        return;
    const bool isMon = TailMonitoringManager::instance()->isMonitoring(editor);
    actionToggleTailMonitoring->setChecked(isMon);
    if (actionTailAutoScroll) {
        actionTailAutoScroll->setChecked(TailMonitoringManager::instance()->autoScrollToEnd(editor));
    }
}

void MainWindow::setupWindowMenu() {
    if (menuWindow)
        return;

    menuWindow = new QMenu(tr("&Window"), this);
    ui->menuBar->insertMenu(ui->menuHelp->menuAction(), menuWindow);

    // 1. Minimize
    auto *actionMinimize = new QAction(tr("&Minimize"), this);
#if defined(Q_OS_MACOS)
    actionMinimize->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
#else
    actionMinimize->setShortcut(QKeySequence(Qt::ALT | Qt::Key_F9));
#endif
    connect(actionMinimize, &QAction::triggered, this, &QWidget::showMinimized);
    menuWindow->addAction(actionMinimize);

    // 2. Zoom
    auto *actionZoom = new QAction(tr("&Zoom"), this);
    connect(actionZoom, &QAction::triggered, this, [this]() {
        if (isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
    });
    menuWindow->addAction(actionZoom);

    // 3. Toggle Full Screen
    menuWindow->addAction(ui->actionFullScreen);

    menuWindow->addSeparator();

    // 4. Next / Previous Document
    menuWindow->addAction(ui->actionNextTab);
    menuWindow->addAction(ui->actionPreviousTab);
    // 5. Focus Editor
    actionFocusEditor = new QAction(tr("Focus &Editor"), this);
    actionFocusEditor->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_E));
    connect(actionFocusEditor, &QAction::triggered, this, [this]() {
        ScintillaNext *ed = currentEditor();
        if (ed) {
            ed->grabFocus();
            AccessibilityManager::instance()->announce(ed, tr("Editor focused"));
        }
    });
    menuWindow->addAction(actionFocusEditor);

    menuWindow->addSeparator();

    // 5. Bring All to Front
    auto *actionBringToFront = new QAction(tr("&Bring All to Front"), this);
    connect(actionBringToFront, &QAction::triggered, this, [this]() {
        raise();
        activateWindow();
    });
    menuWindow->addAction(actionBringToFront);

    menuWindow->addSeparator();

    // Multi-Monitor and Retina screen management
    auto *actionMoveToNextScreen = new QAction(tr("Move to &Next Screen"), this);
    actionMoveToNextScreen->setIcon(QIcon(QStringLiteral(":/icons/arrow_up.svg")));
    actionMoveToNextScreen->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Right));
    connect(actionMoveToNextScreen, &QAction::triggered, this, &MainWindow::onMoveToNextScreenTriggered);
    menuWindow->addAction(actionMoveToNextScreen);

    auto *actionMoveToPrevScreen = new QAction(tr("Move to &Previous Screen"), this);
    actionMoveToPrevScreen->setIcon(QIcon(QStringLiteral(":/icons/arrow_down.svg")));
    actionMoveToPrevScreen->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Left));
    connect(actionMoveToPrevScreen, &QAction::triggered, this, &MainWindow::onMoveToPreviousScreenTriggered);
    menuWindow->addAction(actionMoveToPrevScreen);

    auto *actionCenterScreen = new QAction(tr("&Center Window on Screen"), this);
    actionCenterScreen->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C));
    connect(actionCenterScreen, &QAction::triggered, this, &MainWindow::onCenterOnScreenTriggered);
    menuWindow->addAction(actionCenterScreen);

    auto *actionDisplayDiagnostics = new QAction(tr("&Display & Retina Diagnostics..."), this);
    actionDisplayDiagnostics->setIcon(QIcon(QStringLiteral(":/icons/settings.svg")));
    actionDisplayDiagnostics->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_D));
    connect(actionDisplayDiagnostics, &QAction::triggered, this, &MainWindow::onDisplayDiagnosticsTriggered);
    menuWindow->addAction(actionDisplayDiagnostics);

    menuWindow->addSeparator();

    connect(menuWindow, &QMenu::aboutToShow, this, &MainWindow::populateWindowMenuDocuments);
    ensureMenuIconsVisible(menuWindow);
}

void MainWindow::onNextDocumentTriggered() {
    QVector<ScintillaNext *> allEditors = editors();
    if (allEditors.size() <= 1)
        return;

    auto curIdx = static_cast<int>(allEditors.indexOf(currentEditor()));
    auto nextIdx = static_cast<int>((curIdx + 1) % allEditors.size());
    dockedEditor->switchToEditor(allEditors[nextIdx]);
    allEditors[nextIdx]->grabFocus();
}

void MainWindow::onPreviousDocumentTriggered() {
    QVector<ScintillaNext *> allEditors = editors();
    if (allEditors.size() <= 1)
        return;

    auto curIdx = static_cast<int>(allEditors.indexOf(currentEditor()));
    auto prevIdx = static_cast<int>((curIdx - 1 + allEditors.size()) % allEditors.size());
    dockedEditor->switchToEditor(allEditors[prevIdx]);
    allEditors[prevIdx]->grabFocus();
}

void MainWindow::populateWindowMenuDocuments() {
    for (QAction *act : m_windowDocActions) {
        menuWindow->removeAction(act);
        delete act;  // NOSONAR(cpp:S5025)
    }
    m_windowDocActions.clear();

    QVector<ScintillaNext *> allEditors = editors();
    if (allEditors.isEmpty())
        return;

    auto *docGroup = new QActionGroup(this);
    docGroup->setExclusive(true);

    const ScintillaNext *current = currentEditor();
    int idx = 1;
    for (ScintillaNext *ed : allEditors) {
        QString name = ed->getName();
        if (!ed->isSavedToDisk()) {
            name.prepend(QStringLiteral("● "));
        }
        QString label = (idx <= 9) ? QString("&%1  %2").arg(idx).arg(name) : name;
        auto *act = new QAction(label, menuWindow);
        act->setCheckable(true);
        act->setChecked(ed == current);
        if (ed->isFile()) {
            act->setToolTip(ed->getFilePath());
            act->setStatusTip(ed->getFilePath());
        }
        connect(act, &QAction::triggered, this, [this, ed]() {
            dockedEditor->switchToEditor(ed);
            ed->grabFocus();
        });
        docGroup->addAction(act);
        menuWindow->addAction(act);
        m_windowDocActions.append(act);
        idx++;
    }
}

void MainWindow::setupThemeMenu() {
    if (menuTheme)
        return;

    // Create dedicated top-level Themes menu in menuBar between View and Document/Language
    menuTheme = new QMenu(tr("&Themes"), this);
    if (ui->menuLanguage) {
        ui->menuBar->insertMenu(ui->menuLanguage->menuAction(), menuTheme);
    } else {
        ui->menuBar->addMenu(menuTheme);
    }

    // 1. Open Theme Switcher... (Ctrl+Shift+T)
    actionThemeSwitcher = new QAction(tr("Open Theme Switcher..."), this);
    actionThemeSwitcher->setIcon(QIcon(QStringLiteral(":/icons/settings.svg")));
    actionThemeSwitcher->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_T));
    connect(actionThemeSwitcher, &QAction::triggered, this, [this]() {
        ThemeSwitcherDialog dlg(this);
        dlg.exec();
    });
    menuTheme->addAction(actionThemeSwitcher);

    menuTheme->addSeparator();

    // 2. Light Themes Only (Submenu)
    QMenu *menuLightOnly = menuTheme->addMenu(tr("Light Themes Only"));
    QList<const ThemePalette *> lightPalettes = getThemesByDarkMode(false);
    for (const ThemePalette *pal : lightPalettes) {
        const auto *act = menuLightOnly->addAction(QString::fromUtf8(pal->displayName));
        connect(act, &QAction::triggered, this, [this, pal]() {
            ThemeManager::instance()->setPalette(QString::fromLatin1(pal->id), this);
            ui->statusBar->showMessage(tr("Theme applied: %1").arg(QString::fromUtf8(pal->displayName)), 3000);
        });
    }

    // 3. Dark Themes Only (Submenu)
    QMenu *menuDarkOnly = menuTheme->addMenu(tr("Dark Themes Only"));
    QList<const ThemePalette *> darkPalettes = getThemesByDarkMode(true);
    for (const ThemePalette *pal : darkPalettes) {
        const auto *act = menuDarkOnly->addAction(QString::fromUtf8(pal->displayName));
        connect(act, &QAction::triggered, this, [this, pal]() {
            ThemeManager::instance()->setPalette(QString::fromLatin1(pal->id), this);
            ui->statusBar->showMessage(tr("Theme applied: %1").arg(QString::fromUtf8(pal->displayName)), 3000);
        });
    }

    menuTheme->addSeparator();

    // 4. All 25 Families grouped neatly in a dedicated Submenu
    QMenu *menuFamilies = menuTheme->addMenu(tr("Themes by Family"));
    QStringList families = getThemeFamiliesList();
    for (const QString &family : families) {
        QMenu *famMenu = menuFamilies->addMenu(family);
        QList<const ThemePalette *> palettes = getThemePalettesByFamily(family);
        for (const ThemePalette *pal : palettes) {
            QString itemLabel = QString::fromUtf8(pal->displayName);
            if (pal->isDark) {
                itemLabel += QStringLiteral(" (Dark)");
            } else {
                itemLabel += QStringLiteral(" (Light)");
            }
            const auto *act = famMenu->addAction(itemLabel);
            connect(act, &QAction::triggered, this, [this, pal]() {
                ThemeManager::instance()->setPalette(QString::fromLatin1(pal->id), this);
                ui->statusBar->showMessage(tr("Theme applied: %1").arg(QString::fromUtf8(pal->displayName)), 3000);
            });
        }
    }

    menuTheme->addSeparator();

    // 5. Reset and Mode Toggles
    actionThemeLight = new QAction(tr("Reset to Default (Light Mode - NotepadNext)"), this);
    connect(actionThemeLight, &QAction::triggered, this, [this]() {
        ThemeManager::instance()->setPalette(QStringLiteral("notepadNext"), this);
        ui->statusBar->showMessage(tr("Theme reset to default Light Mode (NotepadNext)"), 3000);
    });
    menuTheme->addAction(actionThemeLight);

    actionThemeDark = new QAction(tr("Default Dark Mode"), this);
    connect(actionThemeDark, &QAction::triggered, this, [this]() {
        ThemeManager::instance()->setPalette(QStringLiteral("dracula"), this);
        ui->statusBar->showMessage(tr("Theme set to Dark Mode (Dracula)"), 3000);
    });
    menuTheme->addAction(actionThemeDark);

    actionToggleTheme = new QAction(tr("Toggle Dark / Light Mode"), this);
    actionToggleTheme->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_T));
    connect(actionToggleTheme, &QAction::triggered, this, [this]() {
        ThemeManager::instance()->toggleTheme(this);
        bool dark = ThemeManager::instance()->isDarkActive();
        ui->statusBar->showMessage(dark ? tr("Switched to Dark Mode") : tr("Switched to Light Mode"), 3000);
    });
    menuTheme->addAction(actionToggleTheme);
    ensureMenuIconsVisible(menuTheme);
}

void MainWindow::onMoveToNextScreenTriggered() {
    ScreenDisplayManager::instance().moveToNextScreen(this);
}

void MainWindow::onMoveToPreviousScreenTriggered() {
    ScreenDisplayManager::instance().moveToPreviousScreen(this);
}

void MainWindow::onCenterOnScreenTriggered() {
    ScreenDisplayManager::instance().centerOnCurrentScreen(this);
}

void MainWindow::onDisplayDiagnosticsTriggered() {
    ScreenDisplayManager::instance().showDisplayInfoDialog(this);
}

void MainWindow::onUserGuideTriggered() {
    DocumentationManager::instance().openUserGuide(this);
}

void MainWindow::onDeveloperGuideTriggered() {
    DocumentationManager::instance().openDeveloperGuide(this);
}

// ─────────────────────────────────────────────────────────────────────────────
// Application Lifecycle: Restart & Exit  (SparkNotePad v1.3.5)
// Security: CVE-2026-25926 (CWE-426 Untrusted Search Path) mitigation applied:
//   • Always resolve executable path via QCoreApplication::applicationFilePath()
//   • Never pass bare binary names or relative paths to QProcess::startDetached()
//   • macOS: use /usr/bin/open with -n -a <BundlePath> for .app bundle relaunch
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::restartApplication(bool safeMode) {
    ui->statusBar->showMessage(tr("Preparing to restart SparkNotePad..."), 3000);

    // 1. Verify and prompt for unsaved changes; abort if user clicks Cancel
    if (!checkEditorsBeforeClose(editors())) {
        ui->statusBar->showMessage(tr("Restart canceled."), 3000);
        return;
    }

    // 2. Save session and flush settings to disk
    saveSettings();

    // 3. Construct canonical executable path — CVE-2026-25926 mitigation (CWE-426)
    //    QCoreApplication::applicationFilePath() returns the absolute path of the
    //    running binary, preventing any working-directory binary hijacking.
    const QString appFilePath = QCoreApplication::applicationFilePath();
    QStringList args = QCoreApplication::arguments().mid(1);
    if (safeMode && !args.contains(QLatin1StringView("--safe-mode"))) {
        args.append(QLatin1StringView("--safe-mode"));
    }

    // 4. Release SingleApplication instance lock so the new process can acquire it
    //    Without this, the relaunched instance detects an existing primary and exits.
    //    SparkNotePadApplication exposes releaseSingleInstanceLock() for this purpose.
    if (auto *sparkApp = qobject_cast<SparkNotePadApplication *>(QCoreApplication::instance())) {
        sparkApp->releaseSingleInstanceLock();
    }

#if defined(Q_OS_MACOS)
    const QDir appDir(QCoreApplication::applicationDirPath());
    bool isBundle = false;
    QString bundlePath;
    if (QDir bundleDir = appDir; bundleDir.dirName() == QLatin1StringView("MacOS") && bundleDir.cdUp() &&
                                 bundleDir.cdUp() && bundleDir.dirName().endsWith(QLatin1StringView(".app"))) {
        isBundle = true;
        bundlePath = bundleDir.absolutePath();
    }

    const qint64 currentPid = QCoreApplication::applicationPid();
    QString relaunchCmd;
    if (isBundle) {
        if (args.isEmpty()) {
            relaunchCmd = QStringLiteral("while kill -0 %1 2>/dev/null; do sleep 0.05; done; exec /usr/bin/open \"%2\"")
                              .arg(currentPid)
                              .arg(bundlePath);
        } else {
            relaunchCmd =
                QStringLiteral("while kill -0 %1 2>/dev/null; do sleep 0.05; done; exec /usr/bin/open \"%2\" --args %3")
                    .arg(currentPid)
                    .arg(bundlePath)
                    .arg(args.join(QLatin1Char(' ')));
        }
    } else {
        relaunchCmd = QStringLiteral("while kill -0 %1 2>/dev/null; do sleep 0.05; done; exec \"%2\" %3")
                          .arg(currentPid)
                          .arg(appFilePath)
                          .arg(args.join(QLatin1Char(' ')));
    }

    if (const bool launched =
            QProcess::startDetached(QStringLiteral("/bin/sh"), QStringList{QStringLiteral("-c"), relaunchCmd});
        !launched) {
        QMessageBox::warning(this, tr("Restart Failed"),
                             tr("Unable to relaunch SparkNotePad automatically from:\n%1").arg(appFilePath));
        return;
    }
#else
    if (const bool launched = QProcess::startDetached(appFilePath, args, QCoreApplication::applicationDirPath());
        !launched) {
        QMessageBox::warning(this, tr("Restart Failed"),
                             tr("Unable to relaunch SparkNotePad automatically from:\n%1").arg(appFilePath));
        return;
    }
#endif

    QCoreApplication::quit();
}

void MainWindow::exitApplication() {
    ui->statusBar->showMessage(tr("Closing SparkNotePad..."), 2000);

    // Prompt for unsaved changes across all open tabs; allow cancellation.
    if (!checkEditorsBeforeClose(editors())) {
        ui->statusBar->showMessage(tr("Exit canceled."), 2000);
        return;
    }

    saveSettings();
    QCoreApplication::quit();
}
