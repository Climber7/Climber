//
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
    bool HasUnsavedChanged();

private:
    void ApplyProxySettings();

    void CancelProxySettings();

private:
    void OnApplyProxySettings(wxCommandEvent &event);

    void OnCancelProxySettings(wxCommandEvent &event);

private:
    wxCheckBox *m_shareOnLanCheckBox = nullptr;
    wxSpinCtrl *m_socksPortSpin = nullptr;
    wxSpinCtrl *m_httpPortSpin = nullptr;
    wxSpinCtrl *m_pacPortSpin = nullptr;
    wxTextCtrl *m_proxyBypassTextCtrl = nullptr;

DECLARE_EVENT_TABLE()

};


#endif //CLIMBER_PROXYSETTINGSPAGE_H
