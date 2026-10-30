cmake_minimum_required(VERSION 3.21)

project(NoteSciPad
    VERSION 1.0.0
    DESCRIPTION "Lightweight Scintilla-based text editor for macOS and Linux"
    LANGUAGES C CXX
)

if(APPLE)
    enable_language(OBJCXX)
endif()

set(APP_VERSION "1.0.0")
set(APP_DISTRIBUTION "" CACHE STRING "Distribution type")
string(TIMESTAMP CURRENT_YEAR "%Y")
set(APP_COPYRIGHT "Copyright 2019-${CURRENT_YEAR} Justin Dailey & NoteSciPad Contributors")
set(APP_COPYRIGHT_BUILDER "Copyright ${CURRENT_YEAR} HARRY DS ALSYUNDAWY (NoteSciPad Maintainer)")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(SCI_OWNREGEX)

if(MSVC)
    # https://stackoverflow.com/questions/78598141/first-stdmutexlock-crashes-in-application-built-with-latest-visual-studio
    add_compile_definitions(_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)
    add_compile_options(/guard:cf)
    add_link_options(/guard:cf)
endif()

if(UNIX AND NOT APPLE)
    add_compile_options(-fstack-protector-strong -D_FORTIFY_SOURCE=2)
    add_link_options(-Wl,-z,relro -Wl,-z,now)
endif()

find_package(Qt6
    REQUIRED
        COMPONENTS
            Core
            Gui
            Widgets
            Network
            PrintSupport
            Core5Compat
)

set(CMAKE_AUTORCC ON)
qt_standard_project_setup()

if(APPLE)
    # Fix for Qt 6 on modern macOS SDKs (Xcode 15+) where Apple removed AGL.framework
    # 1. Remove AGL framework references from Qt6 interface link libraries
    foreach(_qt_comp Core Gui Widgets Network PrintSupport Core5Compat OpenGL Gui_OpenGL)
        if(TARGET Qt6::${_qt_comp})
            get_target_property(_libs Qt6::${_qt_comp} INTERFACE_LINK_LIBRARIES)
            if(_libs)
                set(_filtered_libs "")
                foreach(_item ${_libs})
                    if(NOT "${_item}" MATCHES "AGL")
                        list(APPEND _filtered_libs "${_item}")
                    endif()
                endforeach()
                set_target_properties(Qt6::${_qt_comp} PROPERTIES INTERFACE_LINK_LIBRARIES "${_filtered_libs}")
            endif()
        endif()
    endforeach()

    # 2. Provide a stub AGL framework fallback so -framework AGL never causes linker failure
    set(DUMMY_FRAMEWORK_DIR "${CMAKE_BINARY_DIR}/dummy_frameworks")
    set(DUMMY_AGL_DIR "${DUMMY_FRAMEWORK_DIR}/AGL.framework")
    if(NOT EXISTS "${DUMMY_AGL_DIR}/AGL")
        file(MAKE_DIRECTORY "${DUMMY_AGL_DIR}")
        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -dynamiclib -xc /dev/null -o "${DUMMY_AGL_DIR}/AGL"
            OUTPUT_QUIET ERROR_QUIET
        )
    endif()
    link_directories(BEFORE "${DUMMY_FRAMEWORK_DIR}")

    if(TARGET Cups::Cups)
        set_target_properties(Cups::Cups PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "")
    endif()

    foreach(_qt_comp Core Gui Widgets Network PrintSupport Core5Compat OpenGL Gui_OpenGL PrintSupportPrivate)
        if(TARGET Qt6::${_qt_comp})
            get_target_property(_inc_dirs Qt6::${_qt_comp} INTERFACE_INCLUDE_DIRECTORIES)
            if(_inc_dirs)
                set(_filtered_inc_dirs "")
                foreach(_item ${_inc_dirs})
                    if(NOT "${_item}" MATCHES "usr/include")
                        list(APPEND _filtered_inc_dirs "${_item}")
                    endif()
                endforeach()
                set_target_properties(Qt6::${_qt_comp} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${_filtered_inc_dirs}")
            endif()
        endif()
    endforeach()
endif()

set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cpm-cache" CACHE PATH "CPM source cache directory")

add_subdirectory(thirdparty)
add_subdirectory(src)
option(BUILD_TESTS "Build NoteSciPad unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()


include(${CMAKE_SOURCE_DIR}/cmake/Packaging.cmake)
