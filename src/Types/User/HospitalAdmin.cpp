#include "HospitalAdmin.h"
#include "../../MainFrame.h"

HospitalAdmin::HospitalAdmin(const string &username) : User(username)
{
}

void HospitalAdmin::UpdateHospitalStatus(wxWindow &parent)
{
    int updatedStatus = (activeHospital.status == 0) ? 1 : 0;

    MainFrame *parentFrame = dynamic_cast<MainFrame *>(&parent);
    if (parentFrame)
    {
        activeHospital.status = !activeHospital.status;
        parentFrame->customPanels->GetDatabase().UpdateRecord("hospital", {"status"}, {to_string(updatedStatus)}, "hospitalNumber = '" + to_string(hospitalNumber) + "'");
    }
}
