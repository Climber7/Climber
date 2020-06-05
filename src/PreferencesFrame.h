//
// Created by Climber on 2020/6/4.
//

#ifndef CLIMBER_PREFERENCESFRAME_H
#define CLIMBER_PREFERENCESFRAME_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include "defs.h"
#include "GeneralSettingsPage.h"
#include "ProxySettingsPage.h"
#include "ServersSettingsPage.h"

enum {
    SETTINGS_PAGE_START = -1,
    //
    GENERAL_SETTINGS_PAGE_INDEX = 0,
    PROXY_SETTINGS_PAGE_INDEX,
    SERVERS_SETTINGS_PAGE_INDEX,
    //
    SETTINGS_PAGE_END,
};

class PreferencesFrame : public wxFrame {
public:
    PreferencesFrame(wxWindow *parent, wxWindowID winid);

    void ShowWithTab(int index = 0);

private:
    void AddPages();

    void OnTabChanging(wxBookCtrlEvent &event);

private:
    wxNotebook *m_mainTab;
    GeneralSettingsPage *m_generalSettingsPage;
    ProxySettingsPage *m_proxySettingsPage;
    ServersSettingsPage *m_serversSettingsPage;

DECLARE_EVENT_TABLE()

};


#endif //CLIMBER_PREFERENCESFRAME_H
