#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/wx.h>

class LeftPanel : public wxPanel {
public:
  LeftPanel(wxNotebook *parent);
  ~LeftPanel();

private:
  wxButton *btnIn;
  wxButton *btnReg;
  int count;

  void OnIn(wxCommandEvent &event);
  void OnReg(wxCommandEvent &event);
};

#endif
