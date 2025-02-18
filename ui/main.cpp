#include "MainWin.h"
#include <wx/wx.h>

class MyApp : public wxApp {
public:
  virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  MainWin *mainWin = new MainWin();
  mainWin->Show(true);
  return true;
}
