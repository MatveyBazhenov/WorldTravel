#ifndef LEFTPANEL_H
#define LEFTPANEL_H
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/wx.h>
class LeftPanel : public wxPanel {
public:
  LeftPanel(wxPanel *parent);
  ~LeftPanel();

private:
  wxButton *btnPlus;
  wxButton *btnMinus;
  wxPanel parentPanel;
  int count;

  void OnPlus(wxCommandEvent &event);
  void OnMinus(wxCommandEvent &event);
};
#endif