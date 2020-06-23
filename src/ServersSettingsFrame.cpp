//
// Created by Climber on 2020/6/23.
//

#include <wx/uiaction.h>
#include "ServersSettingsFrame.h"
#include "ClientManager.h"
#include "defs.h"

ServersSettingsFrame::ServersSettingsFrame(wxWindow *parent, wxWindowID winid)
        : wxFrame(parent, winid, _("Servers Settings"), wxDefaultPosition, wxSize(600, 400),
                  wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION) {
#ifdef CLIMBER_WINDOWS
    this->SetBackgroundColour(*wxWHITE);
#endif
    m_dataView = new wxDataViewListCtrl(this, ID_DATA_VIEW_SERVER_LIST);
    m_dataView->AppendTextColumn(_("Name"), wxDATAVIEW_CELL_INERT, 175, wxALIGN_LEFT);
    m_dataView->AppendTextColumn(_("Type"), wxDATAVIEW_CELL_INERT, 110, wxALIGN_LEFT);
    m_dataView->AppendTextColumn(_("Host"), wxDATAVIEW_CELL_INERT, 192, wxALIGN_LEFT);
    m_dataView->AppendTextColumn(_("Port"), wxDATAVIEW_CELL_INERT, 70, wxALIGN_LEFT);
    m_dataView->GetColumn(0)->SetWidth(175);
    m_dataView->GetColumn(1)->SetWidth(110);
    m_dataView->GetColumn(2)->SetWidth(192);
    m_dataView->GetColumn(3)->SetWidth(70);

    // init rows
    for (const auto *item : CLIENT_MANAGER.GetList()) {
        AddRow(item->GetDataViewRowData());
    }

    auto *boxSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(m_dataView, 1, wxALL | wxEXPAND, 0);

    this->SetSizer(boxSizer);
    this->Layout();
    this->Centre(wxBOTH);

    auto *newSubMenu = new wxMenu();
    newSubMenu->Append(ID_MENU_NEW_SERVER_MANUALLY, _("Manually"));
    newSubMenu->Append(ID_MENU_NEW_SERVER_URI, _("URI"));
    newSubMenu->Append(ID_MENU_NEW_SERVER_URI_FROM_CLIPBOARD, _("URI from Clipboard"));
    newSubMenu->Append(ID_MENU_NEW_SERVER_QR_CODE_ON_SCREEN, _("Scan QR Code on Screen"));
    newSubMenu->Append(ID_MENU_NEW_SERVER_QR_CODE_FILE, _("Scan QR Code Image File"));

    m_contextMenu.AppendSubMenu(newSubMenu, _("New"));
    m_contextMenu.Append(ID_MENU_SHARE_SERVER, _("Share"));
    m_contextMenu.Append(ID_MENU_EDIT_SERVER, _("Edit"));
    m_contextMenu.Append(ID_MENU_DELETE_SERVER, _("Delete"));
    m_contextMenu.Append(ID_MENU_DUPLICATE_SERVER, _("Duplicate"));
    m_contextMenu.Append(ID_MENU_MOVE_UP_SERVER, _("Up"));
    m_contextMenu.Append(ID_MENU_MOVE_DOWN_SERVER, _("Down"));

}

void ServersSettingsFrame::AddRow(const wxVector<wxVariant> &rowData) {
    m_dataView->AppendItem(rowData);
}

void ServersSettingsFrame::UpdateRow(int row, const wxVector<wxVariant> &rowData) {
    for (unsigned int i = 0; i < rowData.size(); ++i) {
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

void ServersSettingsFrame::OnContextMenu(wxDataViewEvent &event) {
    wxUIActionSimulator sim;
    sim.MouseClick(wxMOUSE_BTN_LEFT);
    wxYield();
    bool selected = m_dataView->GetSelectedRow() != -1;
    m_contextMenu.Enable(ID_MENU_SHARE_SERVER, selected);
    m_contextMenu.Enable(ID_MENU_EDIT_SERVER, selected);
    m_contextMenu.Enable(ID_MENU_DELETE_SERVER, selected);
    m_contextMenu.Enable(ID_MENU_DUPLICATE_SERVER, selected);
    m_contextMenu.Enable(ID_MENU_MOVE_UP_SERVER, selected);
    m_contextMenu.Enable(ID_MENU_MOVE_DOWN_SERVER, selected);
    PopupMenu(&m_contextMenu);
}

void ServersSettingsFrame::OnNewManually(wxCommandEvent &event) {
    // TODO
}

void ServersSettingsFrame::OnNewUri(wxCommandEvent &event) {
    // TODO
}

void ServersSettingsFrame::OnNewUriClipboard(wxCommandEvent &event) {
    // TODO
}

void ServersSettingsFrame::OnNewQrCodeScreen(wxCommandEvent &event) {
    // TODO
}

void ServersSettingsFrame::OnNewQrCodeFile(wxCommandEvent &event) {
    // TODO
}

void ServersSettingsFrame::OnShare(wxCommandEvent &event) {
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;
    // TODO

}

void ServersSettingsFrame::OnEdit(wxCommandEvent &event) {
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;
    // TODO
}

void ServersSettingsFrame::OnDelete(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;
    DeleteRow(selectedRow);
}

void ServersSettingsFrame::OnDuplicate(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1) return;
    DuplicateRow(selectedRow);
}

void ServersSettingsFrame::OnMoveUp(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1 || selectedRow == 0) return;
    MoveUpRow(selectedRow);
}

void ServersSettingsFrame::OnMoveDown(wxCommandEvent &event) {
    // TODO
    int selectedRow = m_dataView->GetSelectedRow();
    if (selectedRow == -1 || selectedRow == m_dataView->GetItemCount() - 1) return;
    MoveDownRow(selectedRow);
}

BEGIN_EVENT_TABLE(ServersSettingsFrame, wxFrame)
                EVT_DATAVIEW_ITEM_CONTEXT_MENU(ID_DATA_VIEW_SERVER_LIST, ServersSettingsFrame::OnContextMenu)
                EVT_MENU(ID_MENU_NEW_SERVER_MANUALLY, ServersSettingsFrame::OnNewManually)
                EVT_MENU(ID_MENU_NEW_SERVER_URI, ServersSettingsFrame::OnNewUri)
                EVT_MENU(ID_MENU_NEW_SERVER_URI_FROM_CLIPBOARD, ServersSettingsFrame::OnNewUriClipboard)
                EVT_MENU(ID_MENU_NEW_SERVER_QR_CODE_ON_SCREEN, ServersSettingsFrame::OnNewQrCodeScreen)
                EVT_MENU(ID_MENU_NEW_SERVER_QR_CODE_FILE, ServersSettingsFrame::OnNewQrCodeFile)
                EVT_MENU(ID_MENU_SHARE_SERVER, ServersSettingsFrame::OnShare)
                EVT_MENU(ID_MENU_EDIT_SERVER, ServersSettingsFrame::OnEdit)
                EVT_MENU(ID_MENU_DELETE_SERVER, ServersSettingsFrame::OnDelete)
                EVT_MENU(ID_MENU_DUPLICATE_SERVER, ServersSettingsFrame::OnDuplicate)
                EVT_MENU(ID_MENU_MOVE_UP_SERVER, ServersSettingsFrame::OnMoveUp)
                EVT_MENU(ID_MENU_MOVE_DOWN_SERVER, ServersSettingsFrame::OnMoveDown)
END_EVENT_TABLE()