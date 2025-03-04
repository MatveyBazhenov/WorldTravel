#include "RegistrationWindow.hpp"

RegistrationWindow::RegistrationWindow(wxWindow *parent)
    : wxFrame(parent, wxID_ANY, "Регистрация", wxDefaultPosition,
              wxDefaultSize) {

  wxLocale locale;
  locale.Init(wxLANGUAGE_RUSSIAN);

  wxBoxSizer *mainSizer2 = new wxBoxSizer(wxVERTICAL);
  wxGridSizer *centerSizer2 = new wxGridSizer(3, 1, 10, 10);

  txtLogin = new wxTextCtrl(this, wxID_ANY);
  txtLogin->SetHint("Логин");
  txtLogin->SetMinSize(wxSize(200, 100));

  txtPassword = new wxTextCtrl(this, wxID_ANY);
  txtPassword->SetHint("Пароль");
  txtPassword->SetMinSize(wxSize(200, 100));

  btnReg = new wxButton(this, ID_REG2, "Зарегестрироваться");
  btnReg->SetMinSize(wxSize(200, 100));

  centerSizer2->Add(txtLogin, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer2->Add(txtPassword, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer2->Add(btnReg, 0, wxALIGN_CENTER | wxALL, 5);

  mainSizer2->Add(centerSizer2, 1, wxEXPAND | wxALL, 10);
  this->SetSizer(mainSizer2);
  Center();
  this->Fit();
}