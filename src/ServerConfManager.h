//
// Created by Climber on 2020/6/8.
//

#ifndef CLIMBER_SERVERCONFMANAGER_H
#define CLIMBER_SERVERCONFMANAGER_H

#include <vector>
#include <nlohmann/json.hpp>
#include "ServerConfItem.h"

#define SERVER_CONF_MANAGER  (ServerConfManager::GetInstance())

using nlohmann::json;

class ServerConfManager {
public:
    static void Init();

    static void Destroy();

    static ServerConfManager &GetInstance();

private:
    void Load();

//    void Save();

public:
    void Reload();

    const std::vector<ServerConfItem *> &GetServersList();

    int Count() const;

    ServerConfItem *Get(int index) const;

private:
    wxString m_serversListFile;
    std::vector<ServerConfItem *> m_list;

private:
    ServerConfManager();

private:
    static ServerConfManager *s_instance;
};


#endif //CLIMBER_SERVERCONFMANAGER_H
