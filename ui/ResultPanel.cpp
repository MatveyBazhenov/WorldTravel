#include "ResultPanel.h"

const int ID_TICKET = 123;
const int ID_OPTION1 = 124;
const int ID_OPTION2 = 125;
const int ID_BACK2 = 126;
const int ID_DESCRIBTION = 127;
const int ID_SAVEALL = 128;
const int ID_SAVEOPTION1 = 129;
const int ID_SAVEOPTION2 = 130;

ResultPanel::ResultPanel(wxNotebook *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxBORDER_SUNKEN) {
  btnTicket = new wxButton(this, ID_TICKET, wxT("Билеты"), wxPoint(180, 300),
                           wxSize(200, 80));
  btnOption1 = new wxButton(this, ID_OPTION1, wxT("Вариант 1"),
                            wxPoint(180, 500), wxSize(200, 80));
  btnOption2 = new wxButton(this, ID_OPTION2, wxT("Вариант 2"),
                            wxPoint(180, 700), wxSize(200, 80));
  btnDescribtion =
      new wxButton(this, ID_DESCRIBTION, wxT("Описание достопримечательностей"),
                   wxPoint(200, 50), wxSize(300, 80));
  btnBack2 = new wxButton(this, ID_BACK2, wxT("Обратно к поиску"),
                          wxPoint(900, 50), wxSize(150, 80));
  btnSaveAll = new wxButton(this, ID_SAVEALL, wxT("Сохранить все маршруты"),
                            wxPoint(1700, 50), wxSize(150, 80));
  btnSaveOption1 =
      new wxButton(this, ID_SAVEOPTION1, wxT("Сохранить первый маршрут"),
                   wxPoint(1700, 50), wxSize(200, 80));
  btnSaveOption2 =
      new wxButton(this, ID_SAVEOPTION2, wxT("Сохранить второй маршрут"),
                   wxPoint(1700, 50), wxSize(200, 80));
}

ResultPanel::~ResultPanel() {}
