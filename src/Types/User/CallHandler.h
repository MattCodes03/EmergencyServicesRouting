#ifndef CALLHANDLER_H
#define CALLHANDLER_H

#include <wx/wx.h>
#include "User.h"
#include "../../UI/Elements/Map.h"

class CallHandler : public User
{
public:
    CallHandler(){};
    CallHandler(const string &username, const string &firstname, const string &lastname) : username(username), firstname(firstname), lastname(lastname), keepRunning(true){};

    void AcceptEmergency(wxCommandEvent &event, wxWindow &parent) const;
    void PrioritiseEmergency(wxCommandEvent &event, wxWindow &parent, Emergency emergency, int emergencyPriority);
    void RouteEmergency(wxWindow &parent) const;
    void CheckAndRouteEmergency(wxWindow &parent);
    void CheckAndRouteLoop(wxWindow &parent);

    void StopCheckAndRouteLoop()
    {
        keepRunning = false; // Set the flag to false to stop the loop
    }

    string GetName()
    {
        return firstname + " " + lastname;
    };

    string firstname;
    string lastname;
    string username;

    bool keepRunning;
};

#endif