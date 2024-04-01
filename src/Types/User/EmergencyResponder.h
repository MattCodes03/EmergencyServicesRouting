#ifndef EMERGENCYRESPONDER_H
#define EMERGENCYRESPONDER_H

#include <wx/wx.h>
#include "User.h"

class EmergencyResponder : public User
{
public:
    EmergencyResponder(const string &username);

    void GenerateHospitalRoute(wxWindow &parent);
    void CompleteEmergency(wxWindow &parent);

    void CheckForCurrentEmergency(wxWindow &parent);

    void ArriveAtEmergency(wxWindow &parent);

    int unitNumber;
    int currentEmergency = -1;

    bool hospitalRoute = false;
    int hospitalRouteID;
    bool arrivedAtEmergency = false;
};

#endif