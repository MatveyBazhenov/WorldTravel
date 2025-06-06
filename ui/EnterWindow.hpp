
#ifndef ENTERWINDOW_HPP
#define ENTERWINDOW_HPP

#include "RoundedButton.hpp"
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/msgdlg.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/protocol/http.h>
#include <wx/sstream.h>
#include <wx/utils.h>
#include <wx/wx.h>

const int ID_ENTER = wxID_HIGHEST + 16;
const int ID_LOGIN2 = wxID_HIGHEST + 17;
const int ID_PASSWORD2 = wxID_HIGHEST + 18;

class UserData;

class EnterWindow : public wxFrame {
public:
  EnterWindow(wxWindow *parent = nullptr);
  void OnEnter(wxCommandEvent &event);

private:
  wxString ExtractTokenFromResponse(const wxString &response);

  RoundedButton *btnEnter2;
  wxTextCtrl *txtLogin;
  wxTextCtrl *txtPassword;
  wxBoxSizer *sizer;
  wxStaticText *header;
  wxGridSizer *centsizer;
  wxLocale _locale;

  void CustomMessageBox(wxWindow *parent, const wxString &message,
                        const wxString &title, const wxString &imagePath);

  DECLARE_EVENT_TABLE()
};

#endif
