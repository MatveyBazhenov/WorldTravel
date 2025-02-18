#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/wx.h>

class RightPanel : public wxPanel {
public:
  RightPanel(wxNotebook *parent);
  ~RightPanel();

private:
  wxButton *btnLogin;
  wxButton *btnPassword;
  wxButton *btnReg2;
};

#endif
