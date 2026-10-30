#pragma once
/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef MODERNMESSAGEBOX_H
#define MODERNMESSAGEBOX_H

#include <cstdint>
#include <QDialog>
#include <QString>
#include <QVector>

class ScintillaNext;
class QLabel;
class QPushButton;

class ModernMessageBox : public QDialog {
    Q_OBJECT

public:
    enum class IconType : std::uint8_t { Question, Information, Warning, Critical };

    enum class ActionChoice : std::uint8_t { SaveOrAccept, Discard, Cancel };

    explicit ModernMessageBox(QWidget *parent = nullptr, IconType iconType = IconType::Question,
                              const QString &title = QString(), const QString &message = QString(),
                              const QString &detail = QString());
    ~ModernMessageBox() override = default;

    void addButton(const QString &text, ActionChoice role, bool isDefault = false);
    ActionChoice chosenAction() const { return m_chosenAction; }

    // Static Drop-in Helpers
    static ActionChoice promptForSave(QWidget *parent, const QVector<ScintillaNext *> &editors);
    static bool question(QWidget *parent, const QString &title, const QString &text, const QString &detail = QString());
    static void information(QWidget *parent, const QString &title, const QString &text,
                            const QString &detail = QString());
    static void warning(QWidget *parent, const QString &title, const QString &text, const QString &detail = QString());
    static void critical(QWidget *parent, const QString &title, const QString &text, const QString &detail = QString());

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupUi(IconType iconType, const QString &title, const QString &message, const QString &detail);
    QPixmap createIconBadge(IconType iconType) const;

    ActionChoice m_chosenAction = ActionChoice::Cancel;
    QList<QPushButton *> m_buttons;
};

#endif  // MODERNMESSAGEBOX_H
