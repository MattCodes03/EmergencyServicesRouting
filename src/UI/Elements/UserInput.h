#pragma once
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