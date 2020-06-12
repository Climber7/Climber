//
// Created by Climber on 2020/6/3.
//

#include "defs.h"
#include "AboutFrame.h"
#include "Paths.h"

AboutFrame::AboutFrame(wxWindow *parent, wxWindowID winid)
        : wxFrame(parent, winid, _("About"), wxDefaultPosition, wxSize(400, 300),
                  wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION) {
    this->SetBackgroundColour(*wxWHITE);
    auto *sizer = new wxBoxSizer(wxVERTICAL);

    wxImage logoImage(Paths::GetAssetsDirFile("icon.png"), wxBITMAP_TYPE_ANY);
    logoImage = logoImage.Scale(100, 100, wxIMAGE_QUALITY_HIGH);
    auto *logo = new wxStaticBitmap(this, wxID_ANY, logoImage);
    logo->SetScaleMode(wxStaticBitmap::Scale_AspectFit);

    auto *richText = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                        wxVSCROLL | wxHSCROLL | wxNO_BORDER | wxRE_READONLY);
    richText->GetCaret()->Hide();
    richText->SetCanFocus(false);
    this->SetContent(richText);

    sizer->Add(logo, 0, wxALIGN_CENTER | wxALL, 8);
    sizer->Add(richText, 1, wxEXPAND | wxALL, 5);

    this->SetSizer(sizer);
    this->Layout();
    this->Centre(wxBOTH);
}

void AboutFrame::SetContent(wxRichTextCtrl *r) {
    r->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);

    r->BeginFontSize(20);
    r->BeginTextColour(wxColour(0x202020));
    r->BeginBold();
    r->WriteText("Climber");
    r->EndBold();
    r->Newline();
    r->EndTextColour();
    r->EndFontSize();

    r->Newline();

    r->BeginFontSize(12);
    r->BeginTextColour(wxColour(0x404040));
    r->WriteText(wxString::Format("v%s", CLIMBER_VERSION));
    r->Newline();
    r->WriteText(CLIMBER_DESCRIPTION);
    r->Newline();
    r->WriteText(CLIMBER_COPYRIGHT);
    r->Newline();
    r->BeginUnderline();
    r->BeginURL(CLIMBER_REPO_URL);
    r->WriteText(CLIMBER_REPO_URL);
    r->EndURL();
    r->EndUnderline();
    r->Newline();

    r->EndTextColour();
    r->EndFontSize();

    r->Newline();
    r->Newline();

    r->BeginFontSize(16);
    r->BeginTextColour(wxColour(0x202020));
    r->BeginBold();
    r->WriteText("Special Thanks");
    r->EndBold();
    r->Newline();
    r->EndTextColour();
    r->EndFontSize();

    r->Newline();

    r->BeginFontSize(12);
    r->BeginTextColour(wxColour(0x404040));
    r->BeginUnderline();

    r->BeginURL("https://www.wxwidgets.org/");
    r->WriteText("wxWidgets");
    r->EndURL();

    r->Newline();

    r->BeginURL("https://github.com/nlohmann/json");
    r->WriteText("json");
    r->EndURL();

    r->Newline();

    r->BeginURL("https://github.com/shadowsocks/shadowsocks-libev");
    r->WriteText("shadowsocks-libev");
    r->EndURL();

    r->Newline();

    r->BeginURL("https://github.com/DDoSolitary/shadowsocks-libev-win");
    r->WriteText("shadowsocks-libev-win");
    r->EndURL();

    r->Newline();

    r->BeginURL("https://github.com/p4gefau1t/trojan-go");
    r->WriteText("trojan-go");
    r->EndURL();

    r->Newline();

    r->BeginURL("https://www.privoxy.org/");
    r->WriteText("privoxy");
    r->EndURL();

    r->Newline();

    r->BeginURL("https://github.com/Noisyfox/sysproxy");
    r->WriteText("sysproxy");
    r->EndURL();

    r->Newline();

    r->BeginURL("https://github.com/yhirose/cpp-httplib");
    r->WriteText("cpp-httplib");
    r->EndURL();

    r->Newline();

    r->EndUnderline();
    r->EndTextColour();
    r->EndFontSize();

    r->Newline();
    r->Newline();

    r->BeginFontSize(16);
    r->BeginTextColour(wxColour(0x202020));
    r->BeginBold();
    r->WriteText("Announcement");
    r->EndBold();
    r->Newline();
    r->EndTextColour();
    r->EndFontSize();

    r->Newline();

    r->BeginFontSize(12);
    r->BeginTextColour(wxColour(0x404040));

    r->WriteText("This project is just for research and education purpose,");
    r->Newline();
    r->WriteText("using it requires you to ensure compliance with local laws.");
    r->Newline();
    r->WriteText("Otherwise you are not allow to use it, and should uninstall it immediately.");
    r->Newline();
    r->Newline();

    r->BeginBold();
    r->WriteText("Any violation of the law by anyone using this software has nothing to do with the author.");
    r->Newline();
    r->EndBold();

    r->EndTextColour();
    r->EndFontSize();

    r->EndAlignment();
}

