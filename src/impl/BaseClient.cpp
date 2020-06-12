//
// Created by Climber on 2020/6/8.
//

#include <utility>
#include <fstream>
#include "BaseClient.h"
#include "ShadowsocksClient.h"
#include "TrojanClient.h"
#include "../Paths.h"
#include "../utils.h"

// TODO other type
#ifdef CLIMBER_WINDOWS

#define CLIMBER_SHADOWSOCKS_BIN ("climber_shadowsocks_libev.exe")
#define CLIMBER_TROJAN_BIN ("climber_trojan.exe")

#elif defined CLIMBER_DARWIN

#define CLIMBER_SHADOWSOCKS_BIN ("climber_shadowsocks_libev")
#define CLIMBER_TROJAN_BIN ("climber_trojan")

#endif

BaseClient::BaseClient(const wxString &bin, json obj) : m_bin(bin), m_data(std::move(obj)) {
    std::string name = m_data["name"];
    m_name = wxString(name);
    std::string typeName = m_data["type"];
    m_type = wxString(typeName);
}

const wxString &BaseClient::GetName() const {
    return m_name;
}

wxString BaseClient::GetType() const {
    return m_type;
}

void BaseClient::WriteTo(const wxString &file, const wxString &localAddr, int localPort) const {
    auto data = OverrideListening(localAddr, localPort);
    writeTextFile(file, wxString(data["data"].dump(4)));
}

// TODO other type
BaseClient *BaseClient::NewClient(const wxString &type, const json &obj) {
    if (type == "shadowsocks") {
        return new ShadowsocksClient(Paths::GetBinDirFile(CLIMBER_SHADOWSOCKS_BIN), obj);
    } else if (type == "trojan") {
        return new TrojanClient(Paths::GetBinDirFile(CLIMBER_TROJAN_BIN), obj);
    } else {
        return nullptr;
    }
}

// TODO other type
void BaseClient::StopAll() {
    killProcessByName(CLIMBER_SHADOWSOCKS_BIN);
    killProcessByName(CLIMBER_TROJAN_BIN);
}
