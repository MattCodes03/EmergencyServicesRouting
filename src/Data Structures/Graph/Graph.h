#pragma once
#include <vector>
#include "Node.h"

class Graph
{
public:
    Graph(int numNodes) : adjacencyList(numNodes) {}

    /*
    Function will add edge to the graph.
    @param  source  Source Nodes ID
    @param  destination Destinations Nodes ID
    @param  weight  Weight of this edge
     */
    void AddEdge(int source, int destination, int weight);

private:
    vector<vector<Node>> adjacencyList;
};
