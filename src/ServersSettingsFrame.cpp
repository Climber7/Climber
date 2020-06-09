//
// Created by Climber on 2020/6/5.
//

#include "ServersSettingsFrame.h"
#include "ServerConfManager.h"

ServersSettingsFrame::ServersSettingsFrame(wxWindow *parent, wxWindowID winid)
        : wxFrame(parent, winid, _("Servers Settings"), wxDefaultPosition, wxSize(600, 400),
                  wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION) {

    this->SetBackgroundColour(*wxWHITE);

    m_dataView = new wxDataViewListCtrl(this, ID_DATA_VIEW_SERVER_LIST);
    m_dataView->AppendTextColumn(_("Name"), wxDATAVIEW_CELL_INERT, 175, wxALIGN_LEFT);
    m_dataView->AppendTextColumn(_("Type"), wxDATAVIEW_CELL_INERT, 110, wxALIGN_LEFT);
    m_dataView->AppendTextColumn(_("Host"), wxDATAVIEW_CELL_INERT, 192, wxALIGN_LEFT);
    m_dataView->AppendTextColumn(_("Port"), wxDATAVIEW_CELL_INERT, 70, wxALIGN_LEFT);
    m_dataView->GetColumn(0)->SetWidth(175);
    m_dataView->GetColumn(1)->SetWidth(110);
    m_dataView->GetColumn(2)->SetWidth(192);
    m_dataView->GetColumn(3)->SetWidth(70);
    InitRows();

    auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, ID_BUTTON_NEW_SERVER, _("&New")), 0, wxALIGN_BOTTOM | wxALL, 5);
    buttonSizer->Add(new wxButton(this, ID_BUTTON_EDIT_SERVER, _("&Edit")), 0, wxALIGN_BOTTOM | wxALL, 5);
    buttonSizer->Add(new wxButton(this, ID_BUTTON_DELETE_SERVER, _("&Delete")), 0, wxALIGN_BOTTOM | wxALL, 5);
    buttonSizer->Add(new wxButton(this, ID_BUTTON_DUPLICATE_SERVER, _("Dupli&cate")), 0, wxALIGN_BOTTOM | wxALL, 5);
    buttonSizer->Add(new wxButton(this, ID_BUTTON_MOVE_UP_SERVER, _("&Up")), 0, wxALIGN_BOTTOM | wxALL, 5);
    buttonSizer->Add(new wxButton(this, ID_BUTTON_MOVE_DOWN_SERVER, _("&Down")), 0, wxALIGN_BOTTOM | wxALL, 5);

    auto *boxSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(m_dataView, 10, wxALL | wxEXPAND, 5);
    boxSizer->Add(buttonSizer, 1, wxEXPAND, 5);

    this->SetSizer(boxSizer);
    this->Layout();
    this->Centre(wxBOTH);
}

void ServersSettingsFrame::InitRows() {
    for (const auto *item : SERVER_CONF_MANAGER.GetServersList()) {
        AddRow(item->GetDataViewRowData());
    }
}

void ServersSettingsFrame::AddRow(const wxVector<wxVariant> &rowData) {
    m_dataView->AppendItem(rowData);
}

void ServersSettingsFrame::UpdateRow(int row, const wxVector<wxVariant> &rowData) {
    for (int i = 0; i < rowData.size(); ++i) {
        m_dataView->SetValue(rowData[i], row, i);
    }
}

void ServersSettingsFrame::DeleteRow(int row) {
    m_dataView->DeleteItem(row);
    if (row == m_dataView->GetItemCount() && m_dataView->GetItemCount() > 0) {
        m_dataView->SelectRow(m_dataView->GetItemCount() - 1);
    }
    if (row == 0 && m_dataView->GetItemCount() > 0) {
        m_dataView->SelectRow(0);
    }
}

void ServersSettingsFrame::DuplicateRow(int row) {
    wxVector<wxVariant> rowData;
    for (unsigned int i = 0; i < m_dataView->GetColumnCount(); ++i) {
        rowData.push_back(m_dataView->GetTextValue(row, i));
    }
    m_dataView->AppendItem(rowData);
}

#define SwapRow(r1, r2) do {                                                \
    wxString str;                                                           \
    for(unsigned int i = 0; i < m_dataView->GetColumnCount(); ++i) {        \
        str = m_dataView->GetTextValue(r1, i);                              \
        m_dataView->SetTextValue(m_dataView->GetTextValue(r2, i), r1, i);   \
        m_dataView->SetTextValue(str, r2, i);                               \
    }                                                                       \
} while(0)


void ServersSettingsFrame::MoveUpRow(int row) {
    if (row == -1 || row == 0) return;
    SwapRow(row, row - 1);
    m_dataView->SelectRow(row - 1);
}

void ServersSettingsFrame::MoveDownRow(int row) {
    if (row == -1 || row == m_dataView->GetItemCount() - 1) return;
    SwapRow(row, row + 1);
    m_dataView->SelectRow(row + 1);
}

void ServersSettingsFrame::OnNewRow(wxCommandEvent &event) {
    // TODO
}

void ServersSettingsFrame::OnEditRow(wxCommandEvent &event) {
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;
    // TODO
}

void ServersSettingsFrame::OnDeleteRow(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;
    DeleteRow(selectedRow);
}

void ServersSettingsFrame::OnDuplicateRow(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;
    DuplicateRow(selectedRow);
}

void ServersSettingsFrame::OnMoveUpRow(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1 || selectedRow == 0) return;
    MoveUpRow(selectedRow);
}

void ServersSettingsFrame::OnMoveDownRow(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1 || selectedRow == m_dataView->GetItemCount() - 1) return;
    MoveDownRow(selectedRow);
}


BEGIN_EVENT_TABLE(ServersSettingsFrame, wxFrame)
                EVT_BUTTON(ID_BUTTON_NEW_SERVER, ServersSettingsFrame::OnNewRow)
                EVT_BUTTON(ID_BUTTON_EDIT_SERVER, ServersSettingsFrame::OnEditRow)
                EVT_BUTTON(ID_BUTTON_DELETE_SERVER, ServersSettingsFrame::OnDeleteRow)
                EVT_BUTTON(ID_BUTTON_DUPLICATE_SERVER, ServersSettingsFrame::OnDuplicateRow)
                EVT_BUTTON(ID_BUTTON_MOVE_UP_SERVER, ServersSettingsFrame::OnMoveUpRow)
                EVT_BUTTON(ID_BUTTON_MOVE_DOWN_SERVER, ServersSettingsFrame::OnMoveDownRow)
END_EVENT_TABLE()
