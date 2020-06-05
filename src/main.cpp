//
// Created by Climber on 2020/6/3.
//

#include <wx/wx.h>
#include "SystemTray.h"
#include "Configuration.h"

class ClimberApp : public wxApp {

public:
    ClimberApp() : wxApp() {}

    bool OnInit() override {
        Configuration::Init();
        wxInitAllImageHandlers();
        KeepEventLoopRunning();
        InitLanguageSupport();
        SystemTray::Init();
        // TODO start process and set system proxy
        return true;
    }

    static void InitLanguageSupport() {
        wxLocale *locale;
        int language = CONFIGURATION.GetLanguageCode();
        if (wxLocale::IsAvailable(language)) {
            locale = new wxLocale(language);
            locale->AddCatalog("Climber");
            printf("Load locale data %s\n", locale->IsOk() ? "ok" : "failed");
        } else {
            printf("Language %d not available\n", language);
        }
    }

    inline static void KeepEventLoopRunning() {
        // just for keeping event loop running after all other frame closed
        auto *invisibleFrame = new wxFrame(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(0, 0), 0);
        invisibleFrame->Hide();
    }

};

wxIMPLEMENT_APP(ClimberApp);
