//
// Created by Climber on 2020/6/3.
//

#include <fstream>
#include <csignal>
#include <wx/wx.h>
#include "Climber.h"
#include "SystemTray.h"
#include "Configuration.h"
#include "ServerConfManager.h"
#include "Paths.h"
#include "utils.h"

void signalHandler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        wxLogMessage("Recv signal %d, bye!\n", sig);
        wxExit();
    }
}

class SingleInstanceChecker {
public:
    SingleInstanceChecker() {
        m_pidFile = Paths::GetTmpDirFile("Climber.pid");
        if (wxFileExists(m_pidFile)) {
            auto pidStr = readTextFile(m_pidFile);
            if (!pidStr.empty()) {
                long pid = std::strtol(pidStr.c_str().AsChar(), nullptr, 10);
                if (wxProcess::Exists(pid)) {
                    m_isAnotherRunning = true;
                }
            }
        }

        if (!m_isAnotherRunning) {
            writeTextFile(m_pidFile, wxString::Format("%lu", wxGetProcessId()));
        }
    }

    ~SingleInstanceChecker() {
        if (wxFileExists(m_pidFile)) {
            wxRemoveFile(m_pidFile);
        }
    }

    inline bool IsAnotherRunning() const {
        return m_isAnotherRunning;
    }

private:
    wxString m_pidFile = wxEmptyString;
    bool m_isAnotherRunning = false;
};

class ClimberApp : public wxApp {

public:
    ClimberApp() : wxApp() {}

    bool OnInit() override {
        if (!Paths::PrepareDirectories()) {
            return false;
        }
        if (!InitLogger()) {
            return false;
        }

        m_checker = new SingleInstanceChecker();
        if (m_checker->IsAnotherRunning()) {
            wxMessageDialog(nullptr, _("Another program instance is already running, aborting."), _("Warning"))
                    .ShowModal();
            return false;
        }

        KeepEventLoopRunning();
        wxInitAllImageHandlers();

        ServerConfManager::Init();
        Configuration::Init();
        Climber::Init();

        new SystemTray();

        if (CONFIGURATION.GetEnable()) {
            CLIMBER.Start();
        }

        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);

        return true;
    }

    int OnExit() override {
        Climber::Destroy();
        Configuration::Destroy();
        ServerConfManager::Destroy();
        DestroyLogger();
        delete m_checker;
        return 0;
    }

    bool InitLogger() {
#ifdef NDEBUG
        m_climberLogFile = Paths::GetLogDirFile("climber.log");
        m_logStream.open(m_climberLogFile.ToStdString(), std::ios::out | std::ios::app);
        if (!m_logStream.is_open()) {
            wxMessageDialog(nullptr, wxString::Format("Open file \"%s\" failed! %s", m_climberLogFile, strerror(errno)),
                            _("Error"))
                    .ShowModal();
            return false;
        }

        wxLog *logger = new wxLogStream(&m_logStream);
#else
        wxLog *logger = new wxLogStream(&std::cout);
#endif
        wxLog::SetActiveTarget(logger);
        wxLog::DisableTimestamp();
        return true;
    }

    void DestroyLogger() {
#ifdef NDEBUG
        m_logStream.close();
#endif
    }

    inline static void KeepEventLoopRunning() {
        // just for keeping event loop running after all other frame closed
        auto *invisibleFrame = new wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(0, 0), 0);
        invisibleFrame->Hide();
    }

private:
    wxString m_climberLogFile = wxEmptyString;
    std::ofstream m_logStream;
    SingleInstanceChecker *m_checker;

};

wxIMPLEMENT_APP(ClimberApp);
