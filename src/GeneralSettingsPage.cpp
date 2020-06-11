//
// Created by Climber on 2020/6/5.
//

#include "GeneralSettingsPage.h"
#include "Configuration.h"
#include "utils.h"

GeneralSettingsPage::GeneralSettingsPage(wxWindow *parent, wxWindowID winid)
        : wxPanel(parent, winid) {

    auto *flexGridSizer = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer->SetFlexibleDirection(wxBOTH);
    flexGridSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    auto *languageChoice = new wxChoice(this, ID_CHOICE_LANGUAGE, wxDefaultPosition, wxDefaultSize,
                                        Configuration::GetSupportedLanguageNames());
    languageChoice->SetSelection(CONFIGURATION.GetLanguageIndex());

    auto *autoStartCheckBox = new wxCheckBox(this, ID_CHECK_BOX_AUTO_START, wxEmptyString);
    autoStartCheckBox->SetValue(getAutoStart());

    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Language")), 0, wxALL, 5);
    flexGridSizer->Add(languageChoice, 0, wxALL, 5);
    flexGridSizer->Add(new wxStaticText(this, wxID_ANY, _("Start at Login")), 0, wxALL, 5);
    flexGridSizer->Add(autoStartCheckBox, 0, wxALL, 5);

    auto *boxSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(flexGridSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5);

    this->SetSizer(boxSizer);
    this->Layout();
}

void GeneralSettingsPage::OnChooseLanguage(wxCommandEvent &event) {
    int index = event.GetSelection();
    if (index == CONFIGURATION.GetLanguageIndex()) {
        return;
    }
    CONFIGURATION.SetLanguageIndex(index);
    wxMessageDialog(this, _("Your change will take effect after next start."), _("Information")).ShowModal();
}

void GeneralSettingsPage::OnToggleAutoStart(wxCommandEvent &event) {
    bool autoStart = event.IsChecked();
    if (autoStart == getAutoStart()) {
        return;
    }
    setAutoStart(autoStart);
}

BEGIN_EVENT_TABLE(GeneralSettingsPage, wxPanel)
                EVT_CHOICE(ID_CHOICE_LANGUAGE, GeneralSettingsPage::OnChooseLanguage)
                EVT_CHECKBOX(ID_CHECK_BOX_AUTO_START, GeneralSettingsPage::OnToggleAutoStart)
END_EVENT_TABLE()
