cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Core Test Widgets PrintSupport Network Svg)

set(TEST_HEADERS
    test_base64.h
    test_number_converter.h
    test_guid_timestamp.h
    test_case_converter.h
    test_math_evaluator.h
    test_crypto_manager.h
    test_code_compress_pretty.h
    test_lua_security.h
    TestLuaSandbox.h
    test_mime_tools.h
    test_toolbar_icon_manager.h
    test_closed_tabs_manager.h
    test_tab_color_manager.h
    test_autosave_recovery_manager.h
    test_extended_search_parser.h
    test_language_lexer_coverage.h
    test_document_export_manager.h
    test_markdown_preview_enhancer.h
    test_developer_tools_engine.h
    test_corresponding_file_switcher.h
    test_editorconfig_parser.h
    test_clipboard_history_dock.h
    test_quick_switcher.h
    test_v147_features.h
)

set(TEST_SOURCES
    test_main.cpp
    ${CMAKE_SOURCE_DIR}/src/resources.qrc
    ${CMAKE_SOURCE_DIR}/src/toolbar_icons.qrc
    ${CMAKE_SOURCE_DIR}/src/scripts.qrc
    ${CMAKE_SOURCE_DIR}/src/features/ToolbarIconManager.cpp
    ${CMAKE_SOURCE_DIR}/src/features/ClosedTabsManager.cpp
    ${CMAKE_SOURCE_DIR}/src/features/TabColorManager.cpp
    ${CMAKE_SOURCE_DIR}/src/features/AutosaveRecoveryManager.cpp
    ${CMAKE_SOURCE_DIR}/src/features/DocumentExportManager.cpp
    ${CMAKE_SOURCE_DIR}/src/features/DeveloperToolsEngine.cpp
    ${CMAKE_SOURCE_DIR}/src/features/CorrespondingFileSwitcher.cpp
    ${CMAKE_SOURCE_DIR}/src/features/EditorConfigParser.cpp
    ${CMAKE_SOURCE_DIR}/src/features/CharacterInfo.cpp
    ${CMAKE_SOURCE_DIR}/src/docks/ClipboardHistoryDock.cpp
    ${CMAKE_SOURCE_DIR}/src/docks/CharacterInfoDock.cpp
    ${CMAKE_SOURCE_DIR}/src/docks/DocumentMinimapDock.cpp
    ${CMAKE_SOURCE_DIR}/src/dialogs/QuickSwitcherDialog.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/ExtendedSearchParser.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/MarkdownPreviewEnhancer.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/MarkdownThemeManager.cpp
    ${CMAKE_SOURCE_DIR}/src/ApplicationSettings.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Base64Utils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/MimeTools.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/NumberConverter.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/GuidTimestampUtils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/CaseConverter.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/MathEvaluator.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/CryptoManager.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/CodeCompressPretty.cpp
    ${CMAKE_SOURCE_DIR}/src/SparkTransferManager.cpp
    ${CMAKE_SOURCE_DIR}/src/PosixFileInspector.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/EncodingDetector.cpp
    ${CMAKE_SOURCE_DIR}/src/LuaState.cpp
    TestLuaSandbox.cpp
)

add_executable(SparkNotePadTests ${TEST_SOURCES} ${TEST_HEADERS})

target_compile_definitions(SparkNotePadTests PRIVATE UNIT_TESTING=1)

target_include_directories(SparkNotePadTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/thirdparty/lua/src
    ${CMAKE_SOURCE_DIR}/thirdparty/uchardet/src
    ${CMAKE_SOURCE_DIR}/thirdparty/scintilla/include
    ${CMAKE_SOURCE_DIR}/thirdparty/scintilla/qt/ScintillaEdit
    ${CMAKE_SOURCE_DIR}/thirdparty/scintilla/qt/ScintillaEditBase
    ${CMAKE_SOURCE_DIR}/thirdparty/lexilla/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SparkNotePadTests PRIVATE
    Qt6::Core
    Qt6::Test
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::Network
    Qt6::Svg
    lua
    uchardet
    scintilla
    lexilla
)

if(APPLE)
    target_link_directories(SparkNotePadTests PRIVATE "${CMAKE_BINARY_DIR}/dummy_frameworks")
    target_link_libraries(SparkNotePadTests PRIVATE "-framework Security")
endif()

add_test(NAME SparkNotePadUnitTests COMMAND SparkNotePadTests)
