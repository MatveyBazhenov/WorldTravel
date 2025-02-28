#include "EnterWindow.hpp"

EnterWindow::EnterWindow(wxWindow *parent)
    : wxFrame(parent, wxID_ANY, "Вход", wxDefaultPosition, wxDefaultSize) {
  wxLocale locale;
  locale.Init(wxLANGUAGE_RUSSIAN);

  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  wxGridSizer *centsizer = new wxGridSizer(3, 1, 10, 10);

  txtLogin = new wxTextCtrl(this, wxID_ANY);
  txtLogin->SetHint("Логин");
  txtLogin->SetMinSize(wxSize(200, 100));

  txtPassword = new wxTextCtrl(this, wxID_ANY);
  txtPassword->SetHint("Пароль");
  txtPassword->SetMinSize(wxSize(200, 100));

  btnEnter2 = new wxButton(this, ID_ENTER, "Войти");
  btnEnter2->SetMinSize(wxSize(200, 100));

  centsizer->Add(txtLogin, 0, wxALIGN_CENTER | wxALL, 5);
  centsizer->Add(txtPassword, 0, wxALIGN_CENTER | wxALL, 5);
  centsizer->Add(btnEnter2, 0, wxALIGN_CENTER | wxALL, 5);

  sizer->Add(centsizer, 1, wxEXPAND | wxALL, 10);

  this->SetSizer(sizer);
  Center();
  this->Fit();
}