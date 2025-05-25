#ifndef ACCOUNTPANEL_HPP
#define ACCOUNTPANEL_HPP

#include <vector>
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/wx.h>

const int ID_DRIVE1 = wxID_HIGHEST + 19;
const int ID_DRIVE2 = wxID_HIGHEST + 20;
const int ID_DRIVE3 = wxID_HIGHEST + 21;
const int ID_BACK = wxID_HIGHEST + 22;
const int ID_NAME = wxID_HIGHEST + 23;
const int ID_EXIT = wxID_HIGHEST + 24;

class AccountPanel : public wxPanel {
public:
  AccountPanel(wxNotebook *parent);
  ~AccountPanel();
  void RefreshDrives();

private:
  wxButton *btnBack;
  wxTextCtrl *txtName;
  wxButton *btnExit;
  wxBoxSizer *mainSizer5;
  wxBoxSizer *topSizer5;
  wxFlexGridSizer *centerSizer5;
  wxBoxSizer *bottomSizer5;
  wxLocale _locale;

  void OnExit(wxCommandEvent &event);
  void OnBack(wxCommandEvent &event);
  std::vector<wxButton *> driveButtons;
  void UpdateDriveButtons();
};

#endif
