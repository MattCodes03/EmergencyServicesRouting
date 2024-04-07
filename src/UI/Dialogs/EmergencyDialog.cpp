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

#include "EmergencyDialog.h"
#include "../../MainFrame.h"

BEGIN_EVENT_TABLE(EmergencyDialog, wxDialog)
END_EVENT_TABLE()

EmergencyDialog::EmergencyDialog(wxWindow *parent, wxWindowID id,
                                 const wxString &title,
                                 const wxPoint &pos,
                                 const wxSize &size,
                                 long style,
                                 const wxString &name) : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    description = new wxTextCtrl(this, wxID_ANY, emergencyDescription, wxDefaultPosition, wxSize(500, 300), wxTE_MULTILINE | wxTE_READONLY);
    mainSizer->Add(description, 0, wxALIGN_CENTER);

    wxStaticText *buttonLabel = new wxStaticText(this, wxID_ANY, "Severity");
    mainSizer->Add(buttonLabel, 0, wxALIGN_CENTER);

    lowSevereButton = new wxButton(this, wxID_ANY, "Low");
    lowSevereButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, parent](wxCommandEvent &event)
                          { userRef.PrioritiseEmergency(event, *parent, emergency, 0); Close(); });

    medSevereButton = new wxButton(this, wxID_ANY, "Medium");
    medSevereButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, parent](wxCommandEvent &event)
                          { userRef.PrioritiseEmergency(event, *parent, emergency, 1); Close(); });

    highSevereButton = new wxButton(this, wxID_ANY, "High");
    highSevereButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, parent](wxCommandEvent &event)
                           { userRef.PrioritiseEmergency(event, *parent, emergency, 2); Close(); });

    // Create a horizontal box sizer to arrange the buttons in a row
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(lowSevereButton, 0, wxALL, 5);
    buttonSizer->Add(medSevereButton, 0, wxALL, 5);
    buttonSizer->Add(highSevereButton, 0, wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 100));
    Fit();
}

EmergencyDialog::~EmergencyDialog()
{
}
