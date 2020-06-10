# Climber

[English](README.md)
[中文](README_CN.md)

一个新的跨平台 ss/trojan 图形客户端。

目前支持 MacOS 和 Windows, Linux 支持正在计划中。

# 构建

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

# 服务器配置

点击 `打开配置目录`，创建一个 `servers.json` 文件。

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

**注意: 本地监听地址和端口会被 Climber 配置覆盖，所以在配置文件里可以留空这两项或直接删除这两个键。可以在设置中手动设置这两个值。**

# 特别鸣谢

* [wxWidgets](https://www.wxwidgets.org/) 用于跨平台GUI。
* [json](https://github.com/nlohmann/json) 用于json序列化/反序列化
* [shadowsocks-libev](https://github.com/shadowsocks/shadowsocks-libev) 用于支持shadowsocks.
* [shadowsocks-libev-win](https://github.com/DDoSolitary/shadowsocks-libev-win) 用于windows的shadowsocks-libev
* [trojan](https://github.com/trojan-gfw/trojan) 用于支持trojan。
* [privoxy](https://www.privoxy.org/) 用于转换socks5代理为http代理。
* [sysproxy](https://github.com/shadowsocks/sysproxy) 用于Windows设置系统代理。

还有很多优秀的开源项目，不能一一列出，再次感谢！

# 问答

Q: 为什么不做一个图形界面来配置服务器设置？

A: 因为我喜欢用配置文件的方式，够用，自由，并且可以保持更新。实际上是因为我懒。

# 声明

此项目仅作为实验或教育目的，
使用它需要你保证遵守当地的法律，
否则，你不被允许使用，且必须立即卸载它。

**任何人使用此软件产生的任何违法行为均与此软件的作者无关！**
