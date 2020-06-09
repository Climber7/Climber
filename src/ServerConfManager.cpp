//
// Created by Climber on 2020/6/8.
//

#include <fstream>
#include <sstream>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "ServerConfManager.h"
#include "impl/TrojanConfItem.h"

ServerConfManager *ServerConfManager::s_instance = nullptr;

void ServerConfManager::Init() {
    if (s_instance == nullptr) {
        s_instance = new ServerConfManager();
    }
}

ServerConfManager &ServerConfManager::GetInstance() {
    return *s_instance;
}

ServerConfManager::ServerConfManager() {
    InitPath();
    Load();
}

void ServerConfManager::InitPath() {
    auto &paths = wxStandardPaths::Get();
    wxString configurationDir = paths.GetUserLocalDataDir();
    if (!wxDirExists(configurationDir)) {
        printf("Configuration dir not exists\n");
        if (wxMkDir(configurationDir, wxS_DIR_DEFAULT)) {
            printf("Make configuration failed, %s\n", strerror(errno));
        }
    }

    wxFileName file;
    file.AssignDir(configurationDir);
    file.SetFullName("Servers.json");
    m_serversListFile = file.GetFullPath();
    printf("Servers list File: %s\n", m_serversListFile.c_str().AsChar());
}

void ServerConfManager::Load() {
    std::ifstream in(m_serversListFile.ToStdString(), std::ios::in);
    if (!in.is_open()) {
        return;
    }
    std::stringstream ss;
    ss << in.rdbuf();
    in.close();

    std::string jsonStr = ss.str();
    if (jsonStr.empty()) {
        return;
    }

    auto list = json::parse(jsonStr);
    for (auto item : list) {
        std::string typeName = item["type"];
        switch (ServerConfItem::GetServerTypeByName(typeName)) {
            // TODO other type
            case SERVER_TYPE_SS:
                break;
            case SERVER_TYPE_SSR:
                break;
            case SERVER_TYPE_VMESS:
                break;
            case SERVER_TYPE_TROJAN:
                m_list.push_back(new TrojanConfItem(item));
                break;
            default:
                printf("Unsupported type %s", typeName.c_str());
                break;
        }
    }
}

void ServerConfManager::Save() {
    json list;
    for (auto &item : m_list) {
        list.push_back(item->GetJsonObject());
    }
    std::ofstream out(m_serversListFile.ToStdString(), std::ios::out);
    if (!out.is_open()) {
        printf("Open \"%s\" failed, %s\n", m_serversListFile.c_str().AsChar(), strerror(errno));
        return;
    }
    out << list.dump(4) << "\n";
    out.close();
}

const std::vector<ServerConfItem*> &ServerConfManager::GetServersList() {
    return m_list;
}

