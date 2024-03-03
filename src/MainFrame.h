#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include "Types/User/User.h"
#include <string>
#include <memory>
#include "UI/Panels/CustomPanels.h"

using namespace std;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title);

	void Initialize();

	void SetViewType(string v) { view = v; }

	void SetActiveUser(const User &user)
	{
		activeUser = make_unique<User>(user.getUsername());
	}

	unique_ptr<User> activeUser;

	CustomPanels customPanels;

private:
	string view;
};

#endif