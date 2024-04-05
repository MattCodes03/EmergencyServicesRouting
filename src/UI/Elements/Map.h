#ifndef MAP_H
#define MAP_H

#include <wx/wx.h>
#include "../../Data Structures/Graph/Graph.h"
#include "../../Types/Nodes/Emergency.h"
#include "../../Types/Nodes/Ambulance.h"
#include "../../Server/Database.h"
#include <any>
#include <typeinfo>
#include <random>

class Map : public wxPanel, public DatabaseListener
{
public:
    Map() : graph(50, 47){};
    Map(wxWindow *parent, wxPanel *parentPanel) : wxPanel(parent), graph(50, 47), parent(parent), parentPanel(parentPanel){};

    void OnDatabaseChange() override;

    void OnPaint(wxPaintEvent &event);

    void DrawBackground();

    void SetupGraph();

    Graph &GetGraph() { return this->graph; };

    // Copy assignment operator
    Map &operator=(const Map &other)
    {
        if (this != &other)
        {
            this->mapType = other.mapType;
            this->graph = other.graph;
        }
        return *this;
    }

    void AddEdge(pair<int, int> source, pair<int, int> destination)
    {
        edges.push_back({source, destination});
    }

    void ClearEdges()
    {
        edges.clear();
    }

    void SetMapType(string mapType)
    {
        this->mapType = mapType;
    }

    void ClearGraph()
    {
        graph.ClearGraph();
    }

    using PairOfPairs = pair<pair<int, int>, pair<int, int>>;
    vector<PairOfPairs> edges;

private:
    string mapType;
    Graph graph;

    wxDC *dc;
    wxWindow *parent;
    wxPanel *parentPanel;

    void DrawGraph();

    template <typename NodeType>
    void DrawNode(const NodeType &nodeRef);

    // QuickSort function to sort edges based on source node coordinates
    void QuickSortEdges(std::vector<PairOfPairs> &edges, int low, int high)
    {
        if (low < high)
        {
            int pivot = partition(edges, low, high);
            QuickSortEdges(edges, low, pivot - 1);
            QuickSortEdges(edges, pivot + 1, high);
        }
    }

    // Function to partition the array and return the index of the pivot element
int partition(vector<PairOfPairs> &edges, int low, int high) {
    // Randomly choose pivot index within the range [low, high]
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(low, high);
    int pivotIndex = distribution(gen);

    // Swap pivot element with the last element
    swap(edges[pivotIndex], edges[high]);
    PairOfPairs pivot = edges[high];

    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (edges[j].first.first < pivot.first.first) {
            ++i;
            swap(edges[i], edges[j]);
        }
    }
    swap(edges[i + 1], edges[high]);
    return i + 1;
}

    DECLARE_EVENT_TABLE();
};

#endif
