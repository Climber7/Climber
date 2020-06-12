//
// Created by Climber on 2020/6/5.
//

#include "ProxySettingsPage.h"
#include "Configuration.h"
#include "Climber.h"

ProxySettingsPage::ProxySettingsPage(wxWindow *parent, wxWindowID winid)
        : wxPanel(parent, winid) {

    auto *flexGridSizer = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer->SetFlexibleDirection(wxBOTH);
    flexGridSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    m_shareOnLanCheckBox = new wxCheckBox(this, ID_CHECK_BOX_SHARE_ON_LAN, wxEmptyString);
    m_shareOnLanCheckBox->SetValue(CONFIGURATION.GetShareOnLan());

    m_socksPortSpin = new wxSpinCtrl(this, ID_SPIN_SOCKS_PORT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                     wxSP_ARROW_KEYS, 1, 65535, CONFIGURATION.GetSocksPort());

    m_httpPortSpin = new wxSpinCtrl(this, ID_SPIN_HTTP_PORT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                    wxSP_ARROW_KEYS, 1, 65535, CONFIGURATION.GetHttpPort());

    m_pacPortSpin = new wxSpinCtrl(this, ID_SPIN_PAC_PORT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                   wxSP_ARROW_KEYS, 1, 65535, CONFIGURATION.GetPacPort());
    m_proxyBypassTextCtrl = new wxTextCtrl(this, ID_TEXT_CTRL_PROXY_BYPASS, CONFIGURATION.GetProxyBypass(),
                                           wxDefaultPosition,
                                           wxSize(240, -1), wxTE_MULTILINE);

    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Share on Lan")), 0, wxALL, 5);
    flexGridSizer->Add(m_shareOnLanCheckBox, 0, wxALL, 5);
    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Socks Port")), 0, wxALL, 5);
    flexGridSizer->Add(m_socksPortSpin, 0, wxALL, 5);
    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Http Port")), 0, wxALL, 5);
    flexGridSizer->Add(m_httpPortSpin, 0, wxALL, 5);
    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("PAC Port")), 0, wxALL, 5);
    flexGridSizer->Add(m_pacPortSpin, 0, wxALL, 5);
    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Proxy Bypass")), 0, wxALL, 5);
    flexGridSizer->Add(m_proxyBypassTextCtrl, 0, wxALL, 5);

    auto *stdButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    stdButtonSizer->Add(new wxButton(this, ID_BUTTON_CANCEL_PROXY_SETTINGS, _("Cancel")), 0, wxALIGN_BOTTOM | wxALL, 5);
    stdButtonSizer->Add(new wxButton(this, ID_BUTTON_APPLY_PROXY_SETTINGS, _("Apply")), 0, wxALIGN_BOTTOM | wxALL, 5);

    auto *boxSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(flexGridSizer, 10, wxALIGN_CENTER_HORIZONTAL, 5);
    boxSizer->Add(stdButtonSizer, 1, wxALIGN_RIGHT, 5);

    this->SetSizer(boxSizer);
    this->Layout();
}

void ProxySettingsPage::CheckUnsavedChanges() {
    if (!HasUnsavedChanged()) return;
    wxMessageDialog dlg(this, _("Your have unsaved changes, apply now?"),
                        _("Warning"), wxYES_NO | wxCENTRE);
    dlg.SetYesNoLabels(_("Apply"), _("Cancel"));
    int ret = dlg.ShowModal();
    if (ret == wxID_YES) {
        ApplyProxySettings();
    } else {
        CancelProxySettings();
    }
}

void ProxySettingsPage::ApplyProxySettings() {
    if (!HasUnsavedChanged()) {
        wxMessageDialog(this, _("No changes!"), _("Information")).ShowModal();
        return;
    }

    bool needRestart = false;

    auto shareOnLan = m_shareOnLanCheckBox->GetValue();
    auto socksPort = m_socksPortSpin->GetValue();
    auto httpPort = m_httpPortSpin->GetValue();
    auto pacPort = m_pacPortSpin->GetValue();
    auto proxyBypass = m_proxyBypassTextCtrl->GetValue();

    if (shareOnLan != CONFIGURATION.GetShareOnLan()) {
        CONFIGURATION.SetShareOnLan(shareOnLan);
        needRestart = true;
    }

    if (socksPort != CONFIGURATION.GetSocksPort()) {
        if (CONFIGURATION.PortAlreadyInUse(socksPort)) {
            wxMessageDialog(this, wxString::Format(_("Port %d already in use!"), socksPort), _("Warning")).ShowModal();
        } else {
            CONFIGURATION.SetSocksPort(socksPort);
            needRestart = true;
        }
    }

    if (httpPort != CONFIGURATION.GetHttpPort()) {
        if (CONFIGURATION.PortAlreadyInUse(httpPort)) {
            wxMessageDialog(this, wxString::Format(_("Port %d already in use!"), httpPort), _("Warning")).ShowModal();
        } else {
            CONFIGURATION.SetHttpPort(httpPort);
            needRestart = true;
        }
    }

    if (pacPort != CONFIGURATION.GetPacPort()) {
        if (CONFIGURATION.PortAlreadyInUse(pacPort)) {
            wxMessageDialog(this, wxString::Format(_("Port %d already in use!"), pacPort), _("Warning")).ShowModal();
        } else {
            CONFIGURATION.SetPacPort(pacPort);
            needRestart = true;
        }
    }

    if (proxyBypass != CONFIGURATION.GetProxyBypass()) {
        CONFIGURATION.SetProxyBypass(proxyBypass);
        needRestart = true;
    }

    if (CLIMBER.IsRunning() && needRestart) {
        CLIMBER.Restart();
    }


}

void ProxySettingsPage::CancelProxySettings() {
    m_shareOnLanCheckBox->SetValue(CONFIGURATION.GetShareOnLan());
    m_socksPortSpin->SetValue(CONFIGURATION.GetSocksPort());
    m_httpPortSpin->SetValue(CONFIGURATION.GetHttpPort());
    m_pacPortSpin->SetValue(CONFIGURATION.GetPacPort());
    m_proxyBypassTextCtrl->SetValue(CONFIGURATION.GetProxyBypass());
}

bool ProxySettingsPage::HasUnsavedChanged() {
    return m_shareOnLanCheckBox->GetValue() != CONFIGURATION.GetShareOnLan()
           || m_socksPortSpin->GetValue() != CONFIGURATION.GetSocksPort()
           || m_httpPortSpin->GetValue() != CONFIGURATION.GetHttpPort()
           || m_pacPortSpin->GetValue() != CONFIGURATION.GetPacPort()
           || m_proxyBypassTextCtrl->GetValue() != CONFIGURATION.GetProxyBypass();
}

void ProxySettingsPage::OnApplyProxySettings(wxCommandEvent &event) {
    ApplyProxySettings();
}

void ProxySettingsPage::OnCancelProxySettings(wxCommandEvent &event) {
    CancelProxySettings();
}

BEGIN_EVENT_TABLE(ProxySettingsPage, wxPanel)
                EVT_BUTTON(ID_BUTTON_APPLY_PROXY_SETTINGS, ProxySettingsPage::OnApplyProxySettings)
                EVT_BUTTON(ID_BUTTON_CANCEL_PROXY_SETTINGS, ProxySettingsPage::OnCancelProxySettings)
END_EVENT_TABLE()
