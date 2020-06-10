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

static void setAutoStart(bool enable) {
    auto name = wxString("Climber");
    auto path = Paths::GetExecutablePath();
#ifdef CLIMBER_WINDOWS
    wxRegKey *regKey = new wxRegKey("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    if (enable) {
        regKey.SetValue(name, path);
    } else {
        regKey.DeleteKey(name);
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


#endif //CLIMBER_UTILS_H
