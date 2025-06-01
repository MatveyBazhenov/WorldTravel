#ifndef MAINWIN_HPP
#define MAINWIN_HPP

#include "AccountPanel.hpp"
#include "LeftPanel.hpp"
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/icon.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
class MainWin : public wxFrame {
public:
  MainWin();
  ~MainWin();

private:
  wxNotebook *notebook;
  LeftPanel *leftPanel;
  AccountPanel *accountPanel;
  void OnPageChanged(wxBookCtrlEvent &event);
  wxLocale _locale;
  wxDECLARE_EVENT_TABLE();
};

#endif
