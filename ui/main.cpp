#include "MainWin.hpp"
#include <wx/filename.h>
#include <wx/icon.h>
#include <wx/socket.h>
#include <wx/wx.h>

class MyApp : public wxApp {
public:
  virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  wxSocketBase::Initialize();
  MainWin *mainWin = new MainWin();
  mainWin->SetSize(wxGetDisplaySize());
  mainWin->Show(true);
  return true;
}
