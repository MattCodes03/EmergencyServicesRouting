#include "CallHandler.h"
#include "../../UI/Dialogs/EmergencyDialog.h"
#include "../../Data Structures/Utils.h"
#include "../../Data Structures/Priority Queue/Queue.h"
#include "../../MainFrame.h"

template class Queue<Emergency>;

CallHandler::CallHandler(const string &username, const string &firstname, const string &lastname) : User(username)
{
    this->username = username;
    this->firstname = firstname;
    this->lastname = lastname;
}

void CallHandler::AcceptEmergency(wxCommandEvent &event, wxWindow &parent) const
{
    EmergencyDialog emergency(&parent, wxID_ANY, _("Emergency"));
    emergency.SetUserRef(*this);

    // Get an emergency
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    vector<Emergency> emergencies = parentFrame->customPanels.GetDatabase().GetUnRespondedEmergencies();
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

void CallHandler::PrioritiseEmergency(wxCommandEvent &event, wxWindow &parent, Emergency emergency, int emergencyPriority)
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);

    emergency.priority = emergencyPriority;
    parentFrame->customPanels.GetMap().GetGraph().queue.EnQueue(emergency);

    // Update Database and set Emergency as RespondedTo
    parentFrame->customPanels.GetDatabase().UpdateRecord("emergencies", {"respondedTo"}, {"1"}, "emergencyID='" + to_string(emergency.emergencyNumber) + "'");
}

void CallHandler::RouteEmergency(wxCommandEvent &event, wxWindow &parent) const
{
    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);

    pair<int, int> edge = parentFrame->customPanels.GetMap().GetGraph().Dijkstra(parentFrame->customPanels.GetMap().GetGraph().queue.GetQueue()[0].emergencyNumber);
    parentFrame->customPanels.GetMap().GetGraph().queue.DeQueue();

    const std::type_info &typeInfo = parentFrame->customPanels.GetMap().GetGraph().GetNodes()[3].GetData().type();

    try
    {
        // Get nodes;
        Emergency source = any_cast<Emergency>(parentFrame->customPanels.GetMap().GetGraph().GetNodes()[edge.first].GetData());

        Ambulance destination = any_cast<Ambulance>(parentFrame->customPanels.GetMap().GetGraph().GetNodes()[edge.second].GetData());

        // Update the database and assign this emergency to the ambulance and set ambulance availability to false so it doesn't get assigned multiple calls at once

        // Draw the edge on the map between emergency and assigned ambulance
        parentFrame->customPanels.GetMap().DrawEdge(source.location, destination.location);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
