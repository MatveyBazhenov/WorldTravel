#include "LeftPanel.h"

const int ID_IN = 101;
const int ID_REG = 102;

LeftPanel::LeftPanel(wxNotebook *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN) {
  count = 0;
  btnIn = new wxButton(this, ID_IN, wxT("Вход"), wxPoint(900, 300),  wxSize(150,80));
  btnReg = new wxButton(this, ID_REG, wxT("Регистрация"), wxPoint(900, 500), wxSize(150,80));
}

LeftPanel::~LeftPanel() {}

void LeftPanel::OnIn(wxCommandEvent &event) {}
void LeftPanel::OnReg(wxCommandEvent &event) {}
