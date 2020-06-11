//
// Created by Climber on 2020/6/3.
//

#include "defs.h"
#include "AboutFrame.h"

AboutFrame::AboutFrame(wxWindow *parent, wxWindowID winid)
        : wxFrame(parent, winid, _("About"), wxDefaultPosition, wxSize(400, 300),
                  wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION) {
    this->SetBackgroundColour(*wxWHITE);
    auto *sizer = new wxBoxSizer(wxVERTICAL);

    // TODO

    this->SetSizer(sizer);
    this->Layout();
    this->Centre(wxBOTH);
}

