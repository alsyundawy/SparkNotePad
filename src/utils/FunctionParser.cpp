/*
 * This file is part of NoteSciPad.
 *
 * Function and symbol parser for NoteSciPad Function List Dock.
 * Adapted from Notepad++ Function List concept and modern symbol extraction patterns.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "FunctionParser.h"
#include "ScintillaNext.h"

#include <QRegularExpression>
#include <QStringList>

QVector<FunctionSymbol> FunctionParser::parse(ScintillaNext *editor) {
    if (!editor || editor->textLength() <= 0)
        return {};

    const int len = editor->textLength();
    QByteArray bytes(len + 1, '\0');
    editor->getRange(0, len, bytes.data());
    bytes.resize(len);

    const QString text = QString::fromUtf8(bytes);
    QString lang = editor->languageName;
    if (lang.isEmpty() && editor->isFile()) {
        const QString ext = editor->getFileInfo().suffix().toLower();
        if (ext == QStringLiteral("py"))
            lang = QStringLiteral("Python");
        else if (ext == QStringLiteral("cpp") || ext == QStringLiteral("cxx") || ext == QStringLiteral("cc") ||
                 ext == QStringLiteral("c") || ext == QStringLiteral("h") || ext == QStringLiteral("hpp"))
            lang = QStringLiteral("C++");
        else if (ext == QStringLiteral("js") || ext == QStringLiteral("mjs") || ext == QStringLiteral("cjs"))
            lang = QStringLiteral("JavaScript");
        else if (ext == QStringLiteral("ts") || ext == QStringLiteral("tsx") || ext == QStringLiteral("jsx"))
            lang = QStringLiteral("TypeScript");
        else if (ext == QStringLiteral("md") || ext == QStringLiteral("markdown"))
            lang = QStringLiteral("Markdown");
        else if (ext == QStringLiteral("php"))
            lang = QStringLiteral("PHP");
        else if (ext == QStringLiteral("rs"))
            lang = QStringLiteral("Rust");
        else if (ext == QStringLiteral("go"))
            lang = QStringLiteral("Go");
        else if (ext == QStringLiteral("lua"))
            lang = QStringLiteral("Lua");
        else if (ext == QStringLiteral("sh") || ext == QStringLiteral("bash") || ext == QStringLiteral("zsh"))
            lang = QStringLiteral("Bash");
        else if (ext == QStringLiteral("java"))
            lang = QStringLiteral("Java");
        else if (ext == QStringLiteral("cs"))
            lang = QStringLiteral("C#");
    }

    return parseText(text, lang);
}

QVector<FunctionSymbol> FunctionParser::parseText(const QString &text, const QString &languageName) {
    QVector<FunctionSymbol> symbols;
    if (text.isEmpty())
        return symbols;

    const QString lang = languageName.toLower();
    const QStringList lines = text.split(QLatin1Char('\n'));

    int byteOffset = 0;
    QString currentClass;
    int currentClassIndent = -1;

    // Precompiled regexes for high performance
    static const QRegularExpression mdRe(QStringLiteral("^(#{1,6})\\s+(.+)$"));

    static const QRegularExpression pyClassRe(QStringLiteral(R"(^(\s*)class\s+([A-Za-z_][A-Za-z0-9_]*))"));
    static const QRegularExpression pyDefRe(
        QStringLiteral(R"(^(\s*)(?:async\s+)?def\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\))"));

    static const QRegularExpression cppClassRe(
        QStringLiteral(R"(^\s*(?:class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)(?:\s*final)?\s*(?::|{|$))"));
    static const QRegularExpression cppFuncRe(QStringLiteral(
        R"(^\s*(?:(?:inline|static|virtual|explicit|constexpr|const|friend)\s+)*(?:[A-Za-z_][A-Za-z0-9_:*&<>\s]+)\s+([A-Za-z_][A-Za-z0-9_:]*)\s*\(([^;{]*)\)\s*(?:const)?\s*(?:override|noexcept)?\s*(?:{|$))"));

    static const QRegularExpression jsClassRe(
        QStringLiteral(R"(^\s*(?:export\s+)?(?:default\s+)?class\s+([A-Za-z_][A-Za-z0-9_]*))"));
    static const QRegularExpression jsFuncRe(QStringLiteral(
        R"(^\s*(?:export\s+)?(?:default\s+)?(?:async\s+)?function(?:\s+([A-Za-z_][A-Za-z0-9_]*))?\s*\(([^)]*)\))"));
    static const QRegularExpression jsArrowRe(QStringLiteral(
        R"(^\s*(?:export\s+)?(?:const|let|var)\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(?:async\s*)?(?:\(([^)]*)\)|[A-Za-z_][A-Za-z0-9_]*)\s*=>)"));
    static const QRegularExpression jsMethodRe(
        QStringLiteral(R"(^\s*(?:async\s+)?(?:static\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)\s*\{)"));

    static const QRegularExpression phpClassRe(
        QStringLiteral(R"(^\s*(?:(?:abstract|final)\s+)*(?:class|interface|trait|enum)\s+([A-Za-z_][A-Za-z0-9_]*))"));
    static const QRegularExpression phpFuncRe(QStringLiteral(
        R"(^\s*(?:(?:public|protected|private|static|abstract|final)\s+)*function\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\))"));

    static const QRegularExpression rsFnRe(QStringLiteral(
        R"(^\s*(?:pub(?:\([^)]+\))?\s+)?(?:async\s+)?fn\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?:<[^>]+>)?\s*\(([^)]*)\))"));
    static const QRegularExpression rsStructRe(
        QStringLiteral(R"(^\s*(?:pub(?:\([^)]+\))?\s+)?(?:struct|enum|trait|union)\s+([A-Za-z_][A-Za-z0-9_]*))"));
    static const QRegularExpression rsImplRe(
        QStringLiteral(R"(^\s*impl(?:\s*<[^>]+>)?\s+(?:[A-Za-z_][A-Za-z0-9_:]*\s+for\s+)?([A-Za-z_][A-Za-z0-9_:]*))"));

    static const QRegularExpression goFuncRe(QStringLiteral(R"(^\s*func\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\))"));
    static const QRegularExpression goMethodRe(QStringLiteral(
        R"(^\s*func\s*\(\s*(?:[A-Za-z_][A-Za-z0-9_]*\s+)?\*?([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\))"));
    static const QRegularExpression goTypeRe(
        QStringLiteral(R"(^\s*type\s+([A-Za-z_][A-Za-z0-9_]*)\s+(?:struct|interface))"));

    static const QRegularExpression luaFuncRe(
        QStringLiteral(R"(^\s*(?:local\s+)?function\s+([A-Za-z_][A-Za-z0-9_.:]*)\s*\(([^)]*)\))"));

    static const QRegularExpression shFunc1Re(
        QStringLiteral(R"(^\s*(?:function\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*\)\s*\{?)"));
    static const QRegularExpression shFunc2Re(QStringLiteral(R"(^\s*function\s+([A-Za-z_][A-Za-z0-9_]+)\s*\{?)"));

    static const QRegularExpression genericFuncRe(
        QStringLiteral(R"((?:sub|def|func|function|fn|procedure)\s+([A-Za-z_][A-Za-z0-9_]*))"),
        QRegularExpression::CaseInsensitiveOption);

    for (int i = 0; i < lines.size(); ++i) {
        const QString &lineStr = lines.at(i);
        const int lineNumber = i + 1;
        const int linePos = byteOffset;

        // Advance byteOffset for next line
        byteOffset += lineStr.toUtf8().size() + 1;

        if (lineStr.trimmed().isEmpty())
            continue;

        // 1. Markdown
        if (lang.contains(QStringLiteral("markdown")) || lang == QStringLiteral("md")) {
            auto match = mdRe.match(lineStr);
            if (match.hasMatch()) {
                FunctionSymbol sym;
                sym.signature = match.captured(1);
                sym.name = match.captured(2).trimmed();
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Heading;
                symbols.push_back(sym);
            }
            continue;
        }

        // 2. Python
        if (lang.contains(QStringLiteral("python"))) {
            auto classMatch = pyClassRe.match(lineStr);
            if (classMatch.hasMatch()) {
                currentClassIndent = classMatch.captured(1).length();
                currentClass = classMatch.captured(2);
                FunctionSymbol sym;
                sym.name = currentClass;
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Class;
                symbols.push_back(sym);
                continue;
            }

            auto defMatch = pyDefRe.match(lineStr);
            if (defMatch.hasMatch()) {
                int indent = defMatch.captured(1).length();
                if (indent <= currentClassIndent) {
                    currentClass.clear();
                    currentClassIndent = -1;
                }
                FunctionSymbol sym;
                sym.name = defMatch.captured(2);
                sym.signature = QStringLiteral("(%1)").arg(defMatch.captured(3));
                sym.line = lineNumber;
                sym.position = linePos;
                if (!currentClass.isEmpty() && indent > currentClassIndent) {
                    sym.parentScope = currentClass;
                    sym.type = FunctionSymbol::Method;
                } else {
                    sym.type = FunctionSymbol::Function;
                }
                symbols.push_back(sym);
                continue;
            }
            continue;
        }

        // 3. Rust
        if (lang.contains(QStringLiteral("rust"))) {
            auto implMatch = rsImplRe.match(lineStr);
            if (implMatch.hasMatch()) {
                currentClass = implMatch.captured(1);
                FunctionSymbol sym;
                sym.name = QStringLiteral("impl %1").arg(currentClass);
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Class;
                symbols.push_back(sym);
                continue;
            }
            auto structMatch = rsStructRe.match(lineStr);
            if (structMatch.hasMatch()) {
                FunctionSymbol sym;
                sym.name = structMatch.captured(1);
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Struct;
                symbols.push_back(sym);
                continue;
            }
            auto fnMatch = rsFnRe.match(lineStr);
            if (fnMatch.hasMatch()) {
                FunctionSymbol sym;
                sym.name = fnMatch.captured(1);
                sym.signature = QStringLiteral("(%1)").arg(fnMatch.captured(2).trimmed());
                sym.parentScope = currentClass;
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = currentClass.isEmpty() ? FunctionSymbol::Function : FunctionSymbol::Method;
                symbols.push_back(sym);
                continue;
            }
            continue;
        }

        // 4. Go
        if (lang.contains(QStringLiteral("go"))) {
            auto typeMatch = goTypeRe.match(lineStr);
            if (typeMatch.hasMatch()) {
                FunctionSymbol sym;
                sym.name = typeMatch.captured(1);
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Struct;
                symbols.push_back(sym);
                continue;
            }
            auto methodMatch = goMethodRe.match(lineStr);
            if (methodMatch.hasMatch()) {
                FunctionSymbol sym;
                sym.parentScope = methodMatch.captured(1);
                sym.name = methodMatch.captured(2);
                sym.signature = QStringLiteral("(%1)").arg(methodMatch.captured(3).trimmed());
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Method;
                symbols.push_back(sym);
                continue;
            }
            auto funcMatch = goFuncRe.match(lineStr);
            if (funcMatch.hasMatch()) {
                FunctionSymbol sym;
                sym.name = funcMatch.captured(1);
                sym.signature = QStringLiteral("(%1)").arg(funcMatch.captured(2).trimmed());
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Function;
                symbols.push_back(sym);
                continue;
            }
            continue;
        }

        // 5. JavaScript / TypeScript
        if (lang.contains(QStringLiteral("javascript")) || lang.contains(QStringLiteral("typescript")) ||
            lang == QStringLiteral("js") || lang == QStringLiteral("ts")) {
            auto classMatch = jsClassRe.match(lineStr);
            if (classMatch.hasMatch()) {
                currentClass = classMatch.captured(1);
                FunctionSymbol sym;
                sym.name = currentClass;
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Class;
                symbols.push_back(sym);
                continue;
            }
            auto funcMatch = jsFuncRe.match(lineStr);
            if (funcMatch.hasMatch() && !funcMatch.captured(1).isEmpty()) {
                FunctionSymbol sym;
                sym.name = funcMatch.captured(1);
                sym.signature = QStringLiteral("(%1)").arg(funcMatch.captured(2).trimmed());
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Function;
                symbols.push_back(sym);
                continue;
            }
            auto arrowMatch = jsArrowRe.match(lineStr);
            if (arrowMatch.hasMatch()) {
                FunctionSymbol sym;
                sym.name = arrowMatch.captured(1);
                sym.signature = QStringLiteral("(%1)").arg(arrowMatch.captured(2).trimmed());
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Function;
                symbols.push_back(sym);
                continue;
            }
            auto methodMatch = jsMethodRe.match(lineStr);
            if (methodMatch.hasMatch()) {
                const QString mName = methodMatch.captured(1);
                if (mName != QStringLiteral("if") && mName != QStringLiteral("for") &&
                    mName != QStringLiteral("while") && mName != QStringLiteral("switch")) {
                    FunctionSymbol sym;
                    sym.name = mName;
                    sym.signature = QStringLiteral("(%1)").arg(methodMatch.captured(2).trimmed());
                    sym.parentScope = currentClass;
                    sym.line = lineNumber;
                    sym.position = linePos;
                    sym.type = currentClass.isEmpty() ? FunctionSymbol::Function : FunctionSymbol::Method;
                    symbols.push_back(sym);
                    continue;
                }
            }
            continue;
        }

        // 6. PHP
        if (lang.contains(QStringLiteral("php"))) {
            auto classMatch = phpClassRe.match(lineStr);
            if (classMatch.hasMatch()) {
                currentClass = classMatch.captured(1);
                FunctionSymbol sym;
                sym.name = currentClass;
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Class;
                symbols.push_back(sym);
                continue;
            }
            auto funcMatch = phpFuncRe.match(lineStr);
            if (funcMatch.hasMatch()) {
                FunctionSymbol sym;
                sym.name = funcMatch.captured(1);
                sym.signature = QStringLiteral("(%1)").arg(funcMatch.captured(2).trimmed());
                sym.parentScope = currentClass;
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = currentClass.isEmpty() ? FunctionSymbol::Function : FunctionSymbol::Method;
                symbols.push_back(sym);
                continue;
            }
            continue;
        }

        // 7. Lua
        if (lang.contains(QStringLiteral("lua"))) {
            auto luaMatch = luaFuncRe.match(lineStr);
            if (luaMatch.hasMatch()) {
                FunctionSymbol sym;
                sym.name = luaMatch.captured(1);
                sym.signature = QStringLiteral("(%1)").arg(luaMatch.captured(2).trimmed());
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = sym.name.contains(QLatin1Char(':')) || sym.name.contains(QLatin1Char('.'))
                               ? FunctionSymbol::Method
                               : FunctionSymbol::Function;
                symbols.push_back(sym);
                continue;
            }
            continue;
        }

        // 8. Shell / Bash
        if (lang.contains(QStringLiteral("bash")) || lang.contains(QStringLiteral("shell")) ||
            lang == QStringLiteral("sh")) {
            auto sh1 = shFunc1Re.match(lineStr);
            if (sh1.hasMatch()) {
                FunctionSymbol sym;
                sym.name = sh1.captured(1);
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Function;
                symbols.push_back(sym);
                continue;
            }
            auto sh2 = shFunc2Re.match(lineStr);
            if (sh2.hasMatch()) {
                FunctionSymbol sym;
                sym.name = sh2.captured(1);
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = FunctionSymbol::Function;
                symbols.push_back(sym);
                continue;
            }
            continue;
        }

        // 9. C / C++
        if (lang.contains(QStringLiteral("c++")) || lang.contains(QStringLiteral("cpp")) ||
            lang == QStringLiteral("c") || lang.contains(QStringLiteral("java")) ||
            lang.contains(QStringLiteral("c#"))) {
            auto classMatch = cppClassRe.match(lineStr);
            if (classMatch.hasMatch()) {
                currentClass = classMatch.captured(1);
                FunctionSymbol sym;
                sym.name = currentClass;
                sym.line = lineNumber;
                sym.position = linePos;
                sym.type = lineStr.contains(QStringLiteral("struct")) ? FunctionSymbol::Struct : FunctionSymbol::Class;
                symbols.push_back(sym);
                continue;
            }
            auto funcMatch = cppFuncRe.match(lineStr);
            if (funcMatch.hasMatch()) {
                const QString fnName = funcMatch.captured(1);
                if (fnName != QStringLiteral("if") && fnName != QStringLiteral("while") &&
                    fnName != QStringLiteral("for") && fnName != QStringLiteral("switch") &&
                    fnName != QStringLiteral("catch")) {
                    FunctionSymbol sym;
                    if (fnName.contains(QStringLiteral("::"))) {
                        sym.parentScope = fnName.section(QStringLiteral("::"), 0, -2);
                        sym.name = fnName.section(QStringLiteral("::"), -1);
                        sym.type = FunctionSymbol::Method;
                    } else {
                        sym.name = fnName;
                        sym.parentScope = currentClass;
                        sym.type = currentClass.isEmpty() ? FunctionSymbol::Function : FunctionSymbol::Method;
                    }
                    sym.signature = QStringLiteral("(%1)").arg(funcMatch.captured(2).trimmed());
                    sym.line = lineNumber;
                    sym.position = linePos;
                    symbols.push_back(sym);
                    continue;
                }
            }
            continue;
        }

        // 10. Generic Fallback
        auto genMatch = genericFuncRe.match(lineStr);
        if (genMatch.hasMatch()) {
            FunctionSymbol sym;
            sym.name = genMatch.captured(1);
            sym.line = lineNumber;
            sym.position = linePos;
            sym.type = FunctionSymbol::Function;
            symbols.push_back(sym);
        }
    }

    return symbols;
}
