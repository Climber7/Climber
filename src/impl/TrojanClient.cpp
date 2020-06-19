//
// Created by Climber on 2020/6/8.
//

#include <utility>
#include "TrojanClient.h"
#include "../Paths.h"
#include "../Configuration.h"
#include "../utils.h"


TrojanClient::TrojanClient(const wxString &bin, json obj) : BaseClient(bin, std::move(obj)) {}

json TrojanClient::OverrideListening(const wxString &localAddr, int localPort) const {
    json data = m_data;
    data["data"]["local_addr"] = localAddr.ToStdString();
    data["data"]["local_port"] = localPort;
    return data;
}

wxString TrojanClient::GetSystemTrayTitle() const {
    std::string host = m_data["data"]["remote_addr"];
    int port = m_data["data"]["remote_port"];
    return wxString::Format("%s (%s@%s:%d)", m_name, m_type, wxString(host), port);
}

void TrojanClient::Start() const {
    auto clientTmpConfigFile = Paths::GetTmpDirFile("trojan.json");
    auto clientLogFile = Paths::GetLogDirFile("trojan.log");
    auto localAddr = CONFIGURATION.GetShareOnLan() ? "0.0.0.0" : "127.0.0.1";
    auto localPort = CONFIGURATION.GetSocksPort();
    this->WriteTo(clientTmpConfigFile, localAddr, localPort);

    execRedirect(wxString::Format("\"%s\" -config \"%s\"", m_bin, clientTmpConfigFile), clientLogFile);
}
