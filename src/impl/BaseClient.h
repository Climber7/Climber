//
// Created by Climber on 2020/6/8.
//

#ifndef CLIMBER_BASECLIENT_H
#define CLIMBER_BASECLIENT_H

#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include "../defs.h"

using nlohmann::json;

class BaseClient {
public:
    explicit BaseClient(const wxString &bin, json obj);

    virtual ~BaseClient() = default;

    const wxString &GetName() const;

    wxString GetType() const;

    void WriteTo(const wxString &file, const wxString &localAddr, int localPort) const;

    static BaseClient *NewClient(const wxString &type, const json &obj);

public:
    virtual wxString GetSystemTrayTitle() const = 0;

    virtual void Start() const = 0;

    static void StopAll();

protected:
    virtual json OverrideListening(const wxString &localAddr, int localPort) const = 0;

protected:
    wxString m_bin = wxEmptyString;
    wxString m_name = wxEmptyString;
    wxString m_type = wxEmptyString;
    json m_data;
};


#endif //CLIMBER_BASECLIENT_H
