//
// Created by Climber on 2020/6/8.
//

#include "ClientManager.h"
#include "Paths.h"
#include "utils.h"
#include "impl/ShadowsocksClient.h"
#include "impl/TrojanClient.h"

ClientManager *ClientManager::s_instance = nullptr;

void ClientManager::Init() {
    if (s_instance == nullptr) {
        s_instance = new ClientManager();
    }
}

void ClientManager::Destroy() {
    if (s_instance != nullptr) {
        delete s_instance;
        s_instance = nullptr;
    }
}

ClientManager &ClientManager::GetInstance() {
    return *s_instance;
}

ClientManager::ClientManager() {
    m_serversFile = Paths::GetConfigDirFile("servers.json");
    wxLogMessage("Servers list File: %s\n", m_serversFile);

    Load();
}

void ClientManager::Load() {
    std::string jsonStr = readTextFile(m_serversFile).ToStdString();
    if (jsonStr.empty()) {
        return;
    }

    auto list = json::parse(jsonStr);
    for (auto &obj : list) {
        std::string type = obj["type"];
        auto *client = BaseClient::NewClient(type, obj);
        if (client == nullptr) {
            wxMessageDialog(nullptr, wxString::Format(_("Unsupported type %s!"), type), _("Warning")).ShowModal();
            continue;
        }
        m_list.push_back(client);
    }
}

void ClientManager::Reload() {
    for (auto *item : m_list) {
        delete item;
    }
    m_list.clear();
    Load();
}

const std::vector<BaseClient *> &ClientManager::GetList() {
    return m_list;
}

int ClientManager::Count() const {
    return m_list.size();
}

const BaseClient *ClientManager::Get(int index) const {
    if (index < 0 || index >= m_list.size()) return nullptr;
    return m_list[index];
}
