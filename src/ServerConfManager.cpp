//
// Created by Climber on 2020/6/8.
//

#include "ServerConfManager.h"
#include "Paths.h"
#include "utils.h"
#include "impl/ShadowsocksConfItem.h"
#include "impl/TrojanConfItem.h"

ServerConfManager *ServerConfManager::s_instance = nullptr;

void ServerConfManager::Init() {
    if (s_instance == nullptr) {
        s_instance = new ServerConfManager();
    }
}

void ServerConfManager::Destroy() {
    if (s_instance != nullptr) {
        delete s_instance;
        s_instance = nullptr;
    }
}

ServerConfManager &ServerConfManager::GetInstance() {
    return *s_instance;
}

ServerConfManager::ServerConfManager() {
    m_serversListFile = Paths::GetConfigDirFile("servers.json");
    wxLogMessage("Servers list File: %s\n", m_serversListFile);

    Load();
}

void ServerConfManager::Load() {
    std::string jsonStr = readTextFile(m_serversListFile).ToStdString();
    if (jsonStr.empty()) {
        return;
    }

    auto list = json::parse(jsonStr);
    for (auto item : list) {
        std::string typeName = item["type"];
        switch (ServerConfItem::GetServerTypeByName(typeName)) {
            // TODO other type
            case SERVER_TYPE_SS:
                m_list.push_back(new ShadowsocksConfItem(item));
                break;
            case SERVER_TYPE_TROJAN:
                m_list.push_back(new TrojanConfItem(item));
                break;
            default:
                wxLogWarning("Unsupported type %s", wxString(typeName));
                break;
        }
    }
}

void ServerConfManager::Reload() {
    for (auto *item : m_list) {
        delete item;
    }
    m_list.clear();
    Load();
}

//void ServerConfManager::Save() {
//    json list;
//    for (auto &item : m_list) {
//        list.push_back(item->GetJsonObject());
//    }
//    writeTextFile(m_serversListFile, wxString(list.dump(4)));
//}

const std::vector<ServerConfItem *> &ServerConfManager::GetServersList() {
    return m_list;
}

int ServerConfManager::Count() const {
    return m_list.size();
}

const ServerConfItem *ServerConfManager::Get(int index) const {
    return m_list[index];
}
