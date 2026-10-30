message(STATUS "Configuring macOS packaging")

set(INSTALL_DIR ${CMAKE_BINARY_DIR}/install)

# Custom Info.plist
set_target_properties(NoteSciPad PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/deploy/macos/info.plist
)

# Application icon
set(APP_ICON_MACOS ${CMAKE_SOURCE_DIR}/icon/NoteSciPad.icns)
if(NOT EXISTS ${APP_ICON_MACOS})
    set(APP_ICON_MACOS ${CMAKE_SOURCE_DIR}/icon/NotepadNext.icns)
endif()

set_source_files_properties(${APP_ICON_MACOS}
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
)

target_sources(NoteSciPad PRIVATE ${APP_ICON_MACOS})

set_target_properties(NoteSciPad PROPERTIES
    MACOSX_BUNDLE_ICON_FILE NoteSciPad.icns
)

install(TARGETS NoteSciPad
    BUNDLE DESTINATION .
)

install(FILES ${APP_ICON_MACOS}
    DESTINATION NoteSciPad.app/Contents/Resources
)

add_custom_target(install_local
    COMMAND ${CMAKE_COMMAND}
        --install ${CMAKE_BINARY_DIR}
        --prefix ${INSTALL_DIR}
    DEPENDS NoteSciPad
)

# Determine macOS target architecture suffix (arm64 or x64)
if(NOT DEFINED MAC_ARCH_SUFFIX OR MAC_ARCH_SUFFIX STREQUAL "")
    if(CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
        set(MAC_ARCH_SUFFIX "arm64")
    elseif(CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
        set(MAC_ARCH_SUFFIX "x64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
        set(MAC_ARCH_SUFFIX "arm64")
    else()
        set(MAC_ARCH_SUFFIX "x64")
    endif()
endif()

set(MAC_DMG_NAME "NoteSciPad-v${PROJECT_VERSION}-${MAC_ARCH_SUFFIX}.dmg")

find_program(MACDEPLOYQT_EXECUTABLE macdeployqt REQUIRED)

add_custom_target(dmg
    COMMAND ${CMAKE_COMMAND} -E rm -f ${INSTALL_DIR}/NoteSciPad.dmg ${CMAKE_BINARY_DIR}/${MAC_DMG_NAME}
    COMMAND hdiutil detach /Volumes/NoteSciPad -force 2>/dev/null || true
    COMMAND ${MACDEPLOYQT_EXECUTABLE}
        ${INSTALL_DIR}/NoteSciPad.app
    COMMAND ${CMAKE_COMMAND} -E rm -rf
        ${INSTALL_DIR}/NoteSciPad.app/Contents/Frameworks/QtPdf.framework
        ${INSTALL_DIR}/NoteSciPad.app/Contents/Frameworks/QtQml.framework
        ${INSTALL_DIR}/NoteSciPad.app/Contents/Frameworks/QtQmlMeta.framework
        ${INSTALL_DIR}/NoteSciPad.app/Contents/Frameworks/QtQmlModels.framework
        ${INSTALL_DIR}/NoteSciPad.app/Contents/Frameworks/QtQmlWorkerScript.framework
        ${INSTALL_DIR}/NoteSciPad.app/Contents/Frameworks/QtQuick.framework
        ${INSTALL_DIR}/NoteSciPad.app/Contents/Frameworks/QtVirtualKeyboard.framework
        ${INSTALL_DIR}/NoteSciPad.app/Contents/Frameworks/QtVirtualKeyboardQml.framework
        ${INSTALL_DIR}/NoteSciPad.app/Contents/PlugIns/platforminputcontexts/libqtvirtualkeyboardplugin.dylib
        ${INSTALL_DIR}/NoteSciPad.app/Contents/PlugIns/imageformats/libqpdf.dylib
    COMMAND codesign --force --deep -s - ${INSTALL_DIR}/NoteSciPad.app
    COMMAND hdiutil create -volname NoteSciPad -srcfolder ${INSTALL_DIR}/NoteSciPad.app -ov -format UDZO -imagekey zlib-level=9 ${CMAKE_BINARY_DIR}/${MAC_DMG_NAME}
    DEPENDS install_local
)
