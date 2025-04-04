#ifndef REGISTRATIONWINDOW_HPP
#define REGISTRATIONWINDOW_HPP

#include <wx/button.h>
#include <wx/intl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/protocol/http.h>
#include <wx/sstream.h>
#include <wx/wx.h>

const int ID_LOGIN = wxID_HIGHEST + 9;
const int ID_PASSWORD = wxID_HIGHEST + 10;
const int ID_REG2 = wxID_HIGHEST + 11;

class RegistrationWindow : public wxFrame {
public:
    RegistrationWindow(wxWindow *parent = nullptr);
    void OnRegisterButtonClicked(wxCommandEvent &event);

private:
    wxTextCtrl *txtLogin;
    wxTextCtrl *txtPassword;
    wxButton *btnReg;
    wxBoxSizer *mainSizer2;
    wxGridSizer *centerSizer2;

    wxDECLARE_EVENT_TABLE();
};

#endif
