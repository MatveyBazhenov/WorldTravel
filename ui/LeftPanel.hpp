#ifndef LEFTPANEL_HPP
#define LEFTPANEL_HPP

#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/wx.h>
#include "EnterWindow.hpp"
#include "RegistrationWindow.hpp"

const int ID_IN = wxID_HIGHEST + 12;
const int ID_REG = wxID_HIGHEST + 13;

class LeftPanel : public wxPanel {
public:
    LeftPanel(wxNotebook *parent);
    ~LeftPanel();

private:
    void OnIn(wxCommandEvent &event);
    void OnReg(wxCommandEvent &event);

    wxButton *btnIn;
    wxButton *btnReg;
    wxBoxSizer *mainSizer3;
    wxGridSizer *centerSizer3;
};

#endif