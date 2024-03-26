
#include "CallHandler.h"
#include "../../UI/Dialogs/EmergencyDialog.h"
#include "../../Data Structures/Utils.h"
#include "../../Data Structures/Priority Queue/Queue.h"
#include "../../MainFrame.h"
#include <chrono>
#include <thread>

template class Queue<Emergency>;

void CallHandler::CheckAndRouteLoop(wxWindow &parent) const
{

    while (keepRunning)
    {
        // Route emergencies
        RouteEmergency(parent);

        this_thread::sleep_for(chrono::seconds(5));
    }
}

void CallHandler::AcceptEmergency(wxCommandEvent &event, wxWindow &parent) const
{
    EmergencyDialog emergency(&parent, wxID_ANY, _("Emergency"));
    emergency.SetUserRef(*this);

    // Get an emergency
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        if (parentFrame->customPanels)
        {

            vector<Emergency> emergencies = parentFrame->customPanels->GetDatabase().GetUnRespondedEmergencies();

            if (!emergencies.empty())
            {

                // Update all emergency details
                Emergency activeEmergency = GetRandom(emergencies);

                emergency.SetEmergency(activeEmergency);
                emergency.SetDescription(activeEmergency.description);
                // Show emergency
                emergency.ShowModal();
            }
            else
            {
                wxMessageBox("There are no incoming emergencies.", "Information", wxOK | wxICON_INFORMATION, &parent);
            }
        }
    }
}

void CallHandler::PrioritiseEmergency(wxCommandEvent &event, wxWindow &parent, Emergency emergency, int emergencyPriority)
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        if (parentFrame->customPanels)
        {

            emergency.priority = emergencyPriority;
            // Update Database and set Emergency as RespondedTo
            parentFrame->customPanels->GetDatabase().UpdateRecord("emergencies", {"respondedTo"}, {"1"}, "emergencyID='" + to_string(emergency.emergencyNumber) + "'");

            parentFrame->customPanels->GetMap().GetGraph().queue.EnQueue(emergency);
        }
    }
}

void CallHandler::RouteEmergency(wxWindow &parent) const
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        if (parentFrame->customPanels)
        {

            auto graph = parentFrame->customPanels->GetMap().GetGraph();
            graph.queue.Display();

            if (!graph.queue.GetQueue().empty())
            {
                lock_guard<mutex> lock(graph.queue.GetMutex());

                int source = graph.queue.GetQueue()[0].emergencyNumber;
                pair<int, int> edge = graph.Dijkstra(source);

                if (edge.second != -1)
                {
                    auto sourceNode = graph.GetNodes()[edge.first];
                    auto destinationNode = graph.GetNodes()[edge.second];

                    // Get nodes
                    Emergency source = any_cast<Emergency>(sourceNode.GetData());
                    Ambulance destination = any_cast<Ambulance>(destinationNode.GetData());

                    // Draw the edge on the map between emergency and assigned ambulance
                    parentFrame->customPanels->GetMap().AddEdge(source.location, destination.location);

                    // Update the database and assign this emergency to the ambulance and set ambulance availability to false so it doesn't get assigned multiple calls at once
                    parentFrame->customPanels->GetDatabase().UpdateRecord("ambulance", {"available", "active_emergency"}, {"0", "" + source.emergencyNumber}, "unitNumber = '" + to_string(destination.unitNumber) + "'");

                    graph.queue.DeQueue();
                    graph.queue.Display();
                }
                else
                {
                    std::cerr << "Error: Unable to find a route for emergency\n";
                }
            }
            else
            {
                std::cerr << "Error: Empty queue\n";
            }
        }
        else
        {
            std::cerr << "Error: Invalid parent frame\n";
        }
    }
}
