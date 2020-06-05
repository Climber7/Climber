//
// Created by Climber on 2020/6/3.
//

#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "SystemTray.h"
#include "Configuration.h"


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
        case DIRECT_PROXY_MODE:
            m_proxyDirectModeMenuItem->Check(true);
            break;
        case PAC_PROXY_MODE:
            m_proxyPacModeMenuItem->Check(true);
            break;
        case GLOBAL_PROXY_MODE:
            m_proxyGlobalModeMenuItem->Check(true);
            break;
    }

    auto *pacModeMenu = new wxMenu();
    m_taskBarMenu->AppendSubMenu(pacModeMenu, _("PAC Mode"));
    // TODO pac list
    pacModeMenu->Append(wxID_ANY, "...");

    m_taskBarMenu->AppendSeparator();

    m_taskBarMenu->Append(ID_MENU_SCAN_QR_CODE, _("Scan QR Code on Screen"));
    m_taskBarMenu->Append(ID_MENU_IMPORT_FROM_URL, _("Import from Url"));
    m_taskBarMenu->Append(ID_MENU_SHARE_CURRENT_SERVER, _("Share Current Server"));
    m_taskBarMenu->Append(ID_MENU_SERVERS_SETTINGS, _("Servers Settings..."));

    m_serverListMenu = new wxMenu();
    m_taskBarMenu->AppendSubMenu(m_serverListMenu, _("Servers"));
    // TODO server list
    m_serverMenuItemList.push_back(
            m_serverListMenu->AppendRadioItem(wxID_ANY, "Climber-hk-01 (trojan://1.1.1.1:443)"));
    m_serverMenuItemList.push_back(
            m_serverListMenu->AppendRadioItem(wxID_ANY, "Climber-hk-02 (trojan://1.1.1.1:443)"));
    m_serverMenuItemList.push_back(
            m_serverListMenu->AppendRadioItem(wxID_ANY, "Climber-hk-03 (trojan://1.1.1.1:443)"));

    m_taskBarMenu->AppendSeparator();

#ifdef __APPLE__
    m_taskBarMenu->Append(ID_MENU_PREFERENCES, _("Preferences..."));
#else
    m_taskBarMenu->Append(ID_MENU_PREFERENCES, _("Settings..."));
#endif
    auto *copyCommandMenu = new wxMenu();
    m_taskBarMenu->AppendSubMenu(copyCommandMenu, _("Copy Terminal Proxy Command"));
    copyCommandMenu->Append(ID_MENU_COPY_TERMINAL_PROXY_COMMAND_SHELL, _("For Shell"));
    copyCommandMenu->Append(ID_MENU_COPY_TERMINAL_PROXY_COMMAND_BASH, _("For Bash"));
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
        m_proxyMode = DIRECT_PROXY_MODE;
        CONFIGURATION.SetProxyMode(m_proxyMode);
        // TODO reset system proxy
    }
}

void SystemTray::OnSelectPacProxyMode(wxCommandEvent &event) {
    if (event.IsChecked()) {
        m_proxyPacModeMenuItem->Check(true);
        m_proxyMode = PAC_PROXY_MODE;
        CONFIGURATION.SetProxyMode(m_proxyMode);
        // TODO reset system proxy
    }
}

void SystemTray::OnSelectGlobalProxyMode(wxCommandEvent &event) {
    if (event.IsChecked()) {
        m_proxyGlobalModeMenuItem->Check(true);
        m_proxyMode = GLOBAL_PROXY_MODE;
        CONFIGURATION.SetProxyMode(m_proxyMode);
        // TODO reset system proxy
    }
}

void SystemTray::OnShowServersSettings(wxCommandEvent &event) {
    if (m_preferencesFrame == nullptr) {
        m_preferencesFrame = new PreferencesFrame(nullptr, ID_FRAME_PREFERENCES);
        m_preferencesFrame->Bind(wxEVT_CLOSE_WINDOW, &SystemTray::OnPreferencesFrameClose, this);
    }
    m_preferencesFrame->ShowWithTab(SERVERS_SETTINGS_PAGE_INDEX);
}

void SystemTray::OnShowAboutFrame(wxCommandEvent &event) {
    if (m_aboutFrame == nullptr) {
        m_aboutFrame = new AboutFrame(nullptr, ID_FRAME_ABOUT);
        m_aboutFrame->Bind(wxEVT_CLOSE_WINDOW, &SystemTray::OnAboutFrameClose, this);
    }
    m_aboutFrame->Show();
}

void SystemTray::OnShowPreferencesFrame(wxCommandEvent &event) {
    if (m_preferencesFrame == nullptr) {
        m_preferencesFrame = new PreferencesFrame(nullptr, ID_FRAME_PREFERENCES);
        m_preferencesFrame->Bind(wxEVT_CLOSE_WINDOW, &SystemTray::OnPreferencesFrameClose, this);
    }
    m_preferencesFrame->ShowWithTab(GENERAL_SETTINGS_PAGE_INDEX);
}

void SystemTray::OnQuit(wxCommandEvent &event) {
    RemoveIcon();
    // TODO kill process and clean system proxy
    wxExit();
}

void SystemTray::OnPreferencesFrameClose(wxCloseEvent &event) {
    m_preferencesFrame = nullptr;
    event.Skip();
}

void SystemTray::OnAboutFrameClose(wxCloseEvent &event) {
    m_aboutFrame = nullptr;
    event.Skip();
}

BEGIN_EVENT_TABLE(SystemTray, wxTaskBarIcon)
                EVT_MENU(ID_MENU_PROXY_DIRECT_MODE, SystemTray::OnSelectDirectProxyMode)
                EVT_MENU(ID_MENU_PROXY_PAC_MODE, SystemTray::OnSelectPacProxyMode)
                EVT_MENU(ID_MENU_PROXY_GLOBAL_MODE, SystemTray::OnSelectGlobalProxyMode)
                EVT_MENU(ID_MENU_SERVERS_SETTINGS, SystemTray::OnShowServersSettings)
                EVT_MENU(ID_MENU_PREFERENCES, SystemTray::OnShowPreferencesFrame)
                EVT_MENU(ID_MENU_ABOUT, SystemTray::OnShowAboutFrame)
                EVT_MENU(ID_MENU_EXIT, SystemTray::OnQuit)
END_EVENT_TABLE()
