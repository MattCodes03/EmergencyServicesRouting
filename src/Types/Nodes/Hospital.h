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

#ifndef HOSPITAL_H
#define HOSPITAL_H

class Hospital
{
public:
    Hospital(){};
    Hospital(int hospitalNumber, std::string name, std::pair<int, int> location, bool status) : hospitalNumber(hospitalNumber), name(name), location(location), status(status) {}

    int hospitalNumber;
    std::pair<int, int> location;
    bool status;
    std::string name;
};

#endif