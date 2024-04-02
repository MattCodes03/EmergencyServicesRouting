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

    void SetStopThreadCallback(std::function<void()> callback)
    {
        stopThreadCallback = callback;
    }

    void InitialiseUser(const std::string &type, User &activeUser)
    {
        if (type == "HANDLER")
        {
            user = make_any<CallHandler>(activeUser.getUsername(), "Matthew", "McCann");
            // Set the stop routing callback
            SetStopThreadCallback([this]()
                                  { StopRoutingThread(); });
        }

        if (type == "RESPONDER")
        {
            user = make_any<EmergencyResponder>(activeUser.getUsername());
            SetStopThreadCallback([this]()
                                  { StopRoutingThread(); });
        }

        if (type == "HOSPITAL")
        {
            user = make_any<HospitalAdmin>(activeUser.getUsername());
        }
    }

    // Function to stop the routing thread
    void StopRoutingThread()
    {
        {
            lock_guard<std::mutex> lock(mtx);                       // Lock the mutex before modifying shared data
            terminateThread.store(true, std::memory_order_release); // Set the termination flag to true
        }

        cv.notify_all();
    }

    void Logout(wxCommandEvent &event)
    {
        // Stop the Emergency Routing Thread if user was logged in as Callhandler
        if (stopThreadCallback)
        {
            // Call the callback to stop the routing thread
            stopThreadCallback();
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

    function<void()> stopThreadCallback;
};

#endif