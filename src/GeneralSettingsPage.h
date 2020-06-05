//
// Created by Climber on 2020/6/5.
//

#ifndef CLIMBER_GENERALSETTINGSPAGE_H
#define CLIMBER_GENERALSETTINGSPAGE_H

#include <wx/wx.h>
#include "defs.h"

class GeneralSettingsPage : public wxPanel {
public:
    GeneralSettingsPage(wxWindow *parent, wxWindowID winid = wxID_ANY);

private:
    void OnChooseLanguage(wxCommandEvent &event);

    void OnToggleAutoStart(wxCommandEvent &event);

DECLARE_EVENT_TABLE()

};


#endif //CLIMBER_GENERALSETTINGSPAGE_H
