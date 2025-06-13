#include "AccountPanel.hpp"
#include "FindPanel.hpp"
#include "LeftPanel.hpp"
#include "UserData.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

AccountPanel::AccountPanel(wxNotebook *parent) : wxPanel(parent, wxID_ANY) {
  _locale.Init(wxLANGUAGE_RUSSIAN);
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
}

void AccountPanel::ParseJSONResponse(const wxString &response) {
  try {
    std::string jsonStr = response.ToStdString();
    auto root = json::parse(jsonStr);
    if (root.contains("error")) {
      wxMessageBox(root["error"].get<std::string>(), "Server Error",
                   wxICON_ERROR);
      return;
    }

    trips_.clear();

    if (root.contains("trips") && root["trips"].is_array()) {
      for (const auto &tripVal : root["trips"]) {
        Trip trip;
        trip.trip_id = tripVal["trip_id"].get<int>();
        trip.origin_city = tripVal["origin_city"].get<std::string>();
        trip.destination_city = tripVal["destination_city"].get<std::string>();
        trip.origin_IATA = tripVal["origin_IATA"].get<std::string>();
        trip.destination_IATA = tripVal["destination_IATA"].get<std::string>();
        trip.departure_at = tripVal["departure_at"].get<std::string>();
        trip.price = tripVal["price"].get<int>();

        trips_.push_back(trip);
      }
    }
  } catch (const std::exception &e) {
    wxMessageBox("JSON parsing error: " + wxString(e.what()), "Data Error",
                 wxICON_ERROR);
  }
}

void AccountPanel::FetchTripsFromServer() {
  wxString userKey = UserData::GetInstance().GetUserKey();
  if (userKey.empty()) {
    wxMessageBox("User key is missing", "Error", wxICON_ERROR);
    return;
  }

  wxHTTP http;
  http.SetHeader("Accept", "application/json");
  http.SetTimeout(10);

  if (!http.Connect("localhost", 8080)) {
    wxMessageBox("Could not connect to server", "Network Error", wxICON_ERROR);
    return;
  }

  wxString path = wxString::Format("/account?user_key=%s", userKey);
  wxInputStream *stream = http.GetInputStream(path);

  if (http.GetError() != wxPROTO_NOERR || !stream) {
    wxMessageBox("Failed to fetch account data", "Server Error", wxICON_ERROR);
    return;
  }

  wxString response;
  wxStringOutputStream out_stream(&response);
  stream->Read(out_stream);
  delete stream;

  int status = http.GetResponse();
  if (status != 200) {
    wxMessageBox(wxString::Format("Server returned %d status", status),
                 "Server Error", wxICON_ERROR);
    return;
  }

  ParseJSONResponse(response);
  UpdateRouteButtons();
}

void AccountPanel::UpdateRouteButtons() {
  for (auto *btn : routeButtons) {
    btn->Destroy();
  }
  routeButtons.clear();
  centerSizer5->Clear(true);

  for (const auto &trip : trips_) {
    wxButton *btn = new wxButton(this, wxID_ANY, trip.ToString());
    btn->SetMinSize(wxSize(300, 100));
    centerSizer5->Add(btn, 0, wxALIGN_CENTER | wxALL, 5);
    routeButtons.push_back(btn);

    btn->Bind(wxEVT_BUTTON, [this, trip](wxCommandEvent &) {
      wxString details;
      details << "Trip ID: " << trip.trip_id << "\n"
              << "Origin: " << trip.origin_city << " (" << trip.origin_IATA
              << ")\n"
              << "Destination: " << trip.destination_city << " ("
              << trip.destination_IATA << ")\n"
              << "Departure: " << trip.departure_at << "\n"
              << "Price: " << trip.price << " RUB";

      wxMessageBox(details, "Trip Details");
    });
  }

  centerSizer5->Layout();
  Layout();
  Fit();
}

void AccountPanel::OnExit(wxCommandEvent &event) {
  static_cast<wxNotebook *>(GetParent())->SetSelection(0);
  UserData::GetInstance().ClearRoutes();
  UserData::GetInstance().DestroyUserKey();
}

void AccountPanel::OnBack(wxCommandEvent &event) {
  static_cast<wxNotebook *>(GetParent())->SetSelection(1);
}

void AccountPanel::RefreshRoutes() {
  FetchTripsFromServer(); // Refresh data from server
}

AccountPanel::~AccountPanel() {}