#include "FindPanel.hpp"
#include "AccountPanel.hpp"
#include "ResultPanel.hpp"

FindPanel::FindPanel(wxNotebook *parent) : wxPanel(parent, wxID_ANY) {
  wxLocale locale;
  locale.Init(wxLANGUAGE_RUSSIAN);

  wxBoxSizer *mainSizer4 = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *topSizer4 = new wxBoxSizer(wxHORIZONTAL);

  btnAccount = new wxButton(this, ID_ACCOUNT, "Личный кабинет");
  btnAccount->SetMinSize(wxSize(150, 70));
  topSizer4->Add(btnAccount, 0, wxALL, 5);

  wxGridSizer *centerSizer4 = new wxGridSizer(1, 4, 10, 10);

  txtFrom = new wxTextCtrl(this, wxID_ANY);
  txtFrom->SetHint("Откуда");
  txtFrom->SetMinSize(wxSize(150, 70));

  txtTo = new wxTextCtrl(this, wxID_ANY);
  txtTo->SetHint("Куда");
  txtTo->SetMinSize(wxSize(150, 70));

  spinMoney = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition,
                             wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 0);
  wxCalendarCtrl *calendar =
      new wxCalendarCtrl(this, wxID_ANY, wxDateTime::Today(), wxDefaultPosition,
                         wxDefaultSize, wxCAL_MONDAY_FIRST);
  calendar->SetMinSize(wxSize(200, 170));

  spinMoney->SetMinSize(wxSize(150, 70));

  centerSizer4->Add(calendar, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer4->Add(txtTo, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer4->Add(spinMoney, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer4->Add(txtFrom, 0, wxALIGN_CENTER | wxALL, 5);

  wxBoxSizer *bottomSizer4 = new wxBoxSizer(wxHORIZONTAL);
  btnFind = new wxButton(this, ID_FIND, "Найти");
  btnFind->SetMinSize(wxSize(100, 70));
  bottomSizer4->Add(btnFind, 0, wxALL, 5);

  mainSizer4->Add(topSizer4, 0, wxEXPAND | wxALL, 10);
  mainSizer4->Add(centerSizer4, 1, wxEXPAND | wxALL, 10);
  mainSizer4->Add(bottomSizer4, 0, wxALIGN_RIGHT | wxALL, 10);

  this->SetSizer(mainSizer4);

  this->Fit();
  Center();
  btnFind->Bind(wxEVT_BUTTON, &FindPanel::OnFind, this);
  btnAccount->Bind(wxEVT_BUTTON, &FindPanel::OnAccount, this);
}

void FindPanel::OnFind(wxCommandEvent &event) {

  // Берем данные с запроса
  wxString origin = txtFrom->GetValue();
  wxString destination = txtTo->GetValue();
  wxDateTime date = calendar->GetDate();
  wxString departure_at = date.Format("%Y-%m-%d");
  int price = spinMoney->GetValue();

  // JSON
  std::string jsonBody =
      wxString::Format(
          R"({"origin": "%s", "destination": "%s", "departure_at": "%s", "price": %d})",
          origin, destination, departure_at, price)
          .ToStdString();

  // cURL
  CURL *curl = curl_easy_init();
  if (curl) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/http-client");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      wxMessageBox("Ошибка при отправке запроса: " +
                       wxString(curl_easy_strerror(res)),
                   "Ошибка", wxICON_ERROR);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  wxNotebook *parentNotebook = static_cast<wxNotebook *>(GetParent());
  parentNotebook->DeleteAllPages();
  parentNotebook->AddPage(new ResultPanel(parentNotebook), "Результаты", true);

  while (parentNotebook->GetPageCount() > 0) {
    parentNotebook->DeletePage(0);
  }
}

void FindPanel::OnAccount(wxCommandEvent &event) {
  wxNotebook *parentNotebook = static_cast<wxNotebook *>(GetParent());
  while (parentNotebook->GetPageCount() > 0) {
    parentNotebook->DeletePage(0);
  }
  AccountPanel *accountPanel = new AccountPanel(parentNotebook);
  parentNotebook->AddPage(accountPanel, "Личный кабинет", true);
}

FindPanel::~FindPanel() {}