#ifndef MAINWIN_HPP
#define MAINWIN_HPP

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
  //RightPanel *rightPanel;
  
};

#endif
