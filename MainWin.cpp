#include "MainWin.h"
#include <wx/filename.h>
#include <wx/icon.h>

MainWin::MainWin() : wxFrame(NULL, wxID_ANY, "WorldTravel") {
  SetIcon(wxIcon(wxT("ic"), wxBITMAP_TYPE_ICO));
  panel = new wxPanel(this, wxID_ANY);
  hBox = new wxBoxSizer(wxHORIZONTAL);
  leftPanel = new LeftPanel(panel);
  rightPanel = new RightPanel(panel);
  hBox->Add(leftPanel, 1, wxEXPAND | wxALL, 5);
  hBox->Add(rightPanel, 1, wxEXPAND | wxALL, 5);
  panel->SetSizer(hBox);
}

MainWin::~MainWin() {}