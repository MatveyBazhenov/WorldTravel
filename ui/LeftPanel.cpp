#include "LeftPanel.hpp"
#include "EnterWindow.hpp"
#include "RegistrationWindow.hpp"

LeftPanel::LeftPanel(wxSimplebook *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxBORDER_SUNKEN) {
  _locale.Init(wxLANGUAGE_RUSSIAN);
  this->SetBackgroundColour(wxColour(242, 242, 242)); // светло-серый фон
  if (!m_backgroundBitmap.LoadFile("../images/background1.jpg",
                                   wxBITMAP_TYPE_JPEG)) {
    wxLogError("Could not load background image!");
  }
  Bind(wxEVT_PAINT, &LeftPanel::OnPaint, this);
  Bind(wxEVT_SIZE, &LeftPanel::OnSize, this);
  wxBoxSizer *mainSizer3 = new wxBoxSizer(wxHORIZONTAL);
  mainSizer3->AddStretchSpacer(1);
  wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

  wxStaticText *title = new wxStaticText(this, wxID_ANY, "World Travel");
  title->SetForegroundColour(wxColour(28, 124, 84));
  title->SetFont(
      wxFont(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
  wxStaticText *subtitle =
      new wxStaticText(this, wxID_ANY, "Приложение по подбору путешествий");
  subtitle->SetFont(wxFontInfo(15)
                        .Family(wxFONTFAMILY_SWISS)
                        .FaceName("Roboto Light")
                        .Italic());
  subtitle->SetForegroundColour(*wxBLACK);
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
  //
  btnIn = new RoundedButton(this, ID_IN, "Вход", wxColour(28, 124, 84),
                            *wxWHITE, 45, wxDefaultPosition, wxSize(200, 85));

  btnReg = new RoundedButton(this, ID_REG, "Регистрация", *wxWHITE, *wxBLACK,
                             45, wxDefaultPosition, wxSize(200, 85));

  wxStaticLine *lineLeft = new wxStaticLine(this, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, wxLI_HORIZONTAL);
  wxStaticLine *lineRight = new wxStaticLine(this, wxID_ANY, wxDefaultPosition,
                                             wxDefaultSize, wxLI_HORIZONTAL);
  wxStaticText *lblOr = new wxStaticText(this, wxID_ANY, "ИЛИ");

  lblOr->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC,
                        wxFONTWEIGHT_NORMAL));
  lblOr->SetForegroundColour(*wxBLACK);

  wxBoxSizer *separatorSizer = new wxBoxSizer(wxHORIZONTAL);
  separatorSizer->Add(lineLeft, 1, wxEXPAND | wxRIGHT, 5);
  separatorSizer->Add(lblOr, 0, wxALIGN_CENTER);
  separatorSizer->Add(lineRight, 1, wxEXPAND | wxLEFT, 5);

  wxFont btnFont(wxFontInfo(16).Family(wxFONTFAMILY_SWISS).FaceName("Roboto"));
  btnIn->SetFont(btnFont);
  btnReg->SetFont(btnFont);
  btnIn->SetForegroundColour(*wxWHITE);
  btnReg->SetForegroundColour(*wxBLACK);
  buttonSizer->Add(btnIn, 0, wxALIGN_CENTER | wxALL, 105);
  buttonSizer->Add(separatorSizer, 0, wxEXPAND | wxALL, 15);
  buttonSizer->Add(btnReg, 0, wxALIGN_CENTER | wxALL, 105);
  rightSizer->Add(title, 0, wxALIGN_CENTER | wxTOP, 30);
  rightSizer->Add(subtitle, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 20);
  rightSizer->Add(buttonSizer, 1, wxALIGN_CENTER | wxBOTTOM, 50);

  subtitle->MoveAfterInTabOrder(title);
  mainSizer3->Add(rightSizer, 1, wxEXPAND);
  this->SetSizer(mainSizer3);
  btnIn->MoveAfterInTabOrder(subtitle);
  btnReg->MoveAfterInTabOrder(btnIn);

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

void LeftPanel::OnSize(wxSizeEvent &event) {
  if (m_backgroundBitmap.IsOk()) {
    wxSize newSize = GetClientSize();
    int targetWidth = newSize.x / 2.75;
    int targetHeight = newSize.y;

    wxImage image = m_backgroundBitmap.ConvertToImage();
    image.Rescale(targetWidth, targetHeight, wxIMAGE_QUALITY_HIGH);
    m_scaledBackground = wxBitmap(image);
  }
  Refresh();
  event.Skip();
}

void LeftPanel::OnPaint(wxPaintEvent &event) {
  wxPaintDC dc(this);

  if (m_scaledBackground.IsOk()) {
    dc.DrawBitmap(m_scaledBackground, 0, 0, false);
  }

  event.Skip();
}

LeftPanel::~LeftPanel() {}