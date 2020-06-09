//
// Created by Climber on 2020/6/3.
//

#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "SystemTray.h"
#include "Configuration.h"
#include "ServerConfManager.h"

SystemTray *SystemTray::s_instance = nullptr;

void SystemTray::Init() {
    if (s_instance == nullptr) {
        s_instance = new SystemTray();
    }
}

SystemTray &SystemTray::GetInstance() {
    return *s_instance;
}

SystemTray::SystemTray() {
    wxIcon icon;
    auto &paths = wxStandardPaths::Get();
    wxFileName iconFile;
    iconFile.AssignDir(paths.GetResourcesDir());
    iconFile.AppendDir("assets");
    iconFile.SetFullName("icon.png");
    icon.LoadFile(iconFile.GetFullPath(), wxBITMAP_TYPE_ANY);
    wxTaskBarIcon::SetIcon(icon);
}

wxMenu *SystemTray::CreatePopupMenu() {
    m_taskBarMenu = new wxMenu();

    m_statusMenuItem = m_taskBarMenu->Append(ID_MENU_STATUS, _("Climber: Off"));
    m_statusMenuItem->Enable(false);
    m_toggleMenuItem = m_taskBarMenu->Append(ID_MENU_TOGGLE, _("Start Climber"));

    m_taskBarMenu->AppendSeparator();

    auto *proxyModeMenu = new wxMenu();
    m_taskBarMenu->AppendSubMenu(proxyModeMenu, _("Proxy Mode"));
    m_proxyDirectModeMenuItem = proxyModeMenu->AppendRadioItem(ID_MENU_PROXY_DIRECT_MODE, _("Direct Mode"));
    m_proxyPacModeMenuItem = proxyModeMenu->AppendRadioItem(ID_MENU_PROXY_PAC_MODE, _("PAC Mode"));
    m_proxyGlobalModeMenuItem = proxyModeMenu->AppendRadioItem(ID_MENU_PROXY_GLOBAL_MODE, _("Global Mode"));
    m_proxyMode = CONFIGURATION.GetProxyMode();
    switch (m_proxyMode) {
        case PROXY_MODE_DIRECT:
            m_proxyDirectModeMenuItem->Check(true);
            break;
        case PROXY_MODE_PAC:
            m_proxyPacModeMenuItem->Check(true);
            break;
        case PROXY_MODE_GLOBAL:
            m_proxyGlobalModeMenuItem->Check(true);
            break;
    }

    auto *pacModeMenu = new wxMenu();
    m_taskBarMenu->AppendSubMenu(pacModeMenu, _("PAC Mode"));
    // TODO pac list
    pacModeMenu->Append(wxID_ANY, "...");

    m_serverListMenu = new wxMenu();
    m_taskBarMenu->AppendSubMenu(m_serverListMenu, _("Servers"));
//    m_serverListMenu->Append(ID_MENU_SERVERS_SETTINGS, _("Servers Settings..."));
    m_serverListMenu->Append(ID_MENU_SERVERS_REFRESH, _("Refresh"));

    m_serverListMenu->AppendSeparator();
    // TODO server list
    for (const auto *item : SERVER_CONF_MANAGER.GetServersList()) {
        m_serverMenuItemList.push_back(m_serverListMenu->AppendRadioItem(wxID_ANY, item->GetSystemTrayTitle()));
    }

    m_taskBarMenu->AppendSeparator();

#ifdef __APPLE__
    m_taskBarMenu->Append(ID_MENU_PREFERENCES, _("Preferences..."));
#else
    m_taskBarMenu->Append(ID_MENU_PREFERENCES, _("Settings..."));
#endif
    auto *copyCommandMenu = new wxMenu();
    m_taskBarMenu->AppendSubMenu(copyCommandMenu, _("Copy Terminal Proxy Command"));
    copyCommandMenu->Append(ID_MENU_COPY_TERMINAL_PROXY_COMMAND_BASH, _("For Bash"));
    copyCommandMenu->Append(ID_MENU_COPY_TERMINAL_PROXY_COMMAND_SHELL, _("For Shell"));
    copyCommandMenu->Append(ID_MENU_COPY_TERMINAL_PROXY_COMMAND_CMD, _("For CMD"));
    auto *showLogMenu = new wxMenu();
    m_taskBarMenu->AppendSubMenu(showLogMenu, _("Show Log"));
    showLogMenu->Append(ID_MENU_SHOW_LOG_CLIMBER, _("Show Climber Log"));
    m_showShadowsocksLogMenuItem = showLogMenu->Append(ID_MENU_SHOW_LOG_SS, _("Show Shadowsocks Log"));
    m_showShadowsocksRLogMenuItem = showLogMenu->Append(ID_MENU_SHOW_LOG_SSR, _("Show ShadowsocksR Log"));
    m_showVMessLogMenuItem = showLogMenu->Append(ID_MENU_SHOW_LOG_VMESS, _("Show VMess Log"));
    m_showTrojanLogMenuItem = showLogMenu->Append(ID_MENU_SHOW_LOG_TROJAN, _("Show Trojan Log"));
    m_taskBarMenu->Append(ID_MENU_CHECK_FOR_UPDATES, _("Check for Updates..."));
    m_taskBarMenu->Append(ID_MENU_ABOUT, _("About"));

    m_taskBarMenu->AppendSeparator();


    m_taskBarMenu->Append(ID_MENU_EXIT, _("Quit"));
    return m_taskBarMenu;
}

void SystemTray::OnSelectDirectProxyMode(wxCommandEvent &event) {
    if (event.IsChecked()) {
        m_proxyDirectModeMenuItem->Check(true);
        m_proxyMode = PROXY_MODE_DIRECT;
        CONFIGURATION.SetProxyMode(m_proxyMode);
        // TODO reset system proxy
    }
}

void SystemTray::OnSelectPacProxyMode(wxCommandEvent &event) {
    if (event.IsChecked()) {
        m_proxyPacModeMenuItem->Check(true);
        m_proxyMode = PROXY_MODE_PAC;
        CONFIGURATION.SetProxyMode(m_proxyMode);
        // TODO reset system proxy
    }
}

void SystemTray::OnSelectGlobalProxyMode(wxCommandEvent &event) {
    if (event.IsChecked()) {
        m_proxyGlobalModeMenuItem->Check(true);
        m_proxyMode = PROXY_MODE_GLOBAL;
        CONFIGURATION.SetProxyMode(m_proxyMode);
        // TODO reset system proxy
    }
}

//void SystemTray::OnShowServersSettings(wxCommandEvent &event) {
//    if (m_serversSettingsFrame == nullptr) {
//        m_serversSettingsFrame = new ServersSettingsFrame(nullptr, ID_FRAME_SERVERS_SETTINGS);
//        m_serversSettingsFrame->Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent &event) {
//            m_serversSettingsFrame = nullptr;
//            event.Skip();
//        });
//    }
//
////    Raise do not work, why?
////    m_serversSettingsFrame->Raise();
//
//    long style = m_serversSettingsFrame->GetWindowStyle();
//    m_serversSettingsFrame->SetWindowStyle(style | wxSTAY_ON_TOP);
//    m_serversSettingsFrame->Show();
//    m_serversSettingsFrame->SetWindowStyle(style);
//}

void SystemTray::OnRefreshServers(wxCommandEvent &event) {
    printf("Refresh");
}

void SystemTray::OnShowPreferencesFrame(wxCommandEvent &event) {
    if (m_preferencesFrame == nullptr) {
        m_preferencesFrame = new PreferencesFrame(nullptr, ID_FRAME_PREFERENCES);
        m_preferencesFrame->Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent &event) {
            m_preferencesFrame = nullptr;
            event.Skip();
        });
    }

//    Raise do not work, why?
//    m_preferencesFrame->Raise();

    long style = m_preferencesFrame->GetWindowStyle();
    m_preferencesFrame->SetWindowStyle(style | wxSTAY_ON_TOP);
    m_preferencesFrame->Show();
    m_preferencesFrame->SetWindowStyle(style);
}

void SystemTray::OnShowAboutFrame(wxCommandEvent &event) {
    if (m_aboutFrame == nullptr) {
        m_aboutFrame = new AboutFrame(nullptr, ID_FRAME_ABOUT);
        m_aboutFrame->Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent &event) {
            m_aboutFrame = nullptr;
            event.Skip();
        });
    }

//    Raise do not work, why?
//    m_aboutFrame->Raise();

    long style = m_aboutFrame->GetWindowStyle();
    m_aboutFrame->SetWindowStyle(style | wxSTAY_ON_TOP);
    m_aboutFrame->Show();
    m_aboutFrame->SetWindowStyle(style);
}

void SystemTray::OnQuit(wxCommandEvent &event) {
    RemoveIcon();
    // TODO kill process and clean system proxy
    wxExit();
}

BEGIN_EVENT_TABLE(SystemTray, wxTaskBarIcon)
                EVT_MENU(ID_MENU_PROXY_DIRECT_MODE, SystemTray::OnSelectDirectProxyMode)
                EVT_MENU(ID_MENU_PROXY_PAC_MODE, SystemTray::OnSelectPacProxyMode)
                EVT_MENU(ID_MENU_PROXY_GLOBAL_MODE, SystemTray::OnSelectGlobalProxyMode)
//                EVT_MENU(ID_MENU_SERVERS_SETTINGS, SystemTray::OnShowServersSettings)
                EVT_MENU(ID_MENU_SERVERS_REFRESH, SystemTray::OnRefreshServers)
                EVT_MENU(ID_MENU_PREFERENCES, SystemTray::OnShowPreferencesFrame)
                EVT_MENU(ID_MENU_ABOUT, SystemTray::OnShowAboutFrame)
                EVT_MENU(ID_MENU_EXIT, SystemTray::OnQuit)
END_EVENT_TABLE()
