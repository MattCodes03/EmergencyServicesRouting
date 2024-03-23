#include "Map.h"
#include <set>

void Map::OnPaint(wxPaintEvent &event)
{
    wxPaintDC paintDC(this);
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
    DrawGraph(*dc);
};

void Map::DrawNode(wxDC &dc, const Ambulance &nodeRef)
{
    int x = nodeRef.location.first;
    int y = nodeRef.location.second;

    if (nodeRef.available)
    {
        dc.SetBrush(*wxBLUE_BRUSH);
        dc.SetPen(wxPen(wxColor(0, 0, 255), 2));
        dc.DrawCircle(x, y, 5);
    }
}

void Map::DrawNode(wxDC &dc, const Emergency &nodeRef)
{
    int x = nodeRef.location.first;
    int y = nodeRef.location.second;
    dc.SetBrush(*wxRED_BRUSH);
    dc.SetPen(wxPen(wxColor(255, 0, 0), 2));
    dc.DrawCircle(x, y, 5);
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

    set<pair<int, int>> visitedEdges; // Maintain a set to store visited edges

    for (Emergency emergency : database->GetEmergencies())
    {
        if (!emergency.complete && emergency.respondedTo)
        {
            graph.AddNode(Node(emergency.emergencyNumber, any_cast<Emergency>(emergency)));
            for (Ambulance ambulance : ambulances)
            {
                if (ambulance.available && visitedEdges.find({emergency.emergencyNumber, ambulance.unitNumber}) == visitedEdges.end())
                {
                    // Edge does not exist, add it to the graph
                    graph.AddEdge(emergency.emergencyNumber, ambulance.unitNumber, graph.CalculateDistance(emergency.location, ambulance.location));
                    // Add the edge to the visited set
                    visitedEdges.insert({emergency.emergencyNumber, ambulance.unitNumber});
                }
            }
        }
    }

    graph.Display();
}

void Map::DrawEdge(wxDC &dc, pair<int, int> source, pair<int, int> destination)
{
    // Set pen color and width for the edge
    dc.SetPen(wxPen(wxColor(0, 0, 0), 5));

    // Draw line between source and destination points
    dc.DrawLine(source.first, source.second, destination.first, destination.second);
}

void Map::DrawGraph(wxDC &dc)
{

    for (auto &node : graph.GetNodes())
    {
        if (node.GetData().type() == typeid(Emergency))
        {
            DrawNode(dc, any_cast<Emergency>(node.GetData()));
        }
        else if (node.GetData().type() == typeid(Ambulance))
        {
            DrawNode(dc, any_cast<Ambulance>(node.GetData()));
        }
    };
};

void Map::OnDatabaseChange()
{
    Refresh();
};
