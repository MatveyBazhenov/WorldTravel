#include "MainWin.hpp"

MainWin::MainWin() : wxFrame(NULL, wxID_ANY, "WorldTravel") {
  SetIcon(wxIcon(wxT("ic"), wxBITMAP_TYPE_ICO));
  wxLocale locale;
  locale.Init(wxLANGUAGE_RUSSIAN);
  notebook = new wxNotebook(this, wxID_ANY);

  leftPanel = new LeftPanel(notebook);
  // rightPanel = new RightPanel(notebook);
  // EnterPanel *enterPanel = new EnterPanel(notebook);
  FindPanel *findPanel = new FindPanel(notebook);
  AccountPanel *accountPanel = new AccountPanel(notebook);
  notebook->AddPage(findPanel, "Подобрать путешествие");

  notebook->AddPage(leftPanel, "Мой аккаунт", true);
  // notebook->AddPage(rightPanel, "Зарегистрироваться");
  // notebook->AddPage(enterPanel, "Войти");
  notebook->AddPage(findPanel, "Подобрать путешествие");
  notebook->AddPage(accountPanel, "Личный кабинет");
}

MainWin::~MainWin() {}
