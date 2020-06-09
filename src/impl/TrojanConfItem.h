//
// Created by Climber on 2020/6/8.
//

#ifndef CLIMBER_TROJANCONFITEM_H
#define CLIMBER_TROJANCONFITEM_H

#include "../ServerConfItem.h"

class TrojanConfItem : public ServerConfItem {
public:
    explicit TrojanConfItem(json obj);

    void SetLocalAddr(const wxString &localAddr) override;

    void SetLocalPort(int port) override;

    wxString GetSystemTrayTitle() const override;

    wxVector<wxVariant> GetDataViewRowData() const override;

};


#endif //CLIMBER_TROJANCONFITEM_H
