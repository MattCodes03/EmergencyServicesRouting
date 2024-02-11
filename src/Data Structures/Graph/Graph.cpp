#include "Graph.h"
#include <iostream>
#include <cmath>

void Graph::AddEdge(int source, int destination, int weight)
{
    adjacencyList[source].emplace_back(destination, weight);
    adjacencyList[destination].emplace_back(source, weight);
}
void Graph::AddNode(const Node &node)
{
    nodes.emplace_back(node);
};

// Euclidean Distance Formula
int Graph::CalculateDistance(pair<int, int> location1, pair<int, int> location2)
{
    return sqrt(pow(location1.first - location2.first, 2) + pow(location1.second - location2.second, 2));
}

void Graph::ClearGraph()
{
    this->nodes.clear();
    this->adjacencyList.clear();
}

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
