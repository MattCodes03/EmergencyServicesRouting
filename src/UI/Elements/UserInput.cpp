#include "UserInput.h"

void UserInput::initialize(wxWindow *parent, const wxString &label, long style)
{
    inputSizer = new wxBoxSizer(wxHORIZONTAL);
    inputLabel = new wxStaticText(parent, wxID_ANY, label);

    inputLabel->SetMinSize(wxSize(60, inputLabel->GetMinSize().y));
    inputSizer->Add(inputLabel);

    inputCtrl = new wxTextCtrl(parent, wxID_ANY);
    inputCtrl->SetWindowStyle(style);
    inputSizer->Add(inputCtrl, 1);
};

UserInput::UserInput(wxWindow *parent, const wxString &label, long style)
{
    initialize(parent, label, style);
};