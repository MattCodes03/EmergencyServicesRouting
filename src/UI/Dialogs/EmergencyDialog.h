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

#ifndef EMERGENCYDIALOG_H
#define EMERGENCYDIALOG_H

#include <wx/wx.h>
#include "../../Types/User/CallHandler.h"
#include "../../Types/Nodes/Emergency.h"

class EmergencyDialog : public wxDialog
{
public:
    EmergencyDialog(wxWindow *parent, wxWindowID id,
                    const wxString &title,
                    const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize,
                    long style = wxDEFAULT_DIALOG_STYLE,
                    const wxString &name = wxASCII_STR(wxDialogNameStr));

    virtual ~EmergencyDialog();

    void SetDescription(wxString description)
    {
        this->emergencyDescription = description;
        if (this->description)
        {
            this->description->SetValue(emergencyDescription);
        }
    };
    void SetEmergency(const Emergency &emergency) { this->emergency = emergency; };

    void SetUserRef(const CallHandler &user) { this->userRef = user; }

    Emergency emergency;

private:
    CallHandler userRef;
    wxString emergencyDescription = "DEFAULT DESCRIPTION";

    wxTextCtrl *description;

    wxButton *lowSevereButton;
    wxButton *medSevereButton;
    wxButton *highSevereButton;

    DECLARE_EVENT_TABLE()
};

#endif