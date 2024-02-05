#pragma once
#include <wx/wx.h>

class CustomPanels
{
public:
    CustomPanels(){};

    void EmergencyResponderPanel(wxWindow *parent);
    void CallHandlerPanel(wxWindow *parent);
    void HospitalAdminPanel(wxWindow *parent);

private:
    wxPanel *panel;
};