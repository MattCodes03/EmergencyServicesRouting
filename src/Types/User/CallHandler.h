#ifndef CALLHANDLER_H
#define CALLHANDLER_H

#include <wx/wx.h>
#include "User.h"
#include "../../UI/Elements/Map.h"
#include <atomic>
#include <thread>

class CallHandler : public User
{
public:
    CallHandler() {}
    CallHandler(const string &username, const string &firstname, const string &lastname) : username(username), firstname(firstname), lastname(lastname), keepRouting(true){};

    // Copy constructor
    CallHandler(const CallHandler &other)
        : username(other.username), firstname(other.firstname), lastname(other.lastname), keepRouting(other.keepRouting.load()) {}

    // Copy assignment operator
    CallHandler &operator=(const CallHandler &other)
    {
        if (this != &other)
        {
            username = other.username;
            firstname = other.firstname;
            lastname = other.lastname;
            keepRouting.store(other.keepRouting.load());
        }
        return *this;
    }

    // Destructor
    ~CallHandler()
    {
        // Stop the routing thread if it's running
        StopRoutingThread();
    }

    void AcceptEmergency(wxCommandEvent &event, wxWindow &parent) const;
    void PrioritiseEmergency(wxCommandEvent &event, wxWindow &parent, Emergency emergency, int emergencyPriority);
    void RouteEmergency(wxWindow &parent) const;

    // Function to stop the routing thread
    void StopRoutingThread()
    {
        // Set keepRouting to false
        keepRouting.store(false);
    }

    string GetName()
    {
        return firstname + " " + lastname;
    };

    atomic<bool> keepRouting;

private:
    string firstname;
    string lastname;
    string username;
};

#endif