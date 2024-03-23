#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Node.h"
#include "../Priority Queue/Queue.h"
#include "../../Types/Nodes/Emergency.h"
#include <iostream>
#include <cmath>

using namespace std;

class Graph
{
public:
    Graph(int numNodes, const any &defaultData) : adjacencyList(numNodes), nodes(numNodes), queue()
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

    void AddEdge(int source, int destination, int weight)
    {
        adjacencyList[source].emplace_back(destination, weight);
        adjacencyList[destination].emplace_back(source, weight);
    }
    void AddNode(const Node &node)
    {
        nodes.emplace_back(node);
    };

    void ClearGraph()
    {
        this->nodes.clear();
        this->adjacencyList.clear();
    };

    // Euclidean Distance Formula
    int CalculateDistance(pair<int, int> location1, pair<int, int> location2)
    {
        return sqrt(pow(location1.first - location2.first, 2) + pow(location1.second - location2.second, 2));
    }

    void Display()
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

    // Int 1 is Source Node (Emergency or Ambulance), Int 2 is Closest Ambulance or Hospital found by the algorithm
    pair<int, int> Dijkstra(int source)
    {
        int n = adjacencyList.size();
        vector<int> distance(n, numeric_limits<int>::max());
        vector<int> parent(n, -1);
        vector<bool> visited(n, false);

        vector<pair<int, int>> pq;

        distance[source] = 0;
        pq.push_back({0, source});

        while (!pq.empty())
        {
            int u = -1;
            int minDistance = numeric_limits<int>::max();

            if (visited[u])
                continue;
            visited[u] = true;

            for (const auto &neighbor : adjacencyList[u])
            {
                int v = neighbor.first;
                int weight = neighbor.second;
                if (!visited[v] && distance[u] + weight < distance[v])
                {
                    distance[v] = distance[u] + weight;
                    parent[v] = u;
                    pq.push_back({distance[v], v});
                }
            }
        }

        // Find the closest ambulance or hospital from the source
        int destination = -1;
        int minDistance = numeric_limits<int>::max();
        for (int i = 0; i < n; ++i)
        {
            if (i != source && distance[i] < minDistance)
            {
                minDistance = distance[i];
                destination = i;
            }
        }

        return {source, destination};
    }

    vector<Node> GetNodes() const
    {
        return this->nodes;
    };
    vector<vector<pair<int, int>>> GetAdjacencyList() const { return this->adjacencyList; };

    Queue<Emergency> queue;

private:
    vector<vector<pair<int, int>>> adjacencyList;
    vector<Node> nodes;
};

#endif