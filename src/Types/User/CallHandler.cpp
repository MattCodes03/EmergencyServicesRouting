#include "CallHandler.h"
#include "../../UI/Dialogs/EmergencyDialog.h"
#include "../../Data Structures/Utils.h"

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
    unique_ptr<Database> database = make_unique<Database>();
    vector<Emergency> emergencies = database->GetUnRespondedEmergencies();
    // Update all emergency details
    Emergency activeEmergency = GetRandom(emergencies);

    emergency.SetEmergency(activeEmergency);
    emergency.SetDescription(activeEmergency.description);
    // Show emergency
    emergency.ShowModal();
    // Deal with prioritisation
}

void CallHandler::PrioritiseEmergency(wxCommandEvent &event, Emergency emergency, int emergencyPriority, const Map &map) const
{
    emergency.priority = emergencyPriority;
    map.GetGraph().queue.EnQueue(emergency);
}

void CallHandler::SendMessage()
{
}
