//
// Created by Climber on 2020/6/5.
//

#include "ProxySettingsPage.h"
#include "Configuration.h"

ProxySettingsPage::ProxySettingsPage(wxWindow *parent, wxWindowID winid)
        : wxPanel(parent, winid) {

    auto *flexGridSizer = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer->SetFlexibleDirection(wxBOTH);
    flexGridSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    m_shareOnLan = CONFIGURATION.GetShareOnLan();
    m_socksPort = CONFIGURATION.GetSocksPort();
    m_httpPort = CONFIGURATION.GetHttpPort();
    m_pacPort = CONFIGURATION.GetPacPort();

    m_shareOnLanCheckBox = new wxCheckBox(this, ID_CHECK_BOX_SHARE_ON_LAN, wxEmptyString);
    m_shareOnLanCheckBox->SetValue(m_shareOnLan);

    m_socksPortSpin = new wxSpinCtrl(this, ID_SPIN_SOCKS_PORT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                     wxSP_ARROW_KEYS, 1, 65535, m_socksPort);
    m_socksPortSpin->SetIncrement(1);

    m_httpPortSpin = new wxSpinCtrl(this, ID_SPIN_HTTP_PORT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                    wxSP_ARROW_KEYS, 1, 65535, m_httpPort);
    m_httpPortSpin->SetIncrement(1);

    m_pacPortSpin = new wxSpinCtrl(this, ID_SPIN_PAC_PORT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                   wxSP_ARROW_KEYS, 1, 65535, m_pacPort);
    m_pacPortSpin->SetIncrement(1);

    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Share on Lan")), 0, wxALL, 5);
    flexGridSizer->Add(m_shareOnLanCheckBox, 0, wxALL, 5);
    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Socks Port")), 0, wxALL, 5);
    flexGridSizer->Add(m_socksPortSpin, 0, wxALL, 5);
    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Http Port")), 0, wxALL, 5);
    flexGridSizer->Add(m_httpPortSpin, 0, wxALL, 5);
    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("PAC Port")), 0, wxALL, 5);
    flexGridSizer->Add(m_pacPortSpin, 0, wxALL, 5);

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
    if (!m_hasUnsavedChanges) return;
    wxMessageDialog dlg(this, _("Your have unsaved changes, apply now?"),
                        wxMessageBoxCaptionStr, wxYES_NO | wxCENTRE);
    dlg.SetYesNoLabels(_("Apply"), _("Cancel"));
    int ret = dlg.ShowModal();
    if (ret == wxID_YES) {
        ApplyProxySettings();
    } else {
        CancelProxySettings();
    }
}

void ProxySettingsPage::ApplyProxySettings() {
    if (!m_hasUnsavedChanges) {
        wxMessageDialog(this, wxString::Format(_("No changes!"), m_socksPort)).ShowModal();
        return;
    }

    bool needRestart = false;
    if (m_shareOnLan != CONFIGURATION.GetShareOnLan()) {
        CONFIGURATION.SetShareOnLan(m_shareOnLan);
        needRestart = true;
    }
    if (m_socksPort != CONFIGURATION.GetSocksPort()) {
        if (CONFIGURATION.PortAlreadyInUse(m_socksPort)) {
            wxMessageDialog(this, wxString::Format(_("Port %d already in use!"), m_socksPort)).ShowModal();
        } else {
            CONFIGURATION.SetSocksPort(m_socksPort);
            needRestart = true;
        }
    }
    if (m_httpPort != CONFIGURATION.GetHttpPort()) {
        if (CONFIGURATION.PortAlreadyInUse(m_httpPort)) {
            wxMessageDialog(this, wxString::Format(_("Port %d already in use!"), m_httpPort)).ShowModal();
        } else {
            CONFIGURATION.SetHttpPort(m_httpPort);
            needRestart = true;
        }
    }
    if (m_pacPort != CONFIGURATION.GetPacPort()) {
        if (CONFIGURATION.PortAlreadyInUse(m_pacPort)) {
            wxMessageDialog(this, wxString::Format(_("Port %d already in use!"), m_pacPort)).ShowModal();
        } else {
            CONFIGURATION.SetPacPort(m_pacPort);
            needRestart = true;
        }
    }

    if (needRestart) {
        // TODO restart and reset system proxy
    }

    m_hasUnsavedChanges = false;
}

void ProxySettingsPage::CancelProxySettings() {
    m_shareOnLan = CONFIGURATION.GetShareOnLan();
    m_socksPort = CONFIGURATION.GetSocksPort();
    m_httpPort = CONFIGURATION.GetHttpPort();
    m_pacPort = CONFIGURATION.GetPacPort();
    m_shareOnLanCheckBox->SetValue(m_shareOnLan);
    m_socksPortSpin->SetValue(m_socksPort);
    m_httpPortSpin->SetValue(m_httpPort);
    m_pacPortSpin->SetValue(m_pacPort);
    m_hasUnsavedChanges = false;
}

void ProxySettingsPage::OnToggleShareOnLan(wxCommandEvent &event) {
    if (m_shareOnLan == event.IsChecked()) return;
    m_shareOnLan = event.IsChecked();
    m_hasUnsavedChanges = true;
}

void ProxySettingsPage::OnChangeSocksPort(wxSpinEvent &event) {
    if (m_socksPort == event.GetValue()) return;
    m_socksPort = event.GetValue();
    m_hasUnsavedChanges = true;
}

void ProxySettingsPage::OnChangeHttpPort(wxSpinEvent &event) {
    if (m_httpPort == event.GetValue()) return;
    m_httpPort = event.GetValue();
    m_hasUnsavedChanges = true;
}

void ProxySettingsPage::OnChangePacPort(wxSpinEvent &event) {
    if (m_pacPort == event.GetValue()) return;
    m_pacPort = event.GetValue();
    m_hasUnsavedChanges = true;
}

void ProxySettingsPage::OnApplyProxySettings(wxCommandEvent &event) {
    ApplyProxySettings();
}

void ProxySettingsPage::OnCancelProxySettings(wxCommandEvent &event) {
    CancelProxySettings();
}

BEGIN_EVENT_TABLE(ProxySettingsPage, wxPanel)
                EVT_CHECKBOX(ID_CHECK_BOX_SHARE_ON_LAN, ProxySettingsPage::OnToggleShareOnLan)
                EVT_SPINCTRL(ID_SPIN_SOCKS_PORT, ProxySettingsPage::OnChangeSocksPort)
                EVT_SPINCTRL(ID_SPIN_HTTP_PORT, ProxySettingsPage::OnChangeHttpPort)
                EVT_SPINCTRL(ID_SPIN_PAC_PORT, ProxySettingsPage::OnChangePacPort)
                EVT_BUTTON(ID_BUTTON_APPLY_PROXY_SETTINGS, ProxySettingsPage::OnApplyProxySettings)
                EVT_BUTTON(ID_BUTTON_CANCEL_PROXY_SETTINGS, ProxySettingsPage::OnCancelProxySettings)
END_EVENT_TABLE()
