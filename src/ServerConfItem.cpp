//
// Created by Climber on 2020/6/8.
//

#include <utility>
#include <fstream>
#include "ServerConfItem.h"
#include "Paths.h"

ServerConfItem::ServerConfItem(json obj) : m_data(std::move(obj)) {
    std::string name = m_data["name"];
    m_name = wxString(name);
    std::string typeName = m_data["type"];
    m_typeName = wxString(typeName);
}

const wxString &ServerConfItem::GetName() const {
    return m_name;
}

int ServerConfItem::GetType() const {
    return GetServerTypeByName(m_typeName);
}

wxString ServerConfItem::GetTypeName() const {
    return m_typeName;
}

json ServerConfItem::OverrideLocalHost(const wxString &localAddr, int localPort) const {
    return m_data;
}

void ServerConfItem::WriteTo(const wxString &file, const wxString &localAddr, int localPort) const {
    std::ofstream out(file.ToStdString(), std::ios::out);
    if (!out.is_open()) {
        wxMessageDialog(nullptr, wxString::Format("Open file \"%s\" failed!", file), _("Error"))
                .ShowModal();
    } else {
        auto data = OverrideLocalHost(localAddr, localPort);
        out << data["data"].dump(4);
        out.close();
    }
}

const json &ServerConfItem::GetJsonObject() const {
    return m_data;
}
