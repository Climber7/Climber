//
// Created by Climber on 2020/6/8.
//

#include <utility>
#include "TrojanConfItem.h"


TrojanConfItem::TrojanConfItem(json obj) : ServerConfItem(std::move(obj)) {}

json TrojanConfItem::OverrideLocalHost(const wxString &localAddr, int localPort) const {
    json data = m_data;
    data["data"]["local_addr"] = localAddr.ToStdString();
    data["data"]["local_port"] = localPort;
    return data;
}

wxString TrojanConfItem::GetSystemTrayTitle() const {
    std::string host = m_data["data"]["remote_addr"];
    int port = m_data["data"]["remote_port"];
    return wxString::Format("%s (%s@%s:%d)", m_name, m_typeName, wxString(host), port);
}

wxVector<wxVariant> TrojanConfItem::GetDataViewRowData() const {
    std::string host = m_data["data"]["remote_addr"];
    int port = m_data["data"]["remote_port"];
    wxVector<wxVariant> rowData;
    rowData.push_back(m_name);
    rowData.push_back(m_typeName);
    rowData.push_back(wxString(host));
    rowData.push_back(wxString::Format("%d", port));
    return rowData;
}
