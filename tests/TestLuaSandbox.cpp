#include "TestLuaSandbox.h"

void TestLuaSandbox::testOsExecuteBlocked() {
    LuaState lua;
    lua.restrictDangerousApis();

    lua.execute("result = (os.execute == nil)");
    lua_getglobal(lua.L, "result");
    QVERIFY(lua_toboolean(lua.L, -1));
    lua_pop(lua.L, 1);

    lua.execute(
        "result_exit = (os.exit == nil) and (os.tmpname == nil) and (os.remove == nil) and (os.rename == nil) and "
        "(os.getenv == nil)");
    lua_getglobal(lua.L, "result_exit");
    QVERIFY(lua_toboolean(lua.L, -1));
    lua_pop(lua.L, 1);
}

void TestLuaSandbox::testIoBlocked() {
    LuaState lua;
    lua.restrictDangerousApis();

    lua.execute(
        "result_io = (io.popen == nil) and (io.open == nil) and (io.lines == nil) and (io.input == nil) and (io.output "
        "== nil) and (io.tmpfile == nil)");
    lua_getglobal(lua.L, "result_io");
    QVERIFY(lua_toboolean(lua.L, -1));
    lua_pop(lua.L, 1);
}

void TestLuaSandbox::testDebugLibraryBlocked() {
    LuaState lua;
    lua.restrictDangerousApis();

    lua.execute("result_debug = (debug == nil)");
    lua_getglobal(lua.L, "result_debug");
    QVERIFY(lua_toboolean(lua.L, -1));
    lua_pop(lua.L, 1);
}

void TestLuaSandbox::testDynamicLoadlibBlocked() {
    LuaState lua;
    lua.restrictDangerousApis();

    lua.execute("result_pkg = (package.loadlib == nil) and (package.loadclib == nil) and (package.cpath == '')");
    lua_getglobal(lua.L, "result_pkg");
    QVERIFY(lua_toboolean(lua.L, -1));
    lua_pop(lua.L, 1);
}

void TestLuaSandbox::testSafeStandardLibrariesWork() {
    LuaState lua;
    lua.restrictDangerousApis();

    // math library functions
    lua.execute("math_val = math.sqrt(16)");
    lua_getglobal(lua.L, "math_val");
    QCOMPARE(lua_tonumber(lua.L, -1), 4.0);
    lua_pop(lua.L, 1);

    // string library functions
    lua.execute("str_val = string.upper('sparknotepad')");
    lua_getglobal(lua.L, "str_val");
    QCOMPARE(QString::fromUtf8(lua_tostring(lua.L, -1)), QString("SPARKNOTEPAD"));
    lua_pop(lua.L, 1);

    // table library functions
    lua.execute("tbl = {10, 20, 30}; tbl_len = #tbl");
    lua_getglobal(lua.L, "tbl_len");
    QCOMPARE(lua_tointeger(lua.L, -1), 3);
    lua_pop(lua.L, 1);
}
