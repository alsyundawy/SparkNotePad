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

#ifndef SCINTILLANEXT_H
#define SCINTILLANEXT_H

#include "RangeAllocator.h"
#include "ScintillaEdit.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QPoint>
#include <algorithm>
#include <cstdint>

class ScintillaNext : public ScintillaEdit {  // NOSONAR(cpp:S1448, cpp:S110, cpp:S5008)
    Q_OBJECT

public:
    using QObject::setProperty;
    using QWidget::setFocus;
    using ScintillaEdit::getLine;
    using ScintillaEdit::getText;
    using ScintillaEdit::setFocus;
    using ScintillaEdit::setProperty;
    QByteArray getText() { return getText(length() + 1); }
    QVariant qtProperty(const char *propName) const { return QObject::property(propName); }
    bool setQtProperty(const char *propName, const QVariant &value) { return QObject::setProperty(propName, value); }
    QPoint pointFromPosition(Sci_Position pos) {
        return QPoint(static_cast<int>(pointXFromPosition(pos)), static_cast<int>(pointYFromPosition(pos)));
    }
    QString filePath() const { return getFilePath(); }
    QString getFileName() const { return getName(); }
    void goToPos(sptr_t pos) { gotoPos(pos); }
    void getRange(int start, int end, char *text) const {
        if (!text)
            return;
        QByteArray b = get_text_range(start, end);
        const auto sz = static_cast<size_t>(b.size());
        std::copy_n(b.constData(), sz, text);
        text[sz] = '\0';
    }
    void getLine(sptr_t line, char *text) {
        if (!text)
            return;
        QByteArray b = ScintillaEdit::getLine(line);
        const auto sz = static_cast<size_t>(b.size());
        std::copy_n(b.constData(), sz, text);
        text[sz] = '\0';
    }

    static bool isRangeValid(const Sci_CharacterRange &range) {
        return range.cpMin != INVALID_POSITION && range.cpMax != INVALID_POSITION;
    }

    explicit ScintillaNext(QString initialName, QWidget *parent = Q_NULLPTR);
    ~ScintillaNext() override;

    static ScintillaNext *fromFile(const QString &filePath, bool tryToCreate = false);
    static QString eolModeToString(int eolMode);
    static int stringToEolMode(const QString &eolMode);

    int allocateIndicator(const QString &name);

    template <typename Func>
    void forEachMatch(const QString &text, Func callback) const {
        forEachMatch(text.toUtf8(), callback);
    }

    template <typename Func>
    void forEachMatch(const QByteArray &byteArray, Func callback) const {
        forEachMatchInRange(byteArray, callback, {0, (Sci_PositionCR)length()});
    }

    template <typename Func>
    void forEachMatchInRange(const QByteArray &byteArray, Func callback, Sci_CharacterRange range) const;

    template <typename Func>
    void forEachLineInSelection(int selection, Func callback);

    void goToRange(const Sci_CharacterRange &range);

    QByteArray eolString() const;

    bool lineIsEmpty(int line);

    void deleteLine(int line);

    // cppcheck-suppress duplInheritedMember
    void cutAllowLine();  // NOLINT // NOSONAR(cpp:S5495)

    Sci_CharacterRange getContextText();
    Sci_CharacterRange wordAtPosition(int pos);

    void modifyFoldLevels(int level, int action);
    void foldAllLevels(int level);
    void unFoldAllLevels(int level);

    void deleteLeadingEmptyLines();
    void deleteTrailingEmptyLines();

    bool isFile() const;
    QFileInfo getFileInfo() const;

    bool isSavedToDisk() const;
    bool canSaveToDisk() const;

    const QString &getName() const { return name; }
    void setName(const QString &newName);
    QString getPath() const;
    QString getFilePath() const;
    QString getToolTip() const;

    // NOTE: this is dangerous and should only be used in very rare situations
    void setFileInfo(const QString &targetFilePath);

    void detachFileInfo(const QString &newName);

    enum class FileStateChange : std::uint8_t {
        NoChange,
        Modified,
        Deleted,
        Restored,
    };

    enum class BufferType : std::uint8_t {
        New,          // A temporary buffer, e.g. "New 1"
        File,         // Buffer tied to a file on the file system
        FileMissing,  // Buffer with a missing file on the file system
    };

    enum class BomType : std::uint8_t { None, Utf8, Utf16LE, Utf16BE };

    BomType bom() const { return bomType; }
    void setBom(BomType newBom) { bomType = newBom; }

    const QString &encodingName() const { return fileEncoding; }
    void setFileEncoding(const QString &encoding) { fileEncoding = encoding; }

    void convertToEncoding(const QString &newEncoding, BomType newBom);
    bool reloadWithEncoding(const QString &newEncoding);

    bool isTemporary() const { return temporary; }
    void setTemporary(bool temp);

    void setFoldMarkers(const QString &type);

    QString languageName;
    QByteArray languageSingleLineComment;

#include "ScintillaEnums.h"  // IWYU pragma: keep // NOLINT // NOSONAR

public slots:
    void close();  // NOLINT // NOSONAR(cpp:S5495)
    QFileDevice::FileError save(bool preserveTimestamp = false);
    QDateTime fileTimestamp();
    void updateTimestamp();
    void reload();
    void omitModifications();
    QFileDevice::FileError saveAs(const QString &newFilePath);
    QFileDevice::FileError saveCopyAs(const QString &destPath) const;
    bool rename(const QString &newFilePath);
    ScintillaNext::FileStateChange checkFileForStateChange();
    bool moveToTrash() const;

    void toggleCommentSelection();
    void commentLineSelection();
    void uncommentLineSelection();

    void removeDuplicateLines();
    void removeConsecutiveDuplicateLines();

    void trimTrailingWhitespace();

signals:
    void aboutToSave();
    void saved();
    void closed();
    void renamed();

    void lexerChanged();
    void reloaded();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QString name;
    BufferType bufferType = BufferType::New;
    BomType bomType = BomType::None;
    QString fileEncoding = QStringLiteral("UTF-8");
    QFileInfo fileInfo;
    QDateTime modifiedTime;
    RangeAllocator indicatorResources;

    bool temporary = false;  // Temporary file loaded from a session. It can either be a 'New' file or actual 'File'

    bool readFromDisk(QFile &file);
    QByteArray rawBufferData() const;
};

template <typename Func>
void ScintillaNext::forEachLineInSelection(int selection, Func callback) {
    const auto lineStart = static_cast<int>(lineFromPosition(selectionNStart(selection)));
    const auto lineEnd = static_cast<int>(lineFromPosition(selectionNEnd(selection)));

    for (int curLine = lineStart; curLine <= lineEnd; ++curLine) {
        callback(curLine);
    }
}

// Stick this in the header file...because C++, that's why
template <typename Func>
void ScintillaNext::forEachMatchInRange(const QByteArray &text, Func callback, Sci_CharacterRange range) const {
    Sci_TextToFind ttf{range, text.constData(), {INVALID_POSITION, INVALID_POSITION}};
    const auto flags = static_cast<int>(searchFlags());

    while (send(SCI_FINDTEXT, flags, reinterpret_cast<sptr_t>(&ttf)) != INVALID_POSITION &&
           ttf.chrgText.cpMin < ttf.chrgText.cpMax && ttf.chrg.cpMin < range.cpMax) {
        if (const auto nextMin = callback(static_cast<int>(ttf.chrgText.cpMin), static_cast<int>(ttf.chrgText.cpMax));
            nextMin <= ttf.chrgText.cpMin) {
            ttf.chrg.cpMin = ttf.chrgText.cpMax;
        } else {
            ttf.chrg.cpMin = nextMin;
        }
    }
}

#endif  // SCINTILLANEXT_H
