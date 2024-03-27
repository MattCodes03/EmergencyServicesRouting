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
#include <mutex>
#include <condition_variable>

class CustomPanels : public wxPanel
{
public:
    CustomPanels(wxWindow *parent) : wxPanel(parent), terminateThread(false){};

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
            // Set the stop routing callback
            SetStopRoutingCallback([this]()
                                   { StopRoutingThread(); });
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

    // Function to stop the routing thread
    void StopRoutingThread()
    {
        // Set keepRouting to false
        terminateThread.store(true);
    }

    void Logout(wxCommandEvent &event)
    {
        // Stop the Emergency Routing Thread if user was logged in as Callhandler
        if (stopRoutingCallback)
        {
            // Call the callback to stop the routing thread
            stopRoutingCallback();
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

    // Declare a mutex and condition variable for synchronization
    mutex mtx;
    condition_variable cv;

    // Initialize a termination flag for the thread
    std::atomic<bool> terminateThread;

    function<void()> stopRoutingCallback;
};

#endif