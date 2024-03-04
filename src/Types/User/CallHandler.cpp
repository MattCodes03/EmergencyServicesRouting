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

    map.GetGraph().queue.Display();

    Emergency emergency1(1, {10, 10}, 1, "test", false, false);
    map.GetGraph().queue.EnQueue(emergency1);

    map.GetGraph().queue.EnQueue(emergency);
    cout << "emergency1 < emergency2: " << (emergency < emergency1) << endl;
    cout << "Size of Queue - " << map.GetGraph().queue.GetQueue().size() << endl;
    ;

    // Display Priority Queue for Debug Purposes
    map.GetGraph().queue.Display();
}

void CallHandler::SendMessage()
{
}
