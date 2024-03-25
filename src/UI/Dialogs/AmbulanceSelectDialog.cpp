#include "AmbulanceSelectDialog.h"
#include <stdexcept>
#include <string>

BEGIN_EVENT_TABLE(AmbulanceSelectDialog, wxDialog)
EVT_UPDATE_UI(wxID_OK, AmbulanceSelectDialog::onUpdateOKButton)
EVT_BUTTON(wxID_CANCEL, AmbulanceSelectDialog::onCancelButtonClicked)
END_EVENT_TABLE()

AmbulanceSelectDialog::AmbulanceSelectDialog(wxWindow *parent, wxWindowID id,
                                             const wxString &title,
                                             const wxPoint &pos,
                                             const wxSize &size,
                                             long style,
                                             const wxString &name) : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Username Input
    unitNumberInput.initialize(this, _("Unit Number"));

    mainSizer->Add(unitNumberInput.inputSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Buttons
    wxSizer *buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 100));
    Fit();
}

// UI Event Handler to disable the Login/Ok Button until user has entered required data.
void AmbulanceSelectDialog::onUpdateOKButton(wxUpdateUIEvent &event)
{
    event.Enable(false);
    if (!unitNumberInput.GetValue().empty())
    {
        event.Enable(true);
    }
}

void AmbulanceSelectDialog::onCancelButtonClicked(wxCommandEvent &event)
{
    Destroy();
    Close();
}

bool AmbulanceSelectDialog::TransferDataFromWindow()
{
    std::string value = unitNumberInput.GetValue().ToStdString();
    unitNumber = stoi(value);
    return true;
}

AmbulanceSelectDialog::~AmbulanceSelectDialog()
{
}
