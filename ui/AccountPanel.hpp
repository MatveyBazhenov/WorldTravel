#ifndef ACCOUNTPANEL_HPP
#define ACCOUNTPANEL_HPP

#include "LeftPanel.hpp"
#include <wx/button.h>
#include "FindPanel.hpp"
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

private:
  wxButton *btnDrive1;
  wxButton *btnDrive2;
  wxButton *btnDrive3;
  wxButton *btnBack;
  wxButton *btnName;
  wxButton *btnExit;
  wxBoxSizer *mainSizer5;
  wxBoxSizer *topSizer5;
  wxGridSizer *centerSizer5;
  wxBoxSizer *bottomSizer5;

  void OnExit(wxCommandEvent &event);
  void OnBack(wxCommandEvent &event);
};

#endif
