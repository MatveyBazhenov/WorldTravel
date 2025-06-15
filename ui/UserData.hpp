#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <cstring>
#include <memory>
#include <vector>
#include <wx/string.h>
#include <wx/wx.h>

struct FlightRoute {
  wxString originIATA;
  wxString originCity;
  wxString destinationIATA;
  wxString destinationCity;
  wxDateTime departureAt;
  int price;

  FlightRoute() : price(0) {}

  wxString ToString() const {
    return wxString::Format("%s (%s) -> %s (%s)\n%s\n%d руб.", originCity,
                            originIATA, destinationCity, destinationIATA,
                            departureAt.Format("%d %b %Y %H:%M"), price);
  }
};

class UserData {
public:
  void AddRoute(const FlightRoute &route);
  const std::vector<FlightRoute> &GetRoutes() const;
  void ClearRoutes();

  void SetUserKey(const wxString &userKey);
  void DestroyUserKey();

  static UserData &GetInstance();
  void AddDrive(const wxString &drive);
  const std::vector<wxString> &GetDrives() const;
  const wxString &GetUserKey() const;

  UserData(const UserData &) = delete;
  UserData &operator=(const UserData &) = delete;

  const wxString &GetUsername() const;
  void SetUsername(const wxString &username);

private:
  std::vector<FlightRoute> savedRoutes;

  UserData() = default;
  ~UserData();

  wxString m_userKey;

  std::unique_ptr<wxChar[]> m_secureBuffer;
  size_t m_secureLength = 0;

  std::vector<wxString> savedDrives;
  wxString m_username;
};

#endif // USERDATA_HPP