#pragma once
#include <string>
#include <vector>

using namespace std;

class Ambulance
{
public:
    Ambulance(int unitNumber, pair<int, int> location, bool status, bool available) : unitNumber(unitNumber), location(location), status(status) {}

    int unitNumber;
    pair<int, int> location;
    bool status;
    bool available = true;
    int activeEmergency = 0;
};