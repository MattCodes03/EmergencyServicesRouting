#pragma once
#include <wx/wx.h>
#include "Types/User.h"
#include <string>
#include <memory>

using namespace std;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title);

	void Initialize();

	void SetViewType(string v) { view = v; }

	void SetActiveUser(const User& user)
	{
		activeUser = make_unique<User>(user.getUsername());
	}

	unique_ptr<User> activeUser;

private:
	string view;
};
