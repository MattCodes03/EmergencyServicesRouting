#ifndef HOSPITALADMIN_H
#define HOSPITALADMIN_H

#include "User.h"

class HospitalAdmin : public User
{
public:
    HospitalAdmin(const string &username);

    void updateHospitalStatus();

    // Hospital hospital - Implement this once Hospital Class has been created.
};

#endif