#include "LeftPanel.hpp"
#include "RegistrationWindow.hpp"
#include "EnterWindow.hpp"

LeftPanel::LeftPanel(wxNotebook *parent)
    : wxPanel(
          parent,
          wxID_ANY,
          wxDefaultPosition,
          wxDefaultSize,
          wxBORDER_SUNKEN
      ) {
    wxLocale locale;
    locale.Init(wxLANGUAGE_RUSSIAN);

    wxBoxSizer *mainSizer3 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer *centerSizer3 = new wxGridSizer(2, 1, 5);

    btnIn = new wxButton(this, ID_IN, "Вход");
    btnReg = new wxButton(this, ID_REG, "Регистрация");

    btnIn->SetMinSize(wxSize(200, 100));
    btnReg->SetMinSize(wxSize(200, 100));

    centerSizer3->Add(btnIn, 0, wxALIGN_CENTER | wxALL, 5);

    centerSizer3->Add(btnReg, 0, wxALIGN_CENTER | wxALL, 5);

    mainSizer3->Add(centerSizer3, 1, wxEXPAND | wxALL, 10);
    this->SetSizer(mainSizer3);

    this->Fit();
    btnIn->Bind(wxEVT_BUTTON, &LeftPanel::OnIn, this);

    btnReg->Bind(wxEVT_BUTTON, &LeftPanel::OnReg, this);
}

void LeftPanel::OnIn(wxCommandEvent &event) {
    EnterWindow *enterWindow = new EnterWindow(this->GetParent());
    enterWindow->Show();
}

void LeftPanel::OnReg(wxCommandEvent &event) {
    RegistrationWindow *registrationWindow =
        new RegistrationWindow(this->GetParent());
    registrationWindow->Show();
}

LeftPanel::~LeftPanel() {
}