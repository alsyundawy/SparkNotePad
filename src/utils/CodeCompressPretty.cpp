/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) Compress and Pretty Code features.
 * Source: zufuliu/notepad4 (src/Edit.cpp, src/Styles.cpp)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "CodeCompressPretty.h"

#if !defined(UNIT_TESTING)
#include "ScintillaNext.h"
#endif

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

QString CodeCompressPretty::languageName(Language lang) {
    switch (lang) {
        case Json:
            return QStringLiteral("JSON");
        case Json5:
            return QStringLiteral("JSON5");
        case Css:
            return QStringLiteral("CSS");
        case JavaScript:
            return QStringLiteral("JavaScript");
        case Xml:
            return QStringLiteral("XML / HTML");
        case Sql:
            return QStringLiteral("SQL");
        default:
            return QStringLiteral("Auto");
    }
}

#if !defined(UNIT_TESTING)
CodeCompressPretty::Language CodeCompressPretty::detectLanguage(ScintillaNext *editor, const QString &sample) {
    if (editor && editor->isFile()) {
        const QString ext = QFileInfo(editor->filePath()).suffix().toLower();
        if (ext == QLatin1String("json"))
            return Json;
        if (ext == QLatin1String("json5"))
            return Json5;
        if (ext == QLatin1String("css") || ext == QLatin1String("scss") || ext == QLatin1String("less"))
            return Css;
        if (ext == QLatin1String("js") || ext == QLatin1String("mjs") || ext == QLatin1String("cjs") ||
            ext == QLatin1String("ts") || ext == QLatin1String("jsx"))
            return JavaScript;
        if (ext == QLatin1String("xml") || ext == QLatin1String("html") || ext == QLatin1String("svg") ||
            ext == QLatin1String("xhtml") || ext == QLatin1String("ui") || ext == QLatin1String("plist"))
            return Xml;
        if (ext == QLatin1String("sql"))
            return Sql;
    }

    return detectLanguage(sample);
}
#endif

CodeCompressPretty::Language CodeCompressPretty::detectLanguage(const QString &sample) {
    const QString text = sample.trimmed();
    if ((text.startsWith('{') || text.startsWith('[')) && (text.endsWith('}') || text.endsWith(']'))) {
        if (text.contains(QLatin1String("//")) || text.contains(QLatin1String("/*")) || text.contains('\'')) {
            return Json5;
        }
        return Json;
    }

    if (text.startsWith('<') && text.endsWith('>')) {
        return Xml;
    }

    if (text.startsWith(QStringLiteral("SELECT "), Qt::CaseInsensitive) ||
        text.startsWith(QStringLiteral("INSERT INTO "), Qt::CaseInsensitive) ||
        text.startsWith(QStringLiteral("UPDATE "), Qt::CaseInsensitive) ||
        text.startsWith(QStringLiteral("CREATE TABLE "), Qt::CaseInsensitive)) {
        return Sql;
    }

    if (text.contains('{') && text.contains('}') && text.contains(':') && text.contains(';')) {
        if (text.contains(QStringLiteral("function")) || text.contains(QStringLiteral("const ")) ||
            text.contains(QStringLiteral("let ")) || text.contains(QStringLiteral("var "))) {
            return JavaScript;
        }
        return Css;
    }

    if (text.contains(QStringLiteral("function")) || text.contains(QStringLiteral("const ")) ||
        text.contains(QStringLiteral("let ")) || text.contains(QStringLiteral("=>"))) {
        return JavaScript;
    }

    return Json;
}

static QString cleanJson5(const QString &input) {
    QString text = input;

    // 1. Remove single-line comments // ...
    static const QRegularExpression singleLineComments(QStringLiteral("//[^\r\n]*"));
    text.remove(singleLineComments);

    // 2. Remove multi-line comments /* ... */
    static const QRegularExpression multiLineComments(QStringLiteral("/\\*[\\s\\S]*?\\*/"));
    text.remove(multiLineComments);

    // 3. Remove trailing commas before } or ]
    static const QRegularExpression trailingCommas(QStringLiteral(",\\s*([\\]\\}])"));
    text.replace(trailingCommas, QStringLiteral("\\1"));

    // 4. Quote unquoted object keys: { key: ... } or , key: ...
    static const QRegularExpression unquotedKeys(QStringLiteral("([{,]\\s*)([a-zA-Z_$][a-zA-Z0-9_$]*)\\s*:"));
    text.replace(unquotedKeys, QStringLiteral("\\1\"\\2\":"));

    return text;
}

CodeCompressPretty::Result CodeCompressPretty::prettifyJson(const QString &text, bool isJson5) {
    const QString parseTarget = isJson5 ? cleanJson5(text) : text;
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(parseTarget.toUtf8(), &err);

    if (err.error != QJsonParseError::NoError) {
        return {false, false, isJson5 ? Json5 : Json, text, QString(), err.errorString()};
    }

    const QString formatted = QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
    return {true, false, isJson5 ? Json5 : Json, text, formatted, QString()};
}

CodeCompressPretty::Result CodeCompressPretty::compressJson(const QString &text, bool isJson5) {
    const QString parseTarget = isJson5 ? cleanJson5(text) : text;
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(parseTarget.toUtf8(), &err);

    if (err.error != QJsonParseError::NoError) {
        return {false, false, isJson5 ? Json5 : Json, text, QString(), err.errorString()};
    }

    const QString formatted = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    return {true, false, isJson5 ? Json5 : Json, text, formatted, QString()};
}

CodeCompressPretty::Result CodeCompressPretty::compressCss(const QString &text) {
    QString res = text;

    // 1. Remove comments /* ... */
    static const QRegularExpression comments(QStringLiteral("/\\*[\\s\\S]*?\\*/"));
    res.remove(comments);

    // 2. Normalize whitespace around separators
    static const QRegularExpression spaces(QStringLiteral("\\s*([{};:,])\\s*"));
    res.replace(spaces, QStringLiteral("\\1"));

    // 3. Collapse multiple whitespace into single space
    static const QRegularExpression multispaces(QStringLiteral("\\s+"));
    res.replace(multispaces, QStringLiteral(" "));

    return {true, false, Css, text, res.trimmed(), QString()};
}

CodeCompressPretty::Result CodeCompressPretty::prettifyCss(const QString &text) {
    // First normalize to compressed base
    auto compressed = compressCss(text);
    if (!compressed.success) {
        return compressed;
    }

    const QString src = compressed.formatted;
    QString res;
    res.reserve(src.length() * 2);

    int indent = 0;
    for (int i = 0; i < src.length(); ++i) {
        const QChar ch = src[i];
        if (ch == '{') {
            res += QStringLiteral(" {\n");
            indent++;
            res += QString(static_cast<qsizetype>(indent) * 4, ' ');
        } else if (ch == '}') {
            res += QStringLiteral("\n");
            indent = qMax(0, indent - 1);
            res += QString(static_cast<qsizetype>(indent) * 4, ' ');
            res += QStringLiteral("}\n");
        } else if (ch == ';') {
            res += QStringLiteral(";\n");
            res += QString(static_cast<qsizetype>(indent) * 4, ' ');
        } else if (ch == ':') {
            res += QStringLiteral(": ");
        } else if (ch == ',') {
            res += QStringLiteral(", ");
        } else {
            res += ch;
        }
    }

    return {true, false, Css, text, res.trimmed() + QStringLiteral("\n"), QString()};
}

CodeCompressPretty::Result CodeCompressPretty::compressJs(const QString &text) {
    QString res;
    res.reserve(text.length());

    bool inSingle = false;
    bool inDouble = false;
    bool inBacktick = false;
    const qsizetype n = text.length();

    for (qsizetype i = 0; i < n; ++i) {
        const QChar c = text[i];
        if (inSingle) {
            res += c;
            if (c == '\\' && i + 1 < n) {
                res += text[++i];
            } else if (c == '\'') {
                inSingle = false;
            }
        } else if (inDouble) {
            res += c;
            if (c == '\\' && i + 1 < n) {
                res += text[++i];
            } else if (c == '\"') {
                inDouble = false;
            }
        } else if (inBacktick) {
            res += c;
            if (c == '\\' && i + 1 < n) {
                res += text[++i];
            } else if (c == '`') {
                inBacktick = false;
            }
        } else if (c == '\'') {
            inSingle = true;
            res += c;
        } else if (c == '\"') {
            inDouble = true;
            res += c;
        } else if (c == '`') {
            inBacktick = true;
            res += c;
        } else if (c == '/' && i + 1 < n && text[i + 1] == '/') {
            // Skip single-line comment
            i += 2;
            while (i < n && text[i] != '\n')
                i++;
        } else if (c == '/' && i + 1 < n && text[i + 1] == '*') {
            // Skip multi-line comment
            i += 2;
            while (i + 1 < n && !(text[i] == '*' && text[i + 1] == '/'))
                i++;
            i++;
        } else {
            res += c;
        }
    }

    // Collapse multiple whitespace outside strings
    static const QRegularExpression multiline(QStringLiteral("\\s*\\n\\s*"));
    res.replace(multiline, QStringLiteral(" "));
    static const QRegularExpression extraSpaces(QStringLiteral(" {2,}"));
    res.replace(extraSpaces, QStringLiteral(" "));

    return {true, false, JavaScript, text, res.trimmed(), QString()};
}

CodeCompressPretty::Result CodeCompressPretty::prettifyJs(const QString &text) {
    // Token-aware simple formatter
    QString res;
    res.reserve(text.length() * 2);

    int indent = 0;
    bool inSingle = false;
    bool inDouble = false;
    bool inBacktick = false;
    int parenDepth = 0;
    const qsizetype n = text.length();

    for (qsizetype i = 0; i < n; ++i) {
        const QChar c = text[i];
        if (inSingle) {
            res += c;
            if (c == '\\' && i + 1 < n)
                res += text[++i];
            else if (c == '\'')
                inSingle = false;
        } else if (inDouble) {
            res += c;
            if (c == '\\' && i + 1 < n)
                res += text[++i];
            else if (c == '\"')
                inDouble = false;
        } else if (inBacktick) {
            res += c;
            if (c == '\\' && i + 1 < n)
                res += text[++i];
            else if (c == '`')
                inBacktick = false;
        } else if (c == '\'') {
            inSingle = true;
            res += c;
        } else if (c == '\"') {
            inDouble = true;
            res += c;
        } else if (c == '`') {
            inBacktick = true;
            res += c;
        } else if (c == '(') {
            parenDepth++;
            res += c;
        } else if (c == ')') {
            parenDepth = qMax(0, parenDepth - 1);
            res += c;
        } else if (c == '{') {
            res += QStringLiteral(" {\n");
            indent++;
            res += QString(static_cast<qsizetype>(indent) * 4, ' ');
        } else if (c == '}') {
            res += QStringLiteral("\n");
            indent = qMax(0, indent - 1);
            res += QString(static_cast<qsizetype>(indent) * 4, ' ');
            res += QStringLiteral("}\n");
            res += QString(static_cast<qsizetype>(indent) * 4, ' ');
        } else if (c == ';' && parenDepth == 0) {
            res += QStringLiteral(";\n");
            res += QString(static_cast<qsizetype>(indent) * 4, ' ');
        } else {
            res += c;
        }
    }

    return {true, false, JavaScript, text, res.trimmed() + QStringLiteral("\n"), QString()};
}

CodeCompressPretty::Result CodeCompressPretty::prettifyString(const QString &text, Language lang) {
    if (lang == Auto) {
        lang = detectLanguage(text);
    }

    switch (lang) {
        case Json:
            return prettifyJson(text, false);
        case Json5:
            return prettifyJson(text, true);
        case Css:
            return prettifyCss(text);
        case JavaScript:
            return prettifyJs(text);
        case Xml:
            return prettifyXml(text);
        case Sql:
            return prettifySql(text);
        default:
            return prettifyJson(text, false);
    }
}

CodeCompressPretty::Result CodeCompressPretty::compressString(const QString &text, Language lang) {
    if (lang == Auto) {
        lang = detectLanguage(text);
    }

    switch (lang) {
        case Json:
            return compressJson(text, false);
        case Json5:
            return compressJson(text, true);
        case Css:
            return compressCss(text);
        case JavaScript:
            return compressJs(text);
        case Xml:
            return compressXml(text);
        case Sql:
            return prettifySql(text);
        default:
            return compressJson(text, false);
    }
}

#if !defined(UNIT_TESTING)
CodeCompressPretty::Result CodeCompressPretty::prettify(ScintillaNext *editor, Language lang) {
    if (!editor) {
        return {false, false, lang, QString(), QString(), QStringLiteral("No active editor")};
    }
    if (editor->readOnly()) {
        return {false, false, lang, QString(), QString(), QStringLiteral("Document is read-only")};
    }

    sptr_t startPos = editor->selectionStart();
    sptr_t endPos = editor->selectionEnd();
    const bool wholeDoc = (startPos == endPos);

    if (wholeDoc) {
        startPos = 0;
        endPos = editor->length();
    }

    const QString text =
        QString::fromUtf8(editor->get_text_range(static_cast<int>(startPos), static_cast<int>(endPos)));
    if (text.trimmed().isEmpty()) {
        return {false, wholeDoc, lang, text, QString(), QStringLiteral("Selection or document is empty")};
    }

    if (lang == Auto) {
        lang = detectLanguage(editor, text);
    }

    auto result = prettifyString(text, lang);
    result.wholeDocument = wholeDoc;

    if (result.success) {
        editor->beginUndoAction();
        editor->setTargetRange(startPos, endPos);
        const QByteArray utf8 = result.formatted.toUtf8();
        editor->replaceTarget(static_cast<sptr_t>(utf8.length()), utf8.constData());
        editor->setSelection(startPos, startPos + static_cast<sptr_t>(utf8.length()));
        editor->endUndoAction();
    }

    return result;
}

CodeCompressPretty::Result CodeCompressPretty::compress(ScintillaNext *editor, Language lang) {
    if (!editor) {
        return {false, false, lang, QString(), QString(), QStringLiteral("No active editor")};
    }
    if (editor->readOnly()) {
        return {false, false, lang, QString(), QString(), QStringLiteral("Document is read-only")};
    }

    sptr_t startPos = editor->selectionStart();
    sptr_t endPos = editor->selectionEnd();
    const bool wholeDoc = (startPos == endPos);

    if (wholeDoc) {
        startPos = 0;
        endPos = editor->length();
    }

    const QString text =
        QString::fromUtf8(editor->get_text_range(static_cast<int>(startPos), static_cast<int>(endPos)));
    if (text.trimmed().isEmpty()) {
        return {false, wholeDoc, lang, text, QString(), QStringLiteral("Selection or document is empty")};
    }

    if (lang == Auto) {
        lang = detectLanguage(editor, text);
    }

    auto result = compressString(text, lang);
    result.wholeDocument = wholeDoc;

    if (result.success) {
        editor->beginUndoAction();
        editor->setTargetRange(startPos, endPos);
        const QByteArray utf8 = result.formatted.toUtf8();
        editor->replaceTarget(static_cast<sptr_t>(utf8.length()), utf8.constData());
        editor->setSelection(startPos, startPos + static_cast<sptr_t>(utf8.length()));
        editor->endUndoAction();
    }

    return result;
}
#endif

CodeCompressPretty::Result CodeCompressPretty::prettifyXml(const QString &text) {
    if (text.trimmed().isEmpty()) {
        return {false, false, Xml, text, QString(), QStringLiteral("Empty XML input")};
    }

    QXmlStreamReader reader(text);
    QString out;
    QXmlStreamWriter writer(&out);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(2);

    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.hasError()) {
            return {false, false, Xml, text, QString(), reader.errorString()};
        }
        writer.writeCurrentToken(reader);
    }

    return {true, false, Xml, text, out, QString()};
}

CodeCompressPretty::Result CodeCompressPretty::compressXml(const QString &text) {
    if (text.trimmed().isEmpty()) {
        return {false, false, Xml, text, QString(), QStringLiteral("Empty XML input")};
    }

    QXmlStreamReader reader(text);
    QString out;
    QXmlStreamWriter writer(&out);
    writer.setAutoFormatting(false);

    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.hasError()) {
            return {false, false, Xml, text, QString(), reader.errorString()};
        }
        if (reader.tokenType() == QXmlStreamReader::Characters && reader.isWhitespace()) {
            continue;
        }
        writer.writeCurrentToken(reader);
    }

    return {true, false, Xml, text, out, QString()};
}

CodeCompressPretty::Result CodeCompressPretty::prettifySql(const QString &text) {
    if (text.trimmed().isEmpty()) {
        return {false, false, Sql, text, QString(), QStringLiteral("Empty SQL input")};
    }

    static const QStringList majorKeywords = {
        QStringLiteral("SELECT"),      QStringLiteral("FROM"),         QStringLiteral("WHERE"),
        QStringLiteral("GROUP BY"),    QStringLiteral("HAVING"),       QStringLiteral("ORDER BY"),
        QStringLiteral("LEFT JOIN"),   QStringLiteral("RIGHT JOIN"),   QStringLiteral("INNER JOIN"),
        QStringLiteral("OUTER JOIN"),  QStringLiteral("FULL JOIN"),    QStringLiteral("CROSS JOIN"),
        QStringLiteral("JOIN"),        QStringLiteral("ON"),           QStringLiteral("INSERT INTO"),
        QStringLiteral("VALUES"),      QStringLiteral("UPDATE"),       QStringLiteral("SET"),
        QStringLiteral("DELETE FROM"), QStringLiteral("CREATE TABLE"), QStringLiteral("DROP TABLE"),
        QStringLiteral("ALTER TABLE"), QStringLiteral("UNION ALL"),    QStringLiteral("UNION"),
        QStringLiteral("LIMIT"),       QStringLiteral("OFFSET")};

    static const QStringList inlineKeywords = {
        QStringLiteral("AND"),         QStringLiteral("OR"),        QStringLiteral("AS"),
        QStringLiteral("IN"),          QStringLiteral("IS NULL"),   QStringLiteral("IS NOT NULL"),
        QStringLiteral("NOT"),         QStringLiteral("LIKE"),      QStringLiteral("BETWEEN"),
        QStringLiteral("EXISTS"),      QStringLiteral("CASE"),      QStringLiteral("WHEN"),
        QStringLiteral("THEN"),        QStringLiteral("ELSE"),      QStringLiteral("END"),
        QStringLiteral("DISTINCT"),    QStringLiteral("COUNT"),     QStringLiteral("SUM"),
        QStringLiteral("AVG"),         QStringLiteral("MIN"),       QStringLiteral("MAX"),
        QStringLiteral("ASC"),         QStringLiteral("DESC"),      QStringLiteral("PRIMARY KEY"),
        QStringLiteral("FOREIGN KEY"), QStringLiteral("REFERENCES")};

    QString result = text;

    for (const auto &kw : majorKeywords) {
        QRegularExpression re(QStringLiteral("\\b") + QRegularExpression::escape(kw) + QStringLiteral("\\b"),
                              QRegularExpression::CaseInsensitiveOption);
        result.replace(re, kw);
    }
    for (const auto &kw : inlineKeywords) {
        QRegularExpression re(QStringLiteral("\\b") + QRegularExpression::escape(kw) + QStringLiteral("\\b"),
                              QRegularExpression::CaseInsensitiveOption);
        result.replace(re, kw);
    }

    for (const auto &kw : majorKeywords) {
        if (kw == QStringLiteral("ON") || kw == QStringLiteral("VALUES"))
            continue;
        QRegularExpression re(QStringLiteral("\\s*\\b(") + QRegularExpression::escape(kw) + QStringLiteral(")\\b\\s*"));
        result.replace(re, QStringLiteral("\n\\1 "));
    }

    result.replace(QRegularExpression(QStringLiteral("\\n\\s*\\n+")), QStringLiteral("\n"));
    return {true, false, Sql, text, result.trimmed(), QString()};
}
