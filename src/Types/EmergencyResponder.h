#include "User.h"

class EmergencyResponder : User
{
public:
    EmergencyResponder(const string &username, const int &unitNumber);

    void generateHospitalRoute(int &numOfPaitents);
    void sendFeedback(int &emergencyNumber);
    void completeEmergency();

    int unitNumber;
    // Emergency activeEmergency - Implement this once Emergency Class has been created.
};