#pragma once
#include <wx/wx.h>
#include <wx/bitmap.h>
#include <wx/image.h>

class Map : public wxPanel
{
public:
    Map(wxWindow *parent) : wxPanel(parent){};

    void OnPaint(wxPaintEvent &event);

private:
    void DrawGrid(wxDC &dc);

    DECLARE_EVENT_TABLE();
};