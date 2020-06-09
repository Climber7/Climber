//
// Created by Climber on 2020/6/3.
//

#ifndef CLIMBER_SYSTEMTRAY_H
#define CLIMBER_SYSTEMTRAY_H

#include <wx/wx.h>
#include <wx/taskbar.h>
#include <wx/windowptr.h>
#include "defs.h"
#include "AboutFrame.h"
#include "PreferencesFrame.h"
//#include "ServersSettingsFrame.h"

#define SYSTRAY  (SystemTray::GetInstance())

class SystemTray : public wxTaskBarIcon {
public:
    static void Init();

    static SystemTray &GetInstance();

public:
    SystemTray();

protected:
    wxMenu *CreatePopupMenu() override;

private:
    void OnSelectDirectProxyMode(wxCommandEvent &event);

    void OnSelectPacProxyMode(wxCommandEvent &event);

    void OnSelectGlobalProxyMode(wxCommandEvent &event);

//    void OnShowServersSettings(wxCommandEvent &event);

    void OnRefreshServers(wxCommandEvent &event);

    void OnShowPreferencesFrame(wxCommandEvent &event);

    void OnShowAboutFrame(wxCommandEvent &event);

    void OnQuit(wxCommandEvent &event);

private:
    wxMenu *m_taskBarMenu = nullptr;
    wxMenu *m_serverListMenu = nullptr;

private:
    wxMenuItem *m_statusMenuItem = nullptr;
    wxMenuItem *m_toggleMenuItem = nullptr;

    wxMenuItem *m_proxyDirectModeMenuItem = nullptr;
    wxMenuItem *m_proxyPacModeMenuItem = nullptr;
    wxMenuItem *m_proxyGlobalModeMenuItem = nullptr;

    wxMenuItem *m_showShadowsocksLogMenuItem = nullptr;
    wxMenuItem *m_showShadowsocksRLogMenuItem = nullptr;
    wxMenuItem *m_showVMessLogMenuItem = nullptr;
    wxMenuItem *m_showTrojanLogMenuItem = nullptr;

    std::vector<wxMenuItem *> m_serverMenuItemList;

private:
    AboutFrame *m_aboutFrame = nullptr;
    PreferencesFrame *m_preferencesFrame = nullptr;
//    ServersSettingsFrame *m_serversSettingsFrame = nullptr;

private:
    int m_proxyMode = DEFAULT_PROXY_MODE;

private:
    static SystemTray *s_instance;

DECLARE_EVENT_TABLE()

};


#endif //CLIMBER_SYSTEMTRAY_H
