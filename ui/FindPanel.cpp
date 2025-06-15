#include "FindPanel.hpp"
#include "AccountPanel.hpp"
#include "ResultPanel.hpp"

FindPanel::FindPanel(wxSimplebook *parent) : wxPanel(parent, wxID_ANY) {

  _locale.Init(wxLANGUAGE_RUSSIAN);

  wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *topSizer4 = new wxBoxSizer(wxHORIZONTAL);

  verticalSizer->Add(topSizer4, 0, wxEXPAND | wxALL, 10);

  wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);
  verticalSizer->Add(contentSizer, 1, wxEXPAND);

  wxFont btnFont(wxFontInfo(12).Family(wxFONTFAMILY_SWISS).FaceName("Roboto"));

  this->SetBackgroundColour(wxColour(242, 242, 242));
  Bind(wxEVT_SIZE, &FindPanel::OnSize, this);

  wxBitmap userBitmap;
  if (userBitmap.LoadFile("../images/user.png", wxBITMAP_TYPE_JPEG)) {
    wxStaticBitmap *userIcon = new wxStaticBitmap(this, wxID_ANY, userBitmap);
    topSizer4->Add(userIcon, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
  }

  wxStaticText *accountText =
      new wxStaticText(this, wxID_ANY, "Личный кабинет");
  accountText->SetForegroundColour(*wxBLACK);
  accountText->Bind(wxEVT_LEFT_DOWN, &FindPanel::OnAccountClick, this);
  accountText->SetFont(btnFont);

  accountText->SetMinSize(wxSize(150, -1));
  topSizer4->Add(accountText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT,
                 10);

  topSizer4->AddStretchSpacer();

  wxStaticText *header = new wxStaticText(this, wxID_ANY, "Путешествия");
  header->SetForegroundColour(wxColour(28, 124, 84));
  header->SetFont(wxFont(50, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                         wxFONTWEIGHT_BOLD, false, "Arial"));
  header->SetMinSize(wxSize(500, 80));
  contentSizer->Add(header, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM,
                    40);
  contentSizer->AddSpacer(20);

  wxBoxSizer *directionGroupSizer = new wxBoxSizer(wxVERTICAL);
  wxStaticText *directionHeader =
      new wxStaticText(this, wxID_ANY, "Направление");
  directionHeader->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                  wxFONTWEIGHT_BOLD, false, "Roboto"));
  directionHeader->SetForegroundColour(*wxBLACK);
  directionHeader->SetMinSize(wxSize(200, 30));
  directionGroupSizer->Add(directionHeader, 0,
                           wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

  txtFrom = new wxTextCtrl(this, wxID_ANY);
  txtFrom->SetHint("Откуда");
  txtFrom->SetMinSize(wxSize(400, 60));
  txtFrom->SetBackgroundColour(*wxWHITE);
  txtFrom->SetForegroundColour(*wxBLACK);
  txtFrom->SetFont(btnFont);
  directionGroupSizer->Add(txtFrom, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM,
                           10);

  txtTo = new wxTextCtrl(this, wxID_ANY);
  txtTo->SetHint("Куда");
  txtTo->SetMinSize(wxSize(400, 60));
  txtTo->SetBackgroundColour(*wxWHITE);
  txtTo->SetForegroundColour(*wxBLACK);
  txtTo->SetFont(btnFont);
  directionGroupSizer->Add(txtTo, 0, wxALIGN_CENTER_HORIZONTAL, 0);

  contentSizer->Add(directionGroupSizer, 0,
                    wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 40);

  wxBoxSizer *dateGroupSizer = new wxBoxSizer(wxVERTICAL);
  wxStaticText *dateHeader = new wxStaticText(this, wxID_ANY, "Дата");
  dateHeader->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                             wxFONTWEIGHT_BOLD, false, "Roboto"));
  dateHeader->SetForegroundColour(*wxBLACK);
  dateHeader->SetMinSize(wxSize(200, 30));
  dateGroupSizer->Add(dateHeader, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

  calendar =
      new wxCalendarCtrl(this, wxID_ANY, wxDateTime::Today(), wxDefaultPosition,
                         wxSize(300, 180), wxCAL_MONDAY_FIRST);
  calendar->SetForegroundColour(*wxBLACK);
  calendar->SetBackgroundColour(*wxWHITE);
  dateGroupSizer->Add(calendar, 0, wxALIGN_CENTER_HORIZONTAL, 0);

  contentSizer->Add(dateGroupSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM,
                    40);

  wxBoxSizer *costGroupSizer = new wxBoxSizer(wxVERTICAL);
  wxStaticText *costHeader = new wxStaticText(this, wxID_ANY, "Стоимость");
  costHeader->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                             wxFONTWEIGHT_BOLD, false, "Roboto"));
  costHeader->SetForegroundColour(*wxBLACK);
  costHeader->SetMinSize(wxSize(200, 30));
  costGroupSizer->Add(costHeader, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

  spinMoney = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition,
                             wxSize(300, 40), wxSP_ARROW_KEYS, 0, 100000, 0);
  spinMoney->SetForegroundColour(*wxBLACK);
  spinMoney->SetBackgroundColour(*wxWHITE);
  spinMoney->SetFont(btnFont);
  costGroupSizer->Add(spinMoney, 0, wxALIGN_CENTER_HORIZONTAL, 0);

  contentSizer->Add(costGroupSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM,
                    40);

  wxButton *findButton =
      new wxButton(this, wxID_ANY, "Найти", wxDefaultPosition, wxSize(120, 50));
  findButton->SetForegroundColour(*wxWHITE);
  findButton->SetBackgroundColour(wxColour(28, 124, 84));
  findButton->SetFont(btnFont);
  findButton->Bind(wxEVT_BUTTON, &FindPanel::OnFindClick, this);
  contentSizer->Add(findButton, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 20);

  this->SetSizer(verticalSizer);
  this->Layout();
}

void FindPanel::OnFindClick(wxCommandEvent &event) {
  wxString origin = txtFrom->GetValue();
  wxString destination = txtTo->GetValue();
  wxDateTime date = calendar->GetDate();
  wxString departure_at = date.Format("%Y-%m-%d");
  int price = spinMoney->GetValue();

  if (origin.empty() || destination.empty()) {
    wxMessageBox("Заполните поля 'Откуда' и 'Куда'", "Ошибка", wxICON_ERROR);
    return;
  }

  wxString jsonBody = wxString::Format(
      R"({"origin": "%s", "destination": "%s", "departure_at": "%s", "price": %d})",
      origin, destination, departure_at, price);

  wxHTTP http;
  http.SetTimeout(10);

  try {
    if (http.Connect("localhost", 8080)) {
      http.SetMethod("GET");
      http.SetHeader("Content-Type", "application/json");

      if (!http.SetPostText("application/json", jsonBody)) {
        throw std::runtime_error("Не удалось установить POST-данные");
      }

      wxString response;
      wxInputStream *stream = http.GetInputStream("/http-client");
      if (stream) {
        wxStringOutputStream output(&response);
        stream->Read(output);
        delete stream;
      }

      int responseCode = http.GetResponse();
      wxLogDebug("Код ответа: %d", responseCode);

      switch (responseCode) {
      case 200: {
        nlohmann::json response_json =
            nlohmann::json::parse(response.ToStdString());
        wxSimplebook *parentNotebook = static_cast<wxSimplebook *>(GetParent());
        ResultPanel *resultPanel =
            new ResultPanel(parentNotebook, response.ToStdString());
        parentNotebook->AddPage(resultPanel, "Результаты", true);
        parentNotebook->ChangeSelection(parentNotebook->GetPageCount() - 1);
        break;
      }
      case 400: {
        wxMessageBox("Некорректный запрос", "Ошибка", wxOK | wxICON_ERROR,
                     this);
        break;
      }
      case 405: {
        wxMessageBox("Метод не разрешён. Проверьте настройки сервера.",
                     "Ошибка", wxOK | wxICON_ERROR, this);
        break;
      }
      default: {
        throw std::runtime_error("Неизвестная ошибка сервера: " +
                                 std::to_string(responseCode));
      }
      }
    } else {
      throw std::runtime_error("Не удалось подключиться к серверу");
    }
  } catch (const std::exception &e) {
    wxMessageBox(e.what(), "Ошибка", wxOK | wxICON_ERROR, this);
  } catch (...) {
    wxMessageBox("Неизвестная ошибка", "Ошибка", wxOK | wxICON_ERROR, this);
  }

  http.Close();
}

size_t FindPanel::WriteCallback(void *contents, size_t size, size_t nmemb,
                                std::string *output) {
  size_t total_size = size * nmemb;
  output->append(static_cast<char *>(contents), total_size);
  return total_size;
}

void FindPanel::OnAccountClick(wxMouseEvent &event) {
  static_cast<wxSimplebook *>(GetParent())->SetSelection(2);
}

void FindPanel::OnSize(wxSizeEvent &event) {
  Refresh();
  Layout();
  event.Skip();
}
FindPanel::~FindPanel() {}