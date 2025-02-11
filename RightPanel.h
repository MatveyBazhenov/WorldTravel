#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <wx/panel.h>
#include <wx/wx.h>

class RightPanel : public wxPanel {

public:
  RightPanel(wxPanel *parent);
  ~RightPanel();

private:
  wxButton *btnEx;
  wxPanel parentPanel2;
  void OnExit(wxCommandEvent &event);
};
#endif