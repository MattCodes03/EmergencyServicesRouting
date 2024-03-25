#ifndef HOSPITAL_H
#define HOSPITAL_H

class Hospital
{
public:
    Hospital(int hospitalNumber, pair<int, int> location, bool status) : hospitalNumber(hospitalNumber), location(location), status(status) {}

    int hospitalNumber;
    pair<int, int> location;
    bool status;
};

#endif