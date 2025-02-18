#include "RightPanel.h"

const int ID_LOGIN = 103;
const int ID_PASSWORD = 104;
const int ID_REG2 = 105;
RightPanel::RightPanel(wxNotebook *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(270, 150),
              wxBORDER_SUNKEN) {
  btnLogin = new wxButton(this, ID_LOGIN, wxT("Логин"), wxPoint(900, 300),
                          wxSize(150, 80));
  btnPassword = new wxButton(this, ID_PASSWORD, wxT("Пароль"),
                             wxPoint(900, 500), wxSize(150, 80));
  btnReg2 = new wxButton(this, ID_REG2, wxT("Зарегестрироваться"),
                         wxPoint(900, 700), wxSize(150, 80));
}

RightPanel::~RightPanel() {}
