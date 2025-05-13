#include "ResultPanel.hpp"
#include "FindPanel.hpp"
#include "UserData.hpp"

ResultPanel::ResultPanel(wxNotebook *parent) : wxPanel(parent, wxID_ANY) {
  wxLocale locale;
  locale.Init(wxLANGUAGE_RUSSIAN);
  this->SetBackgroundColour(wxColour(242, 242, 242));

  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
  btnDescribtion =
      new wxButton(this, ID_DESCRIBTION, "Описание достопримечательностей");
  btnBack2 = new wxButton(this, ID_BACK2, "Обратно к поиску");
  btnSaveAll = new wxButton(this, ID_SAVEALL, "Сохранить все маршруты");

  topSizer->Add(btnDescribtion, 0, wxALL, 5);
  topSizer->AddStretchSpacer();
  topSizer->Add(btnBack2, 0, wxALL, 5);
  topSizer->Add(btnSaveAll, 0, wxALL, 5);

  wxGridSizer *centerSizer = new wxGridSizer(3, 1, 10, 10);
  btnTicket = new wxButton(this, ID_TICKET, "Билеты");
  btnTicket->SetMinSize(wxSize(200, 100));
  btnOption1 = new wxButton(this, ID_OPTION1, "Вариант 1");
  btnOption1->SetMinSize(wxSize(200, 100));
  btnOption2 = new wxButton(this, ID_OPTION2, "Вариант 2");
  btnOption2->SetMinSize(wxSize(200, 100));

  centerSizer->Add(btnTicket, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer->Add(btnOption1, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer->Add(btnOption2, 0, wxALIGN_CENTER | wxALL, 5);

  wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
  btnSaveOption1 =
      new wxButton(this, ID_SAVEOPTION1, "Сохранить первый маршрут");
  btnSaveOption2 =
      new wxButton(this, ID_SAVEOPTION2, "Сохранить второй маршрут");

  bottomSizer->Add(btnSaveOption1, 0, wxALL, 5);
  bottomSizer->Add(btnSaveOption2, 0, wxALL, 5);

  mainSizer->Add(topSizer, 0, wxEXPAND | wxALL, 10);
  mainSizer->Add(centerSizer, 1, wxEXPAND | wxALL, 10);
  mainSizer->Add(bottomSizer, 0, wxALIGN_RIGHT | wxALL, 10);

  this->SetSizer(mainSizer);
  Center();
  this->Fit();
  btnBack2->Bind(wxEVT_BUTTON, &ResultPanel::OnBack2, this);
  btnSaveOption1->Bind(wxEVT_BUTTON, &ResultPanel::OnSaveOption1, this);
  btnSaveOption2->Bind(wxEVT_BUTTON, &ResultPanel::OnSaveOption2, this);
  btnSaveAll->Bind(wxEVT_BUTTON, &ResultPanel::OnSaveAll, this);
}

void ResultPanel::OnBack2(wxCommandEvent &event) {
  wxNotebook *parentNotebook = static_cast<wxNotebook *>(GetParent());
  FindPanel *findPanel = new FindPanel(parentNotebook);
  parentNotebook->AddPage(findPanel, "Поиск", true);
}

void ResultPanel::OnSaveOption1(wxCommandEvent &event) {
  UserData::GetInstance().AddDrive("Вариант 1");
}

void ResultPanel::OnSaveOption2(wxCommandEvent &event) {
  UserData::GetInstance().AddDrive("Вариант 2");
}

void ResultPanel::OnSaveAll(wxCommandEvent &event) {
  UserData::GetInstance().AddDrive("Все маршруты");
}

ResultPanel::~ResultPanel() {};