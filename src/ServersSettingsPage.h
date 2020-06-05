//
// Created by Climber on 2020/6/5.
//

#ifndef CLIMBER_SERVERSSETTINGSPAGE_H
#define CLIMBER_SERVERSSETTINGSPAGE_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/dataview.h>
#include "defs.h"
#include "ServerConfFrame.h"

class ServersSettingsPage : public wxPanel {
public:
    ServersSettingsPage(wxWindow *parent, wxWindowID winid = wxID_ANY);

private:
    wxDataViewListCtrl *m_dataView;

    void AddRow(const wxString &name, const wxString &type,
                const wxString &host, const wxString &port);

    void UpdateRow(int row,
                   const wxString &name, const wxString &type,
                   const wxString &host, const wxString &port);

    void DeleteRow(int row);

    void DuplicateRow(int row);

    void MoveUpRow(int row);

    void MoveDownRow(int row);

private:
    void OnNewRow(wxCommandEvent &event);

    void OnEditRow(wxCommandEvent &event);

    void OnDeleteRow(wxCommandEvent &event);

    void OnDuplicateRow(wxCommandEvent &event);

    void OnMoveUpRow(wxCommandEvent &event);

    void OnMoveDownRow(wxCommandEvent &event);

    void OnServerConfFrameClose(wxCloseEvent &event);

private:
    ServerConfFrame *m_serverConfFrame = nullptr;


DECLARE_EVENT_TABLE()

};


#endif //CLIMBER_SERVERSSETTINGSPAGE_H
