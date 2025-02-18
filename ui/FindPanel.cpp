#include "FindPanel.h"

const int ID_FROM = 110;
const int ID_TO = 111;
const int ID_MONEY = 112;
const int ID_DATA = 113;
const int ID_FIND = 114;
const int ID_ACCOUNT = 115;

FindPanel::FindPanel(wxNotebook *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxBORDER_SUNKEN) {
  btnFrom = new wxButton(this, ID_FROM, wxT("Откуда"), wxPoint(500, 300),
                         wxSize(150, 80));
  btnTo = new wxButton(this, ID_TO, wxT("Куда"), wxPoint(700, 300),
                       wxSize(150, 80));
  btnMoney = new wxButton(this, ID_MONEY, wxT("Бюджет"), wxPoint(900, 300),
                          wxSize(150, 80));
  btnData = new wxButton(this, ID_DATA, wxT("Дата"), wxPoint(500, 500),
                         wxSize(150, 80));
  btnFind = new wxButton(this, ID_FIND, wxT("Найти"), wxPoint(700, 500),
                         wxSize(150, 80));
  btnAccount = new wxButton(this, ID_ACCOUNT, wxT("Личный кабинет"),
                            wxPoint(1700, 50), wxSize(150, 80));
}

FindPanel::~FindPanel() {}
