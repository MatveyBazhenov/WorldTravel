#ifndef LEFTPANEL_HPP
#define LEFTPANEL_HPP

#include "RoundedButton.hpp"
#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <wx/wx.h>

const int ID_IN = wxID_HIGHEST + 12;
const int ID_REG = wxID_HIGHEST + 13;

class LeftPanel : public wxPanel {
public:
  LeftPanel(wxNotebook *parent);
  ~LeftPanel();

private:
  void OnIn(wxCommandEvent &event);
  void OnReg(wxCommandEvent &event);
  void OnPaint(wxPaintEvent &event);
  void OnSize(wxSizeEvent &event);
  RoundedButton *btnIn;
  RoundedButton *btnReg;
  wxBoxSizer *mainSizer3;
  wxGridSizer *centerSizer3;
  wxBitmap m_backgroundBitmap;
  wxBitmap m_scaledBackground;
};

#endif