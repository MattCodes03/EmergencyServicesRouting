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
    int hospitalPin;
    wxChoice *hospitalChoice;
    UserInput hospitalPinInput;

    void PopulateHospitalList();

    virtual bool TransferDataFromWindow() override;

    void onUpdateOKButton(wxUpdateUIEvent &event);

    void onCancelButtonClicked(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};

#endif