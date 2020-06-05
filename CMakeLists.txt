cmake_minimum_required(VERSION 3.17)
project(Climber)

set(CMAKE_CXX_STANDARD 14)

set(CLIMBER_VERSION 1.0.0)

set(wxBUILD_MONOLITHIC ON)
set(wxBUILD_PRECOMP OFF)
set(wxBUILD_INSTALL OFF)
set(wxBUILD_SHARED OFF)
set(wxUSE_ZLIB builtin)
set(wxUSE_EXPAT builtin)
set(wxUSE_LIBTIFF OFF)

add_subdirectory(vendor/wxWidgets)
include_directories(vendor/json/single_include)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    set(MSGFMT_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/tools/darwin/msgfmt")

    add_executable(Climber MACOSX_BUNDLE ${SOURCES})
    set_target_properties(Climber PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
            MACOSX_BUNDLE_BUNDLE_VERSION "${CLIMBER_VERSION}"
            MACOSX_BUNDLE_LONG_VERSION_STRING "${CLIMBER_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${CLIMBER_VERSION}"
            MACOSX_BUNDLE_INFO_STRING "Yet another cross platform ss/ssr/vmess/trojan GUI client."
            MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.climber"
            MACOSX_BUNDLE_ICON_FILE "icon.icns")

    add_custom_command(
            TARGET Climber
            POST_BUILD
            COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources"
            COMMAND cp -f "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.icns" "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/"
            COMMAND cp -f "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.png" "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/"
            COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/en.lproj"
            COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/zh_CN.lproj"
            COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/zh_TW.lproj"
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_CN.po" -o "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/zh_CN.lproj/Climber.mo"
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_TW.po" -o "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/zh_TW.lproj/Climber.mo"
    )

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

    set(MSGFMT_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/tools/win32/msgfmt.exe")

    add_executable(Climber ${SOURCES})

else ()

    message(FATAL_ERROR "Unsupported system ${CMAKE_SYSTEM_NAME}")

endif ()

target_link_libraries(Climber wxmono)
