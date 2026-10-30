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

#include "ScintillaNext.h"
#include "EncodingDetector.h"
#include "FileTimeUtils.h"
#include "Finder.h"
#include "ScintillaCommenter.h"

#include "ByteArrayUtils.h"

#include <QByteArrayView>
#include <QDir>
#include <QDropEvent>
#include <QMimeData>
#include <QSaveFile>
#include <QTextCodec>

const int CHUNK_SIZE = 1024 * 1024 * 4;  // Not sure what is best

inline constexpr QByteArrayView BOM_UTF8{"\xEF\xBB\xBF", 3};
inline constexpr QByteArrayView BOM_UTF16LE{"\xFF\xFE", 2};
inline constexpr QByteArrayView BOM_UTF16BE{"\xFE\xFF", 2};

ScintillaNext::BomType detectBom(const QByteArray &data) {
    using enum ScintillaNext::BomType;
    if (data.startsWith(BOM_UTF8))
        return Utf8;
    if (data.startsWith(BOM_UTF16LE))
        return Utf16LE;
    if (data.startsWith(BOM_UTF16BE))
        return Utf16BE;

    return None;
}

QByteArray bomData(ScintillaNext::BomType bom) {
    using enum ScintillaNext::BomType;
    switch (bom) {
        case Utf8:
            return BOM_UTF8.toByteArray();
        case Utf16LE:
            return BOM_UTF16LE.toByteArray();
        case Utf16BE:
            return BOM_UTF16BE.toByteArray();
        case None:
            return QByteArray();
    }
    return QByteArray();
}

int bomLength(ScintillaNext::BomType bom) {
    using enum ScintillaNext::BomType;
    if (bom == Utf8)
        return static_cast<int>(BOM_UTF8.size());
    else if (bom == Utf16LE)
        return static_cast<int>(BOM_UTF16LE.size());
    else if (bom == Utf16BE)
        return static_cast<int>(BOM_UTF16BE.size());

    return 0;
}

static QFileDevice::FileError writeToDisk(const QByteArray &data, const QString &path, ScintillaNext::BomType bom,
                                          const QString &encoding) {
    qInfo(Q_FUNC_INFO);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("writeToDisk() failed to open file %s: %s", qPrintable(path), qPrintable(file.errorString()));
        return file.error();
    }

    if (const QByteArray encodedData = EncodingDetector::encodeFromUtf8(data, encoding, bom);
        file.write(encodedData) == -1) {
        qWarning("writeToDisk() failed writing to file %s: %s", qPrintable(path), qPrintable(file.errorString()));
        return file.error();
    }

    return file.error();
}

static bool isNewlineCharacter(char c) {
    return c == '\n' || c == '\r';
}

ScintillaNext::ScintillaNext(QString initialName, QWidget *parent)
    : ScintillaEdit(parent), name(std::move(initialName)), indicatorResources(INDICATOR_MAX + 1) {
    // Explicitly set UTF-8 codepage so multi-byte Unicode/emojis and text selection work accurately
    setCodePage(SC_CP_UTF8);

    // Per the scintilla documentation, some parts of the range are not generally available
    indicatorResources.disableRange(0, 7);
    indicatorResources.disableRange(INDICATOR_IME, INDICATOR_IME_MAX);
    indicatorResources.disableRange(INDICATOR_HISTORY_REVERTED_TO_ORIGIN_INSERTION,
                                    INDICATOR_HISTORY_REVERTED_TO_MODIFIED_DELETION);
}

ScintillaNext::~ScintillaNext() = default;

ScintillaNext *ScintillaNext::fromFile(const QString &filePath, bool tryToCreate) {
    QFile file(filePath);
    auto editor = std::make_unique<ScintillaNext>(file.fileName());

    if (tryToCreate && !file.exists()) {
        qInfo("Trying to create %s", qUtf8Printable(filePath));
        if (QDir d; !d.mkpath(QFileInfo(file).path())) {
            qWarning("Failed to create path: %s", qUtf8Printable(QFileInfo(file).path()));
        }

        if (QFile f(filePath); f.open(QIODevice::WriteOnly)) {
            f.close();
        }
    }

    if (const bool readSuccessful = editor->readFromDisk(file); !readSuccessful) {
        return Q_NULLPTR;
    }

    editor->setFileInfo(filePath);

    return editor.release();
}

QString ScintillaNext::eolModeToString(int eolMode) {
    if (eolMode == SC_EOL_CRLF)
        return QStringLiteral("crlf");
    else if (eolMode == SC_EOL_CR)
        return QStringLiteral("cr");
    else if (eolMode == SC_EOL_LF)
        return QStringLiteral("lf");
    else
        return QString();  // unknown
}

int ScintillaNext::stringToEolMode(const QString &eolMode) {
    if (eolMode == QStringLiteral("crlf"))
        return SC_EOL_CRLF;
    else if (eolMode == QStringLiteral("cr"))
        return SC_EOL_CR;
    else if (eolMode == QStringLiteral("lf"))
        return SC_EOL_LF;
    else
        return -1;
}

int ScintillaNext::allocateIndicator(const QString &indicatorName) {
    return indicatorResources.requestResource(indicatorName);
}

void ScintillaNext::goToRange(const Sci_CharacterRange &range) {
    qInfo(Q_FUNC_INFO);

    if (isRangeValid(range)) {
        // Lines can be folded so make sure they are visible
        ensureVisible(lineFromPosition(range.cpMin));
        ensureVisible(lineFromPosition(range.cpMax));

        setSelection(range.cpMax, range.cpMin);
        scrollRange(range.cpMax, range.cpMin);
    }
}

QByteArray ScintillaNext::eolString() const {
    const auto eol = static_cast<int>(eOLMode());

    if (eol == SC_EOL_LF)
        return QByteArrayLiteral("\n");
    else if (eol == SC_EOL_CRLF)
        return QByteArrayLiteral("\r\n");
    else
        return QByteArrayLiteral("\r");
}

bool ScintillaNext::lineIsEmpty(int line) {
    return (lineEndPosition(line) - positionFromLine(line)) == 0;
}

void ScintillaNext::deleteLine(int line) {
    deleteRange(positionFromLine(line), lineLength(line));
}

// cppcheck-suppress duplInheritedMember
void ScintillaNext::cutAllowLine() {  // NOSONAR(cpp:S5495)
    if (selectionEmpty()) {
        copyAllowLine();
        lineDelete();
    } else {
        cut();
    }
}

Sci_CharacterRange ScintillaNext::getContextText() {
    if (!selectionEmpty()) {
        if (selections() == 1) {
            auto start = static_cast<int>(selectionStart());
            auto end = static_cast<int>(selectionEnd());

            if (end - start < 1024 && lineFromPosition(start) == lineFromPosition(end)) {
                return {start, end};
            }
        }

        return {INVALID_POSITION, INVALID_POSITION};
    }

    return wordAtPosition(static_cast<int>(currentPos()));
}

Sci_CharacterRange ScintillaNext::wordAtPosition(int pos) {
    auto start = static_cast<int>(wordStartPosition(pos, true));
    if (const auto end = static_cast<int>(wordEndPosition(pos, true)); end > start) {
        return {start, end};
    }

    return {INVALID_POSITION, INVALID_POSITION};
}

void ScintillaNext::modifyFoldLevels(int level, int action) {  // NOLINT(bugprone-easily-swappable-parameters)
    const auto totalLines = static_cast<int>(lineCount());

    int line = 0;
    while (line < totalLines) {
        auto foldFlags =
            static_cast<int>(foldLevel(line));  // Even though its called fold level it contains several other flags
        bool isHeader = foldFlags & SC_FOLDLEVELHEADERFLAG;
        int actualLevel = (foldFlags & SC_FOLDLEVELNUMBERMASK) - SC_FOLDLEVELBASE;

        if (isHeader && actualLevel == level) {
            foldLine(line, action);
            line = static_cast<int>(lastChild(line, -1)) + 1;
        } else {
            ++line;
        }
    }
}

void ScintillaNext::foldAllLevels(int level) {
    modifyFoldLevels(level, SC_FOLDACTION_CONTRACT);
}

void ScintillaNext::unFoldAllLevels(int level) {
    modifyFoldLevels(level, SC_FOLDACTION_EXPAND);
}

void ScintillaNext::deleteLeadingEmptyLines() {
    while (lineCount() > 1 && lineIsEmpty(0)) {
        deleteLine(0);
    }
}

void ScintillaNext::deleteTrailingEmptyLines() {
    const auto docLength = static_cast<int>(length());
    int position = docLength;

    while (position > 0 && isNewlineCharacter(static_cast<char>(charAt(position - 1)))) {
        position--;
    }

    deleteRange(position, docLength - position);
}

bool ScintillaNext::isSavedToDisk() const {
    return !canSaveToDisk();
}

bool ScintillaNext::canSaveToDisk() const {
    using enum BufferType;
    // The buffer can be saved if:
    // - It is marked as a temporary since as soon as it gets saved it is no longer a temporary buffer
    // - A modified file
    // - A missing file since as soon as it is saved it is no longer missing.
    return temporary || (bufferType == New && modify()) || (bufferType == File && modify()) ||
           (bufferType == FileMissing);
}

void ScintillaNext::setName(const QString &newName) {
    this->name = newName;

    emit renamed();
}

bool ScintillaNext::isFile() const {
    return bufferType == BufferType::File || bufferType == BufferType::FileMissing;
}

QFileInfo ScintillaNext::getFileInfo() const {
    Q_ASSERT(isFile());

    return fileInfo;
}

QString ScintillaNext::getPath() const {
    if (!isFile()) {
        return QString();
    }

    return QDir::toNativeSeparators(fileInfo.canonicalPath());
}

QString ScintillaNext::getFilePath() const {
    if (!isFile()) {
        return QString();
    }

    return QDir::toNativeSeparators(fileInfo.canonicalFilePath());
}

QString ScintillaNext::getToolTip() const {
    return isFile() ? getFilePath() : getName();
}

void ScintillaNext::setFoldMarkers(const QString &type) {
    QMap<QString, QList<int>> map{
        {"simple",
         {SC_MARK_MINUS, SC_MARK_PLUS, SC_MARK_EMPTY, SC_MARK_EMPTY, SC_MARK_EMPTY, SC_MARK_EMPTY, SC_MARK_EMPTY}},
        {"arrow",
         {SC_MARK_ARROWDOWN, SC_MARK_ARROW, SC_MARK_EMPTY, SC_MARK_EMPTY, SC_MARK_EMPTY, SC_MARK_EMPTY, SC_MARK_EMPTY}},
        {"circle",
         {SC_MARK_CIRCLEMINUS, SC_MARK_CIRCLEPLUS, SC_MARK_VLINE, SC_MARK_LCORNERCURVE, SC_MARK_CIRCLEPLUSCONNECTED,
          SC_MARK_CIRCLEMINUSCONNECTED, SC_MARK_TCORNERCURVE}},
        {"box",
         {SC_MARK_BOXMINUS, SC_MARK_BOXPLUS, SC_MARK_VLINE, SC_MARK_LCORNER, SC_MARK_BOXPLUSCONNECTED,
          SC_MARK_BOXMINUSCONNECTED, SC_MARK_TCORNER}},
    };

    if (!map.contains(type))
        return;

    const auto &types = map[type];
    markerDefine(SC_MARKNUM_FOLDEROPEN, types[0]);
    markerDefine(SC_MARKNUM_FOLDER, types[1]);
    markerDefine(SC_MARKNUM_FOLDERSUB, types[2]);
    markerDefine(SC_MARKNUM_FOLDERTAIL, types[3]);
    markerDefine(SC_MARKNUM_FOLDEREND, types[4]);
    markerDefine(SC_MARKNUM_FOLDEROPENMID, types[5]);
    markerDefine(SC_MARKNUM_FOLDERMIDTAIL, types[6]);
}

void ScintillaNext::close() {
    emit closed();

    deleteLater();
}

QFileDevice::FileError ScintillaNext::save(bool preserveTimestamp) {
    qInfo(Q_FUNC_INFO);

    Q_ASSERT(isFile());

    emit aboutToSave();

    QDateTime originalTime;
    QDateTime originalAccessTime;
    if (preserveTimestamp && fileInfo.exists()) {
        fileInfo.refresh();
        originalTime = fileInfo.lastModified();
        originalAccessTime = fileInfo.lastRead();
    }

    const QByteArray data = rawBufferData();
    const QString path = fileInfo.filePath();
    QFileDevice::FileError writeSuccessful = writeToDisk(data, path, bomType, fileEncoding);

    if (writeSuccessful == QFileDevice::NoError) {
        if (preserveTimestamp && originalTime.isValid()) {
            FileTimeUtils::setFileTimes(path, originalTime, originalAccessTime);
        }
        updateTimestamp();
        setSavePoint();

        // If this was a temporary file, make sure it is not any more
        setTemporary(false);

        emit saved();
    }

    return writeSuccessful;
}

void ScintillaNext::reload() {
    Q_ASSERT(isFile());

    // Ensure the file still exists.
    if (!QFile::exists(fileInfo.canonicalFilePath())) {
        return;
    }

    const auto line = static_cast<int>(firstVisibleLine());
    const auto caret = static_cast<int>(selectionNCaret(mainSelection()));
    const auto anchor = static_cast<int>(selectionNAnchor(mainSelection()));

    // Remove all the text
    {
        const QSignalBlocker blocker(this);
        setUndoCollection(false);
        emptyUndoBuffer();
        setText("");
        setUndoCollection(true);
    }

    // NOTE: if the read fails then the buffer will be completely empty...which probably
    // isn't a good thing, but this should be a rare occurrence.
    QFile f(fileInfo.canonicalFilePath());
    if (const bool readSuccessful = readFromDisk(f); !readSuccessful) {
        return;
    }

    updateTimestamp();
    setSavePoint();

    // If this was a temporary file, make sure it is not any more
    if (isTemporary())
        setTemporary(false);

    scrollVertical(line, 0);
    setSelection(caret, anchor);

    emit reloaded();
}

void ScintillaNext::omitModifications() {
    // If file modifications will be omitted just update file timestamp
    // so pop-up will be displayed only once per file modifications.
    updateTimestamp();
    setTemporary(true);

    return;
}

QFileDevice::FileError ScintillaNext::saveAs(const QString &newFilePath) {
    bool isRenamed = bufferType == BufferType::New || fileInfo.canonicalFilePath() != newFilePath;

    emit aboutToSave();

    const QByteArray data = rawBufferData();
    QFileDevice::FileError saveSuccessful = writeToDisk(data, newFilePath, bomType, fileEncoding);

    if (saveSuccessful == QFileDevice::NoError) {
        setFileInfo(newFilePath);
        setSavePoint();

        // If this was a temporary file, make sure it is not any more
        setTemporary(false);

        emit saved();

        if (isRenamed) {
            emit renamed();
        }
    }

    return saveSuccessful;
}

QFileDevice::FileError ScintillaNext::saveCopyAs(const QString &destPath) const {
    const QByteArray data = rawBufferData();
    return writeToDisk(data, destPath, bomType, fileEncoding);
}

bool ScintillaNext::rename(const QString &newFilePath) {
    emit aboutToSave();

    // Write out the buffer to the new path
    if (saveCopyAs(newFilePath) == QFileDevice::NoError) {
        // Remove the old file
        const QString oldPath = fileInfo.canonicalFilePath();
        QFile::remove(oldPath);

        // Everything worked fine, so update the buffer's info
        setFileInfo(newFilePath);
        setSavePoint();

        // If this was a temporary file, make sure it is not any more
        setTemporary(false);

        emit saved();

        emit renamed();

        return true;
    }

    return false;
}

ScintillaNext::FileStateChange ScintillaNext::checkFileForStateChange() {
    using enum FileStateChange;
    if (bufferType == BufferType::New) {
        return NoChange;
    } else if (bufferType == BufferType::File) {
        // refresh else exists() fails to notice missing file
        fileInfo.refresh();

        if (!fileInfo.exists()) {
            bufferType = BufferType::FileMissing;

            emit savePointChanged(false);

            return Deleted;
        }

        // See if the timestamp changed
        if (modifiedTime != fileTimestamp()) {
            return Modified;
        } else {
            return NoChange;
        }
    } else if (bufferType == BufferType::FileMissing) {
        // See if it reappeared
        fileInfo.refresh();

        if (fileInfo.exists()) {
            bufferType = BufferType::File;

            return Restored;
        } else {
            return NoChange;
        }
    }

    qInfo("type() = %d", bufferType);
    Q_ASSERT(false);

    return NoChange;
}

bool ScintillaNext::moveToTrash() const {
    if (QFile::exists(fileInfo.canonicalFilePath())) {
        QFile f(fileInfo.canonicalFilePath());

        return f.moveToTrash();
    }

    return false;
}

void ScintillaNext::toggleCommentSelection() {
    ScintillaCommenter sc(this);
    sc.toggleSelection();
}

void ScintillaNext::commentLineSelection() {
    ScintillaCommenter sc(this);
    sc.commentSelection();
}

void ScintillaNext::uncommentLineSelection() {
    ScintillaCommenter sc(this);
    sc.uncommentSelection();
}

void ScintillaNext::removeDuplicateLines() {
    const QByteArray data = rawBufferData();
    const QByteArray delim = eolString();

    auto lines = ByteArrayUtils::split(data, delim);
    const auto originalLineCount = static_cast<int>(lines.length());
    ByteArrayUtils::removeDuplicates(lines);

    if (originalLineCount == lines.length()) {
        return;  // No lines were removed
    }

    QByteArray result = ByteArrayUtils::join(lines, delim);

    const UndoAction ua(this);
    setTargetRange(0, textLength());
    replaceTarget(result.length(), result.constData());
}

void ScintillaNext::removeConsecutiveDuplicateLines() {
    const QByteArray data = rawBufferData();
    const QByteArray delim = eolString();

    auto lines = ByteArrayUtils::split(data, delim);
    const auto originalLineCount = static_cast<int>(lines.length());
    ByteArrayUtils::removeConsecutiveDuplicates(lines);
    QByteArray result = ByteArrayUtils::join(lines, delim);

    if (originalLineCount == lines.length()) {
        return;  // No lines were removed
    }

    const UndoAction ua(this);
    setTargetRange(0, textLength());
    replaceTarget(result.length(), result.constData());
}

void ScintillaNext::trimTrailingWhitespace() {
    Finder f(this);
    const UndoAction ua(this);

    // $ alone only matches before \n, so handle CRLF and CR line endings explicitly
    f.options().text = QStringLiteral("[ \\t]+(?=[\\r\\n]|$)");
    f.options().flags = Scintilla::FindOption::RegExp;
    f.replaceAll(QString());
}

void ScintillaNext::dragEnterEvent(QDragEnterEvent *event) {
    // Ignore all drag and drop events with urls and let the main application handle it
    if (event->mimeData()->hasUrls()) {
        return;
    }

    ScintillaEdit::dragEnterEvent(event);
}

void ScintillaNext::dropEvent(QDropEvent *event) {
    // Ignore all drag and drop events with urls and let the main application handle it
    if (event->mimeData()->hasUrls()) {
        return;
    }

    ScintillaEdit::dropEvent(event);
}

bool ScintillaNext::readFromDisk(QFile &file) {
    if (!file.exists()) {
        qWarning("Cannot read \"%s\": doesn't exist", qUtf8Printable(file.fileName()));
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("QFile::open() failed when opening \"%s\" - error code %d: %s", qUtf8Printable(file.fileName()),
                 file.error(), qUtf8Printable(file.errorString()));
        return false;
    }

    // Pre-allocate buffer according to file size
    allocate(file.size());

    // Turn off undo collection and block signals during loading
    setUndoCollection(false);
    blockSignals(true);
    // Note: notifications suppressed during bulk file load

    // Multi-stage encoding & BOM detection on initial sample
    const QByteArray initialSample = file.peek(65536);
    EncodingDetector::DetectionResult det = EncodingDetector::detect(initialSample);
    bomType = det.bomType;
    fileEncoding = det.encoding;

    QByteArray chunk;
    qint64 bytesRead = 0;
    bool first_read = true;

    do {
        chunk.resize(CHUNK_SIZE);
        bytesRead = file.read(chunk.data(), CHUNK_SIZE);
        chunk.resize(bytesRead);

        if (bytesRead <= 0)
            break;

        int offset = 0;
        if (first_read) {
            first_read = false;
            offset = det.bomLength;
        }

        const QByteArray rawChunk = (offset > 0) ? chunk.mid(offset) : chunk;
        const QByteArray utf8 = EncodingDetector::decodeToUtf8(rawChunk, fileEncoding, BomType::None);

        appendText(utf8.size(), utf8.constData());
    } while (!file.atEnd() && status() == SC_STATUS_OK);

    file.close();

    // Restore it back
    this->blockSignals(false);
    setUndoCollection(true);
    // modEventMask(SC_MODEVENTMASKALL)?

    if (status() != SC_STATUS_OK) {
        qWarning("something bad happened in document->add_data() %ld", status());
        return false;
    }

    if (bytesRead == -1) {
        qWarning("Something bad happened when reading disk %d %s", file.error(), qUtf8Printable(file.errorString()));
        return false;
    }

    if (!QFileInfo(file).isWritable()) {
        qInfo("Setting file as read-only");
        setReadOnly(true);
    }

    return true;
}

QDateTime ScintillaNext::fileTimestamp() {
    Q_ASSERT(bufferType != BufferType::New);

    fileInfo.refresh();
    qInfo("%s last modified %s", qUtf8Printable(fileInfo.fileName()),
          qUtf8Printable(fileInfo.lastModified().toString()));
    return fileInfo.lastModified();
}

void ScintillaNext::updateTimestamp() {
    modifiedTime = fileTimestamp();
}

void ScintillaNext::setFileInfo(const QString &targetFilePath) {
    fileInfo.setFile(targetFilePath);
    fileInfo.makeAbsolute();

    Q_ASSERT(fileInfo.exists());

    name = fileInfo.fileName();
    bufferType = BufferType::File;

    updateTimestamp();
}

void ScintillaNext::detachFileInfo(const QString &newName) {
    setName(newName);

    bufferType = BufferType::New;
}

void ScintillaNext::setTemporary(bool temp) {
    temporary = temp;

    // Fake this signal
    emit savePointChanged(temporary);
}

void ScintillaNext::convertToEncoding(const QString &newEncoding, BomType newBom) {
    fileEncoding = newEncoding;
    bomType = newBom;
    emit savePointChanged(false);
}

bool ScintillaNext::reloadWithEncoding(const QString &newEncoding) {
    if (bufferType != BufferType::File) {
        fileEncoding = newEncoding;
        return true;
    }

    QFile file(fileInfo.filePath());
    if (!file.exists())
        return false;

    clearAll();
    fileEncoding = newEncoding;
    return readFromDisk(file);
}

QByteArray ScintillaNext::rawBufferData() const {
    // NOLINTBEGIN(performance-no-int-to-ptr)
    return QByteArray::fromRawData(reinterpret_cast<const char *>(characterPointer()), textLength());  // NOSONAR
    // NOLINTEND(performance-no-int-to-ptr)
}
