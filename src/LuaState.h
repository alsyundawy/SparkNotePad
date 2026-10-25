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

#ifndef LUASTATE_H
#define LUASTATE_H

#include <QString>
#include <QStringList>
#include <QtGlobal>
#include "lua.hpp"

struct lua_State;

class LuaState {
public:
    LuaState();
    ~LuaState();

    // Security: CVE-2026-42214 / GHSA-m5fq-c9x5-w54g mitigation:
    // ALWAYS use setVariable() to pass user-controlled data (filenames, extensions,
    // clipboard content, document text) to Lua. NEVER concatenate user data into
    // execute()/executeAndReturn() string arguments. The template overloads below
    // enforce compile-time literal strings to prevent accidental dynamic string injection.
    void setVariable(const char *name, const QString &value);
    void setVariable(const char *name, const char *value);
    void setVariable(const char *name, bool value);

    void executeFile(const QString &fileName);
    void clearStack();

    // Security sandbox: call after luaL_openlibs() to strip dangerous OS/IO primitives
    // when executing untrusted user macro scripts or extension code.
    // Removes: os.execute, os.exit, os.tmpname, io.popen, package.loadlib, package.loadclib
    // SAFE to call on the shared LuaState; core language/editor functions are unaffected.
    void restrictDangerousApis();

    void raiseError(const char *errorMessage = Q_NULLPTR);

    lua_State *L = Q_NULLPTR;

    // Require literal char pointers — prevents runtime dynamic string injection at compile time.
    template <size_t N>
    void execute(const char (&statement)[N], bool clear = true) {
        internal_execute(statement, clear);
    }

    // Require literal char pointers — prevents runtime dynamic string injection at compile time.
    template <typename T, size_t N>
    T executeAndReturn(const char (&statement)[N]) {
        return internal_executeAndReturn<T>(statement);
    }

private:
    void internal_execute(const char *statement, bool clear);

    template <typename T>
    T internal_executeAndReturn(const char *statement);
};

template <>
inline bool LuaState::internal_executeAndReturn<bool>(const char *statement) {
    internal_execute(statement, false);
    luaL_checktype(L, 1, LUA_TBOOLEAN);

    bool b = lua_toboolean(L, 1);
    lua_pop(L, 1);

    return b;
}

template <>
inline QString LuaState::internal_executeAndReturn<QString>(const char *statement) {
    internal_execute(statement, false);
    luaL_checktype(L, 1, LUA_TSTRING);

    QString s = QString::fromUtf8(lua_tostring(L, 1));
    lua_pop(L, 1);

    return s;
}

template <>
inline QStringList LuaState::internal_executeAndReturn<QStringList>(const char *statement) {
    internal_execute(statement, false);
    luaL_checktype(L, 1, LUA_TTABLE);

    QStringList list;

    /* table is in the stack at index 't' */
    lua_pushnil(L); /* first key */
    while (lua_next(L, 1) != 0) {
        list << QString::fromUtf8(lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    return list;
}

#endif  // LUASTATE_H
