#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <QDockWidget>
#include <cstdint>
#include "features/CharacterInfo.h"

class QLabel;
class QTableWidget;
class QPushButton;
class ScintillaNext;

class CharacterInfoDock : public QDockWidget {
    Q_OBJECT

public:
    explicit CharacterInfoDock(QWidget *parent = nullptr);
    ~CharacterInfoDock() override = default;

    void updateCharacter(uint32_t codePoint);
    void updateFromEditor(ScintillaNext *editor);

    [[nodiscard]] uint32_t currentCodePoint() const;
    [[nodiscard]] const QString &characterName() const;

private slots:
    void onCopyGlyphClicked() const;
    void onCopyCodePointClicked() const;
    void onCopyUtf8Clicked() const;
    void onCopyHtmlClicked() const;

private:
    void setupUi();
    void populateDetails(const CharacterInfo::Details &details);

    CharacterInfo::Details m_details;
    QLabel *m_lblGlyph{nullptr};
    QLabel *m_lblCodePoint{nullptr};
    QLabel *m_lblName{nullptr};
    QLabel *m_lblCategory{nullptr};
    QTableWidget *m_tableDetails{nullptr};

    QPushButton *m_btnCopyGlyph{nullptr};
    QPushButton *m_btnCopyCodePoint{nullptr};
    QPushButton *m_btnCopyUtf8{nullptr};
    QPushButton *m_btnCopyHtml{nullptr};
};
