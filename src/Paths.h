//
// Created by Climber on 2020/6/9.
//

#ifndef CLIMBER_PATHS_H
#define CLIMBER_PATHS_H

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

class Paths {
public:
    static wxString GetExecutablePath() {
        return wxStandardPaths::Get().GetExecutablePath();
    }

    static wxString GetHomePath() {
        return wxGetHomeDir();
    }

    static wxString GetDataDir() {
#ifdef CLIMBER_WINDOWS
        return wxStandardPaths::Get().GetResourcesDir();
#else
        return wxStandardPaths::Get().GetUserLocalDataDir();
#endif
    }

    static wxString GetDataDirFile(const wxString &file) {
        wxFileName f;
        f.AssignDir(GetDataDir());
        f.SetFullName(file);
        return f.GetFullPath();
    }

    static wxString GetConfigDir() {
        return GetDataDirFile("config");
    }

    static wxString GetConfigDirFile(const wxString &file) {
        wxFileName f;
        f.AssignDir(GetConfigDir());
        f.SetFullName(file);
        return f.GetFullPath();
    }

    static wxString GetLogDir() {
        return GetDataDirFile("log");
    }

    static wxString GetLogDirFile(const wxString &file) {
        wxFileName f;
        f.AssignDir(GetLogDir());
        f.SetFullName(file);
        return f.GetFullPath();
    }

    static wxString GetTmpDir() {
        return GetDataDirFile("tmp");
    }

    static wxString GetTmpDirFile(const wxString &file) {
        wxFileName f;
        f.AssignDir(GetTmpDir());
        f.SetFullName(file);
        return f.GetFullPath();
    }

    static wxString GetResourcesDir() {
        return wxStandardPaths::Get().GetResourcesDir();
    }

    static wxString GetResourcesDirFile(const wxString &file) {
        wxFileName f;
        f.AssignDir(GetResourcesDir());
        f.SetFullName(file);
        return f.GetFullPath();
    }

    static wxString GetAssetsDir() {
        return GetResourcesDirFile("assets");
    }

    static wxString GetAssetsDirFile(const wxString &file) {
        wxFileName f;
        f.AssignDir(GetAssetsDir());
        f.SetFullName(file);
        return f.GetFullPath();
    }

    static wxString GetBinDir() {
        return GetAssetsDirFile("bin");
    }

    static wxString GetBinDirFile(const wxString &file) {
        wxFileName f;
        f.AssignDir(GetBinDir());
        f.SetFullName(file);
        return f.GetFullPath();
    }

#ifdef CLIMBER_WINDOWS
    static wxString GetLocaleDir() {
        return GetResourcesDirFile("locale");
    }
#endif

    static bool PrepareDirectories() {
        return PrepareDirectory(GetDataDir())
               && PrepareDirectory(GetConfigDir())
               && PrepareDirectory(GetLogDir())
               && PrepareDirectory(GetTmpDir());
    }

private:
    static bool PrepareDirectory(const wxString &dir) {
        if (!wxDirExists(dir)) {
            if (wxMkDir(dir, wxS_DIR_DEFAULT)) {
                wxMessageDialog(
                        nullptr,
                        wxString::Format("Make directory \"%s\" failed. %s\n", dir, strerror(errno)),
                        _("Error")).ShowModal();
                return false;
            }
        }
        return true;
    }

};


#endif //CLIMBER_PATHS_H
