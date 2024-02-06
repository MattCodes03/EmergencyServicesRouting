#pragma once
#include <wx/wx.h>

class Map : public wxPanel
{
public:
    Map(wxWindow *parent) : wxPanel(parent){};

    void OnPaint(wxPaintEvent &event);

private:
    void DrawGrid(wxDC &dc);

    DECLARE_EVENT_TABLE();
};