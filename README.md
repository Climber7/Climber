# Climber

[English](README.md)
[中文](README_CN.md)

Yet another cross platform ss/trojan GUI client.

Currently support MacOS and Windows, Linux support is on plan.

# Build

## MacOS

Requirement:

1. homebrew [https://brew.sh/](https://brew.sh/)
2. command line tools `xcode-select --install`
3. cmake `brew install cmake`

Build:

```bash
git clone --recurse-submodules https://github.com/Climber7/Climber
cd Climber
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. 
make -j8
```

## Windows

Requirement:

1. Visual Studio or Visual Studio Build Tools
2. cmake

Build:

```bash
git clone --recurse-submodules https://github.com/Climber7/Climber
cd Climber
md build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. 
nmake
```

# Servers Configuration

Click `Open Config Directory` and create a file named `servers.json`

```json
[
    {
        "name": "ss-sample",
        "type": "shadowsocks",
        "data": {
            "server": "1.1.1.1",
            "server_port": 8888,
            "local_address": "127.0.0.1",
            "local_port": 1080,
            "password": "xxxxxx",
            "timeout": 300,
            "method": "xchacha20-ietf-poly1305",
            "fast_open": false
        }
    },
    {
        "name": "tj-sample",
        "type": "trojan",
        "data": {
            "run_type": "client",
            "local_addr": "127.0.0.1",
            "local_port": 1080,
            "remote_addr": "xxx.com",
            "remote_port": 443,
            "password": [
                "xxxxxx"
            ],
            "log_level": 1,
            "ssl": {
                "verify": true,
                "verify_hostname": true,
                "cert": "",
                "cipher": "ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:DHE-RSA-AES128-SHA:DHE-RSA-AES256-SHA:AES128-SHA:AES256-SHA:DES-CBC3-SHA",
                "cipher_tls13": "TLS_AES_128_GCM_SHA256:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_256_GCM_SHA384",
                "sni": "",
                "alpn": [
                    "h2",
                    "http/1.1"
                ],
                "reuse_session": true,
                "session_ticket": false,
                "curves": ""
            },
            "tcp": {
                "no_delay": true,
                "keep_alive": true,
                "reuse_port": false,
                "fast_open": false,
                "fast_open_qlen": 20
            }
        }
    }
]
```

**Note: Local address and port will be override by Climber, so you can let these empty or delete these key. You can set these values in preferences.**

# Special Thanks

* [wxWidgets](https://www.wxwidgets.org/) For cross platform GUI.
* [json](https://github.com/nlohmann/json) For json serialization/deserialization.
* [shadowsocks-libev](https://github.com/shadowsocks/shadowsocks-libev) For shadowsocks support.
* [shadowsocks-libev-win](https://github.com/DDoSolitary/shadowsocks-libev-win) Windows builds for shadowsocks-libev.
* [trojan](https://github.com/trojan-gfw/trojan) For trojan support.
* [privoxy](https://www.privoxy.org/) For converting socks5 proxy to http.
* [sysproxy](https://github.com/Noisyfox/sysproxy) For system proxy setting on Windows.

And thanks to many other good open source project!

# QA

Q: Why not make a GUI for configuring servers?

A: I like config file, it's perfect for me. In fact, I'm lazy.

# Announcement

This project is just for research and education purpose, 
using it requires you to ensure compliance with local laws.
Otherwise you are not allow to use it, and should uninstall it immediately.

**Any violation of the law by anyone using this software has nothing to do with the author.**
