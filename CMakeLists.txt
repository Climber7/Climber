cmake_minimum_required(VERSION 3.0)
project(Climber)

set(CMAKE_CXX_STANDARD 14)

set(CLIMBER_VERSION 0.0.1)
set(CLIMBER_COPYRIGHT "Copyright (C) 2020 Climber.")
set(CLIMBER_DESCRIPTION "Yet another cross platform ss/ssr/vmess/trojan GUI client.")

if (WIN32)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
    add_link_options($<$<CXX_COMPILER_ID:MSVC>:/INCREMENTAL:NO>)
endif ()

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

if (APPLE)
    add_definitions(-DCLIMBER_DARWIN)

    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/assets")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/en.lproj")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/zh_CN.lproj")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/zh_TW.lproj")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.icns" "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/icon.icns" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.png" "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/assets/icon.png" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/privoxy.conf" "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/assets/privoxy.conf" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/bin/darwin/trojan" "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/assets/bin/trojan" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/bin/darwin/privoxy" "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/assets/bin/privoxy" COPYONLY)


    set(MSGFMT_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/tools/darwin/msgfmt")

    add_executable(Climber MACOSX_BUNDLE ${SOURCES})
    set_target_properties(Climber PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
            MACOSX_BUNDLE_BUNDLE_VERSION "${CLIMBER_VERSION}"
            MACOSX_BUNDLE_LONG_VERSION_STRING "${CLIMBER_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${CLIMBER_VERSION}"
            MACOSX_BUNDLE_INFO_STRING "${CLIMBER_DESCRIPTION}"
            MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.climber"
            MACOSX_BUNDLE_COPYRIGHT "${CLIMBER_COPYRIGHT}"
            MACOSX_BUNDLE_ICON_FILE "icon.icns")

    add_custom_command(
            TARGET Climber
            POST_BUILD
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_CN.po" -o "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/zh_CN.lproj/Climber.mo"
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_TW.po" -o "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources/zh_TW.lproj/Climber.mo"
    )

elseif (WIN32)
    add_definitions(-DCLIMBER_WINDOWS)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber")

    string(REPLACE "." "," CLIMBER_VERSION2 "${CLIMBER_VERSION}")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.ico" "${CMAKE_CURRENT_BINARY_DIR}/icon.ico" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/win32.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/win32.rc" @ONLY)

    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber/assets")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber/locale/en")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber/locale/zh_CN")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Climber/locale/zh_TW")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.png" "${CMAKE_CURRENT_BINARY_DIR}/Climber/assets/icon.png" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/privoxy.conf" "${CMAKE_CURRENT_BINARY_DIR}/Climber/assets/privoxy.conf" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/bin/win32/trojan.exe" "${CMAKE_CURRENT_BINARY_DIR}/Climber/assets/bin/trojan.exe" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/bin/win32/climber_privoxy.exe" "${CMAKE_CURRENT_BINARY_DIR}/Climber/assets/bin/climber_privoxy.exe" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/bin/win32/sysproxy.exe" "${CMAKE_CURRENT_BINARY_DIR}/Climber/assets/bin/sysproxy.exe" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assets/bin/win32/libsscrypto.dll" "${CMAKE_CURRENT_BINARY_DIR}/Climber/assets/bin/libsscrypto.dll" COPYONLY)

    set(MSGFMT_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/tools/win32/msgfmt.exe")

    add_executable(Climber WIN32 ${SOURCES} "${CMAKE_CURRENT_BINARY_DIR}/win32.rc")

    add_custom_command(
            TARGET Climber
            POST_BUILD
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_CN.po" -o "${CMAKE_CURRENT_BINARY_DIR}/Climber/locale/zh_CN/Climber.mo"
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_TW.po" -o "${CMAKE_CURRENT_BINARY_DIR}/Climber/locale/zh_TW/Climber.mo"
    )

elseif (UNIX AND NOT APPLE)
    add_definitions(-DCLIMBER_LINUX)

    message(FATAL_ERROR "Linux is not supported yet")

else ()

    message(FATAL_ERROR "Unsupported system ${CMAKE_SYSTEM_NAME}")

endif ()

target_link_libraries(Climber wxmono)
