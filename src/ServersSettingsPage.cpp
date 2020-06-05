//
// Created by Climber on 2020/6/5.
//

#include "ServersSettingsPage.h"

ServersSettingsPage::ServersSettingsPage(wxWindow *parent, wxWindowID winid)
        : wxPanel(parent, winid) {

    m_dataView = new wxDataViewListCtrl(this, ID_DATA_VIEW_SERVER_LIST);
    m_dataView->AppendTextColumn(_("Name"), wxDATAVIEW_CELL_INERT, 180, wxALIGN_LEFT);
    m_dataView->AppendTextColumn(_("Type"), wxDATAVIEW_CELL_INERT, 110, wxALIGN_LEFT);
    m_dataView->AppendTextColumn(_("Host"), wxDATAVIEW_CELL_INERT, 196, wxALIGN_LEFT);
    m_dataView->AppendTextColumn(_("Port"), wxDATAVIEW_CELL_INERT, 70, wxALIGN_LEFT);

    AddRow("Climber-hk-01", "shadowsocks", "1.1.1.1", "8888");
    AddRow("Climber-tj-01", "trojan", "1.1.1.1", "443");
    AddRow("Climber-hk-01", "shadowsocks", "1.1.1.1", "8888");
    AddRow("Climber-tj-01", "trojan", "1.1.1.1", "443");
    AddRow("Climber-hk-01", "shadowsocks", "1.1.1.1", "8888");
    AddRow("Climber-tj-01", "trojan", "1.1.1.1", "443");

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
}

void ServersSettingsPage::AddRow(const wxString &name, const wxString &type,
                                 const wxString &host, const wxString &port) {
    wxVector<wxVariant> rowData;
    rowData.push_back(name);
    rowData.push_back(type);
    rowData.push_back(host);
    rowData.push_back(port);
    m_dataView->AppendItem(rowData);
}

void ServersSettingsPage::UpdateRow(int row,
                                    const wxString &name, const wxString &type,
                                    const wxString &host, const wxString &port) {
    m_dataView->SetValue(name, row, 0);
    m_dataView->SetValue(type, row, 1);
    m_dataView->SetValue(host, row, 2);
    m_dataView->SetValue(port, row, 3);
}

void ServersSettingsPage::DeleteRow(int row) {
    m_dataView->DeleteItem(row);
}

void ServersSettingsPage::DuplicateRow(int row) {
    wxVector<wxVariant> rowData;
    for (int i = 0; i < m_dataView->GetColumnCount(); ++i) {
        rowData.push_back(m_dataView->GetTextValue(row, i));
    }
    m_dataView->AppendItem(rowData);
}

#define SwapValue(r1, r2, col) do {                                     \
    wxString str;                                                       \
    str = m_dataView->GetTextValue(r1, i);                              \
    m_dataView->SetTextValue(m_dataView->GetTextValue(r2, i), r1, i);   \
    m_dataView->SetTextValue(str, r2, i);                               \
} while(0)


void ServersSettingsPage::MoveUpRow(int row) {
    if (row == -1 || row == 0) return;
    for (int i = 0; i < m_dataView->GetColumnCount(); ++i) {
        SwapValue(row, row - 1, i);
    }
}

void ServersSettingsPage::MoveDownRow(int row) {
    if (row == -1 || row == m_dataView->GetItemCount() - 1) return;
    for (int i = 0; i < m_dataView->GetColumnCount(); ++i) {
        SwapValue(row, row + 1, i);
    }
}

void ServersSettingsPage::OnNewRow(wxCommandEvent &event) {
    if (m_serverConfFrame != nullptr) {
        m_serverConfFrame->Close();
    }
    m_serverConfFrame = new ServerConfFrame(this, ID_FRAME_SERVER_CONF, _("New Server"));
    m_serverConfFrame->Bind(wxEVT_CLOSE_WINDOW, &ServersSettingsPage::OnServerConfFrameClose, this);
    m_serverConfFrame->Show();
    // TODO

    AddRow("Climber-tj-01", "trojan", "1.1.1.1", "443");
}

void ServersSettingsPage::OnEditRow(wxCommandEvent &event) {
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;

    if (m_serverConfFrame != nullptr) {
        m_serverConfFrame->Close();
    }
    m_serverConfFrame = new ServerConfFrame(this, ID_FRAME_SERVER_CONF, _("Edit Server"));
    m_serverConfFrame->Bind(wxEVT_CLOSE_WINDOW, &ServersSettingsPage::OnServerConfFrameClose, this);
    m_serverConfFrame->Show();
    // TODO

    UpdateRow(selectedRow, "aaaaa", "aaaaa", "aaaaa", "1000");
}

void ServersSettingsPage::OnDeleteRow(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;
    DeleteRow(selectedRow);
    if (selectedRow == m_dataView->GetItemCount()) {
        m_dataView->SelectRow(m_dataView->GetItemCount() - 1);
    }
}

void ServersSettingsPage::OnDuplicateRow(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;
    DuplicateRow(selectedRow);
}

void ServersSettingsPage::OnMoveUpRow(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1 || selectedRow == 0) return;
    MoveUpRow(selectedRow);
    m_dataView->SelectRow(selectedRow - 1);
}

void ServersSettingsPage::OnMoveDownRow(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1 || selectedRow == m_dataView->GetItemCount() - 1) return;
    MoveDownRow(selectedRow);
    m_dataView->SelectRow(selectedRow + 1);
}

void ServersSettingsPage::OnServerConfFrameClose(wxCloseEvent &event) {
    m_serverConfFrame = nullptr;
    event.Skip();
}


BEGIN_EVENT_TABLE(ServersSettingsPage, wxPanel)
                EVT_BUTTON(ID_BUTTON_NEW_SERVER, ServersSettingsPage::OnNewRow)
                EVT_BUTTON(ID_BUTTON_EDIT_SERVER, ServersSettingsPage::OnEditRow)
                EVT_BUTTON(ID_BUTTON_DELETE_SERVER, ServersSettingsPage::OnDeleteRow)
                EVT_BUTTON(ID_BUTTON_DUPLICATE_SERVER, ServersSettingsPage::OnDuplicateRow)
                EVT_BUTTON(ID_BUTTON_MOVE_UP_SERVER, ServersSettingsPage::OnMoveUpRow)
                EVT_BUTTON(ID_BUTTON_MOVE_DOWN_SERVER, ServersSettingsPage::OnMoveDownRow)
END_EVENT_TABLE()
