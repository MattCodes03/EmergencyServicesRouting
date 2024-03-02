#pragma once
#include <wx/wx.h>
#include "User.h"

class CallHandler : User
{
public:
    CallHandler(const string &username, const string &firstname, const string &lastname);

    void AcceptEmergency(wxCommandEvent &event, wxWindow &parent) const;
    void PrioritiseEmergency();
    void SendMessage();

    string GetName() { return firstname + " " + lastname; };

    string firstname;
    string lastname;
    string username;
};