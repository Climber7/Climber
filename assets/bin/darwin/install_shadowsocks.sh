#!/usr/bin/env bash

chmod 644 *.dylib

otool -L climber_shadowsocks_libev

install_name_tool -change "/usr/local/opt/libev/lib/libev.4.dylib" "@executable_path/libev.4.dylib" climber_shadowsocks_libev
install_name_tool -change "/usr/local/opt/c-ares/lib/libcares.2.dylib" "@executable_path/libcares.2.dylib" climber_shadowsocks_libev
install_name_tool -change "/usr/local/opt/libsodium/lib/libsodium.23.dylib" "@executable_path/libsodium.23.dylib" climber_shadowsocks_libev
install_name_tool -change "/usr/local/opt/mbedtls/lib/libmbedcrypto.3.dylib" "@executable_path/libmbedcrypto.3.dylib" climber_shadowsocks_libev
install_name_tool -change "/usr/local/opt/pcre/lib/libpcre.1.dylib" "@executable_path/libpcre.1.dylib" climber_shadowsocks_libev

otool -L climber_shadowsocks_libev
