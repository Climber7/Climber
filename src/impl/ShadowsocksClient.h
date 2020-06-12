//
// Created by Climber on 2020/6/8.
//

#ifndef CLIMBER_SHADOWSOCKSCLIENT_H
#define CLIMBER_SHADOWSOCKSCLIENT_H

#include "BaseClient.h"

class ShadowsocksClient : public BaseClient {
public:
    explicit ShadowsocksClient(const wxString &bin, json obj);

    wxString GetSystemTrayTitle() const override;

    void Start() const override;

protected:
    json OverrideListening(const wxString &localAddr, int localPort) const override;

};


#endif //CLIMBER_SHADOWSOCKSCLIENT_H
