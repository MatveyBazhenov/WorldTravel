#include "EnterPanel.h"

const int ID_ENTER = 109;
const int ID_LOGIN2 = 107;
const int ID_PASSWORD2 = 108;
EnterPanel::EnterPanel(wxNotebook *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxBORDER_SUNKEN) {
  btnLogin2 = new wxButton(this, ID_LOGIN2, wxT("Логин"), wxPoint(900, 300),
                           wxSize(150, 80));
  btnPassword2 = new wxButton(this, ID_PASSWORD2, wxT("Пароль"),
                              wxPoint(900, 500), wxSize(150, 80));
  btnEnter = new wxButton(this, ID_ENTER, wxT("Войти"), wxPoint(900, 700),
                          wxSize(150, 80));
}

EnterPanel::~EnterPanel() {}
