#ifndef MAINWIN_H
#define MAINWIN_H

#include "LeftPanel.h"
#include "RightPanel.h"
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
  RightPanel *rightPanel;
};

#endif
