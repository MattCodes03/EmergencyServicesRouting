#include "EmergencyResponder.h"
#include "../../MainFrame.h"

EmergencyResponder::EmergencyResponder(const string &username) : User(username), unitNumber(0)
{
}

void EmergencyResponder::GenerateHospitalRoute(wxWindow &parent)
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        if (parentFrame->customPanels)
        {
            if (!currentEmergency <= 0)
            {
                if (arrivedAtEmergency)
                {
                    if (!hospitalRoute)
                    {

                        // Get the source ambulance information
                        SQLite::Statement query(*parentFrame->customPanels->GetDatabase().database, "SELECT * FROM ambulance WHERE unitNumber = ?");
                        query.bind(1, unitNumber);

                        Graph &graph = parentFrame->customPanels->GetMap().GetGraph();

                        int source = unitNumber;
                        pair<int, int> edge = graph.Dijkstra(source);

                        if (edge.second != -1)
                        {

                            auto sourceNode = graph.GetNodes()[edge.first];
                            auto destinationNode = graph.GetNodes()[edge.second];

                            // Get nodes
                            Ambulance source = any_cast<Ambulance>(sourceNode.GetData());
                            Hospital destination = any_cast<Hospital>(destinationNode.GetData());

                            // Draw the edge on the map between emergency and assigned ambulance
                            parentFrame->customPanels->GetMap().AddEdge(source.location, destination.location);

                            hospitalRoute = true;
                            hospitalRouteID = destination.hospitalNumber;

                            parentFrame->customPanels->GetMap().OnDatabaseChange();
                        }
                        else
                        {
                            cout << "No Destination";
                        }
                    }
                    else
                    {
                        wxMessageBox("Cannot generate hospital route as one already exists!", "Cannot Generate Route", wxOK | wxICON_ERROR, parentFrame);
                    }
                }
                else
                {
                    wxMessageBox("Cannot generate hospital route if you haven't arrived at the emergency!", "Cannot Generate Route", wxOK | wxICON_ERROR, parentFrame);
                }
            }
            else
            {
                wxMessageBox("No current emergency assigned!", "No Current Emergency", wxOK | wxICON_ERROR, parentFrame);
            }
        }
    }
}

void EmergencyResponder::CompleteEmergency(wxWindow &parent)
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        if (parentFrame->customPanels)
        {
            if (!currentEmergency <= 0)
            {
                if (arrivedAtEmergency)
                {
                    if (hospitalRoute)
                    {
                        // Get hospital information
                        SQLite::Statement query(*parentFrame->customPanels->GetDatabase().database, "SELECT * FROM hospital WHERE hospitalNumber = ?");
                        query.bind(1, hospitalRouteID);

                        while (query.executeStep())
                        {
                            parentFrame->customPanels->GetDatabase().UpdateRecord("ambulance", {"location", "available", "active_emergency"}, {query.getColumn(2), "1", "0"}, {"unitNumber = '" + to_string(unitNumber) + "'"});
                        }

                        parentFrame->customPanels->GetMap().ClearEdges();
                    }
                    {
                        parentFrame->customPanels->GetMap().ClearEdges();
                        parentFrame->customPanels->GetDatabase().UpdateRecord("ambulance", {"available", "active_emergency"}, {"1", "0"}, {"unitNumber = '" + to_string(unitNumber) + "'"});
                    }

                    parentFrame->customPanels->GetDatabase().UpdateRecord("emergencies", {"complete"}, {"1"}, {"emergencyID = '" + to_string(currentEmergency) + "'"});
                    currentEmergency = -1;
                    hospitalRoute = false;
                }
                else
                {
                    wxMessageBox("Cannot generate complete emergency if you haven't arrived at the emergency!", "Cannot Complete Emergency", wxOK | wxICON_ERROR, parentFrame);
                }
            }
            else
            {
                wxMessageBox("No current emergency assigned!", "No Current Emergency", wxOK | wxICON_ERROR, parentFrame);
            }
        }
    }
}

void EmergencyResponder::CheckForCurrentEmergency(wxWindow &parent)
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        parentFrame->customPanels->GetMap().OnDatabaseChange();
        if (parentFrame->customPanels)
        {
            if (currentEmergency <= 0)
            {
                // Perform a Search through the Database to get ambulance emergency details
                SQLite::Statement query(*parentFrame->customPanels->GetDatabase().database, "SELECT * FROM ambulance WHERE unitNumber = ?");
                query.bind(1, unitNumber);

                while (query.executeStep())
                {
                    currentEmergency = query.getColumn(4).getInt();
                }
            }
        }
    }
}

void EmergencyResponder::ArriveAtEmergency(wxWindow &parent)
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        parentFrame->customPanels->GetMap().OnDatabaseChange();
        if (parentFrame->customPanels)
        {
            if (!currentEmergency <= 0)
            {
                // Perform a Search through the Database to get ambulance emergency details
                SQLite::Statement query(*parentFrame->customPanels->GetDatabase().database, "SELECT * FROM emergencies WHERE emergencyID = ?");
                query.bind(1, currentEmergency);

                parentFrame->customPanels->GetMap().ClearEdges();

                while (query.executeStep())
                {
                    parentFrame->customPanels->GetDatabase().UpdateRecord("ambulance", {"location"}, {query.getColumn(1)}, {"unitNumber = '" + to_string(unitNumber) + "'"});
                }

                arrivedAtEmergency = true;
            }
            else
            {
                wxMessageBox("No current emergency assigned!", "No Current Emergency", wxOK | wxICON_ERROR, parentFrame);
            }
        }
    }
}
