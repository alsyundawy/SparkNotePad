#pragma once
#include <QObject>
#include <QTest>
#include "LuaState.h"

class TestLuaSandbox : public QObject {
    Q_OBJECT

private slots:
    void testOsExecuteBlocked();
    void testIoBlocked();
    void testDebugLibraryBlocked();
    void testDynamicLoadlibBlocked();
    void testSafeStandardLibrariesWork();
};
