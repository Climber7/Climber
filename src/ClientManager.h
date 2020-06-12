//
// Created by Climber on 2020/6/8.
//

#ifndef CLIMBER_CLIENTMANAGER_H
#define CLIMBER_CLIENTMANAGER_H

#include <vector>
#include <nlohmann/json.hpp>
#include "impl/BaseClient.h"

#define CLIENT_MANAGER  (ClientManager::GetInstance())

using nlohmann::json;

class ClientManager {
public:
    static void Init();

    static void Destroy();

    static ClientManager &GetInstance();

private:
    void Load();

public:
    void Reload();

    const std::vector<BaseClient *> &GetList();

    int Count() const;

    const BaseClient *Get(int index) const;

private:
    wxString m_serversFile;
    std::vector<BaseClient *> m_list;

private:
    ClientManager();

private:
    static ClientManager *s_instance;
};


#endif //CLIMBER_CLIENTMANAGER_H
