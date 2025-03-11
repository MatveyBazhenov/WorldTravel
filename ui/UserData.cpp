#include "UserData.hpp"

UserData &UserData::GetInstance() {
    static UserData instance;
    return instance;
}

void UserData::AddDrive(const wxString &drive) {
    savedDrives.push_back(drive);
}

const std::vector<wxString> &UserData::GetDrives() const {
    return savedDrives;
}