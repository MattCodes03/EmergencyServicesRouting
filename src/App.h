#ifndef APP_H
#define APP_H

#include <wx/wx.h>

class App : public wxApp
{
public:
	static App &GetInstance();

	bool OnInit();
	void Restart();
};

#endif