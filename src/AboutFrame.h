//
// Created by Climber on 2020/6/3.
//

#ifndef CLIMBER_ABOUTFRAME_H
#define CLIMBER_ABOUTFRAME_H

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

class AboutFrame : public wxFrame {
public:
    AboutFrame(wxWindow *parent, wxWindowID winid);

protected:
    void SetContent(wxRichTextCtrl *r);
};


#endif //CLIMBER_ABOUTFRAME_H
