#ifndef ACCOUNTPANEL_HPP
#define ACCOUNTPANEL_HPP

#include "RoundedButton.hpp"
#include "UserData.hpp"
#include <vector>
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/protocol/http.h> // Add HTTP client
#include <wx/simplebook.h>
#include <wx/wx.h>

struct Trip {
  int trip_id;
  wxString origin_city;
  wxString destination_city;
  wxString origin_IATA;
  wxString destination_IATA;
  wxString departure_at;
  int price;

  wxString ToString() const { return origin_city + " → " + destination_city; }
};

const int ID_DRIVE1 = wxID_HIGHEST + 19;
const int ID_DRIVE2 = wxID_HIGHEST + 20;
const int ID_DRIVE3 = wxID_HIGHEST + 21;
const int ID_BACK = wxID_HIGHEST + 22;
const int ID_NAME = wxID_HIGHEST + 23;
const int ID_EXIT = wxID_HIGHEST + 24;

class AccountPanel : public wxPanel {
public:
  AccountPanel(wxSimplebook *parent);
  ~AccountPanel();

  void UpdateRouteButtons();
  void RefreshRoutes();

private:
  wxButton *btnBack;
  wxString username;
  wxStaticText *txtName;
  wxButton *btnExit;
  wxBoxSizer *mainSizer5;
  wxBoxSizer *topSizer5;
  wxFlexGridSizer *centerSizer5;
  wxBoxSizer *bottomSizer5;
  wxLocale _locale;
  std::vector<RoundedButton *> routeButtons;

  void OnExit(wxCommandEvent &event);
  void OnBack(wxCommandEvent &event);
  std::vector<wxButton *> driveButtons;
  std::vector<Trip> trips_;

  void FetchTripsFromServer();
  void ParseJSONResponse(const wxString &response);

  void OnShow(wxShowEvent &event) {
    if (event.IsShown()) {
      FetchTripsFromServer();
    }
    event.Skip();
  }
};
#endif
