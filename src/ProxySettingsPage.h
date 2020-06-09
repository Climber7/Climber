﻿//
// Created by Climber on 2020/6/5.
//

#ifndef CLIMBER_PROXYSETTINGSPAGE_H
#define CLIMBER_PROXYSETTINGSPAGE_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "defs.h"

class ProxySettingsPage : public wxPanel {
public:
    explicit ProxySettingsPage(wxWindow *parent, wxWindowID winid = wxID_ANY);

    void CheckUnsavedChanges();

private:
    void ApplyProxySettings();

    void CancelProxySettings();

private:
    void OnToggleShareOnLan(wxCommandEvent &event);

    void OnChangeSocksPort(wxSpinEvent &event);

    void OnChangeHttpPort(wxSpinEvent &event);

    void OnChangePacPort(wxSpinEvent &event);

    void OnApplyProxySettings(wxCommandEvent &event);

    void OnCancelProxySettings(wxCommandEvent &event);

private:
    wxCheckBox *m_shareOnLanCheckBox = nullptr;
    wxSpinCtrl *m_socksPortSpin = nullptr;
    wxSpinCtrl *m_httpPortSpin = nullptr;
    wxSpinCtrl *m_pacPortSpin = nullptr;

private:
    bool m_shareOnLan = DEFAULT_SHARE_ON_LAN;
    int m_socksPort = DEFAULT_SOCKS_PORT;
    int m_httpPort = DEFAULT_HTTP_PORT;
    int m_pacPort = DEFAULT_PAC_PORT;

private:
    bool m_hasUnsavedChanges = false;

DECLARE_EVENT_TABLE()

};


#endif //CLIMBER_PROXYSETTINGSPAGE_H
