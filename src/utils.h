//
// Created by Climber on 2020/6/9.
//

#ifndef CLIMBER_UTILS_H
#define CLIMBER_UTILS_H

#include <fstream>
#include <sstream>
#include <wx/wx.h>
#include "Paths.h"

#ifdef CLIMBER_WINDOWS

#include <wx/msw/registry.h>
#include <Windows.h>

#endif

static inline void openDirectory(const wxString &dir) {
#ifdef CLIMBER_WINDOWS
    wxExecute(wxString::Format("explorer \"%s\"", dir), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
#endif
#ifdef CLIMBER_DARWIN
    wxExecute(wxString::Format("open \"%s\"", dir), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
#endif
#ifdef CLIMBER_LINUX
    wxExecute(wxString::Format("nautilus \"%s\"", dir), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
#endif
}

static inline void killProcess(long pid) {
#ifdef CLIMBER_WINDOWS
    wxExecute(wxString::Format("taskkill /f /pid %ld", pid), wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
#endif
#ifdef CLIMBER_DARWIN
    wxExecute(wxString::Format("kill %ld", pid), wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
#endif
#ifdef CLIMBER_LINUX
    wxExecute(wxString::Format("kill %ld", pid), wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
#endif
}

static inline void killProcessByName(const wxString &name) {
#ifdef CLIMBER_WINDOWS
    wxExecute(wxString::Format("taskkill /f /im %s", name), wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
#endif
#ifdef CLIMBER_DARWIN
    wxExecute(wxString::Format("killall %s", name), wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
#endif
#ifdef CLIMBER_LINUX
    wxExecute(wxString::Format("killall %s", name), wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
#endif
}

static wxString readTextFile(const wxString &file, const wxString &defaultValue = wxEmptyString) {
    std::ifstream in(file.ToStdString(), std::ios::in);
    if (!in.is_open()) {
        wxMessageDialog(nullptr, wxString::Format("Open file \"%s\" failed!", file), _("Error"))
                .ShowModal();
        return defaultValue;
    }
    std::stringstream ss;
    ss << in.rdbuf();
    in.close();
    return wxString(ss.str());
}

static void writeTextFile(const wxString &file, const wxString &content) {
    std::ofstream out(file.ToStdString(), std::ios::out);
    if (!out.is_open()) {
        wxMessageDialog(nullptr, wxString::Format("Open file \"%s\" failed! %s", file, strerror(errno)), _("Error"))
                .ShowModal();
    }
    out << content.ToStdString();
    out.close();
}

static bool getAutoStart() {
    auto name = wxString("Climber");
    auto path = Paths::GetExecutablePath();
#ifdef CLIMBER_WINDOWS
    wxRegKey regKey(wxRegKey::HKCU, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    if (regKey.HasValue(name)) {
        wxString value;
        regKey.QueryValue(name, value);
        return value == wxString::Format("\"%s\"", path);
    } else {
        return false;
    }
#endif
#ifdef CLIMBER_DARWIN
    wxString plistFile = Paths::GetHomePath() + "/Library/LaunchAgents/io.github.climber.plist";
    if (!wxFileExists(plistFile)) {
        return false;
    }
    auto plistContent = readTextFile(plistFile);
    if (plistContent.find(path) == wxNOT_FOUND) {
        return false;
    } else {
        return true;
    }
#endif
#ifdef CLIMBER_LINUX
    wxString desktopFile = Paths::GetHomePath() + "/.config/autostart/Climber.desktop";
    if (!wxFileExists(desktopFile)) {
        return false;
    }
    auto desktopContent = readTextFile(desktopFile);
    if (desktopContent.find(path) == wxNOT_FOUND) {
        return false;
    } else {
        return true;
    }
#endif
}

static void setAutoStart(bool enable) {
    auto name = wxString("Climber");
    auto path = Paths::GetExecutablePath();
#ifdef CLIMBER_WINDOWS
    wxRegKey regKey(wxRegKey::HKCU, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    if (enable) {
        regKey.SetValue(name, wxString::Format("\"%s\"", path));
    } else {
        regKey.DeleteValue(name);
    }
#endif
#ifdef CLIMBER_DARWIN
    wxString plistFile = Paths::GetHomePath() + "/Library/LaunchAgents/io.github.climber.plist";
    if (enable) {
        wxString plistContent = R"(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>Label</key>
    <string>io.github.climber</string>
    <key>ProgramArguments</key>
    <array>
      <string>__EXECUTABLE__</string>
    </array>
    <key>RunAtLoad</key>
    <true/>
    <key>StandardErrorPath</key>
    <string>/dev/null</string>
    <key>StandardOutPath</key>
    <string>/dev/null</string>
  </dict>
</plist>

)";
        plistContent.Replace("__EXECUTABLE__", path);
        writeTextFile(plistFile, plistContent);
    } else {
        if (wxFileExists(plistFile)) {
            wxRemoveFile(plistFile);
        }
    }
#endif
#ifdef CLIMBER_LINUX
    wxString desktopFile = Paths::GetHomePath() + "/.config/autostart/Climber.desktop";
    if (enable) {
        wxString desktopContent = R"[Desktop Entry]
Name=__NAME__
Exec=__EXECUTABLE__
Type=Application
Terminal=false
X-GNOME-Autostart-enabled=true

)";
        desktopContent.Replace("__NAME__", name);
        desktopContent.Replace("__EXECUTABLE__", path);
        writeTextFile(desktopFile, desktopContent);
    } else {
        if(wxFileExists(desktopFile)) {
            wxRemoveFile(desktopFile);
        }
    }
#endif
}

#ifdef CLIMBER_WINDOWS

static void setProxy(
        const wxString &socksHost, int socksPort,
        const wxString &httpHost, int httpPort,
        const wxString &bypass) {
    auto sysproxy = Paths::GetBinDirFile("climber_sysproxy");
    wxExecute(wxString::Format("\"%s\" global \"%s:%d\" \"%s\"", sysproxy, httpHost, httpPort, bypass),
              wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
}

static void setProxyPac(const wxString &pacUrl, const wxString &bypass) {
    auto sysproxy = Paths::GetBinDirFile("climber_sysproxy");
    wxExecute(wxString::Format("\"%s\" pac \"%s\"", sysproxy, pacUrl), wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
}

static void clearProxy() {
    auto sysproxy = Paths::GetBinDirFile("climber_sysproxy");
    wxExecute(wxString::Format("\"%s\" off", sysproxy), wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
}

#endif

#ifdef CLIMBER_DARWIN

static wxArrayString getNetworkServices() {
    wxArrayString lines;
    wxExecute("networksetup -listallnetworkservices", lines, wxEXEC_SYNC | wxEXEC_HIDE_CONSOLE);
    lines.erase(lines.begin(), lines.begin() + 1);
    return lines;
}

static bool isNetworkServicesActive(const wxString &service) {
    wxArrayString lines;
    wxExecute(wxString::Format("networksetup -getinfo \"%s\"", service), lines, wxEXEC_SYNC | wxEXEC_HIDE_CONSOLE);
    for (auto &line : lines) {
        if ((line.find("IP address:") == 0 && line.find("IP address: none") == wxNOT_FOUND)
            || (line.find("IPv6 IP address:") == 0 && line.find("IPv6 IP address: none") == wxNOT_FOUND)) {
            return true;
        }
    }
    return false;
}

static void setProxy(
        const wxString &socksHost, int socksPort,
        const wxString &httpHost, int httpPort,
        const wxString &bypass) {
    auto services = getNetworkServices();
    for (auto &service : services) {
        if (!isNetworkServicesActive(service)) continue;
        wxExecute(
                wxString::Format("networksetup -setsocksfirewallproxy \"%s\" \"%s\" %d", service, socksHost, socksPort),
                wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setwebproxy \"%s\" \"%s\" %d", service, httpHost, httpPort),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setsecurewebproxy \"%s\" \"%s\" %d", service, httpHost, httpPort),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setautoproxystate \"%s\" off", service),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setproxybypassdomains \"%s\" \"%s\"", service, bypass),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
    }
}

static void setProxyPac(const wxString &pacUrl, const wxString &bypass) {
    auto services = getNetworkServices();
    for (auto &service : services) {
        if (!isNetworkServicesActive(service)) continue;
        wxExecute(wxString::Format("networksetup -setautoproxyurl \"%s\" \"%s\"", service, pacUrl),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setsocksfirewallproxystate \"%s\" off", service),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setwebproxystate \"%s\" off", service),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setsecurewebproxystate \"%s\" off", service),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setproxybypassdomains \"%s\" \"%s\"", service, bypass),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
    }
}

static void clearProxy() {
    auto services = getNetworkServices();
    for (auto &service : services) {
        if (!isNetworkServicesActive(service)) continue;
        wxExecute(wxString::Format("networksetup -setsocksfirewallproxystate \"%s\" off", service),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setwebproxystate \"%s\" off", service),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setsecurewebproxystate \"%s\" off", service),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
        wxExecute(wxString::Format("networksetup -setautoproxystate \"%s\" off", service),
                  wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE);
    }
}

#endif


#endif //CLIMBER_UTILS_H
