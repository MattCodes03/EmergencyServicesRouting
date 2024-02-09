#pragma once
#include <wx/wx.h>
#include <any>
#include <string>
#include "../../Types/CallHandler.h"
#include "../../Types/EmergencyResponder.h"
#include "../../Types/HospitalAdmin.h"
#include "../Elements/Map.h"

class CustomPanels
{
public:
    CustomPanels(){};

    void EmergencyResponderPanel(wxWindow *parent);
    void CallHandlerPanel(wxWindow *parent);
    void HospitalAdminPanel(wxWindow *parent);

    void InitialiseUser(const std::string &type, User &activeUser)
    {
        if (type == "HANDLER")
        {
            user = make_any<CallHandler>(activeUser.getUsername(), "Matthew", "McCann");
        }

        if (type == "RESPONDER")
        {
            user = make_any<EmergencyResponder>(activeUser.getUsername(), 12);
        }

        if (type == "HOPSITAL")
        {
            user = std::make_any<HospitalAdmin>(activeUser.getUsername());
        }
    }

    any user;

private:
    wxPanel *panel;
};
