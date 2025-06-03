#include "ResultPanel.hpp"
#include "FindPanel.hpp"
#include "UserData.hpp"
#include <wx/log.h>

#include "FindPanel.hpp"
#include "ResultPanel.hpp"
#include "UserData.hpp"
#include <wx/log.h>
#include <wx/statline.h>

ResultPanel::ResultPanel(wxNotebook *parent, const std::string &responseData)
    : wxPanel(parent, wxID_ANY) {
  _locale.Init(wxLANGUAGE_RUSSIAN);
  this->SetBackgroundColour(wxColour(242, 242, 242));

  try {
    m_jsonData = nlohmann::json::parse(responseData);
  } catch (const std::exception &e) {
    wxLogError("JSON parse error: %s", e.what());
    wxMessageBox("Ошибка обработки данных сервера", "Ошибка", wxICON_ERROR);
    m_jsonData = nlohmann::json();
  }

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

  wxStaticText *ticketInfo = new wxStaticText(this, wxID_ANY, "");
  ticketInfo->SetFont(
      wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

  if (!m_jsonData.is_null()) {
    wxString infoText =
        wxString::Format("Информация о билете:\n\n"
                         "1. Откуда: %s\n"
                         "2. Куда: %s\n"
                         "3. Стоимость: %d руб.\n"
                         "4. Время вылета: %s",
                         m_jsonData["origin_city"].get<std::string>(),
                         m_jsonData["destination_city"].get<std::string>(),
                         m_jsonData["price"].get<int>(),
                         m_jsonData["departure_at"].get<std::string>());
    ticketInfo->SetLabel(infoText);
  } else {
    ticketInfo->SetLabel("Данные о билете недоступны");
  }

  wxGridSizer *centerSizer = new wxGridSizer(2, 1, 10, 10);
  btnOption1 = new wxButton(this, ID_OPTION1, "Вариант 1");
  btnOption1->SetMinSize(wxSize(200, 100));

  centerSizer->Add(ticketInfo, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer->Add(btnOption1, 0, wxALIGN_CENTER | wxALL, 5);

  wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
  btnSaveOption1 = new wxButton(this, ID_SAVEOPTION1, "Сохранить маршрут");

  bottomSizer->Add(btnSaveOption1, 0, wxALL, 5);

  mainSizer->Add(topSizer, 0, wxEXPAND | wxALL, 10);
  mainSizer->Add(centerSizer, 1, wxEXPAND | wxALL, 10);
  mainSizer->Add(bottomSizer, 0, wxALIGN_RIGHT | wxALL, 10);

  this->SetSizer(mainSizer);
  this->Layout();
  Center();
  this->Fit();
  btnDescribtion->Bind(wxEVT_BUTTON, &ResultPanel::OnShowDescription, this);
  btnBack2->Bind(wxEVT_BUTTON, &ResultPanel::OnBack2, this);
  btnSaveOption1->Bind(wxEVT_BUTTON, &ResultPanel::OnSaveOption1, this);
  btnSaveAll->Bind(wxEVT_BUTTON, &ResultPanel::OnSaveAll, this);
}

void ResultPanel::OnBack2(wxCommandEvent &event) {
  wxNotebook *parentNotebook = static_cast<wxNotebook *>(GetParent());
  FindPanel *findPanel = new FindPanel(parentNotebook);
  parentNotebook->AddPage(findPanel, "Поиск", true);
}

void ResultPanel::OnSaveOption1(wxCommandEvent &event) {
  UserData::GetInstance().AddDrive(btnOption1->GetLabel().ToStdString());
}

void ResultPanel::OnSaveAll(wxCommandEvent &event) {
  UserData::GetInstance().AddDrive("Все маршруты");
}

void ResultPanel::OnShowDescription(wxCommandEvent &event) {
  wxDialog *dialog = new wxDialog(this, wxID_ANY, "Достопримечательности",
                                  wxDefaultPosition, wxSize(500, 400));
  dialog->SetBackgroundColour(wxColour(242, 242, 242));

  wxScrolledWindow *scrolled = new wxScrolledWindow(
      dialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
  wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);

  wxLogMessage("Проверка наличия описания: %d",
               !m_jsonData.is_null() && m_jsonData.contains("description") &&
                   m_jsonData["description"].is_object());

  if (!m_jsonData.is_null() && m_jsonData.contains("description") &&
      m_jsonData["description"].is_object()) {

    for (auto &item : m_jsonData["description"].items()) {
      std::string key = item.key();
      std::string value = item.value().get<std::string>();

      wxStaticText *title =
          new wxStaticText(scrolled, wxID_ANY, wxString::FromUTF8(key.c_str()));
      title->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                            wxFONTWEIGHT_BOLD));

      wxStaticText *desc = new wxStaticText(scrolled, wxID_ANY,
                                            wxString::FromUTF8(value.c_str()));
      desc->Wrap(450);

      contentSizer->Add(title, 0, wxTOP | wxLEFT | wxRIGHT, 10);
      contentSizer->Add(desc, 0, wxBOTTOM | wxLEFT | wxRIGHT, 15);
      contentSizer->Add(new wxStaticLine(scrolled), 0,
                        wxEXPAND | wxLEFT | wxRIGHT, 5);
    }
  } else {
    wxString errorMsg = "Описания достопримечательностей недоступны";

    if (m_jsonData.is_null())
      errorMsg += " (JSON is null)";
    else if (!m_jsonData.contains("description"))
      errorMsg += " (field missing)";
    else if (!m_jsonData["description"].is_object())
      errorMsg += " (not object)";

    contentSizer->Add(new wxStaticText(scrolled, wxID_ANY, errorMsg), 0, wxALL,
                      20);
  }

  scrolled->SetSizer(contentSizer);
  scrolled->SetScrollRate(0, 20);
  scrolled->FitInside();

  wxBoxSizer *scrolledSizer = new wxBoxSizer(wxVERTICAL);
  scrolledSizer->Add(scrolled, 1, wxEXPAND | wxALL, 10);

  wxButton *btnClose = new wxButton(dialog, wxID_OK, "Закрыть");

  dialogSizer->Add(scrolledSizer, 1, wxEXPAND);
  dialogSizer->Add(btnClose, 0, wxALIGN_CENTER | wxBOTTOM | wxTOP, 10);

  dialog->SetSizerAndFit(dialogSizer);
  dialog->Centre();
  dialog->ShowModal();
}

ResultPanel::~ResultPanel() {};