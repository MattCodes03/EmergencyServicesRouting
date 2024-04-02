#include "HospitalSelectDialog.h"
#include <stdexcept>
#include <string>
#include <cstdint>

BEGIN_EVENT_TABLE(HospitalSelectDialog, wxDialog)
EVT_UPDATE_UI(wxID_OK, HospitalSelectDialog::onUpdateOKButton)
EVT_BUTTON(wxID_CANCEL, HospitalSelectDialog::onCancelButtonClicked)
END_EVENT_TABLE()

HospitalSelectDialog::HospitalSelectDialog(wxWindow *parent, wxWindowID id,
                                           const wxString &title,
                                           const wxPoint &pos,
                                           const wxSize &size,
                                           long style,
                                           const wxString &name) : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Initialize the hospitalChoice dropdown list
    hospitalChoice = new wxChoice(this, wxID_ANY);
    mainSizer->Add(hospitalChoice, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    PopulateHospitalList();

    // Hospital Pin Number Input
    hospitalPinInput.initialize(this, _("Pin"));
    mainSizer->Add(hospitalPinInput.inputSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Buttons
    wxSizer *buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 100));
    Fit();
}

// UI Event Handler to disable the Login/Ok Button until user has entered required data.
void HospitalSelectDialog::onUpdateOKButton(wxUpdateUIEvent &event)
{
    event.Enable(false);
    if (!hospitalPinInput.GetValue().empty())
    {
        event.Enable(hospitalChoice->GetSelection() != wxNOT_FOUND);
    }
}

void HospitalSelectDialog::onCancelButtonClicked(wxCommandEvent &event)
{
    Destroy();
    Close();
}

void HospitalSelectDialog::PopulateHospitalList()
{
    // Get reference to Database
    std::unique_ptr<Database> db = std::make_unique<Database>();

    vector<Hospital> hospitals = db->GetHospitals();

    // Populate the dropdown list with hospital names
    for (const auto &hospital : hospitals)
    {
        hospitalChoice->Append(hospital.name); // Append hospital name
        // Associate hospital ID with the corresponding index in the dropdown list
        hospitalChoice->SetClientData(hospitalChoice->GetCount() - 1, reinterpret_cast<void *>(hospital.hospitalNumber));
    }
}

bool HospitalSelectDialog::TransferDataFromWindow()
{
    int selectedIndex = hospitalChoice->GetSelection();
    if (selectedIndex != wxNOT_FOUND)
    {
        // Retrieve the associated hospital ID
        hospitalNumber = reinterpret_cast<intptr_t>(hospitalChoice->GetClientData(selectedIndex));
        std::string value = hospitalPinInput.GetValue().ToStdString();
        hospitalPin = stoi(value);
    }
    return true;
}

HospitalSelectDialog::~HospitalSelectDialog()
{
    delete hospitalChoice;
}
