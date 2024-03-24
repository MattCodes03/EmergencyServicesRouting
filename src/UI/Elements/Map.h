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

    void AddEdge(pair<int, int> source, pair<int, int> destination)
    {
        edges.push_back({source, destination});
    }

private:
    Graph graph;

    using PairOfPairs = std::pair<std::pair<int, int>, std::pair<int, int>>;
    vector<PairOfPairs> edges;

    wxDC *dc;

    void DrawGraph();
    void DrawNode(const Ambulance &nodeRef);
    void DrawNode(const Emergency &nodeRef);

    DECLARE_EVENT_TABLE();
};

#endif