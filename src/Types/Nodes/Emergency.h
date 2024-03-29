#ifndef EMERGENCY_H
#define EMERGENCY_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Emergency
{
public:
    Emergency(){};
    Emergency(int emergencyNumber,
              pair<int, int> location,
              int priority,
              string description,
              bool respondedTo,
              bool complete) : emergencyNumber(emergencyNumber), location(location), priority(priority), description(description), respondedTo(respondedTo), complete(complete){};

    // Overload the < operator for this class to compare priorities, this is used when queueing emergencies
    bool operator<(const Emergency &other) const
    {
        return this->priority < other.priority;
    }

    // Overload << operator to define string representation
    friend std::ostream &operator<<(std::ostream &os, const Emergency &obj)
    {
        os << obj.emergencyNumber << " - " << obj.priority;
        return os;
    }

    int emergencyNumber;
    pair<int, int> location;
    int priority;
    string description;
    bool respondedTo;
    bool complete;
};

#endif