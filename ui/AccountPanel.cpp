#include "AccountPanel.h"

const int ID_DRIVE1 = 117;
const int ID_DRIVE2 = 118;
const int ID_DRIVE3 = 119;
const int ID_BACK = 120;
const int ID_NAME = 121;
const int ID_EXIT = 122;

AccountPanel::AccountPanel(wxNotebook *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxBORDER_SUNKEN) {
  btnDrive1 = new wxButton(this, ID_DRIVE1, wxT("Сохранённая поездка 1"),
                           wxPoint(180, 300), wxSize(200, 80));
  btnDrive2 = new wxButton(this, ID_DRIVE2, wxT("Сохранённая поездка 2"),
                           wxPoint(180, 500), wxSize(200, 80));
  btnDrive3 = new wxButton(this, ID_DRIVE3, wxT("Сохранённая поездка 3"),
                           wxPoint(180, 700), wxSize(200, 80));
  btnBack = new wxButton(this, ID_BACK, wxT("Обратно к поиску"),
                         wxPoint(200, 50), wxSize(150, 80));
  btnName = new wxButton(this, ID_NAME, wxT("Имя пользователя"),
                         wxPoint(900, 50), wxSize(150, 80));
  btnExit = new wxButton(this, ID_EXIT, wxT("Выйти из профиля"),
                         wxPoint(1700, 50), wxSize(150, 80));
}

AccountPanel::~AccountPanel() {}
