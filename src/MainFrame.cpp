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

#include "MainFrame.h"
#include "Data Structures/Graph/Graph.h"

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
