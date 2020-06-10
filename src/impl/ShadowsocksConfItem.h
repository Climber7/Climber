//
// Created by Climber on 2020/6/8.
//

#ifndef CLIMBER_SHADOWSOCKSCONFITEM_H
#define CLIMBER_SHADOWSOCKSCONFITEM_H

#include "../ServerConfItem.h"

class ShadowsocksConfItem : public ServerConfItem {
public:
    explicit ShadowsocksConfItem(json obj);

    json OverrideLocalHost(const wxString &localAddr, int localPort) const override;

    wxString GetSystemTrayTitle() const override;

    wxVector<wxVariant> GetDataViewRowData() const override;

};


#endif //CLIMBER_SHADOWSOCKSCONFITEM_H
