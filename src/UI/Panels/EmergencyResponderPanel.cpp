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
#include "../Dialogs/AmbulanceSelectDialog.h"
#include <chrono>
#include <future>
#include <atomic>

void CustomPanels::EmergencyResponderPanel(wxWindow *parent)
{
    auto &userRef = std::any_cast<EmergencyResponder &>(user);

    MainFrame *parentFrame = dynamic_cast<MainFrame *>(parent);
    if (parentFrame)
    {

        AmbulanceSelectDialog ambulanceSelect(parentFrame, wxID_ANY, _("Unit Assignment"));
        ambulanceSelect.Center();

        // Get Details from Database
        std::vector<Ambulance> ambulances = parentFrame->customPanels->GetDatabase().GetAmbulances();

        while (userRef.unitNumber <= 0)
        {
            if (ambulanceSelect.ShowModal() == wxID_OK)
            {
                bool validUnit = false;
                for (const Ambulance &ambulance : ambulances)
                {
                    if (ambulance.unitNumber == ambulanceSelect.getUnitNumber())
                    {
                        userRef.unitNumber = ambulanceSelect.getUnitNumber();
                        validUnit = true;
                        break;
                    }
                }

                if (!validUnit)
                {
                    wxMessageBox("Not a valid ambulance number!", "Unit Assignment Failed", wxOK | wxICON_ERROR, parentFrame);
                }
            }
        }

        wxFont titleFont(wxFontInfo(wxSize(0, 36)).Bold());
        wxFont mainFont(wxFontInfo(wxSize(0, 24)));

        // Creating a panel with explicit size and position
        panel = new wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));

        panel->SetFont(mainFont);

        // Adding a sizer to manage layout
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        wxString unitText = wxString::Format("Unit #%d", userRef.unitNumber);
        wxStaticText *text = new wxStaticText(panel, wxID_ANY, unitText);
        text->SetFont(mainFont);

        wxButton *logoutButton = new wxButton(panel, wxID_ANY, _("Logout"));
        logoutButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, &userRef](wxCommandEvent &event)
                           { Logout(event); });

        sizer->Add(logoutButton, 0, wxALIGN_RIGHT, 10);

        // Adding the text to the sizer
        sizer->Add(text, 0, wxALIGN_CENTER | wxALL, 10);

        // Arrive at Emergency Button
        wxButton *arriveButton = new wxButton(panel, wxID_ANY, _("Arrived at Emergency"));
        arriveButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, &userRef, parent](wxCommandEvent &event)
                           { userRef.ArriveAtEmergency(*parent); });

        // Generate Hospital Route Button
        wxButton *generateRouteButton = new wxButton(panel, wxID_ANY, _("Generate Hospital Route"));
        generateRouteButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, &userRef, parent](wxCommandEvent &event)
                                  { userRef.GenerateHospitalRoute(*parent); });

        // Mark Emergency as Complete Button
        wxButton *completeEmergencyButton = new wxButton(panel, wxID_ANY, _("Complete Emergency"));
        completeEmergencyButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, &userRef, parent](wxCommandEvent &event)
                                      { userRef.CompleteEmergency(*parent); });

        sizer->Add(arriveButton, 0, wxALIGN_CENTER | wxALL, 10);
        sizer->Add(generateRouteButton, 0, wxALIGN_CENTER | wxALL, 10);
        sizer->Add(completeEmergencyButton, 0, wxALIGN_CENTER | wxALL, 10);

        map = new Map(panel, this);
        database->AddListener(map);
        map->SetMapType("AMBULANCE");
        sizer->Add(map, 1, wxEXPAND);

        // Setting the sizer for the panel
        panel->SetSizer(sizer);

        // Refreshing the layout
        panel->Layout();

        // Check for current emergency thread
        thread([this, &userRef, parent]()
               {
               while (!terminateThread.load(std::memory_order_acquire))
               {
                   std::cout << "Thread Call!" << endl;

                   // Route emergencies Asynchronously
                   auto asyncFunc = [this, &userRef](wxWindow *p)
                   {
                       if (!terminateThread.load(memory_order_relaxed)) // Check termination flag before routing
                       {
                           userRef.CheckForCurrentEmergency(*p);
                       }
                   };

                {
                std::lock_guard<std::mutex> lock(mtx); // Lock the mutex before accessing shared data
                if (terminateThread.load(std::memory_order_relaxed)) // Re-check termination flag inside the critical section
                    break; // Exit loop if termination signal received
                }

                   std::async(launch::async, asyncFunc, parent).wait();

                   std::this_thread::sleep_for(chrono::seconds(5));
               }
               std::cout << "Thread terminated!\n"; })
            .detach();
    }
}