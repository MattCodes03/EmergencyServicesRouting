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

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <wx/wx.h>
#include "../Elements/UserInput.h"

class LoginDialog : public wxDialog
{
public:
    LoginDialog(wxWindow *parent, wxWindowID id,
                const wxString &title,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = wxDEFAULT_DIALOG_STYLE,
                const wxString &name = wxASCII_STR(wxDialogNameStr));

    virtual ~LoginDialog();

    wxString getUsername() const { return username; }
    wxString getPassword() const { return password; }

    UserInput usernameInput;
    UserInput passwordInput;

private:
    wxString username = "Username";
    wxString password = "Password";

    virtual bool TransferDataFromWindow() override;

    void onUpdateOKButton(wxUpdateUIEvent &event);

    void onCancelButtonClicked(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};

#endif