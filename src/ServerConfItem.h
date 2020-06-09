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

    void WriteTo(const wxString &file) const;

    const json &GetJsonObject() const;

    virtual void SetLocalAddr(const wxString &localAddr) = 0;

    virtual void SetLocalPort(int port) = 0;

    virtual wxString GetSystemTrayTitle() const = 0;

    virtual wxVector<wxVariant> GetDataViewRowData() const = 0;

    static inline wxString GetServerTypeName(int type) {
        switch (type) {
            case SERVER_TYPE_SS:
                return "shadowsocks";
            case SERVER_TYPE_SSR:
                return "shadowsocksR";
            case SERVER_TYPE_VMESS:
                return "vmess";
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
        if (type == "shadowsocksR") return SERVER_TYPE_SSR;
        if (type == "vmess") return SERVER_TYPE_VMESS;
        if (type == "trojan") return SERVER_TYPE_TROJAN;
        return SERVER_TYPE_UNKNOWN;
    }


protected:
    wxString m_name = wxEmptyString;
    wxString m_typeName = wxEmptyString;
    json m_data;
};


#endif //CLIMBER_SERVERCONFITEM_H
