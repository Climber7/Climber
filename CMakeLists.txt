cmake_minimum_required(VERSION 3.17)
project(Climber)

set(CMAKE_CXX_STANDARD 14)

set(CLIMBER_VERSION 0.0.5)
set(CLIMBER_COPYRIGHT "Copyright (C) 2020 Climber.")
set(CLIMBER_DESCRIPTION "Yet another cross platform ss/trojan GUI client.")
set(CLIMBER_REPO_URL "https://github.com/Climber7/Climber")

if (WIN32)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/w44996>)
    add_link_options($<$<CXX_COMPILER_ID:MSVC>:/INCREMENTAL:NO>)
    set(wxBUILD_USE_STATIC_RUNTIME ON)
    add_compile_options(
            $<$<CONFIG:>:/MT>
            $<$<CONFIG:Debug>:/MTd>
            $<$<CONFIG:Release>:/MT>
    )
endif ()

if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10)
endif ()

#set(wxBUILD_MONOLITHIC ON)
set(wxBUILD_PRECOMP OFF)
set(wxBUILD_INSTALL OFF)
set(wxBUILD_SHARED OFF)
set(wxUSE_ZLIB builtin)
set(wxUSE_EXPAT builtin)
set(wxUSE_LIBTIFF OFF)

add_subdirectory(vendor/wxWidgets)
include_directories(vendor/json/single_include)
add_subdirectory(vendor/zlib)
add_subdirectory(vendor/openssl-cmake)
include_directories(vendor/cpp-httplib)
include_directories(vendor/zlib)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

if (APPLE)
    add_definitions(-DCLIMBER_DARWIN)

    set(BINARY_RESOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/Climber.app/Contents/Resources")

    set(SOURCE_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    set(BINARY_ASSETS_DIR "${BINARY_RESOURCES_DIR}/assets")

    set(SOURCE_BIN_DIR "${SOURCE_ASSETS_DIR}/bin/darwin")
    set(BINARY_BIN_DIR "${BINARY_ASSETS_DIR}/bin")

    configure_file("${SOURCE_ASSETS_DIR}/icon.icns" "${BINARY_RESOURCES_DIR}/icon.icns" COPYONLY)

    file(MAKE_DIRECTORY "${BINARY_ASSETS_DIR}")
    file(MAKE_DIRECTORY "${BINARY_RESOURCES_DIR}/en.lproj")
    file(MAKE_DIRECTORY "${BINARY_RESOURCES_DIR}/zh_CN.lproj")
    file(MAKE_DIRECTORY "${BINARY_RESOURCES_DIR}/zh_TW.lproj")

    configure_file("${SOURCE_ASSETS_DIR}/icon.png" "${BINARY_ASSETS_DIR}/icon.png" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/tray_icon_darwin.png" "${BINARY_ASSETS_DIR}/tray_icon.png" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/privoxy.conf" "${BINARY_ASSETS_DIR}/privoxy.conf" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/gfwlist.txt" "${BINARY_ASSETS_DIR}/gfwlist.txt" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/user-rule.txt" "${BINARY_ASSETS_DIR}/user-rule.txt" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/proxy.pac" "${BINARY_ASSETS_DIR}/proxy.pac" COPYONLY)

    configure_file("${SOURCE_BIN_DIR}/climber_shadowsocks_libev" "${BINARY_BIN_DIR}/climber_shadowsocks_libev" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libev.4.dylib" "${BINARY_BIN_DIR}/libev.4.dylib" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libcares.2.dylib" "${BINARY_BIN_DIR}/libcares.2.dylib" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libsodium.23.dylib" "${BINARY_BIN_DIR}/libsodium.23.dylib" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libmbedcrypto.3.dylib" "${BINARY_BIN_DIR}/libmbedcrypto.3.dylib" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libpcre.1.dylib" "${BINARY_BIN_DIR}/libpcre.1.dylib" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/climber_trojan_go" "${BINARY_BIN_DIR}/climber_trojan_go" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/climber_privoxy" "${BINARY_BIN_DIR}/climber_privoxy" COPYONLY)


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
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_CN.po" -o "${BINARY_RESOURCES_DIR}/zh_CN.lproj/Climber.mo"
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_TW.po" -o "${BINARY_RESOURCES_DIR}/zh_TW.lproj/Climber.mo"
    )

    set(CLIMBER_ZIP "${CMAKE_CURRENT_BINARY_DIR}/Climber_macos_v${CLIMBER_VERSION}.zip")

    add_custom_target(create_zip
            COMMAND ${CMAKE_COMMAND} -E rm -f "${CLIMBER_ZIP}"
            COMMAND ${CMAKE_COMMAND} -E tar cfv "${CLIMBER_ZIP}" --format=zip "${CMAKE_CURRENT_BINARY_DIR}/Climber.app")

    add_dependencies(create_zip Climber)

elseif (WIN32)
    add_definitions(-DCLIMBER_WINDOWS)

    set(BINARY_RESOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/Climber")

    set(SOURCE_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    set(BINARY_ASSETS_DIR "${BINARY_RESOURCES_DIR}/assets")

    set(SOURCE_BIN_DIR "${SOURCE_ASSETS_DIR}/bin/win32")
    set(BINARY_BIN_DIR "${BINARY_ASSETS_DIR}/bin")

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_RESOURCES_DIR}")

    string(REPLACE "." "," CLIMBER_VERSION2 "${CLIMBER_VERSION}")
    configure_file("${SOURCE_ASSETS_DIR}/icon.ico" "${CMAKE_CURRENT_BINARY_DIR}/icon.ico" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/win32.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/win32.rc" @ONLY)

    file(MAKE_DIRECTORY "${BINARY_ASSETS_DIR}")
    file(MAKE_DIRECTORY "${BINARY_RESOURCES_DIR}/locale/en")
    file(MAKE_DIRECTORY "${BINARY_RESOURCES_DIR}/locale/zh_CN")
    file(MAKE_DIRECTORY "${BINARY_RESOURCES_DIR}/locale/zh_TW")

    configure_file("${SOURCE_ASSETS_DIR}/icon.png" "${BINARY_ASSETS_DIR}/icon.png" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/tray_icon_windows.png" "${BINARY_ASSETS_DIR}/tray_icon.png" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/privoxy.conf" "${BINARY_ASSETS_DIR}/privoxy.conf" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/gfwlist.txt" "${BINARY_ASSETS_DIR}/gfwlist.txt" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/user-rule.txt" "${BINARY_ASSETS_DIR}/user-rule.txt" COPYONLY)
    configure_file("${SOURCE_ASSETS_DIR}/proxy.pac" "${BINARY_ASSETS_DIR}/proxy.pac" COPYONLY)

    configure_file("${SOURCE_BIN_DIR}/climber_shadowsocks_libev.exe" "${BINARY_BIN_DIR}/climber_shadowsocks_libev.exe" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libbloom.dll" "${BINARY_BIN_DIR}/libbloom.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libcares-4.dll" "${BINARY_BIN_DIR}/libcares-4.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libcork.dll" "${BINARY_BIN_DIR}/libcork.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libev-4.dll" "${BINARY_BIN_DIR}/libev-4.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libgcc_s_dw2-1.dll" "${BINARY_BIN_DIR}/libgcc_s_dw2-1.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libipset.dll" "${BINARY_BIN_DIR}/libipset.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libmbedcrypto.dll" "${BINARY_BIN_DIR}/libmbedcrypto.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libpcre-1.dll" "${BINARY_BIN_DIR}/libpcre-1.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libsodium-23.dll" "${BINARY_BIN_DIR}/libsodium-23.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/libwinpthread-1.dll" "${BINARY_BIN_DIR}/libwinpthread-1.dll" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/climber_trojan_go.exe" "${BINARY_BIN_DIR}/climber_trojan_go.exe" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/climber_privoxy.exe" "${BINARY_BIN_DIR}/climber_privoxy.exe" COPYONLY)
    configure_file("${SOURCE_BIN_DIR}/climber_sysproxy.exe" "${BINARY_BIN_DIR}/climber_sysproxy.exe" COPYONLY)

    set(MSGFMT_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/tools/win32/msgfmt.exe")

    add_executable(Climber WIN32 ${SOURCES} "${CMAKE_CURRENT_BINARY_DIR}/win32.rc")

    add_custom_command(
            TARGET Climber
            POST_BUILD
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_CN.po" -o "${BINARY_RESOURCES_DIR}/locale/zh_CN/Climber.mo"
            COMMAND "${MSGFMT_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/localization/zh_TW.po" -o "${BINARY_RESOURCES_DIR}/locale/zh_TW/Climber.mo"
    )

    set(CLIMBER_ZIP "${CMAKE_CURRENT_BINARY_DIR}/Climber_windows_v${CLIMBER_VERSION}.zip")

    add_custom_target(create_zip
            COMMAND ${CMAKE_COMMAND} -E rm -f "${CLIMBER_ZIP}"
            COMMAND ${CMAKE_COMMAND} -E tar cfv "${CLIMBER_ZIP}" --format=zip "${BINARY_RESOURCES_DIR}")

    add_dependencies(create_zip Climber)

elseif (UNIX AND NOT APPLE)
    add_definitions(-DCLIMBER_LINUX)

    message(FATAL_ERROR "Linux is not supported yet")

else ()

    message(FATAL_ERROR "Unsupported system ${CMAKE_SYSTEM_NAME}")

endif ()

target_link_libraries(Climber wx::base wx::net wx::core wx::richtext zlibstatic ssl crypto)
target_compile_definitions(Climber PUBLIC
        CLIMBER_VERSION="${CLIMBER_VERSION}"
        CLIMBER_COPYRIGHT="${CLIMBER_COPYRIGHT}"
        CLIMBER_DESCRIPTION="${CLIMBER_DESCRIPTION}"
        CLIMBER_REPO_URL="${CLIMBER_REPO_URL}"
        CPPHTTPLIB_ZLIB_SUPPORT
        CPPHTTPLIB_OPENSSL_SUPPORT)