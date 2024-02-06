#pragma once
#include <wx/wx.h>
#include "Types/User.h"
#include <string>

using namespace std;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title);

	void Initialize();

	void SetViewType(string v) { view = v; }

	void SetActiveUser(User user)
	{
		delete activeUser;

		this->activeUser = new User(user.getUsername());
	}

	User *activeUser;

private:
	string view;
};
