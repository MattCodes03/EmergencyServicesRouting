#ifndef CALLHANDLER_H
#define CALLHANDLER_H

#include <wx/wx.h>
#include "User.h"
#include "../../UI/Elements/Map.h"

class CallHandler : public User
{
public:
    CallHandler(){};
    CallHandler(const string &username, const string &firstname, const string &lastname);

    void AcceptEmergency(wxCommandEvent &event, wxWindow &parent) const;
    void PrioritiseEmergency(wxCommandEvent &event, wxWindow &parent, Emergency emergency, int emergencyPriority);
    void RouteEmergency();

    string GetName() { return firstname + " " + lastname; };

    string firstname;
    string lastname;
    string username;
};

#endif