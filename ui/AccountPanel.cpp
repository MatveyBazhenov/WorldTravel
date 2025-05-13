#include "AccountPanel.hpp"
#include "FindPanel.hpp"
#include "LeftPanel.hpp"
#include "UserData.hpp"

AccountPanel::AccountPanel(wxNotebook *parent) : wxPanel(parent, wxID_ANY) {
  wxLocale locale;
  locale.Init(wxLANGUAGE_RUSSIAN);
  this->SetBackgroundColour(wxColour(242, 242, 242));

  mainSizer5 = new wxBoxSizer(wxVERTICAL);
  centerSizer5 = new wxFlexGridSizer(3, 10, 10);
  topSizer5 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *topSizer5 = new wxBoxSizer(wxHORIZONTAL);

  wxString username = UserData::GetInstance().GetUsername();
  txtName = new wxTextCtrl(this, ID_NAME, username);
  txtName->SetEditable(false);
  btnBack = new wxButton(this, ID_BACK, "Обратно к поиску");
  btnExit = new wxButton(this, ID_EXIT, "Выйти из профиля");
  topSizer5->Add(btnBack, 0, wxALL, 5);
  topSizer5->Add(txtName, 0, wxALL, 5);
  topSizer5->Add(btnExit, 0, wxALL, 5);
  mainSizer5->Add(topSizer5, 0, wxEXPAND | wxALL, 10);
  mainSizer5->Add(centerSizer5, 1, wxEXPAND | wxALL, 10);
  this->SetSizer(mainSizer5);
  Center();
  this->Fit();
  btnExit->Bind(wxEVT_BUTTON, &AccountPanel::OnExit, this);
  btnBack->Bind(wxEVT_BUTTON, &AccountPanel::OnBack, this);
  UpdateDriveButtons();
}

void AccountPanel::OnExit(wxCommandEvent &event) {
  wxNotebook *parentNotebook = static_cast<wxNotebook *>(GetParent());
  while (parentNotebook->GetPageCount() > 0) {
    parentNotebook->DeletePage(0);
  }
  LeftPanel *leftPanel = new LeftPanel(parentNotebook);
  parentNotebook->AddPage(leftPanel, "Выход", true);
}

void AccountPanel::OnBack(wxCommandEvent &event) {
  wxNotebook *parentNotebook = static_cast<wxNotebook *>(GetParent());
  while (parentNotebook->GetPageCount() > 0) {
    parentNotebook->DeletePage(0);
  }
  FindPanel *findPanel = new FindPanel(parentNotebook);
  parentNotebook->AddPage(findPanel, "Поиск", true);
}

void AccountPanel::UpdateDriveButtons() {
  for (auto *btn : driveButtons) {
    btn->Destroy();
  }
  driveButtons.clear();
  centerSizer5->Clear();
  const auto &drives = UserData::GetInstance().GetDrives();
  for (const auto &drive : drives) {
    wxButton *btn = new wxButton(this, wxID_ANY, drive);
    btn->SetMinSize(wxSize(200, 100));
    centerSizer5->Add(btn, 0, wxALIGN_CENTER | wxALL, 5);
    driveButtons.push_back(btn);
  }
  centerSizer5->Layout();
  Layout();
  Fit();
}

void AccountPanel::RefreshDrives() { UpdateDriveButtons(); }

AccountPanel::~AccountPanel() {}
