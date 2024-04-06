/*
Copyright (c) 2024, Matthew McCann
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to no conditions.
*/

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
              std::pair<int, int> location,
              int priority,
              std::string description,
              bool respondedTo,
              bool complete) : emergencyNumber(emergencyNumber), location(location), priority(priority), description(description), respondedTo(respondedTo), complete(complete){};

    // Overload the < operator for this class to compare priorities, this is used when queueing emergencies
    bool operator<(const Emergency &other) const
    {
        return this->priority < other.priority;
    }

    // Overload the > operator for this class to compare priorities, this is used when queueing emergencies
    bool operator>(const Emergency &other) const
    {
        return this->priority > other.priority;
    }

    // Overload << operator to define string representation
    friend std::ostream &operator<<(std::ostream &os, const Emergency &obj)
    {
        os << obj.emergencyNumber << " - " << obj.priority;
        return os;
    }

    int emergencyNumber;
    std::pair<int, int> location;
    int priority;
    std::string description;
    bool respondedTo;
    bool complete;
};

#endif