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

#include <QDebug>
#include <QSettings>
#include <QSysInfo>
#include <QApplication>
#include <QFont>
#include <QDataStream>

#include "NoteSciPadApplication.h"

int main(int argc, char *argv[]) {
    qSetMessagePattern(
        "[%{time process}] "
        "%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}: "
        "%{message}");

    // Set these since other parts of the app references these
    QApplication::setOrganizationName("NoteSciPad");
    QApplication::setApplicationName("NoteSciPad");
    QGuiApplication::setApplicationDisplayName("NoteSciPad");
    QGuiApplication::setApplicationVersion(APP_VERSION);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    // Default settings format
    QSettings::setDefaultFormat(QSettings::IniFormat);

    NoteSciPadApplication app(argc, argv);

    // Log some debug info
    qInfo("=============================");
    for (const auto &d : app.debugInfo()) {
        qInfo("%s", qUtf8Printable(d));
    }
    qInfo("=============================");

    // Establish universal modern GUI font fallback for cross-platform visual consistency
    QFont defaultGuiFont;
#if defined(Q_OS_MACOS)
    defaultGuiFont.setFamilies(
        {"-apple-system", "BlinkMacSystemFont", "SF Pro Text", "Helvetica Neue", "Arial", "sans-serif"});
    defaultGuiFont.setPointSize(12);
#elif defined(Q_OS_LINUX)
    defaultGuiFont.setFamilies(
        {"Inter", "Ubuntu", "Roboto", "Segoe UI", "DejaVu Sans", "Liberation Sans", "sans-serif"});
    defaultGuiFont.setPointSize(10);
#else
    defaultGuiFont.setFamilies({"Segoe UI", "Arial", "sans-serif"});
    defaultGuiFont.setPointSize(10);
#endif
    QApplication::setFont(defaultGuiFont);

    if (app.isPrimary()) {
        app.init();

        return app.exec();
    } else {
        qInfo() << "Primary instance already running. PID:" << app.primaryPid();

        app.sendInfoToPrimaryInstance();

        qInfo() << "Secondary instance closing...";

        app.exit(0);

        return 0;
    }
}
