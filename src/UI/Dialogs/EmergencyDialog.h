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
            this->description->SetLabel(emergencyDescription);
        }
    };
    void SetEmergency(const Emergency &Emergency) { this->emergency = emergency; };

    void SetUserRef(const CallHandler &user) { this->userRef = user; }

private:
    Emergency emergency;
    CallHandler userRef;
    wxString emergencyDescription = "DEFAULT DESCRIPTION";

    wxStaticText *description;

    wxButton *lowSevereButton;
    wxButton *medSevereButton;
    wxButton *highSevereButton;

    DECLARE_EVENT_TABLE()
};

#endif