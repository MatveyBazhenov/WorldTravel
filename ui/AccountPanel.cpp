#include "AccountPanel.hpp"

AccountPanel::AccountPanel(wxNotebook *parent) : wxPanel(parent, wxID_ANY) {
  wxLocale locale;
  locale.Init(wxLANGUAGE_RUSSIAN);

  wxBoxSizer *mainSizer5 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *topSizer5 = new wxBoxSizer(wxHORIZONTAL);

  btnBack = new wxButton(this, ID_BACK, "Обратно к поиску");
  btnName = new wxButton(this, ID_NAME, "Имя пользователя");
  btnExit = new wxButton(this, ID_EXIT, "Выйти из профиля");
  topSizer5->Add(btnBack, 0, wxALL, 5);
  topSizer5->Add(btnName, 0, wxALL, 5);
  topSizer5->Add(btnExit, 0, wxALL, 5);

  wxGridSizer *centerSizer5 = new wxGridSizer(3, 1, 10, 10);

  btnDrive1 = new wxButton(this, ID_DRIVE1, "Сохранённая поездка 1");
  btnDrive2 = new wxButton(this, ID_DRIVE2, "Сохранённая поездка 2");
  btnDrive3 = new wxButton(this, ID_DRIVE3, "Сохранённая поездка 3");

  btnDrive1->SetMinSize(wxSize(200, 100));
  btnDrive2->SetMinSize(wxSize(200, 100));
  btnDrive3->SetMinSize(wxSize(200, 100));

  centerSizer5->Add(btnDrive1, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer5->Add(btnDrive2, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer5->Add(btnDrive3, 0, wxALIGN_CENTER | wxALL, 5);

  mainSizer5->Add(topSizer5, 0, wxEXPAND | wxALL, 10);
  mainSizer5->Add(centerSizer5, 1, wxEXPAND | wxALL, 10);
  this->SetSizer(mainSizer5);
  Center();
  this->Fit();
  btnExit->Bind(wxEVT_BUTTON, &AccountPanel::OnExit, this);
  btnBack->Bind(wxEVT_BUTTON, &AccountPanel::OnBack, this);
}
void AccountPanel::OnExit(wxCommandEvent &event) {
  wxNotebook *parentNotebook = static_cast<wxNotebook *>(GetParent());
  LeftPanel *leftPanel = new LeftPanel(parentNotebook);
  parentNotebook->AddPage(leftPanel, "Выход", true);
}

void AccountPanel::OnBack(wxCommandEvent &event) {
  wxNotebook *parentNotebook = static_cast<wxNotebook *>(GetParent());
  FindPanel *findPanel = new FindPanel(parentNotebook);
  parentNotebook->AddPage(findPanel, "Поиск", true);
}
AccountPanel::~AccountPanel() {}
