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

#include "HTMLAutoCompleteDecorator.h"
#include "SciLexer.h"
#include "UndoAction.h"

#include <algorithm>
#include <string_view>

static constexpr int MAX_TAG_NAME_LENGTH = 64;
static constexpr int MAX_TAG_LENGTH = 1024;

static bool isVoidTag(QByteArrayView tag) {
    static constexpr std::string_view kVoidTags[] = {"area",  "base", "br",   "col",    "embed", "hr", "img",
                                                     "input", "link", "meta", "source", "track", "wbr"};
    const std::string_view sv(tag.data(), static_cast<size_t>(tag.size()));
    return std::any_of(std::begin(kVoidTags), std::end(kVoidTags), [sv](std::string_view vt) { return sv == vt; });
}

HTMLAutoCompleteDecorator::HTMLAutoCompleteDecorator(ScintillaNext *editor) : EditorDecorator(editor) {
    connect(editor, &ScintillaNext::lexerChanged, this, [=, this]() {
        QString language = editor->languageName;
        setEnabled(language == "HTML");
    });
}

void HTMLAutoCompleteDecorator::notify(const Scintilla::NotificationData *pscn) {
    if (pscn->nmhdr.code == Scintilla::Notification::CharAdded) {
        if (pscn->ch == '>') {
            const sptr_t currentPos = editor->currentPos();

            // Skip if it is inside any scripting language
            if (editor->styleAt(currentPos) >= SCE_HJ_START)
                return;

            sptr_t tagEndPos = editor->positionBefore(currentPos);
            sptr_t tagStartPos = editor->positionBefore(tagEndPos);

            // Check the char right before the >
            const sptr_t prevChar = editor->charAt(tagStartPos);
            if (prevChar == '-')
                return;  // Assume -->
            if (prevChar == '/')
                return;  // <tag/>

            // Go backwards searching for the beginning of the tag
            while ((currentPos - tagStartPos) <= MAX_TAG_LENGTH) {
                const sptr_t c = editor->charAt(tagStartPos);

                // While backing up, save the position of the last whitespace character we encounter
                if (std::isspace(static_cast<unsigned char>(c)) != 0) {
                    tagEndPos = tagStartPos;
                }

                // Found the start (yes this can be fooled but this is by far the easiest for now)
                if (c == '<')
                    break;

                // Reached the beginning of the document
                if (tagStartPos == 0)
                    return;

                tagStartPos = editor->positionBefore(tagStartPos);
            }

            // For sanity, check the length of the tag name
            if (tagEndPos - tagStartPos >= MAX_TAG_NAME_LENGTH)
                return;

            const sptr_t afterPos = editor->positionAfter(tagStartPos);
            QByteArray tag = editor->get_text_range(afterPos, tagEndPos);

            if (tag.isEmpty())
                return;  // <>
            if (tag[0] == '/')
                return;  // </div>
            if (tag[0] == '?')
                return;  // <?php>
            if (tag[0] == '!')
                return;  // <!-- and <!doctype
            if (isVoidTag(tag))
                return;  // Some tags are not expected to have closing tags

            // All good to go now, wrap it and insert it
            tag.prepend("</");
            tag.append('>');

            const UndoAction ua(editor);
            editor->insertText(currentPos, tag.constData());
        }
    }
}
