#ifndef CUSTOMPANELS_H
#define CUSTOMPANELS_H

#include <wx/wx.h>
#include <any>
#include <string>
#include "../../Types/User/CallHandler.h"
#include "../../Types/User/EmergencyResponder.h"
#include "../../Types/User/HospitalAdmin.h"
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
            user = make_any<HospitalAdmin>(activeUser.getUsername());
        }
    }

    any user;

    const Map &GetMap() const { return this->map; };

private:
    wxPanel *panel;
    Map *map;
};

#endif