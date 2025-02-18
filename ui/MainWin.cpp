#include "MainWin.h"
#include <wx/filename.h>
#include <wx/icon.h>

#include "AccountPanel.h"
#include "EnterPanel.h"
#include "FindPanel.h"
#include "ResultPanel.h"

MainWin::MainWin() : wxFrame(NULL, wxID_ANY, "WorldTravel") {
  SetIcon(wxIcon(wxT("ic"), wxBITMAP_TYPE_ICO));

  notebook = new wxNotebook(this, wxID_ANY);

  leftPanel = new LeftPanel(notebook);
  rightPanel = new RightPanel(notebook);
  EnterPanel *enterPanel = new EnterPanel(notebook);
  FindPanel *findPanel = new FindPanel(notebook);
  AccountPanel *accountPanel = new AccountPanel(notebook);
  ResultPanel *resultPanel = new ResultPanel(notebook);

  notebook->AddPage(leftPanel, "Login to account", true);
  notebook->AddPage(rightPanel, "Register");
  notebook->AddPage(enterPanel, "Login");
  notebook->AddPage(findPanel, "Pick up a trip");
  notebook->AddPage(accountPanel, "Personal Account");
  notebook->AddPage(resultPanel, "Tickets Found");
}

MainWin::~MainWin() {}
