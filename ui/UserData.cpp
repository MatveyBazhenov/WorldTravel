#include "UserData.hpp"

UserData &UserData::GetInstance() {
  static UserData instance;
  return instance;
}

void UserData::AddDrive(const wxString &drive) { savedDrives.push_back(drive); }

const std::vector<wxString> &UserData::GetDrives() const { return savedDrives; }

void UserData::SetUsername(const wxString &username) { m_username = username; }

const wxString &UserData::GetUsername() const { return m_username; }

void UserData::DestroyToken() {
  if (!m_token.empty()) {
    wxString temp = m_token;
    wxCharBuffer buffer = temp.utf8_str();
    size_t len = buffer.length();

    if (len > 0) {
      memset(buffer.data(), 0, len);
    }
    temp.clear();
    m_token.clear();
  }
}

UserData::~UserData() {
  DestroyToken(); 
}

void UserData::SetToken(const wxString& token) {
    m_token = token;
}

const wxString& UserData::GetToken() const {
    return m_token;
}