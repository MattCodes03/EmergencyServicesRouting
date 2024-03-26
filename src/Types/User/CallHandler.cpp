
#include "CallHandler.h"
#include "../../UI/Dialogs/EmergencyDialog.h"
#include "../../Data Structures/Utils.h"
#include "../../Data Structures/Priority Queue/Queue.h"
#include "../../MainFrame.h"
#include <chrono>
#include <thread>


template class Queue<Emergency>;

void CallHandler::CheckAndRouteLoop(wxWindow &parent)
{

    while (keepRunning)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Adjust the interval as needed

        // Check and route emergencies
        CheckAndRouteEmergency(parent);
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

        vector<Emergency> emergencies = parentFrame->customPanels->GetDatabase().GetUnRespondedEmergencies();

        if (!emergencies.empty())
        {

            // Update all emergency details
            Emergency activeEmergency = GetRandom(emergencies);

            emergency.SetEmergency(activeEmergency);
            emergency.SetDescription(activeEmergency.description);
            // Show emergency
            emergency.ShowModal();

            /*
             Route Emergency Thread, this will create another thread that will periodically check if any emergencies are in the queue to be routed,
             this means all routing is done automatically
             */
            std::thread routingThread([this, &parent]()
                                      {
        // Use const_cast to remove the const qualifier temporarily
        const_cast<CallHandler*>(this)->CheckAndRouteLoop(parent); });
            routingThread.detach();
        }
        else
        {
            wxMessageBox("There are no incoming emergencies.", "Information", wxOK | wxICON_INFORMATION, &parent);
        }
    }
}

void CallHandler::PrioritiseEmergency(wxCommandEvent &event, wxWindow &parent, Emergency emergency, int emergencyPriority)
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {

        emergency.priority = emergencyPriority;
        parentFrame->customPanels->GetMap().GetGraph().queue.EnQueue(emergency);

        // Update Database and set Emergency as RespondedTo
        parentFrame->customPanels->GetDatabase().UpdateRecord("emergencies", {"respondedTo"}, {"1"}, "emergencyID='" + to_string(emergency.emergencyNumber) + "'");
    }
}

void CallHandler::CheckAndRouteEmergency(wxWindow &parent)
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);

    if (parentFrame && parentFrame->customPanels)
    {
        auto& queue = parentFrame->customPanels->GetMap().GetGraph().queue;
        
        if (!queue.GetQueue().empty())
        {
            try
            {
                // Perform the routing operation
                RouteEmergency(parent);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
    else
    {
        std::cerr << "Error: Invalid parent frame or custom panels\n";
    }
}

void CallHandler::RouteEmergency(wxWindow &parent) const
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
     if (parentFrame)
    {
        auto graph = parentFrame->customPanels->GetMap().GetGraph();
        auto queue = graph.queue;
        const auto &nodes = graph.GetNodes();

        if (!nodes.empty() && !queue.GetQueue().empty())
        {
            pair<int, int> edge = graph.Dijkstra(queue.GetQueue()[0].emergencyNumber);

            if (edge.second != -1)
            {
                const auto& sourceNode = nodes[edge.first];
                const auto& destinationNode = nodes[edge.second];

                try{
                    // Get nodes
                    Emergency source = any_cast<Emergency>(sourceNode.GetData());
                    Ambulance destination = any_cast<Ambulance>(destinationNode.GetData());

                    queue.DeQueue();

                    // Draw the edge on the map between emergency and assigned ambulance
                    parentFrame->customPanels->GetMap().AddEdge(source.location, destination.location);

                    // Update the database and assign this emergency to the ambulance and set ambulance availability to false so it doesn't get assigned multiple calls at once
                    parentFrame->customPanels->GetDatabase().UpdateRecord("ambulance", {"available", "active_emergency"}, {"0", "" + source.emergencyNumber}, "unitNumber = '" + to_string(destination.unitNumber) + "'");
                }
                catch(const std::bad_any_cast&){
                    std::cerr << "Error: Invalid source or destination node\n";
                }
            }
            else
            {
                std::cerr << "Error: Unable to find a route for emergency\n";
            }
        }
        else
        {
            std::cerr << "Error: Empty nodes or queue\n";
        }
    }
    else
    {
        std::cerr << "Error: Invalid parent frame\n";
    }
}
