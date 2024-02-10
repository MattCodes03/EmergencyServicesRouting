#include "User.h"

class HospitalAdmin : User
{
public:
    HospitalAdmin(const string &username);

    void updateHospitalStatus();

    // Hospital hospital - Implement this once Hospital Class has been created.
};