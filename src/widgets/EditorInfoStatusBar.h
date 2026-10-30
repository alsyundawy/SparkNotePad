#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad3 status bar customization architecture.
 * Sources: rizonesoft/Notepad3 (src/Notepad3.ini, Statusbar Settings)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef EDITORINFOSTATUSBAR_H
#define EDITORINFOSTATUSBAR_H

#include <QStatusBar>
#include <QMap>
#include "ScintillaTypes.h"

class QLabel;
class QMainWindow;
class ScintillaNext;

class EditorInfoStatusBar : public QStatusBar {
    Q_OBJECT

public:
    enum class Field : std::uint8_t {
        FieldLanguage = 0,
        FieldDocSize,
        FieldDocPos,
        FieldUnicodeChar,
        FieldOccurrences,
        FieldEol,
        FieldEncoding,
        FieldOverType,
        FieldCount
    };
    using enum Field;
    Q_ENUM(Field)

    explicit EditorInfoStatusBar(QMainWindow *window);

    void refresh(ScintillaNext *editor);
    void updateEncoding(const ScintillaNext *editor);

    void setFieldVisible(Field field, bool visible);
    bool isFieldVisible(Field field) const;

    void resetToDefaults();
    void showCustomizerDialog();

public slots:
    void setOccurrencesCount(const QString &query, int count);
    void clearOccurrencesCount();

signals:
    void customContextMenuRequestedForEOLLabel(const QPoint &pos);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void connectToEditor(ScintillaNext *editor);
    void editorUpdated(Scintilla::Update updated);

    void updateDocumentSize(const ScintillaNext *editor);
    void updateSelectionInfo(ScintillaNext *editor);
    void updateUnicodeChar(ScintillaNext *editor);
    void updateLanguage(const ScintillaNext *editor);
    void updateEol(const ScintillaNext *editor);
    void updateOverType(const ScintillaNext *editor);

private:
    void loadSettings();
    void saveSettings() const;
    QLabel *labelForField(Field field) const;

    QLabel *docType{nullptr};
    QLabel *docSize{nullptr};
    QLabel *docPos{nullptr};
    QLabel *unicodeChar{nullptr};
    QLabel *occurrences{nullptr};
    QLabel *eolFormat{nullptr};
    QLabel *unicodeType{nullptr};
    QLabel *overType{nullptr};

    QMap<Field, bool> m_fieldVisibility;

    QMetaObject::Connection editorUiUpdated;
    QMetaObject::Connection documentLexerChanged;
};

#endif  // EDITORINFOSTATUSBAR_H
