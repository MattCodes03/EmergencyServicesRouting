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

#include "UserInput.h"

void UserInput::initialize(wxWindow *parent, const wxString &label, long style)
{
    inputSizer = new wxBoxSizer(wxHORIZONTAL);
    inputLabel = new wxStaticText(parent, wxID_ANY, label);

    inputLabel->SetMinSize(wxSize(80, inputLabel->GetMinSize().y));
    inputSizer->Add(inputLabel, 0);

    inputSizer->AddSpacer(10);

    inputCtrl = new wxTextCtrl(parent, wxID_ANY);
    inputCtrl->SetWindowStyle(style);
    inputSizer->Add(inputCtrl, 5);
};

UserInput::UserInput(wxWindow *parent, const wxString &label, long style)
{
    initialize(parent, label, style);
};