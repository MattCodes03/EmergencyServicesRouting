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

#ifndef USERINPUT_H
#define USERINPUT_H

#include <wx/wx.h>
#include "string"

class UserInput
{
public:
    // Default Constructor
    UserInput() = default;

    UserInput(wxWindow *parent, const wxString &label, long style = 0);

    // Function used in instances where input has been instantiated using default constructor.
    void initialize(wxWindow *parent, const wxString &label, long style = 0);

    wxString GetValue() const
    {
        return inputCtrl->GetValue();
    }

    wxBoxSizer *inputSizer = nullptr;

private:
    wxTextCtrl *inputCtrl = nullptr;
    wxStaticText *inputLabel;
};

#endif