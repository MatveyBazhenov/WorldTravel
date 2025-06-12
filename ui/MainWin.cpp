#include "MainWin.hpp"
#include "FindPanel.hpp"
#include <iostream>

wxBEGIN_EVENT_TABLE(MainWin, wxFrame)
    EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, MainWin::OnPageChanged)
        wxEND_EVENT_TABLE()

            MainWin::MainWin()
    : wxFrame(NULL, wxID_ANY, "WorldTravel") {
  SetIcon(wxIcon(wxT("../images/logo.png"), wxBITMAP_TYPE_ICO));
  _locale.Init(wxLANGUAGE_RUSSIAN);
  notebook = new wxNotebook(this, wxID_ANY);
  std::cout << "huy pizda" << std::endl;
  leftPanel = new LeftPanel(notebook);
  FindPanel *findPanel = new FindPanel(notebook); //

  notebook->AddPage(leftPanel, "Мой аккаунт", true);
  notebook->AddPage(findPanel, "Подобрать путешествие"); //
  accountPanel = new AccountPanel(notebook);             //
  std::cout << "huy pizda" << std::endl;
  notebook->AddPage(accountPanel, "Личный кабинет"); //
  std::cout << "huy pizda" << std::endl;
}

void MainWin::OnPageChanged(wxBookCtrlEvent &event) {
  if (notebook->GetCurrentPage() == accountPanel && accountPanel != nullptr) {
    accountPanel->RefreshRoutes();
  }
  event.Skip();
}

MainWin::~MainWin() {}
