#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <vector>
#include <wx/string.h>
#include <wx/wx.h>

class UserData {
public:
  static UserData &GetInstance();
  void AddDrive(const wxString &drive);
  const std::vector<wxString> &GetDrives() const;
  UserData(const UserData &) = delete;
  UserData &operator=(const UserData &) = delete;
  const wxString &GetUsername() const;
  void SetUsername(const wxString &username);

private:
  UserData() = default;
  std::vector<wxString> savedDrives;
  wxString m_username;
};

#endif // USERDATA_HPP
