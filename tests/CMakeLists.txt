cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Core Test Widgets)

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
)

set(TEST_SOURCES
    test_main.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Base64Utils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/MimeTools.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/NumberConverter.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/GuidTimestampUtils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/CaseConverter.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/MathEvaluator.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/CryptoManager.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/CodeCompressPretty.cpp
    ${CMAKE_SOURCE_DIR}/src/LuaState.cpp
    TestLuaSandbox.cpp
)

add_executable(SparkNotePadTests ${TEST_SOURCES} ${TEST_HEADERS})

target_compile_definitions(SparkNotePadTests PRIVATE UNIT_TESTING=1)

target_include_directories(SparkNotePadTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/thirdparty/lua/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SparkNotePadTests PRIVATE
    Qt6::Core
    Qt6::Test
    Qt6::Widgets
    lua
)

if(APPLE)
    target_link_directories(SparkNotePadTests PRIVATE "${CMAKE_BINARY_DIR}/dummy_frameworks")
    target_link_libraries(SparkNotePadTests PRIVATE "-framework Security")
endif()

add_test(NAME SparkNotePadUnitTests COMMAND SparkNotePadTests)
