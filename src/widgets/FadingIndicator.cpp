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

#include "FadingIndicator.h"

#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPointer>
#include <QPropertyAnimation>
#include <QTimer>

namespace Internal {

class FadingIndicatorPrivate : public QWidget {
    Q_OBJECT

public:
    FadingIndicatorPrivate(QWidget *parent, FadingIndicator::TextSize size)
        : QWidget(parent), m_effect(new QGraphicsOpacityEffect(this)), m_label(new QLabel) {
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        setGraphicsEffect(m_effect);
        m_effect->setOpacity(.999);

        QFont font = m_label->font();
        font.setPixelSize(size == FadingIndicator::LargeText ? 30 : 18);
        m_label->setFont(font);
        m_label->setStyleSheet(
            QStringLiteral("color: #ffffff; background: transparent; padding: 6px 14px; font-weight: bold;"));
        m_label->setAlignment(Qt::AlignCenter);
        auto *layout = new QHBoxLayout;
        setLayout(layout);
        layout->addWidget(m_label);
    }

    void setText(const QString &text) {
        m_pixmap = QPixmap();
        m_label->setText(text);
        m_effect->setOpacity(.6);  // because of the fat opaque background color
        layout()->setSizeConstraint(QLayout::SetFixedSize);
        adjustSize();
        const QWidget *parent = parentWidget();
        QPoint pos = parent ? (parent->rect().center() - rect().center()) : QPoint();
        if (pixmapIndicator && pixmapIndicator->geometry().intersects(QRect(pos, size())))
            pos.setY(pixmapIndicator->geometry().bottom() + 1);
        move(pos);
    }

    void setPixmap(const QString &uri) {
        m_label->hide();
        m_pixmap.load(uri);
        layout()->setSizeConstraint(QLayout::SetNoConstraint);
        resize(m_pixmap.size() / m_pixmap.devicePixelRatio());
        const QWidget *parent = parentWidget();
        QPoint pos = parent ? (parent->rect().center() - rect().center()) : QPoint();
        if (textIndicator && textIndicator->geometry().intersects(QRect(pos, size())))
            pos.setY(textIndicator->geometry().bottom() + 1);
        move(pos);
    }

    void run(int ms) {
        show();
        raise();
        QTimer::singleShot(ms, this, &FadingIndicatorPrivate::runInternal);
    }

    static QPointer<FadingIndicatorPrivate> textIndicator;
    static QPointer<FadingIndicatorPrivate> pixmapIndicator;

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        if (!m_pixmap.isNull()) {
            p.drawPixmap(rect(), m_pixmap);
        } else {
            p.setBrush(QColor(30, 30, 30, 220));
            p.setPen(QColor(255, 255, 255, 45));
            p.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 12, 12);
        }
    }

private:
    void runInternal() {
        auto *anim = new QPropertyAnimation(m_effect, "opacity", this);
        anim->setDuration(200);
        anim->setEndValue(0.);
        connect(anim, &QAbstractAnimation::finished, this, &QObject::deleteLater);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    QGraphicsOpacityEffect *m_effect;
    QLabel *m_label;
    QPixmap m_pixmap;
};

QPointer<FadingIndicatorPrivate> FadingIndicatorPrivate::textIndicator;
QPointer<FadingIndicatorPrivate> FadingIndicatorPrivate::pixmapIndicator;

}  // namespace Internal

namespace FadingIndicator {

void showText(QWidget *parent, const QString &text, TextSize size) {
    QPointer<Internal::FadingIndicatorPrivate> &indicator = Internal::FadingIndicatorPrivate::textIndicator;

    if (indicator)
        delete indicator.data();

    indicator = new Internal::FadingIndicatorPrivate(parent, size);
    indicator->setText(text);
    indicator->run(2500);  // deletes itself
}

void showPixmap(QWidget *parent, const QString &pixmap) {
    QPointer<Internal::FadingIndicatorPrivate> &indicator = Internal::FadingIndicatorPrivate::pixmapIndicator;

    if (indicator)
        delete indicator.data();

    indicator = new Internal::FadingIndicatorPrivate(parent, LargeText);
    indicator->setPixmap(pixmap);
    indicator->run(300);  // deletes itself
}

}  // namespace FadingIndicator

#include "FadingIndicator.moc"
