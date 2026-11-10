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

#include "PreferencesDialog.h"
#include "SparkNotePadApplication.h"
#include "TranslationManager.h"
#include "ui_PreferencesDialog.h"
#include "ScintillaNext.h"
#include "ThemeManager.h"
#include "MainWindow.h"
#include "ShortcutCustomizerDialog.h"
#include "CustomizeToolbarDialog.h"
#include "features/ThemePaletteData.h"
#include "features/TransparentWindowManager.h"
#include "features/ToolbarIconManager.h"

#include <QAbstractItemView>
#include <QListView>
#include <QButtonGroup>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFormLayout>
#include <QScrollArea>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QTabWidget>
#include <QToolBar>
#include <QUrl>

#include <algorithm>
#include <array>
#include <functional>

PreferencesDialog::PreferencesDialog(ApplicationSettings *settings, QWidget *parent)
    : QDialog(parent, Qt::Tool), ui(std::make_unique<Ui::PreferencesDialog>()), settings(settings) {
    ui->setupUi(this);

    // Setup Tabbed Interface:
    // Tab 0: General (existing scrollArea)
    // Tab 1: Shortcuts (ShortcutCustomizerWidget)
    // Tab 2: Themes & Toolbar (Relocated Themes + Toolbar Icon Packs configuration)
    // Tab 3: Custom Toolbar Config (CustomizeToolbarWidget)
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setObjectName(QStringLiteral("preferencesTabWidget"));

    // Detach scrollArea from verticalLayout and place into Tab 0 ("General")
    ui->verticalLayout->removeWidget(ui->scrollArea);
    m_tabWidget->addTab(ui->scrollArea, QIcon(QStringLiteral(":/icons/settings.svg")), tr("&General"));

    // Find main window for action harvesting in ShortcutCustomizerWidget and ToolbarCustomizer
    QMainWindow *mainWin = qobject_cast<QMainWindow *>(parent);
    if (!mainWin) {
        mainWin = qobject_cast<QMainWindow *>(window());
    }
    if (!mainWin) {
        const auto topLevels = QApplication::topLevelWidgets();
        const auto it =
            std::ranges::find_if(topLevels, [](QWidget *w) { return qobject_cast<QMainWindow *>(w) != nullptr; });
        if (it != topLevels.end()) {
            mainWin = qobject_cast<QMainWindow *>(*it);
        }
    }

    // Tab 1: Shortcuts
    m_shortcutWidget = new ShortcutCustomizerWidget(mainWin, this);
    connect(m_shortcutWidget, &ShortcutCustomizerWidget::shortcutsChanged, this, &PreferencesDialog::shortcutsChanged);
    m_tabWidget->addTab(m_shortcutWidget, QIcon(QStringLiteral(":/icons/list_with_icons.svg")), tr("&Shortcuts"));

    // Tab 2: Themes & Toolbar
    setupThemesAndToolbarTab();

    // Tab 3: Custom Toolbar Config
    setupCustomToolbarTab(mainWin);

    // Insert tab widget at the top of the dialog layout
    ui->verticalLayout->insertWidget(0, m_tabWidget, 1);

    resize(860, 680);
    setMinimumSize(740, 560);

    connect(ThemeManager::instance(), &ThemeManager::paletteChanged, this, &PreferencesDialog::updateThemeColors);
    updateThemeColors();

    const QIcon icon = style()->standardIcon(QStyle::SP_MessageBoxInformation);
    const QPixmap pixmap = icon.pixmap(QSize(16, 16));
    ui->labelAppRestartIcon->setPixmap(pixmap);
    ui->labelAppRestartIcon->hide();
    ui->labelAppRestart->hide();

    MapSettingToCheckBox(ui->checkBoxMenuBar, &ApplicationSettings::showMenuBar, &ApplicationSettings::setShowMenuBar,
                         &ApplicationSettings::showMenuBarChanged);
    MapSettingToCheckBox(ui->checkBoxToolBar, &ApplicationSettings::showToolBar, &ApplicationSettings::setShowToolBar,
                         &ApplicationSettings::showToolBarChanged);
    MapSettingToCheckBox(ui->checkBoxStatusBar, &ApplicationSettings::showStatusBar,
                         &ApplicationSettings::setShowStatusBar, &ApplicationSettings::showStatusBarChanged);
    MapSettingToCheckBox(ui->checkBoxRecenterSearchDialog, &ApplicationSettings::centerSearchDialog,
                         &ApplicationSettings::setCenterSearchDialog, &ApplicationSettings::centerSearchDialogChanged);

    MapSettingToGroupBox(ui->gbxRestorePreviousSession, &ApplicationSettings::restorePreviousSession,
                         &ApplicationSettings::setRestorePreviousSession,
                         &ApplicationSettings::restorePreviousSessionChanged);
    connect(ui->gbxRestorePreviousSession, &QGroupBox::toggled, this, [this](bool checked) {
        if (!checked) {
            ui->checkBoxUnsavedFiles->setChecked(false);
            ui->checkBoxRestoreTempFiles->setChecked(false);
        } else {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("This feature is experimental and it should not be considered safe for critically "
                                    "important work. It may lead to possible data loss. Use at your own risk."));
        }
    });

    MapSettingToCheckBox(ui->checkBoxUnsavedFiles, &ApplicationSettings::restoreUnsavedFiles,
                         &ApplicationSettings::setRestoreUnsavedFiles,
                         &ApplicationSettings::restoreUnsavedFilesChanged);
    MapSettingToCheckBox(ui->checkBoxRestoreTempFiles, &ApplicationSettings::restoreTempFiles,
                         &ApplicationSettings::setRestoreTempFiles, &ApplicationSettings::restoreTempFilesChanged);

    MapSettingToCheckBox(ui->checkBoxCombineSearchResults, &ApplicationSettings::combineSearchResults,
                         &ApplicationSettings::setCombineSearchResults,
                         &ApplicationSettings::combineSearchResultsChanged);

    ui->comboBoxTranslation->setView(new QListView(ui->comboBoxTranslation));
    populateTranslationComboBox();
    connect(ui->comboBoxTranslation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        this->settings->setTranslation(ui->comboBoxTranslation->itemData(idx).toString());
        showApplicationRestartRequired();
    });

    MapSettingToCheckBox(ui->checkBoxExitOnLastTabClosed, &ApplicationSettings::exitOnLastTabClosed,
                         &ApplicationSettings::setExitOnLastTabClosed,
                         &ApplicationSettings::exitOnLastTabClosedChanged);
    MapSettingToCheckBox(ui->checkBoxAutoDetectEncoding, &ApplicationSettings::autoDetectEncoding,
                         &ApplicationSettings::setAutoDetectEncoding,
                         &ApplicationSettings::autoDetectEncodingChanged);
    MapSettingToCheckBox(ui->checkBoxConfirmSaveAll, &ApplicationSettings::confirmSaveAll,
                         &ApplicationSettings::setConfirmSaveAll,
                         &ApplicationSettings::confirmSaveAllChanged);

    ui->fcbDefaultFont->setView(new QListView(ui->fcbDefaultFont));
    ui->fcbDefaultFont->setCurrentFont(QFont(settings->fontName()));
    connect(ui->fcbDefaultFont, &QFontComboBox::currentFontChanged, this,
            [this](const QFont &f) { this->settings->setFontName(f.family()); });
    connect(settings, &ApplicationSettings::fontNameChanged, this,
            [this](const QString &fontName) { ui->fcbDefaultFont->setCurrentFont(QFont(fontName)); });

    ui->spbDefaultFontSize->setValue(settings->fontSize());
    connect(ui->spbDefaultFontSize, QOverload<int>::of(&QSpinBox::valueChanged), settings,
            &ApplicationSettings::setFontSize);
    connect(settings, &ApplicationSettings::fontSizeChanged, ui->spbDefaultFontSize, &QSpinBox::setValue);

    ui->comboBoxLineEndings->setView(new QListView(ui->comboBoxLineEndings));
    ui->comboBoxLineEndings->addItem(tr("System Default"), QString(""));
    ui->comboBoxLineEndings->addItem(tr("Windows (CR LF)"), ScintillaNext::eolModeToString(SC_EOL_CRLF));
    ui->comboBoxLineEndings->addItem(tr("Linux (LF)"), ScintillaNext::eolModeToString(SC_EOL_LF));
    ui->comboBoxLineEndings->addItem(tr("Macintosh (CR)"), ScintillaNext::eolModeToString(SC_EOL_CR));

    // Select the current one
    const int index = ui->comboBoxLineEndings->findData(settings->defaultEOLMode());
    ui->comboBoxLineEndings->setCurrentIndex(index == -1 ? 0 : index);

    connect(ui->comboBoxLineEndings, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this](int idx) { this->settings->setDefaultEOLMode(ui->comboBoxLineEndings->itemData(idx).toString()); });
    connect(settings, &ApplicationSettings::defaultEOLModeChanged, this, [this](const QString &defaultEOLMode) {
        const int idx = ui->comboBoxLineEndings->findData(defaultEOLMode);
        ui->comboBoxLineEndings->setCurrentIndex(idx == -1 ? 0 : idx);
    });

    ui->comboBoxDefaultLanguage->setView(new QListView(ui->comboBoxDefaultLanguage));
    if (const auto *app = qobject_cast<SparkNotePadApplication *>(qApp); app != nullptr) {
        ui->comboBoxDefaultLanguage->addItems(app->getLanguages());
    }
    ui->comboBoxDefaultLanguage->setCurrentText(settings->defaultLanguage());

    connect(ui->comboBoxDefaultLanguage, &QComboBox::currentTextChanged, settings,
            &ApplicationSettings::setDefaultLanguage);
    connect(settings, &ApplicationSettings::defaultLanguageChanged, ui->comboBoxDefaultLanguage,
            &QComboBox::setCurrentText);

    MapSettingToCheckBox(ui->checkBoxHighlightURLs, &ApplicationSettings::urlHighlighting,
                         &ApplicationSettings::setURLHighlighting, &ApplicationSettings::urlHighlightingChanged);
    ui->spinBoxUrlThreshold->setValue(settings->urlThreshold());
    connect(ui->spinBoxUrlThreshold, &QSpinBox::valueChanged, settings, &ApplicationSettings::setUrlThreshold);
    connect(settings, &ApplicationSettings::urlThresholdChanged, ui->spinBoxUrlThreshold, &QSpinBox::setValue);

    ui->spinBoxExtraLineAscent->setValue(settings->extraLineAscent());
    connect(ui->spinBoxExtraLineAscent, &QSpinBox::valueChanged, settings, &ApplicationSettings::setExtraLineAscent);
    connect(settings, &ApplicationSettings::extraLineAscentChanged, ui->spinBoxExtraLineAscent, &QSpinBox::setValue);

    ui->spinBoxExtraLineDescent->setValue(settings->extraLineDescent());
    connect(ui->spinBoxExtraLineDescent, &QSpinBox::valueChanged, settings, &ApplicationSettings::setExtraLineDescent);
    connect(settings, &ApplicationSettings::extraLineDescentChanged, ui->spinBoxExtraLineDescent, &QSpinBox::setValue);
    MapSettingToCheckBox(ui->checkBoxShowLineNumbers, &ApplicationSettings::showLineNumbers,
                         &ApplicationSettings::setShowLineNumbers, &ApplicationSettings::showLineNumbersChanged);
    MapSettingToCheckBox(ui->checkBoxShowChangeHistory, &ApplicationSettings::showChangeHistory,
                         &ApplicationSettings::setShowChangeHistory, &ApplicationSettings::showChangeHistoryChanged);
    MapSettingToCheckBox(ui->checkBoxAutoCompletion, &ApplicationSettings::autoCompletion,
                         &ApplicationSettings::setAutoCompletion, &ApplicationSettings::autoCompletionChanged);
    MapSettingToCheckBox(ui->checkBoxAutoCloseBrackets, &ApplicationSettings::autoCloseBrackets,
                         &ApplicationSettings::setAutoCloseBrackets, &ApplicationSettings::autoCloseBracketsChanged);
    MapSettingToCheckBox(ui->checkBoxAutoCloseQuotes, &ApplicationSettings::autoCloseQuotes,
                         &ApplicationSettings::setAutoCloseQuotes, &ApplicationSettings::autoCloseQuotesChanged);
    MapSettingToCheckBox(ui->checkBoxSmartAutoIndent, &ApplicationSettings::smartAutoIndent,
                         &ApplicationSettings::setSmartAutoIndent, &ApplicationSettings::smartAutoIndentChanged);
    MapSettingToCheckBox(ui->checkBoxColorPreviewCallTip, &ApplicationSettings::colorPreviewCallTip,
                         &ApplicationSettings::setColorPreviewCallTip,
                         &ApplicationSettings::colorPreviewCallTipChanged);
    MapSettingToCheckBox(ui->checkBoxPreserveFileTime, &ApplicationSettings::preserveFileTime,
                         &ApplicationSettings::setPreserveFileTime, &ApplicationSettings::preserveFileTimeChanged);
    MapSettingToCheckBox(ui->checkBoxStrictOfflineMode, &ApplicationSettings::strictOfflineMode,
                         &ApplicationSettings::setStrictOfflineMode, &ApplicationSettings::strictOfflineModeChanged);

    auto *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->radioFollowCurrentDirectory,
                           static_cast<int>(ApplicationSettings::DefaultDirectoryBehaviorEnum::FollowCurrentDocument));
    buttonGroup->addButton(ui->radioLastUsedDirectory,
                           static_cast<int>(ApplicationSettings::DefaultDirectoryBehaviorEnum::RememberLastUsed));
    buttonGroup->addButton(ui->radioHardCoded,
                           static_cast<int>(ApplicationSettings::DefaultDirectoryBehaviorEnum::HardCoded));

    connect(buttonGroup, &QButtonGroup::idClicked, this, [this](int id) {
        const auto e = static_cast<ApplicationSettings::DefaultDirectoryBehaviorEnum>(id);
        this->settings->setDefaultDirectoryBehavior(e);
    });

    connect(ui->radioHardCoded, &QRadioButton::toggled, this, [this](bool checked) {
        ui->btnSelectHardCodedPath->setEnabled(checked);
        ui->txtHardCodedPath->setEnabled(checked);
    });

    connect(ui->btnSelectHardCodedPath, &QToolButton::clicked, this, [this] {
        const QString dir = QFileDialog::getExistingDirectory(this, tr("Default Directory"));
        if (dir.isEmpty()) {
            return;  // user cancelled
        }

        this->settings->setDefaultDirectory(QDir::fromNativeSeparators(dir));
        ui->txtHardCodedPath->setText(QDir::toNativeSeparators(dir));
    });

    connect(ui->txtHardCodedPath, &QLineEdit::editingFinished, this, [this] {
        const QString dir = ui->txtHardCodedPath->text();
        this->settings->setDefaultDirectory(QDir::fromNativeSeparators(dir));
        ui->txtHardCodedPath->setText(QDir::toNativeSeparators(dir));
    });

    if (auto *b = buttonGroup->button(static_cast<int>(settings->defaultDirectoryBehavior()))) {
        b->setChecked(true);
    }

    if (settings->defaultDirectoryBehavior() == ApplicationSettings::DefaultDirectoryBehaviorEnum::HardCoded) {
        ui->txtHardCodedPath->setText(QDir::toNativeSeparators(settings->defaultDirectory()));
    } else {
        ui->txtHardCodedPath->setText(QString());
    }
}

PreferencesDialog::~PreferencesDialog() = default;

void PreferencesDialog::showApplicationRestartRequired() const {
    ui->labelAppRestartIcon->show();
    ui->labelAppRestart->show();
}

void PreferencesDialog::openShortcutsTab() {
    if (m_tabWidget && m_shortcutWidget) {
        m_tabWidget->setCurrentWidget(m_shortcutWidget);
    }
}

void PreferencesDialog::openThemesTab() {
    if (m_tabWidget && m_themesAndToolbarWidget) {
        m_tabWidget->setCurrentWidget(m_themesAndToolbarWidget);
    }
}

void PreferencesDialog::openToolbarCustomizerTab() {
    if (m_tabWidget && m_toolbarCustomizerWidget) {
        m_tabWidget->setCurrentWidget(m_toolbarCustomizerWidget);
    }
}

void PreferencesDialog::setCurrentTab(int index) {
    if (m_tabWidget && index >= 0 && index < m_tabWidget->count()) {
        m_tabWidget->setCurrentIndex(index);
    }
}

void PreferencesDialog::accept() {
    if (m_shortcutWidget) {
        m_shortcutWidget->saveShortcuts();
        emit shortcutsChanged();
    }
    if (m_toolbarCustomizerWidget) {
        m_toolbarCustomizerWidget->saveToolbar();
        emit toolbarChanged();
    }
    QDialog::accept();
}

template <typename Func1, typename Func2, typename Func3>
void PreferencesDialog::MapSettingToCheckBox(QCheckBox *checkBox, Func1 getter, Func2 setter, Func3 notifier) const {
    checkBox->setChecked(std::invoke(getter, settings));
    connect(settings, notifier, checkBox, &QCheckBox::setChecked);
    connect(checkBox, &QCheckBox::toggled, settings, setter);
}

template <typename Func1, typename Func2, typename Func3>
void PreferencesDialog::MapSettingToGroupBox(QGroupBox *groupBox, Func1 getter, Func2 setter, Func3 notifier) const {
    groupBox->setChecked(std::invoke(getter, settings));
    connect(settings, notifier, groupBox, &QGroupBox::setChecked);
    connect(groupBox, &QGroupBox::toggled, settings, setter);
}

void PreferencesDialog::populateTranslationComboBox() {
    const auto *app = qobject_cast<SparkNotePadApplication *>(qApp);
    if (!app) {
        return;
    }

    ui->comboBoxTranslation->addItem(tr("<System Default>"), QStringLiteral(""));

    QList<std::pair<QString, QString>> translations;
    for (const auto &localeName : app->getTranslationManager()->availableTranslations()) {
        QLocale locale(localeName);
        translations.emplace_back(TranslationManager::FormatLocaleTerritoryAndLanguage(locale), localeName);
    }

    std::ranges::sort(translations,
                      [](const auto &a, const auto &b) { return a.first.localeAwareCompare(b.first) < 0; });

    for (const auto &[display, localeName] : translations) {
        ui->comboBoxTranslation->addItem(display, localeName);
    }

    const int index = ui->comboBoxTranslation->findData(settings->translation());
    if (index != -1) {
        ui->comboBoxTranslation->setCurrentIndex(index);
    }
}

void PreferencesDialog::setupThemesGroup(QWidget *container) {
    auto *gbxThemes =
        new QGroupBox(tr("Themes & Visual Styling").replace(QLatin1Char('&'), QStringLiteral("&&")), container);
    auto *themeLayout = new QFormLayout(gbxThemes);
    themeLayout->setSpacing(10);
    themeLayout->setContentsMargins(14, 16, 14, 14);

    // Active Theme Palette Selector
    comboBoxTheme = new QComboBox(gbxThemes);
    comboBoxTheme->setMaxVisibleItems(14);
    if (comboBoxTheme->view()) {
        comboBoxTheme->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    int selectedIdx = 0;
    const QString currentId = ThemeManager::instance()->currentPaletteId();
    QString lastFamily;
    for (const auto &pal : kAllThemePalettes) {
        const QString family = QString::fromUtf8(pal.family);
        if (family != lastFamily) {
            if (!lastFamily.isEmpty()) {
                comboBoxTheme->insertSeparator(comboBoxTheme->count());
            }
            lastFamily = family;
        }
        const QString label =
            QStringLiteral("[%1] %2 (%3)")
                .arg(family, QString::fromUtf8(pal.displayName), pal.isDark ? tr("Dark") : tr("Light"));
        comboBoxTheme->addItem(label, QString::fromLatin1(pal.id));
        if (pal.id == currentId) {
            selectedIdx = comboBoxTheme->count() - 1;
        }
    }
    comboBoxTheme->setCurrentIndex(selectedIdx);
    connect(comboBoxTheme, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index < 0) {
            return;
        }
        const QString themeId = comboBoxTheme->itemData(index).toString();
        if (themeId.isEmpty()) {
            return;
        }
        MainWindow *mw = qobject_cast<MainWindow *>(parentWidget());
        ThemeManager::instance()->setPalette(themeId, mw);
    });

    auto *themeRowLayout = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    themeRowLayout->addWidget(comboBoxTheme, 1);

    btnToggleDarkLight = new QPushButton(tr("Toggle Dark/Light"), gbxThemes);
    btnToggleDarkLight->setToolTip(tr("Quickly switch between active dark and light modes"));
    connect(btnToggleDarkLight, &QPushButton::clicked, this, [this] {
        MainWindow *mw = qobject_cast<MainWindow *>(parentWidget());
        ThemeManager::instance()->toggleTheme(mw);
        const QString curId = ThemeManager::instance()->currentPaletteId();
        const int found = comboBoxTheme->findData(curId);
        if (found >= 0) {
            const QSignalBlocker blocker(comboBoxTheme);
            comboBoxTheme->setCurrentIndex(found);
        }
    });
    themeRowLayout->addWidget(btnToggleDarkLight);
    themeLayout->addRow(tr("Active Theme:"), themeRowLayout);

    // Window Opacity / Transparency
    auto *opacityLayout = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    sliderWindowOpacity = new QSlider(Qt::Horizontal, gbxThemes);
    sliderWindowOpacity->setRange(20, 100);
    spinBoxWindowOpacity = new QSpinBox(gbxThemes);
    spinBoxWindowOpacity->setRange(20, 100);
    spinBoxWindowOpacity->setSuffix(QStringLiteral("%"));

    const int currentOpacity = settings->value("Window/Opacity", 100).toInt();
    sliderWindowOpacity->setValue(currentOpacity);
    spinBoxWindowOpacity->setValue(currentOpacity);

    connect(sliderWindowOpacity, &QSlider::valueChanged, spinBoxWindowOpacity, &QSpinBox::setValue);
    connect(spinBoxWindowOpacity, QOverload<int>::of(&QSpinBox::valueChanged), sliderWindowOpacity, &QSlider::setValue);
    connect(sliderWindowOpacity, &QSlider::valueChanged, this, [this](int val) {
        settings->setValue("Window/Opacity", val);
        if (QWidget *topWin = this->window()) {
            topWin->setWindowOpacity(val / 100.0);
        }
        if (QWidget *parentWin = this->parentWidget()) {
            parentWin->setWindowOpacity(val / 100.0);
        }
    });

    opacityLayout->addWidget(sliderWindowOpacity, 1);
    opacityLayout->addWidget(spinBoxWindowOpacity);
    themeLayout->addRow(tr("Window Opacity:"), opacityLayout);

    // Tail -f Monitoring Debounce Interval
    spinBoxTailDebounce = new QSpinBox(gbxThemes);
    spinBoxTailDebounce->setRange(100, 5000);
    spinBoxTailDebounce->setSingleStep(100);
    spinBoxTailDebounce->setSuffix(tr(" ms"));
    const int currentDebounce = settings->value("TailMonitoring/DebounceMs", 500).toInt();
    spinBoxTailDebounce->setValue(currentDebounce);
    connect(spinBoxTailDebounce, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [this](int val) { settings->setValue("TailMonitoring/DebounceMs", val); });
    themeLayout->addRow(tr("Tail -f Reload Interval:"), spinBoxTailDebounce);

    // Multi-Monitor Boundary Safety
    checkBoxMultiMonitorSafety =
        new QCheckBox(tr("Guard windows against offscreen placement across multi-DPI displays"), gbxThemes);
    const bool currentSafety = settings->value("Screen/SafetyGuards", true).toBool();
    checkBoxMultiMonitorSafety->setChecked(currentSafety);
    connect(checkBoxMultiMonitorSafety, &QCheckBox::toggled, this,
            [this](bool checked) { settings->setValue("Screen/SafetyGuards", checked); });
    themeLayout->addRow(tr("Display Safety:"), checkBoxMultiMonitorSafety);

    container->layout()->addWidget(gbxThemes);
}

void PreferencesDialog::setupToolbarPacksGroup(QWidget *container) {
    auto *gbxToolbarPacks =
        new QGroupBox(tr("Toolbar Icon Packs & Icon Size").replace(QLatin1Char('&'), QStringLiteral("&&")), container);
    auto *toolbarLayout = new QFormLayout(gbxToolbarPacks);
    toolbarLayout->setVerticalSpacing(12);
    toolbarLayout->setHorizontalSpacing(14);
    toolbarLayout->setContentsMargins(14, 16, 14, 14);

    // Active Icon Pack Selector
    comboBoxIconPack = new QComboBox(gbxToolbarPacks);
    populateIconPacksComboBox();
    toolbarLayout->addRow(tr("Active Icon Pack:"), comboBoxIconPack);

    // Description Card
    auto *lblDescTitle = new QLabel(tr("Description:"), gbxToolbarPacks);
    lblDescTitle->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    lblIconPackDescription = new QLabel(gbxToolbarPacks);
    lblIconPackDescription->setObjectName(QStringLiteral("lblIconPackDescription"));
    lblIconPackDescription->setWordWrap(true);
    lblIconPackDescription->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    lblIconPackDescription->setMinimumHeight(48);
    if (const auto *curPack =
            ToolbarIconManager::instance()->findPackById(ToolbarIconManager::instance()->currentPackId());
        curPack) {
        lblIconPackDescription->setText(curPack->description);
    }
    toolbarLayout->addRow(lblDescTitle, lblIconPackDescription);

    connect(comboBoxIconPack, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        if (idx < 0) {
            return;
        }
        const QString packId = comboBoxIconPack->itemData(idx).toString();
        if (packId.isEmpty()) {
            return;
        }
        const auto *mw = qobject_cast<const MainWindow *>(parentWidget());
        QToolBar *tb = mw ? mw->findChild<QToolBar *>() : nullptr;
        ToolbarIconManager::instance()->setPack(packId, tb);
        if (const auto *pack = ToolbarIconManager::instance()->findPackById(packId); pack && lblIconPackDescription) {
            const QString desc = pack->description.trimmed().isEmpty()
                                     ? tr("No description provided for this icon pack.")
                                     : pack->description;
            lblIconPackDescription->setText(desc);
        }
        updateIconPreview();
    });

    // Icon Size
    comboBoxIconSize = new QComboBox(gbxToolbarPacks);
    struct SizeEntry {
        QString label;
        QSize size;
    };
    const std::array<SizeEntry, 6> sizes = {{
        {tr("16 x 16 (Compact)"), QSize(16, 16)},
        {tr("20 x 20 (Medium)"), QSize(20, 20)},
        {tr("24 x 24 (Default / Recommended)"), QSize(24, 24)},
        {tr("32 x 32 (Large / High-DPI)"), QSize(32, 32)},
        {tr("48 x 48 (Extra Large)"), QSize(48, 48)},
        {tr("64 x 64 (Ultra High-DPI)"), QSize(64, 64)},
    }};
    const QSize currentSize = ToolbarIconManager::instance()->iconSize();
    int sizeIdx = 2;
    for (size_t i = 0; i < sizes.size(); ++i) {
        comboBoxIconSize->addItem(sizes[i].label, QVariant(sizes[i].size));
        if (sizes[i].size == currentSize) {
            sizeIdx = static_cast<int>(i);
        }
    }
    comboBoxIconSize->setCurrentIndex(sizeIdx);
    connect(comboBoxIconSize, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        if (idx < 0) {
            return;
        }
        const QSize size = comboBoxIconSize->itemData(idx).toSize();
        const auto *mw = qobject_cast<const MainWindow *>(parentWidget());
        QToolBar *tb = mw ? mw->findChild<QToolBar *>() : nullptr;
        ToolbarIconManager::instance()->setIconSize(size, tb);
    });
    toolbarLayout->addRow(tr("Toolbar Icon Size:"), comboBoxIconSize);

    // Pack Preview Row
    widgetIconPreview = new QWidget(gbxToolbarPacks);
    layoutIconPreview = new QHBoxLayout(widgetIconPreview);  // NOSONAR(cpp:S5025)
    layoutIconPreview->setContentsMargins(4, 4, 4, 4);
    layoutIconPreview->setSpacing(8);
    updateIconPreview();
    toolbarLayout->addRow(tr("Icon Preview:"), widgetIconPreview);

    // Buttons: Open Custom Icon Packs & Rescan
    auto *btnRowLayout = new QHBoxLayout();  // NOSONAR(cpp:S5025)
    auto *btnOpenCustomPacks = new QPushButton(tr("Open Custom Icon Packs Folder..."), gbxToolbarPacks);
    btnOpenCustomPacks->setIcon(QIcon(QStringLiteral(":/icons/openFile.svg")));
    connect(btnOpenCustomPacks, &QPushButton::clicked, this, [] {
        const QString path = ToolbarIconManager::customPacksPath();
        QDir().mkpath(path);
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    });
    btnRowLayout->addWidget(btnOpenCustomPacks);

    auto *btnRescan = new QPushButton(tr("Rescan Icon Packs"), gbxToolbarPacks);
    btnRescan->setIcon(QIcon(QStringLiteral(":/icons/arrow_refresh.svg")));
    connect(btnRescan, &QPushButton::clicked, this, [this] {
        ToolbarIconManager::instance()->scanCustomPacks();
        populateIconPacksComboBox();
        updateIconPreview();
    });
    btnRowLayout->addWidget(btnRescan);
    btnRowLayout->addStretch();

    toolbarLayout->addRow(tr("Management:"), btnRowLayout);
    container->layout()->addWidget(gbxToolbarPacks);
}

void PreferencesDialog::setupThemesAndToolbarTab() {
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto *container = new QWidget(scrollArea);
    auto *mainLayout = new QVBoxLayout(container);
    mainLayout->setSpacing(14);
    mainLayout->setContentsMargins(14, 16, 14, 14);

    setupThemesGroup(container);
    setupToolbarPacksGroup(container);
    mainLayout->addStretch();

    scrollArea->setWidget(container);
    m_themesAndToolbarWidget = scrollArea;
    m_tabWidget->addTab(m_themesAndToolbarWidget, QIcon(QStringLiteral(":/icons/view.svg")), tr("&Themes && Toolbar"));
}

void PreferencesDialog::setupCustomToolbarTab(QMainWindow *mainWindow) {
    auto *mw = qobject_cast<MainWindow *>(mainWindow);
    if (!mw) {
        mw = qobject_cast<MainWindow *>(parentWidget());
    }
    m_toolbarCustomizerWidget = new CustomizeToolbarWidget(mw, this);
    connect(m_toolbarCustomizerWidget, &CustomizeToolbarWidget::toolbarChanged, this,
            &PreferencesDialog::toolbarChanged);
    m_tabWidget->addTab(m_toolbarCustomizerWidget, QIcon(QStringLiteral(":/icons/table_edit.svg")),
                        tr("&Custom Toolbar Config"));
}

void PreferencesDialog::populateIconPacksComboBox() {
    if (!comboBoxIconPack) {
        return;
    }
    const QSignalBlocker blocker(comboBoxIconPack);
    comboBoxIconPack->clear();
    const auto &packs = ToolbarIconManager::instance()->availablePacks();
    const QString currentPack = ToolbarIconManager::instance()->currentPackId();
    int currentIdx = 0;
    for (int i = 0; i < packs.size(); ++i) {
        const auto &pack = packs[i];
        QString label = pack.displayName;
        if (!pack.isBuiltIn) {
            label += tr(" [Custom]");
        }
        comboBoxIconPack->addItem(label, pack.id);
        if (pack.id == currentPack) {
            currentIdx = i;
        }
    }
    comboBoxIconPack->setCurrentIndex(currentIdx);
}

void PreferencesDialog::updateIconPreview() {
    if (!layoutIconPreview || !widgetIconPreview) {
        return;
    }

    const QStringList sampleActions = {QStringLiteral("New"),  QStringLiteral("Open"), QStringLiteral("Save"),
                                       QStringLiteral("Cut"),  QStringLiteral("Copy"), QStringLiteral("Paste"),
                                       QStringLiteral("Find"), QStringLiteral("Undo"), QStringLiteral("Redo")};

    if (m_previewLabels.isEmpty()) {
        for (const QString &actName : sampleActions) {
            auto *lbl = new QLabel(widgetIconPreview);
            lbl->setFixedSize(32, 32);
            lbl->setAlignment(Qt::AlignCenter);
            lbl->setStyleSheet(
                QStringLiteral("background-color: rgba(128,128,128,0.12); border-radius: 4px; padding: 2px;"));
            lbl->setToolTip(actName);
            layoutIconPreview->addWidget(lbl);
            m_previewLabels.append(lbl);
        }
        layoutIconPreview->addStretch();
    }

    for (int i = 0; i < sampleActions.size() && i < m_previewLabels.size(); ++i) {
        const QIcon icon = ToolbarIconManager::instance()->actionIcon(sampleActions[i]);
        m_previewLabels[i]->setPixmap(icon.pixmap(22, 22));
    }
}

void PreferencesDialog::updateThemeColors() {
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    const bool dark = ThemeManager::instance()->isDarkActive();

    if (comboBoxTheme) {
        const QString curId = ThemeManager::instance()->currentPaletteId();
        const int found = comboBoxTheme->findData(curId);
        if (found >= 0 && comboBoxTheme->currentIndex() != found) {
            const QSignalBlocker blocker(comboBoxTheme);
            comboBoxTheme->setCurrentIndex(found);
        }
    }

    const QString defaultWinBg = dark ? QStringLiteral("#0f172a") : QStringLiteral("#f8fafc");
    const QString defaultCardBg = dark ? QStringLiteral("#1e293b") : QStringLiteral("#ffffff");
    const QString defaultBorderCol = dark ? QStringLiteral("#334155") : QStringLiteral("#cbd5e1");
    const QString defaultAccentCol = dark ? QStringLiteral("#38bdf8") : QStringLiteral("#2563eb");
    const QString defaultTxtPri = dark ? QStringLiteral("#f8fafc") : QStringLiteral("#0f172a");
    const QString defaultTxtSec = dark ? QStringLiteral("#94a3b8") : QStringLiteral("#475569");

    const QString winBg = pal ? QString::fromLatin1(pal->win) : defaultWinBg;
    const QString cardBg = pal ? QString::fromLatin1(pal->card) : defaultCardBg;
    const QString borderCol = pal ? QString::fromLatin1(pal->border) : defaultBorderCol;
    const QString accentCol = pal ? QString::fromLatin1(pal->blue) : defaultAccentCol;
    const QString txtPri = pal ? QString::fromLatin1(pal->txtPri) : defaultTxtPri;
    const QString txtSec = pal ? QString::fromLatin1(pal->txtSec) : defaultTxtSec;

    const QString prefStyle =
        QString(
            "QDialog {"
            "  background-color: %1;"
            "  color: %4;"
            "  font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', 'Segoe UI', Roboto, sans-serif;"
            "  font-size: 13px;"
            "}"
            "QScrollArea {"
            "  background-color: %1;"
            "  border: none;"
            "}"
            "QWidget#scrollAreaWidgetContents {"
            "  background-color: %1;"
            "}"
            "QGroupBox {"
            "  background-color: %2;"
            "  border: 1px solid %3;"
            "  border-left: 5px solid %6;"
            "  border-radius: 8px;"
            "  margin-top: 14px;"
            "  padding: 16px;"
            "  font-weight: 600;"
            "  color: %4;"
            "}"
            "QGroupBox::title {"
            "  subcontrol-origin: margin;"
            "  subcontrol-position: top left;"
            "  padding: 2px 8px;"
            "  color: %4;"
            "  font-weight: bold;"
            "}"
            "QLabel {"
            "  color: %4;"
            "  font-size: 13px;"
            "}"
            "QLabel#lblIconPackDescription {"
            "  color: %5;"
            "  font-size: 12px;"
            "  padding: 4px 8px;"
            "  background-color: rgba(148, 163, 184, 0.10);"
            "  border: 1px solid %3;"
            "  border-left: 3px solid %6;"
            "  border-radius: 6px;"
            "}"
            "QCheckBox, QRadioButton {"
            "  color: %4;"
            "  spacing: 8px;"
            "  font-size: 13px;"
            "  background-color: transparent;"
            "}"
            "QCheckBox:disabled, QRadioButton:disabled {"
            "  color: %5;"
            "}"
            "QCheckBox::indicator, QRadioButton::indicator {"
            "  width: 15px;"
            "  height: 15px;"
            "  border: 1px solid %3;"
            "  background-color: %2;"
            "}"
            "QCheckBox::indicator {"
            "  border-radius: 3px;"
            "}"
            "QRadioButton::indicator {"
            "  border-radius: 8px;"
            "}"
            "QCheckBox::indicator:hover, QRadioButton::indicator:hover {"
            "  border-color: %6;"
            "}"
            "QCheckBox::indicator:checked, QRadioButton::indicator:checked {"
            "  background-color: %6;"
            "  border-color: %6;"
            "}"
            "QCheckBox::indicator:disabled, QRadioButton::indicator:disabled {"
            "  border-color: %3;"
            "  background-color: %1;"
            "}"
            "QComboBox, QFontComboBox, QSpinBox, QLineEdit {"
            "  background-color: %2;"
            "  color: %4;"
            "  border: 1px solid %3;"
            "  border-radius: 6px;"
            "  padding: 6px 10px;"
            "  font-size: 13px;"
            "}"
            "QComboBox:focus, QFontComboBox:focus, QSpinBox:focus, QLineEdit:focus {"
            "  border-color: %6;"
            "}"
            "QComboBox::drop-down {"
            "  subcontrol-origin: padding;"
            "  subcontrol-position: top right;"
            "  width: 24px;"
            "  border-left: 1px solid %3;"
            "  border-top-right-radius: 6px;"
            "  border-bottom-right-radius: 6px;"
            "  background-color: %1;"
            "}"
            "QComboBox::down-arrow {"
            "  width: 0px;"
            "  height: 0px;"
            "  border-left: 4px solid transparent;"
            "  border-right: 4px solid transparent;"
            "  border-top: 5px solid %4;"
            "  margin: 0 auto;"
            "}"
            "QPushButton {"
            "  background-color: %2;"
            "  color: %4;"
            "  border: 1px solid %3;"
            "  border-radius: 6px;"
            "  padding: 6px 14px;"
            "  font-size: 13px;"
            "  font-weight: 500;"
            "}"
            "QPushButton:hover {"
            "  background-color: %6;"
            "  color: #ffffff;"
            "  border-color: %6;"
            "}"
            "QPushButton:pressed {"
            "  background-color: %6;"
            "  border-color: %6;"
            "}"
            "QSlider::groove:horizontal {"
            "  border: 1px solid %3;"
            "  height: 6px;"
            "  background: %2;"
            "  margin: 2px 0;"
            "  border-radius: 3px;"
            "}"
            "QSlider::handle:horizontal {"
            "  background: %6;"
            "  border: 1px solid %6;"
            "  width: 16px;"
            "  margin: -6px 0;"
            "  border-radius: 8px;"
            "}"
            "QTabWidget::pane {"
            "  border: 1px solid %3;"
            "  background-color: %1;"
            "  border-radius: 6px;"
            "}"
            "QTabBar::tab {"
            "  background: %2;"
            "  color: %5;"
            "  border: 1px solid %3;"
            "  border-bottom: none;"
            "  padding: 8px 16px;"
            "  margin-right: 4px;"
            "  border-top-left-radius: 6px;"
            "  border-top-right-radius: 6px;"
            "  font-weight: 500;"
            "}"
            "QTabBar::tab:selected {"
            "  background: %1;"
            "  color: %4;"
            "  border-color: %3;"
            "  border-bottom: 2px solid %6;"
            "  font-weight: 600;"
            "}"
            "QTabBar::tab:hover:!selected {"
            "  background: %2;"
            "  color: %4;"
            "}")
            .arg(winBg, cardBg, borderCol, txtPri, txtSec, accentCol);

    setStyleSheet(prefStyle);
    if (m_shortcutWidget) {
        m_shortcutWidget->applyThemeStyles();
    }
    updateIconPreview();
}
