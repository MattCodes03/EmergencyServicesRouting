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

void Map::SetupGraph()
{
    try
    {

        graph.ClearGraph();

        unique_ptr<Database> database = make_unique<Database>();

        if (mapType == "HANDLER")
        {
            vector<Ambulance> ambulances;

            for (Ambulance &ambulance : database->GetAmbulances())
            {
                if (ambulance.status)
                {
                    graph.AddNode(Node(ambulance.unitNumber, any_cast<Ambulance>(ambulance)));
                    ambulances.push_back(ambulance);
                }
            };

            for (Emergency &emergency : database->GetEmergencies())
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
                for (Hospital &hospital : database->GetHospitals())
                {
                    if (hospital.status)
                    {
                        graph.AddNode(Node(hospital.hospitalNumber, any_cast<Hospital>(hospital)));
                        graph.AddEdge(activeAmbulance.unitNumber, hospital.hospitalNumber, graph.CalculateDistance(hospital.location, activeAmbulance.location));
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

void Map::DrawGraph()
{

    QuickSortEdges(edges, 0, edges.size() - 1);

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

void Map::OnDatabaseChange()
{
    lock_guard<mutex> lock(graph.GetMutex());
    Refresh();
};
