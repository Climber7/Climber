//
// Created by Climber on 2020/6/9.
//

#ifndef CLIMBER_UTILS_H
#define CLIMBER_UTILS_H

#include <wx/wx.h>

#ifdef CLIMBER_WINDOWS
#include <Windows.h>
#endif

static inline void openDirectory(const wxString &dir) {
#ifdef CLIMBER_WINDOWS
    wxExecute(wxString::Format("explorer \"%s\"", dir), wxEXEC_ASYNC);
#endif
#ifdef CLIMBER_DARWIN
    wxExecute(wxString::Format("open \"%s\"", dir), wxEXEC_ASYNC);
#endif
#ifdef CLIMBER_LINUX
    wxExecute(wxString::Format("nautilus \"%s\"", dir), wxEXEC_ASYNC);
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


#endif //CLIMBER_UTILS_H
