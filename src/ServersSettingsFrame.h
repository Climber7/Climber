//
// Created by Climber on 2020/6/5.
//

#ifndef CLIMBER_SERVERSSETTINGSFRAME_H
#define CLIMBER_SERVERSSETTINGSFRAME_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/dataview.h>
#include "defs.h"

class ServersSettingsFrame : public wxFrame {
public:
    explicit ServersSettingsFrame(wxWindow *parent, wxWindowID winid = wxID_ANY);

private:
    wxDataViewListCtrl *m_dataView;

    void InitRows();

    void AddRow(const wxVector<wxVariant> &rowData);

    void UpdateRow(int row, const wxVector<wxVariant> &rowData);

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

DECLARE_EVENT_TABLE()

};


#endif //CLIMBER_SERVERSSETTINGSFRAME_H
