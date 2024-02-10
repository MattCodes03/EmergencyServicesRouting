#include "Graph.h"
#include <iostream>

void Graph::AddEdge(int source, int destination, int weight)
{
    adjacencyList[source].emplace_back(destination, weight);
    adjacencyList[destination].emplace_back(source, weight);
}
void Graph::AddNode(const Node &node)
{
    nodes.emplace_back(node);
    adjacencyList.emplace_back();
};

void Graph::Display()
{
    for (int i = 0; i < adjacencyList.size(); ++i)
    {
        std::cout << "Node " << nodes[i].GetID() << " --> ";
        for (const auto &neighbor : adjacencyList[i])
        {
            std::cout << "(Node " << nodes[neighbor.first].GetID() << ", " << neighbor.second << ") ";
        }
        std::cout << std::endl;
    }
}
