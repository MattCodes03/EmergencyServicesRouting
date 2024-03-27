#include "MainFrame.h"
#include "Data Structures/Graph/Graph.h"

using namespace std;

MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title)
{
	wxInitAllImageHandlers();
}

void MainFrame::Initialize()
{
	customPanels = new CustomPanels(this);
	customPanels->InitialiseUser(view, *activeUser);

	if (view == "HANDLER")
	{
		CallHandler callHandler;
		// Set the stop routing callback
		customPanels->SetStopRoutingCallback([&callHandler]()
											{ callHandler.StopRoutingThread(); });

		customPanels->CallHandlerPanel(this);
	}

	if (view == "RESPONDER")
	{
		customPanels->EmergencyResponderPanel(this);
	}

	if (view == "HOSPITAL")
	{
		customPanels->HospitalAdminPanel(this);
	}
}
