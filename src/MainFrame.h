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

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include "Types/User/User.h"
#include <string>
#include <memory>
#include "UI/Panels/CustomPanels.h"

// Forward Declartion for member value
class CustomPanels;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title);

	~MainFrame()
	{
		delete customPanels; // Ensure proper cleanup
	}

	void Initialize();

	void SetViewType(string v) { view = v; }

	void SetActiveUser(const User &user)
	{
		activeUser = make_unique<User>(user.getUsername());
	}

	unique_ptr<User> activeUser;

	CustomPanels *customPanels;

private:
	std::string view;
};

#endif