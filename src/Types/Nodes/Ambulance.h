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

#ifndef AMBULANCE_H
#define AMBULANCE_H

#include <string>
#include <vector>

class Ambulance
{
public:
    Ambulance(){};
    Ambulance(int unitNumber, std::pair<int, int> location, bool status, bool available, int activeEmergency) : unitNumber(unitNumber), location(location), status(status), available(available), activeEmergency(activeEmergency) {}

    int unitNumber;
    std::pair<int, int> location;
    bool status;
    bool available;
    int activeEmergency;
};

#endif