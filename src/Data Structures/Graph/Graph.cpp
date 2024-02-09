#include "Graph.h"

void Graph::AddEdge(int source, int destination, int weight)
{
    Node n1(destination, weight);
    Node n2(source, weight);

    adjacencyList[source].push_back(n1);
    adjacencyList[destination].push_back(n2);
};
