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

#include "EditorConfigAppDecorator.h"
#include "EditorManager.h"
#include "ScintillaNext.h"
#include "Finder.h"

#include "EditorConfig.h"

class PreventUnfolding {
    Q_DISABLE_COPY_MOVE(PreventUnfolding)

public:
    explicit PreventUnfolding(ScintillaNext *editor)
        : editor(editor), foldFlags(static_cast<int>(editor->automaticFold())) {
        editor->setAutomaticFold(0);
    }

    ~PreventUnfolding() { editor->setAutomaticFold(foldFlags); }

private:
    ScintillaNext *editor;
    int foldFlags;
};

EditorConfigAppDecorator::EditorConfigAppDecorator(SparkNotePadApplication *app) : ApplicationDecorator(app) {
    const EditorManager *manager = app->getEditorManager();

    connect(manager, &EditorManager::editorCreated, this, &EditorConfigAppDecorator::doEditorConfig);
    // Reload events are handled via editor instance signals.
}

void EditorConfigAppDecorator::doEditorConfig(ScintillaNext *editor) const {
    if (!this->isEnabled() || !editor->isFile()) {
        return;
    }

    const EditorConfigSettings settings = EditorConfig::getFileSettings(editor->getFilePath());

    qDebug() << "EditorConfig settings for:" << editor->getFilePath();
    for (const auto &[key, val] : settings.toStdMap()) {
        qDebug() << key << "=" << val;
    }

    if (settings.contains(QStringLiteral("indent_style"))) {
        const QString indentStyle = settings[QStringLiteral("indent_style")];
        if (indentStyle == QStringLiteral("tab")) {
            editor->setUseTabs(true);
        } else if (indentStyle == QStringLiteral("space")) {
            editor->setUseTabs(false);
        }

        // Set a flag so that the tab/spaces won't get overridden
        editor->QObject::setProperty("nn_skip_usetabs", "EditorConfig");
    }

    if (settings.contains(QStringLiteral("indent_size"))) {
        const int indentSize = settings[QStringLiteral("indent_size")].toInt();
        if (indentSize > 0) {
            editor->setIndent(indentSize);
        }
    }

    if (settings.contains(QStringLiteral("tab_width"))) {
        const int tabWidth = settings[QStringLiteral("tab_width")].toInt();
        if (tabWidth > 0) {
            editor->setTabWidth(tabWidth);

            // Set a flag so that the tab width won't get overridden
            editor->QObject::setProperty("nn_skip_tabwidth", "EditorConfig");
        }
    }

    if (settings.contains(QStringLiteral("end_of_line"))) {
        const QString eol = settings[QStringLiteral("end_of_line")];
        if (eol == QStringLiteral("lf")) {
            editor->setEOLMode(SC_EOL_LF);
        } else if (eol == QStringLiteral("cr")) {
            editor->setEOLMode(SC_EOL_CR);
        } else if (eol == QStringLiteral("crlf")) {
            editor->setEOLMode(SC_EOL_CRLF);
        }
    }

    if (settings.contains(QStringLiteral("trim_trailing_whitespace")) &&
        settings[QStringLiteral("trim_trailing_whitespace")] == QStringLiteral("true")) {
        connect(editor, &ScintillaNext::aboutToSave, this, &EditorConfigAppDecorator::trimTrailingWhitespace);
    }

    if (settings.contains(QStringLiteral("insert_final_newline"))) {
        const QString newlineSetting = settings[QStringLiteral("insert_final_newline")];
        if (newlineSetting == QStringLiteral("true")) {
            connect(editor, &ScintillaNext::aboutToSave, this, &EditorConfigAppDecorator::ensureFinalNewline);
        } else if (newlineSetting == QStringLiteral("false")) {
            connect(editor, &ScintillaNext::aboutToSave, this, &EditorConfigAppDecorator::ensureNoFinalNewline);
        }
    }
}

void EditorConfigAppDecorator::trimTrailingWhitespace() const {
    ScintillaNext *editor = qobject_cast<ScintillaNext *>(sender());
    Finder f(editor);
    const PreventUnfolding pu(editor);

    // NOTE: see note in QRegexSearch about (*ANYCRLF)
    f.options().text = "(*ANYCRLF)[ \t]+$";
    f.options().flags = Scintilla::FindOption::RegExp;
    f.replaceAll("");
}

void EditorConfigAppDecorator::ensureFinalNewline() const {
    ScintillaNext *editor = qobject_cast<ScintillaNext *>(sender());
    const PreventUnfolding pu(editor);
    const int lastLine = static_cast<int>(editor->lineCount() - 1);
    const int lastLineLength = static_cast<int>(editor->lineEndPosition(lastLine) - editor->positionFromLine(lastLine));

    if (lastLineLength != 0) {
        switch (editor->eOLMode()) {
            case SC_EOL_CRLF:
                editor->appendText(2, "\r\n");
                break;
            case SC_EOL_CR:
                editor->appendText(1, "\r");
                break;
            case SC_EOL_LF:
                editor->appendText(1, "\n");
                break;
            default:
                break;
        }
    }
}

void EditorConfigAppDecorator::ensureNoFinalNewline() const {
    ScintillaNext *editor = qobject_cast<ScintillaNext *>(sender());
    const PreventUnfolding pu(editor);

    editor->deleteTrailingEmptyLines();
}
