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

#include "HospitalAdmin.h"
#include "../../MainFrame.h"

HospitalAdmin::HospitalAdmin(const std::string &username) : User(username)
{
}

// Function will toggle hospitals available status in the database
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
