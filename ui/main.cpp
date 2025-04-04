#include <wx/filename.h>
#include <wx/icon.h>
#include <wx/socket.h>
#include <wx/wx.h>
#include "AccountPanel.hpp"
#include "FindPanel.hpp"
#include "MainWin.hpp"
#include "ResultPanel.hpp"
#include "SocketMain.hpp"
#include "UserData.hpp"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    wxSocketBase::Initialize();
    MainWin *mainWin = new MainWin();
    mainWin->Show(true);
    return true;
}
