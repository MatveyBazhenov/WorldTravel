
#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <wx/string.h>
#include <vector>

class UserData {
public:
    static UserData &GetInstance();
    void AddDrive(const wxString &drive);
    const std::vector<wxString> &GetDrives() const;
    UserData(const UserData &) = delete;
    UserData &operator=(const UserData &) = delete;

private:
    UserData() = default;
    std::vector<wxString> savedDrives;
};

#endif  // USERDATA_HPP
