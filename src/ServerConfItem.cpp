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

void ServerConfItem::WriteTo(const wxString &file) const {
    std::ofstream out(file.ToStdString(), std::ios::out);
    if (!out.is_open()) {
        printf("Open \"%s\" failed, %s\n", file.c_str().AsChar(), strerror(errno));
    } else {
        out << m_data["data"].dump(4);
        out.close();
    }
}

const json &ServerConfItem::GetJsonObject() const {
    return m_data;
}
