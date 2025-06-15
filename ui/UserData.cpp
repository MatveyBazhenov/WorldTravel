#include "UserData.hpp"

UserData &UserData::GetInstance() {
  static UserData instance;
  return instance;
}

void UserData::AddDrive(const wxString &drive) { savedDrives.push_back(drive); }

const std::vector<wxString> &UserData::GetDrives() const { return savedDrives; }

void UserData::SetUsername(const wxString &username) { m_username = username; }

const wxString &UserData::GetUsername() const { return m_username; }
void UserData::DestroyUserKey() {
  if (m_secureBuffer && m_secureLength > 0) {
    memset(m_secureBuffer.get(), 0, m_secureLength * sizeof(wxChar));
  }
  m_secureBuffer.reset();
  m_secureLength = 0;
  m_userKey.clear();
}

UserData::~UserData() { DestroyUserKey(); }

void UserData::SetUserKey(const wxString &userKey) {
  DestroyUserKey();

  m_secureLength = userKey.length();
  if (m_secureLength > 0) {
    m_secureBuffer = std::make_unique<wxChar[]>(m_secureLength + 1);
    memcpy(m_secureBuffer.get(), userKey.c_str(),
           m_secureLength * sizeof(wxChar));
    m_secureBuffer[m_secureLength] = '\0';
  }

  m_userKey = userKey;
}

const wxString &UserData::GetUserKey() const { return m_userKey; }

void UserData::AddRoute(const FlightRoute &route) {
  savedRoutes.push_back(route);
}

const std::vector<FlightRoute> &UserData::GetRoutes() const {
  return savedRoutes;
}

void UserData::ClearRoutes() { savedRoutes.clear(); }