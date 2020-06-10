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

void signalHandler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        wxLogMessage("Recv signal %d, bye!\n", sig);
        wxExit();
    }
}

class ClimberApp : public wxApp {

public:
    ClimberApp() : wxApp() {}

    bool OnInit() override {
        if (!InitLogger()) {
            return false;
        }
        if (!Paths::PrepareDirectories()) {
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
        return 0;
    }

    bool InitLogger() {
        m_climberLogFile = Paths::GetLogDirFile("climber.log");
        m_logStream.open(m_climberLogFile.ToStdString(), std::ios::out | std::ios::app);
        if (!m_logStream.is_open()) {
            wxMessageDialog(nullptr, wxString::Format("Open file \"%s\" failed!", m_climberLogFile), _("Error"))
                    .ShowModal();
            return false;
        }

        wxLog *logger = new wxLogStream(&m_logStream);
        wxLog::SetActiveTarget(logger);
        wxLog::DisableTimestamp();
        return true;
    }

    void DestroyLogger() {
        m_logStream.close();
    }

    inline static void KeepEventLoopRunning() {
        // just for keeping event loop running after all other frame closed
        auto *invisibleFrame = new wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(0, 0), 0);
        invisibleFrame->Hide();
    }

private:
    wxString m_climberLogFile = wxEmptyString;
    std::ofstream m_logStream;

};

wxIMPLEMENT_APP(ClimberApp);
