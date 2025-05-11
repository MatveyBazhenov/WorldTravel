#include "MainWin.hpp"
#include "FindPanel.hpp"

wxBEGIN_EVENT_TABLE(MainWin, wxFrame)
    EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, MainWin::OnPageChanged)
        wxEND_EVENT_TABLE()

            MainWin::MainWin()
    : wxFrame(NULL, wxID_ANY, "WorldTravel") {
  SetIcon(wxIcon(wxT("../images/logo.png"), wxBITMAP_TYPE_ICO));
  wxLocale locale;
  locale.Init(wxLANGUAGE_RUSSIAN);
  notebook = new wxNotebook(this, wxID_ANY);

  leftPanel = new LeftPanel(notebook);
  // FindPanel *findPanel = new FindPanel(notebook); //

  notebook->AddPage(leftPanel, "Мой аккаунт", true);
  // notebook->AddPage(findPanel, "Подобрать путешествие"); //
  // accountPanel = new AccountPanel(notebook);             //
  // notebook->AddPage(accountPanel, "Личный кабинет");     //
}

void MainWin::OnPageChanged(wxBookCtrlEvent &event) {
  if (notebook->GetCurrentPage() == accountPanel && accountPanel != nullptr) {
    accountPanel->RefreshDrives();
  }
  event.Skip();
}

MainWin::~MainWin() {}
