#pragma once
#include <string>
#include <vector>

using namespace std;

class Emergency
{
public:
    Emergency(int emergencyNumber,
              pair<int, int> location,
              int priority,
              string description,
              bool respondedTo,
              bool complete) : emergencyNumber(emergencyNumber), location(location), priority(priority), description(description), respondedTo(respondedTo), complete(complete){};

    int emergencyNumber;
    pair<int, int> location;
    int priority;
    string description;
    bool respondedTo;
    bool complete;
};