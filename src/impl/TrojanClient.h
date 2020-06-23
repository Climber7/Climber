//
// Created by Climber on 2020/6/8.
//

#ifndef CLIMBER_TROJANCLIENT_H
#define CLIMBER_TROJANCLIENT_H

#include "BaseClient.h"

class TrojanClient : public BaseClient {
public:
    explicit TrojanClient(const wxString &bin, json obj);

    wxVector<wxVariant> GetDataViewRowData() const override;

    wxString GetSystemTrayTitle() const override;

    void Start() const override;

protected:
    json OverrideListening(const wxString &localAddr, int localPort) const override;


};


#endif //CLIMBER_TROJANCLIENT_H
