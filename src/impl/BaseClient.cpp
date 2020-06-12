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

#define CLIMBER_SHADOWSOCKS ("climber_shadowsocks_libev.exe")
#define CLIMBER_TROJAN ("climber_trojan.exe")

#elif defined CLIMBER_DARWIN

#define CLIMBER_SHADOWSOCKS ("climber_shadowsocks_libev")
#define CLIMBER_TROJAN ("climber_trojan")

#endif

BaseClient::BaseClient(const wxString &bin, json obj) : m_bin(bin), m_data(std::move(obj)) {
    std::string name = m_data["name"];
    m_name = wxString(name);
    std::string typeName = m_data["type"];
    m_typeName = wxString(typeName);
}

const wxString &BaseClient::GetName() const {
    return m_name;
}

int BaseClient::GetType() const {
    return GetTypeByName(m_typeName);
}

wxString BaseClient::GetTypeName() const {
    return m_typeName;
}

void BaseClient::WriteTo(const wxString &file, const wxString &localAddr, int localPort) const {
    auto data = OverrideListening(localAddr, localPort);
    writeTextFile(file, wxString(data["data"].dump(4)));
}

// TODO other type
int BaseClient::GetTypeByName(const wxString &type) {
    if (type == "shadowsocks") return SERVER_TYPE_SS;
    if (type == "trojan") return SERVER_TYPE_TROJAN;
    return SERVER_TYPE_UNKNOWN;
}

// TODO other type
BaseClient *BaseClient::NewClient(int type, const json &obj) {
    switch (type) {
        case SERVER_TYPE_SS:
            return new ShadowsocksClient(Paths::GetBinDirFile(CLIMBER_SHADOWSOCKS), obj);
        case SERVER_TYPE_TROJAN:
            return new TrojanClient(Paths::GetBinDirFile(CLIMBER_TROJAN), obj);
        default:
            return nullptr;
    }
}

// TODO other type
void BaseClient::StopAll() {
    killProcessByName(CLIMBER_SHADOWSOCKS);
    killProcessByName(CLIMBER_TROJAN);
}
