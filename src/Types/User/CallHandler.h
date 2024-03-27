#ifndef CALLHANDLER_H
#define CALLHANDLER_H

#include <wx/wx.h>
#include "User.h"
#include "../../UI/Elements/Map.h"

class CallHandler : public User
{
public:
    CallHandler() {}
    CallHandler(const string &username, const string &firstname, const string &lastname) : username(username), firstname(firstname), lastname(lastname){};

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

    string GetName()
    {
        return firstname + " " + lastname;
    };

private:
    string firstname;
    string lastname;
    string username;
};

#endif