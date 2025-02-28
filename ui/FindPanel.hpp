#ifndef FINDPANEL_HPP
#define FINDPANEL_HPP

#include "AccountPanel.hpp"
#include "ResultPanel.hpp"
#include <wx/button.h>
#include <wx/datectrl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

const int ID_FIND = wxID_HIGHEST + 20;
const int ID_ACCOUNT = wxID_HIGHEST + 21;

class FindPanel : public wxPanel {
public:
  FindPanel(wxNotebook *parent);
  ~FindPanel();

private:
  wxButton *btnAccount;
  wxButton *btnFind;
  wxTextCtrl *txtFrom;
  wxTextCtrl *txtTo;
  wxSpinCtrl *spinMoney;
  wxDatePickerCtrl *datePicker;

  wxBoxSizer *mainSizer4;
  wxBoxSizer *topSizer4;
  wxGridSizer *centerSizer4;
  wxBoxSizer *bottomSizer4;

  void OnFind(wxCommandEvent &event);
  void OnAccount(wxCommandEvent &event);
};

#endif