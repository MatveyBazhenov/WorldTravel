#ifndef ENTERPANEL_H
#define ENTERPANEL_H

#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/wx.h>

class EnterPanel : public wxPanel {
public:
  EnterPanel(wxNotebook *parent);
  ~EnterPanel();

private:
  wxButton *btnEnter;
  wxButton *btnLogin2;
  wxButton *btnPassword2;
};

#endif
