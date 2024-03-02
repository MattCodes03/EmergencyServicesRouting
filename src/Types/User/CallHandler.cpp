#include "CallHandler.h"
#include "../../UI/Dialogs/EmergencyDialog.h"
#include "../../Server/Utils.h"

CallHandler::CallHandler(const string &username, const string &firstname, const string &lastname) : User(username)
{
    this->username = username;
    this->firstname = firstname;
    this->lastname = lastname;
}

void CallHandler::AcceptEmergency(wxCommandEvent &event, wxWindow &parent) const
{
    EmergencyDialog emergency(&parent, wxID_ANY, _("Emergency"));

    // Get an emergency
    unique_ptr<Database> database = make_unique<Database>();
    vector<Emergency> emergencies = database->GetUnRespondedEmergencies();
    // Update all emergency details
    Emergency activeEmergency = GetRandom(emergencies);

    emergency.SetEmergencyID(activeEmergency.emergencyNumber);
    emergency.SetDescription(activeEmergency.description);
    // Show emergency
    emergency.ShowModal();
    // Deal with prioritisation
}

void CallHandler::PrioritiseEmergency()
{
}

void CallHandler::SendMessage()
{
}
