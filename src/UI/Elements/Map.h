#pragma once
#include <wx/wx.h>
#include "../../Data Structures/Graph/Graph.h"
#include "../../Types/Nodes/Emergency.h"
#include "../../Types/Nodes/Ambulance.h"
#include "../../Server/Database.h"
#include <any>
#include <typeinfo>

class Map : public wxPanel
{
public:
    Map(wxWindow *parent, Graph &graph) : wxPanel(parent), graph(graph){};

    void OnPaint(wxPaintEvent &event);

    void SetupGraph();

private:
    Graph &graph;

    void DrawGraph(wxDC &dc);

    void DrawNode(wxDC &dc, const Ambulance &nodeRef);
    void DrawNode(wxDC &dc, const Emergency &nodeRef);

    DECLARE_EVENT_TABLE();
};