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

    pair<int, int> dest = parentFrame->customPanels.GetMap().GetGraph().Dijkstra(parentFrame->customPanels.GetMap().GetGraph().queue.GetQueue()[0].emergencyNumber);
    parentFrame->customPanels.GetMap().GetGraph().queue.DeQueue();

    cout << "Source Node: " << dest.first << " - Destination Node: " << dest.second << endl;
}

void CallHandler::RouteEmergency()
{
}
