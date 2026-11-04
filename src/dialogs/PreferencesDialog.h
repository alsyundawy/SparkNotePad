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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "ApplicationSettings.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

#include <memory>

namespace Ui {
class PreferencesDialog;
}

class Settings;
class QTabWidget;
class ShortcutCustomizerWidget;
class CustomizeToolbarWidget;
class QLabel;
class QHBoxLayout;
class QMainWindow;

class PreferencesDialog : public QDialog {
    Q_OBJECT

public:
    explicit PreferencesDialog(ApplicationSettings *settings, QWidget *parent = nullptr);
    ~PreferencesDialog() override;

    void showApplicationRestartRequired() const;
    void openShortcutsTab();
    void openThemesTab();
    void openToolbarCustomizerTab();
    void setCurrentTab(int index);
    void accept() override;

signals:
    void shortcutsChanged();
    void toolbarChanged();

private:
    std::unique_ptr<Ui::PreferencesDialog> ui;
    ApplicationSettings *settings;

    template <typename Func1, typename Func2, typename Func3>
    void MapSettingToCheckBox(QCheckBox *checkBox, Func1 getter, Func2 setter, Func3 notifier) const;

    template <typename Func1, typename Func2, typename Func3>
    void MapSettingToGroupBox(QGroupBox *groupBox, Func1 getter, Func2 setter, Func3 notifier) const;

    void populateTranslationComboBox();
    void updateThemeColors();
    void setupThemesAndToolbarTab();
    void setupThemesGroup(QWidget *container);
    void setupToolbarPacksGroup(QWidget *container);
    void setupCustomToolbarTab(QMainWindow *mainWindow);
    void populateIconPacksComboBox();
    void updateIconPreview();

    QTabWidget *m_tabWidget = nullptr;
    ShortcutCustomizerWidget *m_shortcutWidget = nullptr;
    QWidget *m_themesAndToolbarWidget = nullptr;
    CustomizeToolbarWidget *m_toolbarCustomizerWidget = nullptr;

    // Themes controls
    QComboBox *comboBoxTheme = nullptr;
    QPushButton *btnToggleDarkLight = nullptr;
    QSlider *sliderWindowOpacity = nullptr;
    QSpinBox *spinBoxWindowOpacity = nullptr;
    QSpinBox *spinBoxTailDebounce = nullptr;
    QCheckBox *checkBoxMultiMonitorSafety = nullptr;

    // Toolbar Icon Packs controls
    QComboBox *comboBoxIconPack = nullptr;
    QLabel *lblIconPackDescription = nullptr;
    QComboBox *comboBoxIconSize = nullptr;
    QHBoxLayout *layoutIconPreview = nullptr;
    QWidget *widgetIconPreview = nullptr;
    QList<QLabel *> m_previewLabels;
};

#endif  // PREFERENCESDIALOG_H
