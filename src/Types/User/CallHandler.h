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

#ifndef CALLHANDLER_H
#define CALLHANDLER_H

#include <wx/wx.h>
#include "User.h"
#include "../../UI/Elements/Map.h"

class CallHandler : public User
{
public:
    CallHandler() {} // Default Constructor
    CallHandler(const std::string &username, const std::string &firstname, const std::string &lastname) : username(username), firstname(firstname), lastname(lastname){};

    // Copy constructor
    CallHandler(const CallHandler &other)
        : username(other.username), firstname(other.firstname), lastname(other.lastname) {}

    // Copy assignment operator
    CallHandler &operator=(const CallHandler &other)
    {
        if (this != &other)
        {
            username = other.username;
            firstname = other.firstname;
            lastname = other.lastname;
        }
        return *this;
    }

    void AcceptEmergency(wxCommandEvent &event, wxWindow &parent) const;
    void PrioritiseEmergency(wxCommandEvent &event, wxWindow &parent, Emergency emergency, int emergencyPriority);
    void RouteEmergency(wxWindow &parent) const;

    std::string GetName()
    {
        return firstname + " " + lastname;
    };

private:
    std::string firstname;
    std::string lastname;
    std::string username;
};

#endif