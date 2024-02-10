#include "CustomPanels.h"
#include <any>

BEGIN_EVENT_TABLE(Map, wxPanel)
EVT_PAINT(Map::OnPaint)
END_EVENT_TABLE()

void CustomPanels::CallHandlerPanel(wxWindow *parent)
{

    auto userRef = std::any_cast<CallHandler>(user);

    wxFont titleFont(wxFontInfo(wxSize(0, 36)).Bold());
    wxFont mainFont(wxFontInfo(wxSize(0, 24)));

    // Creating a panel with explicit size and position
    panel = new wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));

    panel->SetFont(mainFont);

    // Adding a sizer to manage layout
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *text = new wxStaticText(panel, wxID_ANY, userRef.GetName());
    text->SetFont(mainFont);

    // Adding the text to the sizer
    sizer->Add(text, 0, wxALIGN_CENTER | wxALL, 10);

    Graph graph(10, 42);
    Map *map = new Map(panel, graph);
    sizer->Add(map, 1, wxEXPAND);

    // Setting the sizer for the panel
    panel->SetSizer(sizer);
    // Refreshing the layout
    panel->Layout();
}