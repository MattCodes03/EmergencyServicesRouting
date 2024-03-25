#ifndef EMERGENCYRESPONDER_H
#define EMERGENCYRESPONDER_H

#include "User.h"

class EmergencyResponder : public User
{
public:
    EmergencyResponder(const string &username);

    void generateHospitalRoute();
    void completeEmergency();

    int unitNumber;
};

#endif