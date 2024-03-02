#pragma once
#include <wx/wx.h>

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
            this->description->SetLabel(emergencyDescription);
        }
    };
    void SetEmergencyID(int id) { this->emergencyID = id; };

private:
    int emergencyID;
    wxString emergencyDescription = "DEFAULT DESCRIPTION";

    wxStaticText *description;

    wxButton *lowSevereButton;
    wxButton *medSevereButton;
    wxButton *highSevereButton;

    DECLARE_EVENT_TABLE()
};