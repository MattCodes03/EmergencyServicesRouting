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

#ifndef HOSPITALADMIN_H
#define HOSPITALADMIN_H

#include <wx/wx.h>
#include "User.h"

class HospitalAdmin : public User
{
public:
    HospitalAdmin(const std::string &username);

    void UpdateHospitalStatus(wxWindow &parent);

    int hospitalNumber = -1;
    Hospital activeHospital;
};

#endif