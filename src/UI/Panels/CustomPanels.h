#ifndef CUSTOMPANELS_H
#define CUSTOMPANELS_H

#include <wx/wx.h>
#include "../../MainFrame.h"
#include "../../Types/User/CallHandler.h"
#include "../../Types/User/EmergencyResponder.h"
#include "../../Types/User/HospitalAdmin.h"
#include "../../App.h"
#include <any>
#include <string>
#include "../Elements/Map.h"

class CustomPanels : public wxPanel
{
public:
    CustomPanels(wxWindow *parent) : wxPanel(parent){};

    ~CustomPanels() // Destructor to clean up resources
    {
        delete map;      // Delete the map object
        delete database; // Delete the database object
    }

    void EmergencyResponderPanel(wxWindow *parent);
    void CallHandlerPanel(wxWindow *parent);
    void HospitalAdminPanel(wxWindow *parent);

    void SetStopRoutingCallback(function<void()> callback);

    void InitialiseUser(const std::string &type, User &activeUser)
    {
        if (type == "HANDLER")
        {
            user = make_any<CallHandler>(activeUser.getUsername(), "Matthew", "McCann");
        }

        if (type == "RESPONDER")
        {
            user = make_any<EmergencyResponder>(activeUser.getUsername());
        }

        if (type == "HOPSITAL")
        {
            user = make_any<HospitalAdmin>(activeUser.getUsername());
        }
    }

    void Logout(wxCommandEvent &event)
    {
        // Stop the Emergency Routing Thread if user was logged in as Callhandler
        if (user.type() == typeid(CallHandler))
        {
            try
            {
                CallHandler &userRef = any_cast<CallHandler &>(user);
                userRef.StopRoutingThread();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }

        if (timer)
        {
            timer->Stop();
            delete timer;
        }

        App::GetInstance().Restart();
    }

    any user;

    Map &GetMap() { return *map; };

    Database &GetDatabase() { return *database; };

private:
    wxPanel *panel;
    Map *map;
    Database *database = new Database();
    wxTimer *timer = nullptr;

    function<void()> stopRoutingCallback;
};

#endif