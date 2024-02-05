#pragma once
#include <wx/wx.h>
#include <string>

using namespace std;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title);

	void Initialize();

	void SetViewType(string v) { view = v; }

private:
	string view;
};
