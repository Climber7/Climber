//
// Created by Climber on 2020/6/8.
//

#ifndef CLIMBER_SERVERCONFITEM_H
#define CLIMBER_SERVERCONFITEM_H

#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include "defs.h"

using nlohmann::json;

class ServerConfItem {
public:
    explicit ServerConfItem(json obj);

    virtual ~ServerConfItem() = default;

    const wxString &GetName() const;

    int GetType() const;

    wxString GetTypeName() const;

    void WriteTo(const wxString &file, const wxString &localAddr, int localPort) const;

    const json &GetJsonObject() const;

    virtual json OverrideLocalHost(const wxString &localAddr, int localPort) const;

    virtual wxString GetSystemTrayTitle() const = 0;

    virtual wxVector<wxVariant> GetDataViewRowData() const = 0;

    static inline wxString GetServerTypeName(int type) {
        switch (type) {
            case SERVER_TYPE_SS:
                return "shadowsocks";
            case SERVER_TYPE_TROJAN:
                return "trojan";
            default:
                return "";
        }
    }

    static inline int GetServerTypeByName(const std::string &type) {
        return GetServerTypeByName(wxString(type));
    }

    static inline int GetServerTypeByName(const wxString &type) {
        if (type == "shadowsocks") return SERVER_TYPE_SS;
        if (type == "trojan") return SERVER_TYPE_TROJAN;
        return SERVER_TYPE_UNKNOWN;
    }


protected:
    wxString m_name = wxEmptyString;
    wxString m_typeName = wxEmptyString;
    json m_data;
};


#endif //CLIMBER_SERVERCONFITEM_H
