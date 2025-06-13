#include "ResultPanel.hpp"
#include "AccountPanel.hpp"
#include "FindPanel.hpp"
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
  ticketInfo->SetForegroundColour(wxColour(0, 0, 0));
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
}

void ResultPanel::OnBack2(wxCommandEvent &event) {
  static_cast<wxNotebook *>(GetParent())->SetSelection(1);
}

void ResultPanel::OnSaveOption1(wxCommandEvent &event) {
  wxString token = UserData::GetInstance().GetUserKey();
  if (token.empty()) {
    wxMessageBox("Требуется авторизация", "Ошибка", wxICON_ERROR);
    return;
  }

  if (m_jsonData.is_null()) {
    wxMessageBox("Нет данных для сохранения", "Ошибка", wxICON_ERROR);
    return;
  }

  try {
    nlohmann::json saveData;
    saveData["user_key"] = token.ToStdString();
    saveData["origin_city"] = m_jsonData["origin_city"].get<std::string>();
    saveData["destination_city"] =
        m_jsonData["destination_city"].get<std::string>();
    saveData["origin_IATA"] = m_jsonData["origin_IATA"].get<std::string>();
    saveData["destination_IATA"] =
        m_jsonData["destination_IATA"].get<std::string>();
    saveData["departure_at"] = m_jsonData["departure_at"].get<std::string>();
    saveData["price"] = m_jsonData["price"].get<int>();
    if (m_jsonData.contains("description") &&
        m_jsonData["description"].is_object()) {
      saveData["description_city"] = m_jsonData["description"].dump();
    } else {
      saveData["description_city"] = "";
    }
    std::string jsonStr = saveData.dump();

    wxHTTP http;
    http.SetHeader("Content-Type", "application/json");
    http.SetHeader("Connection", "close");
    http.SetTimeout(10);

    if (!http.Connect("localhost", 8080)) {
      wxMessageBox("Ошибка подключения к серверу", "Ошибка", wxICON_ERROR);
      return;
    }
    http.SetMethod("POST");
    wxMemoryBuffer postData;
    postData.AppendData(jsonStr.c_str(), jsonStr.size());
    http.SetPostBuffer("application/json", postData);

    wxString path = "/save-trip";
    wxInputStream *response = http.GetInputStream(path);
    int responseCode = http.GetResponse();

    if (response) {
      if (responseCode == 200) {
        wxMessageBox("Маршрут сохранён", "Успех", wxICON_INFORMATION);
        UserData::GetInstance().AddDrive(
            saveData["origin_city"].get<std::string>() + " → " +
            saveData["destination_city"].get<std::string>());
      } else {
        wxString errorMsg = wxString::Format("HTTP error %d\n", responseCode);
        if (response->CanRead()) {
          wxStringOutputStream out;
          response->Read(out);
          errorMsg += "Response: " + out.GetString();
        }
        wxMessageBox(errorMsg, "Ошибка сервера", wxICON_ERROR);
      }
      delete response;
    } else {
      wxMessageBox("Ошибка отправки запроса. Код: " +
                       wxString::Format("%d", responseCode),
                   "Ошибка", wxICON_ERROR);
    }
  } catch (const std::exception &e) {
    wxMessageBox("Ошибка при сохранении маршрута: " + wxString(e.what()),
                 "Ошибка", wxICON_ERROR);
  }
}

void ResultPanel::OnShowDescription(wxCommandEvent &event) {
  wxDialog *dialog = new wxDialog(this, wxID_ANY, "Достопримечательности",
                                  wxDefaultPosition, wxSize(1000, 1000));
  dialog->SetBackgroundColour(wxColour(242, 242, 242));

  wxScrolledWindow *scrolled = new wxScrolledWindow(
      dialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
  wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);

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
