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

#include <SQLiteCpp/SQLiteCpp.h>
#include "CustomPanels.h"
#include "../Dialogs/HospitalSelectDialog.h"

void CustomPanels::HospitalAdminPanel(wxWindow *parent)
{
    auto &userRef = std::any_cast<HospitalAdmin &>(user);

    MainFrame *parentFrame = dynamic_cast<MainFrame *>(parent);
    if (parentFrame)
    {

        HospitalSelectDialog hospitalSelect(parentFrame, wxID_ANY, _("Hospital Select"));
        hospitalSelect.Center();

        while (userRef.hospitalNumber <= 0)
        {
            if (hospitalSelect.ShowModal() == wxID_OK)
            {
                SQLite::Statement query(*parentFrame->customPanels->GetDatabase().database, "SELECT * FROM hospital WHERE hospitalNumber = ?");
                query.bind(1, hospitalSelect.getHopsitalNumber());

                bool login = false;

                while (query.executeStep())
                {

                    if ((int)query.getColumn(0) == hospitalSelect.getHopsitalNumber())
                    {
                        if ((int)query.getColumn(4) == hospitalSelect.getHospitalPin())
                        {
                            userRef.hospitalNumber = hospitalSelect.getHopsitalNumber();
                            std::pair<int, int> location = parentFrame->customPanels->GetDatabase().ConvertLocation((string)query.getColumn(2));
                            bool status = query.getColumn(3).getInt() != 0;

                            userRef.activeHospital = Hospital((int)query.getColumn(0), (string)query.getColumn(1), location, status);

                            login = true;
                        }
                    }
                }

                if (!login)
                {
                    wxMessageBox("Hospital Login Details Incorrect!", "Hospital Selection Failed", wxOK | wxICON_ERROR, parentFrame);
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

        wxStaticText *text = new wxStaticText(panel, wxID_ANY, userRef.activeHospital.name);
        text->SetFont(mainFont);

        wxButton *logoutButton = new wxButton(panel, wxID_ANY, _("Logout"));
        logoutButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, &userRef](wxCommandEvent &event)
                           { Logout(event); });

        sizer->Add(logoutButton, 0, wxALIGN_RIGHT, 10);

        // Adding the text to the sizer
        sizer->Add(text, 0, wxALIGN_CENTER | wxALL, 10);

        bool acceptingPatients = (userRef.activeHospital.status == 1);

        // Determine the status text based on the boolean value
        string statusText = acceptingPatients ? "Accepting Patients" : "Not Accepting Patients";

        // Create and display the wxStaticText with the hospital name and status text
        wxString labelText = wxString::Format("Status: %s", statusText);
        wxStaticText *statusTextObject = new wxStaticText(panel, wxID_ANY, labelText);
        text->SetFont(mainFont);
        sizer->Add(statusTextObject, 0, wxALIGN_CENTER | wxALL, 10);

        wxButton *changeStatusButton = new wxButton(panel, wxID_ANY, _("Update Hospital Status"));
        changeStatusButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, &userRef, parent](wxCommandEvent &event)
                                 { userRef.UpdateHospitalStatus(*parent);
                                  panel->Layout(); });

        sizer->Add(changeStatusButton, 0, wxALIGN_CENTER | wxALL, 10);

        // Setting the sizer for the panel
        panel->SetSizer(sizer);

        // Refreshing the layout
        panel->Layout();
    }
}
