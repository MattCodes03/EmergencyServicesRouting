#include "LoginDialog.h"

BEGIN_EVENT_TABLE(LoginDialog, wxDialog)
EVT_UPDATE_UI(wxID_OK, LoginDialog::onUpdateOKButton)
EVT_BUTTON(wxID_CANCEL, LoginDialog::onCancelButtonClicked)
END_EVENT_TABLE()

LoginDialog::LoginDialog(wxWindow *parent, wxWindowID id,
                         const wxString &title,
                         const wxPoint &pos,
                         const wxSize &size,
                         long style,
                         const wxString &name) : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Username Input
    usernameInput.initialize(this, _("Username"));

    // Password Input
    passwordInput.initialize(this, _("Password"), wxTE_PASSWORD);

    mainSizer->Add(usernameInput.inputSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    mainSizer->Add(passwordInput.inputSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Buttons
    wxSizer *buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 100));
    Fit();
}

// UI Event Handler to disable the Login/Ok Button until user has entered required data.
void LoginDialog::onUpdateOKButton(wxUpdateUIEvent &event)
{
    event.Enable(false);
    if (!usernameInput.GetValue().empty() && !passwordInput.GetValue().empty())
    {
        event.Enable(true);
    }
}

void LoginDialog::onCancelButtonClicked(wxCommandEvent &event)
{
    Destroy();
    Close();
}

bool LoginDialog::TransferDataFromWindow()
{
    username = usernameInput.GetValue();
    password = passwordInput.GetValue();
    return true;
}

LoginDialog::~LoginDialog()
{
}
