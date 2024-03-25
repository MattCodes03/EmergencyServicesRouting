#include "CustomPanels.h"
#include "../Dialogs/AmbulanceSelectDialog.h"

void CustomPanels::EmergencyResponderPanel(wxWindow *parent)
{
    EmergencyResponder &userRef = std::any_cast<EmergencyResponder &>(user);

    MainFrame *parentFrame = dynamic_cast<MainFrame *>(parent);
    if (parentFrame)
    {

        AmbulanceSelectDialog ambulanceSelect(parentFrame, wxID_ANY, _("Unit Assignment"));
        ambulanceSelect.Center();

        // Get Details from Database
        vector<Ambulance> ambulances = parentFrame->customPanels->GetDatabase().GetAmbulances();

        while (userRef.unitNumber <= 0)
        {
            if (ambulanceSelect.ShowModal() == wxID_OK)
            {
                for (const Ambulance &ambulance : ambulances)
                {
                    if (ambulance.unitNumber == ambulanceSelect.getUnitNumber())
                    {
                        userRef.unitNumber = ambulanceSelect.getUnitNumber();
                        break;
                    }
                }
            }
            else
            {
                wxMessageBox("Not a valid ambulance number!", "Unit Assignment Failed", wxOK | wxICON_ERROR, parentFrame);
            }
        }

        wxFont titleFont(wxFontInfo(wxSize(0, 36)).Bold());
        wxFont mainFont(wxFontInfo(wxSize(0, 24)));

        // Creating a panel with explicit size and position
        panel = new wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));

        panel->SetFont(mainFont);

        // Adding a sizer to manage layout
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText *text = new wxStaticText(panel, wxID_ANY, "Emergency Responder View");
        text->SetFont(mainFont);

        // Adding the text to the sizer
        sizer->Add(text, 0, wxALIGN_CENTER | wxALL, 10);

        wxButton *logoutButton = new wxButton(panel, wxID_ANY, _("Logout"));
        logoutButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, &userRef](wxCommandEvent &event)
                           { Logout(event); });

        map = new Map(panel, this);
        database->AddListener(map);
        map->SetMapType("AMBULANCE");
        sizer->Add(map, 1, wxEXPAND);

        // Setting the sizer for the panel
        panel->SetSizer(sizer);

        // Refreshing the layout
        panel->Layout();
    }
}