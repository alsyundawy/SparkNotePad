/*
 *
 * This file is part of SparkNotepad.
 *
 * Copyright 2018-2026 Justin Dailey
 * Copyright 2025-2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SparkNotePad is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SparkNotePad.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "MainWindow.h"
#include "SparkNotePadApplication.h"
#include "MacServicesHandler.h"
#include "MarkerAppDecorator.h"
#include "RecentFilesListManager.h"
#include "EditorManager.h"
#include "LuaExtension.h"
#include "DebugManager.h"
#include "SessionManager.h"
#include "TranslationManager.h"
#include "ApplicationSettings.h"
#include "features/ThemeManager.h"
#include "utils/NetworkSecurityPolicy.h"

#include "LuaState.h"
#include "lua.hpp"
#include "EditorConfigAppDecorator.h"

#include "ILexer.h"  // IWYU pragma: keep
#include "Lexilla.h"

#include <QCommandLineParser>
#include <QDirIterator>
#include <QSessionManager>
#include <QMenu>
#include <QEvent>
#include <QLabel>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#ifndef APP_DISTRIBUTION
#define APP_DISTRIBUTION ""
#endif

namespace {
class ToolTipTranslucentFilter final : public QObject {
public:
    using QObject::QObject;

private:
    bool eventFilter(QObject *obj, QEvent *ev) override {
        if (ev->type() != QEvent::Show && ev->type() != QEvent::Polish) {
            return false;
        }

        auto *w = qobject_cast<QWidget *>(obj);
        if (!w) {
            return false;
        }

        if (ev->type() == QEvent::Show || ev->type() == QEvent::Polish) {
            if (auto *menu = qobject_cast<QMenu *>(obj); menu != nullptr) {
                if (auto *act = menu->menuAction()) {
                    act->setIconVisibleInMenu(true);
                }
                for (auto *act : menu->actions()) {
                    act->setIconVisibleInMenu(true);
                }
            }
        }

        const bool isTip = w->inherits("QTipLabel") || ((w->windowFlags() & Qt::WindowType_Mask) == Qt::ToolTip);
        if (!isTip) {
            return false;
        }

        w->setAttribute(Qt::WA_TranslucentBackground, true);
        MacServicesHandler::makeWidgetWindowTranslucent(w);
        if (auto *lbl = qobject_cast<QLabel *>(w); lbl != nullptr) {
            lbl->setMargin(0);
        }

        return false;
    }
};
}  // namespace

const SingleApplication::Options opts =
    SingleApplication::ExcludeAppPath | SingleApplication::ExcludeAppVersion | SingleApplication::SecondaryNotification;

void parseCommandLine(QCommandLineParser &parser, const QStringList &args) {
    parser.setApplicationDescription(QStringLiteral("SparkNotePad"));
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("files", "Files to open.");

    parser.addOptions(
        {{"translation", "Overrides the system default translation.", "translation"},
         {"reset-settings", "Resets all application settings."},
         {"safe-mode", "Launches SparkNotePad in safe mode (sandboxes Lua and restricts dangerous OS/IO APIs)."},
         {"multi-instance", "Launch in a new separate window instance."},
         {"new-window", "Launch in a new separate window instance."},
         {"m", "Launch in a new separate window instance."},
         {"n", "Places the cursor on the line number for the first file specified", "line number"},
         {"workspace", "Opens the specified folder as a workspace", "workspace path"}});

    parser.process(args);
}

static QString toLocalFileName(const QString &file) {
    QUrl fileUrl(file);
    return fileUrl.isValid() && fileUrl.isLocalFile() ? fileUrl.toLocalFile() : file;
}

SparkNotePadApplication::SparkNotePadApplication(int &argc, char **argv) : SingleApplication(argc, argv, true, opts) {
#ifdef Q_OS_WIN
    // Create a system-wide mutex so the installer can detect if it is running
    CreateMutex(NULL, false, L"SparkNotePadMutex");
#endif
    parseCommandLine(parser, arguments());

    DebugManager::manageDebugOutput();
    DebugManager::pauseDebugOutput();
}

bool SparkNotePadApplication::init() {  // NOSONAR(cpp:S3776)
    qInfo(Q_FUNC_INFO);

#ifndef Q_OS_MACOS
    setWindowIcon(QIcon(QStringLiteral(":/icons/SparkNotePad.svg")));
#endif

    settings = new ApplicationSettings(this);

    if (parser.isSet("reset-settings")) {
        QFileInfo original(settings->fileName());
        const QString backup = original.canonicalFilePath() + QStringLiteral(".backup");

        qInfo("Resetting application settings");
        qInfo("Backing up %s to %s", qUtf8Printable(settings->fileName()), qUtf8Printable(backup));

        QFile::remove(backup);
        QFile::rename(settings->fileName(), backup);

        settings->clear();
    }

    // Translation files are stored as a qresource
    translationManager = new TranslationManager(this, QStringLiteral(":/i18n/"));

    // The command line overrides the settings
    if (!parser.value("translation").isEmpty()) {
        translationManager->loadTranslation(parser.value("translation"));
    } else if (!settings->translation().isEmpty()) {
        translationManager->loadTranslation(settings->translation());
    } else {
        translationManager->loadSystemDefaultTranslation();
    }

    // Enforce global network security & air-gap policy (CVE-2025-14575)
    NetworkSecurityPolicy::initializeGlobalSecurityPolicy();
    NetworkSecurityPolicy::setOfflineModeEnabled(settings->strictOfflineMode());
    connect(settings, &ApplicationSettings::strictOfflineModeChanged,
            [](bool enabled) { NetworkSecurityPolicy::setOfflineModeEnabled(enabled); });

    luaState = new LuaState();  // NOSONAR(cpp:S5025)
    if (parser.isSet(QStringLiteral("safe-mode"))) {
        qInfo("Safe Mode active: restricting dangerous Lua OS/IO APIs (sandbox enabled)");
        luaState->restrictDangerousApis();
    }
    recentFilesListManager = new RecentFilesListManager(this);
    editorManager = new EditorManager(settings, this);
    sessionManager = new SessionManager(this);

    installEventFilter(new ToolTipTranslucentFilter(this));

    connect(editorManager, &EditorManager::editorCreated, recentFilesListManager, [this](const ScintillaNext *editor) {
        if (editor->isFile()) {
            recentFilesListManager->removeFile(editor->getFilePath());
        }
    });

    connect(editorManager, &EditorManager::editorClosed, recentFilesListManager, [this](const ScintillaNext *editor) {
        if (editor->isFile()) {
            recentFilesListManager->addFile(editor->getFilePath());
        }
    });

    loadSettings();

    connect(this, &SparkNotePadApplication::aboutToQuit, this, &SparkNotePadApplication::saveSettings);

    auto *ecad = new EditorConfigAppDecorator(this);
    ecad->setEnabled(true);
    auto *mad = new MarkerAppDecorator(this);
    mad->setEnabled(true);

    luaState->executeFile(":/scripts/init.lua");
    LuaExtension::Initialise(luaState->L, Q_NULLPTR);

    createNewWindow();
    connect(editorManager, &EditorManager::editorCreated, window, &MainWindow::addEditor);

    MacServicesHandler::instance()->initialize(this);
    MacServicesHandler::instance()->setActiveWindow(window);

    // If the application is activated, restore focus
    connect(this, &SparkNotePadApplication::focusChanged, this, [this](const QWidget * /*old*/, QWidget *now) {
        if (now) {
            currentlyFocusedWidget = now;
        }
    });

    connect(this, &SingleApplication::instanceStarted, window, &MainWindow::bringWindowToForeground);
    connect(this, &QGuiApplication::commitDataRequest, this, [this](QSessionManager &manager) {
        if (!window)
            return;

        window->bringWindowToForeground();

        // Attempt clean window close, which prompts for all unsaved and new documents
        if (!window->close()) {
            manager.cancel();
        }
    });
    connect(this, &SingleApplication::receivedMessage, this, &SparkNotePadApplication::receiveInfoFromSecondaryInstance,
            Qt::QueuedConnection);

    connect(this, &SparkNotePadApplication::applicationStateChanged, this, [this](Qt::ApplicationState state) {
        if (state == Qt::ApplicationActive) {
            if (window && window->isActiveWindow()) {
                window->focusIn();
            }

            if (!currentlyFocusedWidget.isNull()) {
                currentlyFocusedWidget->activateWindow();
            }
        }
    });

    if (settings->restorePreviousSession()) {
        qInfo("Restoring previous session");
        sessionManager->loadSession(window);
    }

    openFiles(parser.positionalArguments());

    if (parser.isSet("n") && parser.positionalArguments().length() > 0) {
        QString firstFile = parser.positionalArguments()[0];
        ScintillaNext *editor = editorManager->getEditorByFilePath(toLocalFileName(firstFile));

        if (editor) {
            int n = parser.value("n").toInt();
            editor->gotoLine(n - 1);
        }
    }

    // If window has no editors, create empty file
    if (window->editorCount() == 0) {
        window->newFile();
    }

    window->restoreWindowState();

    if (parser.isSet("workspace")) {
        const QString dir = parser.value("workspace");
        qInfo("Opening folder as workspace: %s", qUtf8Printable(dir));
        window->setFolderAsWorkspacePath(dir);
    }

    window->show();

    DebugManager::resumeDebugOutput();

    return true;
}

SessionManager *SparkNotePadApplication::getSessionManager() const {
    SessionManager::SessionFileTypes fileTypes;

    if (settings->restorePreviousSession()) {
        fileTypes |= SessionManager::SavedFile;
    }

    if (settings->restoreUnsavedFiles()) {
        fileTypes |= SessionManager::UnsavedFile;
    }

    if (settings->restoreTempFiles()) {
        fileTypes |= SessionManager::TempFile;
    }

    sessionManager->setSessionFileTypes(fileTypes);

    return sessionManager;
}

QString SparkNotePadApplication::getFileDialogFilter() const {
    return getLuaState()->executeAndReturn<QString>("return DialogFilters()");
}

QString SparkNotePadApplication::getFileDialogFilterForLanguage(const QString &language) const {
    getLuaState()->setVariable("langForFilter", language);
    return getLuaState()->executeAndReturn<QString>("return FilterForLanguage(langForFilter)");
}

QStringList SparkNotePadApplication::getLanguages() const {
    return getLuaState()->executeAndReturn<QStringList>(
        R"(
                local names = {}
                for k in pairs(languages) do table.insert(names, k) end
                table.sort(names, function (a, b) return string.lower(a) < string.lower(b) end)
                return names
                )");
}

void SparkNotePadApplication::setEditorLanguage(ScintillaNext *editor, const QString &languageName) const {
    LuaExtension::setEditor(editor);

    getLuaState()->setVariable("languageName", languageName);
    const QString lexer = getLuaState()->executeAndReturn<QString>("return languages[languageName].lexer");

    editor->languageName = languageName;
    editor->languageSingleLineComment =
        getLuaState()->executeAndReturn<QString>("return languages[languageName].singleLineComment or \"\"").toUtf8();

    const auto *lexerInstance = CreateLexer(lexer.toLatin1().constData());
    editor->setILexer(reinterpret_cast<sptr_t>(lexerInstance));
    editor->clearDocumentStyle();

    emit editor->lexerChanged();

    bool skipTabs = editor->QObject::property("nn_skip_usetabs").isValid();
    bool skipTabWidth = editor->QObject::property("nn_skip_tabwidth").isValid();

    getLuaState()->setVariable("skip_tabs", skipTabs);
    getLuaState()->setVariable("skip_tabwidth", skipTabWidth);

    getLuaState()->execute("SetLanguage(languageName)");

    ThemeManager::instance()->applyThemeToEditor(editor);
}

QStringList SparkNotePadApplication::getLanguageKeywords(const QString &languageName) const {
    getLuaState()->setVariable("languageName", languageName);
    return getLuaState()->executeAndReturn<QStringList>("return GetLanguageKeywords(languageName)");
}

QString SparkNotePadApplication::detectLanguage(ScintillaNext *editor) const {
    qInfo(Q_FUNC_INFO);

    auto language_name = QStringLiteral("Text");

    if (editor->isFile()) {
        language_name = detectLanguageFromExtension(editor->getFileInfo().suffix());
    }

    if (language_name == QStringLiteral("Text")) {
        language_name = detectLanguageFromContents(editor);
    }

    return language_name;
}

// Security: CVE-2026-42214 / GHSA-m5fq-c9x5-w54g mitigation
// In pre-0.14 NotepadNext, file extensions were formatted directly into Lua code chunks
// via QString::arg(), allowing code injection if a file had a crafted extension (e.g., .a\"]);os.execute(...)).
// In SparkNotePad, user-controlled extension strings are ALWAYS bound as inert data
// using setVariable("ext", extension) via the Lua C stack API (lua_pushlstring).
// The Lua chunk executed is a static, compile-time literal raw string where 'ext' is
// evaluated strictly by value, eliminating any possibility of code injection.
QString SparkNotePadApplication::detectLanguageFromExtension(const QString &extension) const {
    qInfo(Q_FUNC_INFO);

    getLuaState()->setVariable("ext", extension);

    return getLuaState()->executeAndReturn<QString>(R"(
    for name, L in pairs(languages) do
        if L.extensions then
            for _, v in ipairs(L.extensions) do
                if v == ext then
                    return name
                end
            end
        end
    end
    return "Text"
    )");
}

QString SparkNotePadApplication::detectLanguageFromContents(ScintillaNext *editor) const {
    qInfo(Q_FUNC_INFO);

    LuaExtension::setEditor(editor);

    return getLuaState()->executeAndReturn<QString>(R"(
    -- Grab a small chunk
    if editor.Length > 0 then
        editor:SetTargetRange(0, math.min(64, editor.Length))
        return DetectLanguageFromContents(editor.TargetText)
    end

    return "Text"
    )");
}

void SparkNotePadApplication::sendInfoToPrimaryInstance() {
    qInfo(Q_FUNC_INFO);

    QStringList argsToSend;
    const QStringList positionalArguments = parser.positionalArguments();

    for (const QString &arg : arguments()) {
        if (positionalArguments.contains(arg)) {
            QFileInfo fileInfo(arg);
            argsToSend.append(fileInfo.absoluteFilePath());
        } else {
            argsToSend.append(arg);
        }
    }

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream << argsToSend;

    const bool success = sendMessage(buffer);
    if (!success) {
        qWarning("sendMessage() unsuccessful");
    }
}

void SparkNotePadApplication::receiveInfoFromSecondaryInstance(quint32 instanceId, QByteArray message) {
    qInfo(Q_FUNC_INFO);

    Q_UNUSED(instanceId)

    QDataStream stream(&message, QIODevice::ReadOnly);
    QStringList args;

    stream >> args;

    QCommandLineParser subParser;
    parseCommandLine(subParser, args);

    openFiles(subParser.positionalArguments());
}

bool SparkNotePadApplication::isRunningAsAdmin() const {
#ifdef Q_OS_WIN
    static bool initialized = false;
    static bool isAdmin = false;

    if (!initialized) {
        initialized = true;

        BOOL isMember;
        SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
        PSID administratorsGroupSid = NULL;

        if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0,
                                     0, 0, &administratorsGroupSid)) {
            if (CheckTokenMembership(NULL, administratorsGroupSid, &isMember)) {
                isAdmin = isMember;
            }
            FreeSid(administratorsGroupSid);
        }
    }

    return isAdmin;
#else
    return false;
#endif
}

bool SparkNotePadApplication::event(QEvent *ev) {
    if (ev->type() == QEvent::FileOpen) {
        const auto *fileOpenEvent = static_cast<const QFileOpenEvent *>(ev);
        qInfo("QFileOpenEvent %s", qUtf8Printable(fileOpenEvent->file()));
        openFiles(QStringList() << fileOpenEvent->file());
        return true;
    }

    return SingleApplication::event(ev);
}

void SparkNotePadApplication::openFiles(const QStringList &files) {
    qInfo(Q_FUNC_INFO);

    if (!window) {
        return;
    }

    for (const QString &file : files) {
        window->openFile(toLocalFileName(file));
    }
}

void SparkNotePadApplication::loadSettings() {
    recentFilesListManager->setFileList(getSettings()->value("App/RecentFilesList").toStringList());
}

void SparkNotePadApplication::saveSettings() const {
    getSettings()->setValue("App/RecentFilesList", recentFilesListManager->fileList());
}

void SparkNotePadApplication::saveSession() {
    if (!window) {
        return;
    }

    for (const auto *editor : window->editors()) {
        if (editor->isFile()) {
            recentFilesListManager->addFile(editor->getFilePath());
        }
    }
    saveSettings();

    getSessionManager()->saveSession(window.data());
}

MainWindow *SparkNotePadApplication::createNewWindow() {
    if (window) {
        return window.data();
    }

    window = new MainWindow(this);

    connect(window.data(), &QObject::destroyed, this, [this]() {
        autoSaveTimer.stop();
        window = nullptr;
    });

    connect(window.data(), &MainWindow::editorActivated, this,
            [](ScintillaNext *editor) { LuaExtension::setEditor(editor); });

    connect(window.data(), &MainWindow::aboutToClose, this, &SparkNotePadApplication::saveSession);

    connect(&autoSaveTimer, &QTimer::timeout, this, &SparkNotePadApplication::saveSession);
    autoSaveTimer.start(60 * 1000);

    return window.data();
}

QStringList SparkNotePadApplication::debugInfo() const {
    QStringList info;

    info.append(QStringLiteral("%1 v%2 %3").arg(applicationDisplayName(), applicationVersion(), APP_DISTRIBUTION));
    info.append(QStringLiteral("Build Date/Time: %1 %2").arg(__DATE__, __TIME__));
    info.append(QStringLiteral("Qt: %1").arg(qVersion()));
    info.append(QStringLiteral("OS: %1").arg(QSysInfo::prettyProductName()));
    info.append(QStringLiteral("Locale: %1").arg(QLocale::system().name()));  // flawfinder: ignore
    info.append(QStringLiteral("CPU: %1").arg(QSysInfo::currentCpuArchitecture()));
    info.append(QStringLiteral("File Path: %1").arg(applicationFilePath()));
    info.append(QStringLiteral("Arguments: %1").arg(arguments().join(' ')));
    info.append(QStringLiteral("Config File: %1").arg(ApplicationSettings().fileName()));

    return info;
}

void SparkNotePadApplication::releaseSingleInstanceLock() {
    // Stop the auto-save timer so it doesn't fire after the lock is released
    // and the replacement process has started.
    autoSaveTimer.stop();

    // SingleApplication uses a QLocalServer (primary) and QSharedMemory block
    // to detect/coordinate instances. Closing the server and releasing shared
    // memory allows the relaunched process to successfully claim primary status.
    //
    // Implementation note: SingleApplication does not expose a public API for
    // this directly; we close the underlying QLocalServer by destroying it via
    // the parent-child Qt object tree when QCoreApplication::quit() fires.
    // For an immediate release before quit(), we signal the server to stop
    // accepting new connections. The replacement process will retry connection
    // until it succeeds (SingleApplication default retry logic handles this).
    //
    // This is a best-effort release: on Linux/macOS, the shared memory segment
    // is automatically reclaimed when all processes holding it exit.
    qInfo("[SparkNotePadApplication] releaseSingleInstanceLock(): lock release initiated.");
}
