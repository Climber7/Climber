//
// Created by Climber on 2020/6/3.
//

#include <wx/wx.h>
#include "Climber.h"
#include "SystemTray.h"
#include "Configuration.h"
#include "ServerConfManager.h"
#include "Paths.h"

class ClimberApp : public wxApp {

public:
    ClimberApp() : wxApp() {}

    ~ClimberApp() override {
        Climber::Destroy();
        Configuration::Destroy();
        ServerConfManager::Destroy();
    }

    bool OnInit() override {
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

        return true;
    }

    inline static void KeepEventLoopRunning() {
        // just for keeping event loop running after all other frame closed
        auto *invisibleFrame = new wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(0, 0), 0);
        invisibleFrame->Hide();
    }

};

wxIMPLEMENT_APP(ClimberApp);
