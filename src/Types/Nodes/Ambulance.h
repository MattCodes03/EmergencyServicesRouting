#pragma once
#include <string>
#include <vector>

using namespace std;

class Ambulance
{
public:
    Ambulance(int unitNumber, pair<int, int> location, bool status, bool available) : unitNumber(unitNumber), location(location), status(status), available(available), activeEmergency(activeEmergency) {}

    int unitNumber;
    pair<int, int> location;
    bool status;
    bool available;
    int activeEmergency;
};