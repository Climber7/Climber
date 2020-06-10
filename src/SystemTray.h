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

class SystemTray : public wxTaskBarIcon {

public:
    SystemTray();

protected:
    wxMenu *CreatePopupMenu() override;

    wxMenu *CreateProxyModeMenu();

    wxMenu *CreatePacModeMenu();

    wxMenu *CreateServersListMenu();

    wxMenu *CreateCopyCommandMenu();

private:
    void OnToggleClimber(wxCommandEvent &event);

    void OnSelectDirectProxyMode(wxCommandEvent &event);

    void OnSelectPacProxyMode(wxCommandEvent &event);

    void OnSelectGlobalProxyMode(wxCommandEvent &event);

    void OnRefreshServers(wxCommandEvent &event);

    void OnShowPreferencesFrame(wxCommandEvent &event);

    void OnOpenConfigDirectory(wxCommandEvent &event);

    void OnOpenLogDirectory(wxCommandEvent &event);

    void OnShowAboutFrame(wxCommandEvent &event);

    void OnQuit(wxCommandEvent &event);

private:
    wxMenu *m_taskBarMenu = nullptr;
    std::vector<wxMenuItem *> m_serverMenuItemList;

private:
    AboutFrame *m_aboutFrame = nullptr;
    PreferencesFrame *m_preferencesFrame = nullptr;

DECLARE_EVENT_TABLE()

};


#endif //CLIMBER_SYSTEMTRAY_H
