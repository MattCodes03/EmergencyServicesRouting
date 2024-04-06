/*
Copyright (c) 2024, Matthew McCann
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to no conditions.
*/

#include "CustomPanels.h"
#include <any>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>

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
               while (!terminateThread.load(std::memory_order_acquire))
               {
                   std::cout << "Thread Call!" << endl;

                   // Route emergencies Asynchronously
                   auto asyncFunc = [this, &userRef](wxWindow *p)
                   {
                       if (!terminateThread.load(memory_order_relaxed)) // Check termination flag before routing
                       {
                           userRef.RouteEmergency(*p);
                       }
                   };

                {
                std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before accessing shared data
                if (terminateThread.load(std::memory_order_relaxed)) // Re-check termination flag inside the critical section
                    break; // Exit loop if termination signal received
                }

                   std::async(launch::async, asyncFunc, parent).wait();

                   this_thread::sleep_for(chrono::seconds(5));
               }
               std::cout << "Routing thread terminated!\n"; })
        .detach();
}
