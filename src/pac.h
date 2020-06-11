//
// Created by Climber on 2020/6/11.
//

#ifndef CLIMBER_PAC_H
#define CLIMBER_PAC_H

#include <wx/wx.h>
#include <wx/base64.h>
#include <wx/tokenzr.h>
#include <nlohmann/json.hpp>
#include "Paths.h"
#include "utils.h"

using nlohmann::json;

static wxString getPacScript(const wxString &tpl, const wxString &host, int socksPort, int httpPort) {
    json rules;

    // add gfwlist rules
    {
        wxString gfwlistBase64;
        if (wxFileExists(Paths::GetRuleDirFile("gfwlist.txt"))) {
            gfwlistBase64 = readTextFile(Paths::GetRuleDirFile("gfwlist.txt"));
        } else {
            gfwlistBase64 = readTextFile(Paths::GetAssetsDirFile("gfwlist.txt"));
        }

        gfwlistBase64.Replace("\n", "");
        auto buffer = wxBase64Decode(gfwlistBase64);
        auto gfwlist = wxString(buffer);
        wxStringTokenizer lines(gfwlist, "\n");
        while (lines.HasMoreTokens()) {
            auto line = lines.GetNextToken();
            if (line.empty() || line.StartsWith("!") || line.StartsWith("[")) {
                continue;
            }
            rules.push_back(line);
        }
    }

    // add user rules
    {
        if (wxFileExists(Paths::GetRuleDirFile("user-rule.txt"))) {
            auto userRule = readTextFile(Paths::GetRuleDirFile("user-rule.txt"));
            wxStringTokenizer lines(userRule, "\n");
            while (lines.HasMoreTokens()) {
                auto line = lines.GetNextToken();
                if (line.empty() || line.StartsWith("!") || line.StartsWith("[")) {
                    continue;
                }
                rules.push_back(line);
            }
        }
    }

    auto pac = tpl;
    pac.Replace("__SOCKS5__", wxString::Format("SOCKS5 %s:%d", host, socksPort));
    pac.Replace("__SOCKS__", wxString::Format("SOCKS %s:%d", host, socksPort));
    pac.Replace("__PROXY__", wxString::Format("PROXY %s:%d", host, httpPort));
    pac.Replace("__RULES__", wxString(rules.dump(4)));

    return pac;
}

#endif //CLIMBER_PAC_H
