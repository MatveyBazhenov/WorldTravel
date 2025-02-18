#ifndef ACCOUNTPANEL_H
#define ACCOUNTPANEL_H

#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/wx.h>

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
};

#endif
