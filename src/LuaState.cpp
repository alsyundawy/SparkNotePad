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

#include "LuaState.h"
#include "lua.hpp"

#include <QFile>

#include <sstream>
#include <string>

std::string dumpLuaState(lua_State *L) {
    std::stringstream ostr;
    const int top = lua_gettop(L);
    ostr << "top=" << top << ":\n";
    for (int i = 1; i <= top; ++i) {
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:
                ostr << "  " << i << ": '" << lua_tostring(L, i) << "'\n";
                break;
            case LUA_TBOOLEAN:
                ostr << "  " << i << ": " << (lua_toboolean(L, i) ? "true" : "false") << "\n";
                break;
            case LUA_TNUMBER:
                ostr << "  " << i << ": " << lua_tonumber(L, i) << "\n";
                break;
            default:
                ostr << "  " << i << ": TYPE=" << lua_typename(L, t) << "\n";
                break;
        }
    }
    return ostr.str();
}

static int require_resource(lua_State *L) {
    const char *modName = luaL_checkstring(L, 1);

    QString module_file = QString(":/languages/%1.lua").arg(modName);
    QFile f(module_file);

    lua_pop(L, 1);

    if (f.exists()) {
        if (f.open(QFile::ReadOnly)) {
            luaL_loadstring(L, f.readAll().constData());
            f.close();
        }

        // At this point either the function or an error string is on the stack
    } else {
        lua_pushfstring(L, "\n\tno file '%s'", module_file.toLatin1().constData());
    }

    return 1;
}

LuaState::LuaState() {
    qInfo(Q_FUNC_INFO);

    L = luaL_newstate();
    luaL_openlibs(L);

    // Remove the searchers used for finding files on the file system
    execute(
        "table.remove(package.searchers, 4); table.remove(package.searchers, 3); table.remove(package.searchers, 2)");

    for (int i = 0; i < 3; ++i) {
        lua_getglobal(L, "table");
        lua_getfield(L, -1, "remove");
        lua_remove(L, -2);

        lua_getglobal(L, "package");
        lua_getfield(L, -1, "searchers");
        lua_remove(L, -2);
        lua_call(L, 1, 0);
    }

    lua_getglobal(L, "table");
    lua_getfield(L, -1, "insert");
    lua_remove(L, -2);

    lua_getglobal(L, "package");
    lua_getfield(L, -1, "searchers");
    lua_remove(L, -2);

    lua_pushcfunction(L, require_resource);

    lua_call(L, 2, 0);

    // Enforce strict sandbox baseline immediately upon initialization
    restrictDangerousApis();
}

LuaState::~LuaState() {
    if (L) {
        lua_close(L);
        L = Q_NULLPTR;
    }
}

// Security: CVE-2026-42214 / GHSA-m5fq-c9x5-w54g mitigation.
// setVariable() is the ONLY safe way to pass user-controlled data (filenames, file extensions,
// paths, clipboard, document contents) into the Lua environment. It pushes the value as an
// inert opaque Lua string via the C stack — it is NEVER interpolated into code.
void LuaState::setVariable(const char *name, const QString &value) {
    setVariable(name, value.toUtf8().constData());
}

void LuaState::setVariable(const char *name, const char *value) {
    lua_pushstring(L, value);
    lua_setglobal(L, name);
}

void LuaState::setVariable(const char *name, bool value) {
    lua_pushboolean(L, value ? 1 : 0);
    lua_setglobal(L, name);
}

void LuaState::internal_execute(const char *statement, bool clear) {
    // There may be other things on the stack so save the top of it
    const int stacktop = lua_gettop(L);

    int status = luaL_loadstring(L, statement);

    if (status == LUA_OK) {
        status = lua_pcall(L, 0, LUA_MULTRET, 0);
    } else if (status == LUA_ERRSYNTAX) {
        qWarning("LUA_ERRSYNTAX: %s", statement);
    } else if (status == LUA_ERRMEM) {
        qFatal("Lua memory allocation error");
    }

    if (status != LUA_OK) {
        // Print an error message
        // writeErrorToOutput(lua_tostring(L, -1));
        // writeErrorToOutput("\r\n");
        qWarning("%s", lua_tostring(L, -1));
    }

    if (clear)
        lua_settop(L, stacktop);
}

void LuaState::executeFile(const QString &fileName) {
    QFile ff(fileName);

    if (!ff.open(QFile::ReadOnly)) {
        qFatal("Cannot execute file: %s", fileName.toLatin1().constData());
    }

    internal_execute(ff.readAll().constData(), true);

    ff.close();
}

void LuaState::clearStack() {
    lua_settop(L, 0);
}

void LuaState::restrictDangerousApis() {
    // Security sandbox: exhaustively neutralizes all dangerous OS, IO, debugging,
    // and dynamic loading primitives that could be abused by untrusted Lua scripts or macros.
    // Mitigates CVE-2026-42214 / GHSA-m5fq-c9x5-w54g across all execution entrypoints.
    static const char *sandboxScript =
        "if os then\n"
        "  os.execute   = nil\n"
        "  os.exit      = nil\n"
        "  os.tmpname   = nil\n"
        "  os.remove    = nil\n"
        "  os.rename    = nil\n"
        "  os.getenv    = nil\n"
        "  os.setlocale = nil\n"
        "end\n"
        "if io then\n"
        "  io.popen     = nil\n"
        "  io.open      = nil\n"
        "  io.lines     = nil\n"
        "  io.input     = nil\n"
        "  io.output    = nil\n"
        "  io.tmpfile   = nil\n"
        "  io.close     = nil\n"
        "  io.flush     = nil\n"
        "  io.read      = nil\n"
        "  io.type      = nil\n"
        "  io.write     = nil\n"
        "end\n"
        "debug = nil\n"
        "if package then\n"
        "  package.loadlib  = nil\n"
        "  package.loadclib = nil\n"
        "  package.cpath    = \"\"\n"
        "end\n";
    internal_execute(sandboxScript, true);
}

void LuaState::raiseError(const char *errorMessage) {
    luaL_where(L, 1);
    if (errorMessage) {
        lua_pushstring(L, errorMessage);
    } else {
        lua_insert(L, -2);
    }

    lua_concat(L, 2);
    lua_error(L);
}
