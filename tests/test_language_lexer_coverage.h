#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QDir>
#include <QFile>
#include <QObject>
#include <QTest>
#include "LuaState.h"

class TestLanguageLexerCoverage : public QObject {
    Q_OBJECT

private slots:
    void testLexerFilesCountAtLeast100() const {
        const QDir dir(QStringLiteral(":/languages"));
        // Check in resources or source tree
        QStringList entries;
        if (dir.exists()) {
            entries = dir.entryList({QStringLiteral("*.lua")}, QDir::Files);
        }
        if (entries.isEmpty()) {
            QDir sourceDir(QStringLiteral("../src/languages"));
            if (!sourceDir.exists()) {
                sourceDir = QDir(QStringLiteral("src/languages"));
            }
            if (!sourceDir.exists()) {
                sourceDir = QDir(QStringLiteral("../../src/languages"));
            }
            entries = sourceDir.entryList({QStringLiteral("*.lua")}, QDir::Files);
        }

        QVERIFY2(entries.size() >= 100,
                 qPrintable(QStringLiteral("Found %1 languages, expected >= 100").arg(entries.size())));
    }

    void testModernLanguagesPresent() const {
        QDir sourceDir(QStringLiteral("src/languages"));
        if (!sourceDir.exists()) {
            sourceDir = QDir(QStringLiteral("../src/languages"));
        }
        if (!sourceDir.exists()) {
            sourceDir = QDir(QStringLiteral("../../src/languages"));
        }

        const QStringList required = {
            QStringLiteral("zig.lua"),      QStringLiteral("rust.lua"),       QStringLiteral("go.lua"),
            QStringLiteral("swift.lua"),    QStringLiteral("typescript.lua"), QStringLiteral("crystal.lua"),
            QStringLiteral("cangjie.lua"),  QStringLiteral("llvm.lua"),       QStringLiteral("json5.lua"),
            QStringLiteral("markdown.lua"), QStringLiteral("toml.lua"),       QStringLiteral("yaml.lua")};

        for (const auto &req : required) {
            QVERIFY2(sourceDir.exists(req),
                     qPrintable(QStringLiteral("Missing required language definition: %1").arg(req)));
        }
    }

    void testLuaStateLoadsLanguages() const {
        LuaState state;
        state.executeFile(QStringLiteral(":/scripts/init.lua"));

        // Verify LuaState initialized and 'languages' table is populated
        lua_State *const L = state.L;
        QVERIFY(L != nullptr);

        lua_getglobal(L, "languages");
        QVERIFY(lua_istable(L, -1));

        int count = 0;
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            count++;
            lua_pop(L, 1);  // pop value, keep key for next iteration
        }
        lua_pop(L, 1);  // pop languages table

        QVERIFY2(count >= 100,
                 qPrintable(QStringLiteral("Loaded %1 languages in Lua table, expected >= 100").arg(count)));
    }
};
