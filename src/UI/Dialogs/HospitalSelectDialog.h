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

#ifndef HOSPITALSELECTDIALOG
#define HOSPITALSELECTDIALOG_H

#include <wx/wx.h>
#include "../../MainFrame.h"
#include "../Elements/UserInput.h"

class HospitalSelectDialog : public wxDialog
{
public:
    HospitalSelectDialog(wxWindow *parent, wxWindowID id,
                         const wxString &title,
                         const wxPoint &pos = wxDefaultPosition,
                         const wxSize &size = wxDefaultSize,
                         long style = wxDEFAULT_DIALOG_STYLE,
                         const wxString &name = wxASCII_STR(wxDialogNameStr));

    virtual ~HospitalSelectDialog();

    int getHopsitalNumber() const { return hospitalNumber; }
    int getHospitalPin() const { return hospitalPin; }

private:
    int hospitalNumber = 0;
    int hospitalPin = 0;
    wxChoice *hospitalChoice;
    UserInput hospitalPinInput;

    void PopulateHospitalList();

    virtual bool TransferDataFromWindow() override;

    void onUpdateOKButton(wxUpdateUIEvent &event);

    void onCancelButtonClicked(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};

#endif