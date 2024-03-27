#include "CustomPanels.h"
#include <any>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>

BEGIN_EVENT_TABLE(Map, wxPanel)
EVT_PAINT(Map::OnPaint)
END_EVENT_TABLE()

void CustomPanels::SetStopRoutingCallback(std::function<void()> callback)
{
    stopRoutingCallback = callback;
}

void CustomPanels::CallHandlerPanel(wxWindow *parent)
{

    auto userRef = any_cast<CallHandler>(user);

    wxFont titleFont(wxFontInfo(wxSize(0, 36)).Bold());
    wxFont mainFont(wxFontInfo(wxSize(0, 24)));

    // Creating a panel with explicit size and position
    panel = new wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));

    panel->SetFont(mainFont);

    map = new Map(panel, this);
    database->AddListener(map);
    map->SetMapType("HANDLER");

    // Adding a sizer to manage layout
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *text = new wxStaticText(panel, wxID_ANY, userRef.GetName());
    text->SetFont(mainFont);

    wxButton *logoutButton = new wxButton(panel, wxID_ANY, _("Logout"));
    logoutButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, &userRef](wxCommandEvent &event)
                       { Logout(event); });

    sizer->Add(logoutButton, 0, wxALIGN_RIGHT, 10);
    // Adding the text to the sizer
    sizer->Add(text, 0, wxALIGN_CENTER | wxALL, 10);

    // Accept Emergency Button
    wxButton *acceptEmergencyButton = new wxButton(panel, wxID_ANY, _("Accept Incoming Emergency"));
    acceptEmergencyButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, userRef, parent, acceptEmergencyButton](wxCommandEvent &event) { // Disable the button
        acceptEmergencyButton->Disable();

        userRef.AcceptEmergency(event, *parent);

        if (timer)
        {
            timer->Stop();
            delete timer;
        }

        // Use a timer to temporarily disable the button, this is done to prevent users overloading the system by spamming emergencies
        timer = new wxTimer();
        timer->Bind(wxEVT_TIMER, [acceptEmergencyButton](wxTimerEvent &event)
                    { acceptEmergencyButton->Enable(); });
        timer->StartOnce(5000); // 5 seconds

    });

    sizer->Add(acceptEmergencyButton, 0, wxALIGN_CENTER | wxALL, 10);
    sizer->Add(map, 1, wxEXPAND);

    // Setting the sizer for the panel
    panel->SetSizer(sizer);
    // Refreshing the layout
    panel->Layout();

    // Route Emergency Thread
    thread([this, userRef, parent]()
           {
               while (!terminateThread.load())
               {
                   cout << "Thread Call!" << endl;

                   // Route emergencies Asynchronously
                   auto asyncFunc = [this, &userRef](wxWindow *p)
                   {
                       if (!terminateThread.load()) // Check termination flag before routing
                       {
                           userRef.RouteEmergency(*p);
                       }
                   };

                   async(launch::async, asyncFunc, parent).wait();

                   this_thread::sleep_for(chrono::seconds(5));
               }
               cout << "Routing thread terminated!\n"; })
        .detach();
}
