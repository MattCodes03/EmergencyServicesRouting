/*
Copyright (c) 2024, Matthew McCann
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to no conditions.
*/

#include <SQLiteCpp/SQLiteCpp.h>
#include "Map.h"
#include "../../MainFrame.h"
#include <any>

void Map::OnPaint(wxPaintEvent &event)
{
    wxPaintDC paintDC(this); // Create a wxPaintDC object for this panel
    dc = &paintDC;
    if (dc)
    {

        DrawBackground();

        SetupGraph();
        DrawGraph();
    }
};

// Draw the background image
void Map::DrawBackground()
{
    wxPaintDC paintDC(this); // Create a wxPaintDC object for this panel
    wxDC *dc = &paintDC;
    if (dc)
    {
        // Load the PNG image
        wxBitmap pngBitmap("Assets/map.png", wxBITMAP_TYPE_PNG);

        if (!pngBitmap.IsOk())
        {
            wxLogError("Failed to load PNG image");
            return;
        }

        // Draw the PNG image on the DC
        dc->DrawBitmap(pngBitmap, 0, 0, true);
    }
}

template <typename NodeType>
void Map::DrawNode(const NodeType &nodeRef)
{
    if (dc)
    {
        int x = nodeRef.location.first;
        int y = nodeRef.location.second;

        wxBrush brush;
        wxPen pen;

        // Make use of conditional constant expressions to perform compile-time type checking of template parameters
        if constexpr (std::is_same_v<NodeType, Ambulance>)
        {
            brush = nodeRef.available ? *wxBLUE_BRUSH : *wxBLACK_BRUSH;
            pen = nodeRef.available ? wxPen(wxColor(0, 0, 255), 2) : wxPen(wxColor(128, 128, 128), 2);
            x += 5;
        }
        else if constexpr (std::is_same_v<NodeType, Emergency>)
        {
            brush = *wxRED_BRUSH;
            pen = wxPen(wxColor(255, 0, 0), 2);
        }
        else if constexpr (std::is_same_v<NodeType, Hospital>)
        {
            brush = *wxGREEN_BRUSH;
            pen = wxPen(wxColor(0, 255, 0), 2);
        }

        dc->SetBrush(brush);
        dc->SetPen(pen);
        dc->DrawCircle(x, y, 5);
    }
}

// Function will setup the graphs nodes and edges based on values obtained from the database
void Map::SetupGraph()
{
    try
    {
        // Clear the grpah in-case any junk values are left behind
        graph.ClearGraph();

        // Create smart pointer to database, this means I don't need to worry about memory clean up
        unique_ptr<Database> database = make_unique<Database>();

        // If Map Type is Handler display all ambulances and emergencies that have been accpetd and prioritised
        if (mapType == "HANDLER")
        {
            std::vector<Ambulance> ambulances;

            for (Ambulance &ambulance : database->GetAmbulances())
            {
                if (ambulance.status)
                {
                    graph.AddNode(Node(ambulance.unitNumber, any_cast<Ambulance>(ambulance)));
                    ambulances.push_back(ambulance);
                }
            };
            using namespace std;
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

// If Map Type is Ambulance display only current users Ambulance, current emergency and all hospitals
if (mapType == "AMBULANCE")
{
    CustomPanels *parentFrame = dynamic_cast<CustomPanels *>(parentPanel);
    if (parentFrame)
    {
        // Get the active ambulance and add it to the graph
        Ambulance activeAmbulance;

        for (Ambulance &ambulance : database->GetAmbulances())
        {
            if (ambulance.unitNumber == any_cast<EmergencyResponder>(parentFrame->user).unitNumber)
            {
                graph.AddNode(Node(ambulance.unitNumber, any_cast<Ambulance>(ambulance)));
                activeAmbulance = ambulance;
                break;
            }
        };

        // Add all available hospitals to the graph and create an edge between each and the user's active ambulance
                for (using namespace std;
    }
}

// Plot the ambulance's current emergency
for (Emergency &emergency : database->GetEmergencies())
{
    if (!emergency.complete && emergency.emergencyNumber == activeAmbulance.activeEmergency)
    {
        graph.AddNode(Node(emergency.emergencyNumber, any_cast<Emergency>(emergency)));

        graph.AddEdge(emergency.emergencyNumber, activeAmbulance.unitNumber, graph.CalculateDistance(activeAmbulance.location, emergency.location));
        AddEdge(emergency.location, activeAmbulance.location);
    }
}
}
}
}
catch (const std::exception &e)
{
    std::cerr << "Error setting up graph: " << e.what() << '\n';
}
}

// Function will first QuickSort the edges in the graph to optmise rendering process and will then draw all nodes and edges onto the map
void Map::DrawGraph()
{

    // Sort Edges based on X-Coord
    QuickSortEdges(edges, 0, edges.size() - 1);

    // Draw all Nodes
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
        else if (node.GetData().type() == typeid(Hospital))
        {
            DrawNode(any_cast<Hospital>(node.GetData()));
        }
    };

    // Draw the edges
    for (const auto &edge : edges)
    {
        dc->SetPen(wxPen(wxColor(0, 0, 0), 3));
        dc->DrawLine(edge.first.first, edge.first.second, edge.second.first, edge.second.second);
    }
};

// This is override method from DatabaseListener Class
void Map::OnDatabaseChange()
{
    // Mutex lock for thread safety
    std::lock_guard<std::mutex> lock(graph.GetMutex());
    Refresh();
};
