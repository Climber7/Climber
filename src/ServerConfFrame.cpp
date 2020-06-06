//
// Created by Climber on 2020/6/5.
//

#include "ServerConfFrame.h"

ServerConfFrame::ServerConfFrame(wxWindow *parent, wxWindowID winid, const wxString &title)
        : wxFrame(parent, winid, title, wxDefaultPosition, wxSize(400, 600),
                  wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION | wxTOPLEVEL_EX_DIALOG) {
    this->SetBackgroundColour(*wxWHITE);
    auto *sizer = new wxBoxSizer(wxVERTICAL);



    this->SetSizer(sizer);
    this->Layout();
    this->Centre(wxBOTH);
}
