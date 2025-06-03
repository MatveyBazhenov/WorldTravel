#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <vector>
#include <wx/string.h>
#include <wx/wx.h>
#include <cstring>

class UserData {
public:
  void SetToken(const wxString &token);  // Single declaration
  void DestroyToken();
  static UserData &GetInstance();
  void AddDrive(const wxString &drive);
  const std::vector<wxString> &GetDrives() const;
  
  UserData(const UserData &) = delete;
  UserData &operator=(const UserData &) = delete;
  
  const wxString &GetUsername() const;
  void SetUsername(const wxString &username);
  const wxString& GetToken() const;  // Keep GetToken

private:
  UserData() = default;
  ~UserData();
  
  wxString m_token;
  std::vector<wxString> savedDrives;
  wxString m_username;
};

#endif // USERDATA_HPP