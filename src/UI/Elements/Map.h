#ifndef MAP_H
#define MAP_H

#include <wx/wx.h>
#include "../../Data Structures/Graph/Graph.h"
#include "../../Types/Nodes/Emergency.h"
#include "../../Types/Nodes/Ambulance.h"
#include "../../Server/Database.h"
#include <any>
#include <typeinfo>

class Map : public wxPanel, public DatabaseListener
{
public:
    Map() : graph(50, 47){};
    Map(wxWindow *parent) : wxPanel(parent), graph(50, 47){};

    void OnDatabaseChange() override;

    void OnPaint(wxPaintEvent &event);

    void SetupGraph();

    Graph &GetGraph() { return this->graph; };

    // Copy assignment operator
    Map &operator=(const Map &other)
    {
        if (this != &other)
        {
            this->graph = other.graph;
        }
        return *this;
    }

private:
    Graph graph;
    void DrawGraph(wxDC &dc);

    void DrawNode(wxDC &dc, const Ambulance &nodeRef);
    void DrawNode(wxDC &dc, const Emergency &nodeRef);

    DECLARE_EVENT_TABLE();
};

#endif