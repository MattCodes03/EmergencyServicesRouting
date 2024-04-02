#ifndef HOSPITALADMIN_H
#define HOSPITALADMIN_H

#include "User.h"

class HospitalAdmin : public User
{
public:
    HospitalAdmin(const string &username);

    void UpdateHospitalStatus();

    int hospitalNumber = -1;
    Hospital activeHospital;
};

#endif