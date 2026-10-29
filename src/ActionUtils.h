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

#pragma once

#include <QObject>
#include <QStringList>
#include <QAction>
#include <QApplication>
#include <QStyle>
#include <QPainter>

namespace ActionUtils {

template <typename Container>
void populateActionContainer(Container* container, QObject* context, const QStringList& actionNames) {
    for (const QString& actionName : actionNames) {
        if (actionName.isEmpty()) {
            container->addSeparator();
        } else {
            const QString fullObjectName = QStringLiteral("action") + actionName;
            QAction* action = context->findChild<QAction*>(fullObjectName, Qt::FindDirectChildrenOnly);

            if (action) {
                action->setIconVisibleInMenu(true);
                container->addAction(action);
            } else {
                qWarning() << "Cannot locate action named" << fullObjectName;
            }
        }
    }
}

inline QIcon createSolidIcon(QColor color) {
    constexpr int iconSize = 16;
    const qreal dpr = qApp ? qApp->devicePixelRatio() : 2.0;
    const int physSize = static_cast<int>(iconSize * dpr);

    QPixmap pixmap(physSize, physSize);
    pixmap.setDevicePixelRatio(dpr);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(color);
    painter.setPen(QPen(QColor(0, 0, 0, 90), 1.0));
    painter.drawRoundedRect(QRectF(1.0, 1.0, iconSize - 2.0, iconSize - 2.0), 3.0, 3.0);
    painter.end();

    return QIcon(pixmap);
}

}  // namespace ActionUtils
