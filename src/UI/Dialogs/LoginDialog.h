#pragma once
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

    DECLARE_EVENT_TABLE()
};