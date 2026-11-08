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
#include "test_lua_security.h"                 // CVE-2026-42214 Lua injection regression tests
#include "TestLuaSandbox.h"                    // Lua sandbox restriction unit tests
#include "test_mime_tools.h"                   // Notepad++ MIME tools suite
#include "test_toolbar_icon_manager.h"         // 6 toolbar icon packs suite
#include "test_closed_tabs_manager.h"          // Reopen closed tabs stack suite
#include "test_tab_color_manager.h"            // Tab color tagging suite
#include "test_autosave_recovery_manager.h"    // Autosave & crash recovery suite
#include "test_extended_search_parser.h"       // Extended search escape engine suite
#include "test_language_lexer_coverage.h"      // 100+ language lexers coverage suite
#include "test_document_export_manager.h"      // Native Markdown / HTML / PDF export suite
#include "test_markdown_preview_enhancer.h"    // Markdown preview Mermaid & LaTeX formulas suite
#include "test_developer_tools_engine.h"       // Developer tools suite (JSON, URL, Unicode, UUID, Epoch)
#include "test_corresponding_file_switcher.h"  // Corresponding file switcher suite (header <-> source)
#include "test_editorconfig_parser.h"          // Native .editorconfig parser & enforcement suite
#include "test_clipboard_history_dock.h"       // Clipboard History Dock & Character Info Dock suite
#include "test_quick_switcher.h"               // Quick Switcher fuzzy matcher suite
#include <QApplication>

int runAllTests(int argc, char *argv[]) {
    int status = 0;
    {
        TestBase64 t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestMimeTools t;
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
    {
        TestToolbarIconManager t;  // Unit tests for 6 toolbar icon packs
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestClosedTabsManager t;  // Unit tests for closed tab stack
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestTabColorManager t;  // Unit tests for tab color tagging
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestAutosaveRecoveryManager t;  // Unit tests for autosave & crash recovery
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestExtendedSearchParser t;  // Unit tests for extended search escape parser
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestLanguageLexerCoverage t;  // Unit tests for 100+ language lexers coverage
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestDocumentExportManager t;  // Unit tests for native Markdown / HTML / PDF export
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestMarkdownPreviewEnhancer t;  // Unit tests for Mermaid & LaTeX preview enhancer
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestDeveloperToolsEngine t;  // Unit tests for developer tools engine
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestCorrespondingFileSwitcher t;  // Unit tests for corresponding file switcher
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestEditorConfigParser t;  // Unit tests for .editorconfig parser & rules
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestClipboardHistoryDock t;  // Unit tests for Clipboard History & Character Info Docks
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestQuickSwitcher t;  // Unit tests for Quick Switcher fuzzy matcher
        status |= QTest::qExec(&t, argc, argv);
    }
    return status;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(resources);
    Q_INIT_RESOURCE(toolbar_icons);
    Q_INIT_RESOURCE(scripts);
    std::cout << "===================================================\n";
    std::cout << "⚡ SparkNotePad Unit & Integration Test Suite\n";
    std::cout << "===================================================\n";

    return runAllTests(argc, argv);
}
