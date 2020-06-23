//
// Created by Climber on 2020/6/23.
//

#ifndef CLIMBER_SERVERSSETTINGSFRAME_H
#define CLIMBER_SERVERSSETTINGSFRAME_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/dataview.h>

class ServersSettingsFrame : public wxFrame {
public:
    ServersSettingsFrame(wxWindow *parent, wxWindowID winid);

    void AddRow(const wxVector<wxVariant> &rowData);

    void UpdateRow(int row, const wxVector<wxVariant> &rowData);

    void DeleteRow(int row);

    void DuplicateRow(int row);

    void MoveUpRow(int row);

    void MoveDownRow(int row);

private:
    void OnContextMenu(wxDataViewEvent &event);

    void OnNewManually(wxCommandEvent &event);

    void OnNewUri(wxCommandEvent &event);

    void OnNewUriClipboard(wxCommandEvent &event);

    void OnNewQrCodeScreen(wxCommandEvent &event);

    void OnNewQrCodeFile(wxCommandEvent &event);

    void OnShare(wxCommandEvent &event);

    void OnEdit(wxCommandEvent &event);

    void OnDelete(wxCommandEvent &event);

    void OnDuplicate(wxCommandEvent &event);

    void OnMoveUp(wxCommandEvent &event);

    void OnMoveDown(wxCommandEvent &event);

private:
    wxDataViewListCtrl *m_dataView;
    wxMenu m_contextMenu;

DECLARE_EVENT_TABLE()

};


#endif //CLIMBER_SERVERSSETTINGSFRAME_H
