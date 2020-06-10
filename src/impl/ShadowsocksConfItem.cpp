//
// Created by Climber on 2020/6/8.
//

#include <utility>
#include "ShadowsocksConfItem.h"


ShadowsocksConfItem::ShadowsocksConfItem(json obj) : ServerConfItem(std::move(obj)) {}

json ShadowsocksConfItem::OverrideLocalHost(const wxString &localAddr, int localPort) const {
    auto data = m_data;
    data["data"]["local_address"] = localAddr.ToStdString();
    data["data"]["local_port"] = localPort;
    return data;
}

wxString ShadowsocksConfItem::GetSystemTrayTitle() const {
    std::string host = m_data["data"]["server"];
    int port = m_data["data"]["server_port"];
    return wxString::Format("%s (%s@%s:%d)", m_name, m_typeName, wxString(host), port);
}

wxVector<wxVariant> ShadowsocksConfItem::GetDataViewRowData() const {
    std::string host = m_data["data"]["server"];
    int port = m_data["data"]["server_port"];
    wxVector<wxVariant> rowData;
    rowData.push_back(m_name);
    rowData.push_back(m_typeName);
    rowData.push_back(wxString(host));
    rowData.push_back(wxString::Format("%d", port));
    return rowData;
}
