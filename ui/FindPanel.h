#ifndef FINDPANEL_H
#define FINDPANEL_H

#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/wx.h>

class FindPanel : public wxPanel {
public:
  FindPanel(wxNotebook *parent);
  ~FindPanel();

private:
  wxButton *btnFrom;
  wxButton *btnTo;
  wxButton *btnMoney;
  wxButton *btnData;
  wxButton *btnFind;
  wxButton *btnAccount;
};

#endif
