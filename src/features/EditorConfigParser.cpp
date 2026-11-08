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

#include "EditorConfigParser.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QTextStream>
#include <algorithm>

#ifdef UNIT_TESTING
// Under unit testing, ScintillaNext might not be linked in the test binary
#else
#include "ScintillaNext.h"
#endif

namespace {

bool checkRootConfig(const QString &configPath) {
    if (QFile f(configPath); f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        while (!in.atEnd()) {
            const QString line = in.readLine().trimmed();
            if (line.startsWith(QLatin1Char('['))) {
                return false;
            }
            if (line.startsWith(QStringLiteral("root"), Qt::CaseInsensitive)) {
                const auto parts = line.split(QLatin1Char('='));
                if (parts.size() == 2 && parts[1].trimmed().compare(QStringLiteral("true"), Qt::CaseInsensitive) == 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

QList<QString> findConfigHierarchy(QDir currentDir) {
    QList<QString> configFiles;
    while (true) {
        if (const QString configPath = currentDir.filePath(QStringLiteral(".editorconfig"));
            QFile::exists(configPath)) {
            configFiles.prepend(configPath);
            if (checkRootConfig(configPath)) {
                break;
            }
        }
        if (!currentDir.cdUp() || currentDir.isRoot()) {
            break;
        }
    }
    return configFiles;
}

void applyConfigLine(EditorConfigParser::Settings &resolved, const QString &key, const QString &val) {
    if (key == QStringLiteral("indent_style")) {
        if (val == QStringLiteral("tab")) {
            resolved.indentStyle = EditorConfigParser::IndentStyle::Tab;
        } else if (val == QStringLiteral("space")) {
            resolved.indentStyle = EditorConfigParser::IndentStyle::Space;
        }
    } else if (key == QStringLiteral("indent_size")) {
        bool ok = false;
        if (const int sz = val.toInt(&ok); ok && sz > 0) {
            resolved.indentSize = sz;
        }
    } else if (key == QStringLiteral("tab_width")) {
        bool ok = false;
        if (const int tw = val.toInt(&ok); ok && tw > 0) {
            resolved.tabWidth = tw;
        }
    } else if (key == QStringLiteral("end_of_line")) {
        if (val == QStringLiteral("lf")) {
            resolved.endOfLine = EditorConfigParser::EndOfLine::Lf;
        } else if (val == QStringLiteral("crlf")) {
            resolved.endOfLine = EditorConfigParser::EndOfLine::Crlf;
        } else if (val == QStringLiteral("cr")) {
            resolved.endOfLine = EditorConfigParser::EndOfLine::Cr;
        }
    } else if (key == QStringLiteral("trim_trailing_whitespace")) {
        resolved.trimTrailingWhitespace = (val == QStringLiteral("true"));
    } else if (key == QStringLiteral("insert_final_newline")) {
        resolved.insertFinalNewline = (val == QStringLiteral("true"));
    } else if (key == QStringLiteral("max_line_length")) {
        bool ok = false;
        if (const int len = val.toInt(&ok); ok && len > 0) {
            resolved.maxLineLength = len;
        }
    }
}

void parseConfigFile(EditorConfigParser::Settings &resolved, const QString &configPath, const QString &targetFileName) {
    QFile f(configPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&f);
    bool inMatchingSection = false;

    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith(QLatin1Char('#')) || line.startsWith(QLatin1Char(';'))) {
            continue;
        }

        if (line.startsWith(QLatin1Char('[')) && line.endsWith(QLatin1Char(']'))) {
            const QString pattern = line.mid(1, line.length() - 2).trimmed();
            inMatchingSection = EditorConfigParser::matchGlob(pattern, targetFileName);
            continue;
        }

        if (!inMatchingSection) {
            continue;
        }

        const auto parts = line.split(QLatin1Char('='));
        if (parts.size() != 2) {
            continue;
        }

        applyConfigLine(resolved, parts[0].trimmed().toLower(), parts[1].trimmed().toLower());
    }
}

}  // namespace

QStringList EditorConfigParser::expandBraces(const QString &pattern) {
    const qsizetype openBrace = pattern.indexOf(QLatin1Char('{'));
    const qsizetype closeBrace = pattern.indexOf(QLatin1Char('}'), openBrace + 1);

    if (openBrace == -1 || closeBrace == -1) {
        return {pattern};
    }

    const QString prefix = pattern.left(openBrace);
    const QString suffix = pattern.mid(closeBrace + 1);
    const QString inside = pattern.mid(openBrace + 1, closeBrace - openBrace - 1);
    const QStringList options = inside.split(QLatin1Char(','), Qt::SkipEmptyParts);

    QStringList expanded;
    expanded.reserve(options.size());
    std::transform(options.cbegin(), options.cend(), std::back_inserter(expanded),
                   [&prefix, &suffix](const QString &opt) { return prefix + opt.trimmed() + suffix; });
    return expanded;
}

bool EditorConfigParser::matchGlob(const QString &pattern, const QString &fileName) {
    if (pattern == QStringLiteral("*")) {
        return true;
    }

    const QStringList subPatterns = expandBraces(pattern);
    return std::any_of(subPatterns.cbegin(), subPatterns.cend(), [&fileName](const QString &subPat) {
        QString regexStr = QRegularExpression::escape(subPat);
        regexStr.replace(QStringLiteral("\\*"), QStringLiteral(".*"));
        regexStr.replace(QStringLiteral("\\?"), QStringLiteral("."));
        regexStr.replace(QStringLiteral("\\["), QStringLiteral("["));
        regexStr.replace(QStringLiteral("\\]"), QStringLiteral("]"));

        const QRegularExpression re(QStringLiteral("^") + regexStr + QStringLiteral("$"),
                                    QRegularExpression::CaseInsensitiveOption);
        return re.match(fileName).hasMatch();
    });
}

EditorConfigParser::Settings EditorConfigParser::resolveSettings(const QString &filePath) {
    Settings resolved;
    if (filePath.isEmpty()) {
        return resolved;
    }

    const QFileInfo targetFi(filePath);
    const QString targetFileName = targetFi.fileName();
    const QList<QString> configFiles = findConfigHierarchy(targetFi.absoluteDir());

    for (const auto &cfgPath : configFiles) {
        parseConfigFile(resolved, cfgPath, targetFileName);
    }

    return resolved;
}

void EditorConfigParser::applyToEditor(ScintillaNext *editor, const Settings &settings) {
#ifdef UNIT_TESTING
    Q_UNUSED(editor);
    Q_UNUSED(settings);
#else
    if (!editor) {
        return;
    }

    if (settings.indentStyle == IndentStyle::Tab) {
        editor->setUseTabs(true);
        editor->QObject::setProperty("nn_skip_usetabs", "EditorConfig");
    } else if (settings.indentStyle == IndentStyle::Space) {
        editor->setUseTabs(false);
        editor->QObject::setProperty("nn_skip_usetabs", "EditorConfig");
    }

    if (settings.indentSize > 0) {
        editor->setIndent(settings.indentSize);
    }

    if (settings.tabWidth > 0) {
        editor->setTabWidth(settings.tabWidth);
        editor->QObject::setProperty("nn_skip_tabwidth", "EditorConfig");
    }

    if (settings.endOfLine == EndOfLine::Crlf) {
        editor->setEOLMode(SC_EOL_CRLF);
    } else if (settings.endOfLine == EndOfLine::Lf) {
        editor->setEOLMode(SC_EOL_LF);
    } else if (settings.endOfLine == EndOfLine::Cr) {
        editor->setEOLMode(SC_EOL_CR);
    }
#endif
}

void EditorConfigParser::applyToEditor(ScintillaNext *editor, const QString &filePath) {
    const Settings settings = resolveSettings(filePath);
    applyToEditor(editor, settings);
}
