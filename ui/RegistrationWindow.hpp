#ifndef REGISTRATIONWINDOW_HPP
#define REGISTRATIONWINDOW_HPP

#include "RoundedButton.hpp"
#include "UserData.hpp"
#include <nlohmann/json.hpp>
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
  wxStaticText *header;
  wxTextCtrl *txtLogin;
  wxTextCtrl *txtPassword;
  RoundedButton *btnReg;
  wxBoxSizer *mainSizer2;
  wxGridSizer *centerSizer2;
  wxLocale _locale;
  void CustomMessageBox(wxWindow *parent, const wxString &message,
                        const wxString &title, const wxString &imagePath);
  wxDECLARE_EVENT_TABLE();
};

#endif