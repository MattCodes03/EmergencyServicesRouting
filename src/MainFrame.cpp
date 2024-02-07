#include "MainFrame.h"
#include "Data Structures/Graph.h"
#include "UI/Panels/CustomPanels.h"

using namespace std;

MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title)
{
wxInitAllImageHandlers();
}

void MainFrame::Initialize()
{
	CustomPanels customPanels;
	customPanels.InitialiseUser(view, *activeUser);

	if (view == "HANDLER")
	{
		customPanels.CallHandlerPanel(this);
	}

	if (view == "RESPONDER")
	{
		customPanels.EmergencyResponderPanel(this);
	}

	if (view == "HOSPITAL")
	{
		customPanels.HospitalAdminPanel(this);
	}
}
