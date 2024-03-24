#include "Map.h"
#include <set>

void Map::OnPaint(wxPaintEvent &event)
{
    wxPaintDC paintDC(this); // Create a wxPaintDC object for this panel
    dc = &paintDC;

    // Load the PNG image
    wxBitmap pngBitmap("Assets/map.png", wxBITMAP_TYPE_PNG);

    if (!pngBitmap.IsOk())
    {
        wxLogError("Failed to load PNG image");
        return;
    }

    // Draw the PNG image on the DC
    dc->DrawBitmap(pngBitmap, 0, 0, true);

    SetupGraph();
    DrawGraph();
};

void Map::DrawNode(const Ambulance &nodeRef)
{
    int x = nodeRef.location.first;
    int y = nodeRef.location.second;

    if (nodeRef.available && dc)
    {
        dc->SetBrush(*wxBLUE_BRUSH);
        dc->SetPen(wxPen(wxColor(0, 0, 255), 2));
        dc->DrawCircle(x, y, 5);
    }
    else if (!nodeRef.available)
    {
        dc->SetBrush(*wxBLACK_BRUSH);
        dc->SetPen(wxPen(wxColor(128, 128, 128), 2));
        dc->DrawCircle(x, y, 5);
    }
}

void Map::DrawNode(const Emergency &nodeRef)
{
    if (dc)
    {
        int x = nodeRef.location.first;
        int y = nodeRef.location.second;
        dc->SetBrush(*wxRED_BRUSH);
        dc->SetPen(wxPen(wxColor(255, 0, 0), 2));
        dc->DrawCircle(x, y, 5);
    }
}

void Map::SetupGraph()
{
    unique_ptr<Database> database = make_unique<Database>();
    vector<Ambulance> ambulances;

    for (Ambulance ambulance : database->GetAmbulances())
    {
        if (ambulance.status)
        {
            graph.AddNode(Node(ambulance.unitNumber, any_cast<Ambulance>(ambulance)));
            ambulances.push_back(ambulance);
        }
    };

    for (Emergency emergency : database->GetEmergencies())
    {
        if (!emergency.complete && emergency.respondedTo)
        {
            graph.AddNode(Node(emergency.emergencyNumber, any_cast<Emergency>(emergency)));
            for (Ambulance ambulance : ambulances)
            {
                if (ambulance.available)
                {
                    graph.AddEdge(emergency.emergencyNumber, ambulance.unitNumber, graph.CalculateDistance(emergency.location, ambulance.location));
                }
            }
        }
    }
}

void Map::DrawGraph()
{

    for (auto &node : graph.GetNodes())
    {
        if (node.GetData().type() == typeid(Emergency))
        {
            DrawNode(any_cast<Emergency>(node.GetData()));
        }
        else if (node.GetData().type() == typeid(Ambulance))
        {
            DrawNode(any_cast<Ambulance>(node.GetData()));
        }
    };

    // Draw the edges
    for (const auto &edge : edges)
    {
        dc->SetPen(wxPen(wxColor(0, 0, 0), 3));
        dc->DrawLine(edge.first.first, edge.first.second, edge.second.first, edge.second.second);
    }
};

void Map::OnDatabaseChange()
{
    Refresh();
    SetupGraph();
};
