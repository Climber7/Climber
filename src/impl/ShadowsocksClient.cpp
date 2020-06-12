//
// Created by Climber on 2020/6/8.
//

#include <utility>
#include "ShadowsocksClient.h"
#include "../Paths.h"
#include "../Configuration.h"

ShadowsocksClient::ShadowsocksClient(const wxString &bin, json obj) : BaseClient(bin, std::move(obj)) {}

json ShadowsocksClient::OverrideListening(const wxString &localAddr, int localPort) const {
    auto data = m_data;
    data["data"]["local_address"] = localAddr.ToStdString();
    data["data"]["local_port"] = localPort;
    return data;
}

wxString ShadowsocksClient::GetSystemTrayTitle() const {
    std::string host = m_data["data"]["server"];
    int port = m_data["data"]["server_port"];
    return wxString::Format("%s (%s@%s:%d)", m_name, m_type, wxString(host), port);
}

void ShadowsocksClient::Start() const {
    auto clientTmpConfigFile = Paths::GetTmpDirFile("shadowsocks.json");
//    auto clientLogFile = Paths::GetLogDirFile("shadowsocks.log");
    auto localAddr = CONFIGURATION.GetShareOnLan() ? "0.0.0.0" : "127.0.0.1";
    auto localPort = CONFIGURATION.GetSocksPort();
    this->WriteTo(clientTmpConfigFile, localAddr, localPort);

    wxExecute(wxString::Format("\"%s\" -c \"%s\"", m_bin, clientTmpConfigFile),
              wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
    // TODO shadowsocks log
}
