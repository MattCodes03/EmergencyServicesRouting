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

#include "CallHandler.h"
#include "../../UI/Dialogs/EmergencyDialog.h"
#include "../../Data Structures/Utils.h"
#include "../../Data Structures/Priority Queue/Queue.h"
#include "../../MainFrame.h"
#include <chrono>
#include <thread>

// Forward Declartion of the priotrity queue class template with Emergency type
template class Queue<Emergency>;

/*
Function will allow user to accept an emergency and will subsequently call the prioritise emergency function
after displaying emergency details in a popup modal.
*/
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

            std::vector<Emergency> emergencies = parentFrame->customPanels->GetDatabase().GetUnRespondedEmergencies();

            if (!emergencies.empty())
            {

                // Update all emergency details
                Emergency activeEmergency = GetRandom(emergencies);

                emergency.SetEmergency(activeEmergency);
                emergency.SetDescription(activeEmergency.description);
                // Show emergency
                emergency.ShowModal();

                parentFrame->customPanels->GetDatabase().UpdateRecord("emergencies", {"respondedTo"}, {"1"}, "emergencyID='" + to_string(activeEmergency.emergencyNumber) + "'");
            }
            else
            {
                wxMessageBox("There are no incoming emergencies.", "Information", wxOK | wxICON_INFORMATION, &parent);
            }
        }
    }
}

// Function will allow user to assign a priority to an emregency based on the button they clicked on the popup modal, this will then add the emergency to the prioirty queue
void CallHandler::PrioritiseEmergency(wxCommandEvent &event, wxWindow &parent, Emergency emergency, int emergencyPriority)
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        if (parentFrame->customPanels)
        {

            emergency.priority = emergencyPriority;
            // Set Queue to a Max-Heap
            parentFrame->customPanels->GetMap().GetGraph().queue.SetQueueType("MAX");
            parentFrame->customPanels->GetMap().GetGraph().queue.EnQueue(emergency);
        }
    }
}

// Function is called by Consumer Thread every 5 seconds and will attempt to perform Dijkstras algorithm on emergencies within the queue if there are any
void CallHandler::RouteEmergency(wxWindow &parent) const
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        if (parentFrame->customPanels)
        {
            Graph &graph = parentFrame->customPanels->GetMap().GetGraph();

            if (!graph.queue.GetQueue().empty())
            {
                // Lock the mutex for thread safety
                graph.queue.GetMutex().lock();

                // Perform Dijkstra routing on the first element from the queue as source node
                int source = graph.queue.GetQueue()[0].emergencyNumber;
                std::pair<int, int> edge = graph.Dijkstra(source);

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
                    parentFrame->customPanels->GetDatabase().UpdateRecord("ambulance", {"available", "active_emergency"}, {"0", to_string(source.emergencyNumber)}, "unitNumber = '" + to_string(destination.unitNumber) + "'");

                    // DeQueue the routed emergency from priotity queue
                    graph.queue.DeQueue();
                }

                // Unlock the mutex
                graph.queue.GetMutex().unlock();
            }
        }
    }
}
