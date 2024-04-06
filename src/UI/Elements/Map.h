/*
Copyright (c) 2024, Matthew McCann
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to no conditions.
*/

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

    /*
    Function to add Edges to the Map, this is not related to Graph it is for drawing the edges between nodes
    @param source - Coordinates of source node
    @param destination - Coordinates of destination node
    */
    void AddEdge(pair<int, int> source, pair<int, int> destination)
    {
        edges.push_back({source, destination});
    }

    // Clear all edges, not related to Graph used for removing drawn edges from the map
    void ClearEdges()
    {
        edges.clear();
    }

    // Function to Set Map Type
    void SetMapType(std::string mapType)
    {
        this->mapType = mapType;
    }

    void ClearGraph()
    {
        graph.ClearGraph();
    }

    // Pair of Pairs Type, used to store Edge -> <(X1, X2), (Y1, Y2)>
    using PairOfPairs = std::pair<std::pair<int, int>, std::pair<int, int>>;
    std::vector<PairOfPairs> edges;

private:
    std::string mapType;
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
    int partition(vector<PairOfPairs> &edges, int low, int high)
    {
        // Randomly choose pivot index within the range of low to high
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> distribution(low, high);
        int pivotIndex = distribution(gen);

        // Swap pivot element with the last element
        swap(edges[pivotIndex], edges[high]);
        PairOfPairs pivot = edges[high];

        int i = low - 1;
        for (int j = low; j < high; ++j)
        {
            if (edges[j].first.first < pivot.first.first)
            {
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
