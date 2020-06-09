//
// Created by Climber on 2020/6/8.
//

#include <utility>
#include <fstream>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "ServerConfItem.h"

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

wxString ServerConfItem::WriteToTmp() const {
    auto &paths = wxStandardPaths::Get();
    wxFileName tmpFile;
    tmpFile.AssignDir(paths.GetTempDir());
    tmpFile.SetFullName(wxString::Format("climber_%s_%d.json", m_typeName, rand()));
    wxString tmpConfPath = tmpFile.GetFullPath();
    std::ofstream out(tmpConfPath.ToStdString(), std::ios::out);
    if (!out.is_open()) {
        printf("Open \"%s\" failed, %s\n", tmpConfPath.c_str().AsChar(), strerror(errno));
    } else {
        out << m_data["data"].dump(4);
        out.close();
    }
    return tmpConfPath;
}

const json &ServerConfItem::GetJsonObject() const {
    return m_data;
}
