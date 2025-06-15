#include "AccountPanel.hpp"
#include "FindPanel.hpp"
#include "LeftPanel.hpp"
#include "RoundedButton.hpp"
#include "UserData.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

AccountPanel::AccountPanel(wxSimplebook *parent) : wxPanel(parent, wxID_ANY) {
  _locale.Init(wxLANGUAGE_RUSSIAN);
  this->SetBackgroundColour(wxColour(242, 242, 242));

  mainSizer5 = new wxBoxSizer(wxVERTICAL);
  centerSizer5 = new wxFlexGridSizer(3, 10, 10);
  topSizer5 = new wxBoxSizer(wxHORIZONTAL);

  wxFont btnFont(wxFontInfo(12).Family(wxFONTFAMILY_SWISS).FaceName("Roboto"));

  wxBitmap mapBitmap;
  if (mapBitmap.LoadFile("../images/map.png", wxBITMAP_TYPE_ANY)) {
    wxImage mapImage = mapBitmap.ConvertToImage();
    mapImage.Rescale(32, 32, wxIMAGE_QUALITY_HIGH);
    mapBitmap = wxBitmap(mapImage);

    btnBack = new wxButton(this, ID_BACK, " Обратно к поиску",
                           wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    btnBack->SetBitmap(mapBitmap);
  } else {
    btnBack = new wxButton(this, ID_BACK, "Обратно к поиску");
  }
  btnBack->SetFont(btnFont);
  btnBack->SetBackgroundColour(wxColour(242, 242, 242));
  btnBack->SetForegroundColour(*wxBLACK);

  wxBitmap userBitmap;
  if (userBitmap.LoadFile("../images/user.png", wxBITMAP_TYPE_ANY)) {
    btnExit = new wxButton(this, ID_EXIT, " Выйти из профиля",
                           wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    btnExit->SetBitmap(userBitmap);
  } else {
    btnExit = new wxButton(this, ID_EXIT, "Выйти из профиля");
  }
  btnExit->SetFont(btnFont);
  btnExit->SetBackgroundColour(wxColour(242, 242, 242));
  btnExit->SetForegroundColour(*wxBLACK);

  wxString username = UserData::GetInstance().GetUsername();
  txtName = new wxStaticText(this, ID_NAME, username);
  txtName->SetForegroundColour(*wxBLACK);
  txtName->SetFont(
      wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

  topSizer5->Add(txtName, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);
  topSizer5->Add(btnBack, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);
  topSizer5->Add(btnExit, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);

  mainSizer5->Add(topSizer5, 0, wxEXPAND | wxALL, 10);

  wxBoxSizer *centerWrapper = new wxBoxSizer(wxVERTICAL);
  centerWrapper->AddStretchSpacer();
  centerWrapper->Add(centerSizer5, 0, wxALIGN_CENTER);
  centerWrapper->AddStretchSpacer();

  mainSizer5->Add(centerWrapper, 1, wxEXPAND | wxALL, 10);

  this->SetSizer(mainSizer5);
  Center();
  this->Fit();

  btnExit->Bind(wxEVT_BUTTON, &AccountPanel::OnExit, this);
  btnBack->Bind(wxEVT_BUTTON, &AccountPanel::OnBack, this);
  wxSize parentSize = parent->GetClientSize();
  this->SetSize(parentSize);
  this->SetMinSize(parentSize);
  this->Layout();
  this->Fit();
  this->Refresh();
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
  username = UserData::GetInstance().GetUsername();
  txtName->SetLabel(username);
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
    RoundedButton *btn = new RoundedButton(
        this, wxID_ANY, trip.ToString(), wxColour(255, 255, 255),
        wxColour(0, 0, 0), 10, wxDefaultPosition, wxSize(300, 100));

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
  static_cast<wxSimplebook *>(GetParent())->SetSelection(0);
  UserData::GetInstance().ClearRoutes();
  UserData::GetInstance().DestroyUserKey();
}

void AccountPanel::OnBack(wxCommandEvent &event) {
  static_cast<wxSimplebook *>(GetParent())->SetSelection(1);
}

void AccountPanel::RefreshRoutes() { FetchTripsFromServer(); }

AccountPanel::~AccountPanel() {}