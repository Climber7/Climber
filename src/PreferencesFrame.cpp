//
// Created by Climber on 2020/6/4.
//

#include <wx/dialog.h>
#include "defs.h"
#include "PreferencesFrame.h"
#include "Configuration.h"


#ifdef __APPLE__
#define PREFERENCES_FRAME_TITLE _("Preferences")
#else
#define PREFERENCES_FRAME_TITLE _("Settings")
#endif

PreferencesFrame::PreferencesFrame(wxWindow *parent, wxWindowID winid)
        : wxFrame(parent, winid, PREFERENCES_FRAME_TITLE, wxDefaultPosition, wxSize(600, 400),
                  wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION | wxSTAY_ON_TOP) {
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    m_mainTab = new wxNotebook(this, ID_PREFERENCES_MAIN_TAB);
    sizer->Add(m_mainTab, 1, wxEXPAND | wxALL, 5);

    this->AddPages();
    m_mainTab->SetSelection(0);

    this->SetSizer(sizer);
    this->Layout();
    this->Centre(wxBOTH);
}

void PreferencesFrame::ShowWithTab(int index) {
    if (index > SETTINGS_PAGE_START && index < SETTINGS_PAGE_END) {
        m_mainTab->ChangeSelection(index);
    }
    wxFrame::Show();
}

void PreferencesFrame::AddPages() {
    m_generalSettingsPage = new GeneralSettingsPage(m_mainTab);
    m_proxySettingsPage = new ProxySettingsPage(m_mainTab);
    m_serversSettingsPage = new ServersSettingsPage(m_mainTab);

    for (int i = SETTINGS_PAGE_START + 1; i < SETTINGS_PAGE_END; ++i) {
        switch (i) {
            case GENERAL_SETTINGS_PAGE_INDEX:
                m_mainTab->AddPage(m_generalSettingsPage, _("General"));
                break;
            case PROXY_SETTINGS_PAGE_INDEX:
                m_mainTab->AddPage(m_proxySettingsPage, _("Proxy"));
                break;
            case SERVERS_SETTINGS_PAGE_INDEX:
                m_mainTab->AddPage(m_serversSettingsPage, _("Server"));
                break;
            default:
                break;
        }
    }
}

void PreferencesFrame::OnTabChanging(wxBookCtrlEvent &event) {
    if (event.GetOldSelection() == PROXY_SETTINGS_PAGE_INDEX) {
        m_proxySettingsPage->CheckUnsavedChanges();
    }
}

BEGIN_EVENT_TABLE(PreferencesFrame, wxFrame)
                EVT_NOTEBOOK_PAGE_CHANGING(ID_PREFERENCES_MAIN_TAB, PreferencesFrame::OnTabChanging)
END_EVENT_TABLE()
