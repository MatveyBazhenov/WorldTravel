#include <wx/filename.h>
#include <wx/icon.h>
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

wxBEGIN_EVENT_TABLE(SocketMain, wxApp)
    EVT_SOCKET(SERVER_ID, SocketMain::ServerConnect)
        EVT_SOCKET(SOCKET_ID, SocketMain::ServerSockedData) wxEND_EVENT_TABLE()
            wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    SocketMain *socketMain = new SocketMain();
    MainWin *mainWin = new MainWin();
    mainWin->Show(true);
    return true;
}
