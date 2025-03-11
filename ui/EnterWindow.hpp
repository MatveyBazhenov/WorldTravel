
#ifndef ENTERWINDOW_HPP
#define ENTERWINDOW_HPP

#include <wx/button.h>
#include <wx/intl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/wx.h>

const int ID_ENTER = wxID_HIGHEST + 16;
const int ID_LOGIN2 = wxID_HIGHEST + 17;
const int ID_PASSWORD2 = wxID_HIGHEST + 18;

class EnterWindow : public wxFrame {
public:
    EnterWindow(wxWindow *parent = nullptr);

private:
    wxButton *btnEnter2;
    wxTextCtrl *txtLogin;
    wxTextCtrl *txtPassword;
    wxBoxSizer *sizer;
    wxGridSizer *centsizer;
};

#endif
