#include "CustomPanels.h"
#include <any>
#include <thread>
#include <chrono>

BEGIN_EVENT_TABLE(Map, wxPanel)
EVT_PAINT(Map::OnPaint)
END_EVENT_TABLE()

void CustomPanels::CallHandlerPanel(wxWindow *parent)
{

    auto userRef = std::any_cast<CallHandler>(user);

    wxFont titleFont(wxFontInfo(wxSize(0, 36)).Bold());
    wxFont mainFont(wxFontInfo(wxSize(0, 24)));

    // Creating a panel with explicit size and position
    panel = new wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));

    panel->SetFont(mainFont);

    // Adding a sizer to manage layout
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *text = new wxStaticText(panel, wxID_ANY, userRef.GetName());
    text->SetFont(mainFont);

    wxButton *logoutButton = new wxButton(panel, wxID_ANY, _("Logout"));
    logoutButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, &userRef](wxCommandEvent &event)
                       { userRef.StopCheckAndRouteLoop();
                             Logout(event); });

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

    map = new Map(panel, this);
    database->AddListener(map);
    map->SetMapType("HANDLER");
    sizer->Add(map, 1, wxEXPAND);

    // Setting the sizer for the panel
    panel->SetSizer(sizer);
    // Refreshing the layout
    panel->Layout();

    /*
   Route Emergency Thread, this will create another thread that will periodically check if any emergencies are in the queue to be routed,
   this means all routing is done automatically
   */

    std::thread routingThread([userRef, parent]()
                              { userRef.CheckAndRouteLoop(*parent); });
    routingThread.detach();
}
