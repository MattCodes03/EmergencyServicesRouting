#ifndef HOSPITALADMIN_H
#define HOSPITALADMIN_H

#include <wx/wx.h>
#include "User.h"

class HospitalAdmin : public User
{
public:
    HospitalAdmin(const string &username);

    void UpdateHospitalStatus(wxWindow &parent);

    int hospitalNumber = -1;
    Hospital activeHospital;
};

#endif