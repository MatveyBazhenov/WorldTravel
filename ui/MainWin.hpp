#ifndef MAINWIN_HPP
#define MAINWIN_HPP

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/icon.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include "FindPanel.hpp"
#include "LeftPanel.hpp"
#include "UserData.hpp"

class MainWin : public wxFrame {
public:
    MainWin();
    ~MainWin();

private:
    wxNotebook *notebook;
    LeftPanel *leftPanel;
    AccountPanel *accountPanel;
    void OnPageChanged(wxBookCtrlEvent &event);
    wxDECLARE_EVENT_TABLE();
};

#endif
