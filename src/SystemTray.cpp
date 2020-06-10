//
// Created by Climber on 2020/6/3.
//

#include "SystemTray.h"
#include "Configuration.h"
#include "Climber.h"
#include "ServerConfManager.h"
#include "Paths.h"
#include "utils.h"

#ifdef CLIMBER_DARWIN
#define PREFERENCES_MENU_TITLE _("Preferences")
#else
#define PREFERENCES_MENU_TITLE _("Settings")
#endif

SystemTray::SystemTray() {
    wxTaskBarIcon::SetIcon(wxIcon(Paths::GetAssetsDirFile("icon.png"), wxBITMAP_TYPE_ANY));
}

wxMenu *SystemTray::CreatePopupMenu() {
    m_taskBarMenu = new wxMenu();

    auto *statusMenuItem =
            m_taskBarMenu->Append(ID_MENU_STATUS, CLIMBER.IsRunning() ? _("Climber: On") : _("Climber: Off"));
    statusMenuItem->Enable(false);
    m_taskBarMenu->Append(ID_MENU_TOGGLE, CLIMBER.IsRunning() ? _("Stop Climber") : _("Start Climber"));

    m_taskBarMenu->AppendSeparator();

    m_taskBarMenu->AppendSubMenu(CreateProxyModeMenu(), _("Proxy Mode"));
    m_taskBarMenu->AppendSubMenu(CreatePacModeMenu(), _("PAC Mode"));
    m_taskBarMenu->AppendSubMenu(CreateServersListMenu(), _("Servers"));

    m_taskBarMenu->AppendSeparator();

    m_taskBarMenu->Append(ID_MENU_PREFERENCES, PREFERENCES_MENU_TITLE);
    m_taskBarMenu->AppendSubMenu(CreateCopyCommandMenu(), _("Copy Terminal Proxy Command"));
    m_taskBarMenu->Append(ID_MENU_OPEN_CONFIG_DIRECTORY, _("Open Config Directory"));
    m_taskBarMenu->Append(ID_MENU_OPEN_LOG_DIRECTORY, _("Open Log Directory"));
    m_taskBarMenu->Append(ID_MENU_CHECK_FOR_UPDATES, _("Check for Updates"));
    m_taskBarMenu->Append(ID_MENU_ABOUT, _("About"));

    m_taskBarMenu->AppendSeparator();

    m_taskBarMenu->Append(ID_MENU_EXIT, _("Quit"));
    return m_taskBarMenu;
}

wxMenu *SystemTray::CreateProxyModeMenu() {
    auto proxyMode = CONFIGURATION.GetProxyMode();

    auto *proxyModeMenu = new wxMenu();

    auto *proxyDirectModeMenuItem = proxyModeMenu->AppendRadioItem(ID_MENU_PROXY_DIRECT_MODE, _("Direct Mode"));
    if (proxyMode == PROXY_MODE_DIRECT) {
        proxyDirectModeMenuItem->Check(true);
    }

    auto *proxyPacModeMenuItem = proxyModeMenu->AppendRadioItem(ID_MENU_PROXY_PAC_MODE, _("PAC Mode"));
    if (proxyMode == PROXY_MODE_PAC) {
        proxyPacModeMenuItem->Check(true);
    }

    auto *proxyGlobalModeMenuItem = proxyModeMenu->AppendRadioItem(ID_MENU_PROXY_GLOBAL_MODE, _("Global Mode"));
    if (proxyMode == PROXY_MODE_GLOBAL) {
        proxyGlobalModeMenuItem->Check(true);
    }

    return proxyModeMenu;
}

wxMenu *SystemTray::CreatePacModeMenu() {
    auto *pacModeMenu = new wxMenu();
    // TODO pac list, when change pac file, restart pac server
    pacModeMenu->Append(wxID_ANY, "...");
    return pacModeMenu;
}

wxMenu *SystemTray::CreateServersListMenu() {
    auto *serverListMenu = new wxMenu();
//    serverListMenu->Append(ID_MENU_SERVERS_SETTINGS, _("Servers Settings"));
    serverListMenu->Append(ID_MENU_SERVERS_REFRESH, _("Refresh"));

    const auto &serverList = SERVER_CONF_MANAGER.GetServersList();
    if (serverList.size() > 0) {
        serverListMenu->AppendSeparator();
    }

    const int maxCount = ID_MENU_SERVER_ITEM_END - ID_MENU_SERVER_ITEM_START;
    if (serverList.size() > maxCount) {
        wxLogWarning("Max servers count %d, %lu provided", maxCount, serverList.size());
    }
    const int count = wxMin(serverList.size(), maxCount);
    const int selected = CONFIGURATION.GetSelectedServerIndex();
    m_serverMenuItemList.clear();
    for (int i = 0; i < count; ++i) {
        const auto *serverItem = serverList[i];
        auto menuItem = serverListMenu->AppendCheckItem(
                ID_MENU_SERVER_ITEM_START + i,
                serverItem->GetSystemTrayTitle());
        menuItem->Check(i == selected);
        m_taskBarMenu->Bind(wxEVT_MENU, [count, i, this](wxCommandEvent &event) {
            CONFIGURATION.SetSelectedServerIndex(i);
            if (CLIMBER.IsRunning()) {
                CLIMBER.Restart();
            }
            for (int n = 0; n < count; ++n) {
                m_taskBarMenu->FindItem(ID_MENU_SERVER_ITEM_START + n)->Check(n == i);
            }
        }, ID_MENU_SERVER_ITEM_START + i);
        m_serverMenuItemList.push_back(menuItem);
    }
    return serverListMenu;
}

wxMenu *SystemTray::CreateCopyCommandMenu() {
    auto *copyCommandMenu = new wxMenu();
    copyCommandMenu->Append(ID_MENU_COPY_TERMINAL_PROXY_COMMAND_BASH, _("For Bash"));
    copyCommandMenu->Append(ID_MENU_COPY_TERMINAL_PROXY_COMMAND_SHELL, _("For Shell"));
    copyCommandMenu->Append(ID_MENU_COPY_TERMINAL_PROXY_COMMAND_CMD, _("For CMD"));
    return copyCommandMenu;
}

void SystemTray::OnToggleClimber(wxCommandEvent &event) {
    if (CLIMBER.IsRunning()) {
        CLIMBER.Stop();
        CONFIGURATION.SetEnable(false);
    } else {
        CLIMBER.Start();
        CONFIGURATION.SetEnable(true);
    }
    m_taskBarMenu->FindItem(ID_MENU_STATUS)->SetItemLabel(CLIMBER.IsRunning() ? _("Climber: On") : _("Climber: Off"));
    m_taskBarMenu->FindItem(ID_MENU_TOGGLE)->SetItemLabel(CLIMBER.IsRunning() ? _("Stop Climber") : _("Start Climber"));
}

void SystemTray::OnSelectDirectProxyMode(wxCommandEvent &event) {
    if (event.IsChecked()) {
        ((wxMenuItem *) event.GetEventObject())->Check(true);
        CONFIGURATION.SetProxyMode(PROXY_MODE_DIRECT);
        if (CLIMBER.IsRunning()) {
            CLIMBER.SetSystemProxy();
        }
    }
}

void SystemTray::OnSelectPacProxyMode(wxCommandEvent &event) {
    if (event.IsChecked()) {
        ((wxMenuItem *) event.GetEventObject())->Check(true);
        CONFIGURATION.SetProxyMode(PROXY_MODE_PAC);
        if (CLIMBER.IsRunning()) {
            CLIMBER.SetSystemProxy();
        }
    }
}

void SystemTray::OnSelectGlobalProxyMode(wxCommandEvent &event) {
    if (event.IsChecked()) {
        ((wxMenuItem *) event.GetEventObject())->Check(true);
        CONFIGURATION.SetProxyMode(PROXY_MODE_GLOBAL);
        if (CLIMBER.IsRunning()) {
            CLIMBER.SetSystemProxy();
        }
    }
}

void SystemTray::OnShowServersSettings(wxCommandEvent &event) {
    if (m_serversSettingsFrame == nullptr) {
        m_serversSettingsFrame = new ServersSettingsFrame(nullptr, ID_FRAME_SERVERS_SETTINGS);
        m_serversSettingsFrame->Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent &event) {
            m_serversSettingsFrame = nullptr;
            event.Skip();
        });
    }

//    Raise do not work, why?
//    m_serversSettingsFrame->Raise();

    long style = m_serversSettingsFrame->GetWindowStyle();
    m_serversSettingsFrame->SetWindowStyle(style | wxSTAY_ON_TOP);
    m_serversSettingsFrame->Show();
    m_serversSettingsFrame->SetWindowStyle(style);
}

void SystemTray::OnRefreshServers(wxCommandEvent &event) {
    SERVER_CONF_MANAGER.Reload();
    if (CLIMBER.IsRunning()) {
        CLIMBER.Restart();
    }
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

void SystemTray::OnOpenConfigDirectory(wxCommandEvent &event) {
    openDirectory(Paths::GetConfigDir());
}

void SystemTray::OnOpenLogDirectory(wxCommandEvent &event) {
    openDirectory(Paths::GetLogDir());
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
    wxExit();
}

BEGIN_EVENT_TABLE(SystemTray, wxTaskBarIcon)
                EVT_MENU(ID_MENU_TOGGLE, SystemTray::OnToggleClimber)
                EVT_MENU(ID_MENU_PROXY_DIRECT_MODE, SystemTray::OnSelectDirectProxyMode)
                EVT_MENU(ID_MENU_PROXY_PAC_MODE, SystemTray::OnSelectPacProxyMode)
                EVT_MENU(ID_MENU_PROXY_GLOBAL_MODE, SystemTray::OnSelectGlobalProxyMode)
                EVT_MENU(ID_MENU_SERVERS_SETTINGS, SystemTray::OnShowServersSettings)
                EVT_MENU(ID_MENU_SERVERS_REFRESH, SystemTray::OnRefreshServers)
                EVT_MENU(ID_MENU_PREFERENCES, SystemTray::OnShowPreferencesFrame)
                EVT_MENU(ID_MENU_OPEN_CONFIG_DIRECTORY, SystemTray::OnOpenConfigDirectory)
                EVT_MENU(ID_MENU_OPEN_LOG_DIRECTORY, SystemTray::OnOpenLogDirectory)
                EVT_MENU(ID_MENU_ABOUT, SystemTray::OnShowAboutFrame)
                EVT_MENU(ID_MENU_EXIT, SystemTray::OnQuit)
END_EVENT_TABLE()
