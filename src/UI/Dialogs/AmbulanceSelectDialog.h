#ifndef AMBULANCESELECTDIALOG_H
#define AMBULANCESELECTDIALOG_H

#include <wx/wx.h>
#include "../Elements/UserInput.h"

class AmbulanceSelectDialog : public wxDialog
{
public:
    AmbulanceSelectDialog(wxWindow *parent, wxWindowID id,
                          const wxString &title,
                          const wxPoint &pos = wxDefaultPosition,
                          const wxSize &size = wxDefaultSize,
                          long style = wxDEFAULT_DIALOG_STYLE,
                          const wxString &name = wxASCII_STR(wxDialogNameStr));

    virtual ~AmbulanceSelectDialog();

    int getUnitNumber() const { return unitNumber; }

    UserInput unitNumberInput;

private:
    int unitNumber = 0;

    virtual bool TransferDataFromWindow() override;

    void onUpdateOKButton(wxUpdateUIEvent &event);

    void onCancelButtonClicked(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};

#endif