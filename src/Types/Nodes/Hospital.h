#ifndef HOSPITAL_H
#define HOSPITAL_H

class Hospital
{
public:
    Hospital(){};
    Hospital(int hospitalNumber, string name, pair<int, int> location, bool status) : hospitalNumber(hospitalNumber), name(name), location(location), status(status) {}

    int hospitalNumber;
    pair<int, int> location;
    bool status;
    string name;
};

#endif