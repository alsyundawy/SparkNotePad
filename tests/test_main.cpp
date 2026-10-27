/*
 * SparkNotePad Unit Test Suite Runner
 * Copyright 2026 SparkNotePad Project
 */

#include <QTest>
#include <QCoreApplication>
#include <iostream>

#include "test_base64.h"
#include "test_number_converter.h"
#include "test_guid_timestamp.h"
#include "test_case_converter.h"
#include "test_math_evaluator.h"
#include "test_crypto_manager.h"
#include "test_code_compress_pretty.h"
#include "test_lua_security.h"  // CVE-2026-42214 Lua injection regression tests
#include "TestLuaSandbox.h"     // Lua sandbox restriction unit tests

int runAllTests(int argc, char *argv[]) {
    int status = 0;
    {
        TestBase64 t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestNumberConverter t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestGuidTimestamp t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestCaseConverter t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestMathEvaluator t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestCryptoManager t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestCodeCompressPretty t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestLuaSecurity t;  // Security regression: CVE-2026-42214 Lua injection
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestLuaSandbox t;  // Unit tests for Lua sandbox stripping & primitives
        status |= QTest::qExec(&t, argc, argv);
    }
    return status;
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    std::cout << "===================================================\n";
    std::cout << "⚡ SparkNotePad Unit & Integration Test Suite\n";
    std::cout << "===================================================\n";

    return runAllTests(argc, argv);
}
