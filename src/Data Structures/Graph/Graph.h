#pragma once
#include <vector>
#include <any>
#include "Node.h"

using namespace std;

class Graph
{
public:
    Graph(int numNodes, const any &defaultData) : adjacencyList(numNodes), nodes(numNodes)
    {
        for (int i = 0; i < numNodes; ++i)
        {
            nodes[i] = Node(i, defaultData);
        }
    }

    /*
    Function will add edge to the graph.
    @param  source  Source Nodes ID
    @param  destination Destinations Nodes ID
    @param  weight  Weight of this edge
     */
    void AddEdge(int source, int destination, int weight);

    void AddNode(const Node &node);

    void Display();

    void ClearGraph();

    int CalculateDistance(pair<int, int> location1, pair<int, int> location2);

    vector<Node> GetNodes() const { return this->nodes; };
    vector<vector<pair<int, int>>> GetAdjacencyList() const { return this->adjacencyList; };

private:
    vector<vector<pair<int, int>>> adjacencyList;
    vector<Node> nodes;
};
