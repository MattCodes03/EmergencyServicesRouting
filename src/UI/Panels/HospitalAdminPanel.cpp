#include "CustomPanels.h"

void CustomPanels::HospitalAdminPanel(wxWindow *parent)
{
    wxFont titleFont(wxFontInfo(wxSize(0, 36)).Bold());
    wxFont mainFont(wxFontInfo(wxSize(0, 24)));

    // Creating a panel with explicit size and position
    panel = new wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));

    panel->SetFont(mainFont);

    // Adding a sizer to manage layout
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *text = new wxStaticText(panel, wxID_ANY, "Hospital Admin View");
    text->SetFont(mainFont);

    // Adding the text to the sizer
    sizer->Add(text, 0, wxALIGN_CENTER | wxALL, 10);

    // Setting the sizer for the panel
    panel->SetSizer(sizer);

    // Refreshing the layout
    panel->Layout();
}